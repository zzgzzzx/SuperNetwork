/*********************************************************
ģ����       : 
�ļ���       :BaseApp.cpp
����ļ�     :BaseApp.hpp
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "SrvVPNApp.hpp"
#include "NDFunc.hpp"
#include "NodeSrv.hpp"
#include "HttpRunEnvCKSrv.hpp"
#include "SrvHelloThread.hpp"

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSrvVPNApp::CSrvVPNApp()
{
	mPNode = new CNodeSrv();
	mPHelloSrv = new CSrvHelloThread();
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSrvVPNApp::~CSrvVPNApp()
{
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSrvVPNApp::ShowVersion()
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] services node ver=[%d]", SUPER_VPN_CLIENT_VER_SERVER);
}

/*********************************************************
����˵������ȡ�豸����
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndString CSrvVPNApp::GetDeviceType()
{
	return DEVICE_TYPE_KENBAO;
}

/*********************************************************
����˵����ϵͳ���л������(����edge\iptable\node-version)
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CSrvVPNApp::RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade)
{
	CHttpRunEvnCKSrv httpRunEnvCK(mPNode);
	return httpRunEnvCK.BeginCheck(appname, ifOnlyCheckUpgrade);
}

/*********************************************************
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSrvVPNApp::InitSystem(char *appname, bool ifOnlyCheckUpgrade)
{
	if (!CSuperVPNApp::InitSystem(appname,ifOnlyCheckUpgrade)) return false;
	
	//��ʱ�������
	if(mPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, mPNode->GetNodeInform().lRestartTime, NodeRestartFunc);	
	else
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, 24*3600, NodeRestartFunc);	

	return true;
}


