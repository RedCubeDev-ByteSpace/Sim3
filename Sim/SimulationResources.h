//
// Created by redcube on 13/05/2026.
//

#ifndef SIM3_SIMULATIONRESOURCES_H
#define SIM3_SIMULATIONRESOURCES_H
#include "Components/Chip.h"
#define MAX_CHIP_SPECS 1

extern sim_chip_specification_t SIMRES_chipSpecifications[MAX_CHIP_SPECS];
extern int SIMRES_numChipSpecs;

void SIMRES_init();

#endif //SIM3_SIMULATIONRESOURCES_H
