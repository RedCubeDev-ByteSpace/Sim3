//
// Created by redcube on 13/05/2026.
//

#include "FixedContact.h"

#include "../Grid.h"
#include "../GUI.h"

void DRAWABLES_FIXED_CONTACT_init(drw_fixed_contact_t *me, Vector2 pos, bool high) {
    me->base.type = DRAWABLE_FIXED_CONTACT;
    me->position = pos;
    me->high = high;
}

void DRAWABLES_FIXED_CONTACT_draw(drw_fixed_contact_t *me)
{
    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);
    DrawRectangleLinesEx((Rectangle){
        wpos.x - zoomedSpacing / 2, wpos.y - 2 * zoomedSpacing, zoomedSpacing, zoomedSpacing
    }, LINE_THICKNESS, BLACK);

    DrawLine(wpos.x, wpos.y, wpos.x, wpos.y - zoomedSpacing, BLACK);
    DrawCircleLines(wpos.x, wpos.y, 1 * GRID_zoom, BLACK);

    DrawCircle(wpos.x, wpos.y - 1.5f * zoomedSpacing, zoomedSpacing / 4, me->high ? RED : BLACK);
}

Rectangle DRAWABLES_FIXED_CONTACT_getInteractionRect(drw_fixed_contact_t *me) {
    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);
    return (Rectangle){
        wpos.x - zoomedSpacing / 2, wpos.y - 2 * zoomedSpacing, zoomedSpacing, zoomedSpacing
    };
}