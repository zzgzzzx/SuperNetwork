/*********************************************************
模块名       : strtok()
文件名       :BaseApp.cpp
相关文件     :BaseApp.hpp
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"

list<SServerInfo> gServers;
list<SServerInfo>::iterator gITCurServer,gITBakServer;

/*********************************************************
函数说明：锁处理(运行锁与升级锁)
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndBool CSuperVPNApp::CKArgvAndLock(int argc,char *argv[], ndBool &ifOnlyCheckUpgrade)
{
	if((argc == 2))
	{
		if(strcmp(argv[1], "-u") == 0)
		{
			ifOnlyCheckUpgrade = true;
			if (!GetUpgradeLock())
			{
				AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::CKArgvAndLock] Get Upgrade Lock Err, Exit Run");
				return false;
			}	
		}else if(strcmp(argv[1], "-c") == 0)
		{
				AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::CKArgvAndLock] Hello SuperVPN");
				return false;		
		}
	}
	else
	{
		if (!GetRunLock())
		{
			AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Get Run Lock Err, Exit Run");
			return false;
		}	
	}
	return true;
}

/*********************************************************
函数说明：application初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CSuperVPNApp::InitApplication(int argc,char *argv[])
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ===============");
	ShowVersion();
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ===============");

	//锁检测控制
	bool bOnlyCheckUpgrade = false;
	if (!CKArgvAndLock(argc, argv, bOnlyCheckUpgrade))
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] CKArgvAndLock END");
		return false;	
	}

	//系统数据初始化
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM");
	if (!InitSystem(argv[0], bOnlyCheckUpgrade)) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System END");
		return false;
	}

	//数据包队列循环处理
	TranslatePkt();
	
	return true;
}

/*********************************************************
函数说明：运行锁检测控制机制
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndBool CSuperVPNApp::GetRunLock()
{
	//定义flags:只写，文件不存在那么就创建，文件长度戳为0  
	#define FLAGS O_WRONLY | O_CREAT | O_TRUNC  
	//创建文件的权限，用户读、写、执行、组读、执行、其他用户读、执行  
	#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH  

	struct flock lock;
	int fd;
	if((fd = open(RUN_LOCK_FILE_NAME, FLAGS, MODE))<0)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::GetRunLock] Open File Error");
		return false;
	}
	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;	
	
	if(fcntl(fd, F_SETLKW, &lock) < 0)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::GetRunLock] Fcntl Error");
		return false;
	}

	return true;
}

/*********************************************************
函数说明：升级锁检测控制机制
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndBool CSuperVPNApp::GetUpgradeLock()
{
	//定义flags:只写，文件不存在那么就创建，文件长度戳为0  
	#define FLAGS O_WRONLY | O_CREAT | O_TRUNC  
	//创建文件的权限，用户读、写、执行、组读、执行、其他用户读、执行  
	#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH  

	struct flock lock;
	int fd;
	if((fd = open(UPGRADE_LOCK_FILE_NAME, FLAGS, MODE))<0)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::GetUpgradeLock] Open File Error");
		return false;
	}
	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;	
	
	if(fcntl(fd, F_SETLKW, &lock) < 0)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::GetUpgradeLock] Fcntl Error");
		return false;
	}

	return true;
}

/*********************************************************
函数说明：系统数据初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CSuperVPNApp::InitSystem(char *appname, bool ifOnlyCheckUpgrade)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Begin ifOnlyCheckUpgrade=[%d]", ifOnlyCheckUpgrade);

	//服务器列表处理
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] ServerListCheck");
	ServerListCheck();
	
	//检查结点编号是否有获取并且本地存在
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeInitCheck");
	while(NodeInitCheck() != ND_SUCCESS)
	{
		if(ifOnlyCheckUpgrade) return false;
		sleep(8);
	}

	//系统运行环境检测(包括edge\iptable\node-version)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] RunEnvCheck");
	while(RunEnvCheck(appname, ifOnlyCheckUpgrade) != ND_SUCCESS)
	{
		if(ifOnlyCheckUpgrade) return false;
		sleep(8);
	}
	
	//如果只是检测升级的，检测完成直接退出
	if(ifOnlyCheckUpgrade)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] OnlyCheckUpgrade Return");
		return false;
	}

	//节点配置请求
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeEnvSet...");
	if (mPNode->NodeEnvSet() != ND_SUCCESS)
		return false;

	//启用Hello服务
	if (mPHelloSrv->Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START WORKING...");	

	return true;
}

/*********************************************************
函数说明：节点初始化(包括nodeid)
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndStatus CSuperVPNApp::NodeInitCheck()
{
	//检测结点编号的文件是否存在
	//如果存在，则读出编号，如果不存在，进行申请
	char *nodeid = AfxGetNodeID();
	if(nodeid == NULL){
		//ndStatus ret = mPNode->GetIP();
		//if(ret != ND_SUCCESS)
		//{
		//	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::NodeInitCheck] GetIP Err Ret=[%d]", ret);	
		//	return ret;
		//}
		return mPNode->NodeInit();
	}

	mPNode->SetNodeID(nodeid);
	mPNode->SetNodePWD(AfxGetNodePwd());
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：服务器列表检测
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndStatus CSuperVPNApp::ServerListCheck()
{
	ndStatus ret;
	gITCurServer = gServers.begin();
	gITBakServer = gITCurServer;
	//如果本地不存在服务列表文件，则根据默认创建
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Local ServerList");
	if (AfxGetServerList(gServers))
	{
		gITCurServer = gServers.begin();
		gITBakServer = gITCurServer;
	}
	else
 	{
 		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Local ServerList Error");
		gITCurServer = gServers.begin();
		gITBakServer = gITCurServer;		
		return ND_ERROR_INVALID_PARAM;
	}

	//读取服务器列表
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Server ServerList");
	ret = mPNode->GetServerList(gServers);
	if(ret != ND_SUCCESS) return ret;

	//更新服务器列表
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Update Server ServerList");
	AfxUpdateServerList(gServers);
	gITCurServer = gServers.begin();
	gITBakServer = gITCurServer;
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CSuperVPNApp::CSuperVPNApp()
{
}

/*********************************************************
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CSuperVPNApp::~CSuperVPNApp()
{
	delete mPNode;
}

/*********************************************************
函数说明：获取检测时间
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CSuperVPNApp::GetCheckTime()
{
	//如果时间的文件不存在，则进行用默认时间写入时间文件
	if(AfxFileExist(TASK_CHECK_FILE_NAME))
	{
		AfxWriteTaskTime(TASK_CHECK_TIMER_VALUE);
	}

	mCheckTime = AfxGetTaskTime();
	
	return mCheckTime*60;
}

/*********************************************************
函数说明：设置检测时间
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::SetCheckTime(int time)
{
	AfxWriteTaskTime(TASK_CHECK_TIMER_VALUE);
	mCheckTime = time*60;
}


/*********************************************************
函数说明：节点重启定时器函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::NodeRestartFunc(ndULong param)
{
	char cmd[512]={0};
	sprintf(cmd, "killall %s", EDGE_EXE_FILE_NAME);
	AfxExecCmd(cmd);


	sprintf(cmd, "%s", "killall %s", VPN_EXE_FILE_NAME);
	AfxExecCmd(cmd);

	//重启的动作由系统定时任务来完成cront
}

/*********************************************************
函数说明：消息数据包入队
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::InsertPktToQueue(CPacket *pkt)
{
	//AfxWriteDebugLog("===>INSERT PKT TO SYSTEM MSG QUEUE...");
	mPktQueue.PutMsg(pkt);
}

/*********************************************************
函数说明：数据包队列循环处理
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::TranslatePkt(void)
{
	CPacket *pkt;

	while(true)
	{
		//数据包读取
		pkt = mPktQueue.GetMsg();		
		if (NULL == pkt) continue;
		//数据包处理
		AfxWriteDebugLog("===>CSuperVPNApp Recv Pkt Begin Dispose...");
		pkt->DisposePkt();		
		//数据包释放
		delete pkt;
	}
}


