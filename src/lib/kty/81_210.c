#include <stdint.h>
#include <stdio.h>
#include "kty.h"

#define RESOLUTION 23
#define R_L 0
#define R_R 1
#define T_L 2
#define T_R 3
#define N 4

const int32_t kty81_210_resistance_temperatue[] = {
    980, 1030, -55, -50,
    1030, 1135, -50, -40,
    1135, 1247, -40, -30,
    1247, 1367, -30, -20,
    1367, 1495, -20, -10,
    1495, 1630, -10, 0,
    1630, 1772, 0, 10,
    1772, 1922, 10, 20,
    1922, 2000, 20, 25,
    2000, 2080, 25, 30,
    2080, 2245, 30, 40,
    2245, 2417, 40, 50,
    2417, 2597, 50, 60,
    2597, 2785, 60, 70,
    2785, 2980, 70, 80,
    2980, 3182, 80, 90,
    3182, 3392, 90, 100,
    3392, 3607, 100, 110,
    3607, 3817, 110, 120,
    3817, 3915, 120, 125,
    3915, 4008, 125, 130,
    4008, 4166, 130, 140,
    4166, 4280, 140, 150
};

int8_t kty81_210_valid_resistance(uint resistance) {
    if(resistance < kty81_210_resistance_temperatue[0]) {
        return -1;
    } else if(resistance > kty81_210_resistance_temperatue[RESOLUTION*(N-1) + R_R]) {
        return 1;
    }

    return 0;
}

int kty81_210_temperature(uint resistance) {
    for(int8_t i = 0; i < (RESOLUTION*N); i+=N) {
        if(resistance < kty81_210_resistance_temperatue[i+R_R]) {
            float resistance_range = kty81_210_resistance_temperatue[i+R_R] - kty81_210_resistance_temperatue[i+R_L];
            float temperature_range = kty81_210_resistance_temperatue[i+T_R] - kty81_210_resistance_temperatue[i+T_L];
            float dt = ((float)(resistance - kty81_210_resistance_temperatue[i+R_L])) / resistance_range * temperature_range;
            return kty81_210_resistance_temperatue[i+T_L] + dt;
        }
    }

    return 160;
}
