/*********************************************************
模块名       : 
文件名       :Func.cpp
相关文件     :Func.hpp
文件实现功能 :全局函数实现
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "cJSON.h"
#include "NDFunc.hpp"
#include "NDGlobal.hpp"
#include "IniClass.hpp"

extern CBaseApp *gPSuperVPNApp;
/*********************************************************
函数说明：插入单回定时器
入参说明：
出参说明：
返回值  ：timeout(秒数)
*********************************************************/
bool AfxInsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return gPSuperVPNApp->InsertSingleTimer(callid,timeout,CallBack);
}

/*********************************************************
函数说明：插入循环定时器
入参说明：
出参说明：
返回值  ：timeout(秒数)
*********************************************************/
bool AfxInsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	return gPSuperVPNApp->InsertCircleTimer(callid,timeout,CallBack);
}

/*********************************************************
函数说明：删除循环定时器
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool AfxDeleteCircleTimer(unsigned long callid)
{
	return gPSuperVPNApp->DeleteCircleTimer(callid);
}

/*********************************************************
函数说明：删除单个定时器
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool AfxDeleteSingleTimer(unsigned long callid)
{
	return gPSuperVPNApp->DeleteSingleTimer(callid);
}

/*********************************************************
函数说明：插入数据包至系统队列中
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxInsertPktToSysQue(CPacket *pkt)
{
	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuperVPNApp != NULL) pSuperVPNApp->InsertPktToQueue(pkt);
}

/*********************************************************
函数说明：获取Hello服务
入参说明：
出参说明：
返回值  ：
*********************************************************/
CHelloSrvThread *AfxGetHelloSrv()
{	
	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuperVPNApp == NULL) return NULL;

	return pSuperVPNApp->mPHelloSrv;
}

/*********************************************************
函数说明：取子串
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool AfxGetSubString(string sou, string bFlag, string eFlag, string &out)
{
	int bPos = sou.find(bFlag);
	if (bPos == string::npos) return false;
	
	string tmp = sou.substr(bPos+bFlag.length(), sou.length()-bPos-bFlag.length());
	int ePos = tmp.find(eFlag);
	if (ePos == string::npos) return false;
	
	out = tmp.substr(0, ePos);
	
	return true;
}


/*********************************************************
函数说明：获取网关名称
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndBool AfxGetGatewayName(ndString &host)
{
	FILE *pFile;
	//文件存在
	if ((pFile = fopen(HOST_FILE_NAME, "r")) == NULL)
	{
		AfxWriteDebugLog("SuperVPN run at [AfxGetGatewayName] read [%s] File Fail", HOST_FILE_NAME);
		return false;
	}	
	
	AfxWriteDebugLog("SuperVPN run at [AfxGetGatewayName] read /etc/hosts");
	char buff[128]={0};
	if(!fgets(buff, sizeof buff, pFile))
	{
		AfxWriteDebugLog("SuperVPN run at [AfxGetGatewayName] read [%s] File Fail", HOST_FILE_NAME);
		return false;
	}
	AfxvTrim(buff);
	host = buff;
	
	fclose(pFile);	
	return true;
}

/*********************************************************
函数说明：获取节点
入参说明：
出参说明：
返回值  ：
*********************************************************/
CNodeGeneral *AfxGetVPNNode()
{
	CSuperVPNApp *pSuerVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuerVPNApp == NULL) return NULL;

	return dynamic_cast<CNodeGeneral*> (pSuerVPNApp->mPNode);
}

/*********************************************************
函数说明：获取服务器列表
入参说明：
出参说明：
返回值  ：
*********************************************************/
CCenterHost *AfxGetCenterHost()
{
	CSuperVPNApp *pSuerVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	if(pSuerVPNApp == NULL) return NULL;

	return pSuerVPNApp->mPCenterHost;
}

/*********************************************************
函数说明：写调试日志
入参说明：
出参说明：
返回值  ：
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
函数说明：写调试日志
入参说明：
出参说明：
返回值  ：
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
函数说明：写入节点编号
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxWriteNodeID(const char *nodeid)
{
	CIniDoc iniDOC;
	iniDOC.LoadFromFile(NODEID_INI_FILE_NAME);
	iniDOC.PutItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_NODEID, nodeid);
	iniDOC.SaveToFile(NODEID_INI_FILE_NAME); 
}

/*********************************************************
函数说明：获取节点编号
入参说明：
出参说明：
返回值  ：
*********************************************************/
char *AfxGetNodeID()
{
	CIniDoc iniDOC;
	iniDOC.LoadFromFile(NODEID_INI_FILE_NAME);
	return iniDOC.GetItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_NODEID);
}

/*********************************************************
函数说明：写入节点密码
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxWriteNodePwd(const char *nodepwd)
{
	CIniDoc iniDOC;
	iniDOC.LoadFromFile(NODEID_INI_FILE_NAME);
	iniDOC.PutItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_NODEPWD, nodepwd);
	iniDOC.SaveToFile(NODEID_INI_FILE_NAME); 	
}

/*********************************************************
函数说明：获取节点密码
入参说明：
出参说明：
返回值  ：
*********************************************************/
char *AfxGetNodePwd()
{
	CIniDoc iniDOC;
	iniDOC.LoadFromFile(NODEID_INI_FILE_NAME);
	return iniDOC.GetItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_NODEPWD);

}

