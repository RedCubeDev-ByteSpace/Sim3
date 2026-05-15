//
// Created by redcube on 13/05/2026.
//
#include <json-c/json.h>
#include "SimulationResources.h"

#include <stdio.h>
#include <string.h>

sim_chip_specification_t SIMRES_chipSpecifications[MAX_CHIP_SPECS];
int SIMRES_numChipSpecs;

void SIMRES_init() {

    json_object *listOfChips = json_object_from_file("../Resources/chips.json");
    if (listOfChips == NULL) {
        printf("Unable to open chip catalogue!");
        return;
    }

    // get the number of items
    SIMRES_numChipSpecs = json_object_array_length(listOfChips);

    // iterate through each one
    for (int i = 0; i < SIMRES_numChipSpecs; ++i) {
        sim_chip_specification_t *me = &SIMRES_chipSpecifications[i];
        json_object *myEntry = json_object_array_get_idx(listOfChips, i);

        // -------------------------------------------------------------------------------------------------------------
        // copy over the name and function string
        const char *name = json_object_get_string(json_object_object_get(myEntry, "Name"));
        const char *function = json_object_get_string(json_object_object_get(myEntry, "Function"));

        me->name = malloc(strlen(name) + 1);
        me->function = malloc(strlen(function) + 1);
        strcpy(me->name, name);
        strcpy(me->function, function);

        // -------------------------------------------------------------------------------------------------------------
        // configure the pins
        me->numPins = json_object_get_int(json_object_object_get(myEntry, "Pins"));
        me->pinSpecs = malloc(me->numPins * sizeof(sim_pin_specification_t));

        int num;
        json_object *pinArray;

        pinArray = json_object_object_get(myEntry, "Inputs");
        num = json_object_array_length(pinArray);
        for (int ii = 0; ii < num; ++ii) {
            int index = json_object_get_int(json_object_array_get_idx(pinArray, ii));
            me->pinSpecs[index - 1] = PIN_INPUT;
        }

        pinArray = json_object_object_get(myEntry, "Outputs");
        num = json_object_array_length(pinArray);
        for (int ii = 0; ii < num; ++ii) {
            int index = json_object_get_int(json_object_array_get_idx(pinArray, ii));
            me->pinSpecs[index - 1] = PIN_OUTPUT;
        }

        pinArray = json_object_object_get(myEntry, "Power");
        num = json_object_array_length(pinArray);
        for (int ii = 0; ii < num; ++ii) {
            int index = json_object_get_int(json_object_array_get_idx(pinArray, ii));
            me->pinSpecs[index - 1] = PIN_POWER;
        }

        // -------------------------------------------------------------------------------------------------------------
        // configure the lua
        json_object *lua = json_object_object_get(myEntry, "Lua");

        // assemble the scripts path
        char *pathTemplate = "../Resources/Scripts/%s";
        char *scriptName = json_object_get_string(json_object_object_get(lua, "Script"));
        int bufferSize = snprintf(NULL, 0, pathTemplate, scriptName) + 1;
        me->script = malloc(bufferSize);
        snprintf(me->script, bufferSize, pathTemplate, scriptName);

        // remember if this chip is stateless or stateful
        // -> this determins on if its called via a step() or stepRising() and stepFalling() call
        me->isStateful = json_object_get_boolean(json_object_object_get(lua, "IsStateful"));
    }

    for (int i = 0; i < SIMRES_numChipSpecs; ++i) {
        printf("Name: %s\n", SIMRES_chipSpecifications[i].name);
        printf("Function: %s\n", SIMRES_chipSpecifications[i].function);
        printf("Script: %s\n", SIMRES_chipSpecifications[i].script);
        printf("Stateful: %d\n", SIMRES_chipSpecifications[i].isStateful);
        printf("Pins:\n");
        for (int ii = 0; ii < SIMRES_chipSpecifications[i].numPins / 2; ++ii) {
            sim_pin_specification_t pin = SIMRES_chipSpecifications[i].pinSpecs[SIMRES_chipSpecifications[i].numPins - ii - 1];
            switch (pin) {
                case PIN_INPUT: printf("I "); break;
                case PIN_OUTPUT: printf("O "); break;
                case PIN_POWER: printf("P "); break;
            }
        }
        printf("\n");
        for (int ii = 0; ii < SIMRES_chipSpecifications[i].numPins / 2; ++ii) {
            sim_pin_specification_t pin = SIMRES_chipSpecifications[i].pinSpecs[ii];
            switch (pin) {
                case PIN_INPUT: printf("I "); break;
                case PIN_OUTPUT: printf("O "); break;
                case PIN_POWER: printf("P "); break;
            }
        }
        printf("\n\n");
    }

    json_object_put(listOfChips);
}