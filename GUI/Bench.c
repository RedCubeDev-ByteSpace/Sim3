//
// Created by redcube on 10/05/2026.
//
#include "Bench.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Grid.h"
#include "GUI.h"
#include "../Sim/SimulationResources.h"
#include "../Sim/SimulationSpace.h"
#include "Drawables/Wire.h"
#include "../Sim/Components/Connection.h"
#include "../Sim/SaveAndLoad.h"
#include "Lib/clay.h"
#include "Lib/clay_renderer_raylib.h"

// ---------------------------------------------------------------------------------------------------------------------
bench_working_mode_t BENCH_benchMode;
CustomLayoutElement toolIcons[NUM_MODES];
bool isCursorOnOverlay;
bool isCursorOnOverlayThisFrame;
float cursorRotation;
int pieceRotation;
int activeColor;
// ---------------------------------------------------------------------------------------------------------------------
// Data for wire drawing mode
Vector2 attachmentVertex;
bool hasAttachmentVertex;
sim_connection_t *thisConnection;
// ---------------------------------------------------------------------------------------------------------------------
// Data for contact placement mode
drw_fixed_contact_t *newContact;
// ---------------------------------------------------------------------------------------------------------------------
// Data for led placement mode
drw_led_t *newLED;
// ---------------------------------------------------------------------------------------------------------------------
// Data for chip placement
int selectedChip = 0;
drw_chip_t *newChip;
// ---------------------------------------------------------------------------------------------------------------------
bench_tab_mode_t currentTab;

void BENCH_init() {
    BENCH_benchMode = IDLE;
    currentTab = TAB_CLOSED;
    cursorRotation = 0;
    pieceRotation = 0;
    activeColor = 0;

    for (int i = 0; i < NUM_MODES; ++i) {
        toolIcons[i].type = CUSTOM_LAYOUT_ELEMENT_TOOL_ICON;
        toolIcons[i].customData.icon.iconType = i;
    }

    thisConnection = NULL;
    hasAttachmentVertex = false;

    newContact = malloc(sizeof(drw_fixed_contact_t));
    DRAWABLES_FIXED_CONTACT_init(newContact, (Vector2){0,0}, false, 0);

    newLED = malloc(sizeof(drw_led_t));
    DRAWABLES_LED_init(newLED, (Vector2){0,0}, 0);

    newChip = malloc(sizeof(drw_chip_t));
    DRAWABLES_CHIP_init(newChip, (Vector2){0,0}, &SIMRES_chipSpecifications[0], SIMRES_chipSpecifications[0].numPins / 2);
}

