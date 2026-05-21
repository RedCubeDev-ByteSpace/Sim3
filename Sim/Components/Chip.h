//
// Created by redcube on 13/05/2026.
//

#ifndef SIM3_SIMCHIP_H
#define SIM3_SIMCHIP_H
#include <lua.h>
#include "ChipSpecEnums.h"
#include "../SimulationSpace.h"
#include "../../GUI/Drawables/Chip.h"

typedef struct SIM_CHIP {

    sim_chip_specification_t *chipSpec;
    sim_connectionpoint_list_t connectionPoints;

    lua_State *luaState;
    bool prevClockPinWireState;

    drw_chip_t *chip;

} sim_chip_t;

void SIM_CHIP_init(sim_chip_t *me, Vector2 position, sim_chip_specification_t *chipSpec);
void SIM_CHIP_refreshDrawable(sim_chip_t *me);
void SIM_CHIP_setup(sim_chip_t *me);
void SIM_CHIP_step(sim_chip_t *me);
void SIM_CHIP_buildPinTable(sim_chip_t *me, bool includeWireState);
void SIM_CHIP_readoutPinTable(sim_chip_t *me);
void SIM_CHIP_unload(sim_chip_t *me);

#endif //SIM3_SIMCHIP_H
