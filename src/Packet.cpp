/*********************************************************
ģ����       : 
�ļ���       :Packet.cpp
����ļ�     :Packet.hpp
�ļ�ʵ�ֹ��� :�ͻ������ݰ���
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "Packet.hpp"
#include "NDFunc.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////
//���ݰ�����                 
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CPacket::CPacket()
{
	memset(&mRcvNP,0,sizeof(SNetParam));
	memset(&mSndNP,0,sizeof(SNetParam));
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CPacket::~CPacket()
{

}

/*********************************************************
����˵����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CPacket::CheckPkt()
{
	return true;
}

/*********************************************************
����˵�����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CPacket::AnalysePkt()	
{
	return true;
}

/*********************************************************
����˵�����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CPacket::DealWithPkt()	
{
	return true;
}

/*********************************************************
����˵�����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CPacket::DisposePkt()	
{
	//AfxWriteDebugLog(">>>RECV PKT BEGIN CHECK");
	if (!CheckPkt()) 
	{
		AfxWriteDebugLog(">>>RECV PKT CHECK FAIL");
		return false;
	}
	//AfxWriteDebugLog(">>>RECV PKT BEGIN ANALYSE");
	if (!AnalysePkt()) 
	{
		AfxWriteDebugLog(">>>RECV PKT ANALYSE FAIL");
		return false;
	}
	//AfxWriteDebugLog(">>>RECV PKT BEGIN DEAL WITH");
	if (!DealWithPkt()) 
	{
		AfxWriteDebugLog(">>>RECV PKT DEAL WITH FAIL");
		return false;
	}

	return true;
}

/*********************************************************
����˵������¡���ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
CPacket *CPacket::Clone()
{
	CPacket *NewPkt = new CPacket();

	*NewPkt = *this;

	return NewPkt;
}

/*********************************************************
����˵������ȡ���������Ϣ
���˵����
����˵����
����ֵ  ��
*********************************************************/
SNetParam CPacket::GetRcvNetParam()
{
	return mRcvNP;
}

/*********************************************************
����˵�������ý������������Ϣ
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CPacket::SetRcvNetParam(const SNetParam &sNP)
{
	mRcvNP = sNP;
}

/*********************************************************
����˵�������÷������������Ϣ
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CPacket::SetSndNetParam(const SNetParam &sNP)
{
	mSndNP = sNP;
}

/*********************************************************
����˵�����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CPacket::SetRcvPktBuf(char *buf,int len)
{
	mRcvBuf.append(buf, len);
}

/*********************************************************
����˵��������m_SndNP�����srcip,srcport,desip,desport
		  ��Ϣ����Ӧ�ò����ݵķ���,������ͨUDP��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CPacket::SendTo()	
{
	struct sockaddr_in sSA,sSO;

	int iSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (iSock == -1)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Send UDP Pkt Create Socket Error");
		return false;
	}
	sSA.sin_family = AF_INET;
	sSA.sin_port = mSndNP.uSrcPort;
	sSA.sin_addr.s_addr = mSndNP.lSrcIP;
	bzero(&(sSA.sin_zero), 8);
	if(bind(iSock, (struct sockaddr *)&sSA, sizeof(struct sockaddr_in))<0) 
	{
		close(iSock);	
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Send UDP Pkt Bind Socket Error");
		return false;
	}

	sSO.sin_family = AF_INET; 
	memset(&(sSO.sin_zero),0,sizeof(sSO.sin_zero));
	sSO.sin_port = mSndNP.uDesPort;
	sSO.sin_addr.s_addr = mSndNP.lDesIP;

	int len = sendto(iSock, mSendBuf.c_str(), mSendBuf.length(), 0, (struct sockaddr*)&sSO,sizeof(struct sockaddr_in));

	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Send UDP Pkt TO DesIP=[%s] PORT=[%d] LEN=[%d]",
						mSndNP.lDesIP,
						mSndNP.uDesPort,
						len);

	close(iSock);

	if (len == -1) return false;
	return true;
}

