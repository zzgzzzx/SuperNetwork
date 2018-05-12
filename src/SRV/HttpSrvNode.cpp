/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpSrvNode.cpp
����ļ�     : HttpSrvNode.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpSrvNode.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"

extern CBaseApp *gPSuperVPNApp;

/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpSrvNode::CHttpSrvNode(CNodeBase *node):CHttpGeneral(node)
{
}

/*********************************************************
����˵�����ڵ��ʼ��
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpSrvNode::MakeNodeInitReq()
{
    char *out, subtype[64]={0};
    cJSON *root, *fmt, *actions, *arguments;

	mSrvURL = URL_SERVER_NODE_INIT;

    //��װ��Ϣ��
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
����˵�����ڵ��������󷵻ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpSrvNode::AnalysisNodeEnvSetRsp()
{
	SNodeInform &sNode = mPNode->GetNodeInform();
	//�������ص����ݵ�NdoeInform

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
����˵�����ڵ�����
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpSrvNode::MakeNodeEnvSetReq()
{
    char *out;
    cJSON *root, *fmt, *actions, *arguments;

	mSrvURL = URL_SERVER_NODE_SET;

    //��װ��Ϣ��
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