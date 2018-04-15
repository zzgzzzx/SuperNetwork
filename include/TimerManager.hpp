/***********************************************************
<PRE>
ģ����       : 
�ļ���       : 
����ļ�     : 
�ļ�ʵ�ֹ��� : 
����         :zhangzx 
�汾         :1.0 
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

//��ʱ������
#define TIMER_REPEATE_TYPE 0
#define TIMER_SINGLE_TYPE 1

typedef struct TimerNode
{  
	ndULong lCallId;			//��ʱ�������ĺ���Ψһ�Ա�ʶ	
	int  iTimerType;		//��ʱ�����
	ndULong uParam;			//����
	long uTimeOut;			//��ʱʱ��ļ���
	long uTime;				//��ʱʱ��ı���
	void (*CallBack)(ndULong);	//����TIMEOUT�¼��Ļص�����
}STimerNode;


class CTimerManager : public CBaseThread  
{
public:
	CTimerManager();
	virtual ~CTimerManager();

	//���Ӷ�ʱ��
	int InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));
	int InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));

	//ɾ����ʱ��
	int DeleteCircleTimer(unsigned long callid);
	int DeleteSingleTimer(unsigned long callid);

protected:
	void ProcessVirtual(void);

private:
	CMap<ndULong,STimerNode *> mTimerMap;
	pthread_mutex_t m_Mutex;   	
	pthread_cond_t  m_ProcessCond;

	/*���붨ʱ����ʱ������*/
	int InsertTimer(unsigned long callid,int timertype,unsigned long timeout,void (*CallBack)(ndULong));
	//ɾ����ʱ��
	int DeleteTimer(int timertype,unsigned long callid); 	//ɾ��ָ���Ķ�ʱ���ڵ�	
	//���ö�ʱ��
	int ResetTimer(int timertype,unsigned long timeout,unsigned long callid);//���ö�ʱ��
};

#endif 

