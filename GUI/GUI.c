//
// Created by redcube on 10/05/2026.
//
#include "raylib.h"
#include "GUI.h"

#include <math.h>
#include <stddef.h>

#include "Grid.h"

app_state_t GUI_appState;
Font GUI_computerModern;
Font GUI_computerModern55;
Font GUI_computerModern20;
Font GUI_computerModern16;
Font GUI_computerModern11;

void GUI_init() {
    GUI_appState = APP_STATE_SIM;

    GUI_computerModern = LoadFontEx("../Resources/cmunbx.ttf", 150, NULL, 0);
    GUI_computerModern55 = LoadFontEx("../Resources/cmunbx.ttf", 55, NULL, 0);
    GUI_computerModern20 = LoadFontEx("../Resources/cmunbx.ttf", 20, NULL, 0);
    GUI_computerModern16 = LoadFontEx("../Resources/cmunbx.ttf", 16, NULL, 0);
    GUI_computerModern11 = LoadFontEx("../Resources/cmunbx.ttf", 11, NULL, 0);
    SetTextureFilter(GUI_computerModern.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(GUI_computerModern55.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(GUI_computerModern20.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(GUI_computerModern16.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(GUI_computerModern11.texture, TEXTURE_FILTER_BILINEAR);
}

float LIB_lerp(float a, float b, float f)
{
    return (a * (1.0 - f)) + (b * f);
}

Vector2 LIB_worldSpaceToScreenSpace(Vector2 worldSpace) {
    return (Vector2){
        (worldSpace.x * GRID_SPACING * GRID_zoom + GRID_cameraPos.x + GRID_cameraOffset.x),
        (worldSpace.y * GRID_SPACING * GRID_zoom + GRID_cameraPos.y + GRID_cameraOffset.y)
    };
}

Vector2 LIB_worldSpaceToScreenSpacePrev(Vector2 worldSpace) {
    return (Vector2){
        (worldSpace.x * GRID_SPACING * GRID_prevZoom + GRID_prevCameraPos.x + GRID_prevCameraOffset.x),
        (worldSpace.y * GRID_SPACING * GRID_prevZoom + GRID_prevCameraPos.y + GRID_prevCameraOffset.y)
    };
}

Vector2 LIB_screenSpaceToWorldSpace(Vector2 screenSpace) {
    return (Vector2){
        (screenSpace.x - GRID_cameraPos.x - GRID_cameraOffset.x) / (GRID_SPACING * GRID_zoom),
        (screenSpace.y - GRID_cameraPos.y - GRID_cameraOffset.y) / (GRID_SPACING * GRID_zoom)
    };
}

bool LIB_IsVector2InRectangle(Vector2 pos, Rectangle rect) {
    if (pos.x >= rect.x && pos.y >= rect.y && pos.x <= rect.x + rect.width && pos.y <= rect.y + rect.height) {
        return true;
    }

    return false;
}

// this is required because C will always round towards zero
// it stinkificates.
Vector2 LIB_roundificateToWholePoint(Vector2 pos) {
    if (pos.x > 0) pos.x = (int)(pos.x + 0.5f);
    else if (pos.x < 0) pos.x = (int)(pos.x - 0.5f);
    if (pos.y > 0) pos.y = (int)(pos.y + 0.5f);
    else if (pos.y < 0) pos.y = (int)(pos.y - 0.5f);

    return pos;
}

Vector2 LIB_roundificateToHalfPoint(Vector2 pos) {
    Vector2 halfPos = LIB_roundificateToWholePoint((Vector2){pos.x * 2, pos.y * 2});
    return (Vector2){ halfPos.x / 2, halfPos.y / 2 };
}

float LIB_getVectorLen(Vector2 vec) {
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}