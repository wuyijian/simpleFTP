#ifndef _FACTORY_H_
#define _FACTORY_H_
#include "work_que.h"

typedef void*(*pfunc)(void*);//定义一个函数指针，pfunc 为函数入口地址；


typedef struct factory{
	que_t sque;
	pfunc fun_con;//就是子线程处理函数
	pfunc fun_pro;
	pthread_t *conid;
	pthread_t *proid;
	pthread_cond_t cond;
	int flag;
}factory_t,*pfactory_t;


void factory_init(pfactory_t parr,pfunc fun_con,pfunc fun_pro,int con_num,int pro_num,int capacity);

void factory_start(pfactory_t parr,int npthread);


#endif












