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

	//�ڵ�����
	virtual ndStatus MakeNodeEnvSetReq();
	virtual ndStatus AnalysisNodeEnvSetRsp();

	//�ڵ�Hello
	virtual ndStatus MakeNodeHelloReq();
	virtual ndStatus AnalysisNodeHelloRsp();

	//��ȡ�������б�
	virtual ndStatus MakeServerListReq();
	ndStatus AnalysisServerListRsp(list<SServerInfo> &mServers);

	//��ȡ�������
	ndStatus MakeBindServerReq(ndString devidentify);
	ndStatus AnalysisBindServerRsp(list<SBindInform> &mServers);	

	//ʧ������֪ͨ
	ndStatus MakeServiceErrorReq(SBindInform &sBI);
	ndStatus AnalysisServiceErrorRsp(SBindInform &sBI);		

	//�ͷŷ������
	ndStatus MakeUnBindServerReq(SBindInform sBI);
	ndStatus AnalysisUnBindServerRsp();		

	//���ݰ����Ͳ����մ���
	ndStatus PkgSendAndRecv(ndString url);	

public:
    CHttpGeneral();
	CHttpGeneral(CNodeBase *node);

	ndStatus NodeEnvSet();
	ndStatus NodeHello();
	ndStatus NodeInit();
	
	ndBool GetServerList(list<SServerInfo> &mServers);
	ndStatus GetIdentifyService(list<SBindInform> &ltBSer);
	ndStatus ReleaseIdentifyService(SBindInform sBI);

	ndStatus ServiceErrorNotify(SBindInform &sBindInform);
	
    virtual ~CHttpGeneral();

};

#endif //VPN_NODE_HTTPCLIENT_H
