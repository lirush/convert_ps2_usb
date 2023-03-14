/**
***************************************************************************************************************
\mainpage 
Эта программа была разработана в среде программирования MDK-Lite 5.27.1.0 \n
Преобразователь протокола PS/2 мышии PS/2 клавиатуры в USB. \n
Matrix_scan() - взято из проекта А.Лукьяненко (АМЭ ЦПР Начальник отдела схемотехники и программирования). \n
Кадр PS/2 записывается по прерыванию: Timer2 - мышь, Timer3 - клавиатура. \n
Содержимое в обработчиках прерывания взято из проекта А.Лукьяненко (АМЭ ЦПР Начальник отдела схемотехники и 
программирования). \n
***************************************************************************************************************
Project : Firmware \n
Version : 1.0 \n
Date    : 20.02.2021 \n
Author  : Мурзина Л.Х  \n
Company : АО НПП АМЭ  \n
Comments: \n
\n
Chip type               : Milandr 1986VE92U \n
Program type            : Application \n
External Clock frequency: 16,000000 MHz \n
Data Stack size         : 1024 \n
\n
*******************************************************
*/
 
#include "main.h"
#include "ps2phy.h"
#include <math.h>
#include "uart.h"
#include "kbms.h"
#include "ps2protocol.h"
#include "config.h"
#include <string.h>
#include <ctype.h>

PORT_InitTypeDef PortInitStructure;

// Report Buffers
uint8_t InReport[HID_INPUT_REPORT_BYTES];
uint8_t OutReport[HID_OUTPUT_REPORT_BYTES]={0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00};			
uint8_t FeatureReport[HID_FEATURE_REPORT_BYTES];		
uint8_t InReport2[HID_INPUT_REPORT_BYTES];
uint8_t OutReport2[HID_OUTPUT_REPORT_BYTES]={0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00};			
uint8_t FeatureReport2[HID_FEATURE_REPORT_BYTES];

volatile uint8_t RequestDataID;
U8 StateLEDs;
uint8_t first_start=1;
uint8_t EndPointMS_IN = 0x83;
uint8_t EndPointKB_IN = 0x81;

void GetInReport(void) 
{
	// ����� ������� � �������� Input Report
	// InReport[0]++;
	//USB_WriteEP(0x80,OutReport,8);
}

void SetOutReport(void) 
{
	// ����� �������� Output Report
	// =OutReport[0];
}

// ����������� ������ � ���������� ID
void SetFeatureReport(void) 
{
	// ����� �������� Feature Report
	// =FeatureReport[0];
}

void GetFeatureReport(void) 
{
	// ����� ������� � �������� Input Report
	// FeatureReport[0]++;
}

void IWDG_ReloadCounter(void)
{
  MDR_IWDG->KR = KR_KEY_Reload;
}

void GetStateLedsKeyboard(void)
{		
		if(OutReport[0] & (1<<0)) MDR_PORTA->RXTX |= (1<<0);	
		else MDR_PORTA->RXTX &= ~(1<<0);
		if(OutReport[0] & (1<<1)) MDR_PORTA->RXTX |= (1<<1);	
		else MDR_PORTA->RXTX &= ~(1<<1);
		if(OutReport[0] & (1<<2)) MDR_PORTA->RXTX |= (1<<2);	
		else MDR_PORTA->RXTX &= ~(1<<2);	
}

void Clock_Configure(void)
{
	RST_CLK_HSEconfig(RST_CLK_HSE_ON);
  if (RST_CLK_HSEstatus() != SUCCESS)
  {
    while(1) ;
  }

  // CPU_C1_SEL = HSE
  RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul3);
  RST_CLK_CPU_PLLcmd(ENABLE);
  if (RST_CLK_CPU_PLLstatus() != SUCCESS)
  {
    while(1) ;
  }

  // CPU_C3_SEL = CPU_C2_SEL
  RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
  // CPU_C2_SEL = PLL
  RST_CLK_CPU_PLLuse(ENABLE);
  // HCLK_SEL = CPU_C3_SEL
  RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
	// Get Core Clock Frequency	
	SystemCoreClockUpdate();  
	
	RST_CLK_PCLKcmd(ALL_PORTS_CLK, ENABLE);
	
	RST_CLK_PCLKcmd(RST_CLK_PCLK_IWDG,ENABLE);
	
	SysTickStart(48000);
}


