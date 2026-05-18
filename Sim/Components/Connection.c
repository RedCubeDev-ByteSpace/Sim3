//
// Created by redcube on 12/05/2026.
//
#include "Connection.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../GUI/Drawables/WireBranchingPoint.h"

void SIM_CONNECTION_init(sim_connection_t *me, Color wireColor) {

    SIM_COMP_LIST_init(&me->lstConnectedPoints);
    SIM_CONNECTION_DRAWABLE_LIST_init(&me->lstDrawables);
    SIM_CONNECTION_VECTOR_PAIR_LIST_init(&me->lstVectorPairs);
    SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_init(&me->lstConnectableVectors);

    me->color = wireColor;
}

void SIM_CONNECTION_unload(sim_connection_t *me) {
    SIM_COMP_LIST_clear(&me->lstConnectedPoints);
    SIM_CONNECTION_clearDrawables(me);
    SIM_CONNECTION_VECTOR_PAIR_LIST_clear(&me->lstVectorPairs);
    SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_clear(&me->lstConnectableVectors);
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
        if (me->lstDrawables.buffer[i]->type == DRAWABLE_WIRE) {
            drw_wire_t *wire = (drw_wire_t*)me->lstDrawables.buffer[i];
            wire->active = me->active;
            wire->error = me->error;
        }
        else if (me->lstDrawables.buffer[i]->type == DRAWABLE_WIRE_BRANCHING_POINT) {
            drw_wire_branching_point_t *branchingPoint = (drw_wire_branching_point_t*)me->lstDrawables.buffer[i];
            branchingPoint->error = me->error;
        }
    }
}

void SIM_CONNECTION_refreshDrawablesStructure(sim_connection_t *me) {

    // nuke everything
    SIM_CONNECTION_clearDrawables(me);

    // e v e r y t h i n g
    SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_clear(&me->lstConnectableVectors);

    // hold a list of all used vectors to insert connection points later
    int maxNumRecords = me->lstVectorPairs.length * 2;
    sim_vector_record_t *records = calloc(maxNumRecords, sizeof(sim_vector_record_t));

    // and then build it again
    for (int i = 0; i < me->lstVectorPairs.length; ++i) {
        // create a new wire
        drw_wire_t *newWire = malloc(sizeof(drw_wire_t));
        DRAWABLES_WIRE_init(newWire, me->lstVectorPairs.buffer[i].from, me->lstVectorPairs.buffer[i].to, me->color);
        DRAWABLES_enqueue((drawable_t*)newWire);
        SIM_CONNECTION_DRAWABLE_LIST_add(&me->lstDrawables, (drawable_t*)newWire);

        // register both vectors of this pair
        SIM_VECTOR_RECORD_registerVector(records, me->lstVectorPairs.buffer[i].from, maxNumRecords);
        SIM_VECTOR_RECORD_registerVector(records, me->lstVectorPairs.buffer[i].to, maxNumRecords);

        // take note of all potential points on this line where a wire could connect to
        SIM_CONNECTION_calculateConnectableVectors(me, me->lstVectorPairs.buffer[i].from, me->lstVectorPairs.buffer[i].to, i);
    }

    // go through the records and put down branching points
    for (int i = 0; i < maxNumRecords; ++i) {
        if (records[i].amount == 0) break;
        if (records[i].amount < 3) continue;

        drw_wire_branching_point_t *branchingPoint = malloc(sizeof(drw_wire_branching_point_t));
        DRAWABLES_WIRE_BRANCHING_POINT_init(branchingPoint, records[i].vec, me->color);
        DRAWABLES_enqueue((drawable_t*)branchingPoint);
        SIM_CONNECTION_DRAWABLE_LIST_add(&me->lstDrawables, (drawable_t*)branchingPoint);
    }

    // were so done with this
    free(records);

    // refresh the state
    SIM_CONNECTION_refreshState(me);
}

void SIM_CONNECTION_calculateConnectableVectors(sim_connection_t *me, Vector2 pointA, Vector2 pointB, int index) {

    // if theyre the same then theres nothing for us to do
    if (pointA.x == pointB.x && pointA.y == pointB.y) return;

    // calculate the horizontal distance between the points
    int deltaX = pointA.x - pointB.x;
    if (deltaX < 0) deltaX = -deltaX;

    // if this is a vertical line -> special case
    if (deltaX == 0) {
        int fromY, toY;
        if (pointA.y > pointB.y) {
            fromY = pointB.y;
            toY = pointA.y;
        }
        else {
            fromY = pointA.y;
            toY = pointB.y;
        }

        for (int y = fromY + 1; y < toY; ++y) {

            // add all points on the way as connectable points
            SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_append(&me->lstConnectableVectors, (Vector2){ pointA.x, y }, index);
        }

        // done
        return;
    }

    // if its not -> actually math it out
    Vector2 from, to;
    if (pointA.x < pointB.x) {
        from = pointA;
        to = pointB;
    }
    else {
        from = pointB;
        to = pointA;
    }

    int deltaY = to.y - from.y;
    float slope = (float)deltaY / (float)deltaX;

    for (int x = 1; x < deltaX; ++x) {

        float y = x * slope;

        // if this is a whole number point
        if (y == roundf(y)) {

            // add it to the connectables
            SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_append(&me->lstConnectableVectors, (Vector2){ from.x + x, from.y + y }, index);
        }

    }
}

void SIM_VECTOR_RECORD_registerVector(sim_vector_record_t *records, Vector2 vec, int maxNum) {
    for (int i = 0; i < maxNum; ++i) {

        // if we reached a free slot -> were at the end of the list
        // the vector wasnt found so well create a new record for it
        if (records[i].amount == 0) {
            records[i].vec = vec;
            records[i].amount = 1;
            return;
        }

        // otherwise -> check if we found a fitting record
        if (records[i].vec.x == vec.x && records[i].vec.y == vec.y) {
            records[i].amount++;
            return;
        }
    }

}

void SIM_CONNECTION_clearDrawables(sim_connection_t *me) {

    // unload all drawables
    for (int i = 0; i < me->lstDrawables.length; ++i) {
        DRAWABLES_unload(me->lstDrawables.buffer[i]);
    }

    // clear the buffer
    SIM_CONNECTION_DRAWABLE_LIST_clear(&me->lstDrawables);
}

// ---------------------------------------------------------------------------------------------------------------------
void SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_init(sim_connection_connectable_vector_list_t *me) {
    me->buffer = NULL;
    me->bufferSize = 0;
    me->length = 0;
}

void SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_append(sim_connection_connectable_vector_list_t *me, Vector2 vec, int vectorPairIndex) {
    // grow the list to make space for this entry
    SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_grow(me);

    // assign this entry to the new slot
    me->buffer[me->length - 1] = (sim_connectable_vector_t){ vec, vectorPairIndex };
}

void SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_grow(sim_connection_connectable_vector_list_t *me) {
    me->length++;
    if (me->length <= me->bufferSize) return; // theres still enough space in the buffer

    // otherwise: grow the buffer
    me->bufferSize += 5;
    sim_connectable_vector_t *newBuffer = realloc(me->buffer, me->bufferSize * sizeof(sim_connectable_vector_t));

    if (newBuffer == NULL) {
        printf("SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_grow: this is fucked");
        me->bufferSize -= 5;
        return;
    }

    me->buffer = newBuffer;
}

void SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_clear(sim_connection_connectable_vector_list_t *me) {
    free(me->buffer);
    me->buffer = NULL;
    me->bufferSize = 0;
    me->length = 0;
}