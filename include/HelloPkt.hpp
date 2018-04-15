/****************** Copyright (c) 2008 **************
模块名       : 
文件名       :HelloPkt.hhpp
相关文件     : 
文件实现功能 :客户端数据包类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __HELLOPACKET_HPP__
#define __HELLOPACKET_HPP__

#include "Packet.hpp"
#include "NDStruct.hpp"

//**********************************
//网关与服务节点检测数据包基类                   
//**********************************
class CHelloPkt:public CPacket
{
protected:
	//组装数据包
	bool MakeHelloPkt();
	//分析数据包
	virtual bool AnalysePkt();	
	//处理数据包
	virtual bool DealWithPkt()=0;
public:
	//构造函数
	CHelloPkt();
	//析构函数
	~CHelloPkt();

	//发送数据包
	bool SendHelloPkt();
};

#endif

