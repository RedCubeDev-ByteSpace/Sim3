//
// Created by redcube on 10/05/2026.
//

#include "GUI.h"
#include "Grid.h"

#include <math.h>

float GRID_zoom;
Vector2 GRID_cameraPos;
Vector2 GRID_cameraOffset;

void GRID_init() {
    GRID_cameraPos = (Vector2){0, 0};
    GRID_cameraOffset = (Vector2){0, 0};
    GRID_zoom = 1;
}

void GRID_draw() {
    Vector2 cameraPosition = GRID_cameraPos;
    cameraPosition.x += GRID_cameraOffset.x;
    cameraPosition.y += GRID_cameraOffset.y;

    float gridSpacing = GRID_SPACING * GRID_zoom;
    Color gridColor = (Color) {
        0,0,0, 128 * (GRID_zoom > 1 ? 1 : GRID_zoom),
    };

    for (float x = fmod(cameraPosition.x, gridSpacing); x < WINDOW_WIDTH; x += gridSpacing) {
        for (float y = fmod(cameraPosition.y, gridSpacing); y < WINDOW_HEIGHT; y += gridSpacing) {
            DrawPixel(x, y, gridColor);
        }
    }
}
