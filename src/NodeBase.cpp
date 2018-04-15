/*********************************************************
ģ����       : SDK���ݴ������
�ļ���       : ClientBase.cpp
����ļ�     : ClientBase.h
�ļ�ʵ�ֹ��� : ���๦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeBase.hpp"
#include "NDFunc.hpp"


/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeBase::CNodeBase()
{
	//��ȡ������Mac��ַ
	//mSNodeInform.sNodeMac = "bb:bb:bb:bb:bb:bc";
	//mSNodeInform.sNodeMac = "aa:aa:aa:aa:aa:cb";
	
    mErrCode = ND_SUCCESS;
    mCallBackFunc = NULL;
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeBase::~CNodeBase()
{

}

/*********************************************************
����˵�������ýڵ������Ϣ
���˵������
����˵������
����ֵ  ����
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
����˵�������ý���helloʱ��Ҫ���ӵ��µ���(���ڷ���ڵ�)
���˵������
����˵������
����ֵ  ����
*********************************************************/
void CNodeBase::DealHelloAddNewDomain(list<SDomain> &domain)
{
}


/*********************************************************
����˵�������ýڵ�����Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
void CNodeBase::SetNodeID(ndString nodeid)
{
	mSNodeInform.sNodeID = nodeid;
}


/*********************************************************
����˵������ȡ�ڵ������Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
SNodeInform &CNodeBase::GetNodeInform()
{
	return mSNodeInform;
}


/*********************************************************
����˵������ȡ������
���˵������
����˵������
����ֵ  ����������Ϣ
*********************************************************/
ndULong CNodeBase::GetLastErrCode()
{
    return mErrCode;
}

/*********************************************************
����˵�������ô�����
���˵����code-������ֵ
����˵������
����ֵ  ����
*********************************************************/
void CNodeBase::SetLastErrCode(ndULong code)
{
    mErrCode = code;
}

/*********************************************************
����˵�������ûص�����
���˵����callbackFunction-�ص�����
����˵������
����ֵ  ����
*********************************************************/
void CNodeBase::SetAsyncMode(ndCallbackFunction callbackFunction)
{
    mCallBackFunc = callbackFunction;
}

/*********************************************************
����˵������ȡ�ص�����
���˵������
����˵������
����ֵ  ���ص�����
*********************************************************/
ndCallbackFunction CNodeBase::GetCallBackFunction()
{
    return mCallBackFunc;
}

/*********************************************************
����˵�����̴߳�����
���˵����param-�߳����ݲ���ָ��
����˵������
����ֵ  ���̷߳���ֵ
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
