/***********************************************************
<PRE>
ģ����       : 
�ļ���       :mainsrv.cpp
����ļ�     : 
�ļ�ʵ�ֹ��� : ͨ��FIFO����ʵ�ֲ�ͬ�߳���Ϣ�Ĵ�ȡ
����         :zhangzx 
�汾         :1.0 
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
    
    CMsgFIFO(unsigned int uiMaxMsgNum = MAX_FIFO_MSG_NUM);//��CMsgFIFO�Ĺ��캯��
	
    ~CMsgFIFO();//��CMsgFIFO����������
	
    T GetMsg(); //CMsgFIFO�ĳ�Ա����GetMsg()�����ڴ�FIFO��Ϣ������ȡ����Ϣ      
   
	void PutMsg(T);	//CMsgFIFO�ĳ�Ա����PutMsg(), ���ڽ���Ϣд��FIFO��Ϣ������

	int GetCount();
  
  private:
   
    queue <T> m_pMsgQueue; //FIFO��Ϣ����
	
	unsigned int m_iMaxMsgNum;      //FIFO�����ܴ洢�������Ϣ��Ŀ��
    
	pthread_mutex_t m_Mutex;     //����������������
    
	pthread_cond_t  m_Writecond; //д��Ϣ�������� 
    
	pthread_cond_t  m_Readcond;  //����Ϣ��������
}; 


/*��CMsgFIFO�Ĺ��캯��*/
template <class T>
CMsgFIFO<T>::CMsgFIFO(unsigned int uiMaxMsgNum)
{      
	m_iMaxMsgNum=uiMaxMsgNum;
	pthread_mutex_init(&m_Mutex,NULL);
	pthread_cond_init(&m_Writecond,NULL);
	pthread_cond_init(&m_Readcond,NULL);   
} 

/*��CMsgFIFO����������*/
template <class T>
CMsgFIFO<T>::~CMsgFIFO()
{
    pthread_mutex_destroy(&m_Mutex); 
    pthread_cond_destroy(&m_Writecond); 
	pthread_cond_destroy(&m_Readcond); 
}

/* CMsgFIFO::m_iGetMsg(), ��FIFO��Ϣ������ȡ����Ϣ */
template <class T>
T CMsgFIFO<T>::GetMsg()      
{
	T p_Msg;
	int lockmark = 0;

	lockmark = pthread_mutex_lock(&m_Mutex); //��FIFO���й���������

	while(m_pMsgQueue.empty())  //FIFO������û����Ϣ���߳�ת��ȴ�״̬ 
	{
		pthread_cond_wait(&m_Readcond,&m_Mutex);
	}

	p_Msg=m_pMsgQueue.front();   //��FIFO���ж�����Ϣ
	m_pMsgQueue.pop();  //������Ϣ��FIFO������ɾ��

	if(m_iMaxMsgNum-1 == m_pMsgQueue.size())
	pthread_cond_signal(&m_Writecond);//����������д�߳�

	pthread_mutex_unlock(&m_Mutex);//��FIFO���й���������

	return(p_Msg);
}
 

 /*����CMsgFIFO::m_iPutMsg(), ����Ϣд��FIFO��Ϣ������*/
template <class T>
void CMsgFIFO<T>::PutMsg(T p_Msg)       
{
	int lockmark;
	 
	lockmark=pthread_mutex_lock(&m_Mutex);//��FIFO���й���������
	 
	 while(m_pMsgQueue.size()==m_iMaxMsgNum)//FIFO���������߳�ת��ȴ�״̬
	{
		pthread_cond_wait(&m_Writecond,&m_Mutex);
	}
	 
	 m_pMsgQueue.push(p_Msg);//����Ϣд��FIFO����

	 if(1==m_pMsgQueue.size())	////���������Ķ��߳�
		pthread_cond_signal(&m_Readcond);
		
	 pthread_mutex_unlock(&m_Mutex); //��FIFO���й���������
}

/* the number in FIFO */
template <class T>
int CMsgFIFO<T>::GetCount()
{
	return m_pMsgQueue.size();
}

#endif
