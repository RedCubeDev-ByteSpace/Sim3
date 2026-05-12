//
// Created by redcube on 10/05/2026.
//
#include "Bench.h"

#include <math.h>
#include <stdlib.h>

#include "GUI.h"
#include "../Sim/SimulationSpace.h"
#include "Drawables/Wire.h"
#include "../Sim/Components/Connection.h"

// ---------------------------------------------------------------------------------------------------------------------
bench_working_mode_t BENCH_benchMode;
float rotation;
// ---------------------------------------------------------------------------------------------------------------------
// Data for wire drawing mode
Vector2 attachmentVertex;
bool hasAttachmentVertex;
sim_connection_t *thisConnection;
// ---------------------------------------------------------------------------------------------------------------------
// Data for contact placment mode
drw_fixed_contact_t *newContact;

void BENCH_init() {
    BENCH_benchMode = IDLE;
    rotation = 0;

    thisConnection = NULL;
    hasAttachmentVertex = false;

    newContact = malloc(sizeof(drw_fixed_contact_t));
    DRAWABLES_FIXED_CONTACT_init(newContact, (Vector2){0,0}, false);
}

void BENCH_process() {
    Vector2 mousePos = GetMousePosition();
    Vector2 pos = LIB_screenSpaceToWorldSpace(mousePos);
    Vector2 wpos;
    wpos.x = (int)(pos.x + 0.5f);
    wpos.y = (int)(pos.y + 0.5f);

    // buttons
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

        // "Wire Mode" Button
        if (LIB_IsVector2InRectangle(mousePos, (Rectangle){ 5, 5, 30, 30})) {

            // if we're not currently in wire mode -> go there
            if (BENCH_benchMode != DRAWING_WIRE) {
                BENCH_benchMode = DRAWING_WIRE;
                thisConnection = NULL;
            }

            // otherwise, if we are: switch to idle
            else {
                BENCH_benchMode = IDLE;
            }

            return;
        }

        // "Fixed Contact Place" Button
        if (LIB_IsVector2InRectangle(mousePos, (Rectangle){ 5, 40, 30, 30})) {

            // if we're not currently in wire mode -> go there
            if (BENCH_benchMode != PLACE_FIXED_CONTACT) {
                BENCH_benchMode = PLACE_FIXED_CONTACT;
            }

            // otherwise, if we are: switch to idle
            else {
                BENCH_benchMode = IDLE;
            }

            return;
        }
    }

    // -------------------------------------------------------------------------------------------
    // Idle mode
    if (BENCH_benchMode == IDLE) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            // Flip any fixed contacts when theyre clicked on
            for (int i = 0; i < MAX_CONNECTIONS; ++i) {
                if (SIMSPACE_fixedContacts[i] == NULL) continue;

                if (LIB_IsVector2InRectangle(mousePos, DRAWABLES_FIXED_CONTACT_getInteractionRect(SIMSPACE_fixedContacts[i]->contact))) {
                    sim_connection_point_state_t state = SIMSPACE_fixedContacts[i]->point->state;

                    if (state == CONNECTION_POINT_HIGH)
                        SIMSPACE_fixedContacts[i]->point->state = CONNECTION_POINT_LOW;
                    else
                        SIMSPACE_fixedContacts[i]->point->state = CONNECTION_POINT_HIGH;

                    SIM_FIXED_CONTACT_refreshDrawable(SIMSPACE_fixedContacts[i]);
                }
            }
        }
    }

    // -------------------------------------------------------------------------------------------
    // Wire drawing mode
    if (BENCH_benchMode == DRAWING_WIRE) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            // is this a known connection or are we starting a new one?
            if (thisConnection == NULL) {

                // no connection yet
                // are we trying to continue a connection by clicking on its last vertex?
                for (int i = 0; i < MAX_CONNECTIONS; ++i) {
                    if (SIMSPACE_connections[i] == NULL) continue;
                    if (SIMSPACE_connections[i]->lstConnectionPoints.length < 2) continue;

                    // have we clicked on the first vertex of this connection?
                    Vector2 first = SIMSPACE_connections[i]->lstConnectionPoints.buffer[0];
                    if (first.x == wpos.x && first.y == wpos.y) {

                        // is it connected to a connection point? in that case it cannot be extended
                        if (SIMSPACE_connections[i]->pointA != NULL) return;

                        // if so, select it
                        thisConnection = SIMSPACE_connections[i];
                        attachmentVertex = first;
                        hasAttachmentVertex = true;
                        return;
                    }

                    // have we clicked on the last vertex of this connection?
                    Vector2 last = SIMSPACE_connections[i]->lstConnectionPoints.buffer[SIMSPACE_connections[i]->lstConnectionPoints.length-1];
                    if (last.x == wpos.x && last.y == wpos.y) {

                        // is it connected to a connection point? in that case it cannot be extended
                        if (SIMSPACE_connections[i]->pointB != NULL) return;

                        // if so, select it
                        thisConnection = SIMSPACE_connections[i];
                        attachmentVertex = last;
                        hasAttachmentVertex = true;
                        return;
                    }
                }

                // if we didnt find any existing connection -> create a new one
                thisConnection = malloc(sizeof(sim_connection_t));
                SIM_CONNECTION_init(thisConnection, wireColors[0]);
                SIMSPACE_addConnection(thisConnection);

                // add this vertex to it
                SIM_CONNECTION_POINT_LIST_append(&thisConnection->lstConnectionPoints, wpos);

                // use it as attachment
                attachmentVertex = wpos;
                hasAttachmentVertex = true;

                // is it attached to any connection points??
                for (int i = 0; i < MAX_CONNECTIONS; ++i) {
                    if (SIMSPACE_connectionPoints[i] == NULL) continue;
                    if (SIMSPACE_connectionPoints[i]->position.x == wpos.x && SIMSPACE_connectionPoints[i]->position.y == wpos.y) {
                        thisConnection->pointA = SIMSPACE_connectionPoints[i];
                        break;
                    }
                }

                return;
            }
        }

        // if this is already part of a connection -> expand it
        if ((IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) && thisConnection != NULL) {

            // have we hit any connection points?
            sim_connection_point_t *conPoint = NULL;
            for (int i = 0; i < MAX_CONNECTIONS; ++i) {
                if (SIMSPACE_connectionPoints[i] == NULL) continue;
                if (SIMSPACE_connectionPoints[i]->position.x == wpos.x && SIMSPACE_connectionPoints[i]->position.y == wpos.y) {
                    conPoint = SIMSPACE_connectionPoints[i];
                    break;
                }
            }

            // if the connection only has one point so far -> just append this one
            if (thisConnection->lstConnectionPoints.length == 1) {
                SIM_CONNECTION_POINT_LIST_append(&thisConnection->lstConnectionPoints, wpos);
                thisConnection->pointB = conPoint;
            }

            // alright alright
            // we already have some vertices so we need to figure out if this goes in the front or the back of the wire
            else {
                // is the attachment vertex the first one in the list?
                Vector2 first = thisConnection->lstConnectionPoints.buffer[0];
                if (first.x == attachmentVertex.x && first.y == attachmentVertex.y) {

                    // add it in position 0
                    SIM_CONNECTION_POINT_LIST_insertAt(&thisConnection->lstConnectionPoints, wpos, 0);
                    thisConnection->pointA = conPoint;
                }
                else {
                    // otherwise add it at the last position
                    SIM_CONNECTION_POINT_LIST_append(&thisConnection->lstConnectionPoints, wpos);
                    thisConnection->pointB = conPoint;
                }
            }

            // rebuild and redraw
            SIM_CONNECTION_refreshDrawablesStructure(thisConnection);

            // if this was a right click -> directly add this vertex as the new attachment point
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && conPoint == NULL) {
                attachmentVertex = wpos;
            }
            // otherwise: exit out of wire drawing mode
            else {
                hasAttachmentVertex = false;
                BENCH_benchMode = IDLE;
            }
        }
    }

    // -------------------------------------------------------------------------------------------
    // Fixed contact placement mode
    if (BENCH_benchMode == PLACE_FIXED_CONTACT) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            // make sure there isnt already a contact at this position
            for (int i = 0; i < MAX_CONNECTIONS; ++i) {
                if (SIMSPACE_connectionPoints[i] == NULL) continue;
                if (SIMSPACE_connectionPoints[i]->position.x == wpos.x && SIMSPACE_connectionPoints[i]->position.y == wpos.y) {
                    // this space is already occupied!!
                    return;
                }
            }

            // create a new fixed contact at this position
            sim_fixed_contact_t *contact = malloc(sizeof(sim_fixed_contact_t));
            SIM_FIXED_CONTACT_init(contact, wpos, false);
            SIMSPACE_addFixedContact(contact);
        }
    }
}

