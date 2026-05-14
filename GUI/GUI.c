//
// Created by redcube on 10/05/2026.
//
#include "raylib.h"
#include "GUI.h"

#include <stddef.h>

#include "Grid.h"

Font GUI_computerModern;
void GUI_init() {
    GUI_computerModern = LoadFontEx("../Resources/cmunbx.ttf", 150, NULL, 0);
    SetTextureFilter(GUI_computerModern.texture, TEXTURE_FILTER_BILINEAR);
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