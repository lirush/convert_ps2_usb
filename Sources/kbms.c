#include "type.h"
#include "kbms.h"
#include "ps2phy.h"
#include "systick.h"
#include "ps2protocol.h"
#include "queue.h"
#include "hid.h"
#include "matrix.h"
#include "string.h"

keyrecord_t KeyRecord;

PS2_MOUSE_STATUS_t mouse_status;
PS2_KBOARD_STATUS_t kboard_status;

uint8_t KB_data=0;
uint8_t It_Is_Modifier=0;
uint8_t mode_alt_tab=0;
uint8_t scan_code_out[7]={0};

struct mouseHID_t mouseHID;
struct keyboardHID_t keyboardHID;

void tap_manager(uint8_t code);
uint8_t Mod_ALT_TAB(uint8_t code);

void Send_Repeatly(uint8_t cmd)
{
		MDR_PORTD->FUNC &= ~(1<<10); 											//override func
		MDR_PORTD->FUNC &= ~(1<<11);
	
	while(PS2_Host_Send(cmd)==0);
}

void Mouse_Init(void)
{
	uint8_t rcv=0;
	
	Mouse_PS2_PhyON();

	_delay_ms(1000);
	
	//send reset
	Send_Repeatly(0xFF);
	while(PS2_Host_Receive() != 0xFA){;	}
	
	 //read completion code of BAT
	while(PS2_Host_Receive() != 0xAA){;	}
	
	// read Device ID
	rcv = PS2_Host_Receive();
	
	//send reset
	Send_Repeatly(0xFF);	
	while(PS2_Host_Receive() != 0xFA){;	}
	
	 //read completion code of BAT
	while(PS2_Host_Receive() != 0xAA){;	}
	
	// read Device ID
	rcv = PS2_Host_Receive();	
	
	//send reset
	Send_Repeatly(0xFF);
	while(PS2_Host_Receive() != 0xFA){;	}
	
	//read completion code of BAT
	while(PS2_Host_Receive() != 0xAA){;	}
	
	// read Device ID
	rcv = PS2_Host_Receive();

	Send_Repeatly(0xF3);
	rcv = PS2_Host_Receive();
	
	Send_Repeatly(200);
	rcv = PS2_Host_Receive();
	
	Send_Repeatly(0xF3);
	rcv = PS2_Host_Receive();
	
	Send_Repeatly(100);
	rcv = PS2_Host_Receive();
	
	Send_Repeatly(0xF3);
	rcv = PS2_Host_Receive();	
	
	Send_Repeatly(80);
	rcv = PS2_Host_Receive();	
	
	Send_Repeatly(0xF2);
	rcv = PS2_Host_Receive();		
	
	rcv = PS2_Host_Receive();	
	
	Send_Repeatly(0xF4);
	rcv = PS2_Host_Receive();	
}

void Mouse_GetData(void)
{
	uint8_t tmp=0;	
	uint8_t sx=0,sy=0;
	struct mouseHID_t mouse;
	
	if (QueueIn.last) 
	{
		if(uiQueTop(&QueueIn)==0x00FA)QueDelete(&QueueIn);
		
		if((uiQueTop(&QueueIn) & (1<<3))){
		
		while(QueueIn.last==0)
			;		
		{
			tmp = (uint8_t)uiQueTop(&QueueIn);
			sx = (tmp & (1<<4))?1:0;
			sy = (tmp & (1<<5))?1:0;
			mouse.btn = tmp & 0x07;
			QueDelete(&QueueIn);
		}
		while(QueueIn.last==0)
			;
		{
			mouse.X = (uint8_t)uiQueTop(&QueueIn);
			QueDelete(&QueueIn);
		}
		while(QueueIn.last==0)
			;		
		{
			mouse.Y = (uint8_t)uiQueTop(&QueueIn);
			if(sy)
				mouse.Y = 255-mouse.Y;
			else if(mouse.Y != 0x00)mouse.Y ^= 0xFF;										//чтобы не перемещался курсор по нажатию кнопки
			
			QueDelete(&QueueIn);
		}

		while(QueueIn.last==0)
			;		
		mouse.scroll = (uint8_t)uiQueTop(&QueueIn);
		
		if(USE_MOUSE_STANDART==1){
			mouse.scroll *= -1;
		}
		
		QueDelete(&QueueIn);
		
		Flag_mb=0; BufferReceive_mb=0;
		
		mouse_status=MOUSE_NEW_DATA;
		memcpy(&mouseHID,&mouse,sizeof(mouseHID));
	}
}
}

