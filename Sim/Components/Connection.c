//
// Created by redcube on 12/05/2026.
//
#include "Connection.h"

#include <stddef.h>
#include <stdlib.h>

void SIM_CONNECTION_init(sim_connection_t *me, Color wireColor) {

    SIM_COMP_LIST_init(&me->lstConnectedPoints);
    SIM_CONNECTION_DRAWABLE_LIST_init(&me->lstDrawables);
    SIM_CONNECTION_VECTOR_PAIR_LIST_init(&me->lstVectorPairs);

    me->color = wireColor;
}

void SIM_CONNECTION_unload(sim_connection_t *me) {
    SIM_COMP_LIST_clear(&me->lstConnectedPoints);
    SIM_CONNECTION_DRAWABLE_LIST_clear(&me->lstDrawables);
    SIM_CONNECTION_VECTOR_PAIR_LIST_clear(&me->lstVectorPairs);
    free(me);
}

void SIM_CONNECTION_refreshState(sim_connection_t *me) {

    // count the states of all the connected points
    int numPointsFloating = 0;
    int numPointsHigh = 0;
    int numPointsLow = 0;

    for (int i = 0; i < me->lstConnectedPoints.length; ++i) {
        sim_connection_point_t *point = me->lstConnectedPoints.buffer[i];
        if (point->state == CONNECTION_POINT_FLOATING) numPointsFloating++;
        if (point->state == CONNECTION_POINT_HIGH) numPointsHigh++;
        if (point->state == CONNECTION_POINT_LOW) numPointsLow++;
    }

    // nothin
    if (numPointsFloating == 0 && numPointsHigh == 0 && numPointsLow == 0) {
        me->active = false;
        me->error = false;
    }

    // if one or more is high and none are low -> the line is high
    else if (numPointsHigh > 0 && numPointsLow == 0) {
        me->active = true;
        me->error = false;
    }

    // if one or more is low and none are high -> the line is low
    else if (numPointsHigh == 0 && numPointsLow > 0) {
        me->active = false;
        me->error = false;
    }

    // if there are points that are low and points that are high -> this is a dead short! bad!!
    else if (numPointsHigh > 0 && numPointsLow > 0) {
        me->active = false;
        me->error = true;
    }

    // if all connection points are floating (probably because theyre inputs) -> this is also bad! it should be grounded
    else if (numPointsFloating == me->lstConnectedPoints.length) {
        me->active = false;
        me->error = true;
    }

    // distribute the new wire state to all points so they can act upon it
    for (int i = 0; i < me->lstConnectedPoints.length; ++i) {
        sim_connection_point_t *point = me->lstConnectedPoints.buffer[i];
        point->attachedWireState = me->active;
    }

    // distribute the new state to the wires
    SIM_CONNECTION_refreshDrawablesState(me);
}

void SIM_CONNECTION_refreshDrawablesState(sim_connection_t *me) {

    // distribute this stuff to the wires
    for (int i = 0; i < me->lstDrawables.length; ++i) {
        drw_wire_t *wire = (drw_wire_t*)me->lstDrawables.buffer[i];
        wire->active = me->active;
        wire->error = me->error;
    }
}

void SIM_CONNECTION_refreshDrawablesStructure(sim_connection_t *me) {

    // nuke everything
    SIM_CONNECTION_clearDrawables(me);

    // and then build it again
    for (int i = 0; i < me->lstVectorPairs.length; ++i) {
        // create a new wire
        drw_wire_t *newWire = malloc(sizeof(drw_wire_t));
        DRAWABLES_WIRE_init(newWire, me->lstVectorPairs.buffer[i].from, me->lstVectorPairs.buffer[i].to, me->color);
        DRAWABLES_enqueue((drawable_t*)newWire);
        SIM_CONNECTION_DRAWABLE_LIST_add(&me->lstDrawables, (drawable_t*)newWire);
    }

    // refresh the state
    SIM_CONNECTION_refreshState(me);
}

void SIM_CONNECTION_clearDrawables(sim_connection_t *me) {

    // unload all drawables
    for (int i = 0; i < me->lstDrawables.length; ++i) {
        DRAWABLES_unload(me->lstDrawables.buffer[i]);
    }

    // clear the buffer
    SIM_CONNECTION_DRAWABLE_LIST_clear(&me->lstDrawables);
}