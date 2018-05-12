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
#include "MyDB.hpp"
#include "HttpRunEnvCKUser.hpp"

extern CSuperVPNApp *gPSuperVPNApp;

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CUserVPNApp::CUserVPNApp()
{
	mPNode = new CNodeUser();

	MyDB myDB;
	myDB.IniFileCheck();
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
	//if (!CSuperVPNApp::InitSystem(ifOnlyCheckUpgrade)) return false;

	mARPSet.InitIdentifyFromGW();
	
	//����http����	
	if (mHttpSrv.Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START WORKING...");	

	AfxInsertCircleTimer(TIMER_ID_NODE_ARP_CHECK, VALUE_ARP_CHECK_TIME, ArpCheckFunc);

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
����˵������ȡARP����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CARPSet *CUserVPNApp::GetArpSet()
{
	return &mARPSet;
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

/*********************************************************
����˵����arp���
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CUserVPNApp::ArpCheckFunc(ndULong param)
{
	CUserVPNApp *pSuerVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp);
	if(pSuerVPNApp == NULL) return;

	pSuerVPNApp->GetArpSet()->ARPCheck();
}

