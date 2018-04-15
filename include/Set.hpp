/*********************************************************
ģ����       : 
�ļ���       :Set.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :������
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __SET_HPP__
#define __SET_HPP__

#include "NDStruct.hpp"
#include "Inform.hpp"
#include "Packet.hpp"

//**********************************
//������Ϣ����                       
//**********************************
class CBaseSet: public CMap<ndUInt32, CBaseInform *>
{
protected:
	//���ӽڵ�
	bool AddItem(ndUInt32 Key,CBaseInform *pBI);
	//ɾ���ڵ�
	bool DelItem(ndUInt32 Key);
	//���ҽ��
	CBaseInform *FindItem(ndUInt32 Key);
	//���ҽ��
	CBaseInform *FindItemNL(ndUInt32 Key);

public:
	//���캯��
	CBaseSet();
	//��������
	~CBaseSet();
};

#endif


