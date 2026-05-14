//
// Created by redcube on 14/05/2026.
//

#ifndef SIM3_SIMSPACELISTS_H
#define SIM3_SIMSPACELISTS_H
#include <stdint.h>
#include "../Components/FixedContact.h"
#include "../Components/Chip.h"

// cyclic references
typedef struct SIM_CONNECTION sim_connection_t;

// ---------------------------------------------------------------------------------------------------------------------
// Dynamic list for all kinds of nonsense
typedef struct SIM_COMP_LIST {
    void **buffer;
    uint32_t bufferSize;
    uint32_t length;
} sim_comp_list_t;

// cheeky renames because im a bih, hihi
typedef sim_comp_list_t sim_connection_list_t;
typedef sim_comp_list_t sim_fixedcontact_list_t;
typedef sim_comp_list_t sim_connectionpoint_list_t;
typedef sim_comp_list_t sim_chip_list_t;

// i will never use c++ templates because i hate c++ with a burning passion
// its a thing of principle
void SIM_COMP_LIST_init(sim_comp_list_t *me);
void SIM_COMP_LIST_append(sim_comp_list_t *me, void *comp);
void SIM_COMP_LIST_insertAt(sim_comp_list_t *me, void *comp, uint32_t index);
void SIM_COMP_LIST_removeAt(sim_comp_list_t *me, uint32_t index);
void SIM_COMP_LIST_removeRef(sim_comp_list_t *me, void *comp);
void SIM_COMP_LIST_grow(sim_comp_list_t *me);
void SIM_COMP_LIST_shrink(sim_comp_list_t *me);
void SIM_COMP_LIST_clear(sim_comp_list_t *me);

// CONNECTIONS
void SIM_COMP_LIST_appendConnection(sim_connection_list_t *me, sim_connection_t *comp);
void SIM_COMP_LIST_insertConnectionAt(sim_connection_list_t *me, sim_connection_t *comp, uint32_t index);
void SIM_COMP_LIST_removeConnectionAt(sim_connection_list_t *me, uint32_t index);
void SIM_COMP_LIST_removeConnectionRef(sim_connection_list_t *me, sim_connection_t *comp);

// FIXED CONTACTS
void SIM_COMP_LIST_appendFixedContact(sim_fixedcontact_list_t *me, sim_fixed_contact_t *comp);
void SIM_COMP_LIST_insertFixedContactAt(sim_fixedcontact_list_t *me, sim_fixed_contact_t *comp, uint32_t index);
void SIM_COMP_LIST_removeFixedContactAt(sim_fixedcontact_list_t *me, uint32_t index);
void SIM_COMP_LIST_removeFixedContactRef(sim_fixedcontact_list_t *me, sim_fixed_contact_t *comp);

// CONNECTION POINT
void SIM_COMP_LIST_appendConnectionPoint(sim_connectionpoint_list_t *me, sim_connection_point_t *comp);
void SIM_COMP_LIST_insertConnectionPointAt(sim_connectionpoint_list_t *me, sim_connection_point_t *comp, uint32_t index);
void SIM_COMP_LIST_removeConnectionPointAt(sim_connectionpoint_list_t *me, uint32_t index);
void SIM_COMP_LIST_removeConnectionPointRef(sim_connectionpoint_list_t *me, sim_connection_point_t *comp);

// CHIP
void SIM_COMP_LIST_appendChip(sim_chip_list_t *me, sim_chip_t *comp);
void SIM_COMP_LIST_insertChipAt(sim_chip_list_t *me, sim_chip_t *comp, uint32_t index);
void SIM_COMP_LIST_removeChipAt(sim_chip_list_t *me, uint32_t index);
void SIM_COMP_LIST_removeChipRef(sim_chip_list_t *me, sim_chip_t *comp);

#endif //SIM3_SIMSPACELISTS_H
