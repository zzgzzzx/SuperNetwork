//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef VPN_SRV_NODE_HTTPCLIENT_H
#define VPN_SRV_NODE_HTTPCLIENT_H

#include "NDType.hpp"
#include "HttpGeneral.hpp"
#include "NodeBase.hpp"


/**
 * CHttpSrvNode Class
 */
class CHttpSrvNode : public CHttpGeneral
{
public:
	CHttpSrvNode(CNodeBase *node);	
	
protected:	
	//Node初始化
	ndStatus MakeNodeInitReq();
	//节点配置
	ndStatus MakeNodeEnvSetReq();
	ndStatus AnalysisNodeEnvSetRsp();		
};

#endif //VPN_SRV_NODE_HTTPCLIENT_H
