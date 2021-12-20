#ifndef __common_h__
#define __common_h__

#include <stdlib.h>

#define time_ms() (time_us_64()/1000) 

typedef struct state_s {
    bool active;
    uint32_t last_change_time;
} state_s;

typedef struct relay_state_s {
    state_s fuel_feeder;
    state_s fireplace_fan;
    state_s heating_pump;
    state_s hot_water_pump;
} relay_state_s;

#endif