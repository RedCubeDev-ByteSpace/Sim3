//
// Created by redcube on 13/05/2026.
//

#include "Chip.h"
#include "../Grid.h"
#include "../GUI.h"

void DRAWABLES_CHIP_init(drw_chip_t *me, Vector2 pos, char *name, char *function, int numConnectorsPerRow) {
    me->base.type = DRAWABLE_CHIP;
    me->position = pos;
    me->name = name;
    me->function = function;
    me->numConnectorsPerRow = numConnectorsPerRow;

    for (int i = 0; i < me->numConnectorsPerRow * 2; ++i) {
        me->pinDisplayStates[i] = PIN_DISP_OUTPUT_LOW;
    }
}

void DRAWABLES_CHIP_draw(drw_chip_t *me) {

    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);
    for (int i = 0; i < me->numConnectorsPerRow; i++) {

        // top row
        DrawLine(wpos.x + i * zoomedSpacing, wpos.y, wpos.x + i * zoomedSpacing, wpos.y + zoomedSpacing, BLACK);
        DRAWABLES_CHIP_drawPin((Vector2){wpos.x + i * zoomedSpacing, wpos.y}, true, me->pinDisplayStates[me->numConnectorsPerRow * 2 - i - 1]);

        // bottom row
        DrawLine(wpos.x + i * zoomedSpacing, wpos.y + 4*zoomedSpacing, wpos.x + i * zoomedSpacing, wpos.y + 3*zoomedSpacing, BLACK);
        DRAWABLES_CHIP_drawPin((Vector2){wpos.x + i * zoomedSpacing, wpos.y + 4 * zoomedSpacing}, false, me->pinDisplayStates[i]);
    }

    DrawRectangleLinesEx((Rectangle){
        wpos.x - zoomedSpacing / 2, wpos.y + zoomedSpacing,
        me->numConnectorsPerRow * zoomedSpacing, 2 * zoomedSpacing
    }, LINE_THICKNESS, BLACK);

    DrawCircleSector(
        (Vector2){wpos.x - zoomedSpacing / 2, wpos.y + 2 * zoomedSpacing},
        4 * GRID_zoom,
        -90, 90, 10, BLACK);

    Vector2 size = MeasureTextEx(GUI_computerModern, me->name, zoomedSpacing, 1);
    DrawTextEx(GUI_computerModern, me->name,
        (Vector2) {
            wpos.x - zoomedSpacing / 2 + (me->numConnectorsPerRow * zoomedSpacing) / 2 - size.x / 2,
            wpos.y + zoomedSpacing + zoomedSpacing - size.y / 2 + zoomedSpacing * 0.05f
        },
        zoomedSpacing, 1, BLACK);

}

void DRAWABLES_CHIP_drawPin(Vector2 pos, bool isTopRow, sim_pin_display_state_t state) {
    switch (state) {
        case PIN_DISP_POWER:
            DrawCircle(pos.x, pos.y, 1 * GRID_zoom, BLACK);
            break;
        case PIN_DISP_CLOCK:
            DrawCircleLines(pos.x, pos.y, 1 * GRID_zoom, BLACK);
            break;
        case PIN_DISP_INPUT:
            if (!isTopRow) {
                DrawTriangle(
                    (Vector2){ pos.x, pos.y - GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y + GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y + GRID_zoom },
                    WHITE
                );
                DrawTriangleLines(
                    (Vector2){ pos.x, pos.y - GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y + GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y + GRID_zoom },
                    BLACK
                );
            } else {
                DrawTriangle(
                    (Vector2){ pos.x, pos.y + GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y - GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y - GRID_zoom },
                    WHITE
                );
                DrawTriangleLines(
                    (Vector2){ pos.x, pos.y + GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y - GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y - GRID_zoom },
                    BLACK
                );
            }
            break;
        case PIN_DISP_OUTPUT_LOW:
            if (isTopRow) {
                DrawTriangle(
                    (Vector2){ pos.x, pos.y - GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y + GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y + GRID_zoom },
                    WHITE
                );
                DrawTriangleLines(
                    (Vector2){ pos.x, pos.y - GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y + GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y + GRID_zoom },
                    BLACK
                );
            } else {
                DrawTriangle(
                    (Vector2){ pos.x, pos.y + GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y - GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y - GRID_zoom },
                    WHITE
                );
                DrawTriangleLines(
                    (Vector2){ pos.x, pos.y + GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y - GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y - GRID_zoom },
                    BLACK
                );
            }
            break;
        case PIN_DISP_OUTPUT_HIGH:
            if (isTopRow) {
                DrawTriangle(
                    (Vector2){ pos.x, pos.y - GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y + GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y + GRID_zoom },
                    BLACK
                );
            } else {
                DrawTriangle(
                    (Vector2){ pos.x, pos.y + GRID_zoom },
                    (Vector2){ pos.x + GRID_zoom, pos.y - GRID_zoom },
                    (Vector2){ pos.x - GRID_zoom, pos.y - GRID_zoom },
                    BLACK
                );
            }
            break;
    }


}

Rectangle DRAWABLES_CHIP_getInteractionRect(drw_chip_t *me) {
    return (Rectangle){};
}