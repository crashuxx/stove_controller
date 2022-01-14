#include <stdint.h>
#include <stdio.h>
#include "pt.h"

#define RESOLUTION 58
#define R 1
#define T 0
#define N 2

const int pt1000_resistance_temperatue[] = {
    -70,	723,
    -60,	763,
    -50,	803,
    -40,	842,
    -30,	882,
    -20,	921,
    -10,	960,
    0,  	1000,
    10, 	1039,
    20, 	1077,
    30,	    1116,
    40,	    1155,
    50,	    1194,
    60,	    1232,
    70,	    1270,
    80, 	1309,
    90, 	1347,
    100,	1385,
    110,	1422,
    120,	1460,
    130,	1498,
    140,	1535,
    150,	1573,
    160,	1610,
    170,	1647,
    180,	1684,
    190,	1721,
    200,	1758,
    210,	1795,
    220,	1831,
    230,	1868,
    240,	1904,
    250,	1941,
    260,	1977,
    270,	2013,
    280,	2049,
    290,	2084,
    300,	2120,
    310,	2156,
    320,	2191,
    330,	2226,
    340,	2262,
    350,	2297,
    360,	2332,
    370,	2367,
    380,	2401,
    390,	2436,
    400,	2470,
    410,	2505,
    420,	2539,
    430,	2573,
    440,	2607,
    450,	2641,
    460,	2675,
    470,	2709,
    480,	2742,
    490,	2776,
    500,	2809
};

int8_t pt1000_valid_resistance(unsigned int resistance) {
    if(resistance < pt1000_resistance_temperatue[R]) {
        return -1;
    } else if(resistance > pt1000_resistance_temperatue[(RESOLUTION-1)*N + R]) {
        return 1;
    }

    return 0;
}

int pt1000_temperature(unsigned int resistance) {
    if(resistance < pt1000_resistance_temperatue[R])
        return pt1000_resistance_temperatue[RESOLUTION+T] - 1;

    for(int8_t i = N; i < (RESOLUTION*N); i+=N) {
        if(resistance <= pt1000_resistance_temperatue[i+R]) {
            float resistance_range = pt1000_resistance_temperatue[i+R] - pt1000_resistance_temperatue[i-N+R];
            float temperature_range = pt1000_resistance_temperatue[i+T] - pt1000_resistance_temperatue[i-N+T];
            float dt = ((float)(resistance - pt1000_resistance_temperatue[i-N+R])) / resistance_range * temperature_range;
            return pt1000_resistance_temperatue[i-N+T] + dt;
        }
    }

    return pt1000_resistance_temperatue[RESOLUTION-N+T] + 1;
}