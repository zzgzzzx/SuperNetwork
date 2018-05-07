/****************** Copyright (c) 2017 **************
ģ����       : 
�ļ���       :CSrvVPNApp.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __SRV_VPN_APP_HPP__
#define __SRV_VPN_APP_HPP__

#include "BaseApp.hpp"

//**********************************
//CSrvVPNApp������                       
//**********************************
class CSrvVPNApp: public CSuperVPNApp
{
public:
	//��ʾ�汾��Ϣ
	virtual void ShowVersion();
	//ϵͳ���л������
	ndStatus RunEnvCheck(bool ifOnlyCheckUpgrade);	
	//��ȡ�豸����
	ndString GetDeviceType();	

	//ϵͳ���ݳ�ʼ��
	bool InitSystem(bool ifOnlyCheckUpgrade);	

	//ian���
	static void IanCheckFunc(ndULong param);	

	//���캯��
	CSrvVPNApp();
	//��������
	~CSrvVPNApp();		
};

#endif
