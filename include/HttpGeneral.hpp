//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef VPN_NODE_HTTPCLIENT_H
#define VPN_NODE_HTTPCLIENT_H

#include "NDType.hpp"
#include "HttpClient.hpp"
#include "NodeBase.hpp"
#include "Inform.hpp"

using namespace network;

/**
 * CHttpClientNode Class
 */
class CHttpGeneral : public CHttpClient
{
protected:
	//结点信息
	CNodeBase *mPNode;
	
    //生成认证头
    virtual ndString GenerateAuthHeader();
	//分析认证头
    virtual void AnalysisAuthHeader(ndString &head);

	//节点初始化
	virtual ndStatus MakeNodeInitReq();
	virtual ndStatus AnalysisNodeInitRsp();

	//获取IP
	ndStatus MakeNodeGetIPReq();
	ndStatus AnalysisNodeGetIPRsp();	

	//节点配置
	virtual ndStatus MakeNodeEnvSetReq();
	virtual ndStatus AnalysisNodeEnvSetRsp();

	//获取服务器列表
	virtual ndStatus MakeServerListReq();
	ndStatus AnalysisServerListRsp(list<SServerInfo> &mServers);	

	//数据包发送并接收处理
	ndStatus PkgSendAndRecv(ndString url);	

public:
    CHttpGeneral();
	CHttpGeneral(CNodeBase *node);

	ndStatus NodeEnvSet();
	ndStatus NodeInit();
	ndStatus GetIP();	
	
	ndBool GetServerList(list<SServerInfo> &mServers);
	
    virtual ~CHttpGeneral();

};

#endif //VPN_NODE_HTTPCLIENT_H
