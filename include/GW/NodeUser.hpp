//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_USER_H
#define VPN_NODE_USER_H

#include "NodeGeneral.hpp"

/**
 * NodeUser Class
 */
class CNodeUser: public CNodeGeneral
{
private:
	//IPTable������
	int mIPTableIndex;
public:
    CNodeUser();
    virtual ~CNodeUser();

	//����ʼ���������������������Ϣ
	ndStatus NodeEnvSet();
	//�����û�����������й���
	ndStatus SetEdgeAndRoute(list<SBindInform> ltBSer);
	//�Ƴ������豸��vpnͨ����·����Ϣ
	ndStatus RemoveEdgeAndRoute(SBindInform BI);
	//�����ķ��������������豸��ݶ�Ӧ�ĳ�����Ϣ(����)
	ndStatus BindIdentifyService(SBindInform sBindInform);
	ndStatus BindIdentifyService(list<SBindInform> ltBSer);
	//�����ķ�����֪ͨ�����豸��ݶ�Ӧ�ĳ�����Ϣ�ͷ�(����)
	ndStatus UnBindIdentifyService(SBindInform sBindInform);
	//ʧ���ķ���֪ͨ���ķ��������
	ndStatus ServiceErrorNotify(SBindInform &sBindInform);
};

#endif //VPN_NODE_USER_H