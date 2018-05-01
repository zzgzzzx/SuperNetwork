/*********************************************************
模块名       : 节点http协议
文件名       : HttpUpdateCK.cpp
相关文件     : HttpUpdateCK.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpRunEnvCKSrv.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"

/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpRunEvnCKSrv::CHttpRunEvnCKSrv(CNodeBase *node):CHttpRunEvnCK(node)
{
	mLocalVersion = SUPER_VPN_CLIENT_VER_SERVER;
}

/*********************************************************
函数说明：iptable检测
入参说明：无
出参说明：无
返回值  ：无
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

