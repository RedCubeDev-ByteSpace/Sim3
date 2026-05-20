//
// Created by redcube on 10/05/2026.
//

#include "GUI.h"
#include "Grid.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Vector2 GUI_WindowSize;
float GRID_zoom;
Vector2 GRID_cameraPos;
Vector2 GRID_cameraOffset;

bool GRID_drawMovementTrails;
float GRID_prevZoom;
Vector2 GRID_prevCameraPos;
Vector2 GRID_prevCameraOffset;

void GRID_init() {
    GRID_cameraPos = (Vector2){0, 0};
    GRID_cameraOffset = (Vector2){0, 0};
    GRID_zoom = 1;
    GRID_drawMovementTrails = false;
}

void GRID_draw() {
    Vector2 currentCameraPosition = GRID_cameraPos;
    currentCameraPosition.x += GRID_cameraOffset.x;
    currentCameraPosition.y += GRID_cameraOffset.y;

    float currentOffsetX = fmod(currentCameraPosition.x, GRID_zoom * GRID_SPACING);
    float currentOffsetY = fmod(currentCameraPosition.y, GRID_zoom * GRID_SPACING);

    Color gridColor = (Color) {
        0,0,0, 128 * (GRID_zoom > 1 ? 1 : GRID_zoom),
    };

    int currentNumOfDotsX = (GUI_WindowSize.x - currentOffsetX) / (GRID_zoom * GRID_SPACING) + 1;
    int currentNumOfDotsY = (GUI_WindowSize.y - currentOffsetY) / (GRID_zoom * GRID_SPACING) + 1;

    float zoomPercentage = logf(GRID_zoom - 1) / logf(MAX_ZOOM - 1);
    float intensity = zoomPercentage * zoomPercentage;

    for (int x = 0; x < currentNumOfDotsX; x++) {
        for (int y = 0; y < currentNumOfDotsY; y++) {

            int currentX = x * GRID_zoom * GRID_SPACING + currentOffsetX;
            int currentY = y * GRID_zoom * GRID_SPACING + currentOffsetY;

            DrawPixel(currentX, currentY, gridColor);

            if (GRID_zoom > 1 && zoomPercentage > 0 && GRID_drawMovementTrails) {
                Vector2 wpos = LIB_screenSpaceToWorldSpace((Vector2){currentX, currentY});
                Vector2 prevSPos = LIB_worldSpaceToScreenSpacePrev(wpos);

                DrawLine(prevSPos.x, prevSPos.y, currentX, currentY, (Color){0,0,0, (int)(zoomPercentage * 100)});

                // if (fabsf(currentOffsetX - prevOffsetX) > 20)
                // printf("posX %d, %d (-%d, +%d), (%f); posY %d, %d (-%d, +%d), (%f); spacing %f, %f; dotsX %d, %d; dotsY %d, %d; iOffs %d, %d\n",
                //     x, prevX, (int)((x + indexOffsetX - 1) * GRID_prevZoom * GRID_SPACING + prevOffsetX), (int)((x + indexOffsetX + 1) * GRID_prevZoom * GRID_SPACING + prevOffsetX),
                //     fabsf(currentOffsetX - prevOffsetX),
                //     y, prevY, (int)((y + indexOffsetY - 1) * GRID_prevZoom * GRID_SPACING + prevOffsetY), (int)((y + indexOffsetY + 1) * GRID_prevZoom * GRID_SPACING + prevOffsetY),
                //     fabsf(currentOffsetY - prevOffsetY),
                //     GRID_zoom * GRID_SPACING, GRID_prevZoom * GRID_SPACING,
                //     currentNumOfDotsX, prevNumOfDotsX, currentNumOfDotsY, prevNumOfDotsY,
                //     indexOffsetX, indexOffsetY);
            }
        }
    }
}