void IO_Configure (void)
{
	PORT_InitTypeDef PORT_InitStructure;	
	PORT_StructInit(&PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_1 | PORT_Pin_3);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;	
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init(MDR_PORTE, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2;
  PORT_InitStructure.PORT_OE    = PORT_OE_OUT;	
	PORT_Init(MDR_PORTA, &PORT_InitStructure);

	/*PS/2 для клавиатуры*/
	/*PB4 - CLOCK; PB3 - DATA*/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_4);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_OVERRID;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_OFF;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
	PORT_InitStructure.PORT_GFEN = PORT_GFEN_ON;	
	PORT_Init(MDR_PORTB, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_3);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_OFF;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;	
	PORT_Init(MDR_PORTB, &PORT_InitStructure);
	
	/*конфигурационные вводы*/
	PORT_InitStructure.PORT_Pin = PORT_Pin_0 | PORT_Pin_1;
	PORT_InitStructure.PORT_OE    = PORT_OE_IN;
	PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;	
	PORT_Init(MDR_PORTF, &PORT_InitStructure);
	
}

void InitWdt(void)
{
	
	MDR_IWDG->KR = KR_KEY_Enable;

	MDR_IWDG->KR = KR_KEY_WriteEnable;

	MDR_IWDG->PR = IWDG_Prescaler_128;

	while(MDR_IWDG->SR & IWDG_SR_PVU){;}

	MDR_IWDG->RLR = 0xc35;																			//хотелось 10сек., по факту получилось ~14сек
	
}

void SetConfigUSBDevice(void)
{
	U8 size_dev=0, size_conf=0, size_string=0;
	
	InitHIDev=ScanPortConfiguration();
	
	size_dev = USB_Descriptors[InitHIDev].sizes_desc.size_device;
	size_conf = USB_Descriptors[InitHIDev].sizes_desc.size_conf;
	size_string = USB_Descriptors[InitHIDev].sizes_desc.size_string;
	
	memcpy(USB_DeviceDescriptor,USB_Descriptors[InitHIDev].DeviceDescriptor,size_dev);
	memcpy(USB_ConfigDescriptor,USB_Descriptors[InitHIDev].ConfigDescriptor,size_conf);
	memcpy(USB_StringDescriptor,USB_Descriptors[InitHIDev].StringDescriptor,size_string);
	EndPointKB_IN = USB_Descriptors[InitHIDev].endpoints.Endpoint_KB;
	EndPointMS_IN = USB_Descriptors[InitHIDev].endpoints.Endpoint_MS;
	
}

int main(void)
{
	Clock_Configure();
	IO_Configure();
	
	SetConfigUSBDevice();
	
	USB_Init();
	USB_Connect(__TRUE);
	
	if((InitHIDev==USE_KEYBOARD) || (InitHIDev==USE_BOTH))
	TIMER3_ETR_Setup();
	
//	InitUart1();
	
  keyboardHID.modifiers = 0;
  keyboardHID.key1 = 0;
  keyboardHID.key2 = 0;
  keyboardHID.key3 = 0;
	
  mouseHID.btn=0;
	mouseHID.X=0;
	mouseHID.Y=0;
	mouseHID.scroll=0;
	
	QueCreation(&QueueIn);
	QueCreation(&record_key);
	
	InitWdt();																														//настройка watchdog 10с (по факту вышло ~14с)
	
	if((InitHIDev==USE_MOUSE) || (InitHIDev==USE_BOTH))
	Mouse_Init();
	
	while(1)
	{
			Mouse_GetData();
			if(mouse_status==MOUSE_NEW_DATA)
			{
				mouse_status = MOUSE_INAKTIV;
				USB_WriteEP(EndPointMS_IN, (U8*)&mouseHID, sizeof(struct mouseHID_t));
				memset(&mouseHID,0x00,sizeof(mouseHID));
				_delay_ms(10);
			}
			matrix_scan();
//			scan_code();
			if(kboard_status==KBOARD_NEW_DATA)
//			if(KeyRecord.event.pressed)
			{
				{
					{
						keyboardHID.modifiers=It_Is_Modifier;
						keyboardHID.key1 = KB_data; //KeyRecord.event.code;//
						USB_WriteEP(EndPointKB_IN,(U8*)&keyboardHID,sizeof(struct keyboardHID_t));
						_delay_ms(10);
						
						keyboardHID.modifiers = 0;
						keyboardHID.key1 = 0;
						USB_WriteEP(EndPointKB_IN, (U8*)&keyboardHID,sizeof(struct keyboardHID_t));			
						Flag=0;BufferReceive=0;
					}
				}
			}
			
//			if(kboard_status==KBOARD_UPDATE_DATA)
//			{
//					keyboardHID.modifiers=It_Is_Modifier;
//					keyboardHID.key1 = KeyRecord.event.code;
//					USB_WriteEP(0x83,(U8*)&keyboardHID,sizeof(struct keyboardHID_t));
//					_delay_ms(10);				
//			}
			GetStateLedsKeyboard();
			
			IWDG_ReloadCounter();
	}
}	


