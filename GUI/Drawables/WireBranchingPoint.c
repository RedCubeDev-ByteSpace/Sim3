//
// Created by redcube on 14/05/2026.
//

#include "WireBranchingPoint.h"

#include "../GUI.h"
#include "../../Sim/Components/Connection.h"

void DRAWABLES_WIRE_BRANCHING_POINT_init(drw_wire_branching_point_t *me, Vector2 pos, Color color) {
    me->base.type = DRAWABLE_WIRE_BRANCHING_POINT;
    me->pos = pos;
    me->color = color;
    me->error = false;
}

void DRAWABLES_WIRE_BRANCHING_POINT_draw(drw_wire_branching_point_t *me) {
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->pos);

    if (me->error) {
        DrawCircle((int)wpos.x, (int)wpos.y, LINE_THICKNESS * 2, COLOR_ERROR);
        return;
    }

    DrawCircle((int)wpos.x, (int)wpos.y, LINE_THICKNESS * 2, me->color);
}