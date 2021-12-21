#include <stdint.h>
#include "estyma.h"

#define RESOLUTION 20
#define R_H 0
#define R_L 1
#define T_L 2
#define T_H 3
#define N 4

const int32_t estyma_ct2_resistance_temperatue[] = {
    329927, 173153, -40, 30,
    173153, 95009, -30, 20,
    95009, 54247, -20, 10,
    54247, 32101, -10, 0,
    32101, 19621, 0, 10,
    19621, 12351, 10, 20,
    12351, 9900, 20, 25,
    9900, 7952, 25, 30,
    7952, 5227, 30, 40,
    5227, 3517, 40, 50,
    3517, 2418, 50, 60,
    2418, 1695, 60, 70,
    1695, 1211, 70, 80,
    1211, 881, 80, 90,
    881, 651, 90, 100,
    651, 488, 100, 110,
    488, 372, 110, 120,
    372, 306, 120, 130,
    306, 237, 130, 140,
    237, 153, 140, 150
};

int8_t estyma_ct2_valid_resistance(unsigned int resistance) {
    if(resistance > estyma_ct2_resistance_temperatue[R_H]) {
        return -1;
    } else if(resistance < estyma_ct2_resistance_temperatue[(RESOLUTION-1)*N + R_L]) {
        return 1;
    }

    return 0;
}

int estyma_ct2_temperature(unsigned int resistance) {
    for(int8_t i = 0; i < (RESOLUTION*N); i+=N) {
        if(resistance >= estyma_ct2_resistance_temperatue[i+R_L]) {
            float resistance_range = estyma_ct2_resistance_temperatue[i+R_H] - estyma_ct2_resistance_temperatue[i+R_L];
            float temperature_range = estyma_ct2_resistance_temperatue[i+T_H] - estyma_ct2_resistance_temperatue[i+T_L];
            float dt = (resistance - estyma_ct2_resistance_temperatue[i+R_L]) / resistance_range * temperature_range;
            return estyma_ct2_resistance_temperatue[i+T_H] - dt;
        }
    }

    return 160;
}
