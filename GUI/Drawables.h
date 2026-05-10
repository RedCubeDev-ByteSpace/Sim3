//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_DRAWABLES_H
#define SIM3_DRAWABLES_H
#define MAX_DRAWABLES 100
#define LINE_THICKNESS 2
#define COLOR_ACTIVE (Color){255,0,0,255}
#include <stdbool.h>

// ---------------------------------------------------------------------------------------------------------------------
// All the things we can draw
typedef enum DRAWABLE_TYPE {
    DRAWABLE_WIRE
} drawable_type_t;

// ---------------------------------------------------------------------------------------------------------------------
// The base struct for all drawable things
typedef struct DRAWABLE {
    drawable_type_t type;
} drawable_t;

// ---------------------------------------------------------------------------------------------------------------------
void DRAWABLES_init();
int DRAWABLES_enqueue(drawable_t *drawable);
void DRAWABLES_drawQueue();
void DRAWABLES_drawSingle(drawable_t *drawable);

#endif //SIM3_DRAWABLES_H
