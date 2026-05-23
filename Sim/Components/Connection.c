//
// Created by redcube on 12/05/2026.
//
#include "Connection.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../SimulationSpace.h"
#include "../../GUI/Drawables/WireBranchingPoint.h"

void SIM_CONNECTION_init(sim_connection_t *me) {

    SIM_COMP_LIST_init(&me->lstConnectedPoints);
    SIM_CONNECTION_DRAWABLE_LIST_init(&me->lstDrawables);
    SIM_CONNECTION_VECTOR_PAIR_LIST_init(&me->lstVectorPairs);
    SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_init(&me->lstConnectableVectors);
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
        sim_vector_pair_t vecPair = me->lstVectorPairs.buffer[i];

        // create a new wire
        drw_wire_t *newWire = malloc(sizeof(drw_wire_t));
        DRAWABLES_WIRE_init(newWire, vecPair.from, vecPair.to, vecPair.color);
        DRAWABLES_enqueue((drawable_t*)newWire);
        SIM_CONNECTION_DRAWABLE_LIST_add(&me->lstDrawables, (drawable_t*)newWire);

        // register both vectors of this pair
        SIM_VECTOR_RECORD_registerVector(records, vecPair.from, vecPair.color, maxNumRecords);
        SIM_VECTOR_RECORD_registerVector(records, vecPair.to, vecPair.color, maxNumRecords);

        // take note of all potential points on this line where a wire could connect to
        SIM_CONNECTION_calculateConnectableVectors(me, vecPair.from, vecPair.to, i);
    }

    // go through the records and put down branching points
    for (int i = 0; i < maxNumRecords; ++i) {
        if (records[i].amount == 0) break;
        if (records[i].amount < 3) continue;

        drw_wire_branching_point_t *branchingPoint = malloc(sizeof(drw_wire_branching_point_t));
        DRAWABLES_WIRE_BRANCHING_POINT_init(branchingPoint, records[i].vec, records[i].color);
        DRAWABLES_enqueue((drawable_t*)branchingPoint);
        SIM_CONNECTION_DRAWABLE_LIST_add(&me->lstDrawables, (drawable_t*)branchingPoint);
    }

    // for (int i = 0; i < me->lstConnectableVectors.length; ++i) {
    //     drw_wire_branching_point_t *branchingPoint = malloc(sizeof(drw_wire_branching_point_t));
    //     DRAWABLES_WIRE_BRANCHING_POINT_init(branchingPoint, me->lstConnectableVectors.buffer[i].position, RED);
    //     DRAWABLES_enqueue((drawable_t*)branchingPoint);
    //     SIM_CONNECTION_DRAWABLE_LIST_add(&me->lstDrawables, (drawable_t*)branchingPoint);
    // }

    // were so done with this
    free(records);

    // refresh the state
    SIM_CONNECTION_refreshState(me);
}

