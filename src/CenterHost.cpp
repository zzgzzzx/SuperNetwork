/****************** Copyright (c) 2008 **************
ģ����       : 
�ļ���       :BaseInform.cpp
����ļ�     :Inform.hpp
�ļ�ʵ�ֹ��� :������Ϣ��
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "CenterHost.hpp"
#include "NodeBase.hpp"
#include "NDFunc.hpp"
#include "cJSON.h"

#define CENTER_HOST_DEFAULT_001 "http://n001.zrdx.com:5213"
#define CENTER_HOST_DEFAULT_002 "http://n002.zrdx.com:5213"
#define CENTER_HOST_DEFAULT_003 "http://n003.zrdx.com:5213"

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CCenterHost::CCenterHost()
{
	HostInit();	
	GetLocalHosts();

	mITCur = mListHost.begin();
	mITBak = mITCur;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CCenterHost::~CCenterHost()
{

}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CCenterHost::HostInit()
{
	//��ʼ���б���Ϣ
	SServerInfo srv;
	
	mVersion = AfxGetHostVersion();
	mListHost.clear();

	srv.sSrvURL = CENTER_HOST_DEFAULT_001;
	srv.iWeight = 1;
	mListHost.push_back(srv);

	srv.sSrvURL = CENTER_HOST_DEFAULT_002;
	srv.iWeight = 1;
	mListHost.push_back(srv);

	srv.sSrvURL = CENTER_HOST_DEFAULT_003;
	srv.iWeight = 1;
	mListHost.push_back(srv);

}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CCenterHost::CenterHostCheck()
{
	//�ӷ�������ȡ�µ��б�����и��£�����г�ʼ���ݵĸ��´���
	if(GetHostsFromSrv())
	{
		HostInit();	
		GetLocalHosts();
		mITCur = mListHost.begin();
		mITBak = mITCur;
	}

	SServerInfo sInfo;
	list<SServerInfo>::iterator iter;
	for (iter = mListHost.begin(); iter != mListHost.end(); ++iter)
	{
		sInfo = *iter;
    	AfxWriteDebugLog("SuperVPN run at [CCenterHost::CenterHostCheck] HOST=[%s] WEIGHT=[%d]", sInfo.sSrvURL.c_str(), sInfo.iWeight);		
	}

	return ND_SUCCESS;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
SServerInfo CCenterHost::GetCurrentHost()
{
	return *mITCur;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool CCenterHost::GetNextHost(SServerInfo &host)
{
    if(mITCur == mListHost.end() || ++mITCur == mListHost.end())
    {
        mITCur = mListHost.begin();
		AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetNextHost] it set to begin");
    }

    if (mITCur != mListHost.end())
    {
    		SServerInfo curInfo,bakInfo;
			bakInfo = *mITBak;
			curInfo = *mITCur;

			//AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetNextHost] cur host=[%s]", curInfo.sSrvURL.c_str());
			//AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetNextHost] bak host=[%s]", bakInfo.sSrvURL.c_str());

			if(curInfo.sSrvURL != bakInfo.sSrvURL)
			{	
		        host = *mITCur;
				AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetNextHost] Get new host=[%s]", host.sSrvURL.c_str());
		        return ND_TRUE;
			}
    }
	AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetNextHost] Not more new server");

    return ND_FALSE;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CCenterHost::CurrentHostOK()
{
	mITBak = mITCur;
}

/*********************************************************
����˵�������·����б�
���˵����
����˵����
����ֵ	��
*********************************************************/
ndBool CCenterHost::UpdateServerList(list<SServerInfo> &mServers)
{
	FILE *pFile;
	ndString json;
	
	if ((pFile = fopen(SERVER_LIST_FILE_NAME_VER2, "w+")) == NULL)
	{
		AfxWriteDebugLog("SuperVPN run at [CCenterHost::UpdateServerList] Create [%s] File Fail", SERVER_LIST_FILE_NAME_VER2);
		return ND_FALSE;
	}

	json = ServerListToJson(mServers);
	AfxWriteDebugLog("SuperVPN run at [CCenterHost::UpdateServerList] Json=[%s]", json.c_str());
	if (fwrite(json.c_str(), json.length(), 1, pFile) <= 0)
	{
		AfxWriteDebugLog("SuperVPN run at [CCenterHost::UpdateServerList] Write [%s] File Fail", SERVER_LIST_FILE_NAME_VER2);
		fclose(pFile);
		return ND_FALSE;
	}
	
	fclose(pFile);
	return ND_TRUE;
}

