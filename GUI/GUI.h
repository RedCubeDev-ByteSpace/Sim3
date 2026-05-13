//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_GUI_H
#define SIM3_GUI_H
#include "raylib.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

#define TARGET_FPS 60

#define MAX_ZOOM 25
#define MIN_ZOOM 0.3f
#define ZOOM_LERP_CUTOFF 0.001f

extern Font GUI_computerModern;
void GUI_init();

float LIB_lerp(float a, float b, float f);
Vector2 LIB_worldSpaceToScreenSpace(Vector2 worldSpace);
Vector2 LIB_screenSpaceToWorldSpace(Vector2 screenSpace);
bool LIB_IsVector2InRectangle(Vector2 pos, Rectangle rect);

#endif //SIM3_GUI_H
