//
// Created by redcube on 12/05/2026.
//
#include "SimulationSpace.h"

#include <stddef.h>
#include <stdlib.h>

#include "Components/Chip.h"
#include "Components/Connection.h"

sim_connection_list_t *SIMSPACE_lstConnections;
sim_connectionpoint_list_t *SIMSPACE_lstConnectionPoints;
sim_fixedcontact_list_t *SIMSPACE_lstFixedContacts;
sim_led_list_t *SIMSPACE_lstLEDs;
sim_chip_list_t *SIMSPACE_lstChips;

void SIMSPACE_init() {
    SIMSPACE_lstConnections = malloc(sizeof(sim_comp_list_t));
    SIMSPACE_lstConnectionPoints = malloc(sizeof(sim_comp_list_t));
    SIMSPACE_lstFixedContacts = malloc(sizeof(sim_comp_list_t));
    SIMSPACE_lstLEDs = malloc(sizeof(sim_led_list_t));
    SIMSPACE_lstChips = malloc(sizeof(sim_chip_list_t));

    SIM_COMP_LIST_init(SIMSPACE_lstConnections);
    SIM_COMP_LIST_init(SIMSPACE_lstConnectionPoints);
    SIM_COMP_LIST_init(SIMSPACE_lstFixedContacts);
    SIM_COMP_LIST_init(SIMSPACE_lstLEDs);
    SIM_COMP_LIST_init(SIMSPACE_lstChips);
}

void SIMSPACE_process() {

    // Update all wires
    for (int i = 0; i < SIMSPACE_lstConnections->length; ++i) {
        SIM_CONNECTION_refreshState(SIMSPACE_lstConnections->buffer[i]);
    }

    // Update all LEDs
    for (int i = 0; i < SIMSPACE_lstLEDs->length; ++i) {
        SIM_LED_refreshDrawable(SIMSPACE_lstLEDs->buffer[i]);
    }

    // Update all chipificators
    for (int i = 0; i < SIMSPACE_lstChips->length; ++i) {
        SIM_CHIP_step(SIMSPACE_lstChips->buffer[i]);
    }
}

void SIMSPACE_mergeOverlappingConnections() {

    bool hasDoneMerge;
    while (true) {
        hasDoneMerge = false; // our exit flag

        // go through all wires
        for (int i = 0; i < SIMSPACE_lstConnections->length; ++i) {
            sim_connection_t *outerCon = SIMSPACE_lstConnections->buffer[i];

            // go through all vector pairs of this connection
            for (int ii = 0; ii < outerCon->lstVectorPairs.length; ++ii) {
                sim_vector_pair_t outerPair = outerCon->lstVectorPairs.buffer[ii];

                // again, go through all known connections
                for (int j = 0; j < SIMSPACE_lstConnections->length; ++j) {
                    sim_connection_t *innerCon = SIMSPACE_lstConnections->buffer[j];

                    // skip, if were just comparing something with itself
                    if (outerCon == innerCon) continue;

                    // go through all vector pairs of the inner con
                    for (int jj = 0; jj < innerCon->lstVectorPairs.length; ++jj) {
                        sim_vector_pair_t innerPair = innerCon->lstVectorPairs.buffer[jj];

                        // check if any of the vectors match -> that would mean that these two connections should
                        // be attached to eachother

                        if (
                            // OUTER From = INNER From
                            (outerPair.from.x == innerPair.from.x) && (outerPair.from.y == innerPair.from.y)
                            ||
                            // OUTER From = INNER To
                            (outerPair.from.x == innerPair.to.x) && (outerPair.from.y == innerPair.to.y)
                            ||
                            // OUTER To = INNER From
                            (outerPair.to.x == innerPair.from.x) && (outerPair.to.y == innerPair.from.y)
                            ||
                            // OUTER To = INNER To
                            (outerPair.to.x == innerPair.to.x) && (outerPair.to.y == innerPair.to.y)
                            ) {

                            // if that is the case
                            // -> mergificate

                            // add all the inner connections vector pairs to the outer connection
                            for (int vpIndex = 0; vpIndex < innerCon->lstVectorPairs.length; ++vpIndex) {
                                SIM_CONNECTION_VECTOR_PAIR_LIST_append(&outerCon->lstVectorPairs, innerCon->lstVectorPairs.buffer[vpIndex]);
                            }

                            // add all the inner connections connected points to the outer connection
                            for (int cpIndex = 0; cpIndex < innerCon->lstConnectedPoints.length; ++cpIndex) {
                                SIM_COMP_LIST_appendConnectionPoint(&outerCon->lstConnectedPoints, innerCon->lstConnectedPoints.buffer[cpIndex]);
                            }

                            // destroy the inner connection
                            SIM_CONNECTION_unload(innerCon);
                            SIM_COMP_LIST_removeConnectionRef(SIMSPACE_lstConnections, innerCon);
                            free(innerCon);

                            // refresh the outer connection
                            SIM_CONNECTION_refreshDrawablesStructure(outerCon);

                            // set the flag and do it all again until theres nothing left to be merged
                            hasDoneMerge = true;
                            goto next_merge_con;
                        }

                    }
                }

            }

        }

        next_merge_con:
        if (!hasDoneMerge) break;
    }

}