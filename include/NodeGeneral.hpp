//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_GENERAL_H
#define VPN_NODE_GENERAL_H

#include "NodeBase.hpp"
#include "HttpGeneral.hpp"

/**
 * NodeGeneral Class
 */
class CNodeGeneral: public CNodeBase
{
protected:
	//ͨѶ�Ŀͻ���
	CHttpGeneral *mPHttpClient;
	//�̵߳Ĵ���
	static void *ThreadFunction(void *param);		
public:
    CNodeGeneral();
    virtual ~CNodeGeneral();

    //���ݴ���ǰ�ĳ�ʼ������
    virtual ndStatus InitData();
	//��ȡIP
	ndStatus GetIP();
	
	//�ڵ��ʼ��
	virtual ndStatus NodeInit();

	//�ڵ������������������Ϣ
	virtual ndStatus NodeEnvSet();

	//��ȡ�������б�
	virtual ndBool GetServerList(SCenterHost &mServers);
};

#endif //VPN_NODE_GENERAL_H