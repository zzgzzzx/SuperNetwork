/*********************************************************
ģ����       : 
�ļ���       :Inform.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :��Ϣ��
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __INFORM_HPP__
#define __INFORM_HPP__

#include "Packet.hpp"
#include "NDStruct.hpp"
#include "NDType.hpp"

//**********************************
//��Ϣ����                       
//**********************************
class CBaseInform
{
public:
	CBaseInform();
	virtual ~CBaseInform();
};

//**********************************
//�豸��ݰ󶨳�����Ϣ��                       
//**********************************
class SBindInform: public CBaseInform
{
public:
	 //vpn����ڵ��IP��ַ
	 ndString	sServiceIP;
	 //��id
	 ndString 	sDomainID;
	 //������
	 ndString 	sDomainMask;
	 //���key
	 ndString	sDomainKey;
	 //�����豸��������
	 ndString	sDeviceFlag;
	 //�����豸��IP��ַ
	 ndString	sDeviceIP;
	 //�����豸��Mac��ַ
	 ndString	sDeviceMac;	
public:
	SBindInform();
	virtual ~SBindInform();
};

//**********************************
//������Ϣ��                       
//**********************************
class ServiceInform: public CBaseInform
{
public:
	 //����ڵ㵱ǰ��HelloNumber
	 ndInt8 iHelloNum;
	 //vpn����ڵ��IP��ַ
	 ndString	sServiceIP;	 
	 //�������豸��Ϣ
	 list<SBindInform> 	ltBindInform;
public:
	ServiceInform();
	virtual ~ServiceInform();
};

typedef list<SBindInform>::iterator BindInformItr;


#endif
