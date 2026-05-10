//
// Created by redcube on 10/05/2026.
//
#include "Bench.h"

#include <math.h>
#include <stdlib.h>

#include "GUI.h"
#include "Drawables/Wire.h"

float cursorAngle;
int currentColor;
Vector2 startPos;
bool hasStartPos;

void BENCH_init() {
    currentColor = 0;
    hasStartPos = false;
    cursorAngle = 0;
}

void BENCH_process() {
    cursorAngle += GetFrameTime();
    cursorAngle = fmod(cursorAngle, 360);

    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        if (!hasStartPos) {
            currentColor = (currentColor + 1) % 10;
        } else {
            Vector2 endPos = LIB_screenSpaceToWorldSpace(GetMousePosition());
            endPos.x = (int)(endPos.x + 0.5f);
            endPos.y = (int)(endPos.y + 0.5f);

            drw_wire_t *wire = malloc(sizeof(drw_wire_t));
            DRAWABLES_WIRE_init(wire, startPos, endPos, wireColors[currentColor]);
            //wire->active = true;
            DRAWABLES_enqueue((drawable_t*)wire);

            startPos = endPos;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (!hasStartPos) {
            startPos = LIB_screenSpaceToWorldSpace(GetMousePosition());
            startPos.x = (int)(startPos.x + 0.5f);
            startPos.y = (int)(startPos.y + 0.5f);
            hasStartPos = true;
        } else {
            hasStartPos = false;
            Vector2 endPos = LIB_screenSpaceToWorldSpace(GetMousePosition());
            endPos.x = (int)(endPos.x + 0.5f);
            endPos.y = (int)(endPos.y + 0.5f);

            drw_wire_t *wire = malloc(sizeof(drw_wire_t));
            DRAWABLES_WIRE_init(wire, startPos, endPos, wireColors[currentColor]);
            //wire->active = true;
            DRAWABLES_enqueue((drawable_t*)wire);
        }
    }
}

void BENCH_draw() {
    DrawRectangle(0, 0, 20, 20, wireColors[currentColor]);

    Vector2 pos = LIB_screenSpaceToWorldSpace(GetMousePosition());
    pos.x = (int)(pos.x + 0.5f);
    pos.y = (int)(pos.y + 0.5f);

    if (hasStartPos) {
        DrawLineEx(LIB_worldSpaceToScreenSpace(startPos), LIB_worldSpaceToScreenSpace(pos), 1, wireColors[currentColor]);
    }

    Vector2 wpos = LIB_worldSpaceToScreenSpace(pos);
    DrawLineEx((Vector2){wpos.x, wpos.y - 5}, (Vector2){wpos.x, wpos.y + 5}, 1, wireColors[currentColor]);
    DrawLineEx((Vector2){wpos.x - 5, wpos.y}, (Vector2){wpos.x + 5, wpos.y}, 1, wireColors[currentColor]);
}