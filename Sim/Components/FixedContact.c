//
// Created by redcube on 13/05/2026.
//

#include "FixedContact.h"

#include <stdlib.h>

#include "../SimulationSpace.h"
#include "../SimLists/SimspaceLists.h"

void SIM_FIXED_CONTACT_init(sim_fixed_contact_t *me, Vector2 position, bool state, int rotation) {

    // initialize the contact point
    SIM_CONNECTION_POINT_init(&me->point, state ? CONNECTION_POINT_HIGH : CONNECTION_POINT_LOW, position);

    me->contact = malloc(sizeof(drw_fixed_contact_t));
    DRAWABLES_FIXED_CONTACT_init(me->contact, me->point.position, me->point.state == CONNECTION_POINT_HIGH, rotation);
    DRAWABLES_enqueue((drawable_t*)me->contact);

    SIM_CONNECTION_POINT_autoConnectWires(&me->point);
    SIM_COMP_LIST_appendConnectionPoint(SIMSPACE_lstConnectionPoints, &me->point);
}

void SIM_FIXED_CONTACT_refreshDrawable(sim_fixed_contact_t *me) {
    me->contact->high = me->point.state == CONNECTION_POINT_HIGH;
}

void SIM_FIXED_CONTACT_unload(sim_fixed_contact_t *me) {
    DRAWABLES_unload((drawable_t*)me->contact);
    SIM_COMP_LIST_removeConnectionPointRef(SIMSPACE_lstConnectionPoints, &me->point);
}