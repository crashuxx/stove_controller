#ifndef __ntc_h__
#define __ntc_h__

#ifdef __cplusplus
extern "C" {
#endif

int8_t ntc10k_b3900k_valid_resistance(unsigned int resistance);
int ntc10k_b3900k_temperature(unsigned int resistance);

#ifdef __cplusplus
}
#endif

#endif
