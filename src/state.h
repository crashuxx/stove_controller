#ifndef __state_h__
#define __state_h__

#include <stdio.h>

#define TEMPERATURE_SAMPLES 16

typedef struct temperatures_s {
    int8_t stove[TEMPERATURE_SAMPLES];
    int8_t heating[TEMPERATURE_SAMPLES];
    int8_t heating_return[TEMPERATURE_SAMPLES];
    int8_t water[TEMPERATURE_SAMPLES];
    int8_t feeder[TEMPERATURE_SAMPLES];
    int8_t smoke[TEMPERATURE_SAMPLES];

    uint8_t i;

    int8_t stove_avr;
    int8_t heating_avr;
    int8_t heating_return_avr;
    int8_t water_avr;
    int8_t feeder_avr;
    int8_t smoke_avr;
} temperatures_t;

typedef enum stove_state {
    OFF,
    SETTING_FIRE,
    BURNING,
    COOLING,
    PASSIVE,
    EXCTINCTION

}  stove_state_t;

stove_state_t update_state(stove_state_t state, temperatures_t temps);

#endif