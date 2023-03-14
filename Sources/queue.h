#ifndef QUEUE_H
#define QUEUE_H

//#include "MDR32F9Qx_config.h"
#include "stdint.h"

#define QUANTITY_INQUE		(100) //размер очереди

extern uint16_t Temp[6];
extern uint8_t flag;


struct queue
{
	uint16_t data[100]; //массив данных
	uint16_t last; 		//указатель на начало
};
extern struct queue QueueIn;
extern struct queue record_key;

void QueCreation(struct queue *q);
void QueAdd(struct queue *q, uint16_t value);
void QueDelete(struct queue *q);
uint16_t uiQueTop(struct queue *q);
uint16_t uiQueSize(struct queue *q);
void QueChangeData(struct queue *q, uint16_t val_new);

#endif
