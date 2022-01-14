#ifndef __pt_h__
#define __pt_h__

#ifdef __cplusplus
extern "C" {
#endif

int8_t pt1000_valid_resistance(unsigned int resistance);
int pt1000_temperature(unsigned int resistance);

#ifdef __cplusplus
}
#endif

#endif