/*********************************************************
函数说明：设置定时任务时间
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxWriteTaskTime(int time)
{
	char tasktime[32]={0};
	sprintf(tasktime, "%d", time);

	CIniDoc iniDOC;
	iniDOC.LoadFromFile(NODEID_INI_FILE_NAME);
	iniDOC.PutItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_TASKTIME, tasktime);
	iniDOC.SaveToFile(NODEID_INI_FILE_NAME); 	
}

/*********************************************************
函数说明：获取定时任务时间
入参说明：
出参说明：
返回值  ：
*********************************************************/
int AfxGetTaskTime()
{
	char tasktime[32]={0};

	CIniDoc iniDOC;
	iniDOC.LoadFromFile(NODEID_INI_FILE_NAME);

	return atoi(iniDOC.GetItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_TASKTIME));
}

/*********************************************************
函数说明：服务器列表版本号
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxWriteHostVersion(const char *version)
{
	AfxWriteDebugLog("SuperVPN run at [AfxWriteHostVersion] new version=[%s]", version);

	CIniDoc iniDOC;
	iniDOC.LoadFromFile(NODEID_INI_FILE_NAME);
	iniDOC.PutItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_HOSTVERSION, version);
	iniDOC.SaveToFile(NODEID_INI_FILE_NAME);
}

/*********************************************************
函数说明：获取服务器列表版本号
入参说明：
出参说明：
返回值  ：
*********************************************************/
char *AfxGetHostVersion()
{
	CIniDoc iniDOC;
	iniDOC.LoadFromFile(NODEID_INI_FILE_NAME);
	return iniDOC.GetItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_HOSTVERSION);
}

int GetRandomNumber()
{
	int RandomNumber;
	srand((unsigned)time(NULL));//为rand()函数生成不同的随机种子
	RandomNumber = rand()%10;//生成10以内的随机数

	return RandomNumber;
}

/*********************************************************
函数说明：写调试日志
入参说明：
出参说明：
返回值  ：
*********************************************************/
void AfxWriteDebugLog(char *Format,...)
{
	if (OPEN_DEBUG_FLAG == ND_FALSE) return;

	va_list Args;
	time_t NowTime;
	char StrBuff[8192];

	time(&NowTime);
	strftime(StrBuff, sizeof(StrBuff)-1,"[%b%d %H:%M:%S]", localtime(&NowTime));
	va_start(Args, Format);
	vsnprintf(StrBuff+strlen(StrBuff), sizeof(StrBuff)-strlen(StrBuff)-1, Format, Args);
	va_end(Args);

	//屏幕输出
	if (DEBUG_TYPE & TYPE_DEBUG_EXPORT_SCREEN)
	{
		cout<<StrBuff<<endl;
	}

	//文件输出
	if (DEBUG_TYPE & TYPE_DEBUG_EXPORT_FILE)
	{
		AfxTraceLocalLog(StrBuff);
	}
}

/*********************************************************
函数说明：文件是否存在检测
入参说明：
出参说明：
返回值  ：返回false表示不存在
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
函数说明：获取文件大小检
入参说明：
出参说明：
返回值  ：返回文件的实际大小
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
函数说明：检测命令是否存在
入参说明：
出参说明：
返回值  ：返回true表示存在
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
函数说明：执行命令并取得返回的结果
入参说明：
出参说明：
返回值  ：返回true表示成功
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
函数说明：坑宝写ssh key文件目录
入参说明：
出参说明：
返回值  ：返回false表示失败
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
函数说明：清除所有的EDGE进程
入参说明：
出参说明：
返回值  ：
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
函数说明：执行系统命令
入参说明：
出参说明：
返回值  ：返回false表示失败
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
函数说明：十六进制的串转成整数
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndUInt32 AfxHexStrToInt(const char src[])
{
	unsigned int lDes;

	if ((NULL == src)||(strlen(src)<8)) return 0;

	sscanf(src,"%08X",&lDes);

	return lDes;
}

/*********************************************************
函数说明：转换MAC格式00-11-22-33-44-55转成6字符
入参说明：
出参说明：
返回值  ：
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
函数说明：转换MAC格式00:11:22:33:44:55转成6字符
入参说明：
出参说明：
返回值  ：
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
函数说明：转换MAC6字符格式转成00-11-22-33-44-55
入参说明：
出参说明：
返回值  ：
*********************************************************/
char *AfxMacToStr(const ndUChar src[])
{
	static char sDes[32]={0};

	if (NULL == src) return sDes;
	sprintf(sDes, "%02X:%02X:%02X:%02X:%02X:%02X",src[0],src[1],src[2],src[3],src[4],src[5]);
	
	return sDes;
}

/*********************************************************
函数说明：读取网卡的MAC地址
入参说明：
出参说明：
返回值  ：
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
函数说明：GBK编码转成utf8
入参说明：src:源串,des:返回串,maxlen:返回串最大值
出参说明：
返回值  ：转换后长度
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
函数说明：
入参说明：
出参说明：
返回值  ：
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
函数说明：数组转成16进制字符串
入参说明：src：源数组
		  srclen:数组字符个数
出参说明：
返回值  ：
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
函数说明：字符串理函数
入参说明：src：源字符串
		  tok：分隔符
		  num：至少返回多少个(不足的时候自动补空串)
		  trim：是否过滤空串
		  null_subst：空串替换串
出参说明：
返回值  ：
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

/*********************************************************
函数说明：获取本机所有网卡的名称，包括未启用
入参说明：
出参说明：
返回值  ：返回网卡的数目
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

int AfxGetAllIfName(set<string> &vIFName)
{
	FILE *fh;
	char buf[512];

	if ((fh = fopen(PATH_PROCNET_DEV, "r")) == NULL) return 0;

	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);	/* eat line */

	while (fgets(buf, sizeof buf, fh)) 
	{
		char name[128];
		get_name(name, buf);

		if (strncmp(name, "ian", 3)) continue;
		if (strchr(name, '.') != NULL) continue;

		vIFName.insert(name);
	}

	fclose(fh);

	return vIFName.size();
}

