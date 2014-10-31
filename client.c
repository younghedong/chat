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
#include <gtk/gtk.h>
#define IP "172.17.21.32"
extern char chatlist[20][20];
extern int count;
extern char chatlistfoucs[20];
char chathistorybuffer[20][51200];//每个聊天对话的buf
char table[20][20];
int cfd;
int recbytes=0;
char buffer1[1024]={0};
int target=-1;
char *text;
char nor_text[1024];
char source_name[100];
static char user[20];

extern GtkTextBuffer *buffer;
extern GtkTextIter end;
extern GtkTextIter start;
#define NUM 20

void requre_update(char *username)
{
	printf("刷新\r\n");
	char s[1024];
	sprintf(s,"%d:%s:%s\r\n",-7777,username,"abcdefg");//刷新标志发送
	write(cfd,s,strlen(s));
}


void verify(char *username, char *password)
{
	char ch[1024];
	
	char passwd[20];
	
	sprintf(user,"%s",username);
	sprintf(passwd,"%s",password);
	sprintf(ch,"%d:%s:%s\n",-9999,user,passwd);
	write(cfd,ch,strlen(ch));
}
void tcp_send(char *user, char *text)
{
	
	char buf[1024];
	char send[1024];
	char  targ[100];
	//getchar();
	printf("选择目标\r\n");
	sprintf(targ,"%s",user);
	sprintf(buf,"%s",text);
	//gets(targ);
	//sscanf(targ,"%s",target);
	if(strcmp(targ,"0")==0)
	{
		close(cfd);
		printf("已关闭cfd\r\n");
	}
	//printf("输入正文\r\n");
	//gets(buf);
	sprintf(send,"%d:%s:%s\r\n",5555,targ,buf);//正文发送
	//printf("before write %s\r\n",send);
	write(cfd,send,strlen(send));
	//printf("after write %s\r\n",send);
	//close(cfd);
}
void *tcp_read()
{
	char ff[1024];
	int tt[NUM];
	int num=0;
	int update_flags=0;
	printf("当前在线用户列表:\r\n");
	char tem[51200];
	char s_name[20];
	char public[7] = "public";
	while(1)
	{
		if(cfd>=0)
		{
			//printf("cfd is %d\r\n",cfd);

			bzero(buffer1,1024);
			if(-1 == (recbytes = read(cfd,buffer1,1024)))
			{
				printf("read data fail !\r\n");
				close(cfd);
				cfd=-1;
				break;
			}
			if(recbytes==0)
			{
				printf("connect fail!\r\n");
				close(cfd);
				cfd=-1;
				break;
			}
			//printf("read ok\r\nREC:\r\n");
			//buffer[recbytes]='\0';
			//printf("%s\r\n",buffer);
			//recbytes=0;
			//bzero(buffer,1024);
			sscanf(buffer1,"%d:%[^\n]",&target,ff);

			if(target<0&&target!=-1)//target<0 是更新
			{
					//while()
					//sscanf(ff,"",&tt[num]);//ff存着当前的用户列表
					//printf("用户：%d\r\n",tt);
				//printf("%d 更新 %s\r\n",target,ff);
				int tab=0;
				char ttmp[1024];
				for(tab=0;tab<NUM;tab++)//从ff中分解出每个在线的用户
				{
					bzero(ttmp,1024);
					sscanf(ff,"%[^:]:%s",table[tab],ttmp);
					/*if(strcmp(table[tab],"")==0)
					{
						break;
					}*/
					bzero(ff,1024);
					sprintf(ff,"%s",ttmp);
				}
				for(tab=0;tab<NUM;tab++)
				{
					printf("----%s-----\r\n",table[tab]);
				}

			}
			else//是消息
			{
			
				bzero(source_name,100);
				bzero(nor_text,1024);
				sscanf(ff,"%[^:]:%[^\n]",source_name,nor_text);
				printf("%s 消息 %s\r\n",source_name,nor_text);
				gtk_text_buffer_get_end_iter(buffer,&end);
				
				int ii = 0;
				for(ii = 0; ii < 6; ii++)
				{
					if(public[ii] != source_name[ii])
						break;
				}
				if(ii == 5)
				{
					int length = strlen(source_name);
					for(ii = 0; ii < length; ii++)
					{
						source_name[ii] = source_name[ii+5];
						source_name[ii+5] = '\0';
					}
					strcpy(s_name, source_name);
					char ss[20];
					bzero(ss,20);
					sprintf(ss,"%s: %s\r\n",s_name, nor_text);
					strcpy(nor_text, ss);
					strcpy(source_name, "public");
					
				}
				if(strcmp(source_name, "server") == 0)
				{
					gtk_text_buffer_insert(buffer,&end,ff,-1);
				}
				else
				{
				int chatlistnum = 0; 
				for(chatlistnum = 0; chatlistnum < count; chatlistnum ++)
				{
					if(strcmp(source_name, chatlist[chatlistnum]) == 0)
					{
						break;
					}
				}
				gchar entrybuf[1024];
				bzero(entrybuf,1024);
				sprintf(entrybuf,"%s: %s\r\n",source_name, nor_text);
				
				strcpy(tem, chathistorybuffer[chatlistnum]);
				strcat(tem, entrybuf);
				//bzero(chatlist[chatlistnum], sizeof(chatlist[chatlistnum]));
				strcpy(chathistorybuffer[chatlistnum], tem);
				g_print("tem::: %s\r\n", chathistorybuffer[chatlistnum]);
				//sprintf(tem, "%s%s",chatlist[chatlistnum], entrybuf);
				//bzero(chatlist[chatlistnum], sizeof(chatlist[chatlistnum]));
				//strcpy(chatlist[chatlistnum], tem);
				//gtk_text_buffer_get_end_iter(buffer,&end);
				//gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);
				//gtk_text_buffer_set_text(buffer, entrybuf, sizeof(entrybuf));
				//gtk_entry_set_text(buffer, chatlist[chatlistnum]);
				//gtk_text_view_get_buffer();
				//gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer), &start, &end);
				//gtk_text_buffer_get_end_iter(buffer,&end);
				//g_printf("消息: ");
				//if(strcmp(chatlistfoucs, source_name) == 0)
				
					g_print("0000focus: %s\r\n", chatlistfoucs);
					gtk_text_buffer_insert(buffer,&end,entrybuf,-1);
				
				
				insert_chat_data(0, source_name, user, entrybuf);	
			}
			}
			//printf("%d\r\n",target);
			//printf("%d\r\n",tt);
		}
	}
}
int client()
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
	s_add.sin_addr.s_addr= inet_addr(IP);
	s_add.sin_port=htons(portnum);
	printf("s_addr = %#x ,port : %#x\r\n",s_add.sin_addr.s_addr,s_add.sin_port);


	if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	{
	    printf("connect fail !\r\n");
	    return -1;
	}
	printf("connect ok !\r\n");

	//printf("选择目标\r\n");
	//gets(targ);
	//sscanf(targ,"%d",&target);
	
	pthread_t id2;
	pthread_create(&id2,0,tcp_read,NULL);

	
	return 0;
}


