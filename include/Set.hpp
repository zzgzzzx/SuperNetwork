/*********************************************************
模块名       : 
文件名       :Set.hpp
相关文件     : 
文件实现功能 :集合类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __SET_HPP__
#define __SET_HPP__

#include "NDStruct.hpp"
#include "Inform.hpp"
#include "Packet.hpp"

//**********************************
//集合信息基类                       
//**********************************
class CBaseSet: public CMap<ndUInt32, CBaseInform *>
{
protected:
	//增加节点
	bool AddItem(ndUInt32 Key,CBaseInform *pBI);
	//删除节点
	bool DelItem(ndUInt32 Key);
	//查找结点
	CBaseInform *FindItem(ndUInt32 Key);
	//查找结点
	CBaseInform *FindItemNL(ndUInt32 Key);

public:
	//构造函数
	CBaseSet();
	//析构函数
	~CBaseSet();
};

#endif


