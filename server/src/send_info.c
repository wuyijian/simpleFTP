#include "head.h"
//pwd功能实现
void send_pwd(int sfd)
{
	char pwd_info[100]={0};
	strcpy(pwd_info,getcwd(NULL,0));
	int ret;
	ret=send(sfd,pwd_info,strlen(pwd_info),0);
	if(ret==-1)
	{
		perror("send");
		exit(-1);
	}
}



//ls功能实现
void send_ls(int sfd,char* pathname)
{
		DIR *pdir;
		pdir=opendir(pathname);
		if(pdir==NULL)
		{
				perror("opendir");
				printf("%s\n",pathname);
				exit(-1);
		}
		struct dirent *pd;
		struct stat buf;   
		int ret;
		char path[1024];
		char send_info[1024];
		while((pd=readdir(pdir))!=NULL)
		{
		 		memset(path,0,sizeof(path)); 
				memset(send_info,0,sizeof(send_info));
				sprintf(path,"%s/%s",pathname,pd->d_name);
				ret=stat(path,&buf);
				if(0!=ret)
				{
						perror("stat");
						exit(-1);
				}
				if(!strcmp(".",pd->d_name)||!strcmp("..",pd->d_name))
				{
						continue;
				}
				char a[100];
				strcpy(a,pd->d_name);
				if(a[0]=='.')
				{
						continue;
				}
				char c;
				switch(S_IFMT&buf.st_mode)
				{ 
						case(S_IFSOCK):c='s';break;
						case(S_IFLNK):c='l';break;
						case(S_IFREG):c='-';break;
						case(S_IFBLK):c='b';break;
						case(S_IFDIR):c='d';break;
						case(S_IFCHR):c='c';break;
						case(S_IFIFO):c='p';break;
				}
				char d,e,f,g,h,i,j,k,l;
				if(S_IRUSR&buf.st_mode)
						d='r';else d='-';
				if(S_IWUSR&buf.st_mode)
						e='w';else e='-';
				if(S_IXUSR&buf.st_mode)
						f='x';else f='-';
				if(S_IRGRP&buf.st_mode)
						g='r';else g='-';
				if(S_IWGRP&buf.st_mode)
						h='w';else h='-';
				if(S_IXGRP&buf.st_mode)
						i='x';else i='-';
				if(S_IROTH&buf.st_mode)
						j='r';else j='-';
				if(S_IWOTH&buf.st_mode)
						k='w';else k='-';
				if(S_IXOTH&buf.st_mode)
						l='x';else l='-';

				char time[15];
				char dir_info[100];
				time_t t=buf.st_mtime;
				struct tm lt;
				localtime_r(&t,&lt);
				strftime(time,sizeof(time)," %b %d %H:%M",&lt);
				sprintf(dir_info,"%c%c%c%c%c%c%c%c%c%c%4u%-10s%8ld%-20s%-10s\n",c,d,e,f,g,h,i,j,k,l,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,buf.st_size,time,pd->d_name);
				

				ret=send(sfd,dir_info,strlen(dir_info),0);
				if(ret==-1)
				{
					perror("send");
					exit(-1);
				}
	     //   	printf("%s\n",dir_info);
		//		strcat(send_info,dir_info);可以把字符串拼接赋给第一个参数。
		}
}

