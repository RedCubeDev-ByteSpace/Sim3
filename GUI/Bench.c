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
// Data for contact placement mode
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
    wpos = LIB_roundificateToWholePoint(pos);

    // -----------------------------------------------------------------------------------------------------------------
    // ESC shortcut to exit out of all modes
    if (IsKeyReleased(KEY_ESCAPE)) {
        BENCH_benchMode = IDLE;
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Mode selection buttons
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

        // "Wire Mode" Button
        if (LIB_IsVector2InRectangle(mousePos, (Rectangle){ 5, 5, 30, 30})) {

            // if we're not currently in wire mode -> go there
            if (BENCH_benchMode != DRAWING_WIRE) {
                BENCH_benchMode = DRAWING_WIRE;

                hasAttachmentVertex = false;

                if (thisConnection != NULL && thisConnection->lstConnectedPoints.length == 0) {
                    SIM_CONNECTION_unload(thisConnection);
                }
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
            for (int i = 0; i < SIMSPACE_lstFixedContacts->length; ++i) {
                sim_fixed_contact_t *contact = SIMSPACE_lstFixedContacts->buffer[i];

                // did the mouse hit the interaction rect of this contact?
                if (LIB_IsVector2InRectangle(mousePos, DRAWABLES_FIXED_CONTACT_getInteractionRect(contact->contact))) {

                    // flippificate it!!!
                    sim_connection_point_state_t state = contact->point.state;

                    if (state == CONNECTION_POINT_HIGH)
                        contact->point.state = CONNECTION_POINT_LOW;
                    else
                        contact->point.state = CONNECTION_POINT_HIGH;

                    // refresh please :3
                    SIM_FIXED_CONTACT_refreshDrawable(contact);
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
                // are we trying to attach to an existing connection?
                for (int i = 0; i < SIMSPACE_lstConnections->length; ++i) {
                    sim_connection_t *con = SIMSPACE_lstConnections->buffer[i];
                    if (con->lstVectorPairs.length == 0) continue;

                    for (int ii = 0; ii < con->lstVectorPairs.length; ++ii) {
                        Vector2 from = con->lstVectorPairs.buffer[ii].from;
                        Vector2 to = con->lstVectorPairs.buffer[ii].to;

                        if (from.x == wpos.x && from.y == wpos.y) {
                            attachmentVertex = from;
                            hasAttachmentVertex = true;
                            thisConnection = con;
                            return;
                        }

                        if (to.x == wpos.x && to.y == wpos.y) {
                            attachmentVertex = to;
                            hasAttachmentVertex = true;
                            thisConnection = con;
                            return;
                        }
                    }
                }

                // if we didnt find any existing connection -> create a new one
                thisConnection = malloc(sizeof(sim_connection_t));
                SIM_CONNECTION_init(thisConnection, wireColors[0]);

                // use it as attachment
                attachmentVertex = wpos;
                hasAttachmentVertex = true;

                // is it attached to any connection points??
                for (int i = 0; i < SIMSPACE_lstConnectionPoints->length; ++i) {
                    sim_connection_point_t *conPoint = SIMSPACE_lstConnectionPoints->buffer[i];
                    if (conPoint->position.x == wpos.x && conPoint->position.y == wpos.y) {

                        SIM_COMP_LIST_appendConnectionPoint(&thisConnection->lstConnectedPoints, conPoint);
                        break;
                    }
                }

                return;
            }
        }

        // if this is already part of a connection -> expand it
        if ((IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) && thisConnection != NULL) {

            if (attachmentVertex.x == wpos.x && attachmentVertex.y == wpos.y) return; // bro this doesnt count

            // have we hit any connection points?
            sim_connection_point_t *conPoint = NULL;
            for (int i = 0; i < SIMSPACE_lstConnectionPoints->length; ++i) {
                sim_connection_point_t *_conPoint = SIMSPACE_lstConnectionPoints->buffer[i];
                if (_conPoint->position.x == wpos.x && _conPoint->position.y == wpos.y) {
                    conPoint = _conPoint;
                    break;
                }
            }

            // if this is a brand new connection -> add it to the simulation space!
            if (thisConnection->lstVectorPairs.length == 0) {
                SIM_COMP_LIST_appendConnection(SIMSPACE_lstConnections, thisConnection);
            }

            // append a new vector pair!
            SIM_CONNECTION_VECTOR_PAIR_LIST_append(&thisConnection->lstVectorPairs, (sim_vector_pair_t){attachmentVertex, wpos});
            if (conPoint != NULL) {
                SIM_COMP_LIST_appendConnectionPoint(&thisConnection->lstConnectedPoints, conPoint);
            }

            // rebuild and redraw
            SIM_CONNECTION_refreshDrawablesStructure(thisConnection);

            // if this was a right click -> directly add this vertex as the new attachment point
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && conPoint == NULL) {
                attachmentVertex = wpos;
            }
            // otherwise: just finish this wire
            else {
                hasAttachmentVertex = false;
                thisConnection = NULL;
                //BENCH_benchMode = IDLE;
            }
        }
    }

    // -------------------------------------------------------------------------------------------
    // Fixed contact placement mode
    if (BENCH_benchMode == PLACE_FIXED_CONTACT) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            // make sure there isnt already a contact at this position
            for (int i = 0; i < SIMSPACE_lstConnectionPoints->length; ++i) {
                sim_connection_point_t *conPoint = SIMSPACE_lstConnectionPoints->buffer[i];
                if (conPoint->position.x == wpos.x && conPoint->position.y == wpos.y) {
                    // this space is already occupied!!
                    return;
                }
            }

            // create a new fixed contact at this position
            sim_fixed_contact_t *contact = malloc(sizeof(sim_fixed_contact_t));
            SIM_FIXED_CONTACT_init(contact, wpos, false);
            SIM_COMP_LIST_appendFixedContact(SIMSPACE_lstFixedContacts, contact);
        }
    }
}

