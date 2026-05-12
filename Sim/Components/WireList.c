//
// Created by redcube on 12/05/2026.
//
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "Connection.h"

void SIM_CONNECTION_DRAWABLE_LIST_init(sim_connection_drawable_list_t *me) {
    me->buffer = NULL;
    me->length = 0;
    me->bufferSize = 0;
}

void SIM_CONNECTION_DRAWABLE_LIST_add(sim_connection_drawable_list_t *me, drawable_t *newWire) {
    SIM_CONNECTION_DRAWABLE_LIST_grow(me);
    me->buffer[me->length-1] = newWire;
}

void SIM_CONNECTION_DRAWABLE_LIST_grow(sim_connection_drawable_list_t *me) {
    me->length++;
    if (me->length < me->bufferSize) return;

    me->bufferSize += 5;
    drawable_t **newBuffer = realloc(me->buffer, me->bufferSize * sizeof(drw_wire_t*));

    if (newBuffer == NULL) {
        me->bufferSize -= 5;
        printf("Bro this went bad (connection drawable list)");
    }

    me->buffer = newBuffer;
}

void SIM_CONNECTION_DRAWABLE_LIST_clear(sim_connection_drawable_list_t *me) {
    me->length = 0;
    me->bufferSize = 0;
    free(me->buffer);
    me->buffer = NULL;
}