//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef VPN_USER_NODE_HTTPCLIENT_H
#define VPN_USER_NODE_HTTPCLIENT_H

#include "NDType.hpp"
#include "HttpGeneral.hpp"
#include "NodeBase.hpp"


/**
 * CHttpUserNode Class
 */
class CHttpUserNode : public CHttpGeneral
{
public:
	CHttpUserNode(CNodeBase *node);
	
protected:
	//Node初始化
	ndStatus MakeNodeInitReq();
	//获取IP
	ndStatus MakeNodeGetIPReq();		
	//节点配置
	ndStatus MakeNodeEnvSetReq();
	ndStatus AnalysisNodeEnvSetRsp();	
	//Node Hello
	ndStatus MakeNodeHelloReq();
	//获取服务器列表
	ndStatus MakeServerListReq();	
	//Node Hello Response
	ndStatus AnalysisNodeHelloRsp();	
};

#endif //VPN_USER_NODE_HTTPCLIENT_H
