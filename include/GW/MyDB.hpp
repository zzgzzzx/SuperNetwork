/************************************************************************* 
    > File Name: MyDB.h 
    > Author: SongLee 
    > E-mail: lisong.shine@qq.com 
    > Created Time: 2014年05月04日 星期日 23时25分50秒 
    > Personal Blog: http://songlee24.github.io 
 ************************************************************************/  
#ifndef __MYBD_H__  
#include<iostream>  
#include<string>  
#ifdef GENERAL_NODE_USER_APP
#include<mysql/mysql.h>  
#endif
#include"Inform.hpp"

using namespace std;  
  
class MyDB  
{  
public:  
    MyDB();  
    ~MyDB();  
    bool initDB(string host, string user, string pwd, string db_name);  
    bool exeSQL(string sql);  

	bool GetIndetifyMac(list<SBindInform> &ltSI);
private:  
	#ifdef GENERAL_NODE_USER_APP
    MYSQL *connection;  
    MYSQL_RES *result;  
    MYSQL_ROW row;  
	#endif
};  
  
#endif 

