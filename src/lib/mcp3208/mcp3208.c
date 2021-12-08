#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#include "mcp3208.h"

#define SPI_PORT spi0
#define READ_BIT 0x80

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    asm volatile("nop \n nop \n nop");
}

uint16_t mcp3208_read_raw(uint8_t channel) {
    uint8_t out_buffer[4] = {0x01, (1 << 7 | channel << 4), 0x00, 0x00};
    uint8_t in_buffer[4] = {0x00, 0x00, 0x00, 0x00};
    
    cs_select();
    //spi_write_blocking(SPI_PORT, &out_buffer[0], 2);
    //sleep_ms(10);
    //spi_read_blocking(SPI_PORT, 0, &in_buffer[0], 3);
    spi_write_read_blocking(SPI_PORT, (uint8_t*)&out_buffer, (uint8_t*)&in_buffer, 4);
    cs_deselect();

    return ((uint16_t)(in_buffer[1] & 0x03) << 10) | in_buffer[2] << 2 | (0xC0 & in_buffer[3]) >> 6;
}