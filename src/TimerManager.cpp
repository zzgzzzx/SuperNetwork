/***********************************************************
<PRE>
ģ����       : 
�ļ���       : 
����ļ�     : 
�ļ�ʵ�ֹ��� : 
����         :
�汾         :1.0 
</PRE>
**********************************************************/
#include "TimerManager.hpp"
#include "NDStruct.hpp"
#include "NDFunc.hpp"

#define SLEEPTIME 200000			//΢�ʱ��
#define SLEEPDECTIME SLEEPTIME/1000 //���뼶ʱ��

/*******************************************************
����˵����
����˵����
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
����˵����
����˵����
********************************************************/
CTimerManager::~CTimerManager()
{
	pthread_cond_destroy(&m_ProcessCond);
	pthread_mutex_destroy(&m_Mutex); 
}

/************************************************************************/
//���ߣ�
//���ڣ�
//����ֵ:  None
//��������ʱ����Ƿ��ɶ�ʱʱ�䵽
//������none
/************************************************************************/
void CTimerManager::ProcessVirtual(void)
{
	STimerNode *STNode;
	
	AfxWriteDebugLog("SuperVPN run at [CTimerManager::ProcessVirtual] SUPERVPN TIMER BEGIN WORKING......");
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

	while (true) 
	{
		pthread_mutex_lock(&m_Mutex);
		//���û�ж�ʱ���󣬾Ͷ�ס���ȴ���������
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
				//����ѭ���Ķ�ʱ��ֱ���ͷ�
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
		//�����߳�Cancel
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
		usleep(SLEEPTIME);		
		//��ֹ�߳�Cancel������ĳ�����Critical Section
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
	}
}

/************************************************************************/
//���ߣ�
//���ڣ�
//����ֵ:  
//��������Ӷ�ʱ��
//������timeout(����)
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
	
	if (mTimerMap.size() == 1) pthread_cond_signal(&m_ProcessCond); //����processcond��������

	AfxWriteDebugLog("SuperVPN run at [CTimerManager::ProcessVirtual] INSERT TIMER SUCCESS!");
	pthread_mutex_unlock(&m_Mutex); 
	
	return 0;
}

/************************************************************************/
//���ߣ�
//���ڣ�
//����ֵ:  
//������ɾ����ʱ��
//������
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
//���ߣ�
//���ڣ�
//����ֵ:  
//���������ö�ʱ��
//������
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
//��  �ߣ�
//��  �ڣ�
//����ֵ:  
//��  ��������ѭ����ʱ��
//��  ����timeout(����)
/************************************************************************/
int CTimerManager::InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return InsertTimer(callid,TIMER_REPEATE_TYPE,timeout,CallBack);
}

/************************************************************************/
//���ߣ�
//���ڣ�
//����ֵ:  
//���������ӵ��ض�ʱ��
//������timeout(����)
/************************************************************************/
int CTimerManager::InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return InsertTimer(callid,TIMER_SINGLE_TYPE,timeout,CallBack);
}

/************************************************************************/
//���ߣ�
//���ڣ�
//����ֵ:  
//������ɾ�����ӵ��ض�ʱ��
//������
/************************************************************************/
int CTimerManager::DeleteCircleTimer(unsigned long callid)
{
	return DeleteTimer(TIMER_REPEATE_TYPE,callid);
}

/************************************************************************/
//���ߣ�
//���ڣ�
//����ֵ:  
//������ɾ�����ӵ��ض�ʱ��
//������
/************************************************************************/
int CTimerManager::DeleteSingleTimer(unsigned long callid)
{
	return DeleteTimer(TIMER_SINGLE_TYPE,callid);
}
