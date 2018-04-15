/****************** Copyright (c) 2008 **************
模块名       : 
文件名       :MobilePkt.hhpp
相关文件     : 
文件实现功能 :客户端数据包类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __MOBILEPACKET_HPP__
#define __MOBILEPACKET_HPP__

#include "Packet.hpp"
#include "NDStruct.hpp"

//**********************************
//手机端数据包基类                   
//**********************************
class CMobilePkt:public CPacket
{
protected:
	//处理数据包
	virtual bool DealWithPkt();
public:
	//构造函数
	CMobilePkt();
	//析构函数
	~CMobilePkt();

	//发送数据包
	virtual bool SendPktToClient();
};

#endif

