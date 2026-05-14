//
// Created by redcube on 12/05/2026.
//
#include "SimulationSpace.h"

#include <stddef.h>
#include <stdlib.h>

sim_connection_list_t *SIMSPACE_lstConnections;
sim_connectionpoint_list_t *SIMSPACE_lstConnectionPoints;
sim_fixedcontact_list_t *SIMSPACE_lstFixedContacts;
sim_led_list_t *SIMSPACE_lstLEDs;

void SIMSPACE_init() {
    SIMSPACE_lstConnections = malloc(sizeof(sim_comp_list_t));
    SIMSPACE_lstConnectionPoints = malloc(sizeof(sim_comp_list_t));
    SIMSPACE_lstFixedContacts = malloc(sizeof(sim_comp_list_t));
    SIMSPACE_lstLEDs = malloc(sizeof(sim_led_list_t));

    SIM_COMP_LIST_init(SIMSPACE_lstConnections);
    SIM_COMP_LIST_init(SIMSPACE_lstConnectionPoints);
    SIM_COMP_LIST_init(SIMSPACE_lstFixedContacts);
    SIM_COMP_LIST_init(SIMSPACE_lstLEDs);
}

void SIMSPACE_process() {

    // Update all wires
    for (int i = 0; i < SIMSPACE_lstConnections->length; ++i) {
        SIM_CONNECTION_refreshState(SIMSPACE_lstConnections->buffer[i]);
    }

    // Update all LEDs
    for (int i = 0; i < SIMSPACE_lstLEDs->length; ++i) {
        SIM_LED_refreshDrawable(SIMSPACE_lstLEDs->buffer[i]);
    }
}