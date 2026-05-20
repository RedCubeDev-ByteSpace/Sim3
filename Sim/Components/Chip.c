//
// Created by redcube on 13/05/2026.
//

#include "Chip.h"
#include <lualib.h>
#include <lauxlib.h>

#include <stdlib.h>

#include "Connection.h"

void SIM_CHIP_init(sim_chip_t *me, Vector2 position, sim_chip_specification_t *chipSpec) {
    me->chipSpec = chipSpec;
    me->prevClockPinWireState = false;

    // create the chip drawable for this component
    me->chip = malloc(sizeof(drw_chip_t));
    DRAWABLES_CHIP_init(me->chip, position, chipSpec->name, chipSpec->function, chipSpec->numPins / 2);
    DRAWABLES_enqueue((drawable_t*)me->chip);

    // create all connection points
    SIM_COMP_LIST_init(&me->connectionPoints);

    // bottom row
    for (int i = 0; i < chipSpec->numPins / 2; ++i) {
        sim_connection_point_t *conPoint = malloc(sizeof(sim_connection_point_t));
        SIM_CONNECTION_POINT_init(conPoint, CONNECTION_POINT_FLOATING, (Vector2){ position.x + i, position.y + 4 });
        SIM_CONNECTION_POINT_autoConnectWires(conPoint);
        SIM_COMP_LIST_appendConnectionPoint(&me->connectionPoints, conPoint);
        SIM_COMP_LIST_appendConnectionPoint(SIMSPACE_lstConnectionPoints, conPoint);
    }

    // top row
    for (int i = 0; i < chipSpec->numPins / 2; ++i) {
        sim_connection_point_t *conPoint = malloc(sizeof(sim_connection_point_t));
        SIM_CONNECTION_POINT_init(conPoint, CONNECTION_POINT_FLOATING, (Vector2){ position.x + chipSpec->numPins / 2 - i - 1, position.y });
        SIM_CONNECTION_POINT_autoConnectWires(conPoint);
        SIM_COMP_LIST_appendConnectionPoint(&me->connectionPoints, conPoint);
        SIM_COMP_LIST_appendConnectionPoint(SIMSPACE_lstConnectionPoints, conPoint);
    }

    // load the lua!!
    me->luaState = luaL_newstate();

    // load all lua standard system libs
    luaL_openlibs(me->luaState);

    // try to open the script file, if it goes to shit -> bail
    if (luaL_dofile(me->luaState, me->chipSpec->script) != LUA_OK)
    {
        lua_close(me->luaState);
        me->luaState = NULL;
        printf("SIM_CHIP_init: Couldnt load Script '%s'!! this is very bad\n", me->chipSpec->script);
        return;
    }

    // all chips require a PinSetup() function
    lua_getglobal(me->luaState, "PinSetup");
    if (!lua_isfunction(me->luaState, -1)) {
        lua_close(me->luaState);
        me->luaState = NULL;
        printf("SIM_CHIP_init: Script '%s' does not have a PinSetup() function! this script is unusable!\n", me->chipSpec->script);
        return;
    }
    lua_pop(me->luaState, 1);

    // make sure all required methods exist

    // for both stateful and stateless chips we require a Step() function
    lua_getglobal(me->luaState, "Step");
    if (!lua_isfunction(me->luaState, -1)) {
        lua_close(me->luaState);
        me->luaState = NULL;
        printf("SIM_CHIP_init: Script '%s' does not have a Step() function! this script is unusable!\n", me->chipSpec->script);
        return;
    }
    lua_pop(me->luaState, 1);


    if (chipSpec->isStateful) {

        // for stateful chips we require a StepRising() and a StepFalling() function

        // StepRising()
        lua_getglobal(me->luaState, "StepRising");
        if (!lua_isfunction(me->luaState, -1)) {
            lua_close(me->luaState);
            me->luaState = NULL;
            printf("SIM_CHIP_init: Script '%s' does not have a StepRising() function! this script is unusable!\n", me->chipSpec->script);
            return;
        }
        lua_pop(me->luaState, 1);

        // StepFalling()
        lua_getglobal(me->luaState, "StepFalling");
        if (!lua_isfunction(me->luaState, -1)) {
            lua_close(me->luaState);
            me->luaState = NULL;
            printf("SIM_CHIP_init: Script '%s' does not have a StepFalling() function! this script is unusable!\n", me->chipSpec->script);
            return;
        }
        lua_pop(me->luaState, 1);

        // as well as a LoadState() and SaveState() function

        // LoadState()
        lua_getglobal(me->luaState, "LoadState");
        if (!lua_isfunction(me->luaState, -1)) {
            lua_close(me->luaState);
            me->luaState = NULL;
            printf("SIM_CHIP_init: Script '%s' does not have a LoadState() function! this script is unusable!\n", me->chipSpec->script);
            return;
        }
        lua_pop(me->luaState, 1);

        // SaveState()
        lua_getglobal(me->luaState, "SaveState");
        if (!lua_isfunction(me->luaState, -1)) {
            lua_close(me->luaState);
            me->luaState = NULL;
            printf("SIM_CHIP_init: Script '%s' does not have a SaveState() function! this script is unusable!\n", me->chipSpec->script);
            return;
        }
        lua_pop(me->luaState, 1);
    }

    SIM_CHIP_setup(me);
}

