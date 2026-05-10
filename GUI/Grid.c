//
// Created by redcube on 10/05/2026.
//

#include "GUI.h"
#include "Grid.h"

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

    int gridSpacing = GRID_SPACING * GRID_zoom;

    for (int x = (int)cameraPosition.x % gridSpacing; x < WINDOW_WIDTH; x += gridSpacing) {
        for (int y = (int)cameraPosition.y % gridSpacing; y < WINDOW_HEIGHT; y += gridSpacing) {
            DrawPixel(x, y, BLACK);
        }
    }
}
