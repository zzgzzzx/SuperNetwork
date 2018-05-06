/*********************************************************
模块名       : 
文件名       :CCenterHost.hpp
相关文件     : 
文件实现功能 :中心服务器列表管理类
作者         :lewis 
版本         :1.0 
**********************************************************/
#ifndef __CENTERHOST_HPP__
#define __CENTERHOST_HPP__

#include "Packet.hpp"
#include "NDStruct.hpp"
#include "NDType.hpp"

//**********************************
//设备身份绑定出口信息类                       
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
