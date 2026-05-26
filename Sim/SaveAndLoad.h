//
// Created by redcube on 20/05/2026.
//

#ifndef SIM3_SAVEANDLOAD_H
#define SIM3_SAVEANDLOAD_H

#define SAVE_FILE_FORMAT_VERSION 1
#include "Components/Connection.h"
extern char *SAVE_AND_LOAD_currentProject;

void SAVE_AND_LOAD_init();
void SAVE_AND_LOAD_close();
void SAVE_AND_LOAD_save();
void SAVE_AND_LOAD_saveAs();
void SAVE_AND_LOAD_load();
void SAVE_AND_LOAD_loadSimspace();
void SAVE_AND_LOAD_loadSimspace_Format_v1(json_object *root);
void SAVE_AND_LOAD_saveSimspace();

// ---------------------------------------------------------------------------------------------------------------------
// Component serialization functions
json_object *SAVE_AND_LOAD_serializeVector2(Vector2 vec);
json_object *SAVE_AND_LOAD_serializeColor(Color color);
json_object *SAVE_AND_LOAD_serializeConnectionPointStates(sim_comp_list_t *conPoints);
void SAVE_AND_LOAD_serializeConnection(sim_connection_t *con, json_object *json);
void SAVE_AND_LOAD_serializeFixedContact(sim_fixed_contact_t *contact, json_object *json);
void SAVE_AND_LOAD_serializeLED(sim_led_t *led, json_object *json);
void SAVE_AND_LOAD_serializeChip(sim_chip_t *chip, json_object *json);

// ---------------------------------------------------------------------------------------------------------------------
// Component deserialization functions
Vector2 SAVE_AND_LOAD_deserializeVector2(json_object *json);
Color SAVE_AND_LOAD_deserializeColor(json_object *json);
void SAVE_AND_LOAD_deserializeConnectionPointStates(json_object *json, sim_chip_t *chip);
void SAVE_AND_LOAD_deserializeConnection(json_object *json);
void SAVE_AND_LOAD_deserializeFixedContact(json_object *json);
void SAVE_AND_LOAD_deserializeLED(json_object *json);
void SAVE_AND_LOAD_deserializeChip(json_object *json);

#endif //SIM3_SAVEANDLOAD_H
