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
#include "IniClass.hpp"

using namespace std;  

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
MyDB::MyDB()  
{  
   	connection = mysql_init(NULL); 
    if(connection == NULL)  
    {  
		AfxWriteDebugLog("SuperVPN run at [MyDB::MyDB] mysql_init error=[%s]", mysql_error(connection));
    }
}  

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/  
MyDB::~MyDB()  
{  
    if(connection != NULL)
    {  
        mysql_close(connection);  
    }
}  

/*********************************************************
函数说明：Ini文件检测
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool MyDB::IniFileCheck()
{
	FILE *pFile;
	bool bMustUpdateFile = false;

	if(access(DB_INI_FILE_NAME, NULL) != 0)  
	{  
		if ((pFile = fopen(DB_INI_FILE_NAME, "w+")) == NULL)
		{
			AfxWriteDebugLog("SuperVPN run at [MyDB::IniFileCheck] Create [%s] File Fail", DB_INI_FILE_NAME);
			return false;
		}	
		fclose(pFile);
	} 

	CIniDoc iniDoc;
	iniDoc.LoadFromFile(DB_INI_FILE_NAME);
	if(!iniDoc.ifExistsItem(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_HOST))
	{
		iniDoc.PutItemString(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_HOST, "127.0.0.1");
		bMustUpdateFile = true;
	}

	if(!iniDoc.ifExistsItem(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_USER))
	{
		iniDoc.PutItemString(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_USER, "root");
		bMustUpdateFile = true;
	}	

	if(!iniDoc.ifExistsItem(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_PWD))
	{
		iniDoc.PutItemString(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_PWD, "123456");
		bMustUpdateFile = true;
	}

	if(!iniDoc.ifExistsItem(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_DB))
	{
		iniDoc.PutItemString(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_DB, "SuperVPN");
		bMustUpdateFile = true;
	}	

	if(bMustUpdateFile) iniDoc.SaveToFile(DB_INI_FILE_NAME);		
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool MyDB::initDB()  
{  
	CIniDoc iniDB;
	iniDB.LoadFromFile(DB_INI_FILE_NAME);
	
    connection = mysql_real_connect(connection, 
						iniDB.GetItemString(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_HOST),  
						iniDB.GetItemString(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_USER), 
						iniDB.GetItemString(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_PWD), 
						iniDB.GetItemString(DB_INI_FILE_SECT_SETTINGS, DB_INI_FILE_SECT_ITEM_DB),
						0, NULL, 0);  
    if(connection == NULL)  
    {  
        AfxWriteDebugLog("SuperVPN run at [MyDB::initDB] mysql_real_connect error=[%s]", mysql_error(connection)); 
		return false;
    }

    return true;  
}  

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/  
bool MyDB::exeSQL(string sql)  
{  
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

    return true;  
}  

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/  
bool MyDB::GetIndetifyMac(list<SBindInform> &ltSI)
{
	ndString host;
	SBindInform sBI;

	AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] begin GetIndetifyMac");
	
	if (!AfxGetGatewayName(host))
	{
		AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] Get Host name error");
		return false;
	}

	ndString sql="select DeviceFlag,DeviceMac from T_DeviceInform where GateWay='" + host+"'";
	AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] query SQL=[%s]", sql.c_str());

    if(mysql_query(connection, sql.c_str()))  
    {  
        AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] query error=[%s]", mysql_error(connection));  
		return false;
    }  
    else  
    {  
        result = mysql_store_result(connection);
		if (NULL == result)  
	    {  
		    AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] mysql_store_result is null");
	        return false;
	    } 
		
		while (row = mysql_fetch_row(result))  
        {  
            sBI.sDeviceFlag = row[0];
            sBI.sDeviceMac = row[1];
			AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] flag=[%s] mac=[%s]", row[0], row[1]);
			ltSI.push_back(sBI);  
        } 
        mysql_free_result(result);  
    }
	AfxWriteDebugLog("SuperVPN run at [MyDB::GetIndetifyMac] end GetIndetifyMac");

	return true;
}


