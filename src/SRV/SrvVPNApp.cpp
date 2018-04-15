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

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSrvVPNApp::CSrvVPNApp()
{
	mPNode = new CNodeSrv();
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


