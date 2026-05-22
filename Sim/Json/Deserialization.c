//
// Created by redcube on 20/05/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../SaveAndLoad.h"
#include "../SimulationResources.h"
#include "../Components/Chip.h"

Vector2 SAVE_AND_LOAD_deserializeVector2(json_object *json) {

    return (Vector2){
        json_object_get_double(json_object_array_get_idx(json, 0)),
        json_object_get_double(json_object_array_get_idx(json, 1))
    };
}

Color SAVE_AND_LOAD_deserializeColor(json_object *json) {

    return (Color){
         json_object_get_int(json_object_array_get_idx(json, 0)) % 256,
        json_object_get_int(json_object_array_get_idx(json, 1)) % 256,
        json_object_get_int(json_object_array_get_idx(json, 2)) % 256,
        json_object_get_int(json_object_array_get_idx(json, 3)) % 256,
    };
}

void SAVE_AND_LOAD_deserializeConnectionPointStates(json_object *json, sim_chip_t *chip) {
    int len = json_object_array_length(json);
    for (int i = 0; i < chip->chipSpec->numPins && i < len; ++i) {
        sim_connection_point_t *conPoint = chip->connectionPoints.buffer[i];

        json_object *conPointJson = json_object_array_get_idx(json, i);
        int state = json_object_get_int(json_object_object_get(conPointJson, "pin"));
        bool wire = json_object_get_boolean(json_object_object_get(conPointJson, "wire"));

        if (state == 0) conPoint->state = CONNECTION_POINT_LOW;
        if (state == 1) conPoint->state = CONNECTION_POINT_HIGH;
        if (state == 2) conPoint->state = CONNECTION_POINT_FLOATING;
        conPoint->attachedWireState = wire;
    }
}

void SAVE_AND_LOAD_deserializeConnection(json_object *json) {

    sim_connection_t *connection = malloc(sizeof(sim_connection_t));
    SIM_CONNECTION_init(connection);

    // go through all saved vector pairs
    json_object *vectorPairs = json_object_object_get(json, "vector_pairs");
    int len = json_object_array_length(vectorPairs);

    for (int i = 0; i < len; i++) {
        json_object *vectorPair = json_object_array_get_idx(vectorPairs, i);

        Vector2 from = SAVE_AND_LOAD_deserializeVector2(json_object_object_get(vectorPair, "from"));
        Vector2 to = SAVE_AND_LOAD_deserializeVector2(json_object_object_get(vectorPair, "to"));
        Color color = SAVE_AND_LOAD_deserializeColor(json_object_object_get(vectorPair, "color"));

        // add it to the connection object
        SIM_CONNECTION_VECTOR_PAIR_LIST_append(&connection->lstVectorPairs, (sim_vector_pair_t){
            from, to, color
        });
    }

    // regenerate all connection data from the vector pairs
    SIM_CONNECTION_refreshDrawablesStructure(connection);

    // add it to the global simspace
    SIM_COMP_LIST_appendConnection(SIMSPACE_lstConnections, connection);
}

void SAVE_AND_LOAD_deserializeFixedContact(json_object *json) {

    // read out all its properties
    Vector2 position = SAVE_AND_LOAD_deserializeVector2(json_object_object_get(json, "position"));
    int rotation = json_object_get_int(json_object_object_get(json, "rotation")) % 4;
    bool state = json_object_get_boolean(json_object_object_get(json, "state"));

    // create a new fixed contact based on the data
    sim_fixed_contact_t *fixedContact = malloc(sizeof(sim_fixed_contact_t));
    SIM_FIXED_CONTACT_init(fixedContact, position, state, rotation);

    // add it to the global simspace
    SIM_COMP_LIST_appendFixedContact(SIMSPACE_lstFixedContacts, fixedContact);
}

void SAVE_AND_LOAD_deserializeLED(json_object *json) {

    // read out all its properties
    Vector2 position = SAVE_AND_LOAD_deserializeVector2(json_object_object_get(json, "position"));
    int rotation = json_object_get_int(json_object_object_get(json, "rotation")) % 4;

    // create a new fixed contact based on the data
    sim_led_t *led = malloc(sizeof(sim_led_t));
    SIM_LED_init(led, position, rotation);

    // add it to the global simspace
    SIM_COMP_LIST_appendLED(SIMSPACE_lstLEDs, led);
}

void SAVE_AND_LOAD_deserializeChip(json_object *json) {
    // read out all its properties
    char *specId = json_object_get_string(json_object_object_get(json, "spec_id"));
    Vector2 position = SAVE_AND_LOAD_deserializeVector2(json_object_object_get(json, "position"));
    char *state = json_object_get_string(json_object_object_get(json, "state"));

    // find the spec this is referencing
    sim_chip_specification_t *chipSpec = NULL;
    for (int i = 0; i < SIMRES_numChipSpecs; ++i) {
        if (strcmp(specId, SIMRES_chipSpecifications[i].id) == 0) {
            chipSpec = &SIMRES_chipSpecifications[i];
            break;
        }
    }

    // did we find the spec?
    if (chipSpec == NULL) {
        printf("Error while loading savefile! Chip specification '%s' doesnt exist!\n", specId);
        return;
    }

    // create a new chip instance
    sim_chip_t *chip = malloc(sizeof(sim_chip_t));
    SIM_CHIP_init(chip, position, chipSpec);

    // load the state of the pins
    json_object *pinStatesJson = json_object_object_get(json, "pin_states");
    if (pinStatesJson != NULL) {
        SAVE_AND_LOAD_deserializeConnectionPointStates(pinStatesJson, chip);
    }

    // if this is a stateful chip -> load its state
    if (chip->luaState != NULL && chipSpec->isStateful && chip->luaHasLoadState) {

        // make sure the clock pin state is set correctly
        sim_connection_point_t *conPoint = chip->connectionPoints.buffer[chip->chipSpec->clockPin];
        chip->prevClockPinWireState = conPoint->attachedWireState;

        // load a reference to the LoadState() function
        lua_getglobal(chip->luaState, "LoadState");

        // load a reference to the __deserialize() function
        lua_getglobal(chip->luaState, "__deserialize");

        // load the data as an argument
        lua_pushstring(chip->luaState, state);

        // call the __deserialize() function
        if (lua_pcall(chip->luaState, 1, 1, 0) != LUA_OK) {
            printf("SIM_CHIP_step: Function __deserialize() in Script '%s' effectively shit itself\n%s\n", chip->chipSpec->script, lua_tostring(chip->luaState, -1));
            lua_close(chip->luaState);
            chip->luaState = NULL;
            return;
        }

        // call the LoadState() function
        if (lua_pcall(chip->luaState, 1, 0, 0) != LUA_OK) {
            printf("SIM_CHIP_step: Function LoadState() in Script '%s' effectively shit itself\n%s\n", chip->chipSpec->script, lua_tostring(chip->luaState, -1));
            lua_close(chip->luaState);
            chip->luaState = NULL;
            return;
        }
    }

    // add it to the global simspace
    SIM_COMP_LIST_appendChip(SIMSPACE_lstChips, chip);
}