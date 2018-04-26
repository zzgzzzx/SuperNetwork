/*********************************************************
ģ����       : ���������������(Certificate\DP)
�ļ���       : ClientSrvBase.cpp
����ļ�     : ClientSrvBase.h
�ļ�ʵ�ֹ��� : ʵ��������������Ĺ���
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeGeneral.hpp"
#include "HttpGeneral.hpp"
#include "NDFunc.hpp"


/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeGeneral::CNodeGeneral()
{
	mPHttpClient = NULL;
	AfxGetEthMac("eth0", mSNodeInform.sNodeMac);	
	//mSNodeInform.sNodeMac = "aa:bb:cc:dd:ee:ff";
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeGeneral::~CNodeGeneral()
{
	if(mPHttpClient != NULL) delete mPHttpClient;
}

/*********************************************************
����˵������ɵ����������������
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeGeneral::InitData()
{
    return ND_SUCCESS;
}

/*********************************************************
����˵�����̴߳�����
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
void *CNodeGeneral::ThreadFunction(void *cmd)
{
    pthread_detach(pthread_self());

	char *ExecCMD = (char*)(cmd);
	AfxExecCmd(ExecCMD);
	free(ExecCMD);

    AfxWriteDebugLog("SuperVPN run at [%s] Thread End", __FUNCTION__);
    pthread_exit(0);
}

/*********************************************************
����˵�����ڵ��ʼ��
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeGeneral::NodeInit()
{
	ndStatus ret = mPHttpClient->NodeInit();
	AfxWriteDebugLog("SuperVPN run at [CNodeGeneral::NodeEnvSet] Node init return=[%d]", ret);
	return ret;
}

/*********************************************************
����˵������ȡIP
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeGeneral::GetIP()
{
	ndStatus ret = mPHttpClient->GetIP();
	AfxWriteDebugLog("SuperVPN run at [CNodeGeneral::NodeEnvSet] Node init return=[%d]", ret);
	return ret;

}

/*********************************************************
����˵�����ڵ㻷������
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeGeneral::NodeEnvSet()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵����GetServerList
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndBool CNodeGeneral::GetServerList(list<SServerInfo> &mServers)
{
	return mPHttpClient->GetServerList(mServers);
}


