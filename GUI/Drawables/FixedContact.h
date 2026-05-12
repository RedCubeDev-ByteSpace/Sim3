//
// Created by redcube on 13/05/2026.
//

#ifndef SIM3_FIXEDCONTACT_H
#define SIM3_FIXEDCONTACT_H

#include <raylib.h>
#include "../Drawables.h"

typedef struct DRW_FIXED_CONTACT {
    drawable_t base;

    bool high;
    Vector2 position;

} drw_fixed_contact_t;

void DRAWABLES_FIXED_CONTACT_init(drw_fixed_contact_t *me, Vector2 pos, bool high);
void DRAWABLES_FIXED_CONTACT_draw(drw_fixed_contact_t *me);
Rectangle DRAWABLES_FIXED_CONTACT_getInteractionRect(drw_fixed_contact_t *me);

#endif //SIM3_FIXEDCONTACT_H
