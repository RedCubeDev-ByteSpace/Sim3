//
// Created by redcube on 27/05/2026.
//

#ifndef SIM3_CLAY_RENDERER_RAYLIB_H
#define SIM3_CLAY_RENDERER_RAYLIB_H

#include "raylib.h"

typedef enum
{
    CUSTOM_LAYOUT_ELEMENT_TYPE_3D_MODEL,
    CUSTOM_LAYOUT_ELEMENT_TOOL_ICON,
} CustomLayoutElementType;

typedef struct
{
    Model model;
    float scale;
    Vector3 position;
    Matrix rotation;
} CustomLayoutElement_3DModel;

typedef struct
{
    int iconType;
    bool active;
} CustomLayoutElement_ToolIcon;

typedef struct
{
    CustomLayoutElementType type;
    union {
        CustomLayoutElement_3DModel model;
        CustomLayoutElement_ToolIcon icon;
    } customData;
} CustomLayoutElement;

#endif //SIM3_CLAY_RENDERER_RAYLIB_H
