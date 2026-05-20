//
// Created by redcube on 20/05/2026.
//
#include "../SaveAndLoad.h"
#include "../Components/Chip.h"

json_object *SAVE_AND_LOAD_serializeVector2(Vector2 vec) {
    json_object *vectorJson = json_object_new_array();
    {
        json_object_array_add(vectorJson, json_object_new_double(vec.x));
        json_object_array_add(vectorJson, json_object_new_double(vec.y));
    }
    return vectorJson;
}

json_object *SAVE_AND_LOAD_serializeColor(Color color) {
    json_object *colorJson = json_object_new_array();
    {
        json_object_array_add(colorJson, json_object_new_int(color.r));
        json_object_array_add(colorJson, json_object_new_int(color.g));
        json_object_array_add(colorJson, json_object_new_int(color.b));
        json_object_array_add(colorJson, json_object_new_int(color.a));
    }
    return colorJson;
}

void SAVE_AND_LOAD_serializeConnection(sim_connection_t *con, json_object *json) {

    json_object *vectorPairList = json_object_new_array();

    // write all vector pairs of this connection into a json list
    for (int i = 0; i < con->lstVectorPairs.length; ++i) {
        sim_vector_pair_t pair = con->lstVectorPairs.buffer[i];

        json_object *vectorPairJson = json_object_new_object();
        {
            json_object_object_add(vectorPairJson, "from", SAVE_AND_LOAD_serializeVector2(pair.from));
            json_object_object_add(vectorPairJson, "to", SAVE_AND_LOAD_serializeVector2(pair.to));
            json_object_object_add(vectorPairJson, "color", SAVE_AND_LOAD_serializeColor(pair.color));
        }
        json_object_array_add(vectorPairList, vectorPairJson);
    }

    json_object_object_add(json, "vector_pairs", vectorPairList);
}

void SAVE_AND_LOAD_serializeFixedContact(sim_fixed_contact_t *contact, json_object *json) {

    json_object_object_add(json, "position", SAVE_AND_LOAD_serializeVector2(contact->point.position));
    json_object_object_add(json, "rotation", json_object_new_int(contact->contact->rotation));
    json_object_object_add(json, "state", json_object_new_boolean(contact->point.state == CONNECTION_POINT_HIGH));
}

void SAVE_AND_LOAD_serializeLED(sim_led_t *led, json_object *json) {

    json_object_object_add(json, "position", SAVE_AND_LOAD_serializeVector2(led->point.position));
    json_object_object_add(json, "rotation", json_object_new_int(led->led->rotation));
}

void SAVE_AND_LOAD_serializeChip(sim_chip_t *chip, json_object *json) {

    json_object_object_add(json, "spec_id", json_object_new_string(chip->chipSpec->id));
    json_object_object_add(json, "position", SAVE_AND_LOAD_serializeVector2(chip->chip->position));

    // store the chips state if needed
    if (!chip->chipSpec->isStateful) {

        // this one got no state
        json_object_object_add(json, "state", json_object_new_string(""));
    }
}