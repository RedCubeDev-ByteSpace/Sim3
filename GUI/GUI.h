//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_GUI_H
#define SIM3_GUI_H
#include "raylib.h"

#define INITIAL_WINDOW_WIDTH 800
#define INITIAL_WINDOW_HEIGHT 600

#define TARGET_FPS 60

#define MAX_ZOOM 25
#define MIN_ZOOM 0.3f
#define ZOOM_LERP_CUTOFF 0.001f
#define ZOOM_TRAIL_CUTOFF 0.005f

extern Font GUI_computerModern;
extern Font GUI_computerModern20;
extern Font GUI_computerModern16;
void GUI_init();

float LIB_lerp(float a, float b, float f);
Vector2 LIB_worldSpaceToScreenSpace(Vector2 worldSpace);
Vector2 LIB_worldSpaceToScreenSpacePrev(Vector2 worldSpace);
Vector2 LIB_screenSpaceToWorldSpace(Vector2 screenSpace);
bool LIB_IsVector2InRectangle(Vector2 pos, Rectangle rect);
Vector2 LIB_roundificateToWholePoint(Vector2 pos);
float LIB_getVectorLen(Vector2 vec);

#endif //SIM3_GUI_H
