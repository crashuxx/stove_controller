#ifndef __estyma_h__
#define __estyma_h__

#ifdef __MINGW32__
    #define uint unsigned int
#endif

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int8_t estyma_ct2_valid_resistance(uint resistance);
int estyma_ct2_temperature(uint resistance);

#ifdef __cplusplus
}
#endif

#endif