//
// Created by redcube on 13/05/2026.
//

#include "FixedContact.h"

#include "../Grid.h"
#include "../GUI.h"

void DRAWABLES_FIXED_CONTACT_init(drw_fixed_contact_t *me, Vector2 pos, bool high, int rotation) {
    me->base.type = DRAWABLE_FIXED_CONTACT;
    me->position = pos;
    me->high = high;
    me->rotation = rotation;
}

void DRAWABLES_FIXED_CONTACT_draw(drw_fixed_contact_t *me)
{
    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);

    switch (me->rotation) {
        // Rotation 0 = upwards
        case 0:
            DrawRectangleLinesEx((Rectangle){
                    wpos.x - zoomedSpacing / 2, wpos.y - 2 * zoomedSpacing, zoomedSpacing, zoomedSpacing
                }, LINE_THICKNESS, BLACK);
            DrawLine(wpos.x, wpos.y, wpos.x, wpos.y - zoomedSpacing, BLACK);
            DrawCircle(wpos.x, wpos.y - 1.5f * zoomedSpacing, zoomedSpacing / 4, me->high ? RED : BLACK);
            break;

        // Rotation 1 = right
        case 1:
            DrawRectangleLinesEx((Rectangle){
                    wpos.x + zoomedSpacing, wpos.y - zoomedSpacing / 2, zoomedSpacing, zoomedSpacing
                }, LINE_THICKNESS, BLACK);
            DrawLine(wpos.x + zoomedSpacing, wpos.y, wpos.x, wpos.y, BLACK);
            DrawCircle(wpos.x + 1.5f * zoomedSpacing, wpos.y, zoomedSpacing / 4, me->high ? RED : BLACK);
            break;

        // Rotation 2 = downwards
        case 2:
            DrawRectangleLinesEx((Rectangle){
                               wpos.x - zoomedSpacing / 2, wpos.y + zoomedSpacing, zoomedSpacing, zoomedSpacing
                           }, LINE_THICKNESS, BLACK);
            DrawLine(wpos.x, wpos.y, wpos.x, wpos.y + zoomedSpacing, BLACK);
            DrawCircle(wpos.x, wpos.y + 1.5f * zoomedSpacing, zoomedSpacing / 4, me->high ? RED : BLACK);
            break;

        // Rotation 3 = left
        case 3:
            DrawRectangleLinesEx((Rectangle){
                                wpos.x - 2 * zoomedSpacing, wpos.y - zoomedSpacing / 2, zoomedSpacing, zoomedSpacing
                            }, LINE_THICKNESS, BLACK);
            DrawLine(wpos.x - zoomedSpacing, wpos.y, wpos.x, wpos.y, BLACK);
            DrawCircle(wpos.x - 1.5f * zoomedSpacing, wpos.y, zoomedSpacing / 4, me->high ? RED : BLACK);
            break;
    }

    DrawCircleLines(wpos.x, wpos.y, 1 * GRID_zoom, BLACK);
}

Rectangle DRAWABLES_FIXED_CONTACT_getInteractionRect(drw_fixed_contact_t *me) {
    float zoomedSpacing = GRID_SPACING * GRID_zoom;
    Vector2 wpos = LIB_worldSpaceToScreenSpace(me->position);

    switch (me->rotation) {
        // Rotation 0 = upwards
        case 0:
            return (Rectangle){
                wpos.x - zoomedSpacing / 2, wpos.y - 2 * zoomedSpacing, zoomedSpacing, zoomedSpacing
            };

        // Rotation 1 = right
        case 1:
            return (Rectangle){
                wpos.x + zoomedSpacing, wpos.y - zoomedSpacing / 2, zoomedSpacing, zoomedSpacing
            };

        // Rotation 2 = downwards
        case 2:
            return (Rectangle){
                wpos.x - zoomedSpacing / 2, wpos.y + zoomedSpacing, zoomedSpacing, zoomedSpacing
            };

        // Rotation 3 = left
        case 3:
            return (Rectangle){
                wpos.x - 2 * zoomedSpacing, wpos.y - zoomedSpacing / 2, zoomedSpacing, zoomedSpacing
            };
    }
}