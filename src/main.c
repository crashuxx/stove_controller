#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"

#include "lib/lcd_1602_i2c/lcd_1602_i2c.h"
#include "lib/mcp3208/mcp3208.h"
#include "lib/kty/kty.h"

#define SPI_PORT spi0
#define READ_BIT 0x80


void init_i2c() {
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #error Requires a board with I2C pins
#endif
    
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
}

void init_spi() {
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_SCK_PIN)
    #error Requires a board with SPI pins
#endif

    spi_init(SPI_PORT, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PICO_DEFAULT_SPI_CSN_PIN);
    gpio_set_dir(PICO_DEFAULT_SPI_CSN_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));
}

#define TEMPERATURE_SAMPLES 16

struct temperatures_s {
    int8_t stove[TEMPERATURE_SAMPLES];
    int8_t heating[TEMPERATURE_SAMPLES];
    int8_t heating_return[TEMPERATURE_SAMPLES];
    int8_t water[TEMPERATURE_SAMPLES];
    int8_t smoke[TEMPERATURE_SAMPLES];

    uint8_t i;

    int8_t stove_avr;
    int8_t heating_avr;
    int8_t heating_return_avr;
    int8_t water_avr;
    int8_t smoke_avr;
} temperatures;

uint inline potential_differencef(uint r_value, float resolution, float r1) {
    return r_value/(resolution-r_value)*r1;
}


uint inline potential_difference(uint r_value, uint resolution, uint r1) {
    return potential_differencef(r_value, (float)resolution, (float)r1);
}

void temperatures_calculate_avr() {
    int stove_sum = 0;
    int heating_sum = 0;
    int heating_return_sum = 0;
    int water_sum = 0;
    int smoke_sum = 0;

    for(int8_t i = 0; i < TEMPERATURE_SAMPLES; i++) {
        stove_sum += temperatures.stove[i];
        heating_sum += temperatures.heating[i];
        heating_return_sum += temperatures.heating_return[i];
        water_sum += temperatures.water[i];
        smoke_sum += temperatures.smoke[i];
    }

    temperatures.stove_avr = stove_sum/TEMPERATURE_SAMPLES;
    temperatures.heating_avr = heating_sum/TEMPERATURE_SAMPLES;
    temperatures.heating_return_avr = heating_return_sum/TEMPERATURE_SAMPLES;
    temperatures.water_avr = water_sum/TEMPERATURE_SAMPLES;
    temperatures.smoke_avr = smoke_sum/TEMPERATURE_SAMPLES;
}

void temperatures_read_all() {
    uint8_t i = temperatures.i;

    temperatures.stove[i] = kty81_210_temperature(potential_difference(mcp3208_read_raw(0), 4095, 2000));
    temperatures.heating[i] = kty81_210_temperature(potential_difference(mcp3208_read_raw(1), 4095, 2000));
    temperatures.heating_return[i] = kty81_210_temperature(potential_difference(mcp3208_read_raw(2), 4095, 2000));
    temperatures.water[i] = kty81_210_temperature(potential_difference(mcp3208_read_raw(3), 4095, 2000));
    temperatures.smoke[i] = kty81_210_temperature(potential_difference(mcp3208_read_raw(7), 4095, 2000));

    temperatures.i++;
    if (temperatures.i >= TEMPERATURE_SAMPLES) temperatures.i = 0;
}

int main() {
    init_i2c();
    init_spi();
    lcd_init();

    memset(&temperatures, 0, sizeof(temperatures));

    lcd_set_cursor(0, 0);
    lcd_string("Loading...");

    int stove_sum = 0;
    int heating_sum = 0;
    int heating_return_sum = 0;
    int water_sum = 0;
    int smoke_sum = 0;
    for(int8_t i = temperatures.i; i < TEMPERATURE_SAMPLES; i++) {
        temperatures_read_all();
    }

    temperatures_calculate_avr();
 

    sleep_ms(1000);

    while (1) {
        temperatures_read_all();
        temperatures_calculate_avr();

        uint value = mcp3208_read_raw(0);
        char tmps1[16];
        uint r = potential_difference(value, 4095, 2000);

        sprintf(tmps1, "Temp: %d C    ", temperatures.stove_avr);
        lcd_set_cursor(0, 0);
        lcd_string(tmps1);

        sleep_ms(500);
/*        for (int m = 0; m < sizeof(message) / sizeof(message[0]); m += LCD_MAX_LINES) {
            for (int line = 0; line < LCD_MAX_LINES; line++) {
                lcd_set_cursor(line, (LCD_MAX_CHARS / 2) - strlen(message[m + line]) / 2);
                lcd_string(message[m + line]);
            }
            sleep_ms(2000);
            lcd_clear();
        }*/
    }

    return 0;
}