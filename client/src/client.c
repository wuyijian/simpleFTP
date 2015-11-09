#include "head.h"
#include <sys/epoll.h>
typedef struct recv_info{
		char order_name[10];
		char path_name[100];
}rord_t,*prord_t;

int main(int argc,char *argv[])
{
		if(argc!=3)
		{
				printf("error args\n");
				exit(-1);
		}
		//互联
		int sfd;
		sfd=socket(AF_INET,SOCK_STREAM,0);
		if(-1==sfd)
		{
				perror("socket");
				exit(-1);
		}
		int ret;
		struct sockaddr_in server_addr;
		bzero(&server_addr,sizeof(struct sockaddr)); 
		server_addr.sin_family=AF_INET;
		server_addr.sin_port=htons(atoi(argv[2]));
		server_addr.sin_addr.s_addr=inet_addr(argv[1]);

		ret=connect(sfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
		if(-1==ret)
		{
				perror("connect");
				exit(-1);
		}
		int epfd;
		epfd=epoll_create(1);
		if(-1==epfd)
		{
				perror("epoll_create");
				exit(-1);
		}
		struct epoll_event event;	
		struct epoll_event *eves;
		eves=(struct epoll_event *)malloc(2*sizeof(event));
		event.data.fd=sfd;
		event.events=EPOLLIN;
		epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
		event.data.fd=0;
		event.events=EPOLLIN;
		epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
		char ch;
		int i;
		int j=0;
		rord_t orev;
		char rec_buf[1024];
		//用户名密码输入
     	char username[10]={0};
		char *passwd;
		system("clear");
		printf("tip:if you have the wrong passwd or username,please enter quit to try again\n");
		printf("enter quit to leave\n");
 	 	printf("login as:");
		fgets(username,10,stdin);
		username[strlen(username)-1]='\0';
    	send(sfd,username,strlen(username),0);
		passwd=getpass("password:");
		send(sfd,passwd,strlen(passwd),0);
		
	   
		while(1)
		{
				bzero(eves,2*sizeof(event));
				ret=epoll_wait(epfd,eves,2,-1);
				if(-1==ret)
				{
						perror("epoll_wait");
						exit(-1);
				}
				if(ret>0)	
				{

						for(i=0;i<ret;i++)
						{
								if(eves[i].data.fd==0 && eves[i].events==EPOLLIN)
								{	
										
										j=0;
										bzero(&orev,sizeof(orev));
										ch=getchar();
										while(ch!=' ')
										{
												orev.order_name[j]=ch;
												ch=getchar();
												if(ch=='\n')
														break;
												j++;
										}
										j=0;
										while(ch!='\n')
										{
												ch=getchar();
												orev.path_name[j]=ch;
												j++;
										}
									    orev.path_name[j-1]='\0';
	//									printf("%s\n",orev.order_name);
	//									printf("%s\n",orev.path_name);
										ret=send(sfd,&orev,sizeof(orev),0);
										if(-1==ret)
										{
												perror("send_order");
												exit(-1);
										}
									    if(strcmp(orev.order_name,"puts")==0)
										{	
												send_file(sfd,orev.path_name);
												printf("send success\n");
										}
										if(strcmp(orev.order_name,"quit")==0)
										{
												close(sfd);
												exit(1);
										}
								}
								if(eves[i].data.fd==sfd && eves[i].events==EPOLLIN)
								{	
										bzero(rec_buf,sizeof(rec_buf));
										if(strcmp(orev.order_name,"gets")==0)
										{
												recv_file(sfd);
										}else{
	//									usleep(10);
										ret=recv(sfd,rec_buf,sizeof(rec_buf),0);
										if(-1==ret)
										{
												perror("recv_info");
												exit(-1);
										}
										printf("%s\n",rec_buf);
										
										}
								}
						}

				}



		}

		close(sfd);
}
