//
// Created by redcube on 12/05/2026.
//
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "Connection.h"

void SIM_CONNECTION_VECTOR_PAIR_LIST_init(sim_connection_vector_pair_list_t *me) {
    me->buffer = NULL;
    me->bufferSize = 0;
    me->length = 0;
}

void SIM_CONNECTION_VECTOR_PAIR_LIST_append(sim_connection_vector_pair_list_t *me, sim_vector_pair_t newPair) {

    // make sure this pair doesnt already exist because what
    for (int i = 0; i < me->length; ++i) {
        if (me->buffer[i].from.x == newPair.from.x && me->buffer[i].from.y == newPair.from.y
            &&
            me->buffer[i].to.x == newPair.to.x && me->buffer[i].to.y == newPair.to.y) {
            return;
        }

        // also check the reverse
        if (me->buffer[i].from.x == newPair.to.x && me->buffer[i].from.y == newPair.to.y
            &&
            me->buffer[i].to.x == newPair.from.x && me->buffer[i].to.y == newPair.from.y) {
            return;
        }
    }

    // grow the list and add this point
    SIM_CONNECTION_VECTOR_PAIR_LIST_grow(me);
    me->buffer[me->length-1] = newPair;
}

void SIM_CONNECTION_VECTOR_PAIR_insertAt(sim_connection_vector_pair_list_t *me, sim_vector_pair_t newPair, uint32_t index) {
    if (index >= me->length) return; // huh? what? what are you even trying to do?

    // make sure this pair doesnt already exist because what
    for (int i = 0; i < me->length; ++i) {
        if (me->buffer[i].from.x == newPair.from.x && me->buffer[i].from.y == newPair.from.y
            &&
            me->buffer[i].to.x == newPair.to.x && me->buffer[i].to.y == newPair.to.y) {
            return;
            }

        // also check the reverse
        if (me->buffer[i].from.x == newPair.to.x && me->buffer[i].from.y == newPair.to.y
            &&
            me->buffer[i].to.x == newPair.from.x && me->buffer[i].to.y == newPair.from.y) {
            return;
            }
    }

    // grow the list
    SIM_CONNECTION_VECTOR_PAIR_LIST_grow(me);

    // move everything after it back
    for (uint32_t i = me->length-1; i > index; i--) {
        me->buffer[i] = me->buffer[i - 1];
    }

    // slot it in
    me->buffer[index] = newPair;
}

void SIM_CONNECTION_VECTOR_PAIR_LIST_removeAt(sim_connection_vector_pair_list_t *me, uint32_t index) {
    if (index >= me->length) return; // nah bruv

    // move everything after the index back
    for (int i = index; i < me->length-1; ++i) {
        me->buffer[i] = me->buffer[i + 1];
    }

    // shrinkificate
    SIM_CONNECTION_VECTOR_PAIR_LIST_shrink(me);
}

void SIM_CONNECTION_VECTOR_PAIR_LIST_remove(sim_connection_vector_pair_list_t *me, sim_vector_pair_t oldPair) {

    // look for this pair and / or its reverse
    for (int i = 0; i < me->length; ++i) {
        if (me->buffer[i].from.x == oldPair.from.x && me->buffer[i].from.y == oldPair.from.y
            &&
            me->buffer[i].to.x == oldPair.to.x && me->buffer[i].to.y == oldPair.to.y) {

            SIM_CONNECTION_VECTOR_PAIR_LIST_removeAt(me, i);
            return;
        }

        // also check the reverse
        if (me->buffer[i].from.x == oldPair.to.x && me->buffer[i].from.y == oldPair.to.y
            &&
            me->buffer[i].to.x == oldPair.from.x && me->buffer[i].to.y == oldPair.from.y) {

            SIM_CONNECTION_VECTOR_PAIR_LIST_removeAt(me, i);
            return;
        }
    }
}

void SIM_CONNECTION_VECTOR_PAIR_LIST_clear(sim_connection_vector_pair_list_t *me) {
    me->length = 0;
    me->bufferSize = 0;
    free(me->buffer);
    me->buffer = NULL;
}

void SIM_CONNECTION_VECTOR_PAIR_LIST_grow(sim_connection_vector_pair_list_t *me) {
    me->length++;
    if (me->length < me->bufferSize) return; // nothing to do, theres still room

    // otherwise: grow the bih
    me->bufferSize += 5;
    sim_vector_pair_t *newBuffer = realloc(me->buffer, me->bufferSize * sizeof(sim_vector_pair_t));

    if (newBuffer == NULL) {
        printf("Cannot grow point buffer!!! this will go badly\n");
        me->bufferSize -= 5;
        return;
    }

    me->buffer = newBuffer;
}

void SIM_CONNECTION_VECTOR_PAIR_LIST_shrink(sim_connection_vector_pair_list_t *me) {
    me->length--;
    if (me->length > me->bufferSize - 5) return; // nothing to shrink yet

    // otherwise: shrinkificate
    me->bufferSize -= 5;
    sim_vector_pair_t *newBuffer = realloc(me->buffer, me->bufferSize * sizeof(sim_vector_pair_t));

    if (newBuffer == NULL) {
        printf("Cannot shrink point buffer!!! this will go badly\n");
        me->bufferSize += 5;
        return;
    }

    me->buffer = newBuffer;
}