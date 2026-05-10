#include <stdio.h>
#include "raylib.h"


int main(void) {
    printf("Hello, World!\n");

    // start raylib window
    InitWindow(640, 480, "Sim3");

    // main draw loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    // kill the bih
    CloseWindow();

    return 0;
}
