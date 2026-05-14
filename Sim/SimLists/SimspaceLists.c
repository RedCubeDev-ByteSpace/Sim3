//
// Created by redcube on 14/05/2026.
//
#include "SimspaceLists.h"

#include <stdio.h>
#include <stdlib.h>

void SIM_COMP_LIST_init(sim_comp_list_t *me) {
    me->buffer = NULL;
    me->bufferSize = 0;
    me->length = 0;
}

void SIM_COMP_LIST_append(sim_comp_list_t *me, void *comp) {
    SIM_COMP_LIST_grow(me); // just one more slot please i swear
    me->buffer[me->length - 1] = comp;
}

void SIM_COMP_LIST_insertAt(sim_comp_list_t *me, void *comp, uint32_t index) {
    SIM_COMP_LIST_grow(me); // ANOTHER ONE??!!?

    // move everything after the index back
    for (int i = me->length-1; i > index; i--) {
        me->buffer[i] = me->buffer[i-1];
    }

    // slot this one right in
    me->buffer[index] = comp;
}

void SIM_COMP_LIST_removeAt(sim_comp_list_t *me, uint32_t index) {

    // move everything back into the index that should be removed
    for (int i = index; i < me->length - 1; i++) {
        me->buffer[i] = me->buffer[i+1];
    }

    SIM_COMP_LIST_shrink(me); // we shrinkificatin it
}

void SIM_COMP_LIST_removeRef(sim_comp_list_t *me, void *comp) {

    // find the index of this component
    for (int i = 0; i < me->length; ++i) {
        if (me->buffer[i] == comp) {

            // nuke it
            SIM_COMP_LIST_removeAt(me, i);
            return;
        }
    }
}

void SIM_COMP_LIST_grow(sim_comp_list_t *me) {
    me->length++;
    if (me->length <= me->bufferSize) return; // we good

    // otherwise: REALLOCATEEEE
    me->bufferSize += 5;
    void **newBuffer = realloc(me->buffer, me->bufferSize * sizeof(void *));

    if (newBuffer == NULL) {
        printf("SIM_COMP_LIST_grow: everything has gone to shit!");
        me->bufferSize -= 5;
        return;
    }

    me->buffer = newBuffer;
}

void SIM_COMP_LIST_shrink(sim_comp_list_t *me) {
    me->length--;
    if (me->length >= me->bufferSize - 5) return; // we good

    // otherwise: we bad
    me->bufferSize -= 5;
    void **newBuffer = realloc(me->buffer, me->bufferSize * sizeof(void *));

    if (newBuffer == NULL) {
        printf("SIM_COMP_LIST_shrink: everything has gone to shit!");
        me->bufferSize += 5;
        return;
    }

    me->buffer = newBuffer;
}

void SIM_COMP_LIST_clear(sim_comp_list_t *me) {
    free(me->buffer);
    me->buffer = NULL;
    me->bufferSize = 0;
    me->length = 0;
}

// ---------------------------------------------------------------------------------------------------------------------
// CONNECTIONS
void SIM_COMP_LIST_appendConnection(sim_connection_list_t *me, sim_connection_t *comp) {
    SIM_COMP_LIST_append(me, comp);
}
void SIM_COMP_LIST_insertConnectionAt(sim_connection_list_t *me, sim_connection_t *comp, uint32_t index) {
    SIM_COMP_LIST_insertAt(me, comp, index);
}
void SIM_COMP_LIST_removeConnectionAt(sim_connection_list_t *me, uint32_t index) {
    SIM_COMP_LIST_removeAt(me, index);
}
void SIM_COMP_LIST_removeConnectionRef(sim_connection_list_t *me, sim_connection_t *comp) {
    SIM_COMP_LIST_removeRef(me, comp);
}

// ---------------------------------------------------------------------------------------------------------------------
// FIXED CONTACTS
void SIM_COMP_LIST_appendFixedContact(sim_fixedcontact_list_t *me, sim_fixed_contact_t *comp){
    SIM_COMP_LIST_append(me, comp);
}
void SIM_COMP_LIST_insertFixedContactAt(sim_fixedcontact_list_t *me, sim_fixed_contact_t *comp, uint32_t index){
    SIM_COMP_LIST_insertAt(me, comp, index);
}
void SIM_COMP_LIST_removeFixedContactAt(sim_fixedcontact_list_t *me, uint32_t index){
    SIM_COMP_LIST_removeAt(me, index);
}
void SIM_COMP_LIST_removeFixedContactRef(sim_fixedcontact_list_t *me, sim_fixed_contact_t *comp){
    SIM_COMP_LIST_removeRef(me, comp);
}

// ---------------------------------------------------------------------------------------------------------------------
// CONNECTION POINT
void SIM_COMP_LIST_appendConnectionPoint(sim_connectionpoint_list_t *me, sim_connection_point_t *comp){
    SIM_COMP_LIST_append(me, comp);
}
void SIM_COMP_LIST_insertConnectionPointAt(sim_connectionpoint_list_t *me, sim_connection_point_t *comp, uint32_t index){
    SIM_COMP_LIST_insertAt(me, comp, index);
}
void SIM_COMP_LIST_removeConnectionPointAt(sim_connectionpoint_list_t *me, uint32_t index){
    SIM_COMP_LIST_removeAt(me, index);
}
void SIM_COMP_LIST_removeConnectionPointRef(sim_connectionpoint_list_t *me, sim_connection_point_t *comp){
    SIM_COMP_LIST_removeRef(me, comp);
}

// ---------------------------------------------------------------------------------------------------------------------
// CHIP
void SIM_COMP_LIST_appendChip(sim_chip_list_t *me, sim_chip_t *comp){
    SIM_COMP_LIST_append(me, comp);
}
void SIM_COMP_LIST_insertChipAt(sim_chip_list_t *me, sim_chip_t *comp, uint32_t index){
    SIM_COMP_LIST_insertAt(me, comp, index);
}
void SIM_COMP_LIST_removeChipAt(sim_chip_list_t *me, uint32_t index){
    SIM_COMP_LIST_removeAt(me, index);
}
void SIM_COMP_LIST_removeChipRef(sim_chip_list_t *me, sim_chip_t *comp){
    SIM_COMP_LIST_removeRef(me, comp);
}