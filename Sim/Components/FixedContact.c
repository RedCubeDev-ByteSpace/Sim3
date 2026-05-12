//
// Created by redcube on 13/05/2026.
//

#include "FixedContact.h"

#include <stdlib.h>

void SIM_FIXED_CONTACT_init(sim_fixed_contact_t *me, Vector2 position, bool state) {
    me->point = malloc(sizeof(sim_connection_point_t));
    me->point->position = position;
    me->point->state = state ? CONNECTION_POINT_HIGH : CONNECTION_POINT_LOW;

    me->contact = malloc(sizeof(drw_fixed_contact_t));
    DRAWABLES_FIXED_CONTACT_init(me->contact, me->point->position, me->point->state == CONNECTION_POINT_HIGH);
    DRAWABLES_enqueue((drawable_t*)me->contact);
}

void SIM_FIXED_CONTACT_refreshDrawable(sim_fixed_contact_t *me) {
    me->contact->high = me->point->state == CONNECTION_POINT_HIGH;
}