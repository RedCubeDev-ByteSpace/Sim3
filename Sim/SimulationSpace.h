//
// Created by redcube on 12/05/2026.
//

#ifndef SIM3_SIMULATIONSPACE_H
#define SIM3_SIMULATIONSPACE_H
#include "Components/Connection.h"
#include "Components/FixedContact.h"
#define MAX_CONNECTIONS 100

extern sim_connection_t *SIMSPACE_connections[MAX_CONNECTIONS];
extern sim_connection_point_t *SIMSPACE_connectionPoints[MAX_CONNECTIONS];
extern sim_fixed_contact_t *SIMSPACE_fixedContacts[MAX_CONNECTIONS];

void SIMSPACE_init();
void SIMSPACE_process();
int SIMSPACE_addConnection(sim_connection_t *connection);
int SIMSPACE_addConnectionPoint(sim_connection_point_t *connection);
int SIMSPACE_addFixedContact(sim_fixed_contact_t *contact);

#endif //SIM3_SIMULATIONSPACE_H
