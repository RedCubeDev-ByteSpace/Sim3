//
// Created by redcube on 26/05/2026.
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AppData.h"

void APPDATA_RECENT_RECORD_LIST_init(appdata_recent_record_list_t *me) {
    me->buffer = NULL;
    me->bufferSize = 0;
    me->length = 0;
}
void APPDATA_RECENT_RECORD_LIST_append(appdata_recent_record_list_t *me, appdata_recent_record_t newRecord) {
    APPDATA_RECENT_RECORD_LIST_grow(me);
    me->buffer[me->length - 1] = newRecord;
}
void APPDATA_RECENT_RECORD_LIST_insertAt(appdata_recent_record_list_t *me, appdata_recent_record_t newRecord, uint32_t index) {
    APPDATA_RECENT_RECORD_LIST_grow(me);

    for (int i = me->length-1; i > index; --i) {
        me->buffer[i] = me->buffer[i-1];
    }

    me->buffer[index] = newRecord;
}
void APPDATA_RECENT_RECORD_LIST_remove(appdata_recent_record_list_t *me, char *path) {
    for (int i = 0; i < me->length; ++i) {
        if (strcmp(me->buffer[i].path, path) == 0) {
            APPDATA_RECENT_RECORD_LIST_removeAt(me, i);
            return;
        }
    }
}
void APPDATA_RECENT_RECORD_LIST_removeAt(appdata_recent_record_list_t *me, uint32_t index) {
    for (int i = index; i < me->length-1; ++i) {
        me->buffer[i] = me->buffer[i+1];
    }
    APPDATA_RECENT_RECORD_LIST_shrink(me);
}
void APPDATA_RECENT_RECORD_LIST_clear(appdata_recent_record_list_t *me) {
    free(me->buffer);
    me->buffer = NULL;
    me->bufferSize = 0;
    me->length = 0;
}
void APPDATA_RECENT_RECORD_LIST_grow(appdata_recent_record_list_t *me) {
    me->length++;
    if (me->length < me->bufferSize) return;

    me->bufferSize += 5;
    appdata_recent_record_t *newBuffer = realloc(me->buffer, me->bufferSize*sizeof(appdata_recent_record_t));
    if (newBuffer == NULL) {
        printf("APPDATA_RECENT_RECORD_LIST_grow: cannot grow recents buffer! oh no\n");
        me->bufferSize -= 5;
        return;
    }

    me->buffer = newBuffer;
}
void APPDATA_RECENT_RECORD_LIST_shrink(appdata_recent_record_list_t *me) {
    me->length--;
    if (me->length >= me->bufferSize - 5) return;

    me->bufferSize -= 5;
    appdata_recent_record_t *newBuffer = realloc(me->buffer, me->bufferSize*sizeof(appdata_recent_record_t));
    if (newBuffer == NULL) {
        printf("APPDATA_RECENT_RECORD_LIST_shrink: cannot shrink recents buffer! oh no\n");
        me->bufferSize += 5;
        return;
    }

    me->buffer = newBuffer;
}