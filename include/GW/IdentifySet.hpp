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
#include "HelloPkt.hpp"

/*
 *	�û����ʶ����Ϊkey---����
 */
class CIdentifySet: public CMap<ndString, SBindInform *>
{
private:
	//��macΪkey��map
	CMap<ndString, SBindInform*>mMacMap;
	//�Գ��ڷ���Ϊkey��map
	CMap<ndString, ServiceInform*> mServiceMap;
	//�����������ļ��ж�ȡmac�����ʶ��Ĺ�����
	bool ReadMacIdentifyFromGW(list<SBindInform> &ltSI);

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
	bool DealRcvPkt(CHelloPkt *pkt);

	//�������������ݿ��ж�ȡMAC����ݵ�ʶ���벢����ҵ��
	bool InitIdentifyFromGW();
	//ͨ��ARP���ȡMAC��IP��Ӧ��ϵ
	bool ReadARP(list<SBindInform> &ltBSer);

	//�Ƿ��г�ʱ�ļ��
	void SendHelloAndCheck();
};

#endif
