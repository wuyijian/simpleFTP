#ifndef _WORK_QUE_H_
#define _WORK_QUE_H_

typedef struct node{
	int sockfd;
	struct node *next;
}snode,*pnode;

typedef struct work_que{
	pnode head,tail;
	int size;
	int capacity;
	pthread_mutex_t lock;
}que_t,*pque_t;

void que_init(pque_t que,int capacity);//空链表，首尾指针赋NULL;

void que_set(pque_t que,pnode new_node);

void que_get(pque_t que,pnode *new_node);//传递二级指针，改变new_node的值

#endif
