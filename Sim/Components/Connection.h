//
// Created by redcube on 12/05/2026.
//

#ifndef SIM3_CONNECTION_H
#define SIM3_CONNECTION_H
#include <stdint.h>

#include "raylib.h"
#include "ConnectionPoint.h"
#include "../../GUI/Drawables/Wire.h"

#define COLOR_ACTIVE (Color){255,0,0,255}
#define COLOR_ERROR  (Color){255,0,0,255}

// ---------------------------------------------------------------------------------------------------------------------
// A dynamic list of points making up a connection
typedef struct SIM_CONNECTION_POINT_LIST {
    Vector2 *buffer;
    uint32_t bufferSize;
    uint32_t length;
} sim_connection_point_list_t;

void SIM_CONNECTION_POINT_LIST_init(sim_connection_point_list_t *me);
void SIM_CONNECTION_POINT_LIST_append(sim_connection_point_list_t *me, Vector2 newPoint);
void SIM_CONNECTION_POINT_LIST_insertAt(sim_connection_point_list_t *me, Vector2 newPoint, uint32_t index);
void SIM_CONNECTION_POINT_LIST_insertAfter(sim_connection_point_list_t *me, Vector2 oldPoint, Vector2 newPoint);
void SIM_CONNECTION_POINT_LIST_removeAt(sim_connection_point_list_t *me, uint32_t index);
void SIM_CONNECTION_POINT_LIST_remove(sim_connection_point_list_t *me, Vector2 oldPoint);
void SIM_CONNECTION_POINT_LIST_clear(sim_connection_point_list_t *me);
void SIM_CONNECTION_POINT_LIST_grow(sim_connection_point_list_t *me);
void SIM_CONNECTION_POINT_LIST_shrink(sim_connection_point_list_t *me);

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

    sim_connection_point_t *pointA;
    sim_connection_point_t *pointB;

    Color color;
    bool active;
    bool error;

    sim_connection_point_list_t lstConnectionPoints;
    sim_connection_drawable_list_t lstDrawables;

} sim_connection_t;

void SIM_CONNECTION_init(sim_connection_t *me, Color wireColor);
void SIM_CONNECTION_refreshState(sim_connection_t *me);
void SIM_CONNECTION_refreshDrawablesState(sim_connection_t *me);
void SIM_CONNECTION_refreshDrawablesStructure(sim_connection_t *me);
void SIM_CONNECTION_clearDrawables(sim_connection_t *me);

#endif //SIM3_CONNECTION_H
