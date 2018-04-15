/*********************************************************
模块名       : 节点http协议
文件名       : HttpSrvNode.cpp
相关文件     : HttpSrvNode.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpSrvNode.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"

/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpSrvNode::CHttpSrvNode(CNodeBase *node):CHttpGeneral(node)
{
	mSrvURL = VPN_CENTER_SRV_URL;
}

/*********************************************************
函数说明：获取服务器列表
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpSrvNode::MakeServerListReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_SERVER);
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
ndStatus CHttpSrvNode::MakeNodeHelloReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_SERVER);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeHelloReq] Make Hello actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVER_NODE_HELLO);
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
ndStatus CHttpSrvNode::AnalysisNodeHelloRsp()
{
	AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::AnalysisNodeHelloRsp] Recv Hello actions");

    cJSON *root;
	int iErrCode;
	list<SDomain> listDomains;	

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::AnalysisNodeHelloRsp] Error before: [%s]", cJSON_GetErrorPtr());
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

            cJSON *domains = cJSON_GetObjectItem(repliceslist, "domains");
            if(domains != NULL)
            {
                    cJSON *domainslist = domains->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeHelloRsp] Get Domains");
                    SDomain item;
                    while(domainslist != NULL)
                    {
                        if(cJSON_GetObjectItem(domainslist, "domainid") != NULL &&
                           cJSON_GetObjectItem(domainslist, "domainid")->valuestring != NULL)
                            item.sDomain = cJSON_GetObjectItem(domainslist, "domainid")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeHelloRsp] domain id=[%s]",item.sDomain.c_str());

                        if(cJSON_GetObjectItem(domainslist, "ip") != NULL &&
                           cJSON_GetObjectItem(domainslist, "ip")->valuestring != NULL)
                            item.lNodeIP = cJSON_GetObjectItem(domainslist, "ip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeHelloRsp] domain ip=[%s]",item.lNodeIP.c_str());

                        if(cJSON_GetObjectItem(domainslist, "mask") != NULL &&
                           cJSON_GetObjectItem(domainslist, "mask")->valuestring != NULL)
                            item.lMask = cJSON_GetObjectItem(domainslist, "mask")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeHelloRsp] domain mask=[%s]",item.lMask.c_str());

                        if(cJSON_GetObjectItem(domainslist, "key") != NULL &&
                           cJSON_GetObjectItem(domainslist, "key")->valuestring != NULL)
                            item.sDomainKey = cJSON_GetObjectItem(domainslist, "key")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeHelloRsp] domain key=[%s]",item.sDomainKey.c_str());

                        listDomains.push_back(item);

                        domainslist = domainslist->next;
                    }
			
            }
        }

        cJSON_Delete(root);
    }		
	mPNode->DealHelloAddNewDomain(listDomains);
	return ND_SUCCESS;
}

/*********************************************************
函数说明：节点初始化
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpSrvNode::MakeNodeInitReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_SERVER);
    cJSON_AddStringToObject(fmt, "mac", mPNode->GetNodeInform().sNodeMac.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeEnvSetReq] Make Init actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVER_NODE_INIT);
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
ndStatus CHttpSrvNode::AnalysisNodeEnvSetRsp()
{
	SNodeInform &sNode = mPNode->GetNodeInform();
	//解析返回的数据到NdoeInform

    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Error before: [%s]", cJSON_GetErrorPtr());
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

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Get SuperNode Informs");
                    SSupperNode item;
                    while(supernodelist != NULL)
                    {
                        if(cJSON_GetObjectItem(supernodelist, "nodeip") != NULL &&
                           cJSON_GetObjectItem(supernodelist, "nodeip")->valuestring != NULL)
                            item.sSuperNodeIP = cJSON_GetObjectItem(supernodelist, "nodeip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] supernode ip=[%s]",item.sSuperNodeIP.c_str());

                        if(cJSON_GetObjectItem(supernodelist, "nodeport") != NULL &&
                           cJSON_GetObjectItem(supernodelist, "nodeport")->valuestring != NULL)
                            item.sSuperNodePort = cJSON_GetObjectItem(supernodelist, "nodeport")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] supernode port=[%s]",item.sSuperNodePort.c_str());

                        sNode.mSupperNode.push_back(item);

                        supernodelist = supernodelist->next;
                    }
                }

            cJSON *domains = cJSON_GetObjectItem(repliceslist, "domains");
            if(domains != NULL)
            {
                    cJSON *domainslist = domains->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Get Domains");
                    SDomain item;
                    while(domainslist != NULL)
                    {
                        if(cJSON_GetObjectItem(domainslist, "domainid") != NULL &&
                           cJSON_GetObjectItem(domainslist, "domainid")->valuestring != NULL)
                            item.sDomain = cJSON_GetObjectItem(domainslist, "domainid")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] domain id=[%s]",item.sDomain.c_str());

                        if(cJSON_GetObjectItem(domainslist, "ip") != NULL &&
                           cJSON_GetObjectItem(domainslist, "ip")->valuestring != NULL)
                            item.lNodeIP = cJSON_GetObjectItem(domainslist, "ip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] domain ip=[%s]",item.lNodeIP.c_str());

                        if(cJSON_GetObjectItem(domainslist, "mask") != NULL &&
                           cJSON_GetObjectItem(domainslist, "mask")->valuestring != NULL)
                            item.lMask = cJSON_GetObjectItem(domainslist, "mask")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] domain mask=[%s]",item.lMask.c_str());

                        if(cJSON_GetObjectItem(domainslist, "key") != NULL &&
                           cJSON_GetObjectItem(domainslist, "key")->valuestring != NULL)
                            item.sDomainKey = cJSON_GetObjectItem(domainslist, "key")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] domain key=[%s]",item.sDomainKey.c_str());

                        sNode.mDomainInforms.push_back(item);

                        domainslist = domainslist->next;
                    }
			
            }
        }

        cJSON_Delete(root);
    }	

	mPNode->SetNodeInform(sNode);
	
	return ND_SUCCESS;	
}

/*********************************************************
函数说明：节点配置
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpSrvNode::MakeNodeEnvSetReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_SERVER);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeEnvSetReq] Make Init actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVER_NODE_SET);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);

    return ND_SUCCESS;
}