/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbdesc.c
 * Purpose: USB Descriptors
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Changed string descriptor handling
 *                Changed HID Report Descriptor
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include "type.h"

#include "usb.h"
#include "usbcfg_MDR32F9x.h"
#include "usbdesc.h"
#include "hid.h"

int lenght_conf=0;

U8 InitHIDev=USE_BOTH;

/*------------------------------------------------------------------------------
  HID Report Descriptor
 *------------------------------------------------------------------------------*/

const U8 HID_ReportDescriptor[] = 
	{
0x05, 0x01,	//USAGE_PAGE (Generic Desktop)	
0x09, 0x06,	//USAGE (Keyboard)	
0xA1, 0x01, //COLLECTION (Application)	
//0x85, 0x01, //Report ID (1)
0x05, 0x07,	//USAGE_PAGE (Keyboard)	
0x19, 0xE0,	//USAGE_MINIMUM (Keyboard LeftControl)	
0x29, 0xE7,	//USAGE_MAXIMUM (Keyboard Right GUI)	
0x15, 0x00,	//LOGICAL_MINIMUM (0)	
0x25, 0x01, //LOGICAL_MAXIMUM (1)	
0x75, 0x01, //REPORT_SIZE (1)	
0x95, 0x08, //REPORT_COUNT (8)	
0x81, 0x02, //INPUT (Data,Var,Abs)	
0x95, 0x01, //REPORT_COUNT (1)	
0x75, 0x08, //REPORT_SIZE (8)	
0x81, 0x03, //INPUT (Cnst,Var,Abs)	
0x95, 0x05, //REPORT_COUNT (5)	
0x75, 0x01, //REPORT_SIZE (1)	
0x05, 0x08,	//USAGE_PAGE (LEDs)	
0x19, 0x01,	//USAGE_MINIMUM (Num Lock)	
0x29, 0x05,	//USAGE_MAXIMUM (Kana)	
0x91, 0x02,	//OUTPUT (Data,Var,Abs)	
0x95, 0x01, //REPORT_COUNT (1)	
0x75, 0x03, //REPORT_SIZE (3)	
0x91, 0x03, //OUTPUT (Cnst,Var,Abs)	
0x95, 0x06, //REPORT_COUNT (6)	
0x75, 0x08, //REPORT_SIZE (8)	
0x15, 0x00, //LOGICAL_MINIMUM (0)	
0x25, 0x65, //LOGICAL_MAXIMUM (101)	
0x05, 0x07,	//USAGE_PAGE (Keyboard)	
0x19, 0x00,	//USAGE_MINIMUM (Reserved (no event indicated))	
0x29, 0x65,	//USAGE_MAXIMUM (Keyboard Application)	
0x81, 0x00,	//INPUT (Data,Ary,Abs)	
0xC0,				//END_COLLECTION
};

const U8 HID_ReportDescriptorM[] = 
{
0x05, 0x01,  				//Usage Page (Generic Desktop)
0x09, 0x02,  				//Usage (Mouse)
0xA1, 0x01,					//Collection (Application)  
0x05, 0x09,  				//Usage Page (Button)
0x19, 0x01,  				//Usage Minimum (Button 1)
0x29, 0x03,  				//Usage Maximum (Button 5)
0x15, 0x00,					//Logical Minimum (0)  
0x25, 0x01,  				//Logical Maximum (1)
0x95, 0x03,  				//Report Count (5)
0x75, 0x01,  				//Report Size (1)
0x81, 0x02,  				//Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
0x95, 0x01,  				//Report Count (1)
0x75, 0x05,  				//Report Size (3)
0x81, 0x03,  				//Input (Cnst,Ary,Abs)
0x05, 0x01,  				//Usage Page (Generic Desktop)
0x09, 0x01,  				//Usage (Pointer)
0xA1, 0x00,  				//Collection (Physical)
0x09, 0x30, 				//Usage (X) 
0x09, 0x31, 				//Usage (Y) 
0x15, 0x81,  				//Logical Minimum (-127)
0x25, 0x7F,  				//Logical Maximum (127)
0x75, 0x08,  				//Report Size (8)
0x95, 0x02,  				//Report Count (2)
0x81, 0x06,  				//Input (Data,Var,Rel,NWrp,Lin,Pref,NNul,Bit)
0xC0,  							//End Collection
0x09, 0x38,  				//Usage (Wheel)
0x15, 0x81,  				//Logical Minimum (-127)
0x25, 0x7F,  				//Logical Maximum (127)
0x75, 0x08,  				//Report Size (8)
0x95, 0x01,  				//Report Count (1)
0x81, 0x06,  				//Input (Data,Var,Rel,NWrp,Lin,Pref,NNul,Bit)
0xC0 								//End Collection
};


