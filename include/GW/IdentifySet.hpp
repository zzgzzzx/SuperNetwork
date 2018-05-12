/*********************************************************
模块名       : 
文件名       :IDentifySet.hpp
相关文件     : 
文件实现功能 :用户标识集合类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __IDENTIFY_SET_HPP__
#define __IDENTIFY_SET_HPP__

#include "Set.hpp"
#include "UserHelloPkt.hpp"

/*
 *	用户身份识别码为key---对象
 */
class CIdentifySet: public CMap<ndString, SBindInform *>
{
private:
	//以mac为key的map
	CMap<ndString, SBindInform*> mMacMap;
	//以出口服务为key的map
	CMap<ndString, ServiceInform*> mServiceMap;

public:
	//构造函数
	CIdentifySet();
	//析构函数
	~CIdentifySet();

	//增加节点
	bool AddItem(ndString Key,SBindInform *pBI);
	//删除节点
	bool DelItem(ndString Key);
	//查找结点
	SBindInform *FindItem(ndString Key);
	//查找结点
	SBindInform *FindItemNL(ndString Key);
	//根据mac查找结点
	SBindInform *FindItemByMac(ndString mac);

	//接收处理数据包
	bool DealRcvPkt(CUserHelloPkt *pkt);

	//是否有超时的检测
	void SendHelloAndCheck();
	
	//向中心服务器请求下游设备身份对应的出口信息(上线)
	ndStatus BindIdentifyService(SBindInform sBindInform);
	//向中心服务器通知下游设备身份对应的出口信息释放(下线)
	ndStatus UnBindIdentifyService(SBindInform sBindInform);	
};

#endif
