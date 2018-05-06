/*********************************************************
模块名       : 节点http协议
文件名       : HttpUpdateCK.cpp
相关文件     : HttpUpdateCK.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpRunEnvCKUser.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"

extern CBaseApp *gPSuperVPNApp;

/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpRunEvnCKUser::CHttpRunEvnCKUser(CNodeBase *node):CHttpRunEvnCK(node)
{
	mSrvURL = URL_USER_NODE_ENV_CHECK;
	mLocalVersion = SUPER_VPN_CLIENT_VER_USER;
}

/*********************************************************
函数说明：发送请求数据包
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCKUser::MakeCheckReq()
{
    char *out;
    cJSON *root, *fmt, *actions, *arugments;

    //组装消息体
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::MakeCheckReq] Make check actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_ENV_CHECK);	
	cJSON_AddItemToObject(actions, "devparams", arugments = cJSON_CreateArray());
	cJSON_AddItemToArray(arugments, fmt = cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "checktime", AfxGetTaskTime());

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

