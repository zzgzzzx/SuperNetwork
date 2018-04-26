/*********************************************************
ģ����       : 
�ļ���       :Func.cpp
����ļ�     :Func.hpp
�ļ�ʵ�ֹ��� :ȫ�ֺ���ʵ��
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "cJSON.h"
#include "NDFunc.hpp"
#include "NDGlobal.hpp"

extern CBaseApp *gPSuperVPNApp;
extern list<SServerInfo> gServers;
extern list<SServerInfo>::iterator gITCurServer, gITBakServer;

/*********************************************************
����˵�������뵥�ض�ʱ��
���˵����
����˵����
����ֵ  ��timeout(����)
*********************************************************/
bool AfxInsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return gPSuperVPNApp->InsertSingleTimer(callid,timeout,CallBack);
}

/*********************************************************
����˵��������ѭ����ʱ��
���˵����
����˵����
����ֵ  ��timeout(����)
*********************************************************/
bool AfxInsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return gPSuperVPNApp->InsertCircleTimer(callid,timeout,CallBack);
}

/*********************************************************
����˵����ɾ��ѭ����ʱ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool AfxDeleteCircleTimer(unsigned long callid)
{
	return gPSuperVPNApp->DeleteCircleTimer(callid);
}

/*********************************************************
����˵����ɾ��������ʱ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool AfxDeleteSingleTimer(unsigned long callid)
{
	return gPSuperVPNApp->DeleteSingleTimer(callid);
}

/*********************************************************
����˵�����������ݰ���ϵͳ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxInsertPktToSysQue(CPacket *pkt)
{
	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuperVPNApp != NULL) pSuperVPNApp->InsertPktToQueue(pkt);
}

/*********************************************************
����˵������ȡHello����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHelloSrvThread *AfxGetHelloSrv()
{	
	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuperVPNApp == NULL) return NULL;

	return &(pSuperVPNApp->mHelloSrv);
}

/*********************************************************
����˵������ȡ��������
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool AfxGetGatewayName(ndString &host)
{
	FILE *pFile;
	//�ļ�����
	if ((pFile = fopen(HOST_FILE_NAME, "r")) == NULL)
	{
		AfxWriteDebugLog("SuperVPN run at [AfxGetGatewayName] read [%s] File Fail", HOST_FILE_NAME);
		return false;
	}	
	
	AfxWriteDebugLog("SuperVPN run at [AfxGetGatewayName] read /etc/hosts");
	char buff[128]={0};
	if (fread(buff, sizeof(char), 128, pFile) < 0)
	{
		AfxWriteDebugLog("SuperVPN run at [AfxGetGatewayName] read [%s] File Fail", HOST_FILE_NAME);
		return false;

	}
	host = buff;
	
	fclose(pFile);	
	return true;
}

/*********************************************************
����˵������ȡ�ڵ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
CNodeBase *AfxGetVPNNode()
{
	CSuperVPNApp *pSuerVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuerVPNApp == NULL) return NULL;

	return pSuerVPNApp->mPNode;
}

/*********************************************************
����˵����д������־
���˵����
����˵����
����ֵ  ��
*********************************************************/
#define LOG_MIN_SIZE (500*1024)
#define LOG_MAX_SIZE (1*1024*1024)
	
int Shringk(char *pFileName)
{
	long Count;
	char *pBuff;
	FILE *pFile;
	struct stat StatBuff;

	if (stat(pFileName, &StatBuff) == -1) return -1;

	if (StatBuff.st_size > LOG_MAX_SIZE) 
	{
		if ((pBuff = (char *)malloc(LOG_MIN_SIZE)) == NULL)
			return -1;

		if ((pFile = fopen(pFileName,"r")) == NULL)
		{free(pBuff); return -1;}

		if (fseek(pFile, -LOG_MIN_SIZE, SEEK_END) != 0 || fgets(pBuff, LOG_MIN_SIZE, pFile) == NULL)
		{fclose(pFile);free(pBuff);return -1;}

		Count = fread(pBuff, sizeof(char), LOG_MIN_SIZE, pFile);
		if (fclose(pFile) != 0 || (pFile = fopen(pFileName,"w")) == NULL)
		{free(pBuff); return -1;}

		fwrite(pBuff, sizeof(char), Count, pFile);
		fclose(pFile);
		free(pBuff);
	}
	return 0;
}

