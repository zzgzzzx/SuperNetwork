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
#include "HttpSrvThread.hpp"
#include "IdentifySet.hpp"
#include "ARPSet.hpp"

//**********************************
//CSrvVPNApp工程类                       
//**********************************
class CUserVPNApp: public CSuperVPNApp
{
private:
	//http服务
	CHttpSrvThread mHttpSrv;
	//身份识别管理类
	CIdentifySet mIdentifySet;
	//ARP列表管理类
	CARPSet mARPSet;
public:

	//系统数据初始化
	bool InitSystem(bool ifOnlyCheckUpgrade);
	
	//显示版本信息
	virtual void ShowVersion();
	//系统运行环境检测
	ndStatus RunEnvCheck(bool ifOnlyCheckUpgrade);	
	//获取身份列表
	CIdentifySet *GetIdentifySet();
	//获取ARP列表
	CARPSet *GetArpSet();
	//获取设备类型
	ndString GetDeviceType();
	//arp检测
	static void ArpCheckFunc(ndULong param);		

	//构造函数
	CUserVPNApp();
	//析构函数
	~CUserVPNApp();		
};

#endif
