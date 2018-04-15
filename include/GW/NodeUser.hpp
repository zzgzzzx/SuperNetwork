//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_USER_H
#define VPN_NODE_USER_H

#include "NodeGeneral.hpp"

/**
 * NodeUser Class
 */
class CNodeUser: public CNodeGeneral
{
private:
	//IPTable的索引
	int mIPTableIndex;
public:
    CNodeUser();
    virtual ~CNodeUser();

	//结点初始化，向服务器申请配置信息
	ndStatus NodeEnvSet();
	//根据用户的特征码进行关联
	ndStatus SetEdgeAndRoute(list<SBindInform> ltBSer);
	//移除下线设备的vpn通道与路由信息
	ndStatus RemoveEdgeAndRoute(SBindInform BI);
	//向中心服务器请求下游设备身份对应的出口信息(上线)
	ndStatus BindIdentifyService(SBindInform sBindInform);
	ndStatus BindIdentifyService(list<SBindInform> ltBSer);
	//向中心服务器通知下游设备身份对应的出口信息释放(下线)
	ndStatus UnBindIdentifyService(SBindInform sBindInform);
	//失联的服务通知中心服务器变更
	ndStatus ServiceErrorNotify(SBindInform &sBindInform);
};

#endif //VPN_NODE_USER_H