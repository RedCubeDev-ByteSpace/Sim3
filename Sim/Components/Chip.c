//
// Created by redcube on 13/05/2026.
//

#include "Chip.h"

#include <stdlib.h>

void SIM_CHIP_init(sim_chip_t *me, Vector2 position, sim_chip_specification_t *chipSpec) {
    me->chipSpec = chipSpec;

    // create the chip drawable for this component
    me->chip = malloc(sizeof(drw_chip_t));
    DRAWABLES_CHIP_init(me->chip, position, chipSpec->name, chipSpec->function, chipSpec->numPins / 2, chipSpec->pinSpecs);
    DRAWABLES_enqueue((drawable_t*)me->chip);

    // create all connection points
    SIM_COMP_LIST_init(&me->connectionPoints);

    // bottom row
    for (int i = 0; i < chipSpec->numPins / 2; ++i) {
        sim_connection_point_t *conPoint = malloc(sizeof(sim_connection_point_t));
        SIM_CONNECTION_POINT_init(conPoint, CONNECTION_POINT_FLOATING, (Vector2){ position.x + i, position.y + 4 });
        SIM_COMP_LIST_appendConnectionPoint(&me->connectionPoints, conPoint);
        SIM_COMP_LIST_appendConnectionPoint(SIMSPACE_lstConnectionPoints, conPoint);
    }

    // top row
    for (int i = 0; i < chipSpec->numPins / 2; ++i) {
        sim_connection_point_t *conPoint = malloc(sizeof(sim_connection_point_t));
        SIM_CONNECTION_POINT_init(conPoint, CONNECTION_POINT_FLOATING, (Vector2){ position.x + chipSpec->numPins / 2 - i - 1, position.y });
        SIM_COMP_LIST_appendConnectionPoint(&me->connectionPoints, conPoint);
        SIM_COMP_LIST_appendConnectionPoint(SIMSPACE_lstConnectionPoints, conPoint);
    }
}

void SIM_CHIP_refreshDrawable(sim_chip_t *me) {

}

void SIM_CHIP_unload(sim_chip_t *me) {
    DRAWABLES_unload((drawable_t*)me->chip);

    for (int i = 0; i < me->connectionPoints.length; ++i) {
        sim_connection_point_t *conPoint = me->connectionPoints.buffer[i];
        SIM_COMP_LIST_removeConnectionPointRef(SIMSPACE_lstConnectionPoints, conPoint);
        free(conPoint);
    }
}