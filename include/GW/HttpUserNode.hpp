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

	ndStatus GetIdentifyService(list<SBindInform> &ltBSer);
	ndStatus ReleaseIdentifyService(SBindInform sBI);
	ndStatus ServiceErrorNotify(SBindInform &sBindInform);	
	
protected:
	//Node初始化
	ndStatus MakeNodeInitReq();
		
	//节点配置
	ndStatus MakeNodeEnvSetReq();
	ndStatus AnalysisNodeEnvSetRsp();	

	//获取服务出口
	ndStatus MakeBindServerReq(list<SBindInform> &ltBSer);
	ndStatus AnalysisBindServerRsp(list<SBindInform> &mServers);	

	//失联服务通知
	ndStatus MakeServiceErrorReq(SBindInform &sBI);
	ndStatus AnalysisServiceErrorRsp(SBindInform &sBI);		

	//释放服务出口
	ndStatus MakeUnBindServerReq(SBindInform sBI);
	ndStatus AnalysisUnBindServerRsp();		
};

#endif //VPN_USER_NODE_HTTPCLIENT_H
