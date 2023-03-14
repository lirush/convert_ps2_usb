#include "ps2protocol.h"
#include "ps2phy.h"
#include <stdint.h>
#include "systick.h"
#include "queue.h"

#define WAIT(stat,us)	do{if(!Wait_##stat(us))goto ERR;}while(0)

uint8_t PS2_Host_Send(uint8_t data)
{
	uint8_t i=0;
	uint8_t parity=1;
	
	Mouse_PS2_PhyOFF();

	Clock_Lo();	
	Data_Hi();	
	_delay_us(150);
	Data_Lo();
	Clock_Hi();	

	WAIT(Mouse_Clock_Lo,10000);
	
	for(i=0;i<8;i++)
	{
		if (data&(1<<i)) {
				parity = !parity;
				Data_Hi();
		} else {
				Data_Lo();
		}
		WAIT(Mouse_Clock_Hi,100);
		WAIT(Mouse_Clock_Lo,100);
	}
	
	//бит четности
	if(parity){Data_Hi();}
	else{Data_Lo();}
	WAIT(Mouse_Clock_Hi,100);
	WAIT(Mouse_Clock_Lo,100);	

	//стоп бит
	Data_Hi();
	
	//ack
	WAIT(Mouse_Data_Lo,100);
	WAIT(Mouse_Clock_Lo,100);	
	
	/* wait for idle state */
	WAIT(Mouse_Clock_Hi,100);
	WAIT(Mouse_Data_Hi,100);		

		//idle state
	WAIT(Mouse_Clock_Hi,100);
	WAIT(Mouse_Data_Hi,100);		
		
	Mouse_PS2_PhyON();
	return 1;
		ERR :		
	return 0;
}

unsigned char PS2_Host_Receive(void)
{
//	while(BufferReceive_mb ==0x00);
	while(QueueIn.last==0);
	Flag_mb=0;
	QueDelete(&QueueIn);
	return BufferReceive_mb; 
}
