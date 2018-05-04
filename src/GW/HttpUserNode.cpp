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

extern CBaseApp *gPSuperVPNApp;

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
函数说明：节点初始化
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeNodeInitReq()
{
    char *out;
    cJSON *root, *fmt, *actions, *arguments;

	mSrvURL = URL_USER_NODE_INIT;

    //组装消息体
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeEnvSetReq] Make Init actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());
	
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_INIT);
	cJSON_AddItemToObject(fmt, "arguments", arguments=cJSON_CreateObject());	
    cJSON_AddStringToObject(arguments, "mac", mPNode->GetNodeInform().sNodeMac.c_str());

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
    cJSON *root, *fmt, *actions, *arguments;

	mSrvURL = URL_USER_NODE_SET;

    //组装消息体
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::MakeNodeEnvSetReq] Make EnvSet actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_SET);
	cJSON_AddItemToObject(fmt, "arguments", arguments=cJSON_CreateObject());	
    cJSON_AddStringToObject(arguments, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());	

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
        
        cJSON *replices = cJSON_GetObjectItem(actionslist, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;

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

                        if(cJSON_GetObjectItem(supernodelist, "nodeport") != NULL)
                            item.iSuperNodePort = cJSON_GetObjectItem(supernodelist, "nodeport")->valueint;
						AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::AnalysisNodeEnvSetRsp] supernode port=[%d]",item.iSuperNodePort);

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

