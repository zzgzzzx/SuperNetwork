/*********************************************************
模块名       : 节点http协议
文件名       : HttpUpdateCK.cpp
相关文件     : HttpUpdateCK.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpRunEnvCK.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"
#include "BaseApp.hpp"
#include <strings.h>

extern CBaseApp *gPSuperVPNApp;
/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpRunEvnCK::CHttpRunEvnCK(CNodeBase *node):CHttpGeneral(node)
{
	mRunEnvCK.node.iVerCode = 0;
	mRunEnvCK.node.mDownLodURL.clear();
	mRunEnvCK.deamon.mDownLodURL.clear();
	mRunEnvCK.edge.mDownLodURL.clear();
	mRunEnvCK.iptable.mDownLodURL.clear();
}

/*********************************************************
函数说明：发送请求数据包
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::MakeCheckReq()
{
    char *out, devicetype[128]={0};
    cJSON *root, *fmt, *actions, *arugments;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	mLocalVersion);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::MakeCheckReq] Make check actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_ENV_CHECK);

	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuperVPNApp != NULL) sprintf(devicetype, "devicetype=%s", pSuperVPNApp->GetDeviceType().c_str());
	
	cJSON_AddItemToObject(actions, "devparams", arugments = cJSON_CreateArray());
	cJSON_AddItemToArray(arugments, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "subtype", devicetype);
	cJSON_AddStringToObject(fmt, "other", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：升级检测解包
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::AnalysisCheckRsp()
{
	AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] begin");

    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Error before: [%s]", cJSON_GetErrorPtr());
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
			
			//取node信息
			cJSON *objType = cJSON_GetObjectItem(repliceslist, "node") ;
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

					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Get URL");
					while(url != NULL)
					{
					    if(cJSON_GetObjectItem(url, "url") != NULL &&
					       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
					    {
					        mRunEnvCK.node.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
							AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
					    }
					    url = url->next;
					}	       
			    }		
			}

			//取checknode
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

					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Get URL");
					while(url != NULL)
					{
					    if(cJSON_GetObjectItem(url, "url") != NULL &&
					       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
					    {
					        mRunEnvCK.deamon.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
							AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
					    }
					    url = url->next;
					}	       
			    }		
			}	

			//取edge
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

					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Get URL");
					while(url != NULL)
					{
					    if(cJSON_GetObjectItem(url, "url") != NULL &&
					       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
					    {
					        mRunEnvCK.edge.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
							AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
					    }
					    url = url->next;
					}	       
			    }		
			}	
			//取iptable
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

					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Get URL");
					while(url != NULL)
					{
					    if(cJSON_GetObjectItem(url, "url") != NULL &&
					       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
					    {
					        mRunEnvCK.iptable.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
							AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
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
函数说明：获取文件的md5值
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
#define READ_DATA_SIZE  1024   
ndBool getFileMD5(const ndString& filename, ndString& fileMd5)
{
    int i;  
    int fd;  
    int ret;  
    unsigned char data[READ_DATA_SIZE];  
    unsigned char md5_value[MD5_SIZE];  
    MD5_CTX md5;  
	char md5_str[128]={0};
  
    fd = open(filename.c_str(), O_RDONLY);  
    if (-1 == fd)  
    {  
        AfxWriteDebugLog("SuperVPN run at[getFileMD5] open file error=[%s]", filename.c_str());
        return false;  
    }  
  
    MD5Init(&md5);  
  
    while (1)  
    {  
        ret = read(fd, data, READ_DATA_SIZE);  
        if (-1 == ret)  
        {  
            AfxWriteDebugLog("SuperVPN run at[getFileMD5] read file error=[%s]", filename.c_str());
            return false;  
        }  
  
        MD5Update(&md5, data, ret);  
  
        if (0 == ret || ret < READ_DATA_SIZE)  
        {  
            break;  
        }  
    }  
  
    close(fd);  
  
    MD5Final(md5_value, &md5);  
  
    for(i = 0; i < MD5_SIZE; i++)  
    {  
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);  
    }  
    md5_str[MD5_STR_LEN] = '\0';
	fileMd5 = md5_str;
  
    return true;  
}

/*********************************************************
函数说明：edge检测
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::EdgeCheck()
{
	//判断edge命令是否存在
	//if(AfxCheckCmdExist(EDGE_EXE_FILE_NAME))
	//	return ND_SUCCESS;
	
	//判断文件是否存在
	if(AfxFileExist(EDGE_EXE_PATH_NAME))
	{
		ndString fileMD5;
		if (getFileMD5(EDGE_EXE_PATH_NAME, fileMD5))
		{
			if(strcasecmp(fileMD5.c_str(), mRunEnvCK.edge.sMD5.c_str()))
			{
				AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::EdgeCheck] MD5 value isn't same, must upgrade\n");
			}
			else return ND_SUCCESS;
		}
		else
		{
			AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::EdgeCheck] Get MD5 ERROR\n");
		}
	}

	//增加先清除edge
	AfxCleanAllEdge();

	ndStatus ret = Download(EDGE_EXE_PATH_NAME, mRunEnvCK.edge.mDownLodURL, mRunEnvCK.edge.sMD5);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::EdgeCheck] Download edge command Err ret=[%d]", ret);
        return ret;
    }

	chmod(EDGE_EXE_PATH_NAME, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);

	return ND_SUCCESS;
}

/*********************************************************
函数说明：deamon检测
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/	
ndStatus CHttpRunEvnCK::CheckSuperVPNCheck()
{
	//判断checksupervpn命令是否存在
	if(AfxFileExist(CHECK_VPN_EXE_PATH_NAME))
	{
		//本身这个检测的进程就是CheckSuperVPN启动的
		return ND_SUCCESS;
		/*ndString fileMD5;
		if (getFileMD5(CHECK_VPN_EXE_PATH_NAME, fileMD5))
		{
			if(strcasecmp(fileMD5.c_str(), mRunEnvCK.deamon.sMD5.c_str()))
			{
				AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::CheckSuperVPNCheck] MD5 value isn't same, must upgrade\n");
			}
			else return ND_SUCCESS;
		}
		else
		{
			AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::CheckSuperVPNCheck] Get MD5 ERROR\n");
		}*/
	}
		

	ndStatus ret = Download(CHECK_VPN_EXE_PATH_NAME, mRunEnvCK.deamon.mDownLodURL, mRunEnvCK.deamon.sMD5);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::DeamonCheck] Download CheckSuperVPN command Err ret=[%d]", ret);
        return ret;
    }

	chmod(CHECK_VPN_EXE_PATH_NAME, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);

	return ND_SUCCESS;
}

