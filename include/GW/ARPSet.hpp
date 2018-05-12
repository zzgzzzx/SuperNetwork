/*********************************************************
模块名       : 
文件名       :ARPset.hpp
相关文件     : 
文件实现功能 :ARP集合类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __ARP_SET_HPP__
#define __ARP_SET_HPP__

#include "NDStruct.hpp"
#include "Inform.hpp"

/*
 *	ARP:mac地址----SBindInform
 */
class CARPSet: public CMap<ndString, SBindInform *>
{
private:
	//arp检测是否已启动标识
	ndBool mBArpCheckRun;
	//获取当前的arp列表信息
	bool GetCurrentARP(list<SArp> &liarp);

	//更新标识重置为false
	void RestUpdateFalse();
	
public:
	//构造函数
	CARPSet();
	//析构函数
	~CARPSet();

	//增加节点
	bool AddItem(ndString mac, SBindInform *pBI);
	//删除节点
	bool DelItem(ndString mac);
	//查找结点
	SBindInform *FindItem(ndString mac);
	//查找结点
	SBindInform *FindItemNL(ndString mac);

	//检测ARP
	bool ARPCheck();

	//从网关配置数据库中读取MAC与身份的识别码并处理业务
	bool InitIdentifyFromGW();	
};

#endif
