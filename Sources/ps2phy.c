/**
  ******************************************************************************
  * @file    ps2phy.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    16.12.2020
  * @brief   Содержит инициализацию таймеров по прерыванию ETR по спаду.
	*						в обработчиках прерываний реализован протокол PS/2
	*						Timer3 - используется для клавиатуры
	*						Timer2 - используется для мыши
  ******************************************************************************
  */

#include "ps2phy.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_port.h"
#include "systick.h"

#include "queue.h"

unsigned char BufferReceive=0;
unsigned char BufferReceive_mb=0;
unsigned short int Flag=0;
unsigned short int Flag_mb=0;
unsigned short int Status_mb=0;

uint8_t Data_In(void);

/********************************
обработчик прерывания клавиатуры
кадр протокола ps/2
*********************************/
void Timer3_IRQHandler(void)
{
	static uint8_t tmp=0;
	static uint8_t parity = 1;
	
	static enum{
		INIT,
		START,
		BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,
		PARITY,
		STOP,
	}state=INIT;
	
	if(MDR_TIMER3->STATUS & (1<<3))
	{
		MDR_TIMER3->STATUS = 0;  																														// Сброс флагов прерывания от Timer3
		state++;
		switch(state)
		{
			case START:
					if(MDR_PORTB->RXTX & (1<<3))
						state=INIT;
			break;
			case BIT0:
			case BIT1:
			case BIT2:
			case BIT3:
			case BIT4:
			case BIT5:
			case BIT6:
			case BIT7:
				tmp >>=1;
				if(MDR_PORTB->RXTX & (1<<3))
				{
					tmp |= 0x80;
					parity++;
				}
			break;
			case PARITY:
				if(MDR_PORTB->RXTX & (1<<3))
				{
					if (!(parity & 0x01))
					{
						state=INIT;
						parity=1;
						tmp=0;
					}		
				}					
				else {
							if (parity & 0x01)
							{
								state=INIT;
								parity=1;
								tmp=0;
							}						
					}	
			break;
			case STOP:
				if(!(MDR_PORTB->RXTX & (1<<3)))
				{
					state=INIT;
					parity=1;
					tmp=0;					
					break;
				}
				QueAdd(&record_key,(uint16_t)tmp);
				BufferReceive=tmp;
				state=INIT;
				parity=1;
				tmp=0;
				Flag=1;				
			break;
			default:
//				state=INIT;
//				parity=1;
//				tmp=0;	
//				Flag=0;
			break;
		}
	}
		
	  NVIC_ClearPendingIRQ(Timer3_IRQn);
} 

/********************************
обработчик прерывания мыши
кадр протокола ps/2
*********************************/
void Timer2_IRQHandler(void)
{
//	static uint8_t cnt=0;
	static uint8_t tmp=0;
	static uint8_t parity = 1;
	
	static enum{
		INIT,
		START,
		BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,
		PARITY,
		STOP,
	}state=INIT;
	
	if(MDR_TIMER2->STATUS & (1<<3))
	{
		MDR_TIMER2->STATUS = 0;  																														// Сброс флагов прерывания от Timer3
		if(Clock_In())return;
		state++;
		switch(state)
		{
			case START:
					if(MDR_PORTD->RXTX & (1<<6))
						state = INIT;
			break;
			case BIT0:
			case BIT1:
			case BIT2:
			case BIT3:
			case BIT4:
			case BIT5:
			case BIT6:
			case BIT7:
					tmp >>=1;
					if(MDR_PORTD->RXTX & (1<<6))
						{tmp |= 0x80; parity++;}
			break;
			case PARITY:
            if (Data_In()) {
                if (!(parity & 0x01))
								{
									state=INIT;
									parity=1;
									tmp=0;
							}
            } else {
                if (parity & 0x01)
								{
									state=INIT;
									parity=1;
									tmp=0;
								}
            }
            break;				
			case STOP:
				if(!(MDR_PORTD->RXTX & (1<<6)))
				{
					state=INIT;
					parity=1;
					tmp=0;

				}
				QueAdd(&QueueIn,(uint16_t)tmp);
				BufferReceive_mb=tmp;
				Flag_mb=1;
				state=INIT;
				parity=1;
				tmp=0;
				break;
			default:
				state=INIT;
				parity=1;
				tmp=0;	
				Flag_mb=0;				
				break;
		}
	}
	  NVIC_ClearPendingIRQ(Timer2_IRQn);
} 

void TIMER3_ETR_Setup(void)
{ 
		MDR_TIMER3->CNTRL = 0;
		MDR_TIMER3->IE = 0;
		MDR_RST_CLK->PER_CLOCK |= (1<<16);
		MDR_RST_CLK->TIM_CLOCK |= ((0 << RST_CLK_TIM_CLOCK_TIM3_BRG_Pos) 									/* делитель тактовой частоты Таймера 3 */
		| (1 << RST_CLK_TIM_CLOCK_TIM3_CLK_EN_Pos)); 																			/* разрешение тактовой частоты Таймера 3 */

		MDR_TIMER3->BRKETR_CNTRL |= (0<<4|0<<2);  																				// не делить внешнюю частоту

		MDR_TIMER3->IE = (1<<3);     																											// разрешение прерывания по спаду ETR

		MDR_TIMER3->CNTRL = (8<<TIMER_CNTRL_EVENT_SEL_Pos) | (1<<TIMER_CNTRL_CNT_EN_Pos); //ETR режим2, таймер вкл. 

		NVIC_EnableIRQ(Timer3_IRQn);
}

