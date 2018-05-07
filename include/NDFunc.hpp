/****************** Copyright (c) 2008 **************
ģ����       : 
�ļ���       :Func.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :ȫ�ֺ�������
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __FUNC_HPP__
#define __FUNC_HPP__

#include <iconv.h> 
#include "BaseApp.hpp"
#include "NDType.hpp"
#include "NDStruct.hpp"
#include "HelloSrvThread.hpp"

/*---------------------------------------------------------------------------------------
//��ʱ��
-----------------------------------------------------------------------------------------*/
//���뵥�ض�ʱ��	
bool AfxInsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));
//����ѭ����ʱ��	
bool AfxInsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));

//ɾ��ѭ����ʱ��
bool AfxDeleteCircleTimer(unsigned long callid);
//ɾ��������ʱ��
bool AfxDeleteSingleTimer(unsigned long callid);
//���ݰ�д�����
void AfxInsertPktToSysQue(CPacket *pkt);

//��ȡHello����
CHelloSrvThread *AfxGetHelloSrv();

//��ȡvpn�ڵ�
CNodeGeneral *AfxGetVPNNode();

//��ȡ�������б�
CCenterHost *AfxGetCenterHost();


//��ȡ��������
ndBool AfxGetGatewayName(ndString &host);


/*---------------------------------------------------------------------------------------
//��־
-----------------------------------------------------------------------------------------*/
//д������־
void AfxWriteDebugLog(char *Format,...);
//��ȡ��һ�����õķ�������Ϣ
bool AfxGetNextSrvUrl(SServerInfo &serverInfo);

//�ڵ����ļ���д
void AfxWriteNodeID(const char *nodeid);
char *AfxGetNodeID();
//�ڵ������ļ���д
void AfxWriteNodePwd(const char *nodepwd);
char *AfxGetNodePwd();
//��������ļ���д
void AfxWriteTaskTime(int time);
int AfxGetTaskTime();
//�������б�汾��
void AfxWriteHostVersion(const char *version);
char *AfxGetHostVersion();

//��ȡmac��ַ
void AfxGetEthMac(const char *ethname, ndString &mac);

bool AfxFileExist(const string filepath);
ndDouble AfxFileSize(const string filepath);

bool AfxCheckCmdExist(const string cmd);

bool AfxRunCmdGetResult(const ndString cmd, ndString &result);


//ִ��ϵͳ����
bool AfxExecCmd(const char *cmd);

//�ӱ�дssh key�ļ�Ŀ¼
bool AfxKBWriteSSHKey(const char *filename);

//������е�EDGE����
void AfxCleanAllEdge();



/*---------------------------------------------------------------------------------------
//Socket��д����
-----------------------------------------------------------------------------------------*/
//socket������(����ͬ���ķ�ʽ)
int AfxCompleteRead(int iSocket,	 //socket�׽���
					void *pvBuffer,			//���ջ�����
					unsigned long ulSize, 		//����������
					unsigned long ulTimeout);	//��ʱʱ��

//socketд����(����ͬ���ķ�ʽ)
int AfxCompleteWrite(int iSocket,	//socket�׽���
					 const void *pvBuffer,		//���ͻ�����
					 unsigned long ulSize, 		//����������
					 unsigned long ulTimeout);	//��ʱʱ��

////socketд����(�����첽�ķ�ʽ)
//int AfxCompleteWrite(int iSocket,				//socket�׽���
//					 const void *pvBuffer,		//���ͻ�����
//					 unsigned long ulSize 		//����������
//					 );

/*---------------------------------------------------------------------------------------
//�ַ���ת������
-----------------------------------------------------------------------------------------*/
//gbk==>utf-8
int AfxGBK2UTF8(const char *src, char *des, int maxlen);

void AfxChangeToUTF8(string &context);

/*---------------------------------------------------------------------------------------
//�ַ���������
-----------------------------------------------------------------------------------------*/
StrVector AfxStrTokenize(const string& src, 	//Դ��
						 const string& tok, 	//�ָ���
						 int num=0, 			//���ٷ��ص�Ԫ�ظ���
						 bool trim=false,		//�Ƿ���˿մ�
						 string null_subst=""); //�մ�������ַ���

string AfxByteArrayToHexStr(const ndUChar src[],	//Դ����
							const int &srclen); //�����ַ�����
								

//��ؿ�
inline void AfxvRTrim(char *pcBuf)
{
	int iLen;
	iLen=strlen(pcBuf);
	while( iLen>0)
	{
		if ((' '==pcBuf[iLen-1])||('\t'==pcBuf[iLen-1])||('\n'==pcBuf[iLen-1])||('\r'==pcBuf[iLen-1]))
		{
			pcBuf[iLen-1]='\0';
			iLen--;
		}
		else
			break;
	}
}

inline void AfxvLTrim(char *pcBuf)
{
	int iLen;
	iLen=strlen(pcBuf);
	while( iLen>0 )
	{
		if ((' '==pcBuf[0])||('\t'==pcBuf[0]))
		{
			strcpy(pcBuf,pcBuf+1);
			iLen--;
		}
		else
			break;
	}
}

//���ҽؿ�
inline void AfxvTrim(char *pcBuf)
{
	AfxvRTrim(pcBuf);
	AfxvLTrim(pcBuf);
}

//unsigned long int inet_addr(const char *cp);

//������IP��ַת���ַ���
inline char *AfxNetIPToStr(const ndULong &IP)
{
	static char sIP[32];

	sprintf(sIP,"%d.%d.%d.%d",
		((unsigned char *)&IP)[0], 
		((unsigned char *)&IP)[1], 
		((unsigned char *)&IP)[2], 
		((unsigned char *)&IP)[3]);

	return sIP;
}

//������IP��ַת���ַ���
inline char *AfxHostIPToStr(const ndULong &IP)
{
	static char sIP[32];

	sprintf(sIP,"%d.%d.%d.%d",
		((unsigned char *)&IP)[3], 
		((unsigned char *)&IP)[2], 
		((unsigned char *)&IP)[1], 
		((unsigned char *)&IP)[0]);

	return sIP;
}

//��ȡ�Է���IP��ַ
inline ndULong AfxGetSrcIP(int socket)
{
	struct sockaddr_in sa;
	socklen_t sl = sizeof(struct sockaddr_in);

	if (getpeername(socket, (struct sockaddr *)&sa, &sl) < 0) return 0;

	return sa.sin_addr.s_addr;
}

int AfxGetAllIfName(set<string> &vIFName);

#endif
