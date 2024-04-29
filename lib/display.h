#ifndef HOME_DISPLAY_H
#define HOME_DISPLAY_H

#include "pico/stdlib.h"

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

void init_lcd();

#endif //HOME_DISPLAY_H
