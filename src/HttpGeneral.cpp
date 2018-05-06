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
#include "md5.h"

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
        
        cJSON *replices = cJSON_GetObjectItem(actionslist, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;
			if(cJSON_GetObjectItem(repliceslist, "nodeid") != NULL)
				mPNode->SetNodeID(cJSON_GetObjectItem(repliceslist, "nodeid")->valuestring);
			if(cJSON_GetObjectItem(repliceslist, "pwd") != NULL)
				mPNode->SetNodePWD(cJSON_GetObjectItem(repliceslist, "pwd")->valuestring);			
        }

        cJSON_Delete(root);
    }	
	
	return ND_SUCCESS;	
}

/*********************************************************
����˵������ȡIP
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeNodeGetIPReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

	mSrvURL = URL_NODE_GETIP;
    //��װ��Ϣ��
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeGetIPReq] Make Init actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_GETIP);

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);

    return ND_SUCCESS;
}

/*********************************************************
����˵������ȡIP���󷵻ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeGetIPRsp()
{	
    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeGetIPRsp] Error before: [%s]", cJSON_GetErrorPtr());
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
			mPNode->SetNodeOutIP(cJSON_GetObjectItem(repliceslist, "ip")->valuestring);
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
����˵�����������б�����
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeServerListReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

	mSrvURL = URL_NODE_GET_SERVER_LIST;

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeServerListReq] Make ServerList actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_GET_SERVER_LIST);

    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_GETIP);	

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
����˵�����������б��ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisServerListRsp(SCenterHost &mServers)
{
    cJSON *root;
	char *strAction;
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

		while(actionslist != NULL)
		{
			strAction = cJSON_GetObjectItem(actionslist, "action")->valuestring;
			
			if(strcmp(strAction, SUPER_ACTION_NODE_GET_SERVER_LIST) == 0)
			{
		        iErrCode = cJSON_GetObjectItem(actionslist, "error")->valueint;
		        if(iErrCode != 0)
				{
					cJSON_Delete(root);
					return ND_ERROR_INVALID_RESPONSE;
		        }	
				
            	if(cJSON_GetObjectItem(actionslist, "version") != NULL &&
   						cJSON_GetObjectItem(actionslist, "version")->valuestring != NULL)
  					mServers.version = cJSON_GetObjectItem(actionslist, "version")->valuestring;
				else
					mServers.version = "";
				AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] Get version=[%s]", mServers.version.c_str());

				mServers.liServer.clear();
				
		        cJSON *replices = cJSON_GetObjectItem(actionslist, "replies");
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
		                    	item.iWeight = 10;
								item.sSrvURL = "";
		                        if(cJSON_GetObjectItem(serverslist, "host") != NULL &&
		                           cJSON_GetObjectItem(serverslist, "host")->valuestring != NULL)
		                            item.sSrvURL= cJSON_GetObjectItem(serverslist, "host")->valuestring;

		                        if(cJSON_GetObjectItem(serverslist, "weight") != NULL &&
		                           cJSON_GetObjectItem(serverslist, "weight")->valueint != NULL)
		                            item.iWeight= cJSON_GetObjectItem(serverslist, "weight")->valueint;
								AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] server url=[%s] weight=[%d]", item.sSrvURL.c_str(), item.iWeight);								

		                        mServers.liServer.push_back(item);
		                        serverslist = serverslist->next;
		                    }					
		            }
		        }			
			}

			if(strcmp(strAction, SUPER_ACTION_NODE_GETIP) == 0)
			{
		        iErrCode = cJSON_GetObjectItem(actionslist, "error")->valueint;
		        if(iErrCode != 0)
				{
					cJSON_Delete(root);
					return ND_ERROR_INVALID_RESPONSE;
		        }		

			
		        cJSON *replices = cJSON_GetObjectItem(actionslist, "replies");
		        if(replices != NULL)
		        {
		            cJSON *ipslist = replices->child;

					if(cJSON_GetObjectItem(ipslist , "ip") != NULL &&
		                  cJSON_GetObjectItem(ipslist , "ip")->valuestring != NULL)
					{
						ndString outip = cJSON_GetObjectItem(ipslist , "ip")->valuestring;
						mPNode->SetNodeOutIP(outip);
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] Out ip=[%s]", outip.c_str());
					}
		        }			
			}			
			actionslist = actionslist->next;
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
ndBool CHttpGeneral::GetServerList(SCenterHost &mServers)
{
    //��װ���ݰ�
    ndStatus  ret = MakeServerListReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::GetServerList] MakeRequest Pkg Err ret=[%d]", ret);
        return ND_FALSE;
    }
	//���ݰ�����
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::GetServerList] PkgSendAndRecv Err ret=[%d]", ret);
		return ND_FALSE;
	}
	//���ݰ�����
    ret = AnalysisServerListRsp(mServers);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::GetServerList] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ND_FALSE;
    }

    return ND_TRUE;
}

/*********************************************************
����˵������ȡIP
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::GetIP()
{
    //��װ���ݰ�
    ndStatus  ret = MakeNodeGetIPReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::GetIP] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//���ݰ�����
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::GetIP] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//���ݰ�����
    ret = AnalysisNodeGetIPRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::GetIP] AnalyzeResponsePkg Err ret=[%d]", ret);
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
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::NodeInit] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//���ݰ�����
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::NodeInit] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//���ݰ�����
    ret = AnalysisNodeInitRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::NodeInit] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

	//����nodeid�Ĵ洢����
	AfxWriteNodeID(mPNode->GetNodeInform().sNodeID.c_str());
	AfxWriteNodePwd(mPNode->GetNodeInform().sNodePwd.c_str());

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
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::NodeEnvSet] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }

	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::NodeEnvSet] PkgSendAndRecv Err ret=[%d]", ret);
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
����˵�������ݰ����������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::PkgSendAndRecv(ndString url)
{
	ndString host;
    //�жϷ�������ַ
    SServerInfo serverInfo = AfxGetCenterHost()->GetCurrentHost();
	host = serverInfo.sSrvURL + url;
	
    if(host.empty()){
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::PkgSendAndRecv] Http Server Empty Err");
        return ND_ERROR_INVALID_PARAM;
    }	

    //�жϷ��͵����ݰ�
    if(mSendBuf.empty()){
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::PkgSendAndRecv] SendBuff Is Empty");
        return ND_ERROR_INVALID_PARAM;
    }

    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::PkgSendAndRecv] Begin post data\n ServerURL=[%s]\n [%s]", host.c_str(), mSendBuf.c_str());
rePost:
    //���ͷ���˲����շ���
    CURLcode rtn = Post(host.c_str(), mSendBuf.c_str(), mRcvBuf);
    if(rtn != CURLE_OK){
        while(AfxGetCenterHost()->GetNextHost(serverInfo)){
			host = serverInfo.sSrvURL + url;
            rtn = Post(host.c_str(), mSendBuf.c_str(), mRcvBuf);
            if (rtn == CURLE_OK) {
                break;
            }
        }
    }	

    if(rtn != CURLE_OK){
        AfxWriteDebugLog("SuperVPN run at [CNodeSrvBase::DealActionWithModel] Http Post Err");
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

	AfxGetCenterHost()->CurrentHostOK();

    //http�������������ж�
    if (GetHttpReturnCode() != 200){
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::PkgSendAndRecv] Http Server Response Code=[%d]", GetHttpReturnCode());
        return ND_ERROR_INVALID_RESPONSE;
    }

    //�������ݵķ�������
    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::PkgSendAndRecv] Begin Analyze Response\n[%s]", mRcvBuf.c_str());
    if(mRcvBuf.empty()){
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::PkgSendAndRecv] Server response data is null");
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
	// MD5(post����+��Կ)
	ndString in;
	unsigned char out[MD5_SIZE]; 
	char strMD5[64]={0}, ret[128]={0};
	
	if(strcmp(mSrvURL.c_str(), URL_NODE_GETIP) == 0 ||
		strcmp(mSrvURL.c_str(), URL_SERVER_NODE_INIT) == 0 ||
		strcmp(mSrvURL.c_str(), URL_USER_NODE_INIT) == 0 ||
		strcmp(mSrvURL.c_str(), URL_SERVER_NODE_SET) == 0 ||
		strcmp(mSrvURL.c_str(), URL_USER_NODE_SET) == 0 ||
		strcmp(mSrvURL.c_str(), URL_SERVER_NODE_ENV_CHECK) == 0 ||
		strcmp(mSrvURL.c_str(), URL_USER_NODE_ENV_CHECK) == 0 ||
		strcmp(mSrvURL.c_str(), URL_NODE_GET_SERVER_LIST) == 0 ||
		strcmp(mSrvURL.c_str(), URL_NODE_GET_SERVICES) == 0 ||
		strcmp(mSrvURL.c_str(), URL_NODE_RELEASE_SERVICES) == 0 ||
		strcmp(mSrvURL.c_str(), URL_NODE_SERVICES_ERROR) == 0
		)
	{		
		in = mSendBuf + mPNode->GetNodeInform().sOutIP;		
	}
	else
	{
		in = mSendBuf + mPNode->GetNodeInform().sNodePwd;	
	}

	MD5((char *)in.c_str(),out, in.length());

	for(int i = 0; i <MD5_SIZE; i++)  
    {  
        snprintf(strMD5 + i*2, 2+1, "%02x", out[i]);  
    }  
    strMD5[MD5_STR_LEN] = '\0';

	sprintf(ret, "Authentication=%s", strMD5);
		
	return ret;
}