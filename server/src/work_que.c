#include "head.h"
#include "work_que.h"

void que_init(pque_t que,int cap)
{
	que->head=NULL;
	que->tail=NULL;
	que->size=0;
	que->capacity=cap;
	pthread_mutex_init(&que->lock,NULL);//初始化一个锁
}
void que_set(pque_t que,pnode new_node)
{
	if(que->head==NULL)
	{
		que->head=new_node;
	}else{
		(que->tail)->next=new_node;
	}
	que->tail=new_node;
	que->size++;
}

void que_get(pque_t que,pnode *new_node)
{
	*new_node=que->head;
	que->head=(que->head)->next;
	if(que->head=NULL)
	{
		que->tail=NULL;
	}
	que->size--;
}	
	