/*********************************************************
����˵����д������־
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxTraceLocalLog(char *sLogContext)
{
	FILE *pFile;

	if ((pFile = fopen(LOG_FILE_NAME, "a+")) == NULL) return;

	fputs(sLogContext, pFile);
	fputs("\n", pFile);
	fclose(pFile);
	Shringk(LOG_FILE_NAME);
}

/*********************************************************
����˵������ȡ��һ�����õķ�������ַ
���˵������
����˵������
����ֵ  ��true-�ɹ������һ����ַ
          false-�޿��õ�ַ
          list<SServerInfo> CSuperVPNApp::gServers;
		 list<SServerInfo>::iterator gITServer;
*********************************************************/
bool AfxGetNextSrvUrl(SServerInfo &serverInfo)
{
    if(gITCurServer == gServers.end() || ++gITCurServer == gServers.end())
    {
        gITCurServer = gServers.begin();
		AfxWriteDebugLog("SuperVPN run at [AfxGetNextSrvUrl] it set to begin");
    }

    if (gITCurServer != gServers.end())
    {
    		SServerInfo curInfo,bakInfo;
			bakInfo = *gITBakServer;
			curInfo = *gITCurServer;

			AfxWriteDebugLog("SuperVPN run at [AfxGetNextSrvUrl] cur IP=[%s]", curInfo.sServerIP.c_str());
			AfxWriteDebugLog("SuperVPN run at [AfxGetNextSrvUrl] bak IP=[%s]", bakInfo.sServerIP.c_str());

			if(curInfo.sServerIP != bakInfo.sServerIP)
			{	
		        serverInfo = *gITCurServer;
				AfxWriteDebugLog("SuperVPN run at [AfxGetNextSrvUrl] Get new server ip=[%s]", serverInfo.sServerIP.c_str());
		        return true;
			}
    }
	AfxWriteDebugLog("SuperVPN run at [AfxGetNextSrvUrl] Not more new server");

    return false;
}


/*********************************************************
����˵����д��ڵ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxWriteNodeID(const char *nodeid)
{
	FILE *pFile;

	if ((pFile = fopen(NODEID_FILE_NAME, "w+")) == NULL) return;

	fputs(nodeid, pFile);
	fclose(pFile);
}

/*********************************************************
����˵������ȡ�ڵ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
char *AfxGetNodeID()
{
	FILE *pFile;
	static char nodeid[128]={0};

	if ((pFile = fopen(NODEID_FILE_NAME, "r")) == NULL) return nodeid;

	fgets(nodeid, sizeof(nodeid), pFile);
	fclose(pFile);

	return nodeid;
}

/*********************************************************
����˵����д��ڵ�����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxWriteNodePwd(const char *nodepwd)
{
	FILE *pFile;

	if ((pFile = fopen(NODEPWD_FILE_NAME, "w+")) == NULL) return;

	fputs(nodepwd, pFile);
	fclose(pFile);
}

/*********************************************************
����˵������ȡ�ڵ�����
���˵����
����˵����
����ֵ  ��
*********************************************************/
char *AfxGetNodePwd()
{
	FILE *pFile;
	static char nodepwd[128]={0};

	if ((pFile = fopen(NODEPWD_FILE_NAME, "r")) == NULL) return nodepwd;

	fgets(nodepwd, sizeof(nodepwd), pFile);
	fclose(pFile);

	return nodepwd;
}


/*********************************************************
����˵������ȡ�����б�
���˵����
����˵����
����ֵ  ��
 {
	"servers":[
	         {
	             "ip":"192.168.0.1"
	             "port":"6666"
	         }
	         {
	             "ip":"192.168.0.2"
	             "port":"7777"
	         }
	]
}
*********************************************************/
ndString ServerListToString(list<SServerInfo> &mServers)
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
    	cJSON_AddStringToObject(fmt, "ip", sInfo.sServerIP.c_str());
		cJSON_AddStringToObject(fmt, "port", sInfo.sServerPort.c_str());		
	}
	
    out = cJSON_Print(root);
	result = out;
	
    cJSON_Delete(root);
    free(out);
	
	return result;
}

