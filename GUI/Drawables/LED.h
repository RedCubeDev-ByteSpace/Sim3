//
// Created by redcube on 15/05/2026.
//

#ifndef SIM3_LED_H
#define SIM3_LED_H
#include "../Drawables.h"
#include "raylib.h"

typedef struct DRW_LED {
    drawable_t base;

    Vector2 position;
    bool on;
    int rotation;

} drw_led_t;

void DRAWABLES_LED_init(drw_led_t *me, Vector2 pos, int rotation);
void DRAWABLES_LED_draw(drw_led_t *me);
Rectangle DRAWABLES_LED_getInteractionRect(drw_led_t *me);

#endif //SIM3_LED_H
