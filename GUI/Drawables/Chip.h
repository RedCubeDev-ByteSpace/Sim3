//
// Created by redcube on 13/05/2026.
//

#ifndef SIM3_CHIP_H
#define SIM3_CHIP_H
#include <raylib.h>
#include "../Drawables.h"
#include "../../Sim/Components/ChipSpecEnums.h"

typedef struct DRW_CHIP {
    drawable_t base;

    char *name;
    char *function;
    Vector2 position;
    int numConnectorsPerRow;
    sim_pin_display_state_t pinDisplayStates[64];

} drw_chip_t;

void DRAWABLES_CHIP_init(drw_chip_t *me, Vector2 pos, char *name, char *function, int numConnectorsPerRow);
void DRAWABLES_CHIP_draw(drw_chip_t *me);
void DRAWABLES_CHIP_drawPin(Vector2 pos, bool isTopRow, sim_pin_display_state_t state);
Rectangle DRAWABLES_CHIP_getInteractionRect(drw_chip_t *me);

#endif //SIM3_CHIP_H
