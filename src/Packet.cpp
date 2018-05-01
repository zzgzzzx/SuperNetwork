/*********************************************************
模块名       : 
文件名       :Packet.cpp
相关文件     :Packet.hpp
文件实现功能 :客户端数据包类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "Packet.hpp"
#include "NDFunc.hpp"

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CPacket::CPacket()
{
	memset(&mRcvNP,0,sizeof(SNetParam));
	memset(&mSndNP,0,sizeof(SNetParam));
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CPacket::~CPacket()
{

}

/*********************************************************
函数说明：检测数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CPacket::CheckPkt()
{
	return true;
}

/*********************************************************
函数说明：分析数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CPacket::AnalysePkt()	
{
	return true;
}

/*********************************************************
函数说明：处理数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CPacket::DealWithPkt()	
{
	return true;
}

/*********************************************************
函数说明：处理数据包
入参说明：
出参说明：
返回值  ：
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
函数说明：克隆数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
CPacket *CPacket::Clone()
{
	CPacket *NewPkt = new CPacket();

	*NewPkt = *this;

	return NewPkt;
}

/*********************************************************
函数说明：获取网络参数信息
入参说明：
出参说明：
返回值  ：
*********************************************************/
SNetParam CPacket::GetRcvNetParam()
{
	return mRcvNP;
}

/*********************************************************
函数说明：设置接收网络参数信息
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CPacket::SetRcvNetParam(const SNetParam &sNP)
{
	mRcvNP = sNP;
}

/*********************************************************
函数说明：设置发送网络参数信息
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CPacket::SetSndNetParam(const SNetParam &sNP)
{
	mSndNP = sNP;
}

/*********************************************************
函数说明：设置数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CPacket::SetRcvPktBuf(char *buf,int len)
{
	mRcvBuf.append(buf, len);
}

/*********************************************************
函数说明：根据m_SndNP里面的srcip,srcport,desip,desport
		  信息进行应用层数据的发包,发送普通UDP包
入参说明：
出参说明：
返回值  ：
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

