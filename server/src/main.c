#include "head.h"
#include "factory.h"
#include "work_que.h"
#include "syslog.h"
#define MAXCLIENT 100
void pwd_check(char *,char *,int);


void *handle_request(void *arg);

int main(int argc,char *argv[])
{
		if(argc!=4)
		{
				printf("the args should be ./ELF ADDR PORT NPTHREAD");
				exit(-1);
		}
		int listenfd;
		listenfd=socket(AF_INET,SOCK_STREAM,0);
		if(-1==listenfd)
		{
				perror("socket");	
				exit(-1);
		}
		struct sockaddr_in my_addr;
		memset(&my_addr,0,sizeof(struct sockaddr));
		my_addr.sin_family=AF_INET;
		my_addr.sin_port=htons(atoi(argv[2]));
		my_addr.sin_addr.s_addr=inet_addr(argv[1]);
		int npthread;
		npthread=atoi(argv[3]);
		int ret;
		ret=bind(listenfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr));
		if(-1==ret)
		{
				perror("bind");
				exit(-1);
		}
		ret=listen(listenfd,MAXCLIENT);
		if(-1==ret)
		{
				perror("listen");
				exit(-1);
		}
		struct sockaddr_in client_addr;
		int size=sizeof(struct sockaddr);
		int new_fd;
		pnode new_node;
		factory_t fact;
		factory_init(&fact,handle_request,NULL,npthread,0,100);
		factory_start(&fact,npthread);
		printf("waiting for requests\n");
		while(1)
		{
				memset(&client_addr,0,sizeof(struct sockaddr));
				new_fd=accept(listenfd,(struct sockaddr*)&client_addr,&size);
				if(-1==new_fd)
				{
						perror("accept");
						exit(-1);
				}
				printf("a client have linked  IP:%s PORT:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
				//链表操作
				syslog(LOG_INFO,"IP:%s\n",inet_ntoa(client_addr.sin_addr));
				new_node=(pnode)malloc(sizeof(snode));
				new_node->sockfd=new_fd;
				new_node->next=NULL;
				pthread_mutex_lock(&fact.sque.lock);
				que_set(&fact.sque,new_node);
				pthread_mutex_unlock(&fact.sque.lock);
				pthread_cond_signal(&fact.cond);
		}
		close(new_fd);
		close(listenfd);
		return 0;
}
void *handle_request(void *arg) 
{
		int new_fd;
		pfactory_t pf;
		pf=(pfactory_t)arg;
		pque_t pq=&pf->sque;
		pnode new_node;
		
//		fgets()
		while(1)
		{
				
				pthread_mutex_lock(&pq->lock);
				if(pq->size==0)
				{
						pthread_cond_wait(&pf->cond,&pq->lock);
				}
				que_get(pq,&new_node);
				pthread_mutex_unlock(&pq->lock);
				new_fd=new_node->sockfd;
				free(new_node);
				char username[10]={0};
				char passwd[10]={0};
				bzero(username,sizeof(username));
				recv(new_fd,username,sizeof(username),0);
				printf("username is %s\n",username);
				bzero(passwd,sizeof(passwd));
				recv(new_fd,passwd,sizeof(passwd),0);
	//			printf("passwd is %s\n",passwd);
				pwd_check(username,passwd,new_fd);
				syslog(LOG_INFO,"%s\n",username);

				while(1)
				{
						ord_t hor;	
						bzero(&hor,sizeof(hor));
						int ret;
						ret=recv(new_fd,&hor,sizeof(hor),0);
						if(-1==ret)	
						{
								perror("recv");
								exit(-1);
						}
						if(ret>0)
						{
								
				   				syslog(LOG_INFO,"order:%s pathname:%s\n",hor.order_name,hor.path_name);
								if(strcmp(hor.order_name,"cd")==0)
								{
										chdir(hor.path_name);
										continue;
								}else if(strcmp(hor.order_name,"ls")==0)
								{	
									//	printf("strlen is %d\n",strlen(hor.path_name));
										if(strcmp(hor.path_name,"")==0)
										{
												strcpy(hor.path_name,".");
										}
										send_ls(new_fd,hor.path_name);

										continue;
								}else if(strcmp(hor.order_name,"pwd")==0)
								{ 
										send_pwd(new_fd);
										continue;

								}else if(strcmp(hor.order_name,"remove")==0)
								{      ret=unlink(hor.path_name);
										if(-1==ret)
										{
												printf("file not exit\n");
										}
										continue;
								}else if(strcmp(hor.order_name,"puts")==0)
								{	
										recv_file(new_fd);
										continue;
								}else if(strcmp(hor.order_name,"gets")==0)
								{       send_file(new_fd,hor.path_name);
										continue;
								}else if(strcmp(hor.order_name,"quit")==0)
			                    {    	syslog(LOG_INFO,"client quit\n");
										pthread_exit(NULL);
								
								}else{//若需要完善需要响应客户端
										continue;
								}
						}
				}
		}
}









