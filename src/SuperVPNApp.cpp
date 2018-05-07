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
#include "IniClass.hpp"

ndBool CSuperVPNApp::mBNeedRestart = ND_FALSE;

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

RESTART:
	//ϵͳ���ݳ�ʼ��
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM");
	if (!InitSystem(bOnlyCheckUpgrade)) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System END");
		return false;
	}

	//���ݰ�����ѭ������
	TranslatePkt();

	goto RESTART;
	
	return true;
}

/*********************************************************
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSuperVPNApp::InitSystem(bool ifOnlyCheckUpgrade)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Begin ifOnlyCheckUpgrade=[%d]", ifOnlyCheckUpgrade);

REINIT:
	mBNeedRestart = ND_FALSE;
	
	//���/etc/ian/Ŀ¼�Ƿ����
	AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] Check %s Dir", VPN_DIR_PATH_NAME);
	if(access(VPN_DIR_PATH_NAME, NULL) != 0)  
	{  
		if(mkdir(VPN_DIR_PATH_NAME, 0755) == -1)  
		{   
			AfxWriteDebugLog("SuperVPN run at [AfxGetServerList] create [%s] DIR Fail", VPN_DIR_PATH_NAME);
			return false; 
		}  
	}  

	//���õ�Ini��ʼ�����
	IniFileCheck();

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
	ndStatus ret = RunEnvCheck(ifOnlyCheckUpgrade);
	while(ret != ND_SUCCESS)
	{
		if(ifOnlyCheckUpgrade)
		{
			mShareMem.SetMemValue(ND_TRUE);
			return false;
		}
		
		if(ret == ND_NEED_RESTART)
		{
			AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Server Notify ReInit^_^");
			goto REINIT;
		}
		sleep(8);
		ret = RunEnvCheck(ifOnlyCheckUpgrade);
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
	if(!mPHelloSrv->IfAlreadyRun())
	{
		if (mPHelloSrv->Start()) 
			AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START ERROR...");
		else
			AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START WORKING...");	
	}

	return true;
}

/*********************************************************
����˵����Ini�ļ����
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CSuperVPNApp::IniFileCheck()
{
	FILE *pFile;
	bool bMustUpdateFile = false;

	remove(NODEID_FILE_NAME);
	remove(NODEPWD_FILE_NAME);
	remove(TASK_CHECK_FILE_NAME);
	
	if(access(NODEID_INI_FILE_NAME, NULL) != 0)  
	{  
		if ((pFile = fopen(NODEID_INI_FILE_NAME, "w+")) == NULL)
		{
			AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::IniFileCheck] Create [%s] File Fail", NODEID_INI_FILE_NAME);
			return false;
		}	
		fclose(pFile);
	} 

	CIniDoc iniDoc;
	iniDoc.LoadFromFile(NODEID_INI_FILE_NAME);
	if(!iniDoc.ifExistsItem(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_NODEID))
	{
		iniDoc.PutItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_NODEID, "");
		bMustUpdateFile = true;
	}

	if(!iniDoc.ifExistsItem(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_NODEPWD))
	{
		iniDoc.PutItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_NODEPWD, "");
		bMustUpdateFile = true;
	}	

	if(!iniDoc.ifExistsItem(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_TASKTIME))
	{
		iniDoc.PutItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_TASKTIME, "2");
		bMustUpdateFile = true;
	}

	if(!iniDoc.ifExistsItem(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_HOSTVERSION))
	{
		iniDoc.PutItemString(NODEID_INI_FILE_SECT_SETTINGS, NODEID_INI_FILE_SECT_ITEM_HOSTVERSION, "000");
		bMustUpdateFile = true;
	}	

	if(bMustUpdateFile) iniDoc.SaveToFile(NODEID_INI_FILE_NAME);		
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
	if(nodeid == NULL || strlen(nodeid) == 0){
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
	mPCenterHost->CenterHostCheck();
	
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
	mPCenterHost = new CCenterHost();
	mBNeedRestart = ND_FALSE;
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
	delete mPCenterHost;
}

/*********************************************************
����˵�����ڵ�������ʱ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::NodeRestartFunc(ndULong param)
{
	mBNeedRestart = ND_TRUE;

	//char cmd[512]={0};
	//sprintf(cmd, "killall %s", EDGE_EXE_FILE_NAME);
	//AfxExecCmd(cmd);
	//sprintf(cmd, "%s", "killall %s", VPN_EXE_FILE_NAME);
	//AfxExecCmd(cmd);
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

	while(mBNeedRestart == ND_FALSE)
	{
		//�ж��Ƿ���-U�����reinit�Ķ��������������������
		if(mShareMem.GetMemValue() == ND_TRUE)
		{
			mShareMem.SetMemValue(ND_FALSE);
			break;
		}
		
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


