//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_BENCH_H
#define SIM3_BENCH_H
#include "raylib.h"

#define NUM_COLORS 10
#define CURSOR_WIDTH 10
#define CURSOR_THICKNESS 1.5f

typedef enum BENCH_WORKING_MODE {
    IDLE,
    MOVING_POINT,
    DRAWING_WIRE,
    PLACE_FIXED_CONTACT,
    PLACE_LED,
    PLACE_CHIP,
    DELETION,
} bench_working_mode_t;

static Color wireColors[] = {
    (Color){11, 57, 84, 255},
    (Color){8, 126, 139, 255},
    (Color){255, 90, 95, 255},
    (Color){200, 29, 37, 255},
};

extern bench_working_mode_t BENCH_benchMode;

void BENCH_init();
void BENCH_process();
void BENCH_draw();

#endif //SIM3_BENCH_H