void SIM_CONNECTION_calculateConnectableVectors(sim_connection_t *me, Vector2 pointA, Vector2 pointB, int index) {

    // if theyre the same then theres nothing for us to do
    if (pointA.x == pointB.x && pointA.y == pointB.y) return;

    // calculate the horizontal distance between the points
    float deltaX = pointA.x - pointB.x;
    if (deltaX < 0) deltaX = -deltaX;

    // if this is a vertical line -> special case
    if (deltaX == 0) {
        float fromY, toY;
        if (pointA.y > pointB.y) {
            fromY = pointB.y;
            toY = pointA.y;
        }
        else {
            fromY = pointA.y;
            toY = pointB.y;
        }

        for (float y = fromY + 0.5f; y < toY; y += 0.5f) {

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

    float deltaY = to.y - from.y;
    float slope = deltaY / deltaX;

    for (float x = 0.5f; x < deltaX; x += 0.5f) {

        float y = x * slope;

        // if this is a whole number point
        if (y * 2 == roundf(y * 2)) {

            // add it to the connectables
            SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_append(&me->lstConnectableVectors, (Vector2){ from.x + x, from.y + y }, index);
        }

    }
}

void SIM_CONNECTION_rebuildAndSplitConnection(sim_connection_t *me) {
    if (me->lstVectorPairs.length == 0) return;

    sim_connection_t *newConnection = NULL;

    // create a new dynamic vector pair list as a queue of vectors to check
    sim_connection_vector_pair_list_t vectorPairsToCheck;
    SIM_CONNECTION_VECTOR_PAIR_LIST_init(&vectorPairsToCheck);

    // create a new dynamic vector pair list for any matches we found
    sim_connection_vector_pair_list_t vectorPairsThatConnect;
    SIM_CONNECTION_VECTOR_PAIR_LIST_init(&vectorPairsThatConnect);

    // create a new connection point list for connection points to move over to the new con
    sim_comp_list_t connectionPointsThatConnect;
    SIM_COMP_LIST_init(&connectionPointsThatConnect);

    // as long as there are vector pairs in the connection left
    while (me->lstVectorPairs.length > 0) {

        // if there is no vector pair to check in the queue yet but there are still some left in the connection
        // -> take the first one out of the connection and add it to the checking queue
        if (vectorPairsToCheck.length == 0) {

            // if there already was a connection we were building
            // ->finalize it
            if (newConnection != NULL) {
                SIM_COMP_LIST_appendConnection(SIMSPACE_lstConnections, newConnection);
                SIM_CONNECTION_refreshDrawablesStructure(newConnection);
            }

            // start a new connection object
            newConnection = malloc(sizeof(sim_connection_t));
            SIM_CONNECTION_init(newConnection);

            sim_vector_pair_t firstVecPair = me->lstVectorPairs.buffer[0];
            SIM_CONNECTION_VECTOR_PAIR_LIST_removeAt(&me->lstVectorPairs, 0);
            SIM_CONNECTION_VECTOR_PAIR_LIST_append(&vectorPairsToCheck, firstVecPair);
        }

        // travel through all connected vectors as long as we have any left
        while (vectorPairsToCheck.length > 0) {

            // dequeue the vector pair at the front
            sim_vector_pair_t thisPair = vectorPairsToCheck.buffer[0];
            SIM_CONNECTION_VECTOR_PAIR_LIST_removeAt(&vectorPairsToCheck, 0);

            // add it to the new connection
            SIM_CONNECTION_VECTOR_PAIR_LIST_append(&newConnection->lstVectorPairs, thisPair);

            // find all connection points attached to this pair
            for (int i = 0; i < me->lstConnectedPoints.length; ++i) {
                sim_connection_point_t *thisPoint = me->lstConnectedPoints.buffer[i];

                if (
                    thisPair.from.x == thisPoint->position.x && thisPair.from.y == thisPoint->position.y
                    ||
                    thisPair.to.x == thisPoint->position.x && thisPair.to.y == thisPoint->position.y
                ) {
                    SIM_COMP_LIST_appendConnectionPoint(&connectionPointsThatConnect, thisPoint);
                }
            }

            // shovel them over!!
            for (int i = 0; i < connectionPointsThatConnect.length; ++i) {
                sim_connection_point_t *thisPoint = connectionPointsThatConnect.buffer[i];

                // remove from the old
                SIM_COMP_LIST_removeConnectionPointRef(&me->lstConnectedPoints, thisPoint);

                // add to the new
                SIM_COMP_LIST_appendConnectionPoint(&newConnection->lstConnectedPoints, thisPoint);
            }

            SIM_COMP_LIST_clear(&connectionPointsThatConnect);

            // go through all remaining vectors of this connection and look for any vector pairs
            // that are connected to this one
            // basically, we're walking a tree, idk how to describe this idk man you know how trees work
            for (int i = 0; i < me->lstVectorPairs.length; ++i) {
                sim_vector_pair_t otherPair = me->lstVectorPairs.buffer[i];

                if (
                    // this FROM = other FROM
                    (thisPair.from.x == otherPair.from.x && thisPair.from.y == otherPair.from.y)
                    ||
                    // this FROM = other TO
                    (thisPair.from.x == otherPair.to.x && thisPair.from.y == otherPair.to.y)
                    ||
                    // this TO = other FROM
                    (thisPair.to.x == otherPair.from.x && thisPair.to.y == otherPair.from.y)
                    ||
                    // this TO = other TO
                    (thisPair.to.x == otherPair.to.x && thisPair.to.y == otherPair.to.y)
                ) {
                    // this one is connected to the last one!!!
                    // remember it so we can delete it properly and add it to the queue
                    SIM_CONNECTION_VECTOR_PAIR_LIST_append(&vectorPairsThatConnect, otherPair);
                }
            }

            // go through all the vector pairs that connect to this one
            for (int i = 0; i < vectorPairsThatConnect.length; ++i) {
                sim_vector_pair_t otherPair = vectorPairsThatConnect.buffer[i];

                // remove this vector pair from the connection
                SIM_CONNECTION_VECTOR_PAIR_LIST_remove(&me->lstVectorPairs, otherPair);

                // add this vector pair to the queue to check further
                SIM_CONNECTION_VECTOR_PAIR_LIST_append(&vectorPairsToCheck, otherPair);
            }

            // free the temp lists buffer
            SIM_CONNECTION_VECTOR_PAIR_LIST_clear(&vectorPairsThatConnect);
        }
    }

    SIM_CONNECTION_VECTOR_PAIR_LIST_clear(&vectorPairsToCheck);

    if (newConnection != NULL) {
        SIM_COMP_LIST_appendConnection(SIMSPACE_lstConnections, newConnection);
        SIM_CONNECTION_refreshDrawablesStructure(newConnection);
    }

    // ----------------------------------
    // clean this old empty connection up

    // reset all connected connection points
    for (int ii = 0; ii < me->lstConnectedPoints.length; ++ii) {
        ((sim_connection_point_t*)(me->lstConnectedPoints.buffer[ii]))->attachedWireState = false;
    }

    SIM_CONNECTION_unload(me);
    SIM_COMP_LIST_removeConnectionRef(SIMSPACE_lstConnections, me);
    free(me);
}

void SIM_VECTOR_RECORD_registerVector(sim_vector_record_t *records, Vector2 vec, Color color, int maxNum) {
    for (int i = 0; i < maxNum; ++i) {

        // if we reached a free slot -> were at the end of the list
        // the vector wasnt found so well create a new record for it
        if (records[i].amount == 0) {
            records[i].vec = vec;
            records[i].color = color;
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