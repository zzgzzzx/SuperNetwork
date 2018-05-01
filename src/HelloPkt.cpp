/*********************************************************
ģ����       : 
�ļ���       :HelloPkt.cpp
����ļ�     :HelloPkt.hpp
�ļ�ʵ�ֹ��� :hello���ݰ���
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "HelloPkt.hpp"
#include "NDFunc.hpp"
#include "cJSON.h"

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHelloPkt::CHelloPkt()
{
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHelloPkt::~CHelloPkt()
{

}

/*********************************************************
����˵������װ���ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CHelloPkt::MakeHelloPkt()
{
    char *out;
    cJSON *root, *fmt, *actions;

	AfxWriteDebugLog("SuperVPN run at [CHelloPkt::MakeHelloPkt] Make Hello actions");

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVICES_CHECK_HELLO);
	cJSON_AddStringToObject(fmt, "arugments", "");
	cJSON_AddStringToObject(fmt, "error", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;

	return true;
}


/*********************************************************
����˵�����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CHelloPkt::AnalysePkt()	
{
	return true;
}

/*********************************************************
����˵����Ӧ��hello���ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CHelloPkt::SendHelloPkt()	
{
	MakeHelloPkt();
	CHelloSrvThread *pThread = AfxGetHelloSrv();
	if(pThread == NULL) return false;
	
	pThread->SendTo(mSndNP.lSrcIP, mSndNP.uSrcPort, mSendBuf.c_str(), mSendBuf.length());
	return true;
}


