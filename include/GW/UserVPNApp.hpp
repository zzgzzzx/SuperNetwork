/****************** Copyright (c) 2017 **************
ģ����       : 
�ļ���       :CUserVPNApp.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __USER_VPN_APP_HPP__
#define __USER_VPN_APP_HPP__

#include "BaseApp.hpp"
#include "HttpSrvThread.hpp"
#include "IdentifySet.hpp"
#include "ARPSet.hpp"

//**********************************
//CSrvVPNApp������                       
//**********************************
class CUserVPNApp: public CSuperVPNApp
{
private:
	//http����
	CHttpSrvThread mHttpSrv;
	//���ʶ�������
	CIdentifySet mIdentifySet;
	//ARP�б������
	CARPSet mARPSet;
public:

	//ϵͳ���ݳ�ʼ��
	bool InitSystem(bool ifOnlyCheckUpgrade);
	
	//��ʾ�汾��Ϣ
	virtual void ShowVersion();
	//ϵͳ���л������
	ndStatus RunEnvCheck(bool ifOnlyCheckUpgrade);	
	//��ȡ����б�
	CIdentifySet *GetIdentifySet();
	//��ȡARP�б�
	CARPSet *GetArpSet();
	//��ȡ�豸����
	ndString GetDeviceType();
	//arp���
	static void ArpCheckFunc(ndULong param);		

	//���캯��
	CUserVPNApp();
	//��������
	~CUserVPNApp();		
};

#endif
