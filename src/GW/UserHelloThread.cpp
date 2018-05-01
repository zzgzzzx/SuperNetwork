/*********************************************************
模块名       : 
文件名       :UserHelloThread.cpp
相关文件     :UserHelloThread.hpp
文件实现功能 :Hello服务包处理实现文件
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "UserHelloThread.hpp"
#include "NDFunc.hpp"
#include "HelloPkt.hpp"

/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CUserHelloThread::CUserHelloThread()
{
}

/*********************************************************
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CUserHelloThread::~CUserHelloThread()
{
}

/*********************************************************
函数说明：线程处理函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CUserHelloThread::ProcessVirtual(void)
{
	//启动服务
	if (!StartService())
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Hello Rcv Server Start Error!!!\n");
		return;
	}

	//增加定时Hello	
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, TIMER_VALUE_NODE_HELLO_CHECK, NodeHelloFunc);

	//接收数据包并处理
	while(true)
	{
		RcvAndDealMsg();
	}
}

/*********************************************************
函数说明：创建新包
入参说明：
出参说明：
返回值  ：
*********************************************************/
CPacket* CUserHelloThread::GetNewPkt()
{
	return new CUserHelloPkt();
}


/*********************************************************
函数说明：Hello的定时器函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CUserHelloThread::NodeHelloFunc(ndULong param)
{
	//need update by lewis
	//AfxGetIdentifySet()->SendHelloAndCheck();
}

