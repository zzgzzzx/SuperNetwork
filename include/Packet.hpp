/*********************************************************
模块名       : 
文件名       :Packet.hpp
相关文件     : 
文件实现功能 :数据包类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include "NDStruct.hpp"

//数据包长度定义(以太网的MTU)
#define PACKET_BUFF_LEN 1472

//////////////////////////////////////////////////////////
//数据包基类                     
/////////////////////////////////////////////////////////
class CPacket
{
protected:
    //发送的数据buff
    ndString mSendBuf;
    //接收的数据buff
    ndString mRcvBuf;

	//数据包基本信息
	SNetParam mRcvNP,mSndNP;				
					
	//检测数据包
	virtual bool CheckPkt();		
	//分析数据包
	virtual bool AnalysePkt();	
	//处理数据包
	virtual bool DealWithPkt();	
public:	
	//构造函数
	CPacket();
	//析构函数
	virtual ~CPacket();					

	//获取网络参数信息(返回的都是主机序)
	SNetParam GetRcvNetParam();	
	
	//设备网络参数信息
	void SetRcvNetParam(const SNetParam &sNP);	
	//设置数据包
	void SetRcvPktBuf(char *buf,int len);	

	//设置网络参数信息
	void SetSndNetParam(const SNetParam &sNP);		

	//处理数据包
	bool DisposePkt();	

	//克隆数据包
	virtual CPacket *Clone();

	//发送UDP数据包
	bool SendTo();								
};


#endif
