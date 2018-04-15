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
	//�ڵ��ʼ��
	virtual ndStatus NodeInit();

	//�ڵ������������������Ϣ
	virtual ndStatus NodeEnvSet();

	//������е�EDGE����
	void CleanAllEdge();

	//��ȡ�������б�
	virtual ndBool GetServerList(list<SServerInfo> &mServers);
};

#endif //VPN_NODE_GENERAL_H