void BENCH_draw() {

    rotation += GetFrameTime() * 2;
    rotation = rotation > 360 ? 0 : rotation;

    Vector2 mousePos = GetMousePosition();
    Vector2 pos = LIB_screenSpaceToWorldSpace(mousePos);
    pos.x = (int)(pos.x + 0.5f);
    pos.y = (int)(pos.y + 0.5f);

    // ----------------------------
    // Draw Cursors

    Vector2 wpos = LIB_worldSpaceToScreenSpace(pos);
    switch (BENCH_benchMode) {
        case DRAWING_WIRE:
            DrawLineEx((Vector2){wpos.x + cos(rotation) * 5, wpos.y + sin(rotation) * 5}, (Vector2){wpos.x - cos(rotation) * 5, wpos.y - sin(rotation) * 5}, 1, wireColors[0]);
            DrawLineEx((Vector2){wpos.x + cos(rotation + PI/2) * 5, wpos.y + sin(rotation + PI/2) * 5}, (Vector2){wpos.x - cos(rotation + PI/2) * 5, wpos.y - sin(rotation + PI/2) * 5}, 1, wireColors[0]);
        break;

        case PLACE_FIXED_CONTACT:
            newContact->position = pos;
            DRAWABLES_FIXED_CONTACT_draw(newContact);
        break;

        default:
            DrawLineEx((Vector2){mousePos.x, mousePos.y - 5}, (Vector2){mousePos.x, mousePos.y + 5}, 1, wireColors[0]);
            DrawLineEx((Vector2){mousePos.x - 5, mousePos.y}, (Vector2){mousePos.x + 5, mousePos.y}, 1, wireColors[0]);
        break;
    }

    // ----------------------------
    // Mode specific UI
    if (BENCH_benchMode == DRAWING_WIRE) {
        if (hasAttachmentVertex)
            DrawLineEx(LIB_worldSpaceToScreenSpace(attachmentVertex), wpos, 0.5f, wireColors[0]);
    }


    // ----------------------------
    // Draw Buttons

    // Line enable button
    DrawRectangleRounded((Rectangle){
        5, 5, 30, 30
    }, 0.1f, 3, BENCH_benchMode == DRAWING_WIRE ? BLACK : WHITE);

    DrawLineEx((Vector2){10, 10}, (Vector2){30, 30}, 2, BENCH_benchMode == DRAWING_WIRE ? WHITE : BLACK);

    DrawRectangleRoundedLines((Rectangle){
        5, 5, 30, 30
    }, 0.1f, 3, 1, BLACK);

    // Fixed contact enable button
    DrawRectangleRounded((Rectangle){
        5, 40, 30, 30
    }, 0.1f, 3, BENCH_benchMode == PLACE_FIXED_CONTACT ? BLACK : WHITE);

    DrawRectangleLinesEx((Rectangle){
        10, 45, 20, 20
    }, 2, BENCH_benchMode == PLACE_FIXED_CONTACT ? WHITE : BLACK);

    DrawCircle(20, 55, 5, BENCH_benchMode == PLACE_FIXED_CONTACT ? WHITE : BLACK);

    DrawRectangleRoundedLines((Rectangle){
        5, 40, 30, 30
    }, 0.1f, 3, 1, BLACK);
}