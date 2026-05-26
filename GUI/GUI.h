//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_GUI_H
#define SIM3_GUI_H
#include "raylib.h"

#define RAYLIB_COLOR_TO_CLAY_COLOR(color) (Clay_Color) { .r = color.r, .g = color.g, .b = color.b, .a = color.a }


#define INITIAL_WINDOW_WIDTH 800
#define INITIAL_WINDOW_HEIGHT 600

#define VERSION "v1.00"
#define TARGET_FPS 100

#define MAX_ZOOM 25
#define MIN_ZOOM 0.3f
#define ZOOM_LERP_CUTOFF 0.001f
#define ZOOM_TRAIL_CUTOFF 0.005f

typedef enum APP_STATE {
    APP_STATE_MENU, APP_STATE_SIM
} app_state_t;

#define NUM_FONTS 5
typedef enum FONT {
    COMPUTER_MODERN_11,
    COMPUTER_MODERN_16,
    COMPUTER_MODERN_20,
    COMPUTER_MODERN_55,
    COMPUTER_MODERN_150,
} font_t;

extern app_state_t GUI_appState;

extern Font GUI_fonts[5];
void GUI_init();

float LIB_lerp(float a, float b, float f);
Vector2 LIB_worldSpaceToScreenSpace(Vector2 worldSpace);
Vector2 LIB_worldSpaceToScreenSpacePrev(Vector2 worldSpace);
Vector2 LIB_screenSpaceToWorldSpace(Vector2 screenSpace);
bool LIB_IsVector2InRectangle(Vector2 pos, Rectangle rect);
Vector2 LIB_roundificateToWholePoint(Vector2 pos);
Vector2 LIB_roundificateToHalfPoint(Vector2 pos);
float LIB_getVectorLen(Vector2 vec);

#endif //SIM3_GUI_H
