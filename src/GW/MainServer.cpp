/*********************************************************
ģ����       : 
�ļ���       :MainServer.cpp
����ļ�     :
�ļ�ʵ�ֹ��� :���������
����         :Lewis
�汾         :1.0 
**********************************************************/
#include "UserVPNApp.hpp"

CSuperVPNApp *gPSuperVPNApp;

int main(int argc,char *argv[])
{
	gPSuperVPNApp = new CUserVPNApp();
	gPSuperVPNApp->Run(argc, argv);

	return true;
}
