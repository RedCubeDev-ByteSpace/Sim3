//
// Created by redcube on 10/05/2026.
//

#include "raylib.h"
#include "Input.h"

#include <math.h>
#include <stdio.h>

#include "Grid.h"

Vector2 dragBegin;
bool isDragging;

void INPUT_init() {
    dragBegin = (Vector2){0, 0};
    isDragging = false;
}

void INPUT_process() {
    // -----------------------------------------------------------------------------------------------------------------
    // GRID DRAGGING
    // -------------

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
        dragBegin = GetMousePosition();
        isDragging = true;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
        isDragging = false;

        // apply the offset to the actual real camera position
        GRID_cameraPos.x += GRID_cameraOffset.x;
        GRID_cameraPos.y += GRID_cameraOffset.y;
        GRID_cameraOffset = (Vector2){0, 0};
    }

    if (isDragging) {
        Vector2 currentMousePos = GetMousePosition();
        GRID_cameraOffset = (Vector2) {
            currentMousePos.x - dragBegin.x,
            currentMousePos.y - dragBegin.y
        };
    }

    // -----------------------------------------------------------------------------------------------------------------
    // ZOOMIFICATION
    // -------------

    float wheel = GetMouseWheelMove();
    float steps = ceilf(fabsf(wheel));

    // zoom in
    if (GetMouseWheelMove() > 0) {
        GRID_zoom *= (steps+1);
    }

    // zoom out
    if (GetMouseWheelMove() < 0) {
        GRID_zoom /= (steps+1);
    }

    // -----------------------------------------------------------------------------------------------------------------
}