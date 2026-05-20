//
// Created by redcube on 15/05/2026.
//

#ifndef SIM3_SIMLED_H
#define SIM3_SIMLED_H

#include "../../GUI/Drawables/LED.h"
#include "ConnectionPoint.h"

typedef struct SIM_LED {

    sim_connection_point_t point;
    drw_led_t *led;

} sim_led_t;

void SIM_LED_init(sim_led_t *me, Vector2 position, int rotation);
void SIM_LED_refreshDrawable(sim_led_t *me);
void SIM_LED_unload(sim_led_t *me);


#endif //SIM3_SIMLED_H
