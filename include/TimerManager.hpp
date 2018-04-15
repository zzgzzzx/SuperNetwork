/***********************************************************
<PRE>
模块名       : 
文件名       : 
相关文件     : 
文件实现功能 : 
作者         :zhangzx 
版本         :1.0 
</PRE>
**********************************************************/

#ifndef  __TIMERMANAGER_HPP
#define __TIMERMANAGER_HPP

#include "NDType.hpp"
#include "BThread.hpp"
#include "Map.hpp"
#include <unistd.h>
#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <sys/time.h>

//定时器类型
#define TIMER_REPEATE_TYPE 0
#define TIMER_SINGLE_TYPE 1

typedef struct TimerNode
{  
	ndULong lCallId;			//定时器所属的呼叫唯一性标识	
	int  iTimerType;		//定时器类别
	ndULong uParam;			//参数
	long uTimeOut;			//超时时间的计数
	long uTime;				//超时时间的备份
	void (*CallBack)(ndULong);	//发送TIMEOUT事件的回调函数
}STimerNode;


class CTimerManager : public CBaseThread  
{
public:
	CTimerManager();
	virtual ~CTimerManager();

	//增加定时器
	int InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));
	int InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));

	//删除定时器
	int DeleteCircleTimer(unsigned long callid);
	int DeleteSingleTimer(unsigned long callid);

protected:
	void ProcessVirtual(void);

private:
	CMap<ndULong,STimerNode *> mTimerMap;
	pthread_mutex_t m_Mutex;   	
	pthread_cond_t  m_ProcessCond;

	/*插入定时器定时器队列*/
	int InsertTimer(unsigned long callid,int timertype,unsigned long timeout,void (*CallBack)(ndULong));
	//删除定时器
	int DeleteTimer(int timertype,unsigned long callid); 	//删除指定的定时器节点	
	//重置定时器
	int ResetTimer(int timertype,unsigned long timeout,unsigned long callid);//重置定时器
};

#endif 