static void Modifier_Set(uint8_t code, uint8_t move)
{
	switch(move)
	{
		case 0:
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_LCTRL) It_Is_Modifier |=(1<<HID_USAGE_KEYBOARD_MODIFIER_LCTRL_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_LSHFT) It_Is_Modifier |=(1<<HID_USAGE_KEYBOARD_MODIFIER_LSHFT_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_LALT) It_Is_Modifier |=(1<<HID_USAGE_KEYBOARD_MODIFIER_LALT_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_RSHFT) It_Is_Modifier |=(1<<HID_USAGE_KEYBOARD_MODIFIER_RSHFT_BIT);
		break;
		case 1:
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_LGUI) It_Is_Modifier |=(1<<HID_USAGE_KEYBOARD_MODIFIER_LGUI_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_RCTRL) It_Is_Modifier |=(1<<HID_USAGE_KEYBOARD_MODIFIER_RCTRL_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_RALT) It_Is_Modifier |=(1<<HID_USAGE_KEYBOARD_MODIFIER_RALT_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_RGUI) It_Is_Modifier |=(1<<HID_USAGE_KEYBOARD_MODIFIER_RGUI_BIT);
		break;
	}
}

static void Modifier_Reset(uint8_t code, uint8_t move)
{
	switch(move)
	{
		case 0:
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_LCTRL) It_Is_Modifier &= ~(1<<HID_USAGE_KEYBOARD_MODIFIER_LCTRL_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_LSHFT) It_Is_Modifier &= ~(1<<HID_USAGE_KEYBOARD_MODIFIER_LSHFT_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_LALT) It_Is_Modifier &= ~(1<<HID_USAGE_KEYBOARD_MODIFIER_LALT_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_RSHFT) It_Is_Modifier &= ~(1<<HID_USAGE_KEYBOARD_MODIFIER_RSHFT_BIT);
		break;
		case 1:
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_LGUI) It_Is_Modifier &= ~(1<<HID_USAGE_KEYBOARD_MODIFIER_LGUI_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_RCTRL) It_Is_Modifier &= ~(1<<HID_USAGE_KEYBOARD_MODIFIER_RCTRL_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_RALT) It_Is_Modifier &= ~(1<<HID_USAGE_KEYBOARD_MODIFIER_RALT_BIT);
			if(code ==  PS2_CODE_MODIFIER_KEYBOARD_RGUI) It_Is_Modifier &= ~(1<<HID_USAGE_KEYBOARD_MODIFIER_RGUI_BIT);
		break;
	}	
}

void matrix_scan(void)
{
	uint8_t code=0;
	
	static enum {
			INIT,
			F0,
			E0,
			E0_F0,
			// Pause
			E1,
			E1_14,
			E1_14_77,
			E1_14_77_E1,
			E1_14_77_E1_F0,
			E1_14_77_E1_F0_14,
			E1_14_77_E1_F0_14_F0,
			// Control'd Pause
			E0_7E,
			E0_7E_E0,
			E0_7E_E0_F0,
	} state = INIT;	
	
	kboard_status = KBOARD_INAKTIV;
	
	if(record_key.last==0)return;
	
	code = (uint8_t)uiQueTop(&record_key);
	
	tap_manager(code);
	
//		if(Mod_ALT_TAB(code) == 0)
		switch((uint8_t)state)
		{
			case INIT:
				switch(code)
				{
					case 0xE0:
						state = E0;
					break;
					case 0xF0:
						state = F0;
					break;
					case 0x83:
						KB_data = 0x40;
						state = INIT;
						kboard_status = KBOARD_NEW_DATA;
					break;
					default:
						KB_data = Matrix_Get_Scancode(code,UP_MATRIX);
						state = INIT;
						Modifier_Set(code,UP_MATRIX);
						kboard_status = KBOARD_NEW_DATA;
					break;
				}
			break;
			case E0:
				switch(code)
				{
					case 0x12:  // игнорить
					case 0x59:  // игнорить
							state = INIT;	
					break;
					case 0x7E:  // Control'd Pause
							state = E0_7E;
					break;				
					case 0xF0:
						state = E0_F0;					
					break;
					default:
						KB_data = Matrix_Get_Scancode(code,DOWN_MATRIX);	
						state = INIT;
						Modifier_Set(code,DOWN_MATRIX);
						kboard_status = KBOARD_NEW_DATA;
					break;
				}
			break;
			case F0:		
				Modifier_Reset(code,UP_MATRIX);
				KB_data=0x00;
				state = INIT;		
			break;
			case E0_F0:		
				switch(code)
				{
					case 0x12:  // игнорить
					case 0x59:  // игнорить
							state = INIT;	
					break;	
					default:
						Modifier_Reset(code,DOWN_MATRIX);
						KB_data=0x00;
						state = INIT;			
					break;
				}
			break;		
				// копка Pause
			case E1:
					switch (code) {
							case 0x14:
									state = E1_14;
									break;
							default:
									state = INIT;
					}
					break;
			case E1_14:
					switch (code) {
							case 0x77:
									state = E1_14_77;
									break;
							default:
									state = INIT;
					}
					break;
			case E1_14_77:
					switch (code) {
							case 0xE1:
									state = E1_14_77_E1;
									break;
							default:
									state = INIT;
					}
					break;
			case E1_14_77_E1:
					switch (code) {
							case 0xF0:
									state = E1_14_77_E1_F0;
									break;
							default:
									state = INIT;
					}
					break;
			case E1_14_77_E1_F0:
					switch (code) {
							case 0x14:
									state = E1_14_77_E1_F0_14;
									break;
							default:
									state = INIT;
					}
					break;
			case E1_14_77_E1_F0_14:
					switch (code) {
							case 0xF0:
									state = E1_14_77_E1_F0_14_F0;
									break;
							default:
									state = INIT;
					}
					break;
			case E1_14_77_E1_F0_14_F0:
					switch (code) {
							case 0x77:
								KB_data = 0x48;		
								state = INIT;
								kboard_status = KBOARD_NEW_DATA;
								break;
							default:
									state = INIT;
					}
			break;
			// Control'd Pause
			case E0_7E:
					if (code == 0xE0)
							state = E0_7E_E0;
					else
							state = INIT;
					break;
			case E0_7E_E0:
					if (code == 0xF0)
							state = E0_7E_E0_F0;
					else
							state = INIT;
					break;
			case E0_7E_E0_F0:
					if (code == 0x7E)
					{
						KB_data = 0x48;		
						kboard_status = KBOARD_NEW_DATA;
					}
				state = INIT;
			break;				
			default:
					state = INIT;
			break;		
		}
//	if(KeyRecord.tap.count<=1) kboard_status = KBOARD_NEW_DATA;
//	if(KeyRecord.tap.count>1) kboard_status = KBOARD_UPDATE_DATA;	
	QueDelete(&record_key);
}

