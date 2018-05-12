/*********************************************************
ģ����       : 
�ļ���       :ARPset.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :ARP������
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __ARP_SET_HPP__
#define __ARP_SET_HPP__

#include "NDStruct.hpp"
#include "Inform.hpp"

/*
 *	ARP:mac��ַ----SBindInform
 */
class CARPSet: public CMap<ndString, SBindInform *>
{
private:
	//arp����Ƿ���������ʶ
	ndBool mBArpCheckRun;
	//��ȡ��ǰ��arp�б���Ϣ
	bool GetCurrentARP(list<SArp> &liarp);

	//���±�ʶ����Ϊfalse
	void RestUpdateFalse();
	
public:
	//���캯��
	CARPSet();
	//��������
	~CARPSet();

	//���ӽڵ�
	bool AddItem(ndString mac, SBindInform *pBI);
	//ɾ���ڵ�
	bool DelItem(ndString mac);
	//���ҽ��
	SBindInform *FindItem(ndString mac);
	//���ҽ��
	SBindInform *FindItemNL(ndString mac);

	//���ARP
	bool ARPCheck();

	//�������������ݿ��ж�ȡMAC����ݵ�ʶ���벢����ҵ��
	bool InitIdentifyFromGW();	
};

#endif
