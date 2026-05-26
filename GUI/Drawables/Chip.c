//
// Created by redcube on 13/05/2026.
//

#include "Chip.h"

#include <math.h>
#include <stdio.h>

#include "../Grid.h"
#include "../GUI.h"

void DRAWABLES_CHIP_init(drw_chip_t *me, Vector2 pos, sim_chip_specification_t *chipSpec, int numConnectorsPerRow) {
    me->base.type = DRAWABLE_CHIP;
    me->position = pos;
    me->chipSpec = chipSpec;
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
        DRAWABLES_CHIP_drawPinLabel(me, true, i);

        // bottom row
        DrawLine(wpos.x + i * zoomedSpacing, wpos.y + 4*zoomedSpacing, wpos.x + i * zoomedSpacing, wpos.y + 3*zoomedSpacing, BLACK);
        DRAWABLES_CHIP_drawPin((Vector2){wpos.x + i * zoomedSpacing, wpos.y + 4 * zoomedSpacing}, false, me->pinDisplayStates[i]);
        DRAWABLES_CHIP_drawPinLabel(me, false, i);
    }

    DrawRectangleLinesEx((Rectangle){
        wpos.x - zoomedSpacing / 2, wpos.y + zoomedSpacing,
        me->numConnectorsPerRow * zoomedSpacing, 2 * zoomedSpacing
    }, LINE_THICKNESS, BLACK);

    DrawCircleSector(
        (Vector2){wpos.x - zoomedSpacing / 2, wpos.y + 2 * zoomedSpacing},
        4 * GRID_zoom,
        -90, 90, 10, BLACK);

    Font font = GUI_fonts[COMPUTER_MODERN_150];
    if (zoomedSpacing <= 11) {
        font = GUI_fonts[COMPUTER_MODERN_11];
    } else if (zoomedSpacing <= 16) {
        font = GUI_fonts[COMPUTER_MODERN_16];
    } else if (zoomedSpacing <= 20) {
        font = GUI_fonts[COMPUTER_MODERN_20];
    } else if (zoomedSpacing <= 55) {
        font = GUI_fonts[COMPUTER_MODERN_55];
    }

    Vector2 size = MeasureTextEx(font, me->chipSpec->name, zoomedSpacing, 1);
    DrawTextEx(font, me->chipSpec->name,
        (Vector2) {
            wpos.x - zoomedSpacing / 2 + (me->numConnectorsPerRow * zoomedSpacing) / 2 - size.x / 2,
            wpos.y + zoomedSpacing + zoomedSpacing - size.y / 2 + zoomedSpacing * 0.05f
        },
        zoomedSpacing, 1, BLACK);

}

void DRAWABLES_CHIP_drawPinLabel(drw_chip_t *me, bool isTopRow, int index) {
    float zoomPercentage = log(GRID_zoom - 1) / logf(MAX_ZOOM - 1);
    if (zoomPercentage < 0.5f) return;

    float opacity = 0.0f;
    if (zoomPercentage > 0.7f) {
        opacity = 1;
    } else {
        opacity = 1.0f - (0.7f - zoomPercentage) / 0.2f;
    }

    int lblIdx = index;
    if (isTopRow) {
        lblIdx = me->numConnectorsPerRow * 2 - index - 1;
    }

    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);
    Vector2 size = MeasureTextEx(GUI_fonts[COMPUTER_MODERN_150], me->chipSpec->pinLabels[lblIdx].label, zoomedSpacing / 3, 1);

    if (isTopRow) {
        DrawTextEx(GUI_fonts[COMPUTER_MODERN_150], me->chipSpec->pinLabels[lblIdx].label,
           (Vector2) {
               wpos.x + index * zoomedSpacing - size.x / 2,
               wpos.y + 1.06f * zoomedSpacing
           },
           zoomedSpacing / 3, 1, (Color){0,0,0, (int)(255 * opacity)});

        if (me->chipSpec->pinLabels[lblIdx].activeLow) {
            DrawRectangle(
               wpos.x + index * zoomedSpacing - size.x / 2,
               wpos.y + 1.06f * zoomedSpacing,
               size.x,
               zoomedSpacing * 0.02f,
               (Color){0,0,0, (int)(255 * opacity)}
           );
        }
    } else {
        DrawTextEx(GUI_fonts[COMPUTER_MODERN_150], me->chipSpec->pinLabels[lblIdx].label,
           (Vector2) {
               wpos.x + index * zoomedSpacing - size.x / 2,
               wpos.y + 2.94f * zoomedSpacing - size.y
           },
           zoomedSpacing / 3, 1, (Color){0,0,0, (int)(255 * opacity)});

        if (me->chipSpec->pinLabels[lblIdx].activeLow) {
            DrawRectangle(
               wpos.x + index * zoomedSpacing - size.x / 2,
               wpos.y + 2.94f * zoomedSpacing - size.y,
               size.x,
               zoomedSpacing * 0.02f,
               (Color){0,0,0, (int)(255 * opacity)}
           );
        }
    }
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