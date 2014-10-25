#include<stdio.h>
#include<stdlib.h>
#include<mysql.h>
#include<string.h>
#define true 1
#define false 0

static const char *host = "localhost";
static const char *user = "root";
static const char *psd = "root";
static unsigned int port = 3306;
//登陆查询
int selectpsd(char *dbName, char *uname, char *upsd)
{
    MYSQL *mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *query = (char *)malloc(1024); //记录查询语句

    sprintf(query, "select * from user where userName = '%s'", uname);//查询语句赋值

    //mysql初始化
    if(!(mysql = mysql_init(NULL)))
    {
        return false;
    }

    //判断数据库是否存在
    if(mysql_select_db(mysql, dbName) != 0)
    {
        return false;
    }
    //数据查询
    if(mysql_real_connect(mysql, host, user, psd, dbName, port, NULL, 0))
    {
        mysql_real_query(mysql, query, strlen(query));
        res = mysql_store_result(mysql);
        if(res)
            {
                row = mysql_fetch_row(res);
                if(row[1] == upsd)
                {
                    mysql_free_result(res);
                    mysql_close(mysql);
                    return true;
                }
                else
                {
                    mysql_free_result(res);
                    mysql_close(mysql);
                    return false;
                }
            }
        else
        {
            mysql_free_result(res);
            mysql_close(mysql);
            return false;
        }
    }
}
