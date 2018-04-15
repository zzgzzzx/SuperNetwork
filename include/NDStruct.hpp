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
					��־����
----------------------------------------------------------------*/
/* ��־���� */
typedef struct
{
	ndChar	IfOpenDebug;	//�Ƿ���������־
	ndChar	DebugType;		//������־�����ʽ(1:��Ļ��� 2:�ļ���� 4:������)
	ndInt16 CacheTime;		//���ش洢ʱ�Ļ���ʱ��(���ӣ�0��ʾ������);	
	ndInt16 LogDayNum;		//������־�Ĵ洢������(��);
}SLogConf, *pSLogConf;

 /**
 ******************************************************************************
 * @brief   ip pool structure
 * @note    This is the structure for ip pool
 ******************************************************************************
 */
 typedef struct
{
	//���ؿɷ����ip��ʼ��ַ
	ndUInt32 uBeginIP;
	//���ؿɷ����ip������ַ
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
	//vpn�ڵ���������
	ndString 	sDomain;
	//vpn�ڵ��ip��ַ
	ndString 	lNodeIP;
	//����
	ndString	lMask;
	//vpn�ڵ��������key
	ndString 	sDomainKey;
}SDomain;

 typedef struct
{
	 //vpn�����ڵ��IP��ַ
	 ndString	 sSuperNodeIP;
	 //vpn�����ڵ�Ķ˿ں�
	 ndString	 sSuperNodePort;
}SSupperNode;

typedef struct
{
	//vpn�ڵ�ı��
	ndString 	sNodeID;
	//vpn�ڵ������
	ndString	sNodePwd;
	//vpn�ڵ��MAC��ַ
	ndString 	sNodeMac;
	//HelloTime
	ndInt32		lHelloTime;
	//RestartTime
	ndInt32		lRestartTime;	
	//�������������Ϣ
	vector<SSupperNode> mSupperNode;
	//������������Ϣ
	list<SDomain> mDomainInforms;	
	CMap<ndString, SDomain> mDomainMap;
	//�û��ڵ�/���ص�IP��ַ��
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
	 //���ķ�������IP��ַ
	 ndString	 sServerIP;
	 //���ķ������˿ں�
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
	//���ص�URL
    ndString	sUrl;
	//�·��������
    ndBool		bIsDownloadComplete;
	//�����ص��ļ���С
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
	//�µİ汾��
    ndUInt32 	iVerCode;
	//MD5У��ֵ
    ndString	sMD5;
	//���ص�URL���ӵ�ַ
    list<ndString> mDownLodURL;
}SNodeCKSt;

typedef struct
{
	//MD5У��ֵ
    ndString	sMD5;
	//���ص�URL���ӵ�ַ
    list<ndString> mDownLodURL;
}SUPDeamonCKSt;


typedef struct
{
	//MD5У��ֵ
    ndString	sMD5;
	//���ص�URL���ӵ�ַ
    list<ndString> mDownLodURL;
}SEdgeCKSt;

typedef struct
{
	//MD5У��ֵ
    ndString	sMD5;
	//���ص�URL���ӵ�ַ
    list<ndString> mDownLodURL;
}SIPTableCKSt;

typedef struct
{
	SNodeCKSt node;
	SUPDeamonCKSt deamon;
	SEdgeCKSt edge;
	SIPTableCKSt iptable;
}SRunEnvCKSt;

//���ݰ���������ṹ
struct NetParam
{
	ndString	sSrvURL;			//����˷��ʵ�URL
	ndUInt32	lDesIP;				//Ŀ��IP��ַ
	ndUInt32	lSrcIP;				//ԴIP��ַ
	ndUInt16	uDesPort;			//Ŀ�Ķ˿�
	ndUInt16	uSrcPort;			//Դ�˿�
};
typedef struct NetParam SNetParam,*pSNetParam;


//�ַ�����̬���鶨��
typedef vector<string> StrVector;
typedef vector<string>::iterator StrVtrItr;

#endif //VPN_DPCOMMSTR_H