int GetRandomNumber()
{
	int RandomNumber;
	srand((unsigned)time(NULL));//Ϊrand()�������ɲ�ͬ���������
	RandomNumber = rand()%10;//����10���ڵ������

	return RandomNumber;
}

void StringToServerList(ndString json, list<SServerInfo> &mServers)
{
    cJSON *root;
	SServerInfo sInfo;
	int RandomNumber;

	//�����ݴ��з������������б���Ϣ
    root = cJSON_Parse(json.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [StringToServerList] Error before: [%s]", cJSON_GetErrorPtr());
        return;
    }

    cJSON *serverArray = cJSON_GetObjectItem(root, "servers");
    if(serverArray != NULL)
    {
	    srand((unsigned)time(NULL));
        cJSON *serverlist = serverArray->child;
		while(serverlist != NULL)
		{
		    if(cJSON_GetObjectItem(serverlist, "ip") != NULL &&
		       cJSON_GetObjectItem(serverlist, "ip")->valuestring != NULL)
		        sInfo.sServerIP = cJSON_GetObjectItem(serverlist, "ip")->valuestring;
			AfxWriteDebugLog("SuperVPN run at [StringToServerList] ip=[%s]", sInfo.sServerIP.c_str());

		    if(cJSON_GetObjectItem(serverlist, "port") != NULL &&
		       cJSON_GetObjectItem(serverlist, "port")->valuestring != NULL)
		        sInfo.sServerPort = cJSON_GetObjectItem(serverlist, "port")->valuestring;
			AfxWriteDebugLog("SuperVPN run at [StringToServerList] port=[%s]", sInfo.sServerPort.c_str());

			mServers.push_back(sInfo);

			/*
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
			*/
		    serverlist = serverlist->next;
		}
     }	
}

ndBool AfxGetServerList(list<SServerInfo> &mServers)
{
	FILE *pFile;
	ndString json;

	//��ʼ���б���Ϣ
	SServerInfo server;
	
	server.sServerIP = "n003.zrdx.com";
	server.sServerPort = "5211";
	mServers.push_back(server);

	server.sServerIP = "n001.zrdx.com";
	server.sServerPort = "5211";
	mServers.push_back(server);

	server.sServerIP = "n002.zrdx.com";
	server.sServerPort = "5211";
	mServers.push_back(server);	
	
	//���/etc/networkĿ¼�Ƿ����
	AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] Check /etc/ian Dir");
	if(access(VPN_DIR_PATH_NAME, NULL) != 0)  
	{  
		if(mkdir(VPN_DIR_PATH_NAME, 0755) == -1)  
		{   
			AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] create [%s] File Fail", SERVER_LIST_FILE_NAME);
			return false; 
		}  
	}  

	//�ļ�������
	AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] Check /etc/network/server.list");
	if (access(SERVER_LIST_FILE_NAME, NULL) != 0)
	{
		if ((pFile = fopen(SERVER_LIST_FILE_NAME, "w+")) == NULL)
		{
			AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] Create [%s] File Fail", SERVER_LIST_FILE_NAME);
			return false;
		}

		json = ServerListToString(mServers);
		if (fwrite(json.c_str(), json.length(), 1, pFile) <= 0)
		{
			AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] Write [%s] File Fail", SERVER_LIST_FILE_NAME);
			fclose(pFile);
			return false;
		}
		
		fclose(pFile);
		return true;
	}

	//�ļ�����
	if ((pFile = fopen(SERVER_LIST_FILE_NAME, "r")) == NULL)
	{
		AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] read [%s] File Fail", SERVER_LIST_FILE_NAME);
		return false;
	}	

	mServers.clear();
	
	AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] read /etc/network/server.list");
	char buff[1025]={0};
	while(fread(buff, sizeof(char), 1024, pFile) >0 )
	{
		json.append(buff);
		memset(buff, 0, sizeof(buff));
	}
	AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] Local Inform=[%s]", json.c_str());
	StringToServerList(json, mServers);
	
	fclose(pFile);

	return true;
}

