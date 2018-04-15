// Created by lewis on 2017/3/18.

#ifndef __NDSTRUCT_HPP__
#define __NDSTRUCT_HPP__

#include <string>
#include <string.h>
#include <stdlib.h>
#include <set>
#include <list>
#include <map>
#include <vector>
#include "Map.hpp"
#include "NDType.hpp"

using namespace std;

typedef string ndString;

/*----------------------------------------------------------------
					日志管理
----------------------------------------------------------------*/
/* 日志管理 */
typedef struct
{
	ndChar	IfOpenDebug;	//是否开启调试日志
	ndChar	DebugType;		//调试日志输出方式(1:屏幕输出 2:文件输出 4:监控输出)
	ndInt16 CacheTime;		//本地存储时的缓冲时间(分钟，0表示不缓冲);	
	ndInt16 LogDayNum;		//本地日志的存储的天数(天);
}SLogConf, *pSLogConf;

 /**
 ******************************************************************************
 * @brief   ip pool structure
 * @note    This is the structure for ip pool
 ******************************************************************************
 */
 typedef struct
{
	//网关可分配的ip起始地址
	ndUInt32 uBeginIP;
	//网关可份配的ip结束地址
	ndUInt32 uEndIP;
}SIPPool;

/**
 ******************************************************************************
 * @brief   node inform structure
 * @note    This is the structure for node base inform
 ******************************************************************************
 */
 typedef struct
{
	//vpn节点所属的域
	ndString 	sDomain;
	//vpn节点的ip地址
	ndString 	lNodeIP;
	//掩码
	ndString	lMask;
	//vpn节点所属域的key
	ndString 	sDomainKey;
}SDomain;

 typedef struct
{
	 //vpn超级节点的IP地址
	 ndString	 sSuperNodeIP;
	 //vpn超级节点的端口号
	 ndString	 sSuperNodePort;
}SSupperNode;

typedef struct
{
	//vpn节点的编号
	ndString 	sNodeID;
	//vpn节点的密码
	ndString	sNodePwd;
	//vpn节点的MAC地址
	ndString 	sNodeMac;
	//HelloTime
	ndInt32		lHelloTime;
	//RestartTime
	ndInt32		lRestartTime;	
	//超级结点的相关信息
	vector<SSupperNode> mSupperNode;
	//所属域的相关信息
	list<SDomain> mDomainInforms;	
	CMap<ndString, SDomain> mDomainMap;
	//用户节点/网关的IP地址池
	SIPPool mIPPool;
}SNodeInform, *pSNodeInform;


 /**
  ******************************************************************************
  * @brief	 server list inform structure
  * @note	 This is the structure for server inform
  ******************************************************************************
  */
 typedef struct
{
	 //中心服务器的IP地址
	 ndString	 sServerIP;
	 //中心服务器端口号
	 ndString	 sServerPort;
}SServerInfo;

/*
 ******************************************************************************
 * @brief   File download request structure
 * @note    This is the request structure for
 ******************************************************************************
 */
typedef struct
{
    ndString	sUrl;
    ndString	sLocalFile;
    FILE	*fFile;
    ndDouble iSize;
    ndUInt32 iCRC;
    ndULong iOffset;
}SDownloadFileReqSt;

/**
 ******************************************************************************
 * @brief   File download response structure
 * @note    This is the response structure for
 *			#DownloadFile function.
 ******************************************************************************
 */
typedef struct
{
	//下载的URL
    ndString	sUrl;
	//下否下载完成
    ndBool		bIsDownloadComplete;
	//已下载的文件大小
    ndDouble	uiChunkOffset;
}SDownloadFileRspSt;

/**
 ******************************************************************************
 * @brief   run env check structure
 * @note    This is the response structure for run env check
 ******************************************************************************
 */
typedef struct
{
	//新的版本号
    ndUInt32 	iVerCode;
	//MD5校验值
    ndString	sMD5;
	//下载的URL链接地址
    list<ndString> mDownLodURL;
}SNodeCKSt;

typedef struct
{
	//MD5校验值
    ndString	sMD5;
	//下载的URL链接地址
    list<ndString> mDownLodURL;
}SUPDeamonCKSt;


typedef struct
{
	//MD5校验值
    ndString	sMD5;
	//下载的URL链接地址
    list<ndString> mDownLodURL;
}SEdgeCKSt;

typedef struct
{
	//MD5校验值
    ndString	sMD5;
	//下载的URL链接地址
    list<ndString> mDownLodURL;
}SIPTableCKSt;

typedef struct
{
	SNodeCKSt node;
	SUPDeamonCKSt deamon;
	SEdgeCKSt edge;
	SIPTableCKSt iptable;
}SRunEnvCKSt;

//数据包网络参数结构
struct NetParam
{
	ndString	sSrvURL;			//服务端访问的URL
	ndUInt32	lDesIP;				//目的IP地址
	ndUInt32	lSrcIP;				//源IP地址
	ndUInt16	uDesPort;			//目的端口
	ndUInt16	uSrcPort;			//源端口
};
typedef struct NetParam SNetParam,*pSNetParam;


//字符串动态数组定义
typedef vector<string> StrVector;
typedef vector<string>::iterator StrVtrItr;

#endif //VPN_DPCOMMSTR_H

