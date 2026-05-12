//
// Created by redcube on 10/05/2026.
//
#include "raylib.h"
#include "GUI.h"
#include "Grid.h"

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