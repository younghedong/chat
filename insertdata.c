#include <stdio.h>
#include <stdlib.h>
#include<mysql.h>
#include<string.h>
#define true 1
#define false 0
const char *host = "localhost";
const char *user = "root";
const char *psd = "root";
unsigned int port = 3306;

int insertUser(char *uname, char *upsd)
{
    MYSQL *mysql;
    MYSQL_RES *res;

    //sql语句
    char *createdb = "create database chatMysql";
    char *createtable = "create table user (userName char(15) not null primary key, userPsd char(20) not null)";
    char *selectusertable = (char *)malloc(1024);
    char *query = (char *)malloc(1024);
    sprintf(query, "insert into user values('%s', '%s')", uname, upsd);
    sprintf(selectusertable, "select userName from user where userName = '%s'", uname);

    //mysql初始化
    if(!(mysql = mysql_init(NULL)))
    {
        return false;
    }

    //判断数据库不存在，创建数据库和表
    /*if(mysql_select_db(mysql, "chatMysql") != 0 )
    {
        mysql_real_query(mysql, createdb, strlen(createdb));
        mysql_real_query(mysql, createtable, strlen(createtable));
    }
    */
    //注册查询是否合法
    if(mysql_real_connect(mysql, host, user, psd, "chatMysql", port, NULL, 0))
    {
        mysql_real_query(mysql, selectusertable, strlen(selectusertable));
        res = mysql_store_result(mysql);
        if(res)//账号存在，不能注册
        {
            mysql_free_result(res);
            mysql_close(mysql);
            return false;
        }
        if(mysql_real_query(mysql, query, strlen(query)) ==0)//账号不存在，执行sql插入语句
        {
            mysql_close(mysql);
            return true;
        }
    }
}
