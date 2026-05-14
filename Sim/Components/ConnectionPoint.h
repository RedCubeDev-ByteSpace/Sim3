//
// Created by redcube on 12/05/2026.
//

#ifndef SIM3_CONNECTIONPOINT_H
#define SIM3_CONNECTIONPOINT_H
#include <raylib.h>

// ---------------------------------------------------------------------------------------------------------------------
// Connection Point States
// -----------------------
// In what state can something connected to a wire be?
typedef enum SIM_CONNECTION_POINT_STATE {
    CONNECTION_POINT_FLOATING,
    CONNECTION_POINT_LOW,
    CONNECTION_POINT_HIGH,
} sim_connection_point_state_t;

// ---------------------------------------------------------------------------------------------------------------------
// Connection Points
// -----------------
// Something that a wire can connect to
typedef struct SIM_CONNECTION_POINT {
    sim_connection_point_state_t state;
    bool attachedWireState;
    Vector2 position;
} sim_connection_point_t;

void SIM_CONNECTION_POINT_init(sim_connection_point_t* me, sim_connection_point_state_t state, Vector2 position);
void SIM_CONNECTION_POINT_autoConnectWires(sim_connection_point_t* me);

#endif //SIM3_CONNECTIONPOINT_H
