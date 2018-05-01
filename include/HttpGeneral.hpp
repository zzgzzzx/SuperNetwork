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
	//�����Ϣ
	CNodeBase *mPNode;
	
    //������֤ͷ
    virtual ndString GenerateAuthHeader();
	//������֤ͷ
    virtual void AnalysisAuthHeader(ndString &head);

	//�ڵ��ʼ��
	virtual ndStatus MakeNodeInitReq();
	virtual ndStatus AnalysisNodeInitRsp();

	//��ȡIP
	ndStatus MakeNodeGetIPReq();
	ndStatus AnalysisNodeGetIPRsp();	

	//�ڵ�����
	virtual ndStatus MakeNodeEnvSetReq();
	virtual ndStatus AnalysisNodeEnvSetRsp();

	//��ȡ�������б�
	virtual ndStatus MakeServerListReq();
	ndStatus AnalysisServerListRsp(list<SServerInfo> &mServers);	

	//���ݰ����Ͳ����մ���
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
