//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_DRAWABLES_H
#define SIM3_DRAWABLES_H
#define MAX_DRAWABLES 1024
#define LINE_THICKNESS 2

#include <stdbool.h>
#include <stdint.h>

// ---------------------------------------------------------------------------------------------------------------------
// All the things we can draw
typedef enum DRAWABLE_TYPE {
    DRAWABLE_WIRE,
    DRAWABLE_WIRE_BRANCHING_POINT,
    DRAWABLE_FIXED_CONTACT,
    DRAWABLE_CHIP,
} drawable_type_t;

// ---------------------------------------------------------------------------------------------------------------------
// The base struct for all drawable things
typedef struct DRAWABLE {
    drawable_type_t type;
    uint32_t drawableId;
} drawable_t;

// ---------------------------------------------------------------------------------------------------------------------
void DRAWABLES_init();
int DRAWABLES_enqueue(drawable_t *drawable);
void DRAWABLES_drawQueue();
void DRAWABLES_drawSingle(drawable_t *drawable);
void DRAWABLES_unload(drawable_t *drawable);

#endif //SIM3_DRAWABLES_H
