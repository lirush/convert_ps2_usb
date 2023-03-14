#include "config.h"
#include "usb.h"
#include "usbdesc.h"
#include "hid.h"
#include "MDR32F9Qx_port.h"

#define GET_SIZE_CONF_DESC	WBVAL(USB_CONFIGUARTION_DESC_SIZE + \
																			USB_INTERFACE_DESC_SIZE + \
																			HID_DESC_SIZE + \
																			USB_ENDPOINT_DESC_SIZE + \
																			USB_ENDPOINT_DESC_SIZE	+ \
																			USB_INTERFACE_DESC_SIZE + \
																			HID_DESC_SIZE + \
																			USB_ENDPOINT_DESC_SIZE)
																			
const U8 matrix_configs[4]={0,1,2,0};																			

const ComConfig USB_Descriptors[3] = {
	{
		//DeviceDescriptor
		{	USB_DEVICE_DESC_SIZE,
			USB_DEVICE_DESCRIPTOR_TYPE,
			WBVAL(0x0200),														//USB 2.0
			0xef,																			/* bDeviceClass */
			0x02,																			/* bDeviceSubClass */
			0x01,																			/* bDeviceProtocol */
			0x08,																			/* bMaxPacketSize */
			WBVAL(0x0483),														/* idVendor */
			WBVAL(0x5741),														/* idVendor */	
			WBVAL(0x0100),														/* bcdDevice rel. 1.00 */
			0x01,																			/* Index of manufacturer string */
			0x02,																			/* Index of product string */
			0x00,																			/* Index of serial number string */
			0x01 },																		/* bNumConfigurations */
		//ConfigDescriptor
		{ /* Configuration 1 */
			USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
			USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
			WBVAL(                             /* wTotalLength */
				(USB_CONFIGUARTION_DESC_SIZE +
				USB_INTERFACE_DESC_SIZE     +
				HID_DESC_SIZE               +
				USB_ENDPOINT_DESC_SIZE			+
				USB_ENDPOINT_DESC_SIZE			+
				USB_INTERFACE_DESC_SIZE     +
				HID_DESC_SIZE               +
				USB_ENDPOINT_DESC_SIZE	)
			),
			0x02,                              /* bNumInterfaces */
			0x01,                              /* bConfigurationValue: 0x01 is used to select this configuration */
			0x00,                              /* iConfiguration: no string to describe this configuration */
			USB_CONFIG_SELF_POWERED /*|*/       /* bmAttributes */
		/*USB_CONFIG_REMOTE_WAKEUP*/,
			USB_CONFIG_POWER_MA(500),          /* bMaxPower, device power consumption is 100 mA */
			
		/* Interface 0, Alternate Setting 0, HID Class */
			USB_INTERFACE_DESC_SIZE,           /* bLength */
			USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
			0x00,                              /* bInterfaceNumber */
			0x00,                              /* bAlternateSetting */
			0x01,                              /* bNumEndpoints */
			USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
			HID_SUBCLASS_BOOT,                 /* bInterfaceSubClass */
			HID_PROTOCOL_KEYBOARD,             /* bInterfaceProtocol */
			0x04,                              /* iInterface */
			
		/* HID Class Descriptor */
			HID_DESC_SIZE,                     /* bLength */
			HID_HID_DESCRIPTOR_TYPE,           /* bDescriptorType */
			WBVAL(0x0110), /* 1.10 */           /* bcdHID */
			0x00,                              /* bCountryCode */
			0x01,                              /* bNumDescriptors */
			HID_REPORT_DESCRIPTOR_TYPE,        /* bDescriptorType */
			WBVAL(63),       /* wDescriptorLength */

		/* Endpoint, HID Interrupt In */
			USB_ENDPOINT_DESC_SIZE,            /* bLength */
			USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
			USB_ENDPOINT_IN(ENDPOINT_IN_KB_COMPOSITE),                /* bEndpointAddress */
			USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
			WBVAL(8),   											 /* wMaxPacketSize */
			8,           				           		 /* bInterval = 1, 2, 4, 8, 16, 32 */	
			
		/* Endpoint, HID Interrupt Out */
			USB_ENDPOINT_DESC_SIZE,            /* bLength */
			USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
			USB_ENDPOINT_OUT(2),                /* bEndpointAddress */
			USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
			WBVAL(8),   											 /* wMaxPacketSize */
			8,           				           		 /* bInterval = 1, 2, 4, 8, 16, 32 */		

		/* Interface 1, Alternate Setting 0, HID Class */
			USB_INTERFACE_DESC_SIZE,           /* bLength */
			USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
			0x01,                              /* bInterfaceNumber */
			0x00,                              /* bAlternateSetting */
			0x01,                              /* bNumEndpoints */
			USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
			HID_SUBCLASS_BOOT,                 /* bInterfaceSubClass */
			HID_PROTOCOL_MOUSE,                /* bInterfaceProtocol */
			0x00,                              /* iInterface */
			
		/* HID Class Descriptor */
			HID_DESC_SIZE,                     												/* bLength */
			HID_HID_DESCRIPTOR_TYPE,           												/* bDescriptorType */
			WBVAL(0x0110), /* 1.10 */           											/* bcdHID */
			0x00,                              												/* bCountryCode */
			0x01,                              												/* bNumDescriptors */
			HID_REPORT_DESCRIPTOR_TYPE,        				  							/* bDescriptorType */
			WBVAL(52),       																					/* wDescriptorLength */

		/* Endpoint, HID Interrupt In */
			USB_ENDPOINT_DESC_SIZE,            												/* bLength */
			USB_ENDPOINT_DESCRIPTOR_TYPE,      												/* bDescriptorType */
			USB_ENDPOINT_IN(ENDPOINT_IN_MS_COMPOSITE),                /* bEndpointAddress */
			USB_ENDPOINT_TYPE_INTERRUPT,			 												/* bmAttributes */
			WBVAL(8),   											 												/* wMaxPacketSize */
			4,           				           														/* bInterval = 1, 2, 4, 8, 16, 32 */			
		},
		{
				/* Index 0x00: LANGID Codes */
			0x04,                              /* bLength */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			WBVAL(0x0409), /* US English */    /* wLANGID */
		/* Index 0x01: Manufacturer */
			(8*2 + 2),                        /* bLength (8 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'-',0,
			'N',0,
			'P',0,
			'P',0,
			'A',0,
			'M',0,
			'E',0,
			'-',0,
		/* Index 0x02: Product */
			(16*2 + 2),                        /* bLength (16 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'-',0,
			'K',0,
			'e',0,
			'y',0,
			'b',0,
			'o',0,
			'a',0,
			'r',0,
			'd',0,
			' ',0,
			'M',0,
			'o',0,
			'u',0,
			's',0,
			'e',0,
			'-',0,
		/* Index 0x03: Serial Number */
			(12*2 + 2),                        /* bLength (12 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'0',0,                             /* allowed characters are       */
			'0',0,                             /*   0x0030 - 0x0039 ('0'..'9') */
			'0',0,                             /*   0x0041 - 0x0046 ('A'..'F') */
			'1',0,                             /*   length >= 26               */
			'A',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
		/* Index 0x04: Interface 0, Alternate Setting 0 */
			( 3*2 + 2),                        /* bLength (3 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'H',0,
			'I',0,
			'D',0,},
			SIZE_DEVICE_DESC_DEFAULT,SIZE_CONF_DESC_DEFAULT,SIZE_STRING_DESC_DEFAULT,
			ENDPOINT_IN_KB_COMPOSITE,ENDPOINT_IN_MS_COMPOSITE
	},
	{
		//DeviceDescriptor
		{	USB_DEVICE_DESC_SIZE,
			USB_DEVICE_DESCRIPTOR_TYPE,
			WBVAL(0x0200),														//USB 2.0
			0x00,																			/* bDeviceClass */
			0x00,																			/* bDeviceSubClass */
			0x00,																			/* bDeviceProtocol */
			0x08,																			/* bMaxPacketSize */
			WBVAL(0x0458),														/* idVendor */
			WBVAL(0x003a),														/* idVendor */	
			WBVAL(0x0100),														/* bcdDevice rel. 1.00 */
			0x01,																			/* Index of manufacturer string */
			0x02,																			/* Index of product string */
			0x00,																			/* Index of serial number string */
			0x01 },																		/* bNumConfigurations */
		//ConfigDescriptor
		{ /* Configuration 1 */
			USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
			USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
			WBVAL(                             /* wTotalLength */
				(USB_CONFIGUARTION_DESC_SIZE +
				USB_INTERFACE_DESC_SIZE     +
				HID_DESC_SIZE               +
				USB_ENDPOINT_DESC_SIZE	)
			),
			0x01,                              /* bNumInterfaces */
			0x01,                              /* bConfigurationValue: 0x01 is used to select this configuration */
			0x00,                              /* iConfiguration: no string to describe this configuration */
			USB_CONFIG_BUS_POWERED | USB_CONFIG_SELF_POWERED /*|*/       /* bmAttributes */
		/*USB_CONFIG_REMOTE_WAKEUP*/,
			USB_CONFIG_POWER_MA(100),          /* bMaxPower, device power consumption is 100 mA */
			
		/* Interface 1, Alternate Setting 0, HID Class */
			USB_INTERFACE_DESC_SIZE,           /* bLength */
			USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
			0x00,                              /* bInterfaceNumber */
			0x00,                              /* bAlternateSetting */
			0x01,                              /* bNumEndpoints */
			USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
			HID_SUBCLASS_BOOT,                 /* bInterfaceSubClass */
			HID_PROTOCOL_MOUSE,                /* bInterfaceProtocol */
			0x04,                              /* iInterface */
			
		/* HID Class Descriptor */
			HID_DESC_SIZE,                     	/* bLength */
			HID_HID_DESCRIPTOR_TYPE,           	/* bDescriptorType */
			WBVAL(0x0111), /* 1.10 */           /* bcdHID */
			0x00,                              	/* bCountryCode */
			0x01,                              	/* bNumDescriptors */
			HID_REPORT_DESCRIPTOR_TYPE,        	/* bDescriptorType */
			WBVAL(62),       										/* wDescriptorLength */

		/* Endpoint, HID Interrupt In */
			USB_ENDPOINT_DESC_SIZE,            /* bLength */
			USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
			USB_ENDPOINT_IN(ENDPOINT_IN_MS_SINGLE),                /* bEndpointAddress */
			USB_ENDPOINT_TYPE_INTERRUPT,			 /* bmAttributes */
			WBVAL(8),   											 /* wMaxPacketSize */
			8,           				           /* bInterval = 1, 2, 4, 8, 16, 32 */						
		},
		{
			/* Index 0x00: LANGID Codes */
			0x04,                              /* bLength */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			WBVAL(0x0409), 										/* US English */    /* wLANGID */
		/* Index 0x01: Manufacturer */
			(8*2 + 2),                        /* bLength (8 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'-',0,
			'N',0,
			'P',0,
			'P',0,
			'A',0,
			'M',0,
			'E',0,
			'-',0,
		/* Index 0x02: Product */
			(5*2 + 2),                        /* bLength (16 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'M',0,
			'o',0,
			'u',0,
			's',0,
			'e',0,
		/* Index 0x03: Serial Number */
			(12*2 + 2),                        /* bLength (12 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'0',0,                             /* allowed characters are       */
			'0',0,                             /*   0x0030 - 0x0039 ('0'..'9') */
			'0',0,                             /*   0x0041 - 0x0046 ('A'..'F') */
			'1',0,                             /*   length >= 26               */
			'A',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
		/* Index 0x04: Interface 0, Alternate Setting 0 */
			( 3*2 + 2),                        /* bLength (3 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'H',0,
			'I',0,
			'D',0,			
		},
		SIZE_DEVICE_DESC_MS,SIZE_CONF_DESC_MS,SIZE_STRING_DESC_MS,
		ENDPOINT_IN_KB_COMPOSITE, ENDPOINT_IN_MS_SINGLE
	},	
	{
		//DeviceDescriptor
		{	USB_DEVICE_DESC_SIZE,
			USB_DEVICE_DESCRIPTOR_TYPE,
			WBVAL(0x0200),														//USB 2.0
			0x00,//0xef,																			/* bDeviceClass */
			0x00,//0x02,																			/* bDeviceSubClass */
			0x00,//0x01,																			/* bDeviceProtocol */
			0x08,																			/* bMaxPacketSize */
			WBVAL(0x03EB),														/* idVendor */
			WBVAL(0x3110),														/* idVendor */	
			WBVAL(0x0100),														/* bcdDevice rel. 1.00 */
			0x01,																			/* Index of manufacturer string */
			0x02,																			/* Index of product string */
			0x00,																			/* Index of serial number string */
			0x01 },																		/* bNumConfigurations */
		{
			/* Configuration 1 */
			USB_CONFIGUARTION_DESC_SIZE,       /* bLength */
			USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
			WBVAL(                             /* wTotalLength */
				(USB_CONFIGUARTION_DESC_SIZE +
				USB_INTERFACE_DESC_SIZE     +
				HID_DESC_SIZE               +
				USB_ENDPOINT_DESC_SIZE			+
				USB_ENDPOINT_DESC_SIZE)
			),
			0x01,                              /* bNumInterfaces */
			0x01,                              /* bConfigurationValue: 0x01 is used to select this configuration */
			0x00,                              /* iConfiguration: no string to describe this configuration */
			USB_CONFIG_BUS_POWERED | USB_CONFIG_SELF_POWERED /*|*/       /* bmAttributes */
		/*USB_CONFIG_REMOTE_WAKEUP*/,
			USB_CONFIG_POWER_MA(100),          /* bMaxPower, device power consumption is 100 mA */
			
		/* Interface 0, Alternate Setting 0, HID Class */
			USB_INTERFACE_DESC_SIZE,           /* bLength */
			USB_INTERFACE_DESCRIPTOR_TYPE,     /* bDescriptorType */
			0x00,                              /* bInterfaceNumber */
			0x00,                              /* bAlternateSetting */
			0x01,                              /* bNumEndpoints */
			USB_DEVICE_CLASS_HUMAN_INTERFACE,  /* bInterfaceClass */
			HID_SUBCLASS_BOOT,                 /* bInterfaceSubClass */
			HID_PROTOCOL_KEYBOARD,             /* bInterfaceProtocol */
			0x04,                              /* iInterface */
			
		/* HID Class Descriptor */
			HID_DESC_SIZE,                     /* bLength */
			HID_HID_DESCRIPTOR_TYPE,           /* bDescriptorType */
			WBVAL(0x0111), /* 1.10 */           /* bcdHID */
			0x00,                              /* bCountryCode */
			0x01,                              /* bNumDescriptors */
			HID_REPORT_DESCRIPTOR_TYPE,        /* bDescriptorType */
			WBVAL(63),       										/* wDescriptorLength */

		/* Endpoint, HID Interrupt In */
			USB_ENDPOINT_DESC_SIZE,            /* bLength */
			USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
			USB_ENDPOINT_IN(ENDPOINT_IN_KB_SINGLE),                /* bEndpointAddress */
			USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
			WBVAL(8),   											 /* wMaxPacketSize */
			8,           				           		 /* bInterval = 1, 2, 4, 8, 16, 32 */	
			
		/* Endpoint, HID Interrupt Out */
			USB_ENDPOINT_DESC_SIZE,            /* bLength */
			USB_ENDPOINT_DESCRIPTOR_TYPE,      /* bDescriptorType */
			USB_ENDPOINT_OUT(2),                /* bEndpointAddress */
			USB_ENDPOINT_TYPE_INTERRUPT,       /* bmAttributes */
			WBVAL(8),   											 /* wMaxPacketSize */
			8,           				           		 /* bInterval = 1, 2, 4, 8, 16, 32 */	
		},
		{
			/* Index 0x00: LANGID Codes */
			0x04,                              /* bLength */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			WBVAL(0x0409), 										/* US English */    /* wLANGID */
		/* Index 0x01: Manufacturer */
			(8*2 + 2),                        /* bLength (8 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'-',0,
			'N',0,
			'P',0,
			'P',0,
			'A',0,
			'M',0,
			'E',0,
			'-',0,
		/* Index 0x02: Product */
			(8*2 + 2),                        /* bLength (16 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'K',0,
			'e',0,
			'y',0,
			'b',0,
			'o',0,
			'a',0,
			'r',0,
			'd',0,
		/* Index 0x03: Serial Number */
			(12*2 + 2),                        /* bLength (12 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'0',0,                             /* allowed characters are       */
			'0',0,                             /*   0x0030 - 0x0039 ('0'..'9') */
			'0',0,                             /*   0x0041 - 0x0046 ('A'..'F') */
			'1',0,                             /*   length >= 26               */
			'A',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
			'0',0,
		/* Index 0x04: Interface 0, Alternate Setting 0 */
			( 3*2 + 2),                        /* bLength (3 Char + Type + lenght) */
			USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
			'H',0,
			'I',0,
			'D',0,			
		},
		SIZE_DEVICE_DESC_KB,SIZE_CONF_DESC_KB,SIZE_STRING_DESC_KB,
		ENDPOINT_IN_KB_SINGLE,ENDPOINT_IN_MS_COMPOSITE
	},
};

U8 ScanPortConfiguration(void)
{
	uint8_t tmp=0;
	int i=0;
	for(i=0;i<QUANTITYPORT_CONF;i++)
	{
		if(MDR_PORTF->RXTX & (1<<i))tmp |=(1<<i);
	}
	return matrix_configs[tmp];
}
