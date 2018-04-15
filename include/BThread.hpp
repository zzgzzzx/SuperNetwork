/****************** Copyright (c) 2008 **************
模块名       : 线程基类申明
文件名       : Bthread.hpp
相关文件	 : Bthread.cpp
文件实现功能 : 
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __BASETHREAD_HPP
#define __BASETHREAD_HPP
#include <pthread.h>

//**********************************
//信息基类                       
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
