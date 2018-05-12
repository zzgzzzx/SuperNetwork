/*********************************************************
ģ����       : 
�ļ���       :IDentifySet.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :�û���ʶ������
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __IDENTIFY_SET_HPP__
#define __IDENTIFY_SET_HPP__

#include "Set.hpp"
#include "UserHelloPkt.hpp"

/*
 *	�û����ʶ����Ϊkey---����
 */
class CIdentifySet: public CMap<ndString, SBindInform *>
{
private:
	//��macΪkey��map
	CMap<ndString, SBindInform*> mMacMap;
	//�Գ��ڷ���Ϊkey��map
	CMap<ndString, ServiceInform*> mServiceMap;

public:
	//���캯��
	CIdentifySet();
	//��������
	~CIdentifySet();

	//���ӽڵ�
	bool AddItem(ndString Key,SBindInform *pBI);
	//ɾ���ڵ�
	bool DelItem(ndString Key);
	//���ҽ��
	SBindInform *FindItem(ndString Key);
	//���ҽ��
	SBindInform *FindItemNL(ndString Key);
	//����mac���ҽ��
	SBindInform *FindItemByMac(ndString mac);

	//���մ������ݰ�
	bool DealRcvPkt(CUserHelloPkt *pkt);

	//�Ƿ��г�ʱ�ļ��
	void SendHelloAndCheck();
	
	//�����ķ��������������豸��ݶ�Ӧ�ĳ�����Ϣ(����)
	ndStatus BindIdentifyService(SBindInform sBindInform);
	//�����ķ�����֪ͨ�����豸��ݶ�Ӧ�ĳ�����Ϣ�ͷ�(����)
	ndStatus UnBindIdentifyService(SBindInform sBindInform);	
};

#endif
