/****************** Copyright (c) 2008 **************
ģ����       : Hello��������
�ļ���       : HelloSrv.hpp
����ļ�	    : HelloSrv.cpp
�ļ�ʵ�ֹ��� : ���մ���Hello Server���͵���Ϣ
����         : zhangzx 
�汾         : 1.0 
**********************************************************/
#ifndef __USER_HELLOSRV_HPP__
#define __USER_HELLOSRV_HPP__

#include "HelloSrvThread.hpp"
#include "NDStruct.hpp"

class CUserHelloThread: public CHelloSrvThread
{
private:
	//�ڵ�hello
	static void NodeHelloFunc(ndULong param);		
protected:
	//�̴߳�����
	void ProcessVirtual(void);	
	CPacket *GetNewPkt();
public:
	//���캯��
	CUserHelloThread();
	//��������
	~CUserHelloThread();
};

#endif
