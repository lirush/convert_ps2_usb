#ifndef _CONFIG_H
#define _CONFIG_H

#include "type.h"

#define QUANTITYPORT_CONF		(2)

#define SIZE_DEVICE_DESC_DEFAULT		(18)
#define SIZE_CONF_DESC_DEFAULT			(68)
#define SIZE_STRING_DESC_DEFAULT		(90)

#define SIZE_DEVICE_DESC_KB					(18)
#define SIZE_CONF_DESC_KB						(41)
#define SIZE_STRING_DESC_KB					(74)

#define SIZE_DEVICE_DESC_MS					(18)
#define SIZE_CONF_DESC_MS						(34)
#define SIZE_STRING_DESC_MS					(68)

#define ENDPOINT_IN_KB_SINGLE				(0x83)
#define ENDPOINT_IN_MS_SINGLE				(0x83)
#define ENDPOINT_IN_KB_COMPOSITE		(0x81)
#define ENDPOINT_IN_MS_COMPOSITE		(0x83)

struct Sizes{
	U8 size_device;
	U8 size_conf;
	U8 size_string;	
};

struct Endpoints{
	U8 Endpoint_KB;
	U8 Endpoint_MS;
};

typedef struct{
	U8 DeviceDescriptor[SIZE_DEVICE_DESC_DEFAULT];
	U8 ConfigDescriptor[SIZE_CONF_DESC_DEFAULT];
	U8 StringDescriptor[SIZE_STRING_DESC_DEFAULT];
	struct Sizes sizes_desc;
	struct Endpoints endpoints;
}ComConfig;
extern const ComConfig USB_Descriptors[3];

U8 ScanPortConfiguration(void);

#endif
