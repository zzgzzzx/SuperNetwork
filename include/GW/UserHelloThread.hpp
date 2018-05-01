/****************** Copyright (c) 2008 **************
模块名       : Hello服务处理类
文件名       : HelloSrv.hpp
相关文件	    : HelloSrv.cpp
文件实现功能 : 接收处理Hello Server发送的消息
作者         : zhangzx 
版本         : 1.0 
**********************************************************/
#ifndef __USER_HELLOSRV_HPP__
#define __USER_HELLOSRV_HPP__

#include "HelloSrvThread.hpp"
#include "NDStruct.hpp"

class CUserHelloThread: public CHelloSrvThread
{
private:
	//节点hello
	static void NodeHelloFunc(ndULong param);		
protected:
	//线程处理函数
	void ProcessVirtual(void);	
	CPacket *GetNewPkt();
public:
	//构造函数
	CUserHelloThread();
	//析构函数
	~CUserHelloThread();
};

#endif
