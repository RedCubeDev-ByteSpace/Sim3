//
// Created by redcube on 13/05/2026.
//

#ifndef SIM3_SIMFIXEDCONTACT_H
#define SIM3_SIMFIXEDCONTACT_H
#include <stdbool.h>

#include "../../GUI/Drawables/FixedContact.h"
#include "ConnectionPoint.h"

typedef struct SIM_FIXED_CONTACT {

    sim_connection_point_t point;
    drw_fixed_contact_t *contact;

} sim_fixed_contact_t;

void SIM_FIXED_CONTACT_init(sim_fixed_contact_t *me, Vector2 position, bool state);
void SIM_FIXED_CONTACT_refreshDrawable(sim_fixed_contact_t *me);
void SIM_FIXED_CONTACT_unload(sim_fixed_contact_t *me);

#endif //SIM3_SIMFIXEDCONTACT_H
