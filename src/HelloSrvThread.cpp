/*********************************************************
ģ����       : 
�ļ���       :HelloSrvThread.cpp
����ļ�     :HelloSrvThread.hpp
�ļ�ʵ�ֹ��� :Hello���������ʵ���ļ�
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "HelloSrvThread.hpp"
#include "NDFunc.hpp"
#include "HelloPkt.hpp"

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHelloSrvThread::CHelloSrvThread()
{
	mBRun = ND_FALSE;
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHelloSrvThread::~CHelloSrvThread()
{
}

/*********************************************************
����˵�����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
int CHelloSrvThread::SendTo(ndUInt32 IP, ndInt16 iPort, const char *pBuf, int iBufLen)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Send Pkt To Hello Server SrvIP=[%s] port=[%d]!",AfxHostIPToStr(IP),iPort);
	return mSrvSkt.SendTo(IP, iPort, pBuf, iBufLen);
}

/*********************************************************
����˵�����̴߳�����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CHelloSrvThread::ProcessVirtual(void)
{
	//��������
	if (!StartService())
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Hello Rcv Server Start Error!!!\n");
		return;
	}

	mBRun = ND_TRUE;

	//�������ݰ�������
	while(true)
	{
		RcvAndDealMsg();
	}
}

/*********************************************************
����˵����hello�߳��Ƿ��Ѿ�����
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool CHelloSrvThread::IfAlreadyRun()
{
	return mBRun;
}

/*********************************************************
����˵������������socket
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CHelloSrvThread::StartService()
{
	if (mSrvSkt.Open(HELLO_SRV_LOCAL_PORT) == -1) return false;

	return true;
}

/*********************************************************
����˵�������ղ���������(ip��ַ����������)
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CHelloSrvThread::RcvAndDealMsg()
{
	char sBuff[PACKET_BUFF_LEN];
	int iRcvLen;
	ndUInt32 ip;
	ndUInt16 port;
	SNetParam sNP;	

	iRcvLen = mSrvSkt.RecvFrom((ndUChar*)sBuff, PACKET_BUFF_LEN, ip, port);

	if (iRcvLen <= 0) return;
	
	sNP.lSrcIP = ip;
	sNP.uSrcPort = port;

	CPacket *pkt = GetNewPkt();
	//���ý��հ�������
	pkt->SetRcvPktBuf((char*)sBuff, iRcvLen);
	//���ý��հ����������
	pkt->SetRcvNetParam(sNP);
	//�����ݰ�����ϵͳ����(Ҳ�����ڴ�ֱ�Ӵ���)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Recv Hello Pkt IP=[%s]", AfxNetIPToStr(ip));	
	AfxInsertPktToSysQue(pkt);	
}