void BENCH_process() {
    isCursorOnOverlayThisFrame = isCursorOnOverlay;
    isCursorOnOverlay = false;

    // -----------------------------------------------------------------------------------------------------------------
    // ESC shortcut to exit out of all modes
    if (IsKeyReleased(KEY_ESCAPE)) {
        BENCH_benchMode = IDLE;
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Q rotatificates rotatable objects
    if (IsKeyReleased(KEY_Q)) {
        pieceRotation++;
        pieceRotation = pieceRotation % 4;
    }

    // -----------------------------------------------------------------------------------------------------------------
    // CTRL + S saves this file
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyReleased(KEY_S)) {

        if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) {
            SAVE_AND_LOAD_saveAs();
        } else {
            SAVE_AND_LOAD_save();
        }
    }

    // only process mouse input when the cursor is not inside a clay panel
    if (isCursorOnOverlayThisFrame) return;

    Vector2 mousePos = GetMousePosition();
    Vector2 pos = LIB_screenSpaceToWorldSpace(mousePos);
    Vector2 wpos, hpos;
    wpos = LIB_roundificateToWholePoint(pos);
    hpos = LIB_roundificateToHalfPoint(pos);

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

                // no connection yet -> create a new one
                thisConnection = malloc(sizeof(sim_connection_t));
                SIM_CONNECTION_init(thisConnection);

                // use it as attachment
                attachmentVertex = hpos;
                hasAttachmentVertex = true;

                // is it attached to any connection points??
                for (int i = 0; i < SIMSPACE_lstConnectionPoints->length; ++i) {
                    sim_connection_point_t *conPoint = SIMSPACE_lstConnectionPoints->buffer[i];
                    if (conPoint->position.x == hpos.x && conPoint->position.y == hpos.y) {

                        SIM_COMP_LIST_appendConnectionPoint(&thisConnection->lstConnectedPoints, conPoint);
                        break;
                    }
                }

                return;
            }
        }

        // if this is already part of a connection -> expand it
        if ((IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) && thisConnection != NULL) {

            if (attachmentVertex.x == hpos.x && attachmentVertex.y == hpos.y) return; // bro this doesnt count

            // have we hit any connection points?
            sim_connection_point_t *conPoint = NULL;
            for (int i = 0; i < SIMSPACE_lstConnectionPoints->length; ++i) {
                sim_connection_point_t *_conPoint = SIMSPACE_lstConnectionPoints->buffer[i];
                if (_conPoint->position.x == hpos.x && _conPoint->position.y == hpos.y) {
                    conPoint = _conPoint;
                    break;
                }
            }

            // if this is a brand new connection -> add it to the simulation space!
            if (thisConnection->lstVectorPairs.length == 0) {
                SIM_COMP_LIST_appendConnection(SIMSPACE_lstConnections, thisConnection);
            }

            // append a new vector pair!
            SIM_CONNECTION_VECTOR_PAIR_LIST_append(&thisConnection->lstVectorPairs, (sim_vector_pair_t){attachmentVertex, hpos, wireColors[activeColor]});
            if (conPoint != NULL) {
                SIM_COMP_LIST_appendConnectionPoint(&thisConnection->lstConnectedPoints, conPoint);
            }

            // rebuild and redraw
            SIM_CONNECTION_refreshDrawablesStructure(thisConnection);

            // if this was a right click -> directly add this vertex as the new attachment point
            if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
                attachmentVertex = hpos;
            }
            // otherwise: just finish this wire
            else {
                hasAttachmentVertex = false;
                thisConnection = NULL;
                SIMSPACE_mergeOverlappingConnections();
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
            SIM_FIXED_CONTACT_init(contact, wpos, false, pieceRotation);
            SIM_COMP_LIST_appendFixedContact(SIMSPACE_lstFixedContacts, contact);
        }
    }

    // -------------------------------------------------------------------------------------------
    // LED placement mode
    if (BENCH_benchMode == PLACE_LED) {
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
            sim_led_t *led = malloc(sizeof(sim_led_t));
            SIM_LED_init(led, wpos, pieceRotation);
            SIM_COMP_LIST_appendLED(SIMSPACE_lstLEDs, led);
        }
    }

    // -------------------------------------------------------------------------------------------
    // Chip placement mode
    if (BENCH_benchMode == PLACE_CHIP) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            // make sure there isnt already a contact at this position
            // Todo: expand this to all pins on the chip
            for (int i = 0; i < SIMSPACE_lstConnectionPoints->length; ++i) {
                sim_connection_point_t *conPoint = SIMSPACE_lstConnectionPoints->buffer[i];
                if (conPoint->position.x == wpos.x && conPoint->position.y == wpos.y) {
                    // this space is already occupied!!
                    return;
                }
            }

            // create a new chip at this position
            sim_chip_t *chip = malloc(sizeof(sim_chip_t));
            SIM_CHIP_init(chip, wpos, &SIMRES_chipSpecifications[selectedChip]);
            SIM_COMP_LIST_appendChip(SIMSPACE_lstChips, chip);
        }
    }

    // -------------------------------------------------------------------------------------------
    // Deletion mode
    if (BENCH_benchMode == DELETION) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            sim_comp_list_t lst;
            SIM_COMP_LIST_init(&lst);

            // ---------------------------------------------------------------------------------------------------------
            // have we hit a wire?
            // kill it!
            for (int i = 0; i < SIMSPACE_lstConnections->length; ++i) {
                sim_connection_t *con = SIMSPACE_lstConnections->buffer[i];

                sim_connection_vector_pair_list_t lstPairsToDelete;;
                SIM_CONNECTION_VECTOR_PAIR_LIST_init(&lstPairsToDelete);

                for (int ii = 0; ii < con->lstVectorPairs.length; ++ii) {
                    sim_vector_pair_t pair = con->lstVectorPairs.buffer[ii];

                    // have we hit a vector anchor point?
                    if (pair.from.x == hpos.x && pair.from.y == hpos.y
                        ||
                        pair.to.x == hpos.x && pair.to.y == hpos.y) {

                        // append this vector pair to be deleted
                        SIM_CONNECTION_VECTOR_PAIR_LIST_append(&lstPairsToDelete, pair);
                    }
                }

                // go through all vectors that another connection can connect to
                for (int ii = 0; ii < con->lstConnectableVectors.length; ++ii) {
                    sim_connectable_vector_t vec = con->lstConnectableVectors.buffer[ii];

                    if (vec.position.x == hpos.x && vec.position.y == hpos.y) {

                        // append this vector pair to be deleted
                        SIM_CONNECTION_VECTOR_PAIR_LIST_append(&lstPairsToDelete, con->lstVectorPairs.buffer[vec.vectorPairIndex]);
                    }
                }

                // remove all pairs we hit
                for (int ii = 0; ii < lstPairsToDelete.length; ++ii) {
                    sim_vector_pair_t pair = lstPairsToDelete.buffer[ii];
                    SIM_CONNECTION_VECTOR_PAIR_LIST_remove(&con->lstVectorPairs, pair);
                }

                if (lstPairsToDelete.length > 0) {

                    // append the connection to recombobulate
                    SIM_COMP_LIST_appendConnection(&lst, con);
                }

                SIM_CONNECTION_VECTOR_PAIR_LIST_clear(&lstPairsToDelete);
            }

            // recombobulation
            for (int i = 0; i < lst.length; ++i) {
                sim_connection_t *con = lst.buffer[i];

                if (con->lstVectorPairs.length == 0) {
                    SIM_CONNECTION_unload(con);
                    SIM_COMP_LIST_removeConnectionRef(SIMSPACE_lstConnections, con);
                    free(con);
                }
                else {
                    SIM_CONNECTION_rebuildAndSplitConnection(con);
                }
            }

            SIM_COMP_LIST_clear(&lst);

            // ---------------------------------------------------------------------------------------------------------
            // have we hit a fixed contact?
            for (int i = 0; i < SIMSPACE_lstFixedContacts->length; ++i) {
                sim_fixed_contact_t *contact = SIMSPACE_lstFixedContacts->buffer[i];

                if (contact->point.position.x == hpos.x && contact->point.position.y == hpos.y) {
                    SIM_COMP_LIST_appendFixedContact(&lst, contact);
                }
            }

            // death
            for (int i = 0; i < lst.length; ++i) {
                sim_fixed_contact_t *contact = lst.buffer[i];

                SIM_FIXED_CONTACT_unload(contact);
                SIM_COMP_LIST_removeFixedContactRef(SIMSPACE_lstFixedContacts, contact);
                free(contact);
            }

            SIM_COMP_LIST_clear(&lst);

            // ---------------------------------------------------------------------------------------------------------
            // have we hit an led?
            for (int i = 0; i < SIMSPACE_lstLEDs->length; ++i) {
                sim_led_t *led = SIMSPACE_lstLEDs->buffer[i];

                if (led->point.position.x == hpos.x && led->point.position.y == hpos.y) {
                    SIM_COMP_LIST_appendLED(&lst, led);
                }
            }

            // death
            for (int i = 0; i < lst.length; ++i) {
                sim_led_t *led = lst.buffer[i];

                SIM_LED_unload(led);
                SIM_COMP_LIST_removeLEDRef(SIMSPACE_lstLEDs, led);
                free(led);
            }

            SIM_COMP_LIST_clear(&lst);

            // ---------------------------------------------------------------------------------------------------------
            // have we hit a chip?
            for (int i = 0; i < SIMSPACE_lstChips->length; ++i) {
                sim_chip_t *chip = SIMSPACE_lstChips->buffer[i];

                for (int ii = 0; ii < chip->connectionPoints.length; ++ii) {
                    sim_connection_point_t *conPoint = chip->connectionPoints.buffer[ii];

                    if (conPoint->position.x == hpos.x && conPoint->position.y == hpos.y) {
                        SIM_COMP_LIST_appendChip(&lst, chip);
                        goto next_chip;
                    }
                }

                next_chip:
            }

            // death
            for (int i = 0; i < lst.length; ++i) {
                sim_chip_t *chip = lst.buffer[i];

                SIM_CHIP_unload(chip);
                SIM_COMP_LIST_removeChipRef(SIMSPACE_lstChips, chip);
                free(chip);
            }

            SIM_COMP_LIST_clear(&lst);
        }
    }
}

