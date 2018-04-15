/*********************************************************
ģ����       : 
�ļ���       :Udp.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :���ݰ���
����         :zhangzx 
�汾         :1.0 
**********************************************************/

#ifndef __UDP_HPP__
#define __UDP_HPP__

#include <netinet/in.h>
#include "NDType.hpp"

//**********************************
//UDP SERVER ��                       
//**********************************
class CUdpSvr
{
   int m_iSockfd;

public:
   int Open(int iPort);
   int Open(ndUInt32 ip, int iPort);
   int RecvFrom(ndUChar *pcMsgBuf, int iBufLen, ndUInt32 &ip, ndUInt16 &port);
   int RecvFrom(char *pcMsgBuf,int iBufLen, int iSeconds);
   int Close();
   void SetRecvBuffer(unsigned long ulBufferSize);
   int SendTo(ndUInt32 IP,unsigned short iPort,const char *pcMsgBuf,int iBufLen);
};

//**********************************
//UDP�ͻ�����                       
//**********************************
class CUdpCli
{
private:
	int iSockfd;
public:
	bool Create(unsigned short iLocalPort);
	bool Create(unsigned long uIP,unsigned short iLocalPort);
	int SendTo(char *pcIP,unsigned short iPort,const void *pcMsgBuf,int iBufLen);
	int SendTo(char *pcIP,unsigned short iPort,char *pcMsgBuf,int iBufLen);
	int SendTo(ndUInt32 IP,unsigned short iPort,char *pcMsgBuf,int iBufLen);
	void Close();
};
#endif
