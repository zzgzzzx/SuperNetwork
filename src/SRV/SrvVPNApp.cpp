/*********************************************************
模块名       : 
文件名       :BaseApp.cpp
相关文件     :BaseApp.hpp
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "SrvVPNApp.hpp"
#include "NDFunc.hpp"
#include "NodeSrv.hpp"
#include "HttpRunEnvCKSrv.hpp"
#include "SrvHelloThread.hpp"

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CSrvVPNApp::CSrvVPNApp()
{
	mPNode = new CNodeSrv();
	mPHelloSrv = new CSrvHelloThread();
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CSrvVPNApp::~CSrvVPNApp()
{
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSrvVPNApp::ShowVersion()
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] services node ver=[%d]", SUPER_VPN_CLIENT_VER_SERVER);
}

/*********************************************************
函数说明：获取设备类型
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndString CSrvVPNApp::GetDeviceType()
{
	return DEVICE_TYPE_KENBAO;
}

/*********************************************************
函数说明：系统运行环境检测(包括edge\iptable\node-version)
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndStatus CSrvVPNApp::RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade)
{
	CHttpRunEvnCKSrv httpRunEnvCK(mPNode);
	return httpRunEnvCK.BeginCheck(appname, ifOnlyCheckUpgrade);
}

/*********************************************************
函数说明：系统数据初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CSrvVPNApp::InitSystem(char *appname, bool ifOnlyCheckUpgrade)
{
	if (!CSuperVPNApp::InitSystem(appname,ifOnlyCheckUpgrade)) return false;
	
	//定时重启检测
	if(mPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, mPNode->GetNodeInform().lRestartTime, NodeRestartFunc);	
	else
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, 24*3600, NodeRestartFunc);	

	return true;
}


