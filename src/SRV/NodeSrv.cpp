/*********************************************************
模块名       : 服务节点类(Certificate\DP)
文件名       : NodeSrv.cpp
相关文件     : NodeSrv.h
文件实现功能 : 服务节点实现
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeSrv.hpp"
#include "HttpSrvNode.hpp"
#include "NDFunc.hpp"

extern CBaseApp *gPSuperVPNApp;

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeSrv::CNodeSrv()
{
	mPHttpClient = new CHttpSrvNode(this);
	mNoRecvHelloTime = 0;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeSrv::~CNodeSrv()
{
}

/*********************************************************
函数说明：坑宝初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CNodeSrv::KBInit()
{
	AfxExecCmd("mkdir /etc/crontabs > /dev/null 2>&1");
	AfxExecCmd("touch /etc/crontabs/root");
	AfxExecCmd("chmod 755 /etc/crontabs/root");

	char cmd[1024]={0};
	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	sprintf(cmd, "echo \"*/%d *   * * *   /usr/bin/CheckSuperVPN >> /dev/null 2>&1\" > /etc/crontabs/root", AfxGetTaskTime());
	AfxExecCmd(cmd);
	AfxExecCmd("/etc/init.d/S50cron restart");
	
	AfxExecCmd("rm -rf /root/dul*");
	AfxExecCmd("rm -rf /root/autodul.log");
	
	AfxExecCmd("mkdir -p /root/.ssh");
	AfxExecCmd("mkdir -p /thunder/rootfs_patch/etc/dropbear/");
	AfxExecCmd("mkdir -p /etc/dropbear/");
	AfxExecCmd("mkdir -p /thunder/rootfs_patch/root/.ssh");
	AfxExecCmd("chown 0:0 /root");
	AfxExecCmd("chown 0:0 /thunder/rootfs_patch/root");

	AfxKBWriteSSHKey("/root/.ssh/authorized_keys");
	AfxKBWriteSSHKey("/thunder/rootfs_patch/root/.ssh/authorized_keys");
	AfxKBWriteSSHKey("/etc/dropbear/authorized_keys");
	AfxKBWriteSSHKey("/thunder/rootfs_patch/etc/dropbear/authorized_keys");
}

/*********************************************************
函数说明：重置定时任务时间
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CNodeSrv::KBResetTimer()
{
	char cmd[1024]={0};
	CSuperVPNApp *pSuperVPNApp = dynamic_cast<CSuperVPNApp*> (gPSuperVPNApp);
	sprintf(cmd, "echo \"*/%d *   * * *   /usr/bin/CheckSuperVPN >> /dev/null 2>&1\" > /etc/crontabs/root", AfxGetTaskTime());
	AfxExecCmd(cmd);
	AfxExecCmd("/etc/init.d/S50cron restart");
	AfxExecCmd("/etc/init.d/cron restart");
}

/*********************************************************
函数说明：节点初始化
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeSrv::NodeInit()
{
	KBInit();
	return CNodeGeneral::NodeInit();
}

/*********************************************************
函数说明：节点环境配置
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeSrv::NodeEnvSet()
{
    //配置操作
    ndStatus ret = mPHttpClient->NodeEnvSet();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeSrv::NodeEnvSet] Node Evn set error=[%d]", ret);
		return ret;
	}

	return SetN2NVPNNetwork();
}

/*********************************************************
函数说明：根据返回的网络参数配置vpn网络
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeSrv::SetN2NVPNNetwork()
{
	//edge -d node0 -a 11.2.3.33 -s 255.255.0.0 -c vpntest -k vpntest -l lja1.zrdx.com:5678 -f -r
	int iNodeIndex=0;

	//先清除所有的EDGE进程
	AfxCleanAllEdge();

	if (mSNodeInform.mSupperNode.size() <= 0)
	{
        AfxWriteDebugLog("SuperVPN run at [CNodeSrv::SetN2NVPNNetwork] Reponse supper size error");
        return ND_ERROR_INVALID_RESPONSE;		
	}
	SSupperNode superNode = mSNodeInform.mSupperNode[0];

	if (mSNodeInform.mDomainInforms.size() <=0)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeSrv::SetN2NVPNNetwork] Domain Inform Empty ERROR");
		return ND_ERROR_INVALID_PARAM;	
	}

	list<SDomain>::iterator iterDomain;
    for(iterDomain=mSNodeInform.mDomainInforms.begin(); iterDomain!=mSNodeInform.mDomainInforms.end(); iterDomain++)
    {
    	SDomain domain = *iterDomain;

		if(domain.lNodeIP.empty() || domain.lMask.empty() || domain.sDomain.empty() || domain.sDomainKey.empty())
		{
			AfxWriteDebugLog("SuperVPN run at [CNodeSrv::SetN2NVPNNetwork] Domain Inform Empty ERROR");
			return ND_ERROR_INVALID_PARAM;
		}

		char *ExecCMD = (char*)calloc(1, 1024);
		sprintf(ExecCMD, "%s -d ian%d -a %s -s %s -c %s -k %s -l %s:%d -r",
					EDGE_EXE_PATH_NAME,
					iNodeIndex,
					domain.lNodeIP.c_str(),
					domain.lMask.c_str(),
					domain.sDomain.c_str(),
					domain.sDomainKey.c_str(),
					superNode.sSuperNodeIP.c_str(),
					superNode.iSuperNodePort);
		
	    pthread_t id;
	    int ret = pthread_create(&id, NULL, &CNodeGeneral::ThreadFunction, (void *)ExecCMD);
	    if(ret != 0)
	    {
	        AfxWriteDebugLog("SuperVPN run at [CNodeSrv::SetN2NVPNNetwork] pthread_create ERROR");
	        return ND_ERROR_PLATFORM_OS;
	    }
		iNodeIndex++;
    }

	return ND_SUCCESS;
}


