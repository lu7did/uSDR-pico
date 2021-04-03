#ifndef __LCD_H__
#define __LCD_H__

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define LCD_CLEAR		0
#define LCD_HOME		1
#define LCD_BLINK		2
#define LCD_GOTO		3

void lcd_init(void);
void lcd_ctrl(uint8_t cmd, uint8_t x, uint8_t y);
void lcd_write(uint8_t *s, uint8_t len);


#endif