//
// Created by redcube on 21/05/2026.
//

#include "Menu.h"

#include <raylib.h>

#include "../Grid.h"
#include "../GUI.h"

void MENU_init() {
    GRID_zoom = 2;
}

void MENU_draw() {

    float frameTime = GetFrameTime();
    GRID_cameraPos = (Vector2){
        GRID_cameraPos.x + 20 * frameTime,
        GRID_cameraPos.y + 30 * frameTime
    };

    // Color background = (Color){200, 200, 200, 255};
    Color top = (Color){220, 220, 220, 255};

    Vector2 topLeftCorner = (Vector2){GUI_WindowSize.x / 2 - MENU_WIDTH / 2, GUI_WindowSize.y / 2 - MENU_HEIGHT / 2};

    GRID_draw();

    DrawRectangleRounded((Rectangle){
        topLeftCorner.x, topLeftCorner.y, MENU_WIDTH, MENU_HEIGHT
    }, 0.05f, 10, WHITE);

    DrawRectangleRounded((Rectangle){
        topLeftCorner.x, topLeftCorner.y, MENU_WIDTH, 60
    }, 0.05f, 10, top);
    DrawRectangle(
        topLeftCorner.x, topLeftCorner.y + 50, MENU_WIDTH, 10, top);

    DrawRectangleRoundedLines((Rectangle){
        topLeftCorner.x, topLeftCorner.y, MENU_WIDTH, MENU_HEIGHT
    }, 0.05f, 10, 1.5f, BLACK);

    DrawTextEx(GUI_computerModern55, "sim", (Vector2){topLeftCorner.x + 10, topLeftCorner.y + 10}, 55, 1, BLACK);
    DrawTextEx(GUI_computerModern55, "3", (Vector2){topLeftCorner.x + 80, topLeftCorner.y + 10}, 40, 1, BLACK);

    DrawLineEx((Vector2){
        topLeftCorner.x + MENU_WIDTH * 3 / 4,
        topLeftCorner.y + 70
    }, (Vector2){
        topLeftCorner.x + MENU_WIDTH * 3 / 4,
        topLeftCorner.y + MENU_HEIGHT - 25
    }, 1, BLACK);

    for (int i = 0; i < 6; ++i) {
        DrawTextEx(GUI_computerModern20, "aaaaaaaaaaaaaaaaaaa", (Vector2){topLeftCorner.x + 10, topLeftCorner.y + 80 + i * 30}, 20, 1, BLACK);
    }
}
