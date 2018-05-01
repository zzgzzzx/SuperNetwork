/*********************************************************
模块名       : 
文件名       :HelloPkt.cpp
相关文件     :HelloPkt.hpp
文件实现功能 :hello数据包类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "HelloPkt.hpp"
#include "NDFunc.hpp"
#include "cJSON.h"

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CHelloPkt::CHelloPkt()
{
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
CHelloPkt::~CHelloPkt()
{

}

/*********************************************************
函数说明：组装数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CHelloPkt::MakeHelloPkt()
{
    char *out;
    cJSON *root, *fmt, *actions;

	AfxWriteDebugLog("SuperVPN run at [CHelloPkt::MakeHelloPkt] Make Hello actions");

    //组装消息体
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
函数说明：分析数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CHelloPkt::AnalysePkt()	
{
	return true;
}

/*********************************************************
函数说明：应答hello数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CHelloPkt::SendHelloPkt()	
{
	MakeHelloPkt();
	CHelloSrvThread *pThread = AfxGetHelloSrv();
	if(pThread == NULL) return false;
	
	pThread->SendTo(mSndNP.lSrcIP, mSndNP.uSrcPort, mSendBuf.c_str(), mSendBuf.length());
	return true;
}