void BENCH_draw() {

    // is the cursor currently inside a Clay panel
    // -> not our problem!
    if (isCursorOnOverlayThisFrame) return;

    // otherwise: draw a nice cursor :)

    cursorRotation += GetFrameTime() * 2;
    cursorRotation = cursorRotation > 360 ? 0 : cursorRotation;

    Vector2 mousePos = GetMousePosition();
    Vector2 wpos = LIB_roundificateToWholePoint(LIB_screenSpaceToWorldSpace(mousePos));
    Vector2 whpos = LIB_roundificateToHalfPoint(LIB_screenSpaceToWorldSpace(mousePos));

    // -----------------------------------------------------------------------------------------------------------------
    // Draw Cursors

    Vector2 pos = LIB_worldSpaceToScreenSpace(wpos);
    Vector2 hpos = LIB_worldSpaceToScreenSpace(whpos);
    Color workingColor = wireColors[activeColor];
    switch (BENCH_benchMode) {
        case DRAWING_WIRE:
            DrawLineEx((Vector2){hpos.x + cos(cursorRotation) * CURSOR_WIDTH, hpos.y + sin(cursorRotation) * CURSOR_WIDTH}, (Vector2){hpos.x - cos(cursorRotation) * CURSOR_WIDTH, hpos.y - sin(cursorRotation) * CURSOR_WIDTH}, CURSOR_THICKNESS, workingColor);
            DrawLineEx((Vector2){hpos.x + cos(cursorRotation + PI/2) * CURSOR_WIDTH, hpos.y + sin(cursorRotation + PI/2) * CURSOR_WIDTH}, (Vector2){hpos.x - cos(cursorRotation + PI/2) * CURSOR_WIDTH, hpos.y - sin(cursorRotation + PI/2) * CURSOR_WIDTH}, CURSOR_THICKNESS, workingColor);
            break;

        case PLACE_FIXED_CONTACT:
            newContact->position = wpos;
            newContact->rotation = pieceRotation;
            DRAWABLES_FIXED_CONTACT_draw(newContact);
            break;

        case PLACE_LED:
            newLED->position = wpos;
            newLED->rotation = pieceRotation;
            DRAWABLES_LED_draw(newLED);
            break;

        case PLACE_CHIP:
            newChip->position = wpos;
            DRAWABLES_CHIP_draw(newChip);
            break;

        case DELETION:
            DrawLineEx((Vector2){hpos.x + cos(cursorRotation) * CURSOR_WIDTH, hpos.y + sin(cursorRotation) * CURSOR_WIDTH}, (Vector2){hpos.x - cos(cursorRotation) * CURSOR_WIDTH, hpos.y - sin(cursorRotation) * CURSOR_WIDTH}, CURSOR_THICKNESS, workingColor);
            DrawLineEx((Vector2){hpos.x + cos(-cursorRotation - PI/2) * CURSOR_WIDTH, hpos.y + sin(-cursorRotation - PI/2) * CURSOR_WIDTH}, (Vector2){hpos.x - cos(-cursorRotation - PI/2) * CURSOR_WIDTH, hpos.y - sin(-cursorRotation - PI/2) * CURSOR_WIDTH}, CURSOR_THICKNESS, workingColor);
            break;

        default:
            DrawLineEx((Vector2){mousePos.x, mousePos.y - CURSOR_WIDTH}, (Vector2){mousePos.x, mousePos.y + CURSOR_WIDTH}, CURSOR_THICKNESS, workingColor);
            DrawLineEx((Vector2){mousePos.x - CURSOR_WIDTH, mousePos.y}, (Vector2){mousePos.x + CURSOR_WIDTH, mousePos.y}, CURSOR_THICKNESS, workingColor);
            break;
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Mode specific UI
    if (BENCH_benchMode == DRAWING_WIRE) {
        if (hasAttachmentVertex)
            DrawLineEx(LIB_worldSpaceToScreenSpace(attachmentVertex), hpos, 0.5f, wireColors[0]);
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// Callbacks
void BENCH_handleLuaTab(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    if (pointerInfo.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;

    if (currentTab == TAB_LUA) {
        currentTab = TAB_CLOSED;
    } else {
        currentTab = TAB_LUA;
    }
}
void BENCH_handleSerialTab(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    if (pointerInfo.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;

    if (currentTab == TAB_SERIAL) {
        currentTab = TAB_CLOSED;
    } else {
        currentTab = TAB_SERIAL;
    }
}
void BENCH_handleOscilloscopeTab(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    if (pointerInfo.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;

    if (currentTab == TAB_OSCILLOSCOPE) {
        currentTab = TAB_CLOSED;
    } else {
        currentTab = TAB_OSCILLOSCOPE;
    }
}
void BENCH_handleColorSelection(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    if (pointerInfo.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;
    activeColor = (int)userData;
}
void BENCH_handleModeSelection(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    if (pointerInfo.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;

    bench_working_mode_t mode = (bench_working_mode_t)(int)userData;

    if (BENCH_benchMode == mode) {
        BENCH_benchMode = IDLE;
    } else {

        // set up wire drawing when the mode is selected
        if (mode == DRAWING_WIRE && BENCH_benchMode != DRAWING_WIRE) {
            hasAttachmentVertex = false;

            if (thisConnection != NULL && thisConnection->lstVectorPairs.length == 0) {
                SIM_CONNECTION_unload(thisConnection);
                free(thisConnection);
            }
            thisConnection = NULL;
        }

        BENCH_benchMode = mode;
    }
}
void BENCH_handleCursorOnOverlayPanel(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    isCursorOnOverlay = true;
}
// ---------------------------------------------------------------------------------------------------------------------

void BENCH_layout() {

    CLAY(CLAY_ID("Main"), {
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0)
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
    }) {
        // CLAY(CLAY_ID("Menubar"), {
        //     .layout = {
        //         .sizing = {
        //             .width = CLAY_SIZING_GROW(0),
        //             .height = CLAY_SIZING_FIT(30)
        //         }
        //     },
        //     .border = {
        //         .width = {
        //             .bottom = 1
        //         },
        //         .color = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
        //     },
        //     .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE)
        // }) {
        //
        // }

        CLAY(CLAY_ID("Workspace"), {
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_GROW(0)
                },
                .layoutDirection = CLAY_LEFT_TO_RIGHT
            }
        }) {

            CLAY(CLAY_ID("Workspace_Tools"), {
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(0),
                        .height = CLAY_SIZING_GROW(0)
                    },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = {
                        10, 10, 10, 10
                    }
                }
            }) {

                CLAY(CLAY_ID("Workspace_Tools_Top"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_GROW(0),
                            .height = CLAY_SIZING_FIT(0)
                        }
                    }
                }) {

                    CLAY(CLAY_ID("Workspace_Tools_Colors"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_FIT(0),
                                .height = CLAY_SIZING_FIT(0)
                            },
                            .padding = {
                                10, 10, 10, 10
                            },
                            .childAlignment = {
                                .y = CLAY_ALIGN_Y_CENTER
                            },
                            .childGap = 6,
                            .layoutDirection = CLAY_LEFT_TO_RIGHT
                        },
                        .border = {
                            .width = {
                               1, 1, 1, 1
                            },
                            .color = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                        },
                        .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE)
                    }) {

                        Clay_OnHover(BENCH_handleCursorOnOverlayPanel, 0);

                        for (int i = 0; i < NUM_COLORS; ++i) {

                            CLAY(CLAY_IDI("Workspace_Tools_Color", i), {
                                .layout = {
                                    .sizing = {
                                        .width = CLAY_SIZING_FIXED(26),
                                        .height = CLAY_SIZING_FIXED(26)
                                    },
                                    .childAlignment = {
                                        .x = CLAY_ALIGN_X_CENTER,
                                        .y = CLAY_ALIGN_Y_CENTER
                                    }
                                },
                            }) {
                                Clay_OnHover(BENCH_handleColorSelection, (void*)i);

                                float radius = (activeColor == i || Clay_Hovered()) ? 26 : 22;
                                float hradius = radius / 2;

                                CLAY(CLAY_IDI("Workspace_Tools_Color", i), {
                                    .layout = {
                                        .sizing = {
                                            .width = CLAY_SIZING_FIXED(radius),
                                            .height = CLAY_SIZING_FIXED(radius)
                                        }
                                    },
                                    .cornerRadius = {
                                        hradius, hradius, hradius, hradius
                                    },
                                    .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(wireColors[i])
                                }) {}
                            }
                        }

                    }

                }

                CLAY(CLAY_ID("Workspace_Tools_Main"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_GROW(0),
                            .height = CLAY_SIZING_GROW(0)
                        },
                        .padding = {
                            .top = 5
                        }
                    }
                }) {

                    CLAY(CLAY_ID("Workspace_Tools_Toolbar"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_FIT(0),
                                .height = CLAY_SIZING_FIT(0)
                            },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .padding = {
                                10, 10, 10, 10
                            },
                            .childGap = 10
                        },
                        .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE),
                        .border = {
                            .width = {
                                1, 1, 1, 1
                            },
                            .color = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                        }
                    }) {

                        Clay_OnHover(BENCH_handleCursorOnOverlayPanel, 0);

                        for (int i = DRAWING_WIRE; i < NUM_MODES; ++i) {

                            CLAY(CLAY_IDI("Workspace_ToolButton", i), {
                                .layout = {
                                    .sizing = {
                                        .width = CLAY_SIZING_FIXED(30),
                                        .height = CLAY_SIZING_FIXED(30)
                                    },
                                },
                                .cornerRadius = {
                                    3, 3, 3, 3
                                },
                                .backgroundColor = (BENCH_benchMode == i || Clay_Hovered()) ? RAYLIB_COLOR_TO_CLAY_COLOR(BLACK) : RAYLIB_COLOR_TO_CLAY_COLOR(WHITE),
                                .border = {
                                    .width = {
                                        (BENCH_benchMode == i || Clay_Hovered()) ? 0 : 1,
                                        (BENCH_benchMode == i || Clay_Hovered()) ? 0 : 1,
                                        (BENCH_benchMode == i || Clay_Hovered()) ? 0 : 1,
                                        (BENCH_benchMode == i || Clay_Hovered()) ? 0 : 1,
                                    },
                                    .color = RAYLIB_COLOR_TO_CLAY_COLOR(GRAY)
                                }
                            }) {

                                Clay_OnHover(BENCH_handleModeSelection, (void*)i);
                                toolIcons[i].customData.icon.active = i == BENCH_benchMode || Clay_Hovered();

                                CLAY(CLAY_IDI("Workspace_Tool_Icon", i), {
                                    .layout = { .sizing = { CLAY_SIZING_FIXED(30), CLAY_SIZING_FIXED(30) } },
                                    .custom = {
                                        .customData = &toolIcons[i]
                                    }
                                }) {}
                            }
                        }

                    }

                }

            }

            CLAY(CLAY_ID("Workspace_Chips"), {
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_FIXED(300),
                        .height = CLAY_SIZING_GROW(0)
                    }
                }
            }) {



            }

        }

        CLAY(CLAY_ID("Expandables"), {
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_FIT(0)
                },
                .layoutDirection = CLAY_TOP_TO_BOTTOM
            }
        }) {

            Clay_OnHover(BENCH_handleCursorOnOverlayPanel, 0);

            CLAY(CLAY_ID("ExpandableTabs"), {
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(0),
                        .height = CLAY_SIZING_FIT(0)
                    },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT
                }
            }) {

                Clay_LayoutConfig tabLayout = {
                    .sizing = {
                        .width = CLAY_SIZING_FIT(0),
                        .height = CLAY_SIZING_FIT(0)
                    },
                    .padding = {
                        10, 10, 5, 3
                    }
                };

                Clay_BorderElementConfig tabBorder = {
                    .width = {
                        .top = 1, .right = 1
                    },
                    .color = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                };

                CLAY(CLAY_ID("ExpandableTabs_Lua"), {
                    .layout = tabLayout,
                    .border = tabBorder,
                    .backgroundColor = (currentTab == TAB_LUA || Clay_Hovered()) ? RAYLIB_COLOR_TO_CLAY_COLOR(BLACK) : RAYLIB_COLOR_TO_CLAY_COLOR(WHITE)
                }) {
                    Clay_OnHover(BENCH_handleLuaTab, NULL);

                    CLAY_TEXT(
                        CLAY_STRING("Lua Console"),
                        CLAY_TEXT_CONFIG({
                            .fontId = COMPUTER_MODERN_16,
                            .fontSize = 16,
                            .textColor = (currentTab == TAB_LUA || Clay_Hovered()) ? RAYLIB_COLOR_TO_CLAY_COLOR(WHITE) : RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                        })
                    );
                }

                CLAY(CLAY_ID("ExpandableTabs_Serial"), {
                    .layout = tabLayout,
                    .border = tabBorder,
                    .backgroundColor = (currentTab == TAB_SERIAL || Clay_Hovered()) ? RAYLIB_COLOR_TO_CLAY_COLOR(BLACK) : RAYLIB_COLOR_TO_CLAY_COLOR(WHITE)
                }) {
                    Clay_OnHover(BENCH_handleSerialTab, NULL);

                    CLAY_TEXT(
                        CLAY_STRING("Serial Console"),
                        CLAY_TEXT_CONFIG({
                            .fontId = COMPUTER_MODERN_16,
                            .fontSize = 16,
                            .textColor = (currentTab == TAB_SERIAL || Clay_Hovered()) ? RAYLIB_COLOR_TO_CLAY_COLOR(WHITE) : RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                        })
                    );
                }

                CLAY(CLAY_ID("ExpandableTabs_Oscilloscope"), {
                    .layout = tabLayout,
                    .border = tabBorder,
                    .backgroundColor = (currentTab == TAB_OSCILLOSCOPE || Clay_Hovered()) ? RAYLIB_COLOR_TO_CLAY_COLOR(BLACK) : RAYLIB_COLOR_TO_CLAY_COLOR(WHITE)
                }) {
                    Clay_OnHover(BENCH_handleOscilloscopeTab, NULL);

                    CLAY_TEXT(
                        CLAY_STRING("Oscilloscope"),
                        CLAY_TEXT_CONFIG({
                            .fontId = COMPUTER_MODERN_16,
                            .fontSize = 16,
                            .textColor = (currentTab == TAB_OSCILLOSCOPE || Clay_Hovered()) ? RAYLIB_COLOR_TO_CLAY_COLOR(WHITE) : RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                        })
                    );
                }

            }

            CLAY(CLAY_ID("ExpandableContainer"), {
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(0),
                        .height = CLAY_SIZING_FIXED(currentTab == TAB_CLOSED ? 0 : 300)
                    },
                },
                .border = {
                    .width = {
                        .top = 1
                    },
                    .color = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                },
                .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE),
                .transition = {
                    .handler = Clay_EaseOut,
                    .duration = 0.3f,
                    .properties = CLAY_TRANSITION_PROPERTY_HEIGHT
                }
            }) {

            }
        }

        CLAY(CLAY_ID("Footer"), {
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_FIT(30)
                }
            },
            .border = {
                .width = {
                    .top = 1
                },
                .color = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
            },
            .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE)
        }) {
            Clay_OnHover(BENCH_handleCursorOnOverlayPanel, 0);
        }

    }
}