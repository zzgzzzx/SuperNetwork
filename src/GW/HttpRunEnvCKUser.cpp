/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpUpdateCK.cpp
����ļ�     : HttpUpdateCK.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpRunEnvCKUser.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"

extern CBaseApp *gPSuperVPNApp;

/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpRunEvnCKUser::CHttpRunEvnCKUser(CNodeBase *node):CHttpRunEvnCK(node)
{
	mSrvURL = URL_USER_NODE_ENV_CHECK;
	mLocalVersion = SUPER_VPN_CLIENT_VER_USER;
}

/*********************************************************
����˵���������������ݰ�
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpRunEvnCKUser::MakeCheckReq()
{
    char *out;
    cJSON *root, *fmt, *actions, *arugments;

    //��װ��Ϣ��
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

