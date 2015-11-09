#ifndef _HEAD_H_
#define _HEAD_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <sys/epoll.h>
#include <pthread.h>
#define MAXDATA 1000

typedef struct order{
	char order_name[10];
	char path_name[100];
}ord_t,*pord_t;

typedef struct file_buf{
    int len;
    char buf[MAXDATA];
}sbuf;

typedef struct send_recv{
	int len;
	char buf[MAXDATA];
}smsg,*pmsg;


#endif



