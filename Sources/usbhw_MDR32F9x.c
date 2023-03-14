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

#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"

#include "type.h"

#include "usb.h"
#include "usbcfg_MDR32F9x.h"
#include "usbreg_MDR32F9x.h"
#include "usbhw_MDR32F9x.h"
#include "MDR32F9Qx_usb_handlers.h"
#include "MDR32F9Qx_usb_device.h"
#include "MDR32F9Qx_rst_clk.h"
#include "usbcore.h"
#include "usbuser.h"

U16 FreeBufAddr; // Endpoint Free Buffer Address
USB_Clock_TypeDef USB_Clock_InitStruct;
USB_DeviceBUSParam_TypeDef USB_DeviceBUSParam;

static void USB_IntrEna(void) 
{
  NVIC_EnableIRQ(USB_IRQn);
}

void USB_Init(void) 
{
	// Enables the CPU_CLK clock on USB
  RST_CLK_PCLKcmd(RST_CLK_PCLK_USB, ENABLE);

  // Device layer initialization
  USB_Clock_InitStruct.USB_USBC1_Source = USB_C1HSEdiv2;		//USB_C1HSEdiv1
  USB_Clock_InitStruct.USB_PLLUSBMUL    = USB_PLLUSBMUL6;

  USB_DeviceBUSParam.MODE  = USB_SC_SCFSP_Low;
  USB_DeviceBUSParam.SPEED = USB_SC_SCFSR_1_5Mb; 
  USB_DeviceBUSParam.PULL  = USB_HSCR_DM_PULLUP_Set;

	
//	USB_DeviceBUSParam.MODE  = USB_SC_SCFSP_Full;
//  USB_DeviceBUSParam.SPEED = USB_SC_SCFSR_12Mb;
//  USB_DeviceBUSParam.PULL  = USB_HSCR_DP_PULLUP_Set;
	
  USB_DeviceInit(&USB_Clock_InitStruct, &USB_DeviceBUSParam);

  // Enable all USB interrupts
  USB_SetSIM(USB_SIS_Msk);

  //USB_DevicePowerOn();
	USB_IntrEna();
	USB_DEVICE_HANDLE_RESET;
}

void USB_Connect(BOOL con)
{
	 if(con) 
	 {
			USB_DevicePowerOn(); 
			USB_IntrEna();
			USB_DEVICE_HANDLE_RESET;
	 }
	 else 
	 {
			USB_DevicePowerOff();
	 }
}

/*
 *  USB Reset Function
 *   Called automatically on USB Reset
 *    Return Value:    None
 */
void USB_ResetIRQ(void)
{
	// Double Buffering is not yet supported 
	MDR_USB->SIS=0x1F; //Clear Interrupt Status
	MDR_USB->SIM=RESET_IE | TDONE_IE | (USB_SOF_EVENT ? SOF_IE : 0); //Enable interrupts
	//Setup Control Endpoint 0
	MDR_USB->USB_SEP_FIFO[0].RXFC=1; //Clear RX FIFO EndPoint 0
	MDR_USB->USB_SEP_FIFO[0].TXFDC=1;	//Clear TX FIFO EndPoint 0
	MDR_USB->USB_SEP[0].CTRL=EP_EN | EP_RDY; //Enable Endpoint 0, go to endpoint redy state
	
	USB_DeviceContext.Address = 0;
	USB_SetSA(USB_DeviceContext.Address);
}

/*
 *  USB Suspend Function
 *   Called automatically on USB Suspend
 *    Return Value:    None
 */
void USB_Suspend(void) 
{
}


/*
 *  USB Resume Function
 *   Called automatically on USB Resume
 *    Return Value:    None
 */
void USB_Resume(void) 
{
}

/*
 *  USB Remote Wakeup Function
 *   Called automatically on USB Remote Wakeup
 *    Return Value:    None
 */
void USB_WakeUp (void) 
{
  // Clear Suspend
}

/*
 *  USB Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Enable/Disable
 *    Return Value:    None
 */

void USB_WakeUpCfg(BOOL cfg) 
{
  // Not needed
}

/*
 *  USB Set Address Function
 *    Parameters:      adr:   USB Address
 *    Return Value:    None
 */
void USB_SetAddress (U32 adr) 
{
  MDR_USB->SA=adr;
	MDR_USB->USB_SEP[0].CTRL |=EP_RDY; // EndPoint0 ready again
}

/*
 *  USB Configure Function
 *    Parameters:      cfg:   Configure/Deconfigure
 *    Return Value:    None
 */
void USB_Configure(BOOL cfg)
{
}

/*
 *  Configure USB Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Endpoint Descriptor
 *    Return Value:    None
 */
void USB_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD) 
{
	// Double Buffering is not yet supported
  //U32 num;
	//num = pEPD->bEndpointAddress & 0x0F;
}