void TIMER2_ETR_Setup(void)
{ 
		MDR_TIMER2->CNTRL = 0;
		MDR_TIMER2->IE = 0;
		MDR_RST_CLK->PER_CLOCK |= (1<<15);
		MDR_RST_CLK->TIM_CLOCK |= ((0 << RST_CLK_TIM_CLOCK_TIM2_BRG_Pos) 									/* делитель тактовой частоты Таймера 2 */
		| (1 << RST_CLK_TIM_CLOCK_TIM2_CLK_EN_Pos)); 																			/* разрешение тактовой частоты Таймера 2 */

		MDR_TIMER2->BRKETR_CNTRL |= (0<<4|0<<2);  																				// не делить внешнюю частоту

		MDR_TIMER2->IE = (1<<3);     																											// разрешение прерывания по спаду ETR

		MDR_TIMER2->CNTRL = (8<<TIMER_CNTRL_EVENT_SEL_Pos) | (1<<TIMER_CNTRL_CNT_EN_Pos); //ETR режим2, таймер вкл. 

		NVIC_EnableIRQ(Timer2_IRQn);
}

void PORT_Mouse_Setup(void)
{
	PORT_InitTypeDef PORT_InitStructure;	
	PORT_StructInit(&PORT_InitStructure);	
	
	PORT_DeInit(MDR_PORTD);
	
	/*PS/2 для мыши*/
	/*PD5 - CLOCK; PD6 - DATA*/	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_5);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_OVERRID;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
	PORT_InitStructure.PORT_GFEN = PORT_GFEN_OFF;	
	PORT_InitStructure.PORT_PD = PORT_PD_OPEN;
	PORT_Init(MDR_PORTD, &PORT_InitStructure);
	
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_6);
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_ON;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;	
	PORT_InitStructure.PORT_PD = PORT_PD_OPEN;
	PORT_Init(MDR_PORTD, &PORT_InitStructure);	
}

void Mouse_PS2_PhyON(void)
{
	PORT_Mouse_Setup();
	
	TIMER2_ETR_Setup();							//инициализация CLOCK интерфейса PS/2
	
}

void Mouse_PS2_PhyOFF(void)
{
		MDR_TIMER2->CNTRL &= ~(1<<TIMER_CNTRL_CNT_EN_Pos); 				//таймер выкл. 

		NVIC_DisableIRQ(Timer2_IRQn);	

//		MDR_PORTD->FUNC &= ~(1<<10); 
//		MDR_PORTD->FUNC &= ~(1<<11);
//		MDR_PORTD->FUNC &= ~(1<<12);
//		MDR_PORTD->FUNC &= ~(1<<13);
}

void Data_Lo(void)
{
	MDR_PORTD->RXTX &= ~(1<<6);
	
	MDR_PORTD->OE |= (1<<6);
}

void Data_Hi(void)
{
	MDR_PORTD->OE &= ~(1<<6);
	
	MDR_PORTD->PULL &= ~(1<<6);
	
	MDR_PORTD->PULL |= (1<<22);	
}

void Clock_Hi(void)
{
	MDR_PORTD->OE &= ~(1<<5);
	
	MDR_PORTD->PULL &= ~(1<<5);		
	
	MDR_PORTD->PULL |= (1<<21);	
}

void Clock_Lo(void)
{
	MDR_PORTD->RXTX &= ~(1<<5);
	
	MDR_PORTD->OE |= (1<<5);
}

uint8_t Clock_In(void)
{
	MDR_PORTD->OE &= ~(1<<5);

	MDR_PORTD->PULL &= ~(1<<5);

	MDR_PORTD->PULL |= (1<<21);
	
	return MDR_PORTD->RXTX & (1<<5);
}

uint8_t Data_In(void)
{
	MDR_PORTD->OE &= ~(1<<6);

	MDR_PORTD->PULL &= ~(1<<6);

	MDR_PORTD->PULL |= (1<<22);

	_delay_us(1);
	
	return MDR_PORTD->RXTX&(1<<6);
}

unsigned char Wait_Mouse_Clock_Lo(unsigned int short us)
{
	while(Clock_In() && us){__ASM("nop"); _delay_us(1);us--;}		//
	return us;
}

unsigned char Wait_Mouse_Clock_Hi(unsigned int short us)
{
	while(!Clock_In() && us){__ASM("nop"); _delay_us(1);us--;}
	return us;
}

unsigned char Wait_Mouse_Data_Lo(unsigned int short us)
{
	while(Data_In() && us){__ASM("nop"); _delay_us(1);us--;}
	return us;
}

unsigned char Wait_Mouse_Data_Hi(unsigned int short us)
{
	while(!Data_In() && us){__ASM("nop"); _delay_us(1);us--;}
	return us;
}