void SIM_CHIP_refreshDrawable(sim_chip_t *me) {
    for (int i = 0; i < me->connectionPoints.length; ++i) {
        sim_connection_point_t *conPoint = me->connectionPoints.buffer[i];

        if (me->chipSpec->pinSpecs[i] == PIN_POWER) {
            me->chip->pinDisplayStates[i] = PIN_DISP_POWER;
            continue;
        }

        if (me->chipSpec->pinSpecs[i] == PIN_CLOCK) {
            me->chip->pinDisplayStates[i] = PIN_DISP_CLOCK;
            continue;
        }

        switch (conPoint->state) {
            case CONNECTION_POINT_FLOATING:
                me->chip->pinDisplayStates[i] = PIN_DISP_INPUT;
                continue;
            case CONNECTION_POINT_LOW:
                me->chip->pinDisplayStates[i] = PIN_DISP_OUTPUT_LOW;
                continue;
            case CONNECTION_POINT_HIGH:
                me->chip->pinDisplayStates[i] = PIN_DISP_OUTPUT_HIGH;
                continue;
        }
    }
}

void SIM_CHIP_setup(sim_chip_t *me) {
    if (me->luaState == NULL) return; // no valid lua script for this chip

    // load a reference to the function
    lua_getglobal(me->luaState, "PinSetup");

    // create a lua table that contains the current state of all connected pins
    SIM_CHIP_buildPinTable(me, false);

    // call the pin setup function
    if (lua_pcall(me->luaState, 1, 1, 0) != LUA_OK) {
        printf("SIM_CHIP_setup: Function SetupPins() in Script '%s' effectively shit itself\n%s\n", me->chipSpec->script, lua_tostring(me->luaState, -1));
        lua_close(me->luaState);
        me->luaState = NULL;
        return;
    }

    // do something with the result
    SIM_CHIP_readoutPinTable(me);
}

