/*********************************************************
模块名       : 
文件名       :BaseApp.cpp
相关文件     :BaseApp.hpp
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"

/*********************************************************
函数说明：application初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CCheckSuperVPNApp::InitApplication(int argc,char *argv[])
{
	AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] ...");

	//系统数据初始化
	AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem()) 
	{
		AfxWriteDebugLog("CheckSuperVPN run at [CCheckSuperVPNApp::InitApplication] Init System END......");
		return false;
	}
	
	return true;
}

/*********************************************************
函数说明：检测SuperVPN并启动
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CCheckSuperVPNApp::CheckVPNAndRun()
{
	char cmd[512]={0};
	ndString result;

	sprintf(cmd, "ps | grep %s | grep -v grep | grep -v %s | wc -l", VPN_EXE_FILE_NAME, CHECK_VPN_EXE_FILE_NAME);
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::CheckVPNAndRun] Begin get SuperVPN process num");	
	if (AfxRunCmdGetResult(cmd, result))
	{
		AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::CheckVPNAndRun] Process Num=[%d]", atoi(result.c_str()));	
		if(atoi(result.c_str()) <= 0)
		{
			AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::CheckVPNAndRun] Begin Run %s", VPN_EXE_PATH_NAME);	
			sprintf(cmd, "%s&\n", VPN_EXE_PATH_NAME);
			AfxExecCmd(cmd);
		}
	}
}

/*********************************************************
函数说明：停止SuperVPN执行并升级
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CCheckSuperVPNApp::StopSuperVPNAndUpgrade()
{
	char cmd[512]= {0};
	sprintf(cmd, "killall %s", VPN_EXE_FILE_NAME);
	AfxExecCmd(cmd);
	sprintf(cmd, "mv %s %s\n", VPN_UPGRADE_FILE_NAME, VPN_EXE_PATH_NAME);
	AfxExecCmd(cmd);
}

/*********************************************************
函数说明：系统数据初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CCheckSuperVPNApp::InitSystem(void)
{
	//调用SuperVPN检测升级
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] begin exec supervpn -u");	
	AfxExecCmd("SuperVPN -u");

	//判断是否有新版的升级文件存在，如果有，结束SuperVPN，进行文件的替换
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] begin check if exists upgrade file");	
	if(AfxFileExist(VPN_UPGRADE_FILE_NAME))
	{
		AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] stop all supervpn process");
		StopSuperVPNAndUpgrade();
	}

	//SuperVPN进程是否运行的检测，如果没启动，就进行启动
	AfxWriteDebugLog("CheckSuperVPN run at[CCheckSuperVPNApp::InitSystem] check and run supervpn process");
	CheckVPNAndRun();

	return true;
}

/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CCheckSuperVPNApp::CCheckSuperVPNApp()
{
}

/*********************************************************
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CCheckSuperVPNApp::~CCheckSuperVPNApp()
{
}

