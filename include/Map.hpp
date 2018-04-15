/****************** Copyright (c) 2008 **************
<PRE>
模块名       : 
文件名       :Map.hpp 
相关文件     : 
文件实现功能 : 
作者         :zhangzx 
版本         :1.0 
</PRE>
**********************************************************/
#ifndef __MAPMANAGE_HPP
#define __MAPMANAGE_HPP

using namespace std;

#include <map>
#include <pthread.h>
#include <errno.h>

//**********************************
//Map基类                       
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
		map<TF,TM> m_list;				//Map列表
		typedef typename map<TF,TM>::iterator Map_Iter;
		typedef typename map<TF,TM>::value_type Map_Value;
		Map_Iter	m_item;
		pthread_mutex_t m_Mutex;		//共享数据区变量锁
};

/*********************************************************
函数说明：
入参说明：不同的锁类型在试图对一个已被锁定的互斥锁加锁时表现不同。有四个值可供选择：
PTHREAD_MUTEX_TIMED_NP，这是缺省值，也就是普通锁。当一个线程加锁以后，其余请求锁的线
程将形成一个等待队列，并在解锁后按优先级获得锁。这种锁策略确保了资源分配的公平性。 
PTHREAD_MUTEX_RECURSIVE_NP，嵌套锁，允许同一个线程对同一个锁成功获得多次，并通过多次
unlock解锁。假如是不同线程请求，则在加锁线程解锁时重新竞争。 PTHREAD_MUTEX_ERRORCHECK_NP，
检错锁，假如同一个线程请求同一个锁，则返回EDEADLK，否则和PTHREAD_MUTEX_TIMED_NP类型动作
相同。这样就确保当不允许多次加锁时不会出现最简单情况下的死锁。 
出参说明：
返回值  ：
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
函数说明：map加锁
入参说明：无
出参说明：
返回值  ：
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
函数说明：map解锁
入参说明：无
出参说明：
返回值  ：
*********************************************************/
template <class TF,class TM>
void CMap<TF,TM>::unlock()
{
	pthread_mutex_unlock(&m_Mutex);	
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
template <class TF,class TM>
CMap<TF,TM>::~CMap()
{
	pthread_mutex_destroy(&m_Mutex); 
}

/*********************************************************
函数说明：map获取起点元素
入参说明：
出参说明：元素信息
返回值  ：
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
函数说明：map获取当前元素
入参说明：
出参说明：元素信息
返回值  ：
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::GetCurrent(TM &inform)
{
	if(m_item==m_list.end()) return false;
	inform=m_item->second;

	return true;
}

/*********************************************************
函数说明：map获取下一个元素
入参说明：
出参说明：元素信息
返回值  ：
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
函数说明：移除当前元素
入参说明：
出参说明：
返回值  ：
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::EraseCur()
{
	if (m_item == m_list.end()) return false;

	m_list.erase(m_item);

	return true;
}

/*********************************************************
函数说明：称除当前元素并下移迭代器
入参说明：
出参说明：
返回值  ：
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
函数说明：判断当前迭代器是否是结束
入参说明：
出参说明：
返回值  ：
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::IfCurEnd()
{
	if (m_item == m_list.end()) return true;

	return false;
}

/*********************************************************
函数说明：获取map的个数
入参说明：
出参说明：
返回值  ：
*********************************************************/
template <class TF,class TM>
int CMap<TF,TM>::size()
{
	return m_list.size();
}

/*********************************************************
函数说明：不加锁的查找
入参说明：
出参说明：
返回值  ：
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
函数说明：加锁的查找
入参说明：
出参说明：
返回值  ：
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
函数说明：加锁的增加元素
入参说明：
出参说明：
返回值  ：
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
函数说明：不加锁的增加元素
入参说明：
出参说明：
返回值  ：
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::nlinsert(TF id,TM inform)
{
	//m_list.insert(map<TF,TM>::value_type(id,inform));
	m_list.insert(Map_Value(id,inform));
	return true;
}

/*********************************************************
函数说明：移除元素
入参说明：
出参说明：
返回值  ：
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
函数说明：不加锁的移除元素
入参说明：
出参说明：
返回值  ：
*********************************************************/
template <class TF,class TM>
bool CMap<TF,TM>::nlerase(TF id)
{
	m_list.erase(id);

	return true;
}

/*********************************************************
函数说明：判断map是否为空
入参说明：
出参说明：
返回值  ：
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
函数说明：清空map
入参说明：
出参说明：
返回值  ：
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
