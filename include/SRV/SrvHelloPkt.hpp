/****************** Copyright (c) 2008 **************
ģ����       : 
�ļ���       :HelloPkt.hhpp
����ļ�     : 
�ļ�ʵ�ֹ��� :�ͻ������ݰ���
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __SRV_HELLOPACKET_HPP__
#define __SRV_HELLOPACKET_HPP__

#include "HelloPkt.hpp"

//**********************************
//����ڵ������ݰ�����                   
//**********************************
class CSrvHelloPkt:public CHelloPkt
{
protected:
	//�������ݰ�
	virtual bool DealWithPkt();
public:
	//���캯��
	CSrvHelloPkt();
	//��������
	~CSrvHelloPkt();
};

#endif

