//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODEBASE_H
#define VPN_NODEBASE_H

#include "NDType.hpp"
#include "HttpClient.hpp"

/**
 * NodeBase Class
 */
class CNodeBase
{
protected:
	//�ڵ����������Ϣ
	SNodeInform mSNodeInform;
	
    //���صĴ�����
    ndULong  mErrCode;

    //����ص�����
    ndCallbackFunction mCallBackFunc;

public:
    CNodeBase();
    virtual ~CNodeBase();

	//�ڵ������������������Ϣ
	virtual ndStatus NodeEnvSet()=0;

	//�ڵ��ʼ��
	virtual ndStatus NodeInit()=0;
	//��ȡ�������б�
	virtual ndBool GetServerList(list<SServerInfo> &mServers)=0;	
	

	//�첽�ص�������
    static void *ThreadFunction(void *param);

    //���ûص�����(����callbackFunc)
    void SetAsyncMode(ndCallbackFunction callbackFunction);
    //��ȡ�ص�
    ndCallbackFunction GetCallBackFunction();

	void SetNodeInform(SNodeInform &inform);

	virtual void DealHelloAddNewDomain(list<SDomain> &domain);

	void SetNodeID(ndString nodeid);

	SNodeInform &GetNodeInform();

    //��ȡ������
    ndULong GetLastErrCode();
    //���ô�����
    void SetLastErrCode(ndULong code);
};

#endif //TVPN_CLINETBASE_H
