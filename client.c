#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
//#include<gtk/gtk.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
int cfd;
int recbytes=0;
char buffer[1024]={0};
int target=-1;
char *text;
void *tcp_read()
{
	char ff[1024];
	while(1)
	{
		bzero(buffer,1024);
		if(-1 == (recbytes = read(cfd,buffer,1024)))
		{
			printf("read data fail !\r\n");
			return 0;
		}
		//printf("read ok\r\nREC:\r\n");
		buffer[recbytes]='\0';
		printf("%s\r\n",buffer);
		recbytes=0;
		//bzero(buffer,1024);
		sscanf(buffer,"%d:%[^\n]",&target,ff);
		//printf("%d\r\n",target);
		//printf("%s\r\n",ff);
	}
}
int main()
{


	int sin_size;
	struct sockaddr_in s_add,c_add;
	unsigned short portnum=0x8888; 

	printf("Hello,welcome to client !\r\n");

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == cfd)
	{
	    printf("socket fail ! \r\n");
	    return -1;
	}
	printf("socket ok !\r\n");

	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family=AF_INET;
	s_add.sin_addr.s_addr= inet_addr("127.0.0.1");
	s_add.sin_port=htons(portnum);
	printf("s_addr = %#x ,port : %#x\r\n",s_add.sin_addr.s_addr,s_add.sin_port);


	if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	{
	    printf("connect fail !\r\n");
	    return -1;
	}
	printf("connect ok !\r\n");

	pthread_t id2;
	pthread_create(&id2,0,tcp_read,NULL);

	while(1)
	{
		if(target>0)
		{
			char buf[1024];
			char send[1024];
			gets(buf);
			sprintf(send,"%d:%s\r\n",target,buf);
			//printf("before write pelese call me back\r\n");
			write(cfd,send,strlen(send));
			//printf("after write pelese call me back\r\n");
		}
	}
			getchar();
			getchar();
	//close(cfd);
	return 0;
}


