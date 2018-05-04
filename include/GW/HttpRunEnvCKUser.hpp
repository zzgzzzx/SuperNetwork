//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef UPDATE_CHECK_HTTPCLIENT_USER_H
#define UPDATE_CHECK_HTTPCLIENT_USER_H

#include "NDType.hpp"
#include "HttpRunEnvCK.hpp"
#include "NodeBase.hpp"
#include "BaseApp.hpp"

/**
 * CHttpRunEvnCKUser Class
 */
class CHttpRunEvnCKUser : public CHttpRunEvnCK
{
public:
	CHttpRunEvnCKUser(CNodeBase *node);	
private:
	//check«Î«Û
	ndStatus MakeCheckReq();		
};

#endif //VPN_SRV_NODE_HTTPCLIENT_H