/*********************************************************
函数说明：失联服务通知
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeServiceErrorReq(SBindInform &sBI)
{
    char *out, arugments[64]={0};
    cJSON *root, *fmt, *actions, *arguments, *services;

	mSrvURL = URL_NODE_SERVICES_ERROR;

    //组装消息体
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeBindServerReq] Make ServerList actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_SERVICES_ERROR);
	cJSON_AddItemToObject(fmt, "arguments", arguments=cJSON_CreateObject());	
    cJSON_AddStringToObject(arguments, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());
	
	cJSON_AddItemToObject(arguments, "services", services = cJSON_CreateArray());
	cJSON_AddItemToArray(services, fmt = cJSON_CreateObject());
	cJSON_AddStringToObject(fmt, "ip", sBI.sServiceIP.c_str());

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：失联服务通知
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::AnalysisServiceErrorRsp(SBindInform &sBI)
{
    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServiceErrorRsp] Error before: [%s]", cJSON_GetErrorPtr());
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

            cJSON *servers = cJSON_GetObjectItem(repliceslist, "services");
            if(servers != NULL)
            {
                    cJSON *serverslist = servers->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServiceErrorRsp] Get servers");
                    SBindInform item;
                    while(serverslist != NULL)
                    {
                        if(cJSON_GetObjectItem(serverslist, "oldservicesip") != NULL &&
                           cJSON_GetObjectItem(serverslist, "oldservicesip")->valuestring != NULL)
                            sBI.sServiceIP = cJSON_GetObjectItem(serverslist, "oldservicesip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] oldservicesip ip=[%s]",sBI.sServiceIP.c_str());

                        if(cJSON_GetObjectItem(serverslist, "newservicesip") != NULL &&
                           cJSON_GetObjectItem(serverslist, "newservicesip")->valuestring != NULL)
                            sBI.sServiceIP = cJSON_GetObjectItem(serverslist, "newservicesip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] newservicesip ip=[%s]",sBI.sServiceIP.c_str());

                        break;
                    }			
            }
        }

        cJSON_Delete(root);
    }	
	
	return ND_SUCCESS;	
}
	
/*********************************************************
函数说明：组合获取服务出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeBindServerReq(list<SBindInform> &ltBSer)
{
    char *out;
	SBindInform sBI;
    cJSON *root, *fmt, *actions, *arguments, *devices;

	mSrvURL = URL_NODE_GET_SERVICES;

    //组装消息体
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeBindServerReq] Make ServerList actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_GET_SERVICES);

	cJSON_AddItemToObject(fmt, "arguments", arguments=cJSON_CreateObject());	
    cJSON_AddStringToObject(arguments, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());	

	cJSON_AddItemToObject(fmt, "devices", devices = cJSON_CreateArray());	
	
	BindInformItr iter = ltBSer.begin();
	while(iter != ltBSer.end())
	{
	    sBI = *iter;
		cJSON_AddItemToArray(devices, fmt = cJSON_CreateObject());
    	cJSON_AddStringToObject(fmt, "deviceflag", sBI.sDeviceFlag.c_str());
		iter++;
	}

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：分析获取服务出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::AnalysisBindServerRsp(list<SBindInform> &mServers)
{
    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] Error before: [%s]", cJSON_GetErrorPtr());
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

		mServers.clear();
		
        cJSON *replices = cJSON_GetObjectItem(actionslist, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;

            cJSON *servers = cJSON_GetObjectItem(repliceslist, "services");
            if(servers != NULL)
            {
                    cJSON *serverslist = servers->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] Get servers");
                    SBindInform item;
                    while(serverslist != NULL)
                    {
                        if(cJSON_GetObjectItem(serverslist, "deviceflag") != NULL &&
                           cJSON_GetObjectItem(serverslist, "deviceflag")->valuestring != NULL)
                            item.sDeviceFlag = cJSON_GetObjectItem(serverslist, "deviceflag")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] deviceflag=[%s]", item.sDeviceFlag.c_str());

                        if(cJSON_GetObjectItem(serverslist, "serviceip") != NULL &&
                           cJSON_GetObjectItem(serverslist, "serviceip")->valuestring != NULL)
                            item.sServiceIP = cJSON_GetObjectItem(serverslist, "serviceip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] serviceip=[%s]", item.sServiceIP.c_str());

                        mServers.push_back(item);
                        serverslist = serverslist->next;
                    }
			
            }
        }

        cJSON_Delete(root);
    }	
	
	return ND_SUCCESS;	
}

/*********************************************************
函数说明：组合释放服务出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::MakeUnBindServerReq(SBindInform sBI)
{
    char *out;
    cJSON *root, *fmt, *actions, *arguments, *devices;

	mSrvURL = URL_NODE_RELEASE_SERVICES;

    //组装消息体
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeUnBindServerReq] Make ServerList actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_RELEASE_SERVICES);
	cJSON_AddItemToObject(fmt, "arguments", arguments=cJSON_CreateObject());	
    cJSON_AddStringToObject(arguments, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());	

	cJSON_AddItemToObject(fmt, "devices", devices = cJSON_CreateArray());		
	cJSON_AddItemToArray(devices, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "deviceflag", sBI.sDeviceFlag.c_str());


    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：分析释放服务出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::AnalysisUnBindServerRsp()
{
    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisUnBindServerRsp] Error before: [%s]", cJSON_GetErrorPtr());
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
			AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisUnBindServerRsp] Return Error=[%d]", iErrCode);
			return ND_ERROR_INVALID_RESPONSE;
        }
        cJSON_Delete(root);
    }	
	
	return ND_SUCCESS;	
}

/*********************************************************
函数说明：获取服务器出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::GetIdentifyService(list<SBindInform> &ltBSer)
{
    //组装数据包
    ndStatus  ret = MakeBindServerReq(ltBSer);
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//数据包发送
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//数据包解析
    ret = AnalysisBindServerRsp(ltBSer);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;	
}

/*********************************************************
函数说明：释放服务器出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::ReleaseIdentifyService(SBindInform sBI)
{
    //组装数据包
    ndStatus  ret = MakeUnBindServerReq(sBI);
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ReleaseIdentifyService] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//数据包发送
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ReleaseIdentifyService] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//数据包解析
    ret = AnalysisUnBindServerRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ReleaseIdentifyService] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;	
}

/*********************************************************
函数说明：失联服务通知中心变更
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUserNode::ServiceErrorNotify(SBindInform &sBindInform)
{
    //组装数据包
    ndStatus  ret = MakeServiceErrorReq(sBindInform);
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ServiceErrorNotify] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//数据包发送
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ServiceErrorNotify] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//数据包解析
    ret = AnalysisServiceErrorRsp(sBindInform);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ServiceErrorNotify] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;	
}


