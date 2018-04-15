/*********************************************************
ģ����       : �û��ڵ���(Certificate\DP)
�ļ���       : NodeUser.cpp
����ļ�     : NodeUser.h
�ļ�ʵ�ֹ��� : �û��ڵ���Ĺ���
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeUser.hpp"
#include "HttpUserNode.hpp"
#include "NDFunc.hpp"

/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeUser::CNodeUser()
{
	mPHttpClient = new CHttpUserNode(this);
	mIPTableIndex = 0;
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeUser::~CNodeUser()
{
}

/*********************************************************
����˵��������ʼ���������������������Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeUser::NodeEnvSet()
{
	//=============================================================================
    //���ò���(��ȡ���ؿ��õ�ip��ַ����Ϣ\supernode��Ϣ)
    //=============================================================================
    ndStatus ret = mPHttpClient->NodeEnvSet();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] Node Evn set error=[%d]", ret);
		return ret;
	}

	//=============================================================================
	//��ȡ�����豸MAC�����ID��Ӧ�Ĺ�ϵ(���������ݿ��ȡ),���������ݵĳ�ʼ��
	//=============================================================================
	if (!AfxGetIdentifySet()->InitIdentifyFromGW())
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] ReadMacIdentifyFromGW");
		return ND_ERROR_INVALID_PARAM;	
	}

	return ND_SUCCESS;
}

/*********************************************************
����˵���������û�����������й���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeUser::SetEdgeAndRoute(list<SBindInform> ltBSer)
{
	//need complete by lewis
	SBindInform sRI;

	char ExecCMD[1024] = {0};		
	sprintf(ExecCMD, "iptables -t mangle -A PREROUTING -s %s -j MARK --set-mark %d", sRI.sDeviceIP.c_str(), mIPTableIndex);
	//AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
	AfxExecCmd(ExecCMD);

		sprintf(ExecCMD, "ip rule add fwmark %d table %d", mIPTableIndex, mIPTableIndex);
	//AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
	AfxExecCmd(ExecCMD);

		sprintf(ExecCMD, "ip route add 0/0 via %s table %d", sRI.sServiceIP.c_str(), mIPTableIndex);
	//AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
	AfxExecCmd(ExecCMD);

	mIPTableIndex++;

	BindInformItr iter = ltBSer.begin();
	SBindInform *pBI = new SBindInform();
	*pBI = *iter;

	//֪ͨIdentifySet���ӳ��ڰ���Ϣ
	AfxGetIdentifySet()->AddItem(pBI->sDeviceFlag, pBI);

	return ND_SUCCESS;
}

/*********************************************************
����˵�����Ƴ������豸��vpnͨ����·����Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeUser::RemoveEdgeAndRoute(SBindInform BI)
{
	return ND_SUCCESS;
}

/*********************************************************
����˵����ʧ���ķ���֪ͨ���ķ��������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeUser::ServiceErrorNotify(SBindInform &sBindInform)
{
	//��ȡ�������ڵİ���Ϣ	
    ndStatus ret = mPHttpClient->ServiceErrorNotify(sBindInform);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::ServiceErrorNotify] ErrorCode=[%d]", ret);		
	}
	return ret;
}


/*********************************************************
����˵���������ķ��������������豸��ݶ�Ӧ�ĳ�����Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeUser::BindIdentifyService(SBindInform sBindInform)
{
	list<SBindInform> ltBSer;
	ltBSer.push_back(sBindInform);

	return BindIdentifyService(ltBSer);
}

ndStatus CNodeUser::BindIdentifyService(list<SBindInform> ltBSer)
{
	//��ȡ�������ڵİ���Ϣ	
    ndStatus ret = mPHttpClient->GetIdentifyService(ltBSer);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::BindIdentifyService] Get Services error=[%d]", ret);
		return ret;
	}

	//=============================================================================
	//�������ϻ�ȡmac��ip�Ķ�Ӧ��ϵ������vpnͨ��
	//=============================================================================	
	if (!AfxGetIdentifySet()->ReadARP(ltBSer))
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::BindIdentifyService] Get ARP error=[%d]", ret);
		return ND_ERROR_INVALID_PARAM;	
	}

	//=============================================================================
	//����vpnͨ�����¶�Դ��ַ����·��
	//=============================================================================	
	ret = SetEdgeAndRoute(ltBSer);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::BindIdentifyService] SetEdgeAndRoute error=[%d]", ret);
		return ret;
	}	
}


/*************************************************************
����˵���������ķ�����֪ͨ�����豸��ݶ�Ӧ�ĳ�����Ϣ�ͷ�(����)
���˵������
����˵������
����ֵ  ����
**************************************************************/
ndStatus CNodeUser::UnBindIdentifyService(SBindInform sBindInform)
{
	if (!AfxGetIdentifySet()->FindItem(sBindInform.sDeviceFlag))
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::UnBindIdentifyService] Not Founc Identify");
		return ND_ERROR_INVALID_REQUEST;	
	}

	//֪ͨ���ĳ��ڵİ���Ϣ�ͷ�	
    ndStatus ret = mPHttpClient->ReleaseIdentifyService(sBindInform);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::UnBindIdentifyService] Get Services error=[%d]", ret);
		return ret;
	}

	//�Ƴ�
	RemoveEdgeAndRoute(sBindInform);
	
	//�Ƴ����ݼ���Ӧ�ļ�¼��Ϣ
	AfxGetIdentifySet()->DelItem(sBindInform.sDeviceFlag);
}