const U16 HID_ReportDescSize = sizeof(HID_ReportDescriptor);
const U16 HID_ReportDescSizeM = sizeof(HID_ReportDescriptorM);

U8 USB_DeviceDescriptor[18] = {0};
//{
//  USB_DEVICE_DESC_SIZE,                     /* bLength */
//  USB_DEVICE_DESCRIPTOR_TYPE,  							/* bDescriptorType */
//  WBVAL(0x0200), /* 2.0 */                  /* bcdUSB */
//  0xef,                       							/* bDeviceClass */
//  0x02,                       							/* bDeviceSubClass */
//  0x01,                       							/* bDeviceProtocol */
//  0x08,           													/* bMaxPacketSize */
//  WBVAL(0x0483),           									/* idVendor */
//  WBVAL(0x5741),           									/* idVendor */		
//  WBVAL(0x0100),                       			/* bcdDevice rel. 1.00 */
//  0x01,           													/* Index of manufacturer string */
//  0x02,       															/* Index of product string */
//  0x00,        															/* Index of serial number string */
//  0x01  																		/* bNumConfigurations */
//};

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor) */
U8 USB_ConfigDescriptor[68] = {0};
//{
///* Configuration 1 */
//  USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
//  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
//  WBVAL(                             /* wTotalLength */
//    (USB_CONFIGUARTION_DESC_SIZE +
//    USB_INTERFACE_DESC_SIZE     +
//    HID_DESC_SIZE               +
//    USB_ENDPOINT_DESC_SIZE			+
//		USB_ENDPOINT_DESC_SIZE			+
//    USB_INTERFACE_DESC_SIZE     +
//    HID_DESC_SIZE               +
//    USB_ENDPOINT_DESC_SIZE	)
//  ),
//  0x02,                              /* bNumInterfaces */
//  0x01,                              /* bConfigurationValue: 0x01 is used to select this configuration */
//  0x00,                              /* iConfiguration: no string to describe this configuration */
//  USB_CONFIG_SELF_POWERED /*|*/       /* bmAttributes */
///*USB_CONFIG_REMOTE_WAKEUP*/,
//  USB_CONFIG_POWER_MA(500),          /* bMaxPower, device power consumption is 100 mA */
//	
///* Interface 0, Alternate Setting 0, HID Class */
//  USB_INTERFACE_DESC_SIZE,           /* bLength */
//  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
//  0x00,                              /* bInterfaceNumber */
//  0x00,                              /* bAlternateSetting */
//  0x01,                              /* bNumEndpoints */
//  USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
//  HID_SUBCLASS_BOOT,                 /* bInterfaceSubClass */
//  HID_PROTOCOL_KEYBOARD,             /* bInterfaceProtocol */
//  0x04,                              /* iInterface */
//	
///* HID Class Descriptor */
//  HID_DESC_SIZE,                     /* bLength */
//  HID_HID_DESCRIPTOR_TYPE,           /* bDescriptorType */
//  WBVAL(0x0110), /* 1.10 */           /* bcdHID */
//  0x00,                              /* bCountryCode */
//  0x01,                              /* bNumDescriptors */
//  HID_REPORT_DESCRIPTOR_TYPE,        /* bDescriptorType */
//  WBVAL(HID_REPORT_DESC_SIZE),       /* wDescriptorLength */

