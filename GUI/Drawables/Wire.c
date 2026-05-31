//
// Created by redcube on 10/05/2026.
//
#include "../Drawables.h"
#include "Wire.h"
#include "../GUI.h"
#include "../../Sim/Components/Connection.h"

void DRAWABLES_WIRE_init(drw_wire_t *me, Vector2 start, Vector2 end, Color color) {
    me->base.type = DRAWABLE_WIRE;
    me->start = start;
    me->end = end;
    me->color = color;
    me->active = false;
    me->error = false;
}

void DRAWABLES_WIRE_draw(drw_wire_t *me) {
    Vector2 start = LIB_worldSpaceToScreenSpace(me->start);
    Vector2 end = LIB_worldSpaceToScreenSpace(me->end);

    if (me->error) {
        DrawLineEx(start, end, LINE_THICKNESS * 2, COLOR_ERROR);
    }

    DrawLineEx(start, end, me->active ? LINE_THICKNESS * 1.5f : LINE_THICKNESS, me->color);

    // if (me->active) {
    //     DrawLineEx(LIB_worldSpaceToScreenSpace(me->start), LIB_worldSpaceToScreenSpace(me->end), LINE_THICKNESS * 2, me->color);
    // } else {
    //     DrawLineEx(LIB_worldSpaceToScreenSpace(me->start), LIB_worldSpaceToScreenSpace(me->end), LINE_THICKNESS, me->color);
    // }

}