/***********************************************************
<PRE>
模块名       : 
文件名       :mainsrv.cpp
相关文件     : 
文件实现功能 : 通过FIFO队列实现不同线程消息的存取
作者         :zhangzx 
版本         :1.0 
</PRE>
**********************************************************/
#ifndef __MSGQUEUE_HPP
#define __MSGQUEUE_HPP

using namespace std;

#include<queue> 
#include<pthread.h>
#include<bits/pthreadtypes.h>
#include<unistd.h> 
#include "NDType.hpp"

template <class T>
class CMsgFIFO
{
  public: 
    
    CMsgFIFO(unsigned int uiMaxMsgNum = MAX_FIFO_MSG_NUM);//类CMsgFIFO的构造函数
	
    ~CMsgFIFO();//类CMsgFIFO的析构函数
	
    T GetMsg(); //CMsgFIFO的成员函数GetMsg()，用于从FIFO消息队列中取出消息      
   
	void PutMsg(T);	//CMsgFIFO的成员函数PutMsg(), 用于将消息写入FIFO消息队列中

	int GetCount();
  
  private:
   
    queue <T> m_pMsgQueue; //FIFO消息队列
	
	unsigned int m_iMaxMsgNum;      //FIFO队列能存储的最多消息条目数
    
	pthread_mutex_t m_Mutex;     //共享数据区变量锁
    
	pthread_cond_t  m_Writecond; //写消息条件变量 
    
	pthread_cond_t  m_Readcond;  //读消息条件变量
}; 


/*类CMsgFIFO的构造函数*/
template <class T>
CMsgFIFO<T>::CMsgFIFO(unsigned int uiMaxMsgNum)
{      
	m_iMaxMsgNum=uiMaxMsgNum;
	pthread_mutex_init(&m_Mutex,NULL);
	pthread_cond_init(&m_Writecond,NULL);
	pthread_cond_init(&m_Readcond,NULL);   
} 

/*类CMsgFIFO的析构函数*/
template <class T>
CMsgFIFO<T>::~CMsgFIFO()
{
    pthread_mutex_destroy(&m_Mutex); 
    pthread_cond_destroy(&m_Writecond); 
	pthread_cond_destroy(&m_Readcond); 
}

/* CMsgFIFO::m_iGetMsg(), 从FIFO消息队列中取出消息 */
template <class T>
T CMsgFIFO<T>::GetMsg()      
{
	T p_Msg;
	int lockmark = 0;

	struct timeval now;
	struct timespec outtime;
	
	gettimeofday(&now, NULL);  
	outtime.tv_sec = now.tv_sec + 5;  
	outtime.tv_nsec = now.tv_usec * 1000;

	//将FIFO队列共享区加锁
	lockmark = pthread_mutex_lock(&m_Mutex); 

	//FIFO队列中没有消息，线程转入等待状态 
	while(m_pMsgQueue.empty())  
	{
		if (pthread_cond_timedwait(&m_Readcond,&m_Mutex, &outtime) == ETIMEDOUT)
		{
			pthread_mutex_unlock(&m_Mutex);
			return NULL;
		}
	}

	//从FIFO队列读出消息
	p_Msg=m_pMsgQueue.front();   
	//将该消息从FIFO队列中删除
	m_pMsgQueue.pop();  

	if(m_iMaxMsgNum-1 == m_pMsgQueue.size())
		pthread_cond_signal(&m_Writecond);

	//将FIFO队列共享区解锁
	pthread_mutex_unlock(&m_Mutex);

	return(p_Msg);
}
 

 /*函数CMsgFIFO::m_iPutMsg(), 将消息写入FIFO消息队列中*/
template <class T>
void CMsgFIFO<T>::PutMsg(T p_Msg)       
{
	int lockmark;
	 
	lockmark=pthread_mutex_lock(&m_Mutex);//将FIFO队列共享区加锁
	 
	 while(m_pMsgQueue.size()==m_iMaxMsgNum)//FIFO队列满，线程转入等待状态
	{
		pthread_cond_wait(&m_Writecond,&m_Mutex);
	}
	 
	 m_pMsgQueue.push(p_Msg);//将消息写入FIFO队列

	 if(1==m_pMsgQueue.size())	////激活阻塞的读线程
		pthread_cond_signal(&m_Readcond);
		
	 pthread_mutex_unlock(&m_Mutex); //将FIFO队列共享区解锁
}

/* the number in FIFO */
template <class T>
int CMsgFIFO<T>::GetCount()
{
	return m_pMsgQueue.size();
}

#endif
