//
// Created by redcube on 12/05/2026.
//

#ifndef SIM3_CONNECTION_H
#define SIM3_CONNECTION_H
#include <stdint.h>
#include <json-c/json_object.h>

#include "raylib.h"
#include "../SimLists/SimspaceLists.h"
#include "../../GUI/Drawables/Wire.h"

#define COLOR_ACTIVE (Color){255,0,0,255}
#define COLOR_ERROR  (Color){255,0,0,255}

typedef struct SIM_VECTOR_PAIR {
    Vector2 from;
    Vector2 to;
    Color color;
} sim_vector_pair_t;

typedef struct SIM_VECTOR_RECORD {
    Vector2 vec;
    Color color;
    int amount;
} sim_vector_record_t;

void SIM_VECTOR_RECORD_registerVector(sim_vector_record_t *records, Vector2 vec, Color color, int maxNum);

// ---------------------------------------------------------------------------------------------------------------------
// a dynamic list of vectors between vector pairs that other wires can connect to
typedef struct SIM_CONNECTABLE_VECTOR {
    Vector2 position;
    int vectorPairIndex;
} sim_connectable_vector_t;

typedef struct SIM_CONNECTION_CONNECTABLE_VECTOR_LIST {
    sim_connectable_vector_t *buffer;
    uint32_t bufferSize;
    uint32_t length;
} sim_connection_connectable_vector_list_t;

void SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_init(sim_connection_connectable_vector_list_t *me);
void SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_append(sim_connection_connectable_vector_list_t *me, Vector2 vec, int vectorPairIndex);
void SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_grow(sim_connection_connectable_vector_list_t *me);
void SIM_CONNECTION_CONNECTABLE_VECTOR_LIST_clear(sim_connection_connectable_vector_list_t *me);

// ---------------------------------------------------------------------------------------------------------------------
// A dynamic list of vector pairs making up a connection
typedef struct SIM_CONNECTION_VECTOR_PAIR_LIST {
    sim_vector_pair_t *buffer;
    uint32_t bufferSize;
    uint32_t length;
} sim_connection_vector_pair_list_t;

void SIM_CONNECTION_VECTOR_PAIR_LIST_init(sim_connection_vector_pair_list_t *me);
void SIM_CONNECTION_VECTOR_PAIR_LIST_append(sim_connection_vector_pair_list_t *me, sim_vector_pair_t newPair);
void SIM_CONNECTION_VECTOR_PAIR_LIST_insertAt(sim_connection_vector_pair_list_t *me, sim_vector_pair_t newPair, uint32_t index);
void SIM_CONNECTION_VECTOR_PAIR_LIST_removeAt(sim_connection_vector_pair_list_t *me, uint32_t index);
void SIM_CONNECTION_VECTOR_PAIR_LIST_remove(sim_connection_vector_pair_list_t *me, sim_vector_pair_t oldPair);
void SIM_CONNECTION_VECTOR_PAIR_LIST_clear(sim_connection_vector_pair_list_t *me);
void SIM_CONNECTION_VECTOR_PAIR_LIST_grow(sim_connection_vector_pair_list_t *me);
void SIM_CONNECTION_VECTOR_PAIR_LIST_shrink(sim_connection_vector_pair_list_t *me);

// ---------------------------------------------------------------------------------------------------------------------
// A dynamic list of drawables making up a connection
typedef struct SIM_CONNECTION_DRAWABLE_LIST {
    drawable_t **buffer;
    uint32_t bufferSize;
    uint32_t length;
} sim_connection_drawable_list_t;

void SIM_CONNECTION_DRAWABLE_LIST_init(sim_connection_drawable_list_t *me);
void SIM_CONNECTION_DRAWABLE_LIST_add(sim_connection_drawable_list_t *me, drawable_t *newWire);
void SIM_CONNECTION_DRAWABLE_LIST_grow(sim_connection_drawable_list_t *me);
void SIM_CONNECTION_DRAWABLE_LIST_clear(sim_connection_drawable_list_t *me);

// ---------------------------------------------------------------------------------------------------------------------
typedef struct SIM_CONNECTION {

    sim_connectionpoint_list_t lstConnectedPoints;

    bool active;
    bool error;

    sim_connection_vector_pair_list_t lstVectorPairs;
    sim_connection_drawable_list_t lstDrawables;
    sim_connection_connectable_vector_list_t lstConnectableVectors;

} sim_connection_t;

void SIM_CONNECTION_init(sim_connection_t *me);
void SIM_CONNECTION_unload(sim_connection_t *me);
void SIM_CONNECTION_refreshState(sim_connection_t *me);
void SIM_CONNECTION_refreshDrawablesState(sim_connection_t *me);
void SIM_CONNECTION_refreshDrawablesStructure(sim_connection_t *me);
void SIM_CONNECTION_calculateConnectableVectors(sim_connection_t *me, Vector2 pointA, Vector2 pointB, int index);
void SIM_CONNECTION_clearDrawables(sim_connection_t *me);

#endif //SIM3_CONNECTION_H
