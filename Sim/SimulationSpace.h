//
// Created by redcube on 12/05/2026.
//

#ifndef SIM3_SIMULATIONSPACE_H
#define SIM3_SIMULATIONSPACE_H
#include "Components/Connection.h"
#include "Components/FixedContact.h"
#include "SimLists/SimspaceLists.h"

extern sim_connection_list_t *SIMSPACE_lstConnections;
extern sim_connectionpoint_list_t *SIMSPACE_lstConnectionPoints;
extern sim_fixedcontact_list_t *SIMSPACE_lstFixedContacts;

void SIMSPACE_init();
void SIMSPACE_process();

#endif //SIM3_SIMULATIONSPACE_H
