//
// Created by redcube on 12/05/2026.
//

#ifndef SIM3_SIMULATIONSPACE_H
#define SIM3_SIMULATIONSPACE_H
#include "SimLists/SimspaceLists.h"

extern sim_connection_list_t *SIMSPACE_lstConnections;
extern sim_connectionpoint_list_t *SIMSPACE_lstConnectionPoints;
extern sim_fixedcontact_list_t *SIMSPACE_lstFixedContacts;
extern sim_led_list_t *SIMSPACE_lstLEDs;
extern sim_chip_list_t *SIMSPACE_lstChips;

void SIMSPACE_init();
void SIMSPACE_process();

#endif //SIM3_SIMULATIONSPACE_H
