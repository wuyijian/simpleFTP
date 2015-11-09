#include "head.h"
#include "factory.h"


void factory_init(pfactory_t parr,pfunc fun_con,pfunc fun_pro,int con_num,int pro_num,int capacity)
{
	que_init(&parr->sque,capacity);
//	printf("que_init\n");
	parr->fun_con=fun_con;
	parr->fun_pro=NULL;
	pthread_cond_init(&parr->cond,NULL);
	parr->conid=(pthread_t *)calloc(con_num,sizeof(pthread_t));
	parr->proid=NULL;
	parr->flag=0;
}



void factory_start(pfactory_t parr,int npthread)
{
	int ret;
	if(parr->flag==1)
	{
		return;
	}
	parr->flag=1;
	int i;
	for(i=0;i<npthread;i++)
	{
		ret=pthread_create(&parr->conid[i],NULL,parr->fun_con,(void *)parr);
		if(0!=ret)
		{
			printf("the error_n is %d\n",ret);
			exit(-1);
		}
	}
}
