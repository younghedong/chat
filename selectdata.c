#include<stdio.h>
#include<stdlib.h>
#include<mysql.h>
#include<string.h>
#include"decrypt.h"
#include<time.h>

static const char *host = "localhost";
static const char *user = "root";
static const char *psd = "root";
static unsigned int port = 3306;
static const char *dbName = "chatMysql";
char real_result[50][200];
int flags = 0;

/*****************************登陆查询***********************************/
int selectpsd(char *uname, char *upsd)
{
    MYSQL *mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int rownum = 0;
    char destring[1024] = "\0";//解密字符串
    char *query = (char *)malloc(1024); //记录查询语句

    sprintf(query, "select * from user where userName = '%s'", uname);//查询语句赋值

    //mysql初始化
    if(!(mysql = mysql_init(NULL)))
    {
        return -1;
    }

    //判断数据库是否存在
    if((mysql_real_connect(mysql, host, user, psd, "mysql", port, NULL, 0)))
     {
        mysql_query(mysql, "create datebase if not exists chatMysql");
        mysql_query(mysql, "use chatMysql");
        mysql_query(mysql, "create table if not exists user(userName char(15) not null primary key, userPsd char(20) not null)");
     /*}
    
    //数据查询
    if(mysql_real_connect(mysql, host, user, psd, dbName, port, NULL, 0))
    {*/
        mysql_real_query(mysql, query, strlen(query));
        res = mysql_store_result(mysql);
        if((rownum = mysql_num_rows(res)) > 0)
            {
                row = mysql_fetch_row(res);
                Decrypt(row[1], destring);
                if(strcmp(destring, upsd) == 0)
                {
                    mysql_free_result(res);
                    free(query);
                    mysql_close(mysql);	
                    return 0;
                }
                else
                {
                    mysql_free_result(res);
                    free(query);
                    mysql_close(mysql);
                    return -4;//密码错误
                }
            }
        else
        {
            free(query);
            mysql_close(mysql);
            return -5;//账户不存在
        }
    }
    else
	return -3;//连接数据库失败
}


//********************************聊天记录查询*******************************//
char **select_chat_content(int num, char *send, char *accept, char *seCon)
{
    MYSQL *mysql;
    MYSQL_RES *ress;
    MYSQL_ROW row;
    
    int i = 0;
    char *belong = "\0";
    char *tmp = (char *)malloc(100);
    char **result;
    
    char *select_chat_query = (char *)malloc(100);
    
    result = (char **)malloc(1);//二级指针开辟空间
    for(i = 0; i < 1; i++)
        result[i] = (char *)malloc(20);

    /*real_result = (char **)malloc(50);
    for(i = 0; i < 50; i++)
        real_result[i] = (char *)malloc(200);
    */
    if(num == 0)//判断客户端or服务端
        belong = "client";
    else
        belong = "server";
    if(seCon == NULL)
    {
        sprintf(select_chat_query, "select * from chatRecord where accept = '%s'", accept);
		g_print("%s", select_chat_query);
	//select_chat_query = strcat("select * from chatRecord where accept = '",accept);
	//strcat(select_chat_query, "'");
    }
    else
    {
    	strcpy(select_chat_query, "select * from chatRecord where content like '%");
        //sprintf(select_chat_query, "select * from chatRecord where content like \'\%%s\%\'", seCon);
		//strcat(select_chat_query, "' and content like '%");
		strcat(select_chat_query, seCon);
		strcat(select_chat_query, "%'");
		//g_print("%s\n", select_chat_query);
    }
    //mysql初始化
    if(!(mysql = mysql_init(NULL)))
    {
        strcpy(result[0], "数据库初始化失败！");
        free(tmp);
        free(select_chat_query);
        //free(result[0]);
        for(i = 0; i < 50; i++)
            free(real_result[i]);
        return result;
    }
    
    //查询数据库是否存在
     if((mysql_real_connect(mysql, host, user, psd, "mysql", port, NULL, 0)))
     {
        mysql_query(mysql, "create datebase if not exists chatMysql");
        mysql_query(mysql, "use chatMysql");
        mysql_query(mysql, "create table if not exists chatRecord(belong char(10) not null, send char(15) not null, accept char(15) not null, content char(1024), time char(50))");
     /*}

    if((mysql_real_connect(mysql, host, user, psd, "chatMysql", port, NULL, 0)))
    {*/
    	//mysql_query(mysql, "select * from chatRecord where content like '%哈%'");
    	//g_print("sdfsdfsd\n");
        mysql_real_query(mysql, select_chat_query, strlen(select_chat_query));
        ress = mysql_store_result(mysql);
		//g_print("165465\n");
        flags = mysql_num_rows(ress);
        //g_print("sdfsdf%dsfsss", flags);
        if(flags)
        {
            for(i = 0; i < flags; i++)
            {
                row = mysql_fetch_row(ress);
                sprintf(tmp,"%s  %s  %s  %s  %s",row[0], row[1], row[2], row[3], row[4]);
                g_print("%s\n", tmp);
                strcpy(real_result[i], tmp);
            }
            mysql_free_result(ress);
            mysql_close(mysql);
	        free(tmp);
            free(select_chat_query);
	        free(result[0]);
	        //for(i = 0; i < flags; i++)
	        // g_print("%s\n", real_result[i]);
            return real_result;
        }
        else
        {
            mysql_close(mysql);
	        free(tmp);
            free(select_chat_query);
	        //free(result[0]);
            for(i = 0; i < 50; i++)
                free(real_result[i]);
            strcpy(result[0], "无记录！");
            return result;
        }
    }
    else
    {
	    free(tmp);
        free(select_chat_query);
	    //free(result[0]);
        for(i = 0; i < 50; i++)
            free(real_result[i]);
        strcpy(result[0], "数据库连接失败！");
        return result;
    }
}