/*********************************************************
函数说明：iptable检测
入参说明：无
出参说明：无
返回值  ：无
#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001
*********************************************************/
ndStatus CHttpRunEvnCK::IPTableCheck()
{
	//判断iptable的命令是否存在
	if(AfxCheckCmdExist("iptables"))
	{
		return ND_SUCCESS;
	}

	ndStatus ret = Download("/usr/bin/iptables", mRunEnvCK.iptable.mDownLodURL, mRunEnvCK.iptable.sMD5);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::IPTableCheck] Download iptable command Err ret=[%d]", ret);
        return ret;
    }
	
	chmod("/usr/bin/iptables", S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);

	AfxExecCmd("ln -s /usr/bin/iptables /usr/bin/iptables-restore");
	AfxExecCmd("ln -s /usr/bin/iptables /usr/bin/iptables-save");
	AfxExecCmd("ln -s /usr/bin/iptables /usr/bin/iptables-xml");
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：node检测
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::NodeCheck()
{
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::NodeCheck] Local VER=[%d] New VER=[%d]", mLocalVersion, mRunEnvCK.node.iVerCode);
	//判断版本号是否需要升级
	if (mRunEnvCK.node.iVerCode <= mLocalVersion){
		AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::NodeCheck] Node Not Need Upgrade");
		return ND_SUCCESS;	
	}
	
	//下载新版本
    ndStatus ret = Download(VPN_UPGRADE_TEMP_FILE_NAME, mRunEnvCK.node.mDownLodURL, mRunEnvCK.node.sMD5);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::NodeCheck] DownloadNewVersion Err ret=[%d]", ret);
        return ret;
    }

	//通过-c检测是否是真实的supervpn
	char cmd[256]={0};
	ndString result;
	chmod(VPN_UPGRADE_TEMP_FILE_NAME, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	sprintf(cmd, "%s -c", VPN_UPGRADE_TEMP_FILE_NAME);
	if (AfxRunCmdGetResult(cmd, result))
	{
		if (result.find("Hello SuperVPN") == ndString::npos)
		{
			AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::NodeCheck] Upgrade Download Not Real SuperVPN ,^_^WHY^_^");
			remove(VPN_UPGRADE_TEMP_FILE_NAME);
			return ND_SUCCESS;
		}
	}

	//下载完成，替换文件名
	sprintf(cmd, "mv %s %s", VPN_UPGRADE_TEMP_FILE_NAME, VPN_UPGRADE_FILE_NAME);
	AfxExecCmd(cmd);	

	//chmod(VPN_UPGRADE_FILE_NAME, S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
	chmod(VPN_UPGRADE_FILE_NAME, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);	
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：下载文件
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::Download(ndString filename, list<ndString> urls, ndString md5)
{
	ndStatus ret = ND_ERROR_INVALID_PARAM;
    CHttpFileDown FileDown;
    SDownloadFileReqSt downloadFileReqSt;
    downloadFileReqSt.sLocalFile = filename;
    downloadFileReqSt.fFile = NULL;
    downloadFileReqSt.iOffset = 0;
    downloadFileReqSt.iCRC = 0;
    downloadFileReqSt.iSize = 0;

	list<ndString>::iterator iterURL;
    for(iterURL=urls.begin(); iterURL!=urls.end(); iterURL++)
    {
		downloadFileReqSt.sUrl = *iterURL;
	    if (!FileDown.SetDownFileReq(downloadFileReqSt))
	    {
	        return ND_ERROR_INVALID_REQUEST;
	    }
	    ret = FileDown.BeginDownload();
	    if(ret == ND_SUCCESS)
	    {
	        AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::Download] Download File=[%s] Success\n", downloadFileReqSt.sUrl.c_str());

			ndString fileMD5;
			if (!getFileMD5(filename, fileMD5))
			{
				AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::Download] MD5 calcute ERROR\n");
				return ND_ERROR_CRC_CHECK_FAILED;
			}

			if(strcasecmp(fileMD5.c_str(), md5.c_str()))
			{
				AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::Download] MD5 Check ERROR MUST=[%s] REAL=[%s]\n", md5.c_str(), fileMD5.c_str());

				if( remove(filename.c_str()) != 0 )
        			AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::Download] Remove File Err=[%s]\r\n", filename.c_str());
							
				return ND_ERROR_CRC_CHECK_FAILED;	
			}	
		
	        return ND_SUCCESS;
	    }else{
	    	AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::Download] Download File Err=[%s] Code=[%d]\n", downloadFileReqSt.sUrl.c_str(), ret);
	    }
    }

	return ret;
}

