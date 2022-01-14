#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"

#include "common.h"
#include "state.h"
#include "lib/lcd_1602_i2c/lcd_1602_i2c.h"
#include "lib/mcp3208/mcp3208.h"
#include "lib/kty/kty.h"
#include "lib/estyma/estyma.h"

#define SPI_PORT spi0
#define READ_BIT 0x80

temperatures_t temperatures;


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


#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void init_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

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

    temperatures.stove[i] = estyma_ct2_temperature(potential_difference(mcp3208_read_raw(7), 4095, 10000));
    temperatures.heating[i] = estyma_ct2_temperature(potential_difference(mcp3208_read_raw(6), 4095, 10000));
    temperatures.heating_return[i] = estyma_ct2_temperature(potential_difference(mcp3208_read_raw(5), 4095, 10000));
    temperatures.water[i] = estyma_ct2_temperature(potential_difference(mcp3208_read_raw(4), 4095, 10000));
    temperatures.smoke[i] = kty81_210_temperature(potential_difference(mcp3208_read_raw(0), 4095, 2000));

    temperatures.i++;
    if (temperatures.i >= TEMPERATURE_SAMPLES) temperatures.i = 0;
}


int main() {
    //init_i2c();
    init_spi();
    init_uart();
    

    gpio_init(12);
    gpio_set_dir(12, GPIO_OUT);
    gpio_put(12, 1);
    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);
    gpio_put(13, 1);
    gpio_init(14);
    gpio_set_dir(14, GPIO_OUT);
    gpio_put(14, 1);
    gpio_init(15);
    gpio_set_dir(15, GPIO_OUT);
    gpio_put(15, 1);

    memset(&temperatures, 0, sizeof(temperatures));

    //lcd_set_cursor(0, 0);
    //lcd_string("Loading...");

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

    stove_state_t stove_state = SETTING_FIRE;
    
    bool co_pump = false;
    bool cw_pump = false;
    bool feeder_state = false;
    uint64_t feeder_time = time_us_64() + 5 * 1000000;
    uint64_t msg_time = time_us_64();

    while (1) {
        temperatures_read_all();
        temperatures_calculate_avr();

        stove_state_t stove_state_last = stove_state;
        stove_state = update_state(stove_state, temperatures);
        //stove_state = BURNING;

        if (stove_state_last != stove_state) {
            feeder_state = false;
        }

        switch(stove_state) {
            default:
            case OFF: 
            case EXCTINCTION:
                //gpio_put(12, 1);
                feeder_state = false;
                gpio_put(13, 1);
                gpio_put(14, 1); //co pump
                gpio_put(15, 1); //cw pump
                break;

            case SETTING_FIRE:
                gpio_put(13, 0); // fan
                gpio_put(14, 1); //co pump
                gpio_put(15, 1);

                if (feeder_state) {
                    if ((feeder_time + 4 * 1000000) <= time_us_64()) {
                        feeder_state = false;
                    }
                } else {
                    if ((feeder_time + 4 * 1000000 + 30 * 1000000) <= time_us_64()) {
                        feeder_state = true;
                        feeder_time = time_us_64();
                    }
                }

                break;

            case BURNING:
                gpio_put(13, 0); // fan
                gpio_put(14, 0); //co pump

                if (feeder_state) {
                    if ((feeder_time + 8 * 1000000) <= time_us_64()) {
                        feeder_state = false;
                    }
                } else {
                    if ((feeder_time + 8 * 1000000 + 30 * 1000000) <= time_us_64()) {
                        feeder_state = true;
                        feeder_time = time_us_64();
                    }
                }

                if (temperatures.water_avr < 42) {
                    gpio_put(15, 0); //cw pump
                } else {
                    gpio_put(15, 1); //cw pump
                }

                break;

            case COOLING:
                feeder_state = false;
                gpio_put(13, 1); // fan
                gpio_put(14, 0); //co pump

                if (temperatures.water_avr < 42) {
                    gpio_put(15, 0); //cw pump
                } else {
                    gpio_put(15, 1); //cw pump
                }

                break;
            
            case PASSIVE:
                gpio_put(13, 1); // fan
                gpio_put(14, 0); //co pump

                if (feeder_state) {
                    if ((feeder_time + 10 * 1000000) <= time_us_64()) {
                        feeder_state = false;
                    }
                } else {
                    if ((feeder_time + 10 * 1000000 + 120 * 1000000) <= time_us_64()) {
                        feeder_state = true;
                        feeder_time = time_us_64();
                    }
                }

                if (temperatures.water_avr < 42) {
                    gpio_put(15, 0); //cw pump
                } else {
                    gpio_put(15, 1); //cw pump
                }

                break;

        }

        gpio_put(12, !feeder_state);

        char tmps1[128];
        //char tmps2[16];

        //sprintf(tmps1, "Temp: %d C    ", temperatures.stove_avr);
        //lcd_set_cursor(0, 0);
        //lcd_string(tmps1);

        //sprintf(tmps1, "State: %d C    ", stove_state);
        //lcd_set_cursor(1, 0);
        //lcd_string(tmps1);

        if((msg_time + 10 * 1000000) <= time_us_64()) {
            sprintf(tmps1, "s: %3d h: %3d hr: %3d w: %3d sm: %3d st:%d\n", temperatures.stove_avr, temperatures.heating_avr, temperatures.heating_return_avr, temperatures.water_avr, temperatures.smoke_avr, stove_state);
            uart_puts(UART_ID, tmps1);
            msg_time = time_us_64();
        }

        // int rabt = MIN(uart_is_readable(UART_ID), 16);
        // if (rabt > 0) {
        //     uart_read_blocking()
        // }

        sleep_ms(500);
    }

    return 0;
}