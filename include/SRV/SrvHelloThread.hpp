/****************** Copyright (c) 2008 **************
ģ����       : Hello��������
�ļ���       : HelloSrv.hpp
����ļ�	    : HelloSrv.cpp
�ļ�ʵ�ֹ��� : ���մ���Hello Server���͵���Ϣ
����         : zhangzx 
�汾         : 1.0 
**********************************************************/
#ifndef __SRV_HELLOSRV_HPP__
#define __SRV_HELLOSRV_HPP__

#include "HelloSrvThread.hpp"
#include "Udp.hpp"
#include "NDStruct.hpp"

class CSrvHelloThread: public CHelloSrvThread
{
protected:
	CPacket *GetNewPkt();

public:
	//���캯��
	CSrvHelloThread();
	//��������
	~CSrvHelloThread();
};

#endif
