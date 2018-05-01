/*********************************************************
ģ����       : 
�ļ���       :UserHelloThread.cpp
����ļ�     :UserHelloThread.hpp
�ļ�ʵ�ֹ��� :Hello���������ʵ���ļ�
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "UserHelloThread.hpp"
#include "NDFunc.hpp"
#include "HelloPkt.hpp"

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CUserHelloThread::CUserHelloThread()
{
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CUserHelloThread::~CUserHelloThread()
{
}

/*********************************************************
����˵�����̴߳�����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CUserHelloThread::ProcessVirtual(void)
{
	//��������
	if (!StartService())
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Hello Rcv Server Start Error!!!\n");
		return;
	}

	//���Ӷ�ʱHello	
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, TIMER_VALUE_NODE_HELLO_CHECK, NodeHelloFunc);

	//�������ݰ�������
	while(true)
	{
		RcvAndDealMsg();
	}
}

/*********************************************************
����˵���������°�
���˵����
����˵����
����ֵ  ��
*********************************************************/
CPacket* CUserHelloThread::GetNewPkt()
{
	return new CUserHelloPkt();
}


/*********************************************************
����˵����Hello�Ķ�ʱ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CUserHelloThread::NodeHelloFunc(ndULong param)
{
	//need update by lewis
	//AfxGetIdentifySet()->SendHelloAndCheck();
}