void tap_manager(uint8_t code)
{
	keyrecord_t *record = &KeyRecord;
	keyevent_t *event = &record->event;
	
	switch(code)
	{
		case 0xE0:
		break;
		case 0xF0:
			event->key.col = getCol(code);
			event->key.row = getRow(code);
			event->pressed = 0;
			record->tap.count=0;
		break;
		default:
			event->key.col = getCol(code);
			event->key.row = getRow(code);
			event->pressed = 1;
			record->tap.count++;
		break;
	}	
}

uint8_t Mod_ALT_TAB(uint8_t code)
{
	static enum{
		INIT,
		IN,
		MODE,
		CHECK_MODE,
		OUT
	}state;
	static uint32_t wait_mode=0;
		
	switch((uint8_t) state)
	{
		case INIT:
			if(code == 0x11)state = IN;
			if(wait_mode++>=1000)state = INIT;
			KB_data = 0xE2;
		break;
		case IN:
			if(code == 0x0D)state = MODE;
			KB_data = 0x2B;
		break;
		case MODE:
			if(code == 0xF0)state = CHECK_MODE;
			mode_alt_tab=1;
		break;
		case CHECK_MODE:
			if(code==0x11)state=OUT;
			if(code == 0x0D)state = MODE;
		break;
		case OUT:
			if(code == 0x11)state = INIT;
			mode_alt_tab=0;
			wait_mode=0;
		break;									
		default:
			
		break;
	}
	return (uint8_t)state;
}

void scan_code(void)
{
	static enum{
		F0=0xF0,
		AFTER_F0
	}state;
	uint8_t code=0;
	static uint8_t move_matrix=UP_MATRIX;
	
	if(record_key.last==0)return;
	
	code = (uint8_t)uiQueTop(&record_key);
	
	if(code==0xE0){move_matrix=DOWN_MATRIX;QueDelete(&record_key);return;}
	
	if(code==0xF0){
		KeyRecord.event.pressed=0;
		KeyRecord.event.code=0;
		KeyRecord.tap.count=0;	
		kboard_status=KBOARD_INAKTIV;
		state = F0;
		QueDelete(&record_key);
		return;
	}

	if(state==F0){
		state=0;
		QueDelete(&record_key);
		return;
	}
	
	KeyRecord.event.code=Matrix_Get_Scancode(code,move_matrix);
	KeyRecord.event.pressed=1;
	if(++KeyRecord.tap.count==1)kboard_status=KBOARD_NEW_DATA;
	if(KeyRecord.tap.count>1)kboard_status=KBOARD_UPDATE_DATA;
	
	QueDelete(&record_key);
}