/*********************************************************
函数说明：开始升级检测
入参说明：无
出参说明：无
返回值  ：true为需要升级,false无需升级
*********************************************************/
ndStatus CHttpRunEvnCK::BeginCheck(char *appname, bool ifOnlyCheckUpgrade)
{
    //组装数据包
    AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] MakeRequest Pkg");
    ndStatus  ret = MakeCheckReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//发送请求的数据包
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] PkgSendAndRecv");
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//解析返回数据并处理
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] AnalyzeResponsePkg");
    ret = AnalysisCheckRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] AnalyzeResponsePkg ret=[%d]", ret);
        return ret;
    }
	//edge check
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] EdgeCheck");
	ret = EdgeCheck();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] EdgeCheck ret=[%d]", ret);
        return ret;
    }
	
	//iptable check
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] IPTableCheck");
	ret = IPTableCheck();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] IPTableCheck ret=[%d]", ret);
        return ret;
    }

	//CheckSuperVPN check
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] CheckSuperVPN");
	ret = CheckSuperVPNCheck();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] CheckSuperVPN ret=[%d]", ret);
        return ret;
    }

	//检测自己的路径与文件名是否在/usr/bin目录，如果不是，先下载到/usr/bin后就直接退出
	if(AfxFileExist(VPN_EXE_PATH_NAME) == false)
	{
		//下载当前版本，一直需要下载到成功为止
    	ndStatus ret = Download(VPN_EXE_PATH_NAME, mRunEnvCK.node.mDownLodURL, mRunEnvCK.node.sMD5);
    	if(ret != ND_SUCCESS)
		{
       	 	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] Download /usr/bin/SuperVPN Err ret=[%d]", ret);
        	return ret;
    	}

		char cmd[256]={0};
		ndString result;
		chmod(VPN_EXE_PATH_NAME, S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
		sprintf(cmd, "%s -c", VPN_EXE_PATH_NAME);
		if (AfxRunCmdGetResult(cmd, result))
		{
			if (result.find("Hello SuperVPN") == ndString::npos)
			{
				AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] Download Not Real SuperVPN ,^_^WHY^_^");
				remove(VPN_EXE_PATH_NAME);
			}
		}		
		exit(0);
	}
	
	//node check
	if(ifOnlyCheckUpgrade)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] NodeCheck");
		ret = NodeCheck();
	    if(ret != ND_SUCCESS){
	        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] NodeCheck ret=[%d]", ret);
	        return ret;
	    }	
	}
	
	return ND_SUCCESS;
}