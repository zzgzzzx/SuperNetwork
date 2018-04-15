/*********************************************************
模块名       : 
文件名       :Udp.hpp
相关文件     : 
文件实现功能 :数据包类
作者         :zhangzx 
版本         :1.0 
**********************************************************/

#ifndef __UDP_HPP__
#define __UDP_HPP__

#include <netinet/in.h>
#include "NDType.hpp"

//**********************************
//UDP SERVER 类                       
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
//UDP客户端类                       
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
