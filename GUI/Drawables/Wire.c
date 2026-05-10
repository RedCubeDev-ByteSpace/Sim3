//
// Created by redcube on 10/05/2026.
//
#include "../Drawables.h"
#include "Wire.h"

#include "../GUI.h"

void DRAWABLES_WIRE_init(drw_wire_t *me, Vector2 start, Vector2 end, Color color) {
    me->base.type = DRAWABLE_WIRE;
    me->start = start;
    me->end = end;
    me->color = color;
    me->active = false;
}

void DRAWABLES_WIRE_draw(drw_wire_t *me) {
    if (me->active) {
        DrawLineEx(LIB_worldSpaceToScreenSpace(me->start), LIB_worldSpaceToScreenSpace(me->end), LINE_THICKNESS, COLOR_ACTIVE);
    } else {
        DrawLineEx(LIB_worldSpaceToScreenSpace(me->start), LIB_worldSpaceToScreenSpace(me->end), LINE_THICKNESS, me->color);
    }

}