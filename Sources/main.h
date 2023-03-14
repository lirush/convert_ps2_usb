#ifndef _MAIN_H_
#define _MAIN_H_

#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_rst_clk.h"
#include "systick.h"

#include "usbhw_MDR32F9x.h"
#include "usbdesc.h"
#include "hid.h"

#include <stdio.h>

#include "queue.h"

#define ALL_PORTS_CLK   (RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD | RST_CLK_PCLK_PORTF | RST_CLK_PCLK_PORTE)

/******настройка WDT********/
#define KR_KEY_Reload       				((uint32_t)0xAAAA)
#define KR_KEY_Enable       				((uint32_t)0xCCCC)
#define KR_KEY_WriteEnable  				((uint32_t)0x5555)
#define KR_KEY_WriteDisable 				((uint32_t)0x0000)

#define IWDG_Prescaler_4            ((uint32_t)0x00)
#define IWDG_Prescaler_8            ((uint32_t)0x01)
#define IWDG_Prescaler_16           ((uint32_t)0x02)
#define IWDG_Prescaler_32           ((uint32_t)0x03)
#define IWDG_Prescaler_64           ((uint32_t)0x04)
#define IWDG_Prescaler_128          ((uint32_t)0x05)
#define IWDG_Prescaler_256          ((uint32_t)0x06)

#define IWDG_SR_PVU                 ((uint32_t)0x00000001)
#define IWDG_SR_RVU                 ((uint32_t)0x00000002)
/**************************/

// Report Buffers
extern uint8_t InReport[HID_INPUT_REPORT_BYTES];
extern uint8_t OutReport[HID_OUTPUT_REPORT_BYTES];			
extern uint8_t FeatureReport[HID_FEATURE_REPORT_BYTES];	
extern uint8_t InReport2[HID_INPUT_REPORT_BYTES];
extern uint8_t OutReport2[HID_OUTPUT_REPORT_BYTES];			
extern uint8_t FeatureReport2[HID_FEATURE_REPORT_BYTES];	

extern void GetInReport(void);
extern void SetOutReport(void);
extern void SetFeatureReport(void);
extern void GetFeatureReport(void);

#endif // _MAIN_H_
