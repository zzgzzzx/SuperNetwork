/*********************************************************
模块名       : SDK数据处理基类
文件名       : ClientBase.cpp
相关文件     : ClientBase.h
文件实现功能 : 基类功能
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeBase.hpp"
#include "NDFunc.hpp"


/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeBase::CNodeBase()
{
	//获取本机的Mac地址
	//mSNodeInform.sNodeMac = "bb:bb:bb:bb:bb:bc";
	//mSNodeInform.sNodeMac = "aa:aa:aa:aa:aa:cb";
	
    mErrCode = ND_SUCCESS;
    mCallBackFunc = NULL;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeBase::~CNodeBase()
{

}

/*********************************************************
函数说明：设置节点参数信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
void CNodeBase::SetNodeInform(SNodeInform &inform)
{
	inform.sNodeMac = mSNodeInform.sNodeMac;
	mSNodeInform = inform;
	SDomain domain;

	list<SDomain>::iterator iterDomain;
    for(iterDomain=mSNodeInform.mDomainInforms.begin(); iterDomain!=mSNodeInform.mDomainInforms.end(); iterDomain++)
    {
    	domain = *iterDomain;
		mSNodeInform.mDomainMap.insert(domain.sDomain, domain);
    }
}

/*********************************************************
函数说明：设置接收hello时需要增加的新的域(用于服务节点)
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
void CNodeBase::DealHelloAddNewDomain(list<SDomain> &domain)
{
}


/*********************************************************
函数说明：设置节点编号信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
void CNodeBase::SetNodeID(ndString nodeid)
{
	mSNodeInform.sNodeID = nodeid;
}


/*********************************************************
函数说明：获取节点参数信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
SNodeInform &CNodeBase::GetNodeInform()
{
	return mSNodeInform;
}


/*********************************************************
函数说明：获取错误码
入参说明：无
出参说明：无
返回值  ：错误码信息
*********************************************************/
ndULong CNodeBase::GetLastErrCode()
{
    return mErrCode;
}

/*********************************************************
函数说明：设置错误码
入参说明：code-错误码值
出参说明：无
返回值  ：无
*********************************************************/
void CNodeBase::SetLastErrCode(ndULong code)
{
    mErrCode = code;
}

/*********************************************************
函数说明：设置回调处理
入参说明：callbackFunction-回调函数
出参说明：无
返回值  ：无
*********************************************************/
void CNodeBase::SetAsyncMode(ndCallbackFunction callbackFunction)
{
    mCallBackFunc = callbackFunction;
}

/*********************************************************
函数说明：获取回调函数
入参说明：无
出参说明：无
返回值  ：回调函数
*********************************************************/
ndCallbackFunction CNodeBase::GetCallBackFunction()
{
    return mCallBackFunc;
}

/*********************************************************
函数说明：线程处理函数
入参说明：param-线程数据参数指针
出参说明：无
返回值  ：线程返回值
*********************************************************/
void *CNodeBase::ThreadFunction(void *param)
{
    CNodeBase *clientBase = (CNodeBase*)(param);
    if (clientBase != NULL)
    {
        AfxWriteDebugLog("SuperVPN run at [CNodeBase::ThreadFunction] Thread DealActionWithModel");
        if(clientBase->GetLastErrCode() == ND_SUCCESS){
            AfxWriteDebugLog("SuperVPN run at [CNodeBase::ThreadFunction] Set Last]\n");
            clientBase->SetLastErrCode(ND_SUCCESS);
        }
        ndCallbackFunction callbackFunction = clientBase->GetCallBackFunction();
        if( callbackFunction!= NULL){
            callbackFunction(clientBase);
        }
        delete clientBase;
    }
    AfxWriteDebugLog("SuperVPN run at [CNodeBase::ThreadFunction] Thread End\n");
    pthread_exit(0);
}
