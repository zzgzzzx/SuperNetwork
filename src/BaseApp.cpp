/*********************************************************
ģ����       : 
�ļ���       :BaseApp.cpp
����ļ�     :BaseApp.hpp
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CBaseApp::Run(int argc,char *argv[])
{
	//�߳�����
	mTimer.Start();
	//�����������
	InitApplication(argc, argv);
	AfxWriteDebugLog("SuperVPN run at [CBaseApp::Run] APP RUN END......");
}

/*********************************************************
����˵�������뵥�ض�ʱ��
���˵����
����˵����
����ֵ  ��timeout(����)
*********************************************************/
bool CBaseApp::InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	if (mTimer.InsertSingleTimer(callid,timeout,CallBack) == 0)
		return true;
	else
		return false;
}

/*********************************************************
����˵��������ѭ����ʱ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CBaseApp::InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	if (mTimer.InsertCircleTimer(callid,timeout,CallBack) == 0)
		return true;
	else
		return false;
}


/*********************************************************
����˵����ɾ��ѭ����ʱ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CBaseApp::DeleteCircleTimer(unsigned long callid)
{
	if (mTimer.DeleteCircleTimer(callid) == 0)
		return true;
	else
		return false;
}

/*********************************************************
����˵����ɾ��������ʱ��
���˵����callid����ʱ��ID
����˵����
����ֵ  ��
*********************************************************/
bool CBaseApp::DeleteSingleTimer(unsigned long callid)
{
	if (mTimer.DeleteSingleTimer(callid) == 0)
		return true;
	else
		return false;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CBaseApp::InitApplication(int argc,char *argv[])
{
	return true;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CBaseApp::CBaseApp()
{

}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CBaseApp::~CBaseApp()
{

}
