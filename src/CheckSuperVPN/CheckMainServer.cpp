/*********************************************************
ģ����       : 
�ļ���       :MainServer.cpp
����ļ�     :
�ļ�ʵ�ֹ��� :���������
����         :Lewis
�汾         :1.0 
**********************************************************/
#include "BaseApp.hpp"

CBaseApp *gPSuperVPNApp;

int main(int argc,char *argv[])
{
	gPSuperVPNApp = new CCheckSuperVPNApp();
	gPSuperVPNApp->Run(argc, argv);

	return true;
}
