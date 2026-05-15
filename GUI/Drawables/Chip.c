//
// Created by redcube on 13/05/2026.
//

#include "Chip.h"
#include "../Grid.h"
#include "../GUI.h"

void DRAWABLES_CHIP_init(drw_chip_t *me, Vector2 pos, char *name, char *function, int numConnectorsPerRow, sim_pin_specification_t *chipSpec) {
    me->base.type = DRAWABLE_CHIP;
    me->position = pos;
    me->name = name;
    me->function = function;
    me->numConnectorsPerRow = numConnectorsPerRow;
    me->chipSpec = chipSpec;
}

void DRAWABLES_CHIP_draw(drw_chip_t *me) {

    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);
    for (int i = 0; i < me->numConnectorsPerRow; i++) {

        // top row
        if (me->chipSpec[me->numConnectorsPerRow - i - 1] == PIN_POWER) {
            DrawCircle(wpos.x + i * zoomedSpacing, wpos.y, 1 * GRID_zoom, BLACK);
        }
        else {
            DrawCircleLines(wpos.x + i * zoomedSpacing, wpos.y, 1 * GRID_zoom, BLACK);
        }

        DrawLine(wpos.x + i * zoomedSpacing, wpos.y, wpos.x + i * zoomedSpacing, wpos.y + zoomedSpacing, BLACK);

        // bottom row
        if (me->chipSpec[i] == PIN_POWER) {
            DrawCircle(wpos.x + i * zoomedSpacing, wpos.y + 4*zoomedSpacing, 1 * GRID_zoom, BLACK);
        }
        else {
            DrawCircleLines(wpos.x + i * zoomedSpacing, wpos.y + 4*zoomedSpacing, 1 * GRID_zoom, BLACK);
        }


        DrawLine(wpos.x + i * zoomedSpacing, wpos.y + 4*zoomedSpacing, wpos.x + i * zoomedSpacing, wpos.y + 3*zoomedSpacing, BLACK);
    }

    DrawRectangleLinesEx((Rectangle){
        wpos.x - zoomedSpacing / 2, wpos.y + zoomedSpacing,
        me->numConnectorsPerRow * zoomedSpacing, 2 * zoomedSpacing
    }, LINE_THICKNESS, BLACK);

    DrawCircleSector(
        (Vector2){wpos.x - zoomedSpacing / 2, wpos.y + 2 * zoomedSpacing},
        4 * GRID_zoom,
        -90, 90, 10, BLACK);

    // int fontSize = 0;
    // for (int i = 0; i < 300; i += 2) {
    //     Vector2 size = MeasureTextEx(GUI_computerModern, me->name, i, 1);
    //     if (size.y > zoomedSpacing) {
    //         fontSize = i;
    //         break;
    //     }
    // }

    Vector2 size = MeasureTextEx(GUI_computerModern, me->name, zoomedSpacing, 1);
    DrawTextEx(GUI_computerModern, me->name,
        (Vector2) {
            wpos.x - zoomedSpacing / 2 + (me->numConnectorsPerRow * zoomedSpacing) / 2 - size.x / 2,
            wpos.y + zoomedSpacing + zoomedSpacing - size.y / 2 + zoomedSpacing * 0.05f
        },
        zoomedSpacing, 1, BLACK);

}

Rectangle DRAWABLES_CHIP_getInteractionRect(drw_chip_t *me) {
    return (Rectangle){};
}