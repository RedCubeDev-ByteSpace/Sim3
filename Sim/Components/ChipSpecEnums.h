//
// Created by redcube on 15/05/2026.
//

#ifndef SIM3_CHIPSPECENUMS_H
#define SIM3_CHIPSPECENUMS_H

typedef enum SIM_PIN_SPECIFICATION {
    PIN_INPUT, PIN_OUTPUT, PIN_POWER, PIN_CLOCK
} sim_pin_specification_t;

typedef enum SIM_PIN_DISPLAY_STATE {
    PIN_DISP_INPUT, PIN_DISP_OUTPUT_LOW, PIN_DISP_OUTPUT_HIGH, PIN_DISP_POWER, PIN_DISP_CLOCK
} sim_pin_display_state_t;

#endif //SIM3_CHIPSPECENUMS_H
