/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpUpdateCK.cpp
����ļ�     : HttpUpdateCK.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpRunEnvCKSrv.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"

/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpRunEvnCKSrv::CHttpRunEvnCKSrv(CNodeBase *node):CHttpRunEvnCK(node)
{
	mSrvURL = VPN_CENTER_SRV_URL;
	mLocalVersion = SUPER_VPN_CLIENT_VER_SERVER;
}

/*********************************************************
����˵����iptable���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpRunEvnCKSrv::IPTableCheck()
{
	ndStatus ret = CHttpRunEvnCK::IPTableCheck();
	if (ret != ND_SUCCESS) return ret;

	AfxExecCmd("iptables -F");
	AfxExecCmd("iptables -F -t nat");
	AfxExecCmd("iptables -X");
	AfxExecCmd("iptables -X -t nat");
	AfxExecCmd("iptables -A FORWARD -j ACCEPT");
	AfxExecCmd("iptables -t nat -I POSTROUTING -o eth0 -j MASQUERADE");

	AfxExecCmd("echo \"1\"> /proc/sys/net/ipv4/ip_forward");
	
	return ND_SUCCESS;
}

