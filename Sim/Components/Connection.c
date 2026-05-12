//
// Created by redcube on 12/05/2026.
//
#include "Connection.h"

#include <stddef.h>
#include <stdlib.h>

void SIM_CONNECTION_init(sim_connection_t *me, Color wireColor) {
    me->pointA = NULL;
    me->pointB = NULL;

    SIM_CONNECTION_DRAWABLE_LIST_init(&me->lstDrawables);
    SIM_CONNECTION_POINT_LIST_init(&me->lstConnectionPoints);

    me->color = wireColor;
}

void SIM_CONNECTION_refreshState(sim_connection_t *me) {

    // if both points are null (nothing connected on either side) -> not active
    if (me->pointA == NULL && me->pointB == NULL) {
        me->active = false;
        me->error = false;
    }

    // if only A or only B is connected
    else if (me->pointA == NULL || me->pointB == NULL) {

        // select the point thats connected
        sim_connection_point_t *point = me->pointA;
        if (point == NULL) point = me->pointB;

        me->error = false;

        // use its state
        if (point->state == CONNECTION_POINT_DISCONNECTED || point->state == CONNECTION_POINT_FLOATING ||
            point->state == CONNECTION_POINT_LOW) {

            me->active = false;
        }
        else {

            me->active = true;
        }

    // if both sides are connected -> figure this shit out
    } else {

        // both sides floating
        if ((me->pointA->state == CONNECTION_POINT_DISCONNECTED || me->pointA->state == CONNECTION_POINT_FLOATING)
            &&
            (me->pointB->state == CONNECTION_POINT_DISCONNECTED || me->pointB->state == CONNECTION_POINT_FLOATING)) {

            me->active = false;
            me->error = true; // this is not good, probably two inputs connected together
        }

        // A side floating
        else if ((me->pointA->state == CONNECTION_POINT_DISCONNECTED || me->pointA->state == CONNECTION_POINT_FLOATING)
            &&
            (me->pointB->state == CONNECTION_POINT_HIGH || me->pointB->state == CONNECTION_POINT_LOW)) {

            me->active = me->pointB->state == CONNECTION_POINT_HIGH;
            me->error = false;
        }

        // B side floating
        else if ((me->pointB->state == CONNECTION_POINT_DISCONNECTED || me->pointB->state == CONNECTION_POINT_FLOATING)
            &&
            (me->pointA->state == CONNECTION_POINT_HIGH || me->pointA->state == CONNECTION_POINT_LOW)) {

            me->active = me->pointA->state == CONNECTION_POINT_HIGH;
            me->error = false;
        }

        // neither side floating -> this shit is getting a little more involved
        else {

            // if both sides are high or both sides are low -> use that state
            if (me->pointA->state == me->pointB->state) {
                me->active = me->pointA->state == CONNECTION_POINT_HIGH;
                me->error = false;
            }

            // if one side is high and the other is low -> dead short! this is bad!
            else {
                me->active = false;
                me->error = true;
            }

        }
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
    for (int i = 0; i < me->lstConnectionPoints.length-1; ++i) {
        // create a new wire
        drw_wire_t *newWire = malloc(sizeof(drw_wire_t));
        DRAWABLES_WIRE_init(newWire, me->lstConnectionPoints.buffer[i], me->lstConnectionPoints.buffer[i + 1], me->color);
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