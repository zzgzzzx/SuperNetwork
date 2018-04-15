/****************** Copyright (c) 2008 **************
模块名       : Http服务数据据接收处理类
文件名       : HttpSrv.hpp
相关文件	    : HttpSrv.cpp
文件实现功能 : 接收处理Http Server发送的消息
作者         : zhangzx 
版本         : 1.0 
**********************************************************/
#ifndef __HTTP_SRV_HPP__
#define __HTTP_SRV_HPP__

#include "BThread.hpp"
#include "Udp.hpp"

class CHttpSrvThread: public CBaseThread
{
private:
	//是否需要退出标识
	ndBool mStopRun;	
	//启动服务socket
	bool StartService();
protected:
	//线程处理函数
	virtual void ProcessVirtual(void);
public:
	//构造函数
	CHttpSrvThread();
	//析构函数
	~CHttpSrvThread();
	//发送数据包
	int SendTo(ndUInt32 IP,ndInt16 iPort,char *pBuf,int iBufLen);
};

#endif
