/*********************************************************
ģ����       : 
�ļ���       :CCenterHost.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :���ķ������б������
����         :lewis 
�汾         :1.0 
**********************************************************/
#ifndef __CENTERHOST_HPP__
#define __CENTERHOST_HPP__

#include "Packet.hpp"
#include "NDStruct.hpp"
#include "NDType.hpp"

//**********************************
//�豸��ݰ󶨳�����Ϣ��                       
//**********************************
class CCenterHost
{
private:
	ndString mVersion;
	list<SServerInfo> mListHost; 
	list<SServerInfo>::iterator mITCur,mITBak;

	void HostInit();
	ndBool UpdateServerList(list<SServerInfo> &mServers);
	void JsonToServerList(ndString json, list<SServerInfo> &mServers);
	ndString ServerListToJson(list<SServerInfo> &mServers);
	ndBool GetHostsFromSrv();

	ndBool GetLocalHosts();
public:
	CCenterHost();
	~CCenterHost();

	ndStatus CenterHostCheck();
	SServerInfo GetCurrentHost();
	ndBool GetNextHost(SServerInfo &host);
	void CurrentHostOK();
};

#endif
