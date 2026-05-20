//
// Created by redcube on 20/05/2026.
//

#include "SaveAndLoad.h"

#include <stdio.h>
#include <json-c/json_object.h>
#include <json-c/json_util.h>

#include "SimulationSpace.h"
#include "Components/Chip.h"

char *SAVE_AND_LOAD_currentProject;

void SAVE_AND_LOAD_init() {
    SAVE_AND_LOAD_currentProject = "./temp.s3";
    SAVE_AND_LOAD_loadSimspace();
}

void SAVE_AND_LOAD_loadSimspace() {
    if (SAVE_AND_LOAD_currentProject == NULL) return;

    // open the savefile as a json object
    json_object *root = json_object_from_file(SAVE_AND_LOAD_currentProject);
    if (root == NULL) {
        printf("Unable to open savefile '%s'!", SAVE_AND_LOAD_currentProject);
        return;
    }

    // find out which file format version this is
    json_object *metadata = json_object_object_get(root, "metadata");
    if (metadata == NULL) {
        printf("Savefile '%s' doesnt follow the S3 format specification!", SAVE_AND_LOAD_currentProject);
        json_object_put(root);
        return;
    }

    int formatVersion = json_object_get_int(json_object_object_get(metadata, "format_version"));
    if (formatVersion != 1) {
        printf("Unknown savefile version %d!", formatVersion);
        json_object_put(root);
        return;
    }

    // load the saved simspace
    SAVE_AND_LOAD_loadSimspace_Format_v1(root);

    // free the json object
    json_object_put(root);
}

void SAVE_AND_LOAD_loadSimspace_Format_v1(json_object *root) {

    json_object *simspace = json_object_object_get(root, "simspace");
    {
        int len;

        // -------------------------------------------------------------------------------------------------------------
        // deserialize all wires
        json_object *connections = json_object_object_get(simspace, "connections");
        len = json_object_array_length(connections);

        for (int i = 0; i < len; ++i) {
            json_object *compJson = json_object_array_get_idx(connections, i);
            SAVE_AND_LOAD_deserializeConnection(compJson);
        }

        // -------------------------------------------------------------------------------------------------------------
        // deserialize all fixed contacts
        json_object *fixedContacts = json_object_object_get(simspace, "fixed_contacts");
        len = json_object_array_length(fixedContacts);

        for (int i = 0; i < len; ++i) {
            json_object *compJson = json_object_array_get_idx(fixedContacts, i);
            SAVE_AND_LOAD_deserializeFixedContact(compJson);
        }

        // -------------------------------------------------------------------------------------------------------------
        // deserialize all LEDs
        json_object *leds = json_object_object_get(simspace, "leds");
        len = json_object_array_length(leds);

        for (int i = 0; i < len; ++i) {
            json_object *compJson = json_object_array_get_idx(leds, i);
            SAVE_AND_LOAD_deserializeLED(compJson);
        }

        // -------------------------------------------------------------------------------------------------------------
        // deserialize all chips
        json_object *chips = json_object_object_get(simspace, "chips");
        len = json_object_array_length(chips);

        for (int i = 0; i < len; ++i) {
            json_object *compJson = json_object_array_get_idx(chips, i);
            SAVE_AND_LOAD_deserializeChip(compJson);
        }
    }

    // make sure all touching wires connect properly
    SIMSPACE_autoconnectToConnectionPoints();
}

void SAVE_AND_LOAD_saveSimspace() {

    // create a new json root object
    json_object *root = json_object_new_object();

    // store some metadata about this file
    json_object *metadata = json_object_new_object();
    {
        json_object_object_add(metadata, "format_version", json_object_new_int(SAVE_FILE_FORMAT_VERSION));
    }
    json_object_object_add(root, "metadata", metadata);

    // store the state of the simulation space
    json_object *simspace = json_object_new_object();
    {

        // -------------------------------------------------------------------------------------------------------------
        // serialize all wires
        json_object *connections = json_object_new_array();
        for (int i = 0; i < SIMSPACE_lstConnections->length; ++i) {
            json_object *conJson = json_object_new_object();
            {
                SAVE_AND_LOAD_serializeConnection(SIMSPACE_lstConnections->buffer[i], conJson);
            }
            json_object_array_add(connections, conJson);
        }
        json_object_object_add(simspace, "connections", connections);

        // -------------------------------------------------------------------------------------------------------------
        // serialize all fixed contacts
        json_object *fixedContacts = json_object_new_array();
        for (int i = 0; i < SIMSPACE_lstFixedContacts->length; ++i) {
            json_object *contactJson = json_object_new_object();
            {
                SAVE_AND_LOAD_serializeFixedContact(SIMSPACE_lstFixedContacts->buffer[i], contactJson);
            }
            json_object_array_add(fixedContacts, contactJson);
        }
        json_object_object_add(simspace, "fixed_contacts", fixedContacts);

        // -------------------------------------------------------------------------------------------------------------
        // serialize all LEDs
        json_object *leds = json_object_new_array();
        for (int i = 0; i < SIMSPACE_lstLEDs->length; ++i) {
            json_object *ledJson = json_object_new_object();
            {
                SAVE_AND_LOAD_serializeLED(SIMSPACE_lstLEDs->buffer[i], ledJson);
            }
            json_object_array_add(leds, ledJson);
        }
        json_object_object_add(simspace, "leds", leds);

        // -------------------------------------------------------------------------------------------------------------
        // serialize all Chips
        json_object *chips = json_object_new_array();
        for (int i = 0; i < SIMSPACE_lstChips->length; ++i) {
            json_object *chipJson = json_object_new_object();
            {
                SAVE_AND_LOAD_serializeChip(SIMSPACE_lstChips->buffer[i], chipJson);
            }
            json_object_array_add(chips, chipJson);
        }
        json_object_object_add(simspace, "chips", chips);

    }
    json_object_object_add(root, "simspace", simspace);

    // ------------------------------------------------------------
    // write it to disk
    json_object_to_file_ext(SAVE_AND_LOAD_currentProject, root, JSON_C_TO_STRING_PRETTY);
    json_object_put(root);
}