/*********************************************************
模块名       : 
文件名       :BaseApp.cpp
相关文件     :BaseApp.hpp
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "UserVPNApp.hpp"
#include "NDFunc.hpp"
#include "NodeUser.hpp"
#include "MyDB.hpp"
#include "HttpRunEnvCKUser.hpp"

extern CSuperVPNApp *gPSuperVPNApp;

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CUserVPNApp::CUserVPNApp()
{
	mPNode = new CNodeUser();

	MyDB myDB;
	myDB.IniFileCheck();
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CUserVPNApp::~CUserVPNApp()
{

}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CUserVPNApp::ShowVersion()
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] client node ver=[%d]", SUPER_VPN_CLIENT_VER_USER);
}

/*********************************************************
函数说明：系统数据初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CUserVPNApp::InitSystem(bool ifOnlyCheckUpgrade)
{
	//if (!CSuperVPNApp::InitSystem(ifOnlyCheckUpgrade)) return false;

	mARPSet.InitIdentifyFromGW();
	
	//启动http服务	
	if (mHttpSrv.Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START WORKING...");	

	AfxInsertCircleTimer(TIMER_ID_NODE_ARP_CHECK, VALUE_ARP_CHECK_TIME, ArpCheckFunc);

	return true;
}


/*********************************************************
函数说明：系统运行环境检测(包括edge\iptable\node-version)
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndStatus CUserVPNApp::RunEnvCheck(bool ifOnlyCheckUpgrade)
{
	CHttpRunEvnCKUser httpRunEnvCK(mPNode);
	return httpRunEnvCK.BeginCheck(ifOnlyCheckUpgrade);
}

/*********************************************************
函数说明：获取身份列表
入参说明：
出参说明：
返回值  ：
*********************************************************/
CIdentifySet *CUserVPNApp::GetIdentifySet()
{
	return &mIdentifySet;
}

/*********************************************************
函数说明：获取ARP集合
入参说明：
出参说明：
返回值  ：
*********************************************************/
CARPSet *CUserVPNApp::GetArpSet()
{
	return &mARPSet;
}

/*********************************************************
函数说明：获取设备类型
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndString CUserVPNApp::GetDeviceType()
{
	return "";
}

/*********************************************************
函数说明：arp检测
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CUserVPNApp::ArpCheckFunc(ndULong param)
{
	CUserVPNApp *pSuerVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp);
	if(pSuerVPNApp == NULL) return;

	pSuerVPNApp->GetArpSet()->ARPCheck();
}

