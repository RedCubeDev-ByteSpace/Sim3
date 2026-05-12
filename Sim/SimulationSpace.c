//
// Created by redcube on 12/05/2026.
//
#include "SimulationSpace.h"

#include <stddef.h>
#include <stdlib.h>

sim_connection_t *SIMSPACE_connections[MAX_CONNECTIONS];
sim_connection_point_t *SIMSPACE_connectionPoints[MAX_CONNECTIONS];
sim_fixed_contact_t *SIMSPACE_fixedContacts[MAX_CONNECTIONS];

void SIMSPACE_init() {
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        SIMSPACE_connections[i] = NULL;
    }
}

void SIMSPACE_process() {

    // Update all wires
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        if (SIMSPACE_connections[i] == NULL) continue;
        SIM_CONNECTION_refreshState(SIMSPACE_connections[i]);
    }

}

int SIMSPACE_addConnection(sim_connection_t *connection) {
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        if (SIMSPACE_connections[i] != NULL) continue;

        SIMSPACE_connections[i] = connection;
        return i;
    }

    return -1;
}

int SIMSPACE_addConnectionPoint(sim_connection_point_t *connectionPoint) {
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        if (SIMSPACE_connectionPoints[i] != NULL) continue;

        SIMSPACE_connectionPoints[i] = connectionPoint;
        return i;
    }

    return -1;
}

int SIMSPACE_addFixedContact(sim_fixed_contact_t *contact) {
    for (int i = 0; i < MAX_CONNECTIONS; ++i) {
        if (SIMSPACE_fixedContacts[i] != NULL) continue;

        SIMSPACE_fixedContacts[i] = contact;
        SIMSPACE_addConnectionPoint(contact->point);
        return i;
    }

    return -1;
}