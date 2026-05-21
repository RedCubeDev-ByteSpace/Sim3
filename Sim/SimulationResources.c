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
        // copy over the id, name and function string
        const char *id = json_object_get_string(json_object_object_get(myEntry, "ID"));
        const char *name = json_object_get_string(json_object_object_get(myEntry, "Name"));
        const char *function = json_object_get_string(json_object_object_get(myEntry, "Function"));

        // -------------------------------------------------------------------------------------------------------------
        // make sure this chip is legal
        me->numPins = json_object_get_int(json_object_object_get(myEntry, "Pins"));
        if (me->numPins > MAX_CHIP_PINS) {
            printf("Number of pins on chip '%s' exceeds MAX_CHIP_PINS!\n", name);
        }

        // allocate and copy the data
        me->pinSpecs = malloc(me->numPins * sizeof(sim_pin_specification_t));
        me->id = malloc(strlen(id) + 1);
        me->name = malloc(strlen(name) + 1);
        me->function = malloc(strlen(function) + 1);

        strcpy(me->id, id);
        strcpy(me->name, name);
        strcpy(me->function, function);

        // -------------------------------------------------------------------------------------------------------------
        // load all pin labels

        // clear out all pin labels
        me->pinLabels = malloc(me->numPins * sizeof(sim_chip_pin_label_t));

        // load the pin labels
        json_object *pinLabels = json_object_object_get(myEntry, "PinLabels");
        int len = json_object_array_length(pinLabels);
        for (int ii = 0; ii < len; ++ii) {
            json_object *pinLabel = json_object_array_get_idx(pinLabels, ii);

            // copy over the label, max 4 chars per label
            snprintf(me->pinLabels[ii].label, 5, "%s", json_object_get_string(json_object_object_get(pinLabel, "Text")));

            // remember is this is active high or low
            json_object *activeLow = json_object_object_get(pinLabel, "ActiveLow");
            if (activeLow == NULL) {
                me->pinLabels[ii].activeLow = false;
            } else {
                me->pinLabels[ii].activeLow = json_object_get_boolean(activeLow);
            }
        }

        // -------------------------------------------------------------------------------------------------------------
        // configure the pins

        int num;
        json_object *pinArray;

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

        // if it is stateful -> which pin is the clock?
        if (me->isStateful) {
            me->clockPin = json_object_get_int(json_object_object_get(lua, "ClockPin")) - 1;
            me->pinSpecs[me->clockPin] = PIN_CLOCK;
        }
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