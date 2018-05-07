/****************** Copyright (c) 2008 **************
模块名       : 
文件名       :ShareMem.cpp
相关文件     :ShareMem.hpp
文件实现功能 :基本信息类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "ShareMem.hpp"

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CShareMem::CShareMem()
{
	mPMemFlag = NULL;
	CreateMem();
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CShareMem::~CShareMem()
{
	FreeMem();
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CShareMem::CreateMem()
{
	//创建共享内存 ，相当于打开文件，文件不存在则创建
	mShmid = shmget(0x2234, sizeof(SShareMemFlag), IPC_CREAT | 0666); 
	if (mShmid == -1)
	{
		return;
	}

	//将共享内存段连接到进程地址空间
	mPMemFlag = (SShareMemFlag *)shmat(mShmid, NULL, 0);
	if (mPMemFlag == (void *)-1 )
	{
		mPMemFlag = NULL;
	}
	mPMemFlag->bFlag = ND_FALSE;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CShareMem::FreeMem()
{
	if (mPMemFlag != NULL)
		shmdt(mPMemFlag);
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndBool CShareMem::GetMemValue()
{
	if(mPMemFlag != NULL)
		return mPMemFlag->bFlag;
	else
		return ND_FALSE;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CShareMem::SetMemValue(ndBool value)
{
	if(mPMemFlag != NULL)
		mPMemFlag->bFlag = value;
}

