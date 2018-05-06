/*********************************************************
模块名       : 
文件名       :HelloSrvThread.cpp
相关文件     :HelloSrvThread.hpp
文件实现功能 :Hello服务包处理实现文件
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "HelloSrvThread.hpp"
#include "NDFunc.hpp"
#include "HelloPkt.hpp"

/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CHelloSrvThread::CHelloSrvThread()
{
	mBRun = ND_FALSE;
}

/*********************************************************
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CHelloSrvThread::~CHelloSrvThread()
{
}

/*********************************************************
函数说明：发送数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CHelloSrvThread::SendTo(ndUInt32 IP, ndInt16 iPort, const char *pBuf, int iBufLen)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Send Pkt To Hello Server SrvIP=[%s] port=[%d]!",AfxHostIPToStr(IP),iPort);
	return mSrvSkt.SendTo(IP, iPort, pBuf, iBufLen);
}

/*********************************************************
函数说明：线程处理函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CHelloSrvThread::ProcessVirtual(void)
{
	//启动服务
	if (!StartService())
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Hello Rcv Server Start Error!!!\n");
		return;
	}

	mBRun = ND_TRUE;

	//接收数据包并处理
	while(true)
	{
		RcvAndDealMsg();
	}
}

/*********************************************************
函数说明：hello线程是否已经运行
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndBool CHelloSrvThread::IfAlreadyRun()
{
	return mBRun;
}

/*********************************************************
函数说明：启动服务socket
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CHelloSrvThread::StartService()
{
	if (mSrvSkt.Open(HELLO_SRV_LOCAL_PORT) == -1) return false;

	return true;
}

/*********************************************************
函数说明：接收并处理数据(ip地址都用网络序)
入参说明：
出参说明：
返回值  ：
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
	//设置接收包的内容
	pkt->SetRcvPktBuf((char*)sBuff, iRcvLen);
	//设置接收包的网络参数
	pkt->SetRcvNetParam(sNP);
	//把数据包丢进系统队列(也可以在此直接处理)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Recv Hello Pkt IP=[%s]", AfxNetIPToStr(ip));	
	AfxInsertPktToSysQue(pkt);	
}

