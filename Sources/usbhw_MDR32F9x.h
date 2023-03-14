/*----------------------------------------------------------------------------
 *      PSTU MDR32F9Q2I projects
 *----------------------------------------------------------------------------
 *      Name:    usbhw_MDR32F9x.c
 *      Purpose: USB Hardware Layer Module for Milandr MDR32F9x
 *      Rev.:    V1.10
 *----------------------------------------------------------------------------
 *      Author:	 			A. Sharonov
 *      Created:			23.02.2014
 *			Last modifed:	24.07.2015
 *---------------------------------------------------------------------------*/

#include "type.h"
#include "usb.h"

#ifndef __USBHW_MDR32F9X_H
#define __USBHW_MDR32F9X_H

extern void USB_Init (void);
extern void USB_Connect (BOOL con);

extern void USB_WakeUpCfg (BOOL cfg);
extern void USB_SetAddress (U32 adr);
extern void USB_Configure (BOOL cfg);
extern void USB_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD);
extern void USB_DirCtrlEP (U32 dir);
extern void USB_EnableEP (U32 EPNum);
extern void USB_DisableEP (U32 EPNum);
extern void USB_ResetEP (U32 EPNum);
extern void USB_SetStallEP (U32 EPNum);
extern void USB_ClrStallEP (U32 EPNum);
extern void USB_ClearEPBuf (U32 EPNum);
extern U32 USB_ReadEP (U32 EPNum, U8 *pData);
extern U32 USB_WriteEP (U32 EPNum, U8 *pData, U32 cnt);

#endif // __USBHW_MDR32F9X_H