/*********************************************************
����˵�������·����б�
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool AfxUpdateServerList(list<SServerInfo> &mServers)
{
	FILE *pFile;
	ndString json;
	
	if ((pFile = fopen(SERVER_LIST_FILE_NAME, "w+")) == NULL)
	{
		AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] Create [%s] File Fail", SERVER_LIST_FILE_NAME);
		return false;
	}

	json = ServerListToString(mServers);
	if (fwrite(json.c_str(), json.length(), 1, pFile) <= 0)
	{
		AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] Write [%s] File Fail", SERVER_LIST_FILE_NAME);
		fclose(pFile);
		return false;
	}
	
	fclose(pFile);
	return true;
}

/*********************************************************
����˵����д������־
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxWriteDebugLog(char *Format,...)
{
	if (OPEN_DEBUG_FLAG == ND_FALSE) return;

	va_list Args;
	time_t NowTime;
	char StrBuff[1024];

	time(&NowTime);
	strftime(StrBuff, sizeof(StrBuff)-1,"[%b%d %H:%M:%S]", localtime(&NowTime));
	va_start(Args, Format);
	vsnprintf(StrBuff+strlen(StrBuff), sizeof(StrBuff)-strlen(StrBuff)-1, Format, Args);
	va_end(Args);

	//��Ļ���
	if (DEBUG_TYPE & TYPE_DEBUG_EXPORT_SCREEN)
	{
		cout<<StrBuff<<endl;
	}

	//�ļ����
	if (DEBUG_TYPE & TYPE_DEBUG_EXPORT_FILE)
	{
		AfxTraceLocalLog(StrBuff);
	}
}

/*********************************************************
����˵������ȡ�����������������ƣ�����δ����
���˵����
����˵����
����ֵ  ��������������Ŀ
*********************************************************/
#define isspace(c) ((((c) == ' ') || (((unsigned int)((c) - 9)) <= (13 - 9))))

static char *get_name(char *name, char *p)
{
	while (isspace(*p))
		p++;
	while (*p) {
		if (isspace(*p))
			break;
		if (*p == ':') {	/* could be an alias */
			char *dot = p, *dotname = name;

			*name++ = *p++;
			while (isdigit(*p))
				*name++ = *p++;
			if (*p != ':') {	/* it wasn't, backup */
				p = dot;
				name = dotname;
			}
			if (*p == '\0')
				return NULL;
			p++;
			break;
		}
		*name++ = *p++;
	}
	*name++ = '\0';

	return p;
}

/*********************************************************
����˵�����ļ��Ƿ���ڼ��
���˵����
����˵����
����ֵ  ������false��ʾ������
*********************************************************/
bool AfxFileExist(const string filepath)
{
	if(filepath.length() <= 0)
		return false;
	if (access(filepath.c_str(), F_OK) == 0)
		return true;
	return false;
}

