#include <stdio.h>
#include "raylib.h"
#include "GUI/Bench.h"
#include "GUI/Drawables.h"
#include "GUI/GUI.h"
#include "GUI/Grid.h"
#include "GUI/Input.h"
#include "Sim/SimulationResources.h"
#include "Sim/SimulationSpace.h"

int main(void) {
    printf("HELLORLD!\n");

    // start raylib window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "sim3");
    SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);

    // Initialize everything
    SIMRES_init();
    GUI_init();
    GRID_init();
    INPUT_init();
    DRAWABLES_init();
    BENCH_init();
    SIMSPACE_init();

    // main draw loop
    while (!WindowShouldClose()) {

        // hihi, haha, window size
        GUI_WindowSize = (Vector2){ GetScreenWidth(), GetScreenHeight() };

        // -------------------------------------------------------------------------------------------------------------
        // Process all events
        INPUT_process();
        BENCH_process();
        SIMSPACE_process();

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

    // the lion does not concern himself with freeing memory
    return 0;
}
