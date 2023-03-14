/**
  ******************************************************************************
  * @file    ps2phy.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    16.12.2020
  * @brief   Содержит объявление функций: инициализация таймеров
  ******************************************************************************
  */

#ifndef _PS2PHY_H
#define _PS2PHY_H

extern unsigned char BufferReceive;
extern unsigned char BufferReceive_mb;
extern unsigned short int Flag;
extern unsigned short int Flag_mb;

void TIMER2_ETR_Setup(void);
void TIMER3_ETR_Setup(void);
void Mouse_PS2_PhyON(void);
void Mouse_PS2_PhyOFF(void);
void Data_Lo(void);
void Data_Hi(void);
void Clock_Hi(void);
void Clock_Lo(void);
unsigned char Wait_Mouse_Clock_Lo(unsigned int short us);
unsigned char Wait_Mouse_Clock_Hi(unsigned int short us);
unsigned char Wait_Mouse_Data_Lo(unsigned int short us);
unsigned char Wait_Mouse_Data_Hi(unsigned int short us);

unsigned char Clock_In(void);

void PORT_Mouse_Setup(void);

#endif
