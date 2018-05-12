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
	AfxWriteDebugLog("SuperVPN run at [CSrvVPNApp::ShowVersion] services node ver=[%d]", SUPER_VPN_CLIENT_VER_SERVER);
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
ndStatus CSrvVPNApp::RunEnvCheck(bool ifOnlyCheckUpgrade)
{
	CHttpRunEvnCKSrv httpRunEnvCK(mPNode);
	return httpRunEnvCK.BeginCheck(ifOnlyCheckUpgrade);
}

/*********************************************************
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSrvVPNApp::InitSystem(bool ifOnlyCheckUpgrade)
{
	AfxWriteDebugLog("SuperVPN run at [CSrvVPNApp::InitSystem] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	if (!CSuperVPNApp::InitSystem(ifOnlyCheckUpgrade)) return false;
	
	//��ʱ�������
	AfxWriteDebugLog("SuperVPN run at [CSrvVPNApp::InitSystem] begin add restart timer");
	if(mPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, mPNode->GetNodeInform().lRestartTime, NodeRestartFunc);	
	else
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, 24*3600, NodeRestartFunc);	
	
	AfxWriteDebugLog("SuperVPN run at [CSrvVPNApp::InitSystem] begin add iancheck timer");
	AfxInsertCircleTimer(TIMER_ID_NODE_IAN_CHECK, VALUE_IAN_CHECK_TIME, IanCheckFunc);	

	return true;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSrvVPNApp::IanCheckFunc(ndULong param)
{
	CNodeSrv *pNodeSrv = dynamic_cast<CNodeSrv*> (AfxGetVPNNode());
	pNodeSrv->IanCheck();
}

