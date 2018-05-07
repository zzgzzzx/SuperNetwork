/*********************************************************
模块名       : 
文件名       :ShareMem.hpp
相关文件     : 
文件实现功能 :信息类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __SHARE_MEMEORY_HPP__
#define __SHARE_MEMORY_HPP__

#include "NDStruct.hpp"
#include "NDType.hpp"

//**********************************
//共享内存类                       
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
