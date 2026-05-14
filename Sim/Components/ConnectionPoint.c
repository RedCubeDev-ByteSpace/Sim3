//
// Created by redcube on 12/05/2026.
//

#include "ConnectionPoint.h"

#include "../SimulationSpace.h"

void SIM_CONNECTION_POINT_init(sim_connection_point_t* me, sim_connection_point_state_t state, Vector2 position) {
    me->state = state;
    me->position = position;
    me->attachedWireState = false;
}

void SIM_CONNECTION_POINT_autoConnectWires(sim_connection_point_t* me) {

    // go through all wires
    for (int i = 0; i < SIMSPACE_lstConnections->length; ++i) {

        sim_connection_t *con = SIMSPACE_lstConnections->buffer[i];

        // go through all points on the wire
        for (int ii = 0; ii < con->lstVectorPairs.length; ++ii) {
            sim_vector_pair_t pair = con->lstVectorPairs.buffer[ii];

            if (me->position.x == pair.from.x && me->position.y == pair.from.y) {
                SIM_COMP_LIST_appendConnectionPoint(&con->lstConnectedPoints, me);
                return;
            }

            if (me->position.x == pair.to.x && me->position.y == pair.to.y) {
                SIM_COMP_LIST_appendConnectionPoint(&con->lstConnectedPoints, me);
                return;
            }
        }
    }
}