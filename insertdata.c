#include <stdio.h>
#include <stdlib.h>
#include<mysql.h>
#include<string.h>
#include<time.h>
#include"encrypt.h"

const char *host = "localhost";
const char *user = "root";
const char *psd = "root";
unsigned int port = 3306;

int insertUser(char *uname, char *upsd)//注册，用户表，插入
{
    MYSQL *mysql;
    MYSQL_RES *res;
    int rownum = 0;
    char enstring[1024] = "\0";

    //字符串加密
    Encrypt(upsd, enstring);

    //sql语句
    char *createdb = "create database chatMysql if not exist 'chatMysql'";
    char *createtable = "create table user (userName char(15) not null primary key, userPsd char(20) not null)";
    char *selectusertable = (char *)malloc(1024);
    char *query = (char *)malloc(1024);
    sprintf(query, "insert into user values('%s', '%s')", uname, enstring);
    sprintf(selectusertable, "select userName from user where userName = '%s'", uname);

    //mysql初始化
    if(!(mysql = mysql_init(NULL)))
    {
        return -1;
    }

    //判断数据库不存在，创建数据库和表
    if((mysql_real_connect(mysql, host, user, psd, "mysql", port, NULL, 0)))
    {
         mysql_query(mysql, "create database if not exists chatMysql");
         mysql_query(mysql, "use chatMysql");
         mysql_query(mysql, "create table if not exists user(userName char(15) not null primary key, userPsd char(20) not null)");
    }
    //注册查询是否合法
    if(mysql_real_connect(mysql, host, user, psd, "chatMysql", port, NULL, 0))
    {

        mysql_real_query(mysql, selectusertable, strlen(selectusertable));
        res = mysql_store_result(mysql);
        if((rownum = mysql_num_rows(res)))//账号存在，不能注册
        {
            mysql_free_result(res);
            mysql_close(mysql);
            return -2;
        }
        if(mysql_real_query(mysql, query, strlen(query)) == 0)//账号不存在，执行sql插入语句
        {
            mysql_close(mysql);
            return 0;
        }
    }
    else 
	return -3;//连接数据库失败
}


int insert_chat_data(int num, char *send, char *accept, char *chatContent)
{
    MYSQL *mysql;
    MYSQL_RES *res;
    time_t timep;
    char *nowtime = "\0";
    char *belong = "\0";
    char *insertquery = (char *)malloc(1024);
    
    if(num == 0)
        belong = "client";
    else
        belong = "server";
    
    time(&timep);
    nowtime = ctime(&timep);
    sprintf(insertquery, "insert into chatRecord values(%s, %s, %s, %s, %s)", belong, send, accept, chatContent, nowtime);
    
    //mysql初始化
    if(!(mysql = mysql_init(NULL)))
    {
        return -1;
    }

    if(mysql_real_connect(mysql, host, user, psd, "chatMysql", port, NULL, 0))
    {
        mysql_query(mysql,"create table if not exists chatRecord(belong char(10) not null, send char(15) not null, accept char(15) not null, content char(200), time char(50))");
        if((mysql_real_query(mysql, insertquery, strlen(insertquery))) == 0)
        {
            mysql_close(mysql);
            return 0;
        }
	else
	{
	    mysql_close(mysql);
	    return -7;//数据库操作失败
	}
    }
    else
	return -3;//连接数据库失败
}	
