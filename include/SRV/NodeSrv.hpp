//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_SRV_H
#define VPN_NODE_SRV_H

#include "NodeGeneral.hpp"

/**
 * NodeSrv Class
 */
class CNodeSrv: public CNodeGeneral
{
private:
	ndInt8 mNoRecvHelloTime;
	//根据返回的网络参数配置vpn网络
	ndStatus SetN2NVPNNetwork();	
public:
    CNodeSrv();
    virtual ~CNodeSrv();
	//坑宝初始化
	void KBInit();
	//重置定时任务时间
	void KBResetTimer();
	
	//节点初始化
	ndStatus NodeInit();	

	//节点向服务器申请配置信息
	ndStatus NodeEnvSet();
	//ian进程检测
	void IanCheck();
};

#endif //VPN_NODE_SRV_H