/*********************************************************
����˵������ȡ�����б�
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndString CCenterHost::ServerListToJson(list<SServerInfo> &mServers)
{
	ndString result;
    char *out;
    cJSON *root, *fmt, *actions;

	list<SServerInfo>::iterator iter;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "servers", actions = cJSON_CreateArray());

	SServerInfo sInfo;
	for (iter = mServers.begin(); iter != mServers.end(); ++iter)
	{
		sInfo = *iter;
    	cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    	cJSON_AddStringToObject(fmt, "host", sInfo.sSrvURL.c_str());
		cJSON_AddNumberToObject(fmt, "weight", sInfo.iWeight);		
	}
	
    out = cJSON_Print(root);
	result = out;
	
    cJSON_Delete(root);
    free(out);
	
	return result;
}

/*********************************************************
����˵������ȡ���ط������б���Ϣ
���˵����
����˵����
����ֵ  ��

srand((unsigned)time(NULL));
switch(rand()%10){
	case 1: 
		case 3: 
			case 5: 
				case 7: 
					case 9:
	   mServers.push_back(sInfo);
	   break; 
	case 2:
		case 4:
			case 6:
				case 8:
					case 10:
	   mServers.push_front(sInfo);
	   break;
	default:
	   mServers.push_front(sInfo);
}	
*********************************************************/
void CCenterHost::JsonToServerList(ndString json, list<SServerInfo> &mServers)
{
    cJSON *root;
	SServerInfo sInfo;
	int RandomNumber;

	//�����ݴ��з������������б���Ϣ
    root = cJSON_Parse(json.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CCenterHost::JsonToServerList] Error before: [%s]", cJSON_GetErrorPtr());
        return;
    }

    cJSON *serverArray = cJSON_GetObjectItem(root, "servers");
    if(serverArray != NULL)
    {	    
        cJSON *serverlist = serverArray->child;
		while(serverlist != NULL)
		{
		    if(cJSON_GetObjectItem(serverlist, "host") != NULL &&
		       cJSON_GetObjectItem(serverlist, "host")->valuestring != NULL)
		        sInfo.sSrvURL = cJSON_GetObjectItem(serverlist, "host")->valuestring;
			AfxWriteDebugLog("SuperVPN run at [CCenterHost::JsonToServerList] host=[%s]", sInfo.sSrvURL.c_str());

		    if(cJSON_GetObjectItem(serverlist, "weight") != NULL &&
		       cJSON_GetObjectItem(serverlist, "weight")->valueint != NULL)
		        sInfo.iWeight = cJSON_GetObjectItem(serverlist, "weight")->valueint;
			AfxWriteDebugLog("SuperVPN run at [CCenterHost::JsonToServerList] iWeight=[%d]", sInfo.iWeight);

			list<SServerInfo>::iterator iter;
			for (iter = mServers.begin(); iter != mServers.end(); ++iter)
			{
				if((*iter).sSrvURL == sInfo.sSrvURL)
				{
					mServers.erase(iter);
					break;
				}
			}
			mServers.push_front(sInfo);

		    serverlist = serverlist->next;
		}
     }	
}

/*********************************************************
����˵������ȡ���ط������б���Ϣ
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool CCenterHost::GetLocalHosts()
{
	FILE *pFile;
	ndString json;
	list<SServerInfo> mServers;

	//�ļ�������
	AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetLocalHosts] Check %s", SERVER_LIST_FILE_NAME_VER2);
	if (access(SERVER_LIST_FILE_NAME_VER2, NULL) != 0)
	{
		AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetLocalHosts] %s Not Exists", SERVER_LIST_FILE_NAME_VER2);

		char cmd[128]={0};
		sprintf(cmd, "rm %s*", SERVER_LIST_FILE_NAME);
		AfxExecCmd(cmd);	
	
		return true;
	}

	//�ļ�����
	if ((pFile = fopen(SERVER_LIST_FILE_NAME_VER2, "r")) == NULL)
	{
		AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetLocalHosts] read [%s] File Fail", SERVER_LIST_FILE_NAME_VER2);
		return false;
	}	

	AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetLocalHosts] read %s", SERVER_LIST_FILE_NAME_VER2);
	char buff[1024]={0};
	while(fread(buff, sizeof(char), 1024, pFile) >0 )
	{
		json.append(buff);
		memset(buff, 0, sizeof(buff));
	}
	
	AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetLocalHosts] Local Inform=[%s]", json.c_str());
	JsonToServerList(json, mListHost);	
	fclose(pFile);

	return true;
}


/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool CCenterHost::GetHostsFromSrv()
{
	SCenterHost sCenterHost;

	CNodeGeneral *pNode = AfxGetVPNNode();
	//��ȡ�������б�
	AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetHostsFromSrv] Get Server ServerList");
	ndBool ret = pNode->GetServerList(sCenterHost);
	if(ret != ND_TRUE) return ret;

	mVersion = sCenterHost.version;
	//�жϰ汾���Ƿ��и��£�����У�����Ҫ�����滻����
	AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetHostsFromSrv] Begin Check Host Version...");
	if(strcmp(mVersion.c_str(), AfxGetHostVersion()) == 0)
	{
		AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetHostsFromSrv] Hosts Version Not Upgrade");
		return ND_FALSE;
	}

	//���·������б�
	AfxWriteDebugLog("SuperVPN run at [CCenterHost::GetHostsFromSrv] Begin Update Server ServerList");
	ret = UpdateServerList(sCenterHost.liServer);
	if(ret) AfxWriteHostVersion(mVersion.c_str());

	return ret;
}


