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
static const char *dbName = "chatMysql";
//登陆查询
int selectpsd(char *uname, char *upsd)
{
    MYSQL *mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int rownum = 0;
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
        if((rownum = mysql_num_rows(res)))
            {
                row = mysql_fetch_row(res);
                if(strcmp(row[1], upsd) == 0)
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
            mysql_free_result(res);
            mysql_close(mysql);
            return -5;//账户不存在
        }
    }
    else
	return -3;//连接数据库失败
}
