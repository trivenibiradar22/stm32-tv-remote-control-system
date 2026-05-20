/**
 * lcd16x2.c — HD44780 16x2 LCD driver (4-bit mode)
 *
 * All pins must be on the SAME GPIO port (passed to lcd16x2_init).
 * Pin wiring used in TV Remote project:
 *   Port B: RS=PB6, RW=PB7, EN=PB8, D4=PB9, D5=PB10, D6=PB1, D7=PB0
 */

#include "lcd16x2.h"
#include <stdio.h>
#include <string.h>

/* ---- Internal state ---- */
static GPIO_TypeDef *_port;
static uint16_t _rs, _rw, _en;
static uint16_t _d4, _d5, _d6, _d7;

/* ---- LCD commands ---- */
#define CMD_CLEAR        0x01
#define CMD_HOME         0x02
#define CMD_ENTRY_MODE   0x06   /* increment, no shift */
#define CMD_DISPLAY_ON   0x0C   /* display on, cursor off, blink off */
#define CMD_FUNCTION_SET 0x28   /* 4-bit, 2-line, 5x8 */
#define CMD_SET_DDRAM    0x80

/* ================================================================
   Low-level helpers
   ================================================================ */

static inline void _delay(uint32_t ms) { HAL_Delay(ms); }

/* Pulse the EN pin to latch data */
static void _pulse_en(void) {
    HAL_GPIO_WritePin(_port, _en, GPIO_PIN_SET);
    _delay(1);
    HAL_GPIO_WritePin(_port, _en, GPIO_PIN_RESET);
    _delay(1);
}

/* Send one nibble (upper 4 bits of value) on D4–D7 */
static void _send_nibble(uint8_t nibble) {
    /* nibble bits 0-3 map to D4-D7 */
    HAL_GPIO_WritePin(_port, _d4, (nibble >> 0) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_port, _d5, (nibble >> 1) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_port, _d6, (nibble >> 2) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_port, _d7, (nibble >> 3) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    _pulse_en();
}

/* Send a full byte in two nibbles: high nibble first, then low nibble */
static void _send_byte(uint8_t byte, uint8_t is_data) {
    /* RS: 1 = data, 0 = command */
    HAL_GPIO_WritePin(_port, _rs, is_data ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_port, _rw, GPIO_PIN_RESET);   /* always write */

    _send_nibble(byte >> 4);   /* high nibble */
    _send_nibble(byte & 0x0F); /* low  nibble */

    _delay(2);
}

/* ================================================================
   Public API
   ================================================================ */

/**
 * lcd16x2_init — initialise the LCD in 4-bit mode.
 *
 * All pins MUST be on the same port.
 * Call once after MX_GPIO_Init().
 */
void lcd16x2_init(GPIO_TypeDef *port,
                  uint16_t rs_pin, uint16_t rw_pin, uint16_t en_pin,
                  uint16_t d4_pin, uint16_t d5_pin,
                  uint16_t d6_pin, uint16_t d7_pin)
{
    _port = port;
    _rs   = rs_pin;
    _rw   = rw_pin;
    _en   = en_pin;
    _d4   = d4_pin;
    _d5   = d5_pin;
    _d6   = d6_pin;
    _d7   = d7_pin;

    /* HD44780 power-on sequence */
    _delay(50);

    HAL_GPIO_WritePin(_port, _rs | _rw, GPIO_PIN_RESET);

    /* Step 1-3: send nibble 0x3 three times (8-bit reset) */
    for (int i = 0; i < 3; i++) {
        _send_nibble(0x3);
        _delay(5);
    }

    /* Step 4: send nibble 0x2 → switches to 4-bit mode */
    _send_nibble(0x2);
    _delay(5);

    /* Now use full 2-nibble commands */
    _send_byte(CMD_FUNCTION_SET, 0);  /* 4-bit, 2-line, 5x8 */
    _send_byte(CMD_DISPLAY_ON,   0);  /* display on           */
    _send_byte(CMD_CLEAR,        0);  /* clear display        */
    _delay(2);
    _send_byte(CMD_ENTRY_MODE,   0);  /* entry mode           */
}

/** Clear display and return cursor to home */
void lcd16x2_clear(void) {
    _send_byte(CMD_CLEAR, 0);
    _delay(2);
}

/** Move cursor to (0,0) without clearing */
void lcd16x2_home(void) {
    _send_byte(CMD_HOME, 0);
    _delay(2);
}

/**
 * lcd16x2_setCursor — move cursor to row/col.
 * row: 0 = first line, 1 = second line
 * col: 0–15
 */
void lcd16x2_setCursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? (CMD_SET_DDRAM + col)
                               : (CMD_SET_DDRAM + 0x40 + col);
    _send_byte(addr, 0);
}

/** Write a single character */
void lcd16x2_putChar(char c) {
    _send_byte((uint8_t)c, 1);
}

/** Write a null-terminated string */
void lcd16x2_print(const char *str) {
    while (*str) {
        lcd16x2_putChar(*str++);
    }
}

/**
 * lcd16x2_printf — printf-style formatted output to LCD.
 * Maximum formatted string length: 32 characters.
 */
void lcd16x2_printf(const char *fmt, ...) {
    char buf[33];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    lcd16x2_print(buf);
}
