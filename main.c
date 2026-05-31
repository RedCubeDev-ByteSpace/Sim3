#include <stdio.h>
#include "raylib.h"
#include "GUI/Bench.h"
#include "GUI/Drawables.h"
#include "GUI/GUI.h"
#include "GUI/Grid.h"
#include "GUI/Input.h"
#include "GUI/Statusbar.h"
#include "GUI/Menu/Menu.h"
#include "Sim/SimulationResources.h"
#include "Sim/SimulationSpace.h"
#include "Sim/SaveAndLoad.h"
#include "GUI/Lib/clay_renderer_raylib.c"
#include "Sim/Json/AppData.h"

int main(void) {
    printf("HELLORLD!\n");

    // ----- YOU ARE ENTERING THE CLAY ZONE -----
    // Initialize Clay!
    uint64_t clayMinMemorySize = Clay_MinMemorySize();
    Clay_Arena clayArena = Clay_CreateArenaWithCapacityAndMemory(
        clayMinMemorySize,
        malloc(clayMinMemorySize)
    );
    Clay_Initialize(
        clayArena,
        (Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenWidth()},
        (Clay_ErrorHandler){0}
    );

    // create a raylib window
    Clay_Raylib_Initialize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "sim3", FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    SetExitKey(KEY_NULL);
    SetTargetFPS(TARGET_FPS);

    // load all the fonts used by the application
    GUI_init();

    // Hook up clays text measuring to raylib
    Clay_SetMeasureTextFunction(Raylib_MeasureText, GUI_fonts);

    // Initialize everything
    APPDATA_init();
    SIMRES_init();
    GUI_init();
    GRID_init();
    INPUT_init();
    DRAWABLES_init();
    BENCH_init();
    SIMSPACE_init();
    SAVE_AND_LOAD_init();
    MENU_init();

    // main draw loop
    while (!WindowShouldClose()) {

        // hihi, haha, window size
        GUI_WindowSize = (Vector2){ GetScreenWidth(), GetScreenHeight() };

        // -------------------------------------------------------------------------------------------------------------
        // Update Clay with everything thats been going on

        float mouseWheelY = GetMouseWheelMove();
        Clay_Vector2 mousePosition = RAYLIB_VECTOR2_TO_CLAY_VECTOR2(GetMousePosition());
        Clay_SetPointerState(mousePosition, IsMouseButtonDown(0));
        Clay_SetLayoutDimensions((Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() });
        Clay_UpdateScrollContainers(false, (Clay_Vector2){0, mouseWheelY}, GetFrameTime());

        // -------------------------------------------------------------------------------------------------------------
        // Process all events
        if (GUI_appState == APP_STATE_SIM) {
            INPUT_process();
            BENCH_process();
            SIMSPACE_process();
            STATUSBAR_process();
        }

        // -------------------------------------------------------------------------------------------------------------
        // Layout the GUI
        Clay_BeginLayout();

        if (GUI_appState == APP_STATE_SIM) {

            // layout the workbench
            BENCH_layout();
        }
        else {

            // layout the main menu
            MENU_layout();
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout(GetFrameTime());

        // -------------------------------------------------------------------------------------------------------------
        // Draw the screen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // render all the raylib native things
        // -> mainly the simulation space
        if (GUI_appState == APP_STATE_SIM) {

            // draw the nav grid
            GRID_draw();

            // draw all components
            DRAWABLES_drawQueue();

            // draw the bench cursors
            BENCH_draw();
        }
        else {

            // draw the menu background
            MENU_draw();
        }

        // render the clay ui on top
        Clay_Raylib_Render(renderCommands, GUI_fonts);

        EndDrawing();
    }

    // kill the bih
    CloseWindow();

    // the lion does not concern himself with freeing memory
    return 0;
}
