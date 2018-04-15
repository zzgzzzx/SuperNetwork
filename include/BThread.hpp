/****************** Copyright (c) 2008 **************
ģ����       : �̻߳�������
�ļ���       : Bthread.hpp
����ļ�	 : Bthread.cpp
�ļ�ʵ�ֹ��� : 
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __BASETHREAD_HPP
#define __BASETHREAD_HPP
#include <pthread.h>

//**********************************
//��Ϣ����                       
//**********************************
class CBaseThread
{
public:
	int Start(void);
	int Join(void);
	void Stop();
	CBaseThread();	
	virtual ~CBaseThread();
protected:
	static void * EntryPoint(void*);
	virtual void ProcessVirtual(void);
	virtual void StopPreDeal(void);
private:
	pthread_t m_ptThreadID;
	int m_iState;
};

#endif
