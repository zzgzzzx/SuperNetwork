//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef UPDATE_CHECK_HTTPCLIENT_SRV_H
#define UPDATE_CHECK_HTTPCLIENT_SRV_H

#include "NDType.hpp"
#include "HttpRunEnvCK.hpp"
#include "NodeBase.hpp"


/**
 * CHttpRunEvnCKSrv Class
 */
class CHttpRunEvnCKSrv : public CHttpRunEvnCK
{
public:
	CHttpRunEvnCKSrv(CNodeBase *node);	
private:
	//iptable检测
	virtual ndStatus IPTableCheck();
	//check请求
	ndStatus MakeCheckReq();	
	//Check应答
	ndStatus AnalysisCheckRsp();	
};

#endif //VPN_SRV_NODE_HTTPCLIENT_H
