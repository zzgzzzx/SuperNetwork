/*********************************************************
ģ����       : strtok()
�ļ���       :BaseApp.cpp
����ļ�     :BaseApp.hpp
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"

list<SServerInfo> gServers;
list<SServerInfo>::iterator gITCurServer,gITBakServer;

/*********************************************************
����˵����������(��������������)
���˵����
����˵����
����ֵ  ��
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
����˵����application��ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSuperVPNApp::InitApplication(int argc,char *argv[])
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ===============");
	ShowVersion();
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ===============");

	//��������
	bool bOnlyCheckUpgrade = false;
	if (!CKArgvAndLock(argc, argv, bOnlyCheckUpgrade))
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] CKArgvAndLock END");
		return false;	
	}

	//ϵͳ���ݳ�ʼ��
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM");
	if (!InitSystem(argv[0], bOnlyCheckUpgrade)) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System END");
		return false;
	}

	//���ݰ�����ѭ������
	TranslatePkt();
	
	return true;
}

/*********************************************************
����˵���������������ƻ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool CSuperVPNApp::GetRunLock()
{
	//����flags:ֻд���ļ���������ô�ʹ������ļ����ȴ�Ϊ0  
	#define FLAGS O_WRONLY | O_CREAT | O_TRUNC  
	//�����ļ���Ȩ�ޣ��û�����д��ִ�С������ִ�С������û�����ִ��  
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
����˵���������������ƻ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool CSuperVPNApp::GetUpgradeLock()
{
	//����flags:ֻд���ļ���������ô�ʹ������ļ����ȴ�Ϊ0  
	#define FLAGS O_WRONLY | O_CREAT | O_TRUNC  
	//�����ļ���Ȩ�ޣ��û�����д��ִ�С������ִ�С������û�����ִ��  
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
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSuperVPNApp::InitSystem(char *appname, bool ifOnlyCheckUpgrade)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Begin ifOnlyCheckUpgrade=[%d]", ifOnlyCheckUpgrade);

	//�������б���
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] ServerListCheck");
	ServerListCheck();
	
	//��������Ƿ��л�ȡ���ұ��ش���
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeInitCheck");
	while(NodeInitCheck() != ND_SUCCESS)
	{
		if(ifOnlyCheckUpgrade) return false;
		sleep(8);
	}

	//ϵͳ���л������(����edge\iptable\node-version)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] RunEnvCheck");
	while(RunEnvCheck(appname, ifOnlyCheckUpgrade) != ND_SUCCESS)
	{
		if(ifOnlyCheckUpgrade) return false;
		sleep(8);
	}
	
	//���ֻ�Ǽ�������ģ�������ֱ���˳�
	if(ifOnlyCheckUpgrade)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] OnlyCheckUpgrade Return");
		return false;
	}

	//�ڵ���������
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeEnvSet...");
	if (mPNode->NodeEnvSet() != ND_SUCCESS)
		return false;

	//����Hello����
	if (mPHelloSrv->Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START WORKING...");	

	return true;
}

/*********************************************************
����˵�����ڵ��ʼ��(����nodeid)
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CSuperVPNApp::NodeInitCheck()
{
	//������ŵ��ļ��Ƿ����
	//������ڣ��������ţ���������ڣ���������
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
����˵�����������б���
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CSuperVPNApp::ServerListCheck()
{
	ndStatus ret;
	gITCurServer = gServers.begin();
	gITBakServer = gITCurServer;
	//������ز����ڷ����б��ļ��������Ĭ�ϴ���
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

	//��ȡ�������б�
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Server ServerList");
	ret = mPNode->GetServerList(gServers);
	if(ret != ND_SUCCESS) return ret;

	//���·������б�
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Update Server ServerList");
	AfxUpdateServerList(gServers);
	gITCurServer = gServers.begin();
	gITBakServer = gITCurServer;
	
	return ND_SUCCESS;
}

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSuperVPNApp::CSuperVPNApp()
{
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSuperVPNApp::~CSuperVPNApp()
{
	delete mPNode;
}

/*********************************************************
����˵������ȡ���ʱ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
int CSuperVPNApp::GetCheckTime()
{
	//���ʱ����ļ������ڣ��������Ĭ��ʱ��д��ʱ���ļ�
	if(AfxFileExist(TASK_CHECK_FILE_NAME))
	{
		AfxWriteTaskTime(TASK_CHECK_TIMER_VALUE);
	}

	mCheckTime = AfxGetTaskTime();
	
	return mCheckTime*60;
}

/*********************************************************
����˵�������ü��ʱ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::SetCheckTime(int time)
{
	AfxWriteTaskTime(TASK_CHECK_TIMER_VALUE);
	mCheckTime = time*60;
}


/*********************************************************
����˵�����ڵ�������ʱ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::NodeRestartFunc(ndULong param)
{
	char cmd[512]={0};
	sprintf(cmd, "killall %s", EDGE_EXE_FILE_NAME);
	AfxExecCmd(cmd);


	sprintf(cmd, "%s", "killall %s", VPN_EXE_FILE_NAME);
	AfxExecCmd(cmd);

	//�����Ķ�����ϵͳ��ʱ���������cront
}

/*********************************************************
����˵������Ϣ���ݰ����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::InsertPktToQueue(CPacket *pkt)
{
	//AfxWriteDebugLog("===>INSERT PKT TO SYSTEM MSG QUEUE...");
	mPktQueue.PutMsg(pkt);
}

/*********************************************************
����˵�������ݰ�����ѭ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::TranslatePkt(void)
{
	CPacket *pkt;

	while(true)
	{
		//���ݰ���ȡ
		pkt = mPktQueue.GetMsg();		
		if (NULL == pkt) continue;
		//���ݰ�����
		AfxWriteDebugLog("===>CSuperVPNApp Recv Pkt Begin Dispose...");
		pkt->DisposePkt();		
		//���ݰ��ͷ�
		delete pkt;
	}
}