/*
 *  Set Direction for USB Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void USB_DirCtrlEP (U32 dir) 
{
}

/*
 *  Enable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_EnableEP (U32 EPNum) 
{
	U8 num;
	
	num = EPNum & 0x0F;																		//Get EndPoint's number
	MDR_USB->USB_SEP_FIFO[num].RXFC=1;									//Clear RX FIFO EndPoint
	MDR_USB->USB_SEP_FIFO[num].TXFDC=1;									//Clear TX FIFO EndPoint
	MDR_USB->USB_SEP[num].CTRL = EP_EN | EP_RDY;					//Enable Endpoint, go to endpoint redy state 
}


/*
 *  Disable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_DisableEP (U32 EPNum) 
{
	U8 num;
	
	num=EPNum & 0x0F;																		//Get EndPoint's number
  MDR_USB->USB_SEP[num].CTRL &=~EP_EN;								//Disable EndPoint
}


/*
 *  Reset USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_ResetEP (U32 EPNum) 
{
  USB_EP_Reset((USB_EP_TypeDef)EPNum);
}

/*
 *  Set Stall for USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_SetStallEP (U32 EPNum) 
{
	//__NOP();
	USB_EP_Stall((USB_EP_TypeDef)EPNum, USB_STALL_HALT);
}


/*
 *  Clear Stall for USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */
void USB_ClrStallEP (U32 EPNum) 
{
}


/*
 *  Clear USB Endpoint Buffer
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USB_ClearEPBuf (U32 EPNum) 
{
}


/*
 *  Read USB Endpoint Data
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *    Return Value:    Number of bytes read
 */
u8 Temp=0;
U32 USB_ReadEP (U32 EPNum, U8 *pData) 
{
	// Double Buffering is not yet supported
  U8 num;
	U32 cnt, n;

  num = EPNum & 0x0F;
//	if (MDR_USB->USB_SEP[num].TS==0)
//		data1 = (1<<num);
//	if (data1 &(1<<num))
//	{
//		MDR_USB->USB_SEP[num].CTRL |=DATA1_EN;
//		data1 = (0<<num);
//	}
//	else
//	{
//		MDR_USB->USB_SEP[num].CTRL &=~DATA1_EN;
//		data1 |= (1<<num);
//	}
	cnt=MDR_USB->USB_SEP_FIFO[num].RXFDC_H;							//Get number of bytes in FIFO

	for (n=0; n<cnt; n++)
	{
		pData[n]=MDR_USB->USB_SEP_FIFO[num].RXFD;					//Get data form USB RX FIFO
		Temp = pData[n];
	}
	MDR_USB->USB_SEP_FIFO[num].RXFC=1;									//Clear EndPoint's RX FIFO 
	//MDR_USB->USB_SEP[num].CTRL |=EP_RDY;							//EndPoint ready again

	return (cnt);
}

/*
 *  Write USB Endpoint Data
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *                     cnt:   Number of bytes to write
 *    Return Value:    Number of bytes written
 */

U32 USB_WriteEP (U32 EPNum, U8 *pData, U32 cnt) 
{
// Double Buffering is not yet supported
  U32 num, n;
	static unsigned char data1 = 0;

	num = EPNum & 0x0F;

	if(num == 0)
	{
		if (MDR_USB->USB_SEP[num].TS==0)
			data1 |= (1<<num);
	}
	
	if (data1 &(1<<num))
	{
		MDR_USB->USB_SEP[num].CTRL |=DATA1_EN;
		data1 &= ~(1<<num);
	}
	else
	{
		MDR_USB->USB_SEP[num].CTRL &=~DATA1_EN;
		data1 |=(1<<num);
	}
	
	MDR_USB->USB_SEP_FIFO[num].TXFDC=1;									//Clear EndPoint's TX FIFO 
	
	for (n=0; n<cnt; n++)
	{
		MDR_USB->USB_SEP_FIFO[num].TXFD=pData[n];
	}
	
	MDR_USB->USB_SEP[num].CTRL |=EP_RDY;								//EndPoint ready again
	return(cnt);
}

// USB Interrupt Service Routine
void USB_IRQHandler (void)
{
	unsigned int num, val;
	//USB reset request
	if (MDR_USB->SIS &RESET_EV)
	{
		USB_ResetIRQ();
#if USB_RESET_EVENT
		USB_Reset_Event();
#endif
		MDR_USB->SIS |=RESET_EV;
	}
#if USB_SOF_EVENT
	//Start of frame
	if (MDR_USB->SIS &SOF_EV)
	{
    USB_SOF_Event();		
		MDR_USB->SIS |=SOF_EV;
	}
#endif
	//Endpoint interrupts
	if (MDR_USB->SIS &TDONE_EV)
	{
		MDR_USB->SIS |=TDONE_EV;
		for (num=0; num<USB_EP_NUM; num++)							//Check data in EndPoints
		{
			if ((MDR_USB->USB_SEP[num].CTRL &EP_EN)&&(!(MDR_USB->USB_SEP[num].CTRL &EP_RDY)))
				//If transaction complited, enabled EndPoint is not ready
				break;
		}
#if USB_ERROR_EVENT
		while (MDR_USB->USB_SEP[num].STS &0x0F)
		{
			//if error ocured
			if (MDR_USB->USB_SEP[num].STS &RXO_ERR)
			{
				USB_Error_Event(1);
				MDR_USB->USB_SEP[num].STS &= ~RXO_ERR;
			}
			else
			{
				USB_Error_Event(0);
				MDR_USB->USB_SEP[num].STS &= ~0x0F;
			}
		}
#endif
		val=MDR_USB->USB_SEP[num].TS;										//Check type of transaction
		if (val==SETUP)
			USB_P_EP[num](USB_EVT_SETUP);									//Setup transaction
		if (val==RESAVE)
		{
			USB_P_EP[num](USB_EVT_OUT);										//Resave bytes
		}
		if (val==TRANSMIT)
			USB_P_EP[num](USB_EVT_IN);										//Transmit bytes
	}
}
