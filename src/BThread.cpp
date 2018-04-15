/***********************************************************
模块名       : 线程的基类
文件名       : Bthread.cpp
相关文件     : Bthread.hpp
文件实现功能 : 
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "BThread.hpp"

/*************************************************
函数说明：线程同步控制
参数说明：无
*************************************************/
int CBaseThread::Join(void)
{
	int iRet=-1;
	if(0==m_iState)	
	    iRet=pthread_join(m_ptThreadID,NULL);
	return iRet;
}

/*************************************************
函数说明：构造函数
参数说明：无
*************************************************/
CBaseThread::CBaseThread()
{
    m_iState=-1;	
}

/*************************************************
函数说明：析构函数
参数说明：无
*************************************************/
CBaseThread::~CBaseThread()
{	
	 
}

/*************************************************
函数说明：线程启动
参数说明：无
*************************************************/
int CBaseThread::Start(void)
{
	int iCode = pthread_create(&m_ptThreadID,NULL,
		CBaseThread::EntryPoint,this);
	m_iState=iCode;
	return iCode;
}

/*************************************************
函数说明：线程控制
参数说明：
*************************************************/
void* CBaseThread::EntryPoint(void * pArg)
{
	pthread_detach(pthread_self());

	CBaseThread* poThread=(CBaseThread*)pArg;
	poThread->ProcessVirtual();
	return pArg;
}

/*************************************************
函数说明：线程运行虚函数
参数说明：无
*************************************************/
void CBaseThread::ProcessVirtual(void)
{
}

/*************************************************
函数说明：线程运行停止前处理虚函数
参数说明：无
*************************************************/
void CBaseThread::StopPreDeal(void)
{

}

/*************************************************
函数说明：线程停止
参数说明：无
*************************************************/
void CBaseThread::Stop()
{
	StopPreDeal();
	if(0 == m_iState)	pthread_cancel(m_ptThreadID);	
}


