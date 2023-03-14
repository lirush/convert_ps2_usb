#ifndef _MATRIX_H
#define _MATRIX_H

#include "stdint.h"

/* matrix size */
#define MATRIX_ROWS 													32  // keycode bit: 7-3
#define MATRIX_COLS 													8   // keycode bit: 2-0

#define PS2_CODE_MODIFIER_KEYBOARD_LCTRL      0x14
#define PS2_CODE_MODIFIER_KEYBOARD_LSHFT      0x12
#define PS2_CODE_MODIFIER_KEYBOARD_LALT       0x11
#define PS2_CODE_MODIFIER_KEYBOARD_LGUI       0x1F
#define PS2_CODE_MODIFIER_KEYBOARD_RCTRL      0x14
#define PS2_CODE_MODIFIER_KEYBOARD_RSHFT      0x59
#define PS2_CODE_MODIFIER_KEYBOARD_RALT       0x11
#define PS2_CODE_MODIFIER_KEYBOARD_RGUI       0x27

extern uint8_t matrix[MATRIX_ROWS][MATRIX_COLS];

enum {UP_MATRIX,DOWN_MATRIX};

enum {MAKE_MATRIX, BREAK_MATRIX};

uint8_t Matrix_Get_Scancode(uint8_t code, uint8_t move);
uint8_t getRow(uint8_t code);
uint8_t getCol(uint8_t code);

#endif
