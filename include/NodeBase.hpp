//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODEBASE_H
#define VPN_NODEBASE_H

#include "NDType.hpp"
#include "HttpClient.hpp"

/**
 * NodeBase Class
 */
class CNodeBase
{
protected:
	//节点基本参数信息
	SNodeInform mSNodeInform;
	
    //返回的错误码
    ndULong  mErrCode;

    //处理回调函数
    ndCallbackFunction mCallBackFunc;

public:
    CNodeBase();
    virtual ~CNodeBase();

	//节点初服务器申请配置信息
	virtual ndStatus NodeEnvSet()=0;

	//节点初始化
	virtual ndStatus NodeInit()=0;
	//获取服务器列表
	virtual ndBool GetServerList(SCenterHost &mServers)=0;	
	

	//异步回调处理函数
    static void *ThreadFunction(void *param);

    //设置回调处理(设置callbackFunc)
    void SetAsyncMode(ndCallbackFunction callbackFunction);
    //获取回调
    ndCallbackFunction GetCallBackFunction();

	void SetNodeInform(SNodeInform &inform);

	virtual void DealHelloAddNewDomain(list<SDomain> &domain);

	void SetNodeID(ndString nodeid);
	void SetNodePWD(ndString pwd);

	void SetNodeOutIP(ndString outip);

	SNodeInform &GetNodeInform();

    //获取错误码
    ndULong GetLastErrCode();
    //设置错误码
    void SetLastErrCode(ndULong code);
};

#endif //TVPN_CLINETBASE_H
