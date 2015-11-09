#include "head.h"
#define MAXDATA 1000

typedef struct send_recv{
	int len;
	char buf[MAXDATA];
}smsg,*pmsg;

void recv_file(int sfd)
{
	smsg msg;
	bzero(&msg,4+sizeof(msg.buf));
    int ret;
	ret=recv(sfd,&msg.len,sizeof(int),0);
	if(-1==ret)
	{
		perror("recv_file_name");
		exit(-1);
	}
	ret=recv(sfd,msg.buf,msg.len,0);
	if(-1==ret)
	{
			perror("recv");
			exit(-1);
	}
	char filename[128]={0};
	strcpy(filename,msg.buf);
	printf("the file name is %s\n",msg.buf);
	int fd;
	fd=open(filename,O_CREAT|O_RDWR,0644);
    if(-1==fd)
	{
		perror("open");
		exit(-1);
	}
	while(1)
	{
		recv(sfd,&msg.len,sizeof(int),0);
		if(msg.len==0)
		{
		break;
		}
		recv(sfd,msg.buf,msg.len,0);
		write(fd,msg.buf,msg.len);
		bzero(&msg,4+sizeof(msg.buf));
	}	
	printf("you file has finished download\n");
	close(fd);	
//	close(sfd);
	
}
	
