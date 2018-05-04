/*********************************************************
模块名       : 用户节点类(Certificate\DP)
文件名       : NodeUser.cpp
相关文件     : NodeUser.h
文件实现功能 : 用户节点类的功能
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeUser.hpp"
#include "HttpUserNode.hpp"
#include "NDFunc.hpp"
#include "UserVPNApp.hpp"

extern CSuperVPNApp *gPSuperVPNApp;

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeUser::CNodeUser()
{
	mPHttpClient = new CHttpUserNode(this);
	mIPTableIndex = 0;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeUser::~CNodeUser()
{
}

/*********************************************************
函数说明：结点初始化，向服务器申请配置信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CNodeUser::NodeEnvSet()
{
	//=============================================================================
    //配置操作(获取网关可用的ip地址段信息\supernode信息)
    //=============================================================================
    ndStatus ret = mPHttpClient->NodeEnvSet();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] Node Evn set error=[%d]", ret);
		return ret;
	}

	//=============================================================================
	//获取下游设备MAC与身份ID对应的关系(从网关数据库获取),并进行数据的初始化
	//=============================================================================
	CUserVPNApp *pSuperVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp);
	if (!pSuperVPNApp->GetIdentifySet()->InitIdentifyFromGW())
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] ReadMacIdentifyFromGW");
		return ND_ERROR_INVALID_PARAM;	
	}

	return ND_SUCCESS;
}

/*********************************************************
函数说明：根据用户的特征码进行关联
入参说明：无
出参说明：无
返回值  ：无
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

	//通知IdentifySet增加出口绑定信息
	CUserVPNApp *pSuperVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp);
	pSuperVPNApp->GetIdentifySet()->AddItem(pBI->sDeviceFlag, pBI);

	return ND_SUCCESS;
}

/*********************************************************
函数说明：移除下线设备的vpn通道与路由信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CNodeUser::RemoveEdgeAndRoute(SBindInform BI)
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：失联的服务通知中心服务器变更
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CNodeUser::ServiceErrorNotify(SBindInform &sBindInform)
{
	//获取身份与出口的绑定信息	
	CHttpUserNode *pHttpUser = dynamic_cast<CHttpUserNode *>(mPHttpClient);
    ndStatus ret = pHttpUser->ServiceErrorNotify(sBindInform);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::ServiceErrorNotify] ErrorCode=[%d]", ret);		
	}
	return ret;
}


/*********************************************************
函数说明：向中心服务器请求下游设备身份对应的出口信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CNodeUser::BindIdentifyService(SBindInform sBindInform)
{
	list<SBindInform> ltBSer;
	ltBSer.push_back(sBindInform);

	return BindIdentifyService(ltBSer);
}

ndStatus CNodeUser::BindIdentifyService(list<SBindInform> ltBSer)
{
	//获取身份与出口的绑定信息	
	CHttpUserNode *pHttpUser = dynamic_cast<CHttpUserNode *>(mPHttpClient);
    ndStatus ret = pHttpUser->GetIdentifyService(ltBSer);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::BindIdentifyService] Get Services error=[%d]", ret);
		return ret;
	}

	//=============================================================================
	//在网关上获取mac与ip的对应关系，配置vpn通道
	//=============================================================================	
	CUserVPNApp *pSuperVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp);	
	if (!pSuperVPNApp->GetIdentifySet()->ReadARP(ltBSer))
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::BindIdentifyService] Get ARP error=[%d]", ret);
		return ND_ERROR_INVALID_PARAM;	
	}

	//=============================================================================
	//配置vpn通道与下端源地址策略路由
	//=============================================================================	
	ret = SetEdgeAndRoute(ltBSer);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::BindIdentifyService] SetEdgeAndRoute error=[%d]", ret);
		return ret;
	}	
}


/*************************************************************
函数说明：向中心服务器通知下游设备身份对应的出口信息释放(下线)
入参说明：无
出参说明：无
返回值  ：无
**************************************************************/
ndStatus CNodeUser::UnBindIdentifyService(SBindInform sBindInform)
{
	CUserVPNApp *pSuperVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp); 
	if (!pSuperVPNApp->GetIdentifySet()->FindItem(sBindInform.sDeviceFlag))
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::UnBindIdentifyService] Not Founc Identify");
		return ND_ERROR_INVALID_REQUEST;	
	}

	//通知中心出口的绑定信息释放	
	CHttpUserNode *pHttpUser = dynamic_cast<CHttpUserNode*> (mPHttpClient);	
    ndStatus ret = pHttpUser->ReleaseIdentifyService(sBindInform);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::UnBindIdentifyService] Get Services error=[%d]", ret);
		return ret;
	}

	//移除
	RemoveEdgeAndRoute(sBindInform);
	
	//移除数据集对应的记录信息
	pSuperVPNApp->GetIdentifySet()->DelItem(sBindInform.sDeviceFlag);
}


