//
// Created by redcube on 15/05/2026.
//

#include "LED.h"
#include "../Grid.h"
#include "../GUI.h"

void DRAWABLES_LED_init(drw_led_t *me, Vector2 pos, int rotation) {
    me->base.type = DRAWABLE_LED;
    me->position = pos;
    me->on = false;
    me->rotation = rotation;
}

void DRAWABLES_LED_draw(drw_led_t *me) {
    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);

    Color off = (Color){0,0,0,0};

    switch (me->rotation) {
        // Rotation 0 = upwards
        case 0:
            DrawLine(wpos.x, wpos.y, wpos.x, wpos.y - zoomedSpacing, BLACK);
            DrawCircle(wpos.x, wpos.y - 1.5f * zoomedSpacing, zoomedSpacing / 5 * 2, me->on ? RED : off);
            DrawCircleLines(wpos.x, wpos.y - 1.5f * zoomedSpacing, zoomedSpacing / 2, BLACK);
            break;

        // Rotation 1 = right
        case 1:
            DrawLine(wpos.x, wpos.y, wpos.x + zoomedSpacing, wpos.y, BLACK);
            DrawCircle(wpos.x + 1.5f * zoomedSpacing, wpos.y, zoomedSpacing / 5 * 2, me->on ? RED : off);
            DrawCircleLines(wpos.x + 1.5f * zoomedSpacing, wpos.y, zoomedSpacing / 2, BLACK);
            break;

        // Rotation 2 = downwards
        case 2:
            DrawLine(wpos.x, wpos.y, wpos.x, wpos.y + zoomedSpacing, BLACK);
            DrawCircle(wpos.x, wpos.y + 1.5f * zoomedSpacing, zoomedSpacing / 5 * 2, me->on ? RED : off);
            DrawCircleLines(wpos.x, wpos.y + 1.5f * zoomedSpacing, zoomedSpacing / 2, BLACK);
            break;

        // Rotation 3 = left
        case 3:
            DrawLine(wpos.x, wpos.y, wpos.x - zoomedSpacing, wpos.y, BLACK);
            DrawCircle(wpos.x - 1.5f * zoomedSpacing, wpos.y, zoomedSpacing / 5 * 2, me->on ? RED : off);
            DrawCircleLines(wpos.x - 1.5f * zoomedSpacing, wpos.y, zoomedSpacing / 2, BLACK);
            break;
    }

    // contact circle
    DrawCircleLines(wpos.x, wpos.y, 1 * GRID_zoom, BLACK);
}

Rectangle DRAWABLES_LED_getInteractionRect(drw_led_t *me) {
    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);
    return (Rectangle){
        wpos.x - zoomedSpacing / 2, wpos.y - 2 * zoomedSpacing, zoomedSpacing, zoomedSpacing
    };
}