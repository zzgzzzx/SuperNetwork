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
CUserVPNApp::~CUserVPNApp()
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
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] client node ver=[%d]", SUPER_VPN_CLIENT_VER_USER);
}

/*********************************************************
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CUserVPNApp::InitSystem(bool ifOnlyCheckUpgrade)
{
	if (!CSuperVPNApp::InitSystem(ifOnlyCheckUpgrade)) return false;
	
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
ndStatus CUserVPNApp::RunEnvCheck(bool ifOnlyCheckUpgrade)
{
	CHttpRunEvnCKUser httpRunEnvCK(mPNode);
	return httpRunEnvCK.BeginCheck(ifOnlyCheckUpgrade);
}

/*********************************************************
����˵������ȡ����б�
���˵����
����˵����
����ֵ  ��
*********************************************************/
CIdentifySet *CUserVPNApp::GetIdentifySet()
{
	return &mIdentifySet;
}

/*********************************************************
����˵������ȡ�豸����
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndString CUserVPNApp::GetDeviceType()
{
	return "";
}


