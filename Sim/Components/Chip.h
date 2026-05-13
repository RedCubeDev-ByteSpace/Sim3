//
// Created by redcube on 13/05/2026.
//

#ifndef SIM3_CHIP_H
#define SIM3_CHIP_H

typedef enum SIM_PIN_SPECIFICATION {
    PIN_INPUT, PIN_OUTPUT, PIN_POWER
} sim_pin_specification_t;

typedef struct SIM_CHIP_SPECIFICATION {

    char *name;
    char *function;
    int numPins;
    sim_pin_specification_t *pinSpecs;

} sim_chip_specification_t;

#endif //SIM3_CHIP_H
