/****************** Copyright (c) 2017 **************
模块名       : 
文件名       :CUserVPNApp.hpp
相关文件     : 
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __USER_VPN_APP_HPP__
#define __USER_VPN_APP_HPP__

#include "BaseApp.hpp"

//**********************************
//CSrvVPNApp工程类                       
//**********************************
class CUserVPNApp: public CSuperVPNApp
{
public:
		
	//系统数据初始化
	bool InitSystem(char *appname, bool ifOnlyCheckUpgrade);
	
	//显示版本信息
	virtual void ShowVersion();
	//系统运行环境检测
	ndStatus RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade);	

	//构造函数
	CUserVPNApp();
	//析构函数
	~CUserVPNApp();		
};

#endif
