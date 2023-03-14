#include "queue.h"

struct queue QueueIn;
struct queue record_key;

//uint16_t Temp[6];
//uint8_t flag=0;

void QueCreation(struct queue *q)
{ 
	q->last=0; 
}

void QueAdd(struct queue *q, uint16_t value)
{
	if (q->last==QUANTITY_INQUE) return;
	
	q->data[q->last++]=value;

}

void QueDelete(struct queue *q)
{
	int i=0;
	if(q->last==0)return;
	for (i=0; i<q->last && i<QUANTITY_INQUE; i++)
		q->data[i]=q->data[i+1];
	
	q->last--;
}

void QueChangeData(struct queue *q, uint16_t val_new)
{
	if(q->last==0)return;
	q->data[0] = val_new;
}

uint16_t uiQueTop(struct queue *q)
{ 
	return q->data[0]; 
}

uint16_t uiQueSize(struct queue *q)
{ 
	return q->last; 
}
