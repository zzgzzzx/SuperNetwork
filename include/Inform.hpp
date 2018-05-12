/*********************************************************
模块名       : 
文件名       :Inform.hpp
相关文件     : 
文件实现功能 :信息类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __INFORM_HPP__
#define __INFORM_HPP__

#include "Packet.hpp"
#include "NDStruct.hpp"
#include "NDType.hpp"

//**********************************
//信息基类                       
//**********************************
class CBaseInform
{
public:
	CBaseInform();
	virtual ~CBaseInform();
};

//**********************************
//设备身份绑定出口信息类                       
//**********************************
class SBindInform: public CBaseInform
{
public:
	 //vpn服务节点的IP地址
	 ndString	sServiceIP;
	 //域id
	 ndString 	sDomainID;
	 //域掩码
	 ndString 	sDomainMask;
	 //域的key
	 ndString	sDomainKey;
	 //下游设备的特征码
	 ndString	sDeviceFlag;
	 //下游设备的IP地址
	 ndString	sDeviceIP;
	 //下游设备的Mac地址
	 ndString	sDeviceMac;	
	 //是否是已上线标识(这个标识只用于网关数据库配置身份标识)
	 ndBool		ifOnline;
	 //更新检测标识(这个标识只用于网关数据库配置身份标识)
	 ndBool		ifUpdate;
public:
	SBindInform();
	virtual ~SBindInform();
};

//**********************************
//出口信息类                       
//**********************************
class ServiceInform: public CBaseInform
{
public:
	 //服务节点当前的HelloNumber
	 ndInt8 iHelloNum;
	 //vpn服务节点的IP地址
	 ndString	sServiceIP;	 
	 //关联的设备信息
	 list<SBindInform> 	ltBindInform;
public:
	ServiceInform();
	virtual ~ServiceInform();
};

typedef list<SBindInform>::iterator BindInformItr;


#endif
