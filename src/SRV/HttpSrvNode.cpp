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

extern CBaseApp *gPSuperVPNApp;

/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpSrvNode::CHttpSrvNode(CNodeBase *node):CHttpGeneral(node)
{
}

/*********************************************************
函数说明：节点初始化
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpSrvNode::MakeNodeInitReq()
{
    char *out, subtype[64]={0};
    cJSON *root, *fmt, *actions, *arguments;

	mSrvURL = URL_SERVER_NODE_INIT;

    //组装消息体
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeEnvSetReq] Make Init actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuperVPNApp != NULL) strcpy(subtype, pSuperVPNApp->GetDeviceType().c_str());
	
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVER_NODE_INIT);
	cJSON_AddItemToObject(fmt, "arguments", arguments=cJSON_CreateObject());	
    cJSON_AddStringToObject(arguments, "mac", mPNode->GetNodeInform().sNodeMac.c_str());
	cJSON_AddStringToObject(fmt, "subtype", subtype);

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
        
        cJSON *replices = cJSON_GetObjectItem(actionslist, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;

			if(repliceslist != NULL)
			{
				sNode.lRestartTime = cJSON_GetObjectItem(repliceslist, "restarttime")->valueint;				
	            cJSON *supernode = cJSON_GetObjectItem(repliceslist, "server");
	            if(supernode != NULL)
	            {
	                    cJSON *supernodelist = supernode->child;
						sNode.mSupperNode.clear();

	                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Get SuperNode Informs");
	                    SSupperNode item;
	                    while(supernodelist != NULL)
	                    {
	                        if(cJSON_GetObjectItem(supernodelist, "host") != NULL &&
	                           cJSON_GetObjectItem(supernodelist, "host")->valuestring != NULL)
	                            item.sSuperNodeHost = cJSON_GetObjectItem(supernodelist, "host")->valuestring;
							AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] supernode host=[%s]",item.sSuperNodeHost.c_str());

	                        sNode.mSupperNode.push_back(item);
	                        supernodelist = supernodelist->next;
	                    }
	            }else
	            {
		             cJSON_Delete(root);
					 return ND_ERROR_INVALID_RESPONSE;
	            }

	            cJSON *domains = cJSON_GetObjectItem(repliceslist, "domains");
	            if(domains != NULL)
	            {
	                    cJSON *domainslist = domains->child;

	                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Get Domains");
						sNode.mDomainInforms.clear();
						
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
	            	}else{
		             	cJSON_Delete(root);
					 	return ND_ERROR_INVALID_RESPONSE;
	            	}
				}else{
		             cJSON_Delete(root);
					 return ND_ERROR_INVALID_RESPONSE;
	            }
        	}else{
				cJSON_Delete(root);
				return ND_ERROR_INVALID_RESPONSE;
			}       
    }	else{
    	cJSON_Delete(root);
		return ND_ERROR_INVALID_RESPONSE;
    }

	cJSON_Delete(root);
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
    cJSON *root, *fmt, *actions, *arguments;

	mSrvURL = URL_SERVER_NODE_SET;

    //组装消息体
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeEnvSetReq] Make EnvSet actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVER_NODE_SET);
	cJSON_AddItemToObject(fmt, "arguments", arguments=cJSON_CreateObject());	
    cJSON_AddStringToObject(arguments, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());	

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);

    return ND_SUCCESS;
}