/*********************************************************
����˵������ȡ�ļ���С��
���˵����
����˵����
����ֵ  �������ļ���ʵ�ʴ�С
*********************************************************/
ndDouble AfxFileSize(const string filepath)
{
    FILE *fp;
    ndDouble size = -1;

    if(!AfxFileExist(filepath)) return -1;

    if ((fp = fopen(filepath.c_str(), "r")) == NULL) {
        printf("SuperVPN run at [%s] Read File size open Error\n", __FUNCTION__);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fclose(fp);

    return size;
}

/*********************************************************
����˵������������Ƿ����
���˵����
����˵����
����ֵ  ������true��ʾ����
*********************************************************/
bool AfxCheckCmdExist(const string cmd)
{
	FILE   *stream;
	char buf[2048]={0}, strCMD[128]={0};

	sprintf(strCMD, "which %s", cmd.c_str());	
	stream = popen( strCMD, "r" );	
	fread( buf, sizeof(char), sizeof(buf), stream);
	pclose( stream );

	if(!strcmp(buf, ""))
		return false;

	return true;
}

/*********************************************************
����˵����ִ�����ȡ�÷��صĽ��
���˵����
����˵����
����ֵ  ������true��ʾ�ɹ�
*********************************************************/
bool AfxRunCmdGetResult(const ndString cmd, ndString &result)
{
    FILE *fstream=NULL; 
	
    char buff[1024]={0};    

  	AfxWriteDebugLog("SuperVPN run at [AfxRunCmdGetResult] execute command [%s]", cmd.c_str());
    if(NULL == (fstream = popen(cmd.c_str(), "r")))      
    {     
		AfxWriteDebugLog("SuperVPN run at [AfxRunCmdGetResult] execute command failed [%s]", strerror(errno));
        return false;      
    }   
	
    while(NULL != fgets(buff, sizeof(buff), fstream)) {  
            result += buff;    
    }  
    pclose(fstream); 


	return true;
}

/*********************************************************
����˵�����ӱ�дssh key�ļ�Ŀ¼
���˵����
����˵����
����ֵ  ������false��ʾʧ��
*********************************************************/
bool AfxKBWriteSSHKey(const char *filename)
{
	FILE *pFile;
	char content[]="ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCkBVQdayA2UP49BroYdhh9I2gbr0T6lS893bv2BnyMKfpLCrQsh4QBLVHDMna/wzWdfCo7/PF0OIqnNVPUHDaZKIIOexgWLruSZ64iIY1BhirGQoSrhMy1FnWzCAlTJWXgMoMstAdRUPoW8p4RsvjYObQpXSgfyNzA012O7RWcUmYY6urj5LqKX3EZ+WtqpD0quz+FduksOzHCU4afwEDkn1zY1bXoMFIFRonTwmdxG7w7vX++JQ6KmWpminUjwQuNzRAjjHlXbJvX7/Q0KSjCsZRzf5qzfuq28/XEzwr1qzV24gXZBUl9FywDzS+0enKNMrm7cKGaEwLTsnB/IApJ rsa.openssh";
	
	if ((pFile = fopen(filename, "w+")) == NULL) return false;

	fputs(content, pFile);
	fclose(pFile);
}

/*********************************************************
����˵����������е�EDGE����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxCleanAllEdge()
{
	char cmd[512]={0};
	sprintf(cmd, "killall %s", EDGE_EXE_FILE_NAME);
	AfxExecCmd(cmd);

	strcpy(cmd, "killall edge");
	AfxExecCmd(cmd);	
}


/*********************************************************
����˵����ִ��ϵͳ����
���˵����
����˵����
����ֵ  ������false��ʾʧ��
*********************************************************/
bool AfxExecCmd(const char *cmd)
{
	AfxWriteDebugLog("SuperVPN run at [AfxExecCmd] Exec Cmd=[%s]",cmd);
	int status = system(cmd);
	if (status == -1)
	{
		return false;
	}

	return true;

	/*if (WIFEXITED(status))
	{
		if (!WEXITSTATUS(status))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}*/ 
}

/*********************************************************
����˵����ʮ�����ƵĴ�ת������
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndUInt32 AfxHexStrToInt(const char src[])
{
	unsigned int lDes;

	if ((NULL == src)||(strlen(src)<8)) return 0;

	sscanf(src,"%08X",&lDes);

	return lDes;
}

/*********************************************************
����˵����ת��MAC��ʽ00-11-22-33-44-55ת��6�ַ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxFormatMac(const char src[],ndUChar des[])
{
	int lMac[6];

	if ((NULL == src)||(strlen(src)<17)) return;

	sscanf(src,"%02X:%02X:%02X:%02X:%02X:%02X",\
					&lMac[0],&lMac[1],&lMac[2],\
					&lMac[3],&lMac[4],&lMac[5]);

	for(int m=0; m<6; m++) des[m] = lMac[m];
}

/*********************************************************
����˵����ת��MAC��ʽ00:11:22:33:44:55ת��6�ַ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxFormatMacT(const char src[],ndUChar des[])
{
	int lMac[6];

	if ((NULL == src)||(strlen(src)<17)) return;

	sscanf(src,"%02X:%02X:%02X:%02X:%02X:%02X",\
		&lMac[0],&lMac[1],&lMac[2],\
		&lMac[3],&lMac[4],&lMac[5]);

	for(int m=0; m<6; m++) des[m] = lMac[m];
}

/*********************************************************
����˵����ת��MAC6�ַ���ʽת��00-11-22-33-44-55
���˵����
����˵����
����ֵ  ��
*********************************************************/
char *AfxMacToStr(const ndUChar src[])
{
	static char sDes[32]={0};

	if (NULL == src) return sDes;
	sprintf(sDes, "%02X:%02X:%02X:%02X:%02X:%02X",src[0],src[1],src[2],src[3],src[4],src[5]);
	
	return sDes;
}

/*********************************************************
����˵������ȡ������MAC��ַ
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxGetEthMac(const char *ethname, ndString &mac)
{
	ndUChar sMac[6];
	char sTemp[64]={0};
	int sd = 0;
	struct  ifreq  if_data;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy( if_data.ifr_name, ethname );
	ioctl( sd, SIOCGIFHWADDR, &if_data);
	memcpy(sMac, &if_data.ifr_hwaddr.sa_data, 6);
	sprintf(sTemp,"%02X:%02X:%02X:%02X:%02X:%02X",sMac[0],sMac[1],sMac[2],sMac[3],sMac[4],sMac[5]);
	mac = sTemp;
	
	close( sd );
}

/*********************************************************
����˵����GBK����ת��utf8
���˵����src:Դ��,des:���ش�,maxlen:���ش����ֵ
����˵����
����ֵ  ��ת���󳤶�
*********************************************************/
int AfxGBK2UTF8(const char *src, char *des, int maxlen)
{
	iconv_t cd = iconv_open("UTF-8", "GBK");
	if ((iconv_t) -1 == cd)	return 0;

	size_t ilen = strlen(src);
	size_t olen = maxlen;
	char *in = (char *)src;
	char *out = des;
	int iLen = maxlen;

	if (iconv(cd, &in, &ilen, &out, &olen) == -1) return 0;

	iLen -= olen;

	des[iLen] = 0;

	iconv_close(cd);

	return iLen;
} 

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void AfxChangeToUTF8(string &context)
{
	if (context.size() <= 0) return;

	char *pos = (char *)malloc(context.size()*2);

	int iLen = AfxGBK2UTF8(context.c_str(), pos, context.size()*2-1);

	if (iLen <= 0)
	{
		free(pos);
		return;
	}

	context.clear();
	context.append(pos, iLen);

	free(pos);
}

/*********************************************************
����˵��������ת��16�����ַ���
���˵����src��Դ����
		  srclen:�����ַ�����
����˵����
����ֵ  ��
*********************************************************/
string AfxByteArrayToHexStr(const ndByte src[],const int &srclen)
{
	string sDes;
	char sTemp[8];

	for (int i=0; i<srclen; i++)
	{
		sprintf(sTemp,"%02X",src[i]);
		sDes.append(sTemp);
	}
	return sDes;
}

/*********************************************************
����˵�����ַ�������
���˵����src��Դ�ַ���
		  tok���ָ���
		  num�����ٷ��ض��ٸ�(�����ʱ���Զ����մ�)
		  trim���Ƿ���˿մ�
		  null_subst���մ��滻��
����˵����
����ֵ  ��
*********************************************************/
StrVector AfxStrTokenize(const string& src,const string& tok,int num,bool trim, string null_subst)
{
	StrVector v;
	v.clear();
	if( src.empty() || tok.empty() ) return v;

	unsigned int pre_index = 0, index = 0, len = 0;   
	while( (index = src.find_first_of(tok, pre_index)) != string::npos )   
	{   
		if( (len = index-pre_index) != 0 )   
			v.push_back(src.substr(pre_index, len));   
		else if(trim == false)   
			v.push_back(null_subst);   
		pre_index = index+1;   
	}   
	string endstr = src.substr(pre_index);   
	if( trim == false ) 
		v.push_back( endstr.empty()?null_subst:endstr );   
	else 
		if( !endstr.empty() ) v.push_back(endstr);   

	for (int m=v.size(); m<num; m++) v.push_back(null_subst);

	return v; 
}
