#ifndef __lcd_1602_i2c_h__
#define __lcd_1602_i2c_h__

#define LCD_MAX_LINES      2
#define LCD_MAX_CHARS      16

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val);
void lcd_toggle_enable(uint8_t val);

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode);
void lcd_clear(void);
// go to location on LCD
void lcd_set_cursor(int line, int position);
static void inline lcd_char(char val);
void lcd_string(const char *s);
void lcd_init();

#endif