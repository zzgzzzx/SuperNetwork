/*********************************************************
ģ����       : 
�ļ���       :BaseApp.cpp
����ļ�     :BaseApp.hpp
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "UserVPNApp.hpp"
#include "NDFunc.hpp"
#include "NodeUser.hpp"
#include "HttpRunEnvCKUser.hpp"

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CUserVPNApp::CUserVPNApp()
{
	mPNode = new CNodeUser();
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CUserVPNApp::~CUserVPNAppp()
{

}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CUserVPNApp::ShowVersion()
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] client node ver=[%d]", SUPER_VPN_CLIENT_VER_NODE);
}

/*********************************************************
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CUserVPNApp::InitSystem(char *appname, bool ifOnlyCheckUpgrade)
{
	CSuperVPNApp::InitSystem(appname,ifOnlyCheckUpgrade);
	
	//����http����	
	if (mHttpSrv.Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START WORKING...");	

	return true;
}


/*********************************************************
����˵����ϵͳ���л������(����edge\iptable\node-version)
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CUserVPNApp::RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade)
{
	CHttpRunEvnCKUser httpRunEnvCK(mPNode);
	return httpRunEnvCK.BeginCheck(appname, ifOnlyCheckUpgrade);
}


