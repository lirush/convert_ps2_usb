#ifndef _KBMS_H
#define _KBMS_H

#include "stdint.h"

#define TICK \
    (keyevent_t) { .key = (keypos_t){.row = 255, .col = 255}, .pressed = false, .time = 1 }
		
#define USE_MOUSE_STANDART		(1)

struct keyboardHID_t {
//      uint8_t id;
      uint8_t modifiers;
			uint8_t rezerv;
      uint8_t key1;
      uint8_t key2;
      uint8_t key3;
			uint8_t key4;
			uint8_t key5;
			uint8_t key6;
};
extern struct keyboardHID_t keyboardHID;
	
struct mediaHID_t {
    uint8_t id;
		uint8_t keys;
};

struct mouseHID_t {
//		uint8_t id;
		uint8_t btn;
		uint8_t X;
		uint8_t Y;
		uint8_t scroll;
};
extern struct mouseHID_t mouseHID;

typedef enum {
  MOUSE_INAKTIV = 0,    // Мышка неактивна
  MOUSE_AKTIV,          // Мышка активна
  MOUSE_NEW_DATA        // Новые данные
}PS2_MOUSE_STATUS_t;
extern PS2_MOUSE_STATUS_t mouse_status;

typedef enum {
  KBOARD_INAKTIV = 0,    // Клавиатура неактивна
  KBOARD_AKTIV,          // Клавиатура активна
  KBOARD_NEW_DATA,       // Новые данные
	KBOARD_UPDATE_DATA     //
}PS2_KBOARD_STATUS_t;
extern PS2_KBOARD_STATUS_t kboard_status;

/* key matrix position */
typedef struct {
    uint8_t col;
    uint8_t row;
} keypos_t;

/* key event */
typedef struct {
		uint8_t code;
    keypos_t key;
    bool     pressed;
    uint16_t time;
} keyevent_t;

typedef struct {
    bool    interrupted : 1;
    bool    reserved2 	: 1;
    bool    reserved1 : 1;
    bool    reserved0 : 1;
    uint8_t count : 4;
} tap_t;

typedef struct{
	keyevent_t event;
	tap_t tap;
}keyrecord_t;

extern uint8_t KB_data;
extern uint8_t It_Is_Modifier;

extern keyrecord_t KeyRecord;

void Mouse_Init(void);

void Mouse_GetData(void);
void scan_code(void);
void matrix_scan(void);
void Keyboard_Init(void);
#endif
