/****************** Copyright (c) 2017 **************
模块名       : 
文件名       :CSrvVPNApp.hpp
相关文件     : 
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __SRV_VPN_APP_HPP__
#define __SRV_VPN_APP_HPP__

#include "BaseApp.hpp"

//**********************************
//CSrvVPNApp工程类                       
//**********************************
class CSrvVPNApp: public CSuperVPNApp
{
public:
	//显示版本信息
	virtual void ShowVersion();
	//系统运行环境检测
	ndStatus RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade);	
	//获取设备类型
	ndString GetDeviceType();	

	//构造函数
	CSrvVPNApp();
	//析构函数
	~CSrvVPNApp();		
};

#endif
