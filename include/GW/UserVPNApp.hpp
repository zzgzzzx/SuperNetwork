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

//**********************************
//CSrvVPNApp������                       
//**********************************
class CUserVPNApp: public CSuperVPNApp
{
public:
		
	//ϵͳ���ݳ�ʼ��
	bool InitSystem(char *appname, bool ifOnlyCheckUpgrade);
	
	//��ʾ�汾��Ϣ
	virtual void ShowVersion();
	//ϵͳ���л������
	ndStatus RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade);	

	//���캯��
	CUserVPNApp();
	//��������
	~CUserVPNApp();		
};

#endif
