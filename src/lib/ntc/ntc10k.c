#include <stdint.h>
#include <stdio.h>
#include "ntc.h"

#define RESOLUTION 34
#define R_B3900K 1
#define T 0
#define N 2

const int ntc10k_resistance_temperatue[] = {
    -40,	328996,
    -35,	237387,
    -30,	173185,
    -25,	127773,
    -20,	95327,
    -15,	71746,
    -10,	54564,
    -5, 	41813,
    0,	    32330,
    5,	    25194,
    10,	    19785,
    15,	    15651,
    20,	    12468,
    25,	    10000,
    30,	    8072,
    35,	    6556,
    40,	    5356,
    45,	    4401,
    50,	    3635,
    55,	    3019,
    60,	    2521,
    65,	    2115,
    70,	    1781,
    75,	    1509,
    80,	    1284,
    85,	    1097,
    90,	    941,
    95,	    810,
    100,	701,
    105,	608,
    110,	530,
    115,	463,
    120,	406,
    125,	358
};

int8_t ntc10k_b3900k_valid_resistance(unsigned int resistance) {
    if(resistance > ntc10k_resistance_temperatue[R_B3900K]) {
        return -1;
    } else if(resistance < ntc10k_resistance_temperatue[(RESOLUTION-1)*N + R_B3900K]) {
        return 1;
    }

    return 0;
}

int ntc10k_b3900k_temperature(unsigned int resistance) {
    if(resistance > ntc10k_resistance_temperatue[R_B3900K])
        return ntc10k_resistance_temperatue[T] - 1;

    for(int8_t i = N; i < (RESOLUTION*N); i+=N) {
        if(resistance >= ntc10k_resistance_temperatue[i+R_B3900K]) {
            float resistance_range =  ntc10k_resistance_temperatue[i-N+R_B3900K] - ntc10k_resistance_temperatue[i+R_B3900K];
            float temperature_range = ntc10k_resistance_temperatue[i+T] - ntc10k_resistance_temperatue[i-N+T];
            float dt = ((float)(resistance - ntc10k_resistance_temperatue[i+R_B3900K])) / resistance_range * temperature_range;
            return ntc10k_resistance_temperatue[i+T] - dt;
        }
    }

    return ntc10k_resistance_temperatue[RESOLUTION-N+T] + 1;
}
