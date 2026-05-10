#include <stdio.h>
#include "raylib.h"
#include "GUI/Bench.h"
#include "GUI/Drawables.h"
#include "GUI/GUI.h"
#include "GUI/Grid.h"
#include "GUI/Input.h"

int main(void) {
    printf("Hello, World!\n");

    // start raylib window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "sim3");
    SetTargetFPS(TARGET_FPS);

    // Initialize everything
    GRID_init();
    INPUT_init();
    DRAWABLES_init();
    BENCH_init();

    // main draw loop
    while (!WindowShouldClose()) {

        // -------------------------------------------------------------------------------------------------------------
        // Process all events
        INPUT_process();
        BENCH_process();

        // -------------------------------------------------------------------------------------------------------------
        // Draw the screen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // draw the nav grid
        GRID_draw();

        // draw all components
        DRAWABLES_drawQueue();
        BENCH_draw();

        EndDrawing();
    }

    // kill the bih
    CloseWindow();

    return 0;
}
