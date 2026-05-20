#ifndef INC_LCD16X2_H_
#define INC_LCD16X2_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdarg.h>

/* ============================================================
 * lcd16x2 — 4-bit mode driver for HD44780 compatible LCD
 * Pins used (set by lcd16x2_init):
 *   RS, RW, EN  — control lines
 *   D4, D5, D6, D7 — data lines (all on same GPIO port)
 * ============================================================ */

void lcd16x2_init(GPIO_TypeDef *port,
                  uint16_t rs_pin, uint16_t rw_pin, uint16_t en_pin,
                  uint16_t d4_pin, uint16_t d5_pin,
                  uint16_t d6_pin, uint16_t d7_pin);

void lcd16x2_clear(void);
void lcd16x2_home(void);
void lcd16x2_setCursor(uint8_t row, uint8_t col);
void lcd16x2_putChar(char c);
void lcd16x2_print(const char *str);
void lcd16x2_printf(const char *fmt, ...);

#endif /* INC_LCD16X2_H_ */
