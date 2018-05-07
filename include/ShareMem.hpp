/*********************************************************
ģ����       : 
�ļ���       :ShareMem.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :��Ϣ��
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __SHARE_MEMEORY_HPP__
#define __SHARE_MEMORY_HPP__

#include "NDStruct.hpp"
#include "NDType.hpp"

//**********************************
//�����ڴ���                       
//**********************************
class CShareMem
{
private:
	int    mShmid;	
	SShareMemFlag *mPMemFlag;
	
	void CreateMem();
	void FreeMem();
public:
	CShareMem();
	virtual ~CShareMem();

	ndBool GetMemValue();
	void SetMemValue(ndBool value);	
};

#endif