void BENCH_draw() {

    rotation += GetFrameTime() * 2;
    rotation = rotation > 360 ? 0 : rotation;

    Vector2 mousePos = GetMousePosition();
    Vector2 pos = LIB_screenSpaceToWorldSpace(mousePos);
    pos = LIB_roundificateToWholePoint(pos);

    // -----------------------------------------------------------------------------------------------------------------
    // Draw Cursors

    Vector2 wpos = LIB_worldSpaceToScreenSpace(pos);
    switch (BENCH_benchMode) {
        case DRAWING_WIRE:
            DrawLineEx((Vector2){wpos.x + cos(rotation) * CURSOR_WIDTH, wpos.y + sin(rotation) * CURSOR_WIDTH}, (Vector2){wpos.x - cos(rotation) * CURSOR_WIDTH, wpos.y - sin(rotation) * CURSOR_WIDTH}, CURSOR_THICKNESS, wireColors[0]);
            DrawLineEx((Vector2){wpos.x + cos(rotation + PI/2) * CURSOR_WIDTH, wpos.y + sin(rotation + PI/2) * CURSOR_WIDTH}, (Vector2){wpos.x - cos(rotation + PI/2) * CURSOR_WIDTH, wpos.y - sin(rotation + PI/2) * CURSOR_WIDTH}, CURSOR_THICKNESS, wireColors[0]);
        break;

        case PLACE_FIXED_CONTACT:
            newContact->position = pos;
            DRAWABLES_FIXED_CONTACT_draw(newContact);
        break;

        default:
            DrawLineEx((Vector2){mousePos.x, mousePos.y - CURSOR_WIDTH}, (Vector2){mousePos.x, mousePos.y + CURSOR_WIDTH}, CURSOR_THICKNESS, wireColors[0]);
            DrawLineEx((Vector2){mousePos.x - CURSOR_WIDTH, mousePos.y}, (Vector2){mousePos.x + CURSOR_WIDTH, mousePos.y}, CURSOR_THICKNESS, wireColors[0]);
        break;
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Mode specific UI
    if (BENCH_benchMode == DRAWING_WIRE) {
        if (hasAttachmentVertex)
            DrawLineEx(LIB_worldSpaceToScreenSpace(attachmentVertex), wpos, 0.5f, wireColors[0]);
    }


    // -----------------------------------------------------------------------------------------------------------------
    // Draw Buttons

    // ---------------------------
    // Line enable button
    bool fillLineButton = BENCH_benchMode == DRAWING_WIRE || IsKeyDown(KEY_ESCAPE);
    DrawRectangleRounded((Rectangle){
        5, 5, 30, 30
    }, 0.1f, 3, fillLineButton ? BLACK : WHITE);

    DrawLineEx((Vector2){10, 10}, (Vector2){30, 30}, 2, fillLineButton ? WHITE : BLACK);

    DrawRectangleRoundedLines((Rectangle){
        5, 5, 30, 30
    }, 0.1f, 3, 1, BLACK);


    // ---------------------------
    // Fixed contact enable button
    bool fillFixedContactButton = BENCH_benchMode == PLACE_FIXED_CONTACT || IsKeyDown(KEY_ESCAPE);
    DrawRectangleRounded((Rectangle){
        5, 40, 30, 30
    }, 0.1f, 3, fillFixedContactButton ? BLACK : WHITE);

    DrawRectangleLinesEx((Rectangle){
        10, 45, 20, 20
    }, 2, fillFixedContactButton ? WHITE : BLACK);

    DrawCircle(20, 55, 5, fillFixedContactButton ? WHITE : BLACK);

    DrawRectangleRoundedLines((Rectangle){
        5, 40, 30, 30
    }, 0.1f, 3, 1, BLACK);
}