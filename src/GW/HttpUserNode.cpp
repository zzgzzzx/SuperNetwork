/*********************************************************
模块名       : 结点http协议
文件名       : HttpUserNode.cpp
相关文件     : HttpUserNode.h
文件实现功能 : 结点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpUserNode.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"


/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpUserNode::CHttpUserNode(CNodeBase *node):CHttpGeneral(node)
{
}

/*********************************************************
函数说明：获取服务器列表
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeServerListReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

	mSrvURL = URL_NODE_GET_SERVER_LIST;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_USER);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeServerListReq] Make ServerList actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_GET_SERVER_LIST);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}


/*********************************************************
函数说明：节点Hello
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeNodeHelloReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_USER);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::MakeNodeHelloReq] Make Hello actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_HELLO);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：节点Hello返回处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::AnalysisNodeHelloRsp()
{
	AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::AnalysisNodeHelloRsp] Recv Hello actions");

	return ND_SUCCESS;
}

/*********************************************************
函数说明：节点初始化
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeNodeInitReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

	mSrvURL = URL_USER_NODE_INIT;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_USER);
    cJSON_AddStringToObject(fmt, "mac", mPNode->GetNodeInform().sNodeMac.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::MakeNodeEnvSetReq] Make Init actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_INIT);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);

    return ND_SUCCESS;
}

/*********************************************************
函数说明：获取IP
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeNodeGetIPReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

	mSrvURL = URL_USER_NODE_GETIP;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_USER);
    cJSON_AddStringToObject(fmt, "mac", mPNode->GetNodeInform().sNodeMac.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::MakeNodeGetIPReq] Make Init actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_GETIP);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);

    return ND_SUCCESS;
}


/*********************************************************
函数说明：节点配置
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeNodeEnvSetReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

	mSrvURL = URL_USER_NODE_SET;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_USER);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::MakeNodeEnvSetReq] Make EnvSet actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_SET);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);

    return ND_SUCCESS;
}

/*********************************************************
函数说明：节点配置请求返回处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::AnalysisNodeEnvSetRsp()
{
	SNodeInform &sNode = mPNode->GetNodeInform();
	//解析返回的数据到NdoeInform

    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::AnalysisNodeEnvSetRsp] Error before: [%s]", cJSON_GetErrorPtr());
        return ND_ERROR_INVALID_RESPONSE;
    }

    cJSON *actionsArray = cJSON_GetObjectItem(root, "actions");
    if(actionsArray != NULL)
    {

        cJSON *actionslist = actionsArray->child;

        iErrCode = cJSON_GetObjectItem(actionslist, "error")->valueint;
        if(iErrCode != 0)
		{
			cJSON_Delete(root);
			return ND_ERROR_INVALID_RESPONSE;
        }		
        
        cJSON *replices = cJSON_GetObjectItem(root, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;

			sNode.lHelloTime = cJSON_GetObjectItem(repliceslist, "hellotime")->valueint;
			sNode.lRestartTime = cJSON_GetObjectItem(repliceslist, "restarttime")->valueint;
				
            cJSON *supernode = cJSON_GetObjectItem(repliceslist, "supernode");
            if(supernode != NULL)
            {
                    cJSON *supernodelist = supernode->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::AnalysisNodeEnvSetRsp] Get SuperNode Informs");
                    SSupperNode item;
                    while(supernodelist != NULL)
                    {
                        if(cJSON_GetObjectItem(supernodelist, "nodeip") != NULL &&
                           cJSON_GetObjectItem(supernodelist, "nodeip")->valuestring != NULL)
                            item.sSuperNodeIP = cJSON_GetObjectItem(supernodelist, "nodeip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::AnalysisNodeEnvSetRsp] supernode ip=[%s]",item.sSuperNodeIP.c_str());

                        if(cJSON_GetObjectItem(supernodelist, "nodeport") != NULL &&
                           cJSON_GetObjectItem(supernodelist, "nodeport")->valuestring != NULL)
                            item.sSuperNodePort = cJSON_GetObjectItem(supernodelist, "nodeport")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::AnalysisNodeEnvSetRsp] supernode port=[%s]",item.sSuperNodePort.c_str());

                        sNode.mSupperNode.push_back(item);

                        supernodelist = supernodelist->next;
                    }
                }

            cJSON *ippools = cJSON_GetObjectItem(repliceslist, "ippool");
            if(ippools != NULL)
            {
                    cJSON *ippoolslist = ippools->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::AnalysisNodeEnvSetRsp] Get IPPools");
                    while(ippoolslist != NULL)
                    {
                        if(cJSON_GetObjectItem(ippoolslist, "begin") != NULL &&
                           cJSON_GetObjectItem(ippoolslist, "begin")->valuestring != NULL)
                            sNode.mIPPool.uBeginIP = inet_addr(cJSON_GetObjectItem(ippoolslist, "domainid")->valuestring);
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] beginip=[%s]", AfxHostIPToStr(sNode.mIPPool.uBeginIP));

                        if(cJSON_GetObjectItem(ippoolslist, "end") != NULL &&
                           cJSON_GetObjectItem(ippoolslist, "end")->valuestring != NULL)
                            sNode.mIPPool.uEndIP = inet_addr(cJSON_GetObjectItem(ippoolslist, "domainid")->valuestring);
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] endip=[%s]", AfxHostIPToStr(sNode.mIPPool.uEndIP));

                        break;
                    }
			
            }
        }

        cJSON_Delete(root);
    }	

	mPNode->SetNodeInform(sNode);
	
	return ND_SUCCESS;	

}

