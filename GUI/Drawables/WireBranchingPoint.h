//
// Created by redcube on 14/05/2026.
//

#ifndef SIM3_WIREBRANCHINGPOINT_H
#define SIM3_WIREBRANCHINGPOINT_H
#include <raylib.h>

#include "../Drawables.h"

typedef struct DRW_WIRE_BRANCHING_POINT {
    drawable_t base;

    Vector2 pos;
    bool error;
    Color color;

} drw_wire_branching_point_t;

void DRAWABLES_WIRE_BRANCHING_POINT_init(drw_wire_branching_point_t *me, Vector2 pos, Color color);
void DRAWABLES_WIRE_BRANCHING_POINT_draw(drw_wire_branching_point_t *me);

#endif //SIM3_WIREBRANCHINGPOINT_H
