/****************** Copyright (c) 2008 **************
ģ����       : 
�ļ���       :HelloPkt.hhpp
����ļ�     : 
�ļ�ʵ�ֹ��� :�ͻ������ݰ���
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __USER_HELLOPACKET_HPP__
#define __USER_HELLOPACKET_HPP__

#include "HelloPkt.hpp"

//**********************************
//����ڵ������ݰ�����                   
//**********************************
class CUserHelloPkt:public CHelloPkt
{
protected:
	//�������ݰ�
	virtual bool DealWithPkt();
public:
	//���캯��
	CUserHelloPkt();
	//��������
	~CUserHelloPkt();
};

#endif

