//
// Created by redcube on 21/05/2026.
//

#include "Menu.h"

#include <raylib.h>
#include <string.h>

#include "../Grid.h"
#include "../GUI.h"
#include "../../Sim/SaveAndLoad.h"
#include "../../Sim/Json/AppData.h"
#include "../Lib/clay.h"

void MENU_init() {
    GRID_zoom = 2;
}

void MENU_draw() {

    float frameTime = GetFrameTime();
    GRID_cameraPos = (Vector2){
        GRID_cameraPos.x + 20 * frameTime,
        GRID_cameraPos.y + 30 * frameTime
    };

    GRID_draw();
}

void MENU_handleNewButton(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    if (pointerInfo.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;

    SAVE_AND_LOAD_close();
    GUI_appState = APP_STATE_SIM;
}

void MENU_handleOpenButton(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    if (pointerInfo.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;

    SAVE_AND_LOAD_close();
    SAVE_AND_LOAD_load();

    if (SAVE_AND_LOAD_currentProject != NULL) {
        GUI_appState = APP_STATE_SIM;
    }
}

void MENU_handleRecentFile(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *idx) {
    if (pointerInfo.state != CLAY_POINTER_DATA_RELEASED_THIS_FRAME) return;

    int index = (int)idx;

    SAVE_AND_LOAD_close();
    SAVE_AND_LOAD_loadSimspace(APPDATA_lstRecentFiles->buffer[index].path);

    if (SAVE_AND_LOAD_currentProject != NULL) {
        GUI_appState = APP_STATE_SIM;
    }
}

void MENU_layout() {
    CLAY(CLAY_ID("Main"), {
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW()
            },
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER
            }
        },
        .backgroundColor = (Clay_Color){0, 0, 0, 0}
    }) {
        CLAY(CLAY_ID("WindowContainer"), {
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_FIT(600),
                    .height = CLAY_SIZING_FIXED(400)
                },
                .layoutDirection = CLAY_TOP_TO_BOTTOM
            },
            .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(RAYWHITE),
            .border = {
                .width = {
                    .top = 1, .right = 1, .bottom = 1, .left = 1
                },
                .color = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK),
            }
        }) {
            CLAY(CLAY_ID("Header"), {
                .layout = {
                    .sizing = {
                        .height = CLAY_SIZING_FIT(80),
                        .width = CLAY_SIZING_GROW(0),
                    }
                },
                .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE),
            }) {
                CLAY(CLAY_ID("Header_Sim"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIT(0),
                            .height = CLAY_SIZING_GROW(0),
                        },
                        .childAlignment = {
                            .y = CLAY_ALIGN_Y_BOTTOM,
                        },
                        .padding = {
                            .left = 15
                        }
                    }
                }) {
                    CLAY_TEXT(
                        CLAY_STRING("sim"),
                        CLAY_TEXT_CONFIG({
                            .fontId = COMPUTER_MODERN_55,
                            .fontSize = 55,
                            .textColor = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                        })
                    );
                }

                CLAY(CLAY_ID("Header_3"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIT(0),
                            .height = CLAY_SIZING_GROW(0),
                        },
                        .childAlignment = {
                            .y = CLAY_ALIGN_Y_CENTER,
                        },
                    }
                }) {
                    CLAY_TEXT(
                        CLAY_STRING("3"),
                        CLAY_TEXT_CONFIG({
                            .fontId = COMPUTER_MODERN_55,
                            .fontSize = 45,
                            .textColor = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                        })
                    );
                }

                CLAY(CLAY_ID("Header_Filler"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_GROW(0),
                            .height = CLAY_SIZING_GROW(0),
                        }
                    }
                }) {}

                CLAY(CLAY_ID("Header_Version"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIT(0),
                            .height = CLAY_SIZING_GROW(0),
                        },
                        .padding = {
                            .right = 15,
                            .bottom = 10
                        },
                        .childAlignment = {
                            .y = CLAY_ALIGN_Y_BOTTOM,
                        }
                    }
                }) {
                    CLAY_TEXT(
                        CLAY_STRING(VERSION),
                        CLAY_TEXT_CONFIG({
                            .fontId = COMPUTER_MODERN_55,
                            .fontSize = 25,
                            .textColor = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                        })
                    );
                }
            }

            CLAY(CLAY_ID("Main"), {
                .layout = {
                    .sizing = {
                        .height = CLAY_SIZING_GROW(0),
                        .width = CLAY_SIZING_GROW(0),
                    },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    .padding = {
                        .top = 10, .right = 10, .bottom = 10, .left = 10,
                    },
                    .childGap = 10
                },
                .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(RAYWHITE),
            }) {

                CLAY(CLAY_ID("Main_RecentsWindowContainer"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_GROW(0),
                            .height = CLAY_SIZING_GROW(0)
                        },
                        .padding = {
                            .top = 5, .right = 5, .bottom = 5, .left = 5,
                        },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM
                    },
                    .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE),
                }) {

                    CLAY(CLAY_ID("Main_Recent_Header"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_GROW(0),
                                .height = CLAY_SIZING_FIT(0)
                            },
                            .padding = {
                                .top = 4, .bottom = 4,
                                .left = 3
                            }
                        },
                        .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE)
                    }) {
                        CLAY_TEXT(
                            CLAY_STRING("Recents"),
                            CLAY_TEXT_CONFIG({
                                .fontId = COMPUTER_MODERN_20,
                                .fontSize = 20,
                                .textColor = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                            })
                        );
                    }

                    CLAY(CLAY_ID("Main_Recent_ListBox"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_GROW(0),
                                .height = CLAY_SIZING_GROW(0)
                            },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .childGap = 1
                        },
                        .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(RAYWHITE),
                        .clip = {
                            .vertical = true,
                            .childOffset = Clay_GetScrollOffset()
                        }
                    }) {
                        for (int i = 0; i < APPDATA_lstRecentFiles->length; ++i) {
                            appdata_recent_record_t *record = &APPDATA_lstRecentFiles->buffer[i];

                            CLAY(CLAY_IDI("Main_Recent_Entry", i), {
                                .layout = {
                                    .sizing = {
                                        .width = CLAY_SIZING_GROW(0),
                                        .height = CLAY_SIZING_FIT(0)
                                    },
                                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                    .padding = {
                                    5, 5, 5,5
                                    }
                                },
                                .backgroundColor = Clay_Hovered() ? RAYLIB_COLOR_TO_CLAY_COLOR(BLACK) : RAYLIB_COLOR_TO_CLAY_COLOR(RAYWHITE)
                            }) {
                                Clay_Color textColor = Clay_Hovered() ? RAYLIB_COLOR_TO_CLAY_COLOR(WHITE) : RAYLIB_COLOR_TO_CLAY_COLOR(BLACK);
                                Clay_OnHover(MENU_handleRecentFile, (void*)i);

                                CLAY(CLAY_ID("Main_Recent_Entry_Header"), {
                                    .layout = {
                                        .sizing = {
                                            .width = CLAY_SIZING_GROW(0),
                                            .height = CLAY_SIZING_FIT(10)
                                        }
                                    }
                                }) {
                                    CLAY(CLAY_ID("Main_Recent_Entry_Header_Name"), {
                                        .layout = {
                                            .sizing = {
                                                .width = CLAY_SIZING_FIT(0),
                                                .height = CLAY_SIZING_GROW(0)
                                            }
                                        }
                                    }) {

                                        CLAY_TEXT(
                                            ((Clay_String){
                                                .length = strlen(record->filename),
                                                .chars = record->filename
                                            }),
                                            CLAY_TEXT_CONFIG({
                                                .fontId = COMPUTER_MODERN_20,
                                                .fontSize = 20,
                                                .textColor = textColor
                                            })
                                        );
                                    }

                                    CLAY(CLAY_ID("Main_Recent_Entry_Header_Name"), {
                                        .layout = {
                                            .sizing = {
                                                .width = CLAY_SIZING_GROW(0),
                                            }
                                        }
                                    }) {}

                                    CLAY(CLAY_ID("Main_Recent_Entry_Header_Name"), {
                                        .layout = {
                                            .sizing = {
                                                .width = CLAY_SIZING_FIT(0),
                                                .height = CLAY_SIZING_GROW(0)
                                            }
                                        }
                                    }) {
                                        CLAY_TEXT(
                                            ((Clay_String){
                                                .length = strlen(record->accessTime),
                                                .chars = record->accessTime
                                            }),
                                            CLAY_TEXT_CONFIG({
                                                .fontId = COMPUTER_MODERN_20,
                                                .fontSize = 20,
                                                .textColor = textColor
                                            })
                                        );
                                    }
                                }

                                CLAY(CLAY_ID("Main_Recent_Entry_Path"), {
                                    .layout = {
                                        .sizing = {
                                            .width = CLAY_SIZING_GROW(0),
                                            .height = CLAY_SIZING_FIT(0)
                                        }
                                    }
                                }) {
                                    CLAY_TEXT(
                                        ((Clay_String){
                                            .length = strlen(record->path),
                                            .chars = record->path
                                        }),
                                        CLAY_TEXT_CONFIG({
                                            .fontId = COMPUTER_MODERN_16,
                                            .fontSize = 16,
                                            .textColor = textColor,
                                            .wrapMode = CLAY_TEXT_WRAP_WORDS
                                        })
                                    );
                                }
                            }
                        }
                    }

                }

                CLAY(CLAY_ID("Main_Buttons"), {
                    .layout = {
                        .sizing = {
                            .width = CLAY_SIZING_FIT(100),
                            .height = CLAY_SIZING_GROW(0)
                        },
                        .childAlignment = {
                            .x = CLAY_ALIGN_X_CENTER,
                            .y = CLAY_ALIGN_Y_TOP,
                        },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .childGap = 10
                    },
                }) {

                    CLAY(CLAY_ID("Main_Buttons_New"), {
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_GROW(0),
                                .height = CLAY_SIZING_FIT(0)
                            },
                            .childAlignment = {
                                .x = CLAY_ALIGN_X_CENTER,
                                .y = CLAY_ALIGN_Y_CENTER
                            },
                            .padding = {
                                .top = 7, .bottom = 7
                            }
                        },
                        .backgroundColor = Clay_Hovered() ? RAYLIB_COLOR_TO_CLAY_COLOR(BLACK) : RAYLIB_COLOR_TO_CLAY_COLOR(WHITE),
                    }) {
                        Clay_OnHover(MENU_handleNewButton, 0);

                        CLAY_TEXT(
                            CLAY_STRING("New"),
                            CLAY_TEXT_CONFIG({
                                .fontId = COMPUTER_MODERN_20,
                                .fontSize = 20,
                                .textColor = Clay_Hovered() ? RAYLIB_COLOR_TO_CLAY_COLOR(WHITE) : RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                            })
                        );
                    }

                    CLAY(CLAY_ID("Main_Buttons_Open"), {
                       .layout = {
                           .sizing = {
                               .width = CLAY_SIZING_GROW(0),
                               .height = CLAY_SIZING_FIT(0)
                           },
                           .childAlignment = {
                               .x = CLAY_ALIGN_X_CENTER,
                               .y = CLAY_ALIGN_Y_CENTER
                           },
                           .padding = {
                               .top = 7, .bottom = 7
                           }
                       },
                       .backgroundColor = Clay_Hovered() ? RAYLIB_COLOR_TO_CLAY_COLOR(BLACK) : RAYLIB_COLOR_TO_CLAY_COLOR(WHITE),
                   }) {
                        Clay_OnHover(MENU_handleOpenButton, 0);

                        CLAY_TEXT(
                            CLAY_STRING("Open"),
                            CLAY_TEXT_CONFIG({
                                .fontId = COMPUTER_MODERN_20,
                                .fontSize = 20,
                                .textColor = Clay_Hovered() ? RAYLIB_COLOR_TO_CLAY_COLOR(WHITE) : RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                            })
                        );
                    }
                }

            }

            CLAY(CLAY_ID("Footer"), {
                .layout = {
                    .sizing = {
                        .height = CLAY_SIZING_FIT(0),
                        .width = CLAY_SIZING_GROW(0),
                    },
                    .padding = {
                        .top = 2,
                        .bottom = 2,
                    },
                    .childAlignment = {
                        .x = CLAY_ALIGN_X_CENTER,
                        .y = CLAY_ALIGN_Y_BOTTOM
                    }
                },
                .backgroundColor = RAYLIB_COLOR_TO_CLAY_COLOR(WHITE),
            }) {
                CLAY_TEXT(
                    CLAY_STRING("made with love, caffeine, clay, and raylib - by redcube"),
                    CLAY_TEXT_CONFIG({
                        .fontId = COMPUTER_MODERN_16,
                        .fontSize = 16,
                        .textColor = RAYLIB_COLOR_TO_CLAY_COLOR(BLACK)
                    })
                );
            }
        }
    }
}