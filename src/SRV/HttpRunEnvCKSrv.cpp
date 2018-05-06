/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpUpdateCK.cpp
����ļ�     : HttpUpdateCK.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpRunEnvCKSrv.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"
#include "BaseApp.hpp"
#include "NodeSrv.hpp"

extern CBaseApp *gPSuperVPNApp;

/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpRunEvnCKSrv::CHttpRunEvnCKSrv(CNodeBase *node):CHttpRunEvnCK(node)
{
	mSrvURL = URL_SERVER_NODE_ENV_CHECK;
	mLocalVersion = SUPER_VPN_CLIENT_VER_SERVER;
}

/*********************************************************
����˵���������������ݰ�
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpRunEvnCKSrv::MakeCheckReq()
{
    char *out, subtype[64]={0};
    cJSON *root, *fmt, *actions, *arugments;

    //��װ��Ϣ��
	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuperVPNApp != NULL) strcpy(subtype, pSuperVPNApp->GetDeviceType().c_str());
	
    root = cJSON_CreateObject();
    AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::MakeCheckReq] Make check actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVER_NODE_ENV_CHECK);
	
	cJSON_AddItemToObject(fmt, "arguments", arugments = cJSON_CreateObject());
	cJSON_AddStringToObject(arugments, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());
    cJSON_AddStringToObject(arugments, "subtype", subtype);
	cJSON_AddNumberToObject(arugments, "checktime", AfxGetTaskTime());

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
����˵�������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpRunEvnCKSrv::AnalysisCheckRsp()
{
	AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] begin");

    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] Error before: [%s]", cJSON_GetErrorPtr());
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

			//ȡreinit
			cJSON *objType = cJSON_GetObjectItem(repliceslist, "reinit") ;
			AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] begin get reinit");
			if(objType != NULL)
			{
				AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] get init value");
				if(cJSON_GetObjectItem(objType, "init") != NULL &&
						cJSON_GetObjectItem(objType, "init")->valuestring != NULL)
				{
				        ndString init = cJSON_GetObjectItem(objType, "init")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] return init=[%s]===========", init.c_str());
						if (init == "true")
						{
							AfxWriteNodeID("");
							AfxWriteNodePwd("");
							return ND_NEED_RESTART;
						}
				}
			}			

			//ȡ�µ�id
			objType = cJSON_GetObjectItem(repliceslist, "newnodeid") ;
			if(objType != NULL)
			{
				if(cJSON_GetObjectItem(objType, "newid") != NULL &&
						cJSON_GetObjectItem(objType, "newid")->valuestring != NULL)
				{
				        ndString newid= cJSON_GetObjectItem(objType, "newid")->valuestring;
						AfxWriteNodeID(newid.c_str());
						mPNode->SetNodeID(newid);
				}
			}

			//ȡ�µļ��ʱ��
			objType = cJSON_GetObjectItem(repliceslist, "newchecktime") ;
			if(objType != NULL)
			{
				//����Ƿ���ڵ�ӱ��豸�ģ���Ҫ�������ô���
				if(cJSON_GetObjectItem(objType, "checktime") != NULL &&
						cJSON_GetObjectItem(objType, "checktime")->valueint!= NULL)
				{
				        int checktime= cJSON_GetObjectItem(objType, "checktime")->valueint;
						AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] get newchecktime=[%d]", checktime);
						AfxWriteTaskTime(checktime);
						CNodeSrv *pNodeSrv = dynamic_cast<CNodeSrv*> (mPNode);
						pNodeSrv->KBResetTimer();
				}				
			}
			
			//ȡnode��Ϣ
			objType = cJSON_GetObjectItem(repliceslist, "node") ;
			if(objType != NULL)
			{
			    if(cJSON_GetObjectItem(objType, "version") != NULL &&
					cJSON_GetObjectItem(objType, "version")->valueuint != NULL)
			        mRunEnvCK.node.iVerCode = cJSON_GetObjectItem(objType, "version")->valueuint;

			    if(cJSON_GetObjectItem(objType, "md5") != NULL &&
					cJSON_GetObjectItem(objType, "md5")->valuestring != NULL)
			        mRunEnvCK.node.sMD5 = cJSON_GetObjectItem(objType, "md5")->valuestring;	

			       
			    cJSON *URLS = cJSON_GetObjectItem(objType, "download");
			    if(URLS != NULL)
			    {
					cJSON *url = URLS->child;

					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] Get URL");
					while(url != NULL)
					{
					    if(cJSON_GetObjectItem(url, "url") != NULL &&
					       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
					    {
					        mRunEnvCK.node.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
							AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
					    }
					    url = url->next;
					}	       
			    }		
			}

			//ȡchecknode
			objType = cJSON_GetObjectItem(repliceslist, "checknode") ;
			if(objType != NULL)
			{
			    if(cJSON_GetObjectItem(objType, "md5") != NULL &&
					cJSON_GetObjectItem(objType, "md5")->valuestring != NULL)
			        mRunEnvCK.deamon.sMD5 = cJSON_GetObjectItem(objType, "md5")->valuestring;	

			       
			    cJSON *URLS = cJSON_GetObjectItem(objType, "download");
			    if(URLS != NULL)
			    {
					cJSON *url = URLS->child;

					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] Get URL");
					while(url != NULL)
					{
					    if(cJSON_GetObjectItem(url, "url") != NULL &&
					       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
					    {
					        mRunEnvCK.deamon.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
							AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
					    }
					    url = url->next;
					}	       
			    }		
			}	

			//ȡedge
			objType = cJSON_GetObjectItem(repliceslist, "edge") ;
			if(objType != NULL)
			{
			    if(cJSON_GetObjectItem(objType, "md5") != NULL &&
					cJSON_GetObjectItem(objType, "md5")->valuestring != NULL)
			        mRunEnvCK.edge.sMD5 = cJSON_GetObjectItem(objType, "md5")->valuestring;	

			       
			    cJSON *URLS = cJSON_GetObjectItem(objType, "download");
			    if(URLS != NULL)
			    {
					cJSON *url = URLS->child;

					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] Get URL");
					while(url != NULL)
					{
					    if(cJSON_GetObjectItem(url, "url") != NULL &&
					       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
					    {
					        mRunEnvCK.edge.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
							AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
					    }
					    url = url->next;
					}	       
			    }		
			}	
			//ȡiptable
			objType = cJSON_GetObjectItem(repliceslist, "iptable") ;
			if(objType != NULL)
			{
			    if(cJSON_GetObjectItem(objType, "md5") != NULL &&
					cJSON_GetObjectItem(objType, "md5")->valuestring != NULL)
			        mRunEnvCK.iptable.sMD5 = cJSON_GetObjectItem(objType, "md5")->valuestring;	

			       
			    cJSON *URLS = cJSON_GetObjectItem(objType, "download");
			    if(URLS != NULL)
			    {
					cJSON *url = URLS->child;

					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] Get URL");
					while(url != NULL)
					{
					    if(cJSON_GetObjectItem(url, "url") != NULL &&
					       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
					    {
					        mRunEnvCK.iptable.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
							AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCKSrv::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
					    }
					    url = url->next;
					}	       
			    }		
			}		
        }
    }
 	cJSON_Delete(root);

	return ND_SUCCESS;
}


/*********************************************************
����˵����iptable���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpRunEvnCKSrv::IPTableCheck()
{
	ndStatus ret = CHttpRunEvnCK::IPTableCheck();
	if (ret != ND_SUCCESS) return ret;

	AfxExecCmd("iptables -F");
	AfxExecCmd("iptables -F -t nat");
	AfxExecCmd("iptables -X");
	AfxExecCmd("iptables -X -t nat");
	AfxExecCmd("iptables -A FORWARD -j ACCEPT");
	AfxExecCmd("iptables -t nat -I POSTROUTING -o eth0 -j MASQUERADE");

	AfxExecCmd("echo \"1\"> /proc/sys/net/ipv4/ip_forward");
	
	return ND_SUCCESS;
}

