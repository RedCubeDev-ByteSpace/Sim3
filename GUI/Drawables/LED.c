//
// Created by redcube on 15/05/2026.
//

#include "LED.h"
#include "../Grid.h"
#include "../GUI.h"

void DRAWABLES_LED_init(drw_led_t *me, Vector2 pos) {
    me->base.type = DRAWABLE_LED;
    me->position = pos;
    me->on = false;
}

void DRAWABLES_LED_draw(drw_led_t *me) {
    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);

    DrawLine(wpos.x, wpos.y, wpos.x, wpos.y - zoomedSpacing, BLACK);
    DrawCircleLines(wpos.x, wpos.y, 1 * GRID_zoom, BLACK);

    DrawCircle(wpos.x, wpos.y - 1.5f * zoomedSpacing, zoomedSpacing / 4, me->on ? RED : BLACK);
    DrawCircleLines(wpos.x, wpos.y - 1.5f * zoomedSpacing, zoomedSpacing / 2, BLACK);
}

Rectangle DRAWABLES_LED_getInteractionRect(drw_led_t *me) {
    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);
    return (Rectangle){
        wpos.x - zoomedSpacing / 2, wpos.y - 2 * zoomedSpacing, zoomedSpacing, zoomedSpacing
    };
}