///* Endpoint, HID Interrupt In */
//  USB_ENDPOINT_DESC_SIZE,            /* bLength */
//  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
//  USB_ENDPOINT_IN(1),                /* bEndpointAddress */
//  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
//  WBVAL(8),   											 /* wMaxPacketSize */
//  1,           				           		 /* bInterval = 1, 2, 4, 8, 16, 32 */	
//	
///* Endpoint, HID Interrupt Out */
//  USB_ENDPOINT_DESC_SIZE,            /* bLength */
//  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
//  USB_ENDPOINT_OUT(2),                /* bEndpointAddress */
//  USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
//  WBVAL(8),   											 /* wMaxPacketSize */
//  8,           				           		 /* bInterval = 1, 2, 4, 8, 16, 32 */		

///* Interface 1, Alternate Setting 0, HID Class */
//  USB_INTERFACE_DESC_SIZE,           /* bLength */
//  USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
//  0x01,                              /* bInterfaceNumber */
//  0x00,                              /* bAlternateSetting */
//  0x01,                              /* bNumEndpoints */
//  USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
//  HID_SUBCLASS_BOOT,                 /* bInterfaceSubClass */
//  HID_PROTOCOL_MOUSE,                /* bInterfaceProtocol */
//  0x00,                              /* iInterface */
//	
///* HID Class Descriptor */
//  HID_DESC_SIZE,                     					/* bLength */
//  HID_HID_DESCRIPTOR_TYPE,           					/* bDescriptorType */
//  WBVAL(0x0110), /* 1.10 */           					/* bcdHID */
//  0x00,                              					/* bCountryCode */
//  0x01,                              					/* bNumDescriptors */
//  HID_REPORT_DESCRIPTOR_TYPE,        				  /* bDescriptorType */
//  WBVAL(HID_REPORTM_DESC_SIZE),       /* wDescriptorLength */

///* Endpoint, HID Interrupt In */
//  USB_ENDPOINT_DESC_SIZE,            /* bLength */
//  USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
//  USB_ENDPOINT_IN(3),                /* bEndpointAddress */
//  USB_ENDPOINT_TYPE_INTERRUPT,			 /* bmAttributes */
//  WBVAL(8),   											 /* wMaxPacketSize */
//  4,           				           /* bInterval = 1, 2, 4, 8, 16, 32 */	
//};


/* USB String Descriptor (optional) */
U8 USB_StringDescriptor[90] = {0};
//{
///* Index 0x00: LANGID Codes */
//  0x04,                              /* bLength */
//  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
//  WBVAL(0x0409), /* US English */    /* wLANGID */
///* Index 0x01: Manufacturer */
//  (8*2 + 2),                        /* bLength (8 Char + Type + lenght) */
//  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
//  '-',0,
//  'N',0,
//  'P',0,
//  'P',0,
//  'A',0,
//  'M',0,
//  'E',0,
//  '-',0,
///* Index 0x02: Product */
//  (16*2 + 2),                        /* bLength (16 Char + Type + lenght) */
//  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
//  '-',0,
//  'K',0,
//  'e',0,
//  'y',0,
//  'b',0,
//  'o',0,
//  'a',0,
//  'r',0,
//  'd',0,
//  ' ',0,
//  'M',0,
//  'o',0,
//  'u',0,
//  's',0,
//  'e',0,
//  '-',0,
///* Index 0x03: Serial Number */
//  (12*2 + 2),                        /* bLength (12 Char + Type + lenght) */
//  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
//  '0',0,                             /* allowed characters are       */
//  '0',0,                             /*   0x0030 - 0x0039 ('0'..'9') */
//  '0',0,                             /*   0x0041 - 0x0046 ('A'..'F') */
//  '1',0,                             /*   length >= 26               */
//  'A',0,
//  '0',0,
//  '0',0,
//  '0',0,
//  '0',0,
//  '0',0,
//  '0',0,
//  '0',0,
///* Index 0x04: Interface 0, Alternate Setting 0 */
//  ( 3*2 + 2),                        /* bLength (3 Char + Type + lenght) */
//  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
//  'H',0,
//  'I',0,
//  'D',0,
//};
