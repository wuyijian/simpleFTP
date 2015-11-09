#include "head.h"






void send_file(int sfd,char *pathname)
{
	sbuf fbuf;
	printf("the sfd is %d\n",sfd);
	bzero(&fbuf,sizeof(fbuf));
	int ret;
	strcpy(fbuf.buf,pathname);
	fbuf.len=strlen(fbuf.buf);
	ret=send(sfd,(void *)&fbuf,4+fbuf.len,0);
	if(ret==-1)
	{
		perror("send_file_name");
		exit(-1);
	}
	printf("the filename is %s\n",fbuf.buf);
	int fd;
	int flag;
	fd=open(pathname,O_RDONLY);
	if(fd==-1)
	{
		perror("open");
		exit(-1);
	}
	while(bzero(&fbuf,sizeof(fbuf)),(fbuf.len=read(fd,fbuf.buf,sizeof(fbuf.buf)))>0)
	{
  		ret=send(sfd,(void *)&fbuf,4+fbuf.len,0);
		if(-1==ret)
		{
			perror("send_file");
			exit(-1);
		}
	//	flag=0;
	}
	flag=0;
	send(sfd,(void *)&flag,sizeof(int),0);
	//send(sockfd,(void *)&flag,sizeof(int),0);
}
	
