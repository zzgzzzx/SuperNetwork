/****************** Copyright (c) 2008 **************
ģ����       : 
�ļ���       :MobilePkt.hhpp
����ļ�     : 
�ļ�ʵ�ֹ��� :�ͻ������ݰ���
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __MOBILEPACKET_HPP__
#define __MOBILEPACKET_HPP__

#include "Packet.hpp"
#include "NDStruct.hpp"

//**********************************
//�ֻ������ݰ�����                   
//**********************************
class CMobilePkt:public CPacket
{
protected:
	//�������ݰ�
	virtual bool DealWithPkt();
public:
	//���캯��
	CMobilePkt();
	//��������
	~CMobilePkt();

	//�������ݰ�
	virtual bool SendPktToClient();
};

#endif

