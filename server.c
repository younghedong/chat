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
#include <signal.h>
#include<errno.h>
#define NUM 6
volatile int sfp,nfp[NUM];
volatile int nfp_f[100]={0};
volatile char user[100][100];
volatile char password[100][100];
volatile int num=0;
volatile int flag=1;
pthread_t th[NUM];
volatile struct sockaddr_in s_add,c_add[NUM],tmp_add;
int name_to_socket(char a[1024])
{
	int i;
	for(i=0;i<100;i++)
	{
		if(strcmp(user[i],a)==0)
			return i;
	}
}
char * socket_to_name(int a)
{

}
void *read_write(int *a)
{
	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	printf("执行到了read_write\r\n");
	char buffer[1024]={0};
	int recbytes;
	int i;
	int target_scoket;
	int source_scoket;
	char target_name[1024];
	char tmp_text[1024];
	char send_buffer[1024];
	void handle_quit(int signo)
	{
		pthread_exit(NULL);
	}
	signal(SIGQUIT,handle_quit);

	while(1)
	{
			//printf("%d!\r\n",*a);
			if(*a>0)
			{
				bzero(buffer,1024);
				//printf("cbhhf %d!\r\n",*a);
				if(-1 == (recbytes = read(*a,buffer,1024)))
				{
					//printf("read data fail !\r\n");
					
						//shutdown(*a,SHUT_RDWR);
						close(*a);
						nfp_f[*a]=0;
						//*a=-9;
						flag=1;
						num--;
						
						break;
					


				}
				if(0 == recbytes)
				{

					
						//shutdown(*a,SHUT_RDWR);
						close(*a);
						nfp_f[*a]=0;
						flag=1;
						num--;
						//*a=-9;
						break;
					

				}
				//printf("read OK\r\nREC:\r\n");
				//buffer[recbytes]='\0';//结尾
				printf("%s\r\n",buffer);
				sscanf(buffer,"%d:%[^\n]",&target_scoket,tmp_text);//拿到目标socket
				//printf("%s\r\n",tmp_text);
				if(target_scoket==-9999)//接收用户名密码
				{

					sscanf(tmp_text,"%[^:]:%s",user[*a],password[*a]);
					printf("%s\r\n",tmp_text);
					printf("a is %d用户名%s密码%s\r\n",*a,user[*a],password[*a]);
					flag=1;
				}
				else//正常转发
				{
					char nor_text[1024];
					sscanf(tmp_text,"%[^:]:%[^\n]",target_name,nor_text);
					int tar_sock=name_to_socket(target_name);
					sprintf(send_buffer,"%d:%s:%s\n",5555,user[*a],nor_text);

					//printf("before write %s\r\n",send_buffer);
					//write(4,send_buffer,strlen(send_buffer));
					if(strlen(send_buffer)!=write(tar_sock,send_buffer,strlen(send_buffer)))
					{
						char send_buff[30];
						sprintf(send_buff,"%d:%s:%s\n",5555,"server","对方不在线");
						write(*a,send_buff,strlen(send_buff));
					}
					//printf("after write %s\r\n",send_buffer);
				}
		}
	}

}
void *acceptclient()//接受链接，记录地址，生成socket
{
	printf("执行到了accept\r\n");
	int sin_size = sizeof(struct sockaddr_in);
	while(1)
	{//for(i=0;i<NUM;i++)
		nfp[num] = accept(sfp, (struct sockaddr *)(&c_add[num]), &sin_size);
		nfp_f[(nfp[num])]=1;//可用
		if(-1 == nfp[num])
		{
		    printf("accept fail !\r\n");
		}

		printf("num is %d accept %d OK!\r\nServer start get connect from %#x : %#x\r\n",num,nfp[num],ntohl(c_add[num].sin_addr.s_addr),ntohs(c_add[num].sin_port));
		//pthread_t id3;
		pthread_create(&th[num],0,read_write,&nfp[num]);
		num++;//记录当前连接数
		//flag=1;//连接表有变化分发开关打开


	}

}
void *update()//向所有已连接客户端发送最新的连接表
{
	printf("执行到了update\r\n");
	int j,tmp=0;
	char send_buf[50];
	int update_flags=-7776;
	while(1)
	{
		for(j=0;j<num;j++){
			if(nfp_f[(nfp[j])]==0){//检察到有连接断开,将该nfp【j】被后面的覆盖，依次前移
				tmp=j;
				c_add[tmp]=tmp_add;
				if(ESRCH!=pthread_kill(th[tmp],0))
				{

					pthread_kill(th[tmp],SIGQUIT);
					printf("pthread_cancel\r\n");
				}


				while(1)
				{
					if(tmp+1<NUM){
					nfp[tmp]=nfp[tmp+1];
					th[tmp]=th[tmp+1];
					tmp++;
					}
					else{
						break;
					}
				}
				//num--;// 连接数-1
				j=0;
				flag=1;//连接表有变化分发开关打开
			}
		}
		if(flag==1&&num>=0)//向所有client分发最新的连接表
		{
			update_flags--;//发起一次新的更新的的标志
			printf("执行到了分发\r\n");
			flag=0;
			bzero(send_buf,50);
			char tttmp[50];
			for(j=0;j<num;j++)
			{
				sprintf(tttmp,"%s:%s",user[(nfp[j])],send_buf);
				sprintf(send_buf,"%s",tttmp);

			}
			char u_text[50];
			sprintf(u_text,"%d:%s",update_flags,send_buf);
			for(j=0;j<num;j++)
			{

					if(-1 == write(nfp[j],u_text,strlen(u_text)))
					{
						printf("write fail!\r\n");
					}
				//	printf("%s\r\n",u_text);

				//printf("write OK!\r\n");
			}
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
	printf("socket OK !\r\n");


	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family=AF_INET;
	s_add.sin_addr.s_addr=htonl(INADDR_ANY);
	s_add.sin_port=htons(portnum);

	if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	{
	    printf("bind fail !\r\n");
	    return -1;
	}
	printf("bind OK !\r\n");

	if(-1 == listen(sfp,5))
	{
	    printf("listen fail !\r\n");
	    return -1;
	}
	printf("listen OK\r\n");
	pthread_t id1;
	pthread_create(&id1,0,acceptclient,NULL);
	pthread_t id2;
	pthread_create(&id2,0,update,NULL);

	getchar();
	getchar();
	//close(sfp);
	return 0;
}


