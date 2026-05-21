//
// Created by redcube on 21/05/2026.
//

#ifndef SIM3_CHIPSPEC_H
#define SIM3_CHIPSPEC_H

#define MAX_CHIP_SPECS 10
#define MAX_CHIP_PINS 32
#include "ChipSpecEnums.h"

typedef struct SIM_CHIP_PIN_LABEL {
    char label[5];
    bool activeLow;
} sim_chip_pin_label_t;

typedef struct SIM_CHIP_SPECIFICATION {

    char *id;
    char *name;
    char *function;

    int numPins;
    sim_chip_pin_label_t *pinLabels;

    char *script;
    bool isStateful;

    int clockPin;

    sim_pin_specification_t *pinSpecs;

} sim_chip_specification_t;

#endif //SIM3_CHIPSPEC_H
