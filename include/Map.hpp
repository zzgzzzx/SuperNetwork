/****************** Copyright (c) 2008 **************
<PRE>
ģ����       : 
�ļ���       :Map.hpp 
����ļ�     : 
�ļ�ʵ�ֹ��� : 
����         :zhangzx 
�汾         :1.0 
</PRE>
**********************************************************/
#ifndef __MAPMANAGE_HPP
#define __MAPMANAGE_HPP

using namespace std;

#include <map>
#include <pthread.h>
#include <errno.h>

//**********************************
//Map����                       
//**********************************
template <class TF,class TM>
class CMap
{
	public:
		CMap();
		~CMap();
		void lock();
		void unlock();
		bool find(TF id,TM &inform);
		bool nlfind(TF id,TM &inform);
		bool insert(TF id,TM inform);	
		bool nlinsert(TF id,TM inform);	
		bool erase(TF id);
		bool nlerase(TF id);
		bool empty();
		bool clear();
		int size();
		bool GetBegin(TM &inform);
		bool GetCurrent(TM &inform);
		bool GetNext(TM &inform);
		bool EraseCur();
		bool EraseCurMNext();
		bool IfCurEnd();
	private:
		map<TF,TM> m_list;				//Map�б�
		typedef typename map<TF,TM>::iterator Map_Iter;
		typedef typename map<TF,TM>::value_type Map_Value;
		Map_Iter	m_item;
		pthread_mutex_t m_Mutex;		//����������������
};

/*********************************************************
����˵����
���˵������ͬ������������ͼ��һ���ѱ������Ļ���������ʱ���ֲ�ͬ�����ĸ�ֵ�ɹ�ѡ��
PTHREAD_MUTEX_TIMED_NP������ȱʡֵ��Ҳ������ͨ������һ���̼߳����Ժ���������������
�̽��γ�һ���ȴ����У����ڽ��������ȼ������������������ȷ������Դ����Ĺ�ƽ�ԡ� 
PTHREAD_MUTEX_RECURSIVE_NP��Ƕ����������ͬһ���̶߳�ͬһ�����ɹ���ö�Σ���ͨ�����
unlock�����������ǲ�ͬ�߳��������ڼ����߳̽���ʱ���¾����� PTHREAD_MUTEX_ERRORCHECK_NP��
�����������ͬһ���߳�����ͬһ�������򷵻�EDEADLK�������PTHREAD_MUTEX_TIMED_NP���Ͷ���
��ͬ��������ȷ�����������μ���ʱ��������������µ������� 
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
CMap<TF,TM>::CMap()
{
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_ERRORCHECK_NP);

	pthread_mutex_init(&m_Mutex,&attr);

	pthread_mutexattr_destroy(&attr);

	m_item=m_list.begin();
}

/*********************************************************
����˵����map����
���˵������
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
void CMap<TF,TM>::lock()
{
	if (pthread_mutex_lock(&m_Mutex) == EDEADLK)
	{
		//printf("^_^ ^_^ ====>Same Thread Request The Same Lock,But Work Ok Now......");
	}
}

/*********************************************************
����˵����map����
���˵������
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
void CMap<TF,TM>::unlock()
{
	pthread_mutex_unlock(&m_Mutex);	
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
CMap<TF,TM>::~CMap()
{
	pthread_mutex_destroy(&m_Mutex); 
}

/*********************************************************
����˵����map��ȡ���Ԫ��
���˵����
����˵����Ԫ����Ϣ
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::GetBegin(TM &inform)
{
	m_item=m_list.begin();
	if(m_item==m_list.end()) return false;
	inform=m_item->second;

	return true;
}

/*********************************************************
����˵����map��ȡ��ǰԪ��
���˵����
����˵����Ԫ����Ϣ
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::GetCurrent(TM &inform)
{
	if(m_item==m_list.end()) return false;
	inform=m_item->second;

	return true;
}

/*********************************************************
����˵����map��ȡ��һ��Ԫ��
���˵����
����˵����Ԫ����Ϣ
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::GetNext(TM &inform)
{
	m_item++;
	
	if(m_item==m_list.end()) return false;
	inform=m_item->second;

	return true;
}

/*********************************************************
����˵�����Ƴ���ǰԪ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::EraseCur()
{
	if (m_item == m_list.end()) return false;

	m_list.erase(m_item);

	return true;
}

/*********************************************************
����˵�����Ƴ���ǰԪ�ز����Ƶ�����
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::EraseCurMNext()
{
	if (m_item == m_list.end()) return false;

	Map_Iter next = m_item;
	next++;
	m_list.erase(m_item);
	m_item = next;
	
	return true;	
}

/*********************************************************
����˵�����жϵ�ǰ�������Ƿ��ǽ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::IfCurEnd()
{
	if (m_item == m_list.end()) return true;

	return false;
}

/*********************************************************
����˵������ȡmap�ĸ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
int CMap<TF,TM>::size()
{
	return m_list.size();
}

/*********************************************************
����˵�����������Ĳ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::nlfind(TF id,TM &inform)
{	
	m_item=m_list.find(id);

	if(m_item==m_list.end()) 	return false;
	inform=m_item->second;

	return true;
}

/*********************************************************
����˵���������Ĳ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::find(TF id,TM &inform)
{
	lock();
	
	m_item=m_list.find(id);

	if(m_item==m_list.end()) 
	{
		unlock();
		return false;
	}
	inform=m_item->second;

	unlock();
	return true;
}

/*********************************************************
����˵��������������Ԫ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::insert(TF id,TM inform)
{
	lock();	
	//m_list.insert(map<TF,TM>::value_type(id,inform));
	m_list.insert(Map_Value(id,inform));
	unlock();
	return true;
}

/*********************************************************
����˵����������������Ԫ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::nlinsert(TF id,TM inform)
{
	//m_list.insert(map<TF,TM>::value_type(id,inform));
	m_list.insert(Map_Value(id,inform));
	return true;
}

/*********************************************************
����˵�����Ƴ�Ԫ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::erase(TF id)
{
	lock();
	m_list.erase(id);
	unlock();

	return true;
}

/*********************************************************
����˵�������������Ƴ�Ԫ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::nlerase(TF id)
{
	m_list.erase(id);

	return true;
}

/*********************************************************
����˵�����ж�map�Ƿ�Ϊ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::empty()
{
	bool ret;

	lock();
	ret=m_list.empty();
	unlock();

	return ret;
}

/*********************************************************
����˵�������map
���˵����
����˵����
����ֵ  ��
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::clear()
{
	lock();
	m_list.clear();
	unlock();

	return true;
}

#endif
