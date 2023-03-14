/*----------------------------------------------------------------------------
 *      PSTU MDR32F9Q2I Libs
 *----------------------------------------------------------------------------
 *      Name:    USBREG_MDR32F9x.h
 *      Purpose: USB Hardware Layer Definitions for Milandr MDR32F9x
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *      Author:	 			A. Sharonov
 *      Created:			21.03.2014
 *			Last modifed:	20.04.2014
 *---------------------------------------------------------------------------*/
 
#ifndef __USBREG_MDR32F9X_H
#define __USBREG_MDR32F9X_H

//Event flags
#define TDONE_EV	0x01							//Transfer done event flag
#define RESUME_EV	0x02							//Resume event flag
#define RESET_EV	0x04							//Reset event flag
#define SOF_EV		0x08							//SOF event flag
#define NAK_EV		0x10							//NAK event flag

//Bits SIM register
#define TDONE_IE	0x01							//Transfer done interrupt enable
#define RESUME_IE	0x02							//Resume interrupt enable
#define RESET_IE	0x04							//Reset interrupt enable
#define SOF_IE		0x08							//SOF interrupt enable
#define NAK_IE		0x10							//NAK send interrrupt enable

//Bit Endpoint control register
#define EP_EN			0x01							//Endpoint enable
#define EP_RDY		0x02							//Endpoint ready
#define DATA1_EN	0x04							//DATA1 host answer enable
#define STALL_EN	0x08							//STALL answer enable
#define EPISO_EN	0x10							//Isohor mode enable

//Error bits
#define CRC_ERR		0x01							//CRC error
#define STUFF_ERR	0x02							//STUFF error
#define RXO_ERR		0x04							//RX overrun error
#define TO_ERR		0x08							//RX timeout error

//Type of transaction
#define SETUP			0									//Setup transaction
#define TRANSMIT	1									//Transmit of bytes
#define RESAVE		2									//Resave of bytes

#endif // __USBREG_MDR32F9X_H
