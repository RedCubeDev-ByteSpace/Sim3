//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_BENCH_H
#define SIM3_BENCH_H
#include "raylib.h"

#define NUM_COLORS 10

static Color wireColors[] = {
    (Color){11, 57, 84, 255},
    (Color){8, 126, 139, 255},
    (Color){255, 90, 95, 255},
    (Color){200, 29, 37, 255},
};

extern int currentColor;
extern Vector2 startPos;
extern bool hasStartPos;

void BENCH_init();
void BENCH_process();
void BENCH_draw();

#endif //SIM3_BENCH_H
