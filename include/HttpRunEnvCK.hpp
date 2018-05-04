//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef UPDATE_CHECK_HTTPCLIENT_H
#define UPDATE_CHECK_HTTPCLIENT_H

#include "NDType.hpp"
#include "HttpGeneral.hpp"
#include "NodeBase.hpp"

using namespace network;

/**
 * CHttpRunEvnCK Class
 */
class CHttpRunEvnCK : public CHttpGeneral
{
public:
	CHttpRunEvnCK(CNodeBase *node);	
	//开始检测
	ndStatus BeginCheck(char *appname, bool ifOnlyCheckUpgrade);	
	
private:
	//edge检测
	ndStatus EdgeCheck();
	//deamon检测
	ndStatus CheckSuperVPNCheck();
	
	//node检测
	ndStatus NodeCheck();
	
	//下载文件
	ndStatus Download(ndString filename, list<ndString> urls, ndString md5);

protected:
	//当前版本号
	int mLocalVersion;	
	//检测数据
	SRunEnvCKSt mRunEnvCK;	
	//iptable检测
	virtual ndStatus IPTableCheck();
	//check请求
	virtual ndStatus MakeCheckReq() = 0;	
	//Check应答
	virtual ndStatus AnalysisCheckRsp();		
};

#endif //VPN_SRV_NODE_HTTPCLIENT_H
