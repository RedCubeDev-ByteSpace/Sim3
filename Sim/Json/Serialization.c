//
// Created by redcube on 20/05/2026.
//
#include <stdio.h>

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

json_object *SAVE_AND_LOAD_serializeConnectionPointStates(sim_comp_list_t *conPoints) {
    json_object *conPointsJson = json_object_new_array();
    for (int i = 0; i < conPoints->length; ++i) {
        sim_connection_point_t *conPoint = conPoints->buffer[i];

        json_object *conPointJson = json_object_new_object();
        json_object_object_add(conPointJson, "pin", json_object_new_int(conPoint->state));
        json_object_object_add(conPointJson, "wire", json_object_new_boolean(conPoint->attachedWireState));

        json_object_array_add(conPointsJson, conPointJson);
    }
    return conPointsJson;
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

    // store the state of the attached pins
    json_object_object_add(json, "pin_states", SAVE_AND_LOAD_serializeConnectionPointStates(&chip->connectionPoints));

    // store the chips state if needed
    if (!chip->chipSpec->isStateful || !chip->luaHasSaveState || chip->luaState == NULL) {

        // this one got no state
        json_object_object_add(json, "state", json_object_new_string(""));
        return;
    }

    // load a reference to the __serialize() function
    lua_getglobal(chip->luaState, "__serialize");

    // load a reference to the SaveState() function
    lua_getglobal(chip->luaState, "SaveState");

    // call the SaveState() function
    if (lua_pcall(chip->luaState, 0, 1, 0) != LUA_OK) {
        printf("SIM_CHIP_step: Function SaveState() in Script '%s' effectively shit itself\n%s\n", chip->chipSpec->script, lua_tostring(chip->luaState, -1));
        lua_close(chip->luaState);
        chip->luaState = NULL;
        return;
    }

    // call the __serialize() function
    if (lua_pcall(chip->luaState, 1, 1, 0) != LUA_OK) {
        printf("SIM_CHIP_step: Function __serialize() in Script '%s' effectively shit itself\n%s\n", chip->chipSpec->script, lua_tostring(chip->luaState, -1));
        lua_close(chip->luaState);
        chip->luaState = NULL;
        return;
    }

    // store the state
    json_object_object_add(json, "state", json_object_new_string(lua_tostring(chip->luaState, -1)));
    lua_pop(chip->luaState, 1);
}