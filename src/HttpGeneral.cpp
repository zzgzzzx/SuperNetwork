/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpClientNode.cpp
����ļ�     : HttpClientNode.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpGeneral.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"

extern list<SServerInfo> gServers;
extern list<SServerInfo>::iterator gITCurServer,gITBakServer;

/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CHttpGeneral::CHttpGeneral()
{
	mPNode = NULL;
}

/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpGeneral::CHttpGeneral(CNodeBase *node)
{
	mPNode = node;
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CHttpGeneral::~CHttpGeneral()
{

}

/*********************************************************
����˵�����ڵ��ʼ������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeNodeInitReq()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ��ʼ�����󷵻ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeInitRsp()
{	
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
			mPNode->SetNodeID(cJSON_GetObjectItem(repliceslist, "nodeid")->valuestring);
        }

        cJSON_Delete(root);
    }	
	
	return ND_SUCCESS;	
}

/*********************************************************
����˵�����ڵ���������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeNodeEnvSetReq()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ��������󷵻ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeEnvSetRsp()
{
	return ND_SUCCESS;	
}

/*********************************************************
����˵�����ڵ�Hello
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeNodeHelloReq()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ�Hello���ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeHelloRsp()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵����ʧ������֪ͨ
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeServiceErrorReq(SBindInform &sBI)
{
    char *out, arugments[64]={0};
    cJSON *root, *fmt, *actions;

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_NODE);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeBindServerReq] Make ServerList actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_SERVICES_ERROR);
	sprintf(arugments, "servicesip=%s", sBI.sServiceIP.c_str());
	cJSON_AddStringToObject(fmt, "arugments", arugments);

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

ndStatus CHttpGeneral::AnalysisServiceErrorRsp(SBindInform &sBI)
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

        cJSON *replices = cJSON_GetObjectItem(root, "replies");
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
����˵������ϻ�ȡ�������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeBindServerReq(ndString devidentify)
{
    char *out;
    cJSON *root, *fmt, *actions;

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_NODE);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeBindServerReq] Make ServerList actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_GET_SERVICES);
	cJSON_AddStringToObject(fmt, "arugments", devidentify.c_str());

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
����˵����������ȡ�������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisBindServerRsp(list<SBindInform> &mServers)
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
		
        cJSON *replices = cJSON_GetObjectItem(root, "replies");
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
����˵��������ͷŷ������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeUnBindServerReq(SBindInform sBI)
{
    char *out;
    cJSON *root, *fmt, *actions;

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_NODE);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeUnBindServerReq] Make ServerList actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_RELEASE_SERVICES);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
����˵���������ͷŷ������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisUnBindServerRsp()
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
����˵�����������б�����
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeServerListReq()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵�����������б��ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisServerListRsp(list<SServerInfo> &mServers)
{
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

		mServers.clear();
		
        cJSON *replices = cJSON_GetObjectItem(root, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;

            cJSON *servers = cJSON_GetObjectItem(repliceslist, "servers");
            if(servers != NULL)
            {
                    cJSON *serverslist = servers->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] Get servers");
                    SServerInfo item;
                    while(serverslist != NULL)
                    {
                        if(cJSON_GetObjectItem(serverslist, "ip") != NULL &&
                           cJSON_GetObjectItem(serverslist, "ip")->valuestring != NULL)
                            item.sServerIP = cJSON_GetObjectItem(serverslist, "ip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] server ip=[%s]",item.sServerIP.c_str());

                        if(cJSON_GetObjectItem(serverslist, "port") != NULL &&
                           cJSON_GetObjectItem(serverslist, "port")->valuestring != NULL)
                            item.sServerPort = cJSON_GetObjectItem(serverslist, "port")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] server port=[%s]",item.sServerPort.c_str());

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
����˵������ȡ�������б�
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndBool CHttpGeneral::GetServerList(list<SServerInfo> &mServers)
{
    //��װ���ݰ�
    ndStatus  ret = MakeServerListReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//���ݰ�����
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//���ݰ�����
    ret = AnalysisServerListRsp(mServers);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;
}

/*********************************************************
����˵����ʧ������֪ͨ���ı��
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::ServiceErrorNotify(SBindInform &sBindInform)
{
    //��װ���ݰ�
    ndStatus  ret = MakeServiceErrorReq(sBindInform);
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ServiceErrorNotify] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//���ݰ�����
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ServiceErrorNotify] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//���ݰ�����
    ret = AnalysisServiceErrorRsp(sBindInform);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ServiceErrorNotify] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;	
}

/*********************************************************
����˵������ȡ����������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::GetIdentifyService(list<SBindInform> &ltBSer)
{
	ndString devidentify;
	SBindInform sBI;
	BindInformItr iter = ltBSer.begin();
	while(iter != ltBSer.end())
	{
		sBI = *iter;
		devidentify += sBI.sDeviceFlag;
		iter++;
		if(iter != ltBSer.end()) devidentify += "&";
	}
	
    //��װ���ݰ�
    ndStatus  ret = MakeBindServerReq(devidentify);
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//���ݰ�����
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//���ݰ�����
    ret = AnalysisBindServerRsp(ltBSer);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;	
}

/*********************************************************
����˵�����ͷŷ���������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::ReleaseIdentifyService(SBindInform sBI)
{
    //��װ���ݰ�
    ndStatus  ret = MakeUnBindServerReq(sBI);
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ReleaseIdentifyService] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//���ݰ�����
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ReleaseIdentifyService] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//���ݰ�����
    ret = AnalysisUnBindServerRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::ReleaseIdentifyService] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;	
}

/*********************************************************
����˵�����ڵ��ʼ������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::NodeInit()
{
    //��װ���ݰ�
    ndStatus  ret = MakeNodeInitReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//���ݰ�����
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//���ݰ�����
    ret = AnalysisNodeInitRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

	//����nodeid�Ĵ洢����
	AfxWriteNodeID(mPNode->GetNodeInform().sNodeID.c_str());

    return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ����ô���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::NodeEnvSet()
{
    //��װ���ݰ�
    ndStatus  ret = MakeNodeEnvSetReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }

	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}

    ret = AnalysisNodeEnvSetRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;
	
}


/*********************************************************
����˵�������Hello����
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::NodeHello()
{
    //��װ���ݰ�
    ndStatus  ret = MakeNodeHelloReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeHello] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }

	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeHello] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}

    ret = AnalysisNodeHelloRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeHello] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;
}

/*********************************************************
����˵�������ݰ����������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::PkgSendAndRecv(ndString url)
{
	ndString sSrvUrl;
    //�жϷ�������ַ
    SServerInfo serverInfo = *gITCurServer;
	sSrvUrl =  "http://"+serverInfo.sServerIP+":"+serverInfo.sServerPort+url;
	
    if(sSrvUrl.empty()){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Http Server Empty Err");
        return ND_ERROR_INVALID_PARAM;
    }	

    //�жϷ��͵����ݰ�
    if(mSendBuf.empty()){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] SendBuff Is Empty");
        return ND_ERROR_INVALID_PARAM;
    }

    AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Begin post data\n ServerURL=[%s]\n [%s]", sSrvUrl.c_str(), mSendBuf.c_str());
rePost:
    //���ͷ���˲����շ���
    CURLcode rtn = Post(sSrvUrl.c_str(), mSendBuf.c_str(), mRcvBuf);
    if(rtn != CURLE_OK){
        while(AfxGetNextSrvUrl(serverInfo)){
			sSrvUrl =  "http://"+serverInfo.sServerIP+":"+serverInfo.sServerPort+url;
            rtn = Post(sSrvUrl.c_str(), mSendBuf.c_str(), mRcvBuf);
            if (rtn == CURLE_OK) {
                break;
            }
        }
    }	

    if(rtn != CURLE_OK){
        AfxWriteDebugLog("SuperVPN run at[CNodeSrvBase::DealActionWithModel] Http Post Err");
        switch (rtn){
            case CURLE_COULDNT_CONNECT:
            case CURLE_OPERATION_TIMEDOUT:
            case CURLE_COULDNT_RESOLVE_HOST:
            case CURLE_SEND_ERROR:
                return ND_ERROR_SEND_FAILED;
            case CURLE_RECV_ERROR:
                return ND_ERROR_RECEIVE_FAILED;
            case CURLE_SSL_CACERT:
            case CURLE_SSL_CACERT_BADFILE:
                return ND_ERROR_LOAD_CA_CERTIFICATE;
            default:
                return ND_ERROR_INVALID_PARAM;
        }
    }

	gITBakServer = gITCurServer;

    //http�������������ж�
    if (GetHttpReturnCode() != 200){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Http Server Response Code=[%d]", GetHttpReturnCode());
        return ND_ERROR_INVALID_RESPONSE;
    }

    //�������ݵķ�������
    AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Begin Analyze Response\n[%s]", mRcvBuf.c_str());
    if(mRcvBuf.empty()){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Server response data is null");
        return ND_ERROR_INVALID_RESPONSE;
    }

	return ND_SUCCESS;
}

/*********************************************************
����˵��������httpͷ����Ϣ
���˵����head-ͷ����Ϣ
����˵������
����ֵ  ����
*********************************************************/
void CHttpGeneral::AnalysisAuthHeader(ndString &head)
{

}

/*********************************************************
����˵��������У��ͷ
���˵������
����˵������
����ֵ  ��У����Ϣ
*********************************************************/
ndString CHttpGeneral::GenerateAuthHeader()
{
    return NULL;
}