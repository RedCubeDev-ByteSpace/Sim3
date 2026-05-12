//
// Created by redcube on 12/05/2026.
//
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "Connection.h"

void SIM_CONNECTION_POINT_LIST_init(sim_connection_point_list_t *me) {
    me->buffer = NULL;
    me->bufferSize = 0;
    me->length = 0;
}

void SIM_CONNECTION_POINT_LIST_append(sim_connection_point_list_t *me, Vector2 newPoint) {

    // make sure this point isnt the same as the previous one, because that would be stupid
    if (me->length > 0) {
        Vector2 oldPoint = me->buffer[me->length - 1];
        if (oldPoint.x == newPoint.x && oldPoint.y == newPoint.y) {
            return; // this point is redundant
        }
    }

    // grow the list and add this point
    SIM_CONNECTION_POINT_LIST_grow(me);
    me->buffer[me->length-1] = newPoint;
}

void SIM_CONNECTION_POINT_LIST_insertAt(sim_connection_point_list_t *me, Vector2 newPoint, uint32_t index) {
    if (index >= me->length) return; // huh? what? what are you even trying to do?

    // make sure this point isnt the same as the previous one, because that would be stupid
    if (index > 0) {
        Vector2 oldPoint = me->buffer[index - 1];
        if (oldPoint.x == newPoint.x && oldPoint.y == newPoint.y) {
            return; // this point is redundant
        }
    }

    // grow the list
    SIM_CONNECTION_POINT_LIST_grow(me);

    // move everything after it back
    for (uint32_t i = me->length-1; i > index; i--) {
        me->buffer[i] = me->buffer[i - 1];
    }

    // slot it in
    me->buffer[index] = newPoint;
}
void SIM_CONNECTION_POINT_LIST_insertAfter(sim_connection_point_list_t *me, Vector2 oldPoint, Vector2 newPoint) {
    // no idea if ill actually ever need this
}

void SIM_CONNECTION_POINT_LIST_removeAt(sim_connection_point_list_t *me, uint32_t index) {
    if (index >= me->length) return; // nah bruv

    // move everything after the index back
    for (int i = index; i < me->length-1; ++i) {
        me->buffer[i] = me->buffer[i + 1];
    }

    // shrinkificate
    SIM_CONNECTION_POINT_LIST_shrink(me);
}

void SIM_CONNECTION_POINT_LIST_remove(sim_connection_point_list_t *me, Vector2 oldPoint) {
    // no idea if ill actually ever need this
}

void SIM_CONNECTION_POINT_LIST_clear(sim_connection_point_list_t *me) {
    me->length = 0;
    me->bufferSize = 0;
    free(me->buffer);
    me->buffer = NULL;
}

void SIM_CONNECTION_POINT_LIST_grow(sim_connection_point_list_t *me) {
    me->length++;
    if (me->length < me->bufferSize) return; // nothing to do, theres still room

    // otherwise: grow the bih
    me->bufferSize += 5;
    Vector2 *newBuffer = realloc(me->buffer, me->bufferSize * sizeof(Vector2));

    if (newBuffer == NULL) {
        printf("Cannot grow point buffer!!! this will go badly\n");
        me->bufferSize -= 5;
        return;
    }

    me->buffer = newBuffer;
}

void SIM_CONNECTION_POINT_LIST_shrink(sim_connection_point_list_t *me) {
    me->length--;
    if (me->length > me->bufferSize - 5) return; // nothing to shrink yet

    // otherwise: shrinkificate
    me->bufferSize -= 5;
    Vector2 *newBuffer = realloc(me->buffer, me->bufferSize * sizeof(Vector2));

    if (newBuffer == NULL) {
        printf("Cannot shrink point buffer!!! this will go badly\n");
        me->bufferSize += 5;
        return;
    }

    me->buffer = newBuffer;
}