#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<gtk/gtk.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#define NUM 4
int sfp,nfp[NUM];
int num=0;
struct sockaddr_in s_add,c_add[NUM];

void *accept()//接受链接，记录地址，生成socket
{
	int sin_size = sizeof(struct sockaddr_in);
	int i=0;
	while(num<NUM)
	for(i=0;i<NUM;i++)
	{	

		nfp[i] = accept(sfp, (struct sockaddr *)(&c_add[i]), &sin_size);
		if(-1 == nfp[i])
		{
		    printf("accept fail !\r\n");
		    return -1;
		}
		num=i+1;//记录当前连接数
		printf("accept %d ok!\r\nServer start get connect from %#x : %#x\r\n",i,ntohl(c_add[i].sin_addr.s_addr),ntohs(c_add[i].sin_port));

	}

}
void *update()//向所有已连接客户端发送最新的连接表
{
	int j,tmp=0;
	for(j=0;j<num;j++){
		if(nfp[j]<0){//检察到有连接断开
			tmp=j;
			while(1)
			{
				if(tmp+1<NUM){
				nfp[tmp]=nfp[tmp++];
				}
				else{
					break;
				}
			}
			num--;// 连接数-1
		}
	}
	char send_buf[NUM][4];
	for(j=0;j<num;j++)
	{
		sprintf(send_buf[j],"%d",nfp[j]);

	}
	for(j=0;j<num;j++)
	{
		if(-1 == write(nfp[j],send_buf,4*(num+1)))
		{
			printf("write fail!\r\n");
			return -1;
		}
		printf("write ok!\r\n");
	}
}
void *read_write()
{
	char buffer[1024]={0};
	int recbytes;
	int i;
	int target_scoket;
	int source_scoket;
	char *tmp_text;
	char *send_buffer;
	while(1)
	{
		for(i=0;i<num;i++)//分发消息
		{
			if(-1 == (recbytes = read(nfp[i],buffer,1024)))
			{
				printf("read data fail !\r\n");
				return -1;
			}
			printf("read ok\r\nREC:\r\n");
			buffer[recbytes]='\0';//结尾
			sscanf(buffer,"%d:%s",&target_scoket,tmp_text);//拿到目标socket
			sprintf(send_buffer,"%d:%s",nfp[i],tmp_text);
			write(target_scoket,send_buffer,strlen(send_buffer));
		}
	}
}
int main()
{
	

	unsigned short portnum=0x8888;

	printf("Hello,welcome to my server !\r\n");
	sfp = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfp)
	{
	    printf("socket fail ! \r\n");
	    return -1;
	}
	printf("socket ok !\r\n");


	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family=AF_INET;
	s_add.sin_addr.s_addr=htonl(INADDR_ANY);
	s_add.sin_port=htons(portnum);

	if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	{
	    printf("bind fail !\r\n");
	    return -1;
	}
	printf("bind ok !\r\n");

	if(-1 == listen(sfp,5))
	{
	    printf("listen fail !\r\n");
	    return -1;
	}
	printf("listen ok\r\n");
	pthread_t id2;
	pthread_create(&id2,0,accept,NULL);
	
	close(sfp);
	return 0;
}


