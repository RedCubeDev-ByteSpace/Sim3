//
// Created by redcube on 15/05/2026.
//

#include "LED.h"
#include "../SimulationSpace.h"
#include <stdlib.h>

void SIM_LED_init(sim_led_t *me, Vector2 position) {

    // initialize the contact point
    SIM_CONNECTION_POINT_init(&me->point, CONNECTION_POINT_FLOATING, position);

    me->led = malloc(sizeof(drw_led_t));
    DRAWABLES_LED_init(me->led, me->point.position);
    DRAWABLES_enqueue((drawable_t*)me->led);

    SIM_CONNECTION_POINT_autoConnectWires(&me->point);
    SIM_COMP_LIST_appendConnectionPoint(SIMSPACE_lstConnectionPoints, &me->point);
}

void SIM_LED_refreshDrawable(sim_led_t *me) {
    me->led->on = me->point.attachedWireState;
}

void SIM_LED_unload(sim_led_t *me) {
    DRAWABLES_unload((drawable_t*)me->led);
    SIM_COMP_LIST_removeConnectionPointRef(SIMSPACE_lstConnectionPoints, &me->point);
}