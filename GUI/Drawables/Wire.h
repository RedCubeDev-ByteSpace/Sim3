//
// Created by redcube on 10/05/2026.
//

#ifndef SIM3_WIRE_H
#define SIM3_WIRE_H
#include <raylib.h>

#include "../Drawables.h"

typedef struct DRW_WIRE {
    drawable_t base;

    bool active;
    Color color;
    Vector2 start;
    Vector2 end;

} drw_wire_t;

void DRAWABLES_WIRE_init(drw_wire_t *me, Vector2 start, Vector2 end, Color color);
void DRAWABLES_WIRE_draw(drw_wire_t *me);

#endif //SIM3_WIRE_H
