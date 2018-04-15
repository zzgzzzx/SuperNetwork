/***********************************************************
<PRE>
模块名       : 
文件名       : 
相关文件     : 
文件实现功能 : 
作者         :
版本         :1.0 
</PRE>
**********************************************************/
#include "TimerManager.hpp"
#include "NDStruct.hpp"
#include "NDFunc.hpp"

#define SLEEPTIME 200000			//微妙级时间
#define SLEEPDECTIME SLEEPTIME/1000 //毫秒级时间

/*******************************************************
函数说明：
参数说明：
********************************************************/
CTimerManager::CTimerManager()
{
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_ERRORCHECK_NP);

	pthread_mutex_init(&m_Mutex,&attr);

	pthread_mutexattr_destroy(&attr);
	
	pthread_cond_init(&m_ProcessCond,NULL);
}

/*******************************************************
函数说明：
参数说明：
********************************************************/
CTimerManager::~CTimerManager()
{
	pthread_cond_destroy(&m_ProcessCond);
	pthread_mutex_destroy(&m_Mutex); 
}

/************************************************************************/
//作者：
//日期：
//返回值:  None
//描述：定时检测是否由定时时间到
//参数：none
/************************************************************************/
void CTimerManager::ProcessVirtual(void)
{
	STimerNode *STNode;
	
	AfxWriteDebugLog("SuperVPN run at [CTimerManager::ProcessVirtual] SUPERVPN TIMER BEGIN WORKING......");
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

	while (true) 
	{
		pthread_mutex_lock(&m_Mutex);
		//如果没有定时请求，就堵住，等待条件变量
		if (mTimerMap.empty()) 
		{
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
			pthread_cond_wait(&m_ProcessCond,&m_Mutex);
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
		}

		mTimerMap.GetBegin(STNode);
		while(mTimerMap.GetCurrent(STNode))
		{
			if (STNode->uTimeOut <= 0)
			{
				STNode->CallBack(STNode->uParam);
				//不是循环的定时器直接释放
				if(STNode->iTimerType==TIMER_SINGLE_TYPE)
				{
					//mTimerMap.erase(STNode->lCallId);
					mTimerMap.EraseCurMNext();
					delete STNode;
					continue;
				}
				else STNode->uTimeOut=STNode->uTime;
			}
			else STNode->uTimeOut=STNode->uTimeOut-SLEEPDECTIME;
			//AfxWriteDebugLog("Timer Node=[%d],Time=[%d]",STNode->lCallId,STNode->uTimeOut);
			mTimerMap.GetNext(STNode);
		};

		pthread_mutex_unlock(&m_Mutex);
		//允许线程Cancel
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
		usleep(SLEEPTIME);		
		//禁止线程Cancel，后面的程序是Critical Section
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	}
}

/************************************************************************/
//作者：
//日期：
//返回值:  
//描述：添加定时器
//参数：timeout(秒数)
/************************************************************************/
int CTimerManager::InsertTimer(unsigned long callid,int timertype,unsigned long timeout,void (*CallBack)(ndULong))
{
	STimerNode *STNode;

	pthread_mutex_lock(&m_Mutex); 

	if (mTimerMap.find(callid,STNode))
	{
		AfxWriteDebugLog("SuperVPN run at [CTimerManager::ProcessVirtual] INSERT TIMER ALREADY EXISTS!");
		pthread_mutex_unlock(&m_Mutex);
		return -1;
	}
	
	STNode = new STimerNode;
	if (STNode == NULL) 
	{
		pthread_mutex_unlock(&m_Mutex);
		return -1;
	}
		
	STNode->CallBack	= CallBack;
	STNode->iTimerType	= timertype;
	STNode->lCallId		= callid;
	STNode->uTime		= timeout*1000;
	STNode->uTimeOut	= timeout*1000;
	STNode->uParam		= callid;
	mTimerMap.insert(callid,STNode);
	
	if (mTimerMap.size() == 1) pthread_cond_signal(&m_ProcessCond); //激活processcond条件变量

	AfxWriteDebugLog("SuperVPN run at [CTimerManager::ProcessVirtual] INSERT TIMER SUCCESS!");
	pthread_mutex_unlock(&m_Mutex); 
	
	return 0;
}

/************************************************************************/
//作者：
//日期：
//返回值:  
//描述：删除定时器
//参数：
/************************************************************************/
int CTimerManager::DeleteTimer(int timertype,unsigned long callid)
{
	STimerNode *STNode;
	pthread_mutex_lock(&m_Mutex); 

	AfxWriteDebugLog("SuperVPN run at [CTimerManager::ProcessVirtual] DELETE TIMER Callid=[%lu]",callid);

	if (mTimerMap.find(callid,STNode))
	{
		mTimerMap.erase(callid);
		delete STNode;
	}

	pthread_mutex_unlock(&m_Mutex); 
	
	return 0;
}

/************************************************************************/
//作者：
//日期：
//返回值:  
//描述：重置定时器
//参数：
/************************************************************************/
int CTimerManager::ResetTimer(int timertype,unsigned long timeout,unsigned long callid)
{
	STimerNode *STNode;

	pthread_mutex_lock(&m_Mutex); 

	if (mTimerMap.find(callid,STNode))
	{
		STNode->uTime=timeout*1000;
		STNode->uTimeOut=timeout*1000;
		pthread_mutex_unlock(&m_Mutex);
		return 0;
	}
	pthread_mutex_unlock(&m_Mutex);
	return -1;
}

/************************************************************************/
//作  者：
//日  期：
//返回值:  
//描  述：增加循环定时器
//参  数：timeout(秒数)
/************************************************************************/
int CTimerManager::InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return InsertTimer(callid,TIMER_REPEATE_TYPE,timeout,CallBack);
}

/************************************************************************/
//作者：
//日期：
//返回值:  
//描述：增加单回定时器
//参数：timeout(秒数)
/************************************************************************/
int CTimerManager::InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return InsertTimer(callid,TIMER_SINGLE_TYPE,timeout,CallBack);
}

/************************************************************************/
//作者：
//日期：
//返回值:  
//描述：删除增加单回定时器
//参数：
/************************************************************************/
int CTimerManager::DeleteCircleTimer(unsigned long callid)
{
	return DeleteTimer(TIMER_REPEATE_TYPE,callid);
}

/************************************************************************/
//作者：
//日期：
//返回值:  
//描述：删除增加单回定时器
//参数：
/************************************************************************/
int CTimerManager::DeleteSingleTimer(unsigned long callid)
{
	return DeleteTimer(TIMER_SINGLE_TYPE,callid);
}
