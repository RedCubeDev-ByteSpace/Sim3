//
// Created by redcube on 10/05/2026.
//
#include <stddef.h>
#include "Drawables.h"

#include <stdlib.h>

#include "Drawables/Chip.h"
#include "Drawables/FixedContact.h"
#include "Drawables/LED.h"
#include "Drawables/Wire.h"
#include "Drawables/WireBranchingPoint.h"
#include "../Sim/SimulationResources.h"


drawable_t *drawables[MAX_DRAWABLES];

void DRAWABLES_init() {
    for (int i = 0; i < MAX_DRAWABLES; i++) {
        drawables[i] = NULL;
    }
}

int DRAWABLES_enqueue(drawable_t *drawable) {
    if (drawable == NULL) return -1; // bro what do you expect this to do

    // look through the buffer for a free space
    for (int i = 0; i < MAX_DRAWABLES; i++) {
        if (drawables[i] != NULL) continue;

        // found one -> use it
        drawables[i] = (drawable_t*)drawable;
        drawable->drawableId = i; // remember where this is for cleanup later
        return i;
    }

    // sorry m8
    return -1;
}

void DRAWABLES_drawQueue() {
    for (int i = 0; i < MAX_DRAWABLES; i++) {
        if (drawables[i] == NULL) continue;
        DRAWABLES_drawSingle(drawables[i]);
    }
}

void DRAWABLES_drawSingle(drawable_t *drawable) {

    switch (drawable->type) {

        case DRAWABLE_WIRE:
            DRAWABLES_WIRE_draw((drw_wire_t*)drawable);
            break;

        case DRAWABLE_WIRE_BRANCHING_POINT:
            DRAWABLES_WIRE_BRANCHING_POINT_draw((drw_wire_branching_point_t*)drawable);
            break;

        case DRAWABLE_FIXED_CONTACT:
            DRAWABLES_FIXED_CONTACT_draw((drw_fixed_contact_t*)drawable);
            break;

        case DRAWABLE_CHIP:
            DRAWABLES_CHIP_draw((drw_chip_t*)drawable);
            break;

        case DRAWABLE_LED:
            DRAWABLES_LED_draw((drw_led_t*)drawable);
            break;

    }
}

void DRAWABLES_unload(drawable_t *drawable) {

    drawables[drawable->drawableId] = NULL;
    free(drawable);

    // switch (drawable->type) {
    //     case DRAWABLE_WIRE:
    //         drawables[drawable->drawableId] = NULL;
    //         free(drawable);
    //         break;
    // }
}