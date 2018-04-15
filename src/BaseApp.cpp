/*********************************************************
模块名       : 
文件名       :BaseApp.cpp
相关文件     :BaseApp.hpp
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CBaseApp::Run(int argc,char *argv[])
{
	//线程启动
	mTimer.Start();
	//主体程序运行
	InitApplication(argc, argv);
	AfxWriteDebugLog("SuperVPN run at [CBaseApp::Run] APP RUN END......");
}

/*********************************************************
函数说明：插入单回定时器
入参说明：
出参说明：
返回值  ：timeout(秒数)
*********************************************************/
bool CBaseApp::InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	if (mTimer.InsertSingleTimer(callid,timeout,CallBack) == 0)
		return true;
	else
		return false;
}

/*********************************************************
函数说明：插入循环定时器
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CBaseApp::InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	if (mTimer.InsertCircleTimer(callid,timeout,CallBack) == 0)
		return true;
	else
		return false;
}


/*********************************************************
函数说明：删除循环定时器
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CBaseApp::DeleteCircleTimer(unsigned long callid)
{
	if (mTimer.DeleteCircleTimer(callid) == 0)
		return true;
	else
		return false;
}

/*********************************************************
函数说明：删除单个定时器
入参说明：callid：定时器ID
出参说明：
返回值  ：
*********************************************************/
bool CBaseApp::DeleteSingleTimer(unsigned long callid)
{
	if (mTimer.DeleteSingleTimer(callid) == 0)
		return true;
	else
		return false;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CBaseApp::InitApplication(int argc,char *argv[])
{
	return true;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CBaseApp::CBaseApp()
{

}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CBaseApp::~CBaseApp()
{

}
