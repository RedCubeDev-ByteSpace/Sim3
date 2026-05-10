//
// Created by redcube on 10/05/2026.
//
#include <stddef.h>
#include "Drawables.h"

#include <stdlib.h>

#include "Drawables/Wire.h"


drawable_t *drawables[MAX_DRAWABLES];

void DRAWABLES_init() {
    for (int i = 0; i < MAX_DRAWABLES; i++) {
        drawables[i] = NULL;
    }

    drw_wire_t *wire;
    for (int i = 0; i < 10; ++i) {
        wire = malloc(sizeof(drw_wire_t));
        DRAWABLES_WIRE_init(wire, (Vector2){5 + i,5}, (Vector2){5 + i,10}, ORANGE);
        DRAWABLES_enqueue((drawable_t*)wire);

        wire = malloc(sizeof(drw_wire_t));
        DRAWABLES_WIRE_init(wire, (Vector2){5 + i,10}, (Vector2){7 + i,20}, ORANGE);
        DRAWABLES_enqueue((drawable_t*)wire);

        wire = malloc(sizeof(drw_wire_t));
        DRAWABLES_WIRE_init(wire, (Vector2){7 + i,20}, (Vector2){7 + i,30}, ORANGE);
        DRAWABLES_enqueue((drawable_t*)wire);
    }

}

int DRAWABLES_enqueue(drawable_t *drawable) {

    // look through the buffer for a free space
    for (int i = 0; i < MAX_DRAWABLES; i++) {
        if (drawables[i] != NULL) continue;

        // found one -> use it
        drawables[i] = (drawable_t*)drawable;
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

    }
}