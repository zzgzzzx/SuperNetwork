/****************** Copyright (c) 2008 **************
模块名       : 
文件名       :HelloPkt.hhpp
相关文件     : 
文件实现功能 :客户端数据包类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __SRV_HELLOPACKET_HPP__
#define __SRV_HELLOPACKET_HPP__

#include "HelloPkt.hpp"

//**********************************
//服务节点检测数据包基类                   
//**********************************
class CSrvHelloPkt:public CHelloPkt
{
protected:
	//处理数据包
	virtual bool DealWithPkt();
public:
	//构造函数
	CSrvHelloPkt();
	//析构函数
	~CSrvHelloPkt();
};

#endif

