//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_BENCH_H
#define SIM3_BENCH_H
#include "raylib.h"

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

#define NUM_COLORS 9
static Color wireColors[] = {
    (Color){ 84, 110, 122, 255}, // Slate Gray    #546E7A
    (Color){198,  40,  40, 255}, // Deep Red      #C62828
    (Color){239, 108,   0, 255}, // Burnt Orange  #EF6C00
    (Color){249, 168,  37, 255}, // Golden Yellow #F9A825
    (Color){ 46, 125,  50, 255}, // Emerald Green #2E7D32
    (Color){  0, 137, 123, 255}, // Teal          #00897B
    (Color){ 21, 101, 192, 255}, // Royal Blue    #1565C0
    (Color){ 57,  73, 171, 255}, // Indigo        #3949AB
    (Color){142,  36, 170, 255}, // Violet        #8E24AA
};

extern bench_working_mode_t BENCH_benchMode;

void BENCH_init();
void BENCH_process();
void BENCH_draw();
void BENCH_layout();

#endif //SIM3_BENCH_H