void SIM_CHIP_step(sim_chip_t *me) {
    if (me->luaState == NULL) return; // no valid lua script for this chip

    // -----------------------------------------------------------------------------------------------------------------
    // Call the step function

    // load a reference to the function
    lua_getglobal(me->luaState, "Step");

    // push the state of all pins onto the stack
    SIM_CHIP_buildPinTable(me, true);

    // call the step function
    if (lua_pcall(me->luaState, 1, 1, 0) != LUA_OK) {
        printf("SIM_CHIP_step: Function Step() in Script '%s' effectively shit itself\n%s\n", me->chipSpec->script, lua_tostring(me->luaState, -1));
        lua_close(me->luaState);
        me->luaState = NULL;
        return;
    }

    // do something with the result
    SIM_CHIP_readoutPinTable(me);

    // -----------------------------------------------------------------------------------------------------------------
    // Handle stateful chips
    if (!me->chipSpec->isStateful) return;

    // has the state of the clock changed?
    sim_connection_point_t *conPoint = me->connectionPoints.buffer[me->chipSpec->clockPin];
    if (me->prevClockPinWireState == conPoint->attachedWireState) return;

    // there has been a change! -> is it a rising or a falling edge?
    char *functionName = "StepFalling";
    if (conPoint->attachedWireState) functionName = "StepRising";

    // load a reference to the function
    lua_getglobal(me->luaState, functionName);

    // push the state of all pins onto the stack
    SIM_CHIP_buildPinTable(me, true);

    // call the step function
    if (lua_pcall(me->luaState, 1, 1, 0) != LUA_OK) {
        printf("SIM_CHIP_step: Function %s() in Script '%s' effectively shit itself\n%s\n", functionName, me->chipSpec->script, lua_tostring(me->luaState, -1));
        lua_close(me->luaState);
        me->luaState = NULL;
        return;
    }

    // do something with the result
    SIM_CHIP_readoutPinTable(me);

    // remember the current state of the clock pin
    me->prevClockPinWireState = conPoint->attachedWireState;
}

void SIM_CHIP_buildPinTable(sim_chip_t *me, bool includeWireState) {

    // create a lua table that contains the current state of all connected pins
    lua_newtable(me->luaState);
    for (int i = 0; i < me->connectionPoints.length; ++i) {

        // for each pin, create another table with fields "pin" and "wire"
        lua_newtable(me->luaState);

        // get the data into a suitable format
        // state 0 = LOW
        //       1 = HIGH
        //       2 = FLOATING
        sim_connection_point_t *conPoint = me->connectionPoints.buffer[i];

        int pinState = 0;
        if (conPoint->state == CONNECTION_POINT_HIGH) pinState = 1;
        if (conPoint->state == CONNECTION_POINT_FLOATING) pinState = 2;

        int wireState = conPoint->attachedWireState ? 1 : 0;

        // push the data into the record
        lua_pushnumber(me->luaState, pinState);
        lua_setfield(me->luaState, -2, "pin");

        // if required, also push the state of the wire
        if (includeWireState) {
            lua_pushnumber(me->luaState, wireState);
            lua_setfield(me->luaState, -2, "wire");
        }

        // push the record onto the pin table
        lua_seti(me->luaState, -2, i + 1);
    }
}

void SIM_CHIP_readoutPinTable(sim_chip_t *me) {

    // apply the changes made to the pin states
    for (int i = 0; i < me->connectionPoints.length; ++i) {

        // get this record on the stack
        lua_geti(me->luaState, -1, i + 1);

        // only take a look at the pin state
        lua_getfield(me->luaState, -1, "pin");
        int pinState = lua_tointeger(me->luaState, -1);
        lua_pop(me->luaState, 2);

        // transfer the state
        sim_connection_point_t *conPoint = me->connectionPoints.buffer[i];
        if (pinState == 0) conPoint->state = CONNECTION_POINT_LOW;
        if (pinState == 1) conPoint->state = CONNECTION_POINT_HIGH;
        if (pinState == 2) conPoint->state = CONNECTION_POINT_FLOATING;
    }

    lua_pop(me->luaState, 1);
}

void SIM_CHIP_unload(sim_chip_t *me) {
    DRAWABLES_unload((drawable_t*)me->chip);

    for (int i = 0; i < me->connectionPoints.length; ++i) {
        sim_connection_point_t *conPoint = me->connectionPoints.buffer[i];

        // remove this connection point from all connections
        for (int conIndex = 0; conIndex < SIMSPACE_lstConnections->length; ++conIndex) {
            sim_connection_t *con = SIMSPACE_lstConnections->buffer[conIndex];
            SIM_COMP_LIST_removeConnectionPointRef(&con->lstConnectedPoints, conPoint);
        }

        // destroy it
        SIM_COMP_LIST_removeConnectionPointRef(SIMSPACE_lstConnectionPoints, conPoint);
        free(conPoint);
    }
}