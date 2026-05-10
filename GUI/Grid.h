//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_GRID_H
#define SIM3_GRID_H
#include "raylib.h"

#define GRID_SPACING 10

void GRID_init();
void GRID_draw();

extern float GRID_zoom;
extern Vector2 GRID_cameraPos;
extern Vector2 GRID_cameraOffset;

#endif //SIM3_GRID_H
