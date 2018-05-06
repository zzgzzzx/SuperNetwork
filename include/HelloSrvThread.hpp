/****************** Copyright (c) 2008 **************
模块名       : Hello服务处理类
文件名       : HelloSrv.hpp
相关文件	    : HelloSrv.cpp
文件实现功能 : 接收处理Hello Server发送的消息
作者         : zhangzx 
版本         : 1.0 
**********************************************************/
#ifndef __HELLOSRV_HPP__
#define __HELLOSRV_HPP__

#include "BThread.hpp"
#include "Udp.hpp"
#include "NDStruct.hpp"
#include "Packet.hpp"

class CHelloSrvThread: public CBaseThread
{
private:
	//服务器socket
	CUdpSvr mSrvSkt;
	//是否已启动
	ndBool mBRun;
	
protected:
	//线程处理函数
	virtual void ProcessVirtual(void);
	virtual CPacket *GetNewPkt()=0;
	//启动服务socket
	bool StartService();	
	//接收并处理数据
	void RcvAndDealMsg();	
public:
	//构造函数
	CHelloSrvThread();
	//析构函数
	~CHelloSrvThread();
	//发送数据包
	int SendTo(ndUInt32 IP, ndInt16 iPort, const char *pBuf, int iBufLen);
	//hello线程是否已经运行
	ndBool IfAlreadyRun();
};

#endif
