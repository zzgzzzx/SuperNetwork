//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_SRV_H
#define VPN_NODE_SRV_H

#include "NodeGeneral.hpp"

/**
 * NodeSrv Class
 */
class CNodeSrv: public CNodeGeneral
{
private:
	ndInt8 mNoRecvHelloTime;
	//���ݷ��ص������������vpn����
	ndStatus SetN2NVPNNetwork();	
public:
    CNodeSrv();
    virtual ~CNodeSrv();
	//�ӱ���ʼ��
	void KBInit();
	
	//�ڵ��ʼ��
	virtual ndStatus NodeInit();	

	//�ڵ������������������Ϣ
	ndStatus NodeEnvSet();
};

#endif //VPN_NODE_SRV_H