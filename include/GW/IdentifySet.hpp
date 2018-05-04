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
	CMap<ndString, SBindInform*>mMacMap;
	//以出口服务为key的map
	CMap<ndString, ServiceInform*> mServiceMap;
	//从网关配置文件中读取mac与身份识别的关联表
	bool ReadMacIdentifyFromGW(list<SBindInform> &ltSI);

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

	//从网关配置数据库中读取MAC与身份的识别码并处理业务
	bool InitIdentifyFromGW();
	//通过ARP表读取MAC与IP对应关系
	bool ReadARP(list<SBindInform> &ltBSer);

	//是否有超时的检测
	void SendHelloAndCheck();
};

#endif
