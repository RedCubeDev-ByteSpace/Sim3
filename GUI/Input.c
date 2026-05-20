//
// Created by redcube on 10/05/2026.
//

#include "raylib.h"
#include "Input.h"

#include <math.h>
#include <stdio.h>

#include "Grid.h"
#include "GUI.h"

float targetZoom;
Vector2 dragBegin;
bool isDragging;

void INPUT_init() {
    dragBegin = (Vector2){0, 0};
    isDragging = false;
    targetZoom = GRID_zoom;
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

        // stop any trail drawing
        GRID_drawMovementTrails = false;
        GRID_prevCameraOffset = GRID_cameraOffset;
        GRID_prevCameraPos = GRID_cameraPos;
    }

    if (isDragging) {
        Vector2 currentMousePos = GetMousePosition();

        // draw the difference between where points were the last frame and this frame
        GRID_drawMovementTrails = true;
        GRID_prevCameraOffset = GRID_cameraOffset;

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
        if (targetZoom < MAX_ZOOM)
            targetZoom *= (1 + steps/2);
    }

    // zoom out
    if (GetMouseWheelMove() < 0) {
        if (targetZoom > MIN_ZOOM)
            targetZoom /= (1 + steps/2);
    }

    // smooth zoom lerpin'
    if (targetZoom != GRID_zoom) {

        // draw some connection lines for zooming to be less confusing
        GRID_drawMovementTrails = true;
        GRID_prevCameraPos = GRID_cameraPos;
        GRID_prevZoom = GRID_zoom;


        // hihi, haha, some vector math for nice zooming :)

        // convert the mouse pos into world space with the current zoom
        Vector2 mousePos = GetMousePosition();
        Vector2 mousePosWorldA = LIB_screenSpaceToWorldSpace(mousePos);

        // approach targetZoom
        GRID_zoom = LIB_lerp(GRID_zoom, targetZoom, 0.2f);

        // convert the mouse pos into world space with the new zoom
        Vector2 mousePosWorldB = LIB_screenSpaceToWorldSpace(mousePos);

        // convert both positions back into screen space
        Vector2 mousePosScreenA = LIB_worldSpaceToScreenSpace(mousePosWorldA);
        Vector2 mousePosScreenB = LIB_worldSpaceToScreenSpace(mousePosWorldB);

        // apply the difference to the screen space camera pos to compensate
        GRID_cameraPos.x += mousePosScreenB.x - mousePosScreenA.x;
        GRID_cameraPos.y += mousePosScreenB.y - mousePosScreenA.y;

        // end this madness when were close enough
        if (fabsf(GRID_zoom - targetZoom) < ZOOM_LERP_CUTOFF) {
            GRID_zoom = targetZoom;
        }

        if (GRID_drawMovementTrails && fabsf(GRID_zoom - targetZoom) < ZOOM_TRAIL_CUTOFF) {
            GRID_drawMovementTrails = false;
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
}