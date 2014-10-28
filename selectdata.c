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
    /*if(mysql_select_db(mysql, dbName) != 0)
    {
        return false;
    }
	*/
    //数据查询
    if(mysql_real_connect(mysql, host, user, psd, dbName, port, NULL, 0))
    {
        mysql_real_query(mysql, query, strlen(query));
        res = mysql_store_result(mysql);
        if((rownum = mysql_num_rows(res)) > 0)
            {
                row = mysql_fetch_row(res);
                Decrypt(row[1], destring);
                if(strcmp(destring, upsd) == 0)
                {
                    mysql_free_result(res);
                    mysql_close(mysql);	
                    return 0;
                }
                else
                {
                    mysql_free_result(res);
                    mysql_close(mysql);
                    return -4;//密码错误
                }
            }
        else
        {
            mysql_close(mysql);
            return -5;//账户不存在
        }
    }
    else
	return -3;//连接数据库失败
}


//********************************聊天记录查询*******************************//
char **select_chat_content(int num, char *send, char *accept)
{
    MYSQL *mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int flags = 0;
    int i = 0;
    char *belong = "\0";
    char *tmp = (char *)malloc(1024);
    char **result = (char **)malloc(1024);
    char **real_result = (char **)malloc(4096);
    char *select_chat_query = (char *)malloc(1024);
    
    if(num == 0)
        belong = "client";
    else
        belong = "server";

    sprintf(select_chat_query, "select * from chatRecord where (belong = '%s' and send = '%s' and accept = '%s')", belong, send, accept);
	g_print("%s",select_chat_query);
    //mysql初始化
    if(!(mysql = mysql_init(NULL)))
    {
        strcpy("数据库初始化失败！", result[0]);
        return result;
    }
    
    //查询数据库是否存在
    /* if((mysql_real_connect(mysql, host, user, psd, "mysql", port, NULL, 0)))
     {
        mysql_query(mysql, "create datebase if not exists chatMysql");
        mysql_query(mysql, "use chatMysql");
        mysql_query(mysql, "create table if not exists chatRecord(belong char(10) not null, send char(15) not null, accept char(15) not null, content char(1024), time char(50))");
        strcpy("无数据库！", result[0]);
        return result;
     }
*/
    if((mysql_real_connect(mysql, host, user, psd, "chatMysql", port, NULL, 0)))
    {
        mysql_real_query(mysql, select_chat_query, strlen(select_chat_query));
        res = mysql_store_result(mysql);
		g_print("123\n");
        if((flags = mysql_num_rows(res)) > 0)
        {
			g_print("321");
            for(i = 0; i < flags; i++)
            {
                row = mysql_fetch_row(res);
                sprintf(tmp,"%s  %s  %s  %s  %s",row[0], row[1], row[2], row[3], row[4]);
                strcpy(tmp, real_result[i]);
            }
            mysql_free_result(res);
            mysql_close(mysql);
            return real_result;
        }
        else
        {
			g_print("shit shit\n");
            mysql_close(mysql);
            strcpy("无记录！",result[0]);
            return result;
        }
    }
    else
    {
		g_print("123456\n");
        strcpy("数据库连接失败！", result[0]);
        return result;
    }
}
