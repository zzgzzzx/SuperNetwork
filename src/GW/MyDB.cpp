/************************************************************************* 
    > File Name: MyDB.cpp 
    > Author: SongLee 
    > E-mail: lisong.shine@qq.com 
    > Created Time: 2014年05月04日 星期日 23时37分08秒 
    > Personal Blog: http://songlee24.github.io 
 ************************************************************************/  
#include<iostream>  
#include<cstdlib>  
#include "NDFunc.hpp"
#include "MyDB.hpp"  
using namespace std;  
  
MyDB::MyDB()  
{  
#ifdef GENERAL_NODE_USER_APP
   	connection = mysql_init(NULL); 
    if(connection == NULL)  
    {  
		AfxWriteDebugLog("SuperVPN run at [MyDB::MyDB] mysql_init error=[%s]", mysql_error(connection));
    }
#endif
}  
  
MyDB::~MyDB()  
{  
#ifdef GENERAL_NODE_USER_APP
    if(connection != NULL)
    {  
        mysql_close(connection);  
    }
#endif
}  
  
bool MyDB::initDB(string host, string user, string pwd, string db_name)  
{  
#ifdef GENERAL_NODE_USER_APP
    connection = mysql_real_connect(connection, host.c_str(),  
            user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0);  
    if(connection == NULL)  
    {  
        AfxWriteDebugLog("SuperVPN run at [MyDB::initDB] mysql_real_connect error=[%s]", mysql_error(connection)); 
    }
#endif
    return true;  
}  
  
bool MyDB::exeSQL(string sql)  
{  
#ifdef GENERAL_NODE_USER_APP
    if(mysql_query(connection, sql.c_str()))  
    {  
        AfxWriteDebugLog("SuperVPN run at [MyDB::exeSQL] exeSQL error=[%s]", mysql_error(connection));  
    }  
    else  
    {  
        result = mysql_use_result(connection);
        for(int i=0; i < mysql_field_count(connection); ++i)  
        {  
            row = mysql_fetch_row(result);  
            if(row <= 0)  
            {  
                break;  
            }  
            for(int j=0; j < mysql_num_fields(result); ++j)  
            {  
                cout << row[j] << " ";  
            }  
            cout << endl;  
        }  
        mysql_free_result(result);  
    }
#endif
    return true;  
}  

bool MyDB::GetIndetifyMac(list<SBindInform> &ltSI)
{
#ifdef GENERAL_NODE_USER_APP
	ndString host;
	SBindInform sBI;
	
	if (!AfxGetGatewayName(host))
	{
		AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] Get Host name error");
		return false;
	}

	ndString sql="select DeviceFlag DeviceMac from T_DeviceInform where GateWay=" + host;

    if(mysql_query(connection, sql.c_str()))  
    {  
        AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] query error=[%s]", mysql_error(connection));  
		return false;
    }  
    else  
    {  
        result = mysql_use_result(connection);
        for(int i=0; i < mysql_field_count(connection); ++i)  
        {  
            row = mysql_fetch_row(result);  
            if(row <= 0)  
            {  
                break;  
            }  
            for(int j=0; j < mysql_num_fields(result); ++j)  
            {  
            	sBI.sDeviceFlag = row[0];
                sBI.sDeviceMac = row[1];
				ltSI.push_back(sBI);
            }  
        }  
        mysql_free_result(result);  
    }
#endif
	return true;
}


