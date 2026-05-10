#include <stdio.h>
#include "raylib.h"
#include "GUI/GUI.h"
#include "GUI/Grid.h"
#include "GUI/Input.h"

int main(void) {
    printf("Hello, World!\n");

    // start raylib window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "sim3");

    // Initialize everything
    GRID_init();
    INPUT_init();

    // main draw loop
    while (!WindowShouldClose()) {

        // -------------------------------------------------------------------------------------------------------------
        // Process all events
        INPUT_process();

        // -------------------------------------------------------------------------------------------------------------
        // Draw the screen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // draw the nav grid
        GRID_draw();

        EndDrawing();
    }

    // kill the bih
    CloseWindow();

    return 0;
}
