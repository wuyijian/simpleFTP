#include "head.h"

#include <shadow.h>
#include <errno.h>
#include <crypt.h>
void get_salt(char *salt,char *passwd);
//void pwd_check(char *username,char *passwd);

/*void help(void)
{
	printf("password checking program\n,the first arg is user name\n");
	exit(-1);
}

void error_quit(char *msg)
{
	perror(msg);
	exit(-2);
}*/

void get_salt(char *salt,char *passwd)
{
 //   printf("i am in get_sal\n");
	int i,j;
	for(i=0,j=0;passwd[i]&&j!=3;++i)
	{
		if(passwd[i]=='$')
			++j;
	}

	strncpy(salt,passwd,i-1);
//	printf("%s\n",salt);
}

void pwd_check(char *username,char *passwd,int sfd)
{
	
//	printf("i am here\n");
	struct spwd *sp;
	sp=(struct spwd *)malloc(sizeof(struct spwd));
	bzero(sp,sizeof(struct spwd));
	char salt[512]={0};
	
	sp=getspnam(username);
 	if(sp==NULL)
	{
		perror("getspnam");
		send(sfd,"the user name not exit",22,0);
		pthread_exit(NULL);
	}else{ 
	    get_salt(salt,sp->sp_pwdp);
	}

	if(strcmp(sp->sp_pwdp,crypt(passwd,salt))==0)
	{
		send(sfd,"success,enjoy",13,0);
	        return;	
	}else{
		send(sfd,"error password",14,0);
		pthread_exit(NULL);
	}
}
		
