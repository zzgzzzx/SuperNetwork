/****************** Copyright (c) 2008 **************
ģ����       : 
�ļ���       :HelloPkt.hhpp
����ļ�     : 
�ļ�ʵ�ֹ��� :�ͻ������ݰ���
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __HELLOPACKET_HPP__
#define __HELLOPACKET_HPP__

#include "Packet.hpp"
#include "NDStruct.hpp"

//**********************************
//���������ڵ������ݰ�����                   
//**********************************
class CHelloPkt:public CPacket
{
protected:
	//��װ���ݰ�
	bool MakeHelloPkt();
	//�������ݰ�
	virtual bool AnalysePkt();	
	//�������ݰ�
	virtual bool DealWithPkt()=0;
public:
	//���캯��
	CHelloPkt();
	//��������
	~CHelloPkt();

	//�������ݰ�
	bool SendHelloPkt();
};

#endif

