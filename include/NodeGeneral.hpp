//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_GENERAL_H
#define VPN_NODE_GENERAL_H

#include "NodeBase.hpp"
#include "HttpGeneral.hpp"

/**
 * NodeGeneral Class
 */
class CNodeGeneral: public CNodeBase
{
protected:
	//通讯的客户端
	CHttpGeneral *mPHttpClient;
	//线程的处理
	static void *ThreadFunction(void *param);		
public:
    CNodeGeneral();
    virtual ~CNodeGeneral();

    //数据处理前的初始化操作
    virtual ndStatus InitData();
	//获取IP
	ndStatus GetIP();
	
	//节点初始化
	virtual ndStatus NodeInit();

	//节点向服务器申请配置信息
	virtual ndStatus NodeEnvSet();

	//获取服务器列表
	virtual ndBool GetServerList(SCenterHost &mServers);
};

#endif //VPN_NODE_GENERAL_H