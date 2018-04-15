/*********************************************************
模块名       : 
文件名       :Udp.cpp
相关文件     :Udp.hpp
文件实现功能 :数据包类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "Udp.hpp"

typedef struct sockaddr SA;

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CUdpSvr::Open(int iPort)//init udp 
{
	int iRet;
	struct sockaddr_in stSockAddr;
	m_iSockfd= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_iSockfd == -1){
		return -1;
	}

	int val = 0;
	val = fcntl(m_iSockfd, F_GETFL, 0) ;
	fcntl(m_iSockfd, F_SETFD, val|FD_CLOEXEC);
		
	int opt = 1;
	setsockopt(m_iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	::memset(&stSockAddr,0,sizeof(stSockAddr));
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_addr.s_addr = HTONL(INADDR_ANY);
	stSockAddr.sin_port = HTONS(iPort);
	iRet = bind(m_iSockfd,(SA*)&stSockAddr,sizeof(stSockAddr));

	return iRet;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CUdpSvr::Open(ndUInt32 ip, int iPort)
{
	struct sockaddr_in  stSockAddr;
	int iRet;
	m_iSockfd= socket(AF_INET, SOCK_DGRAM, 0);
	if(m_iSockfd == -1)	return -1;

	int val = 0;
	val = fcntl(m_iSockfd, F_GETFL, 0) ;
	fcntl(m_iSockfd, F_SETFD, val|FD_CLOEXEC);	

	::memset(&stSockAddr,0,sizeof(stSockAddr));
	int opt = 1;
	setsockopt(m_iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_addr.s_addr = HTONL(ip);
	stSockAddr.sin_port = HTONS(iPort);
	iRet = bind(m_iSockfd,(SA*)&stSockAddr,sizeof(stSockAddr));

	return iRet;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CUdpSvr::RecvFrom(ndUChar *pcMsgBuf, int iBufLen, ndUInt32 &ip, ndUInt16 &port)
{
	int iRevcNum;
	int iCliLen;
	struct sockaddr_in stSockAddr;
	iCliLen = sizeof(stSockAddr);
	iRevcNum = recvfrom(m_iSockfd,pcMsgBuf,iBufLen,0,(SA*)&stSockAddr,(socklen_t*)&iCliLen);
	ip = HTONL(stSockAddr.sin_addr.s_addr);
	port = HTONS(stSockAddr.sin_port);		
	return iRevcNum;    
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ： 0 超时 ;-1 socket err ;-2 socketfd not set ;>0 recv num
*********************************************************/
int CUdpSvr::RecvFrom(char *pcMsgBuf,int iBufLen, int iSeconds)
{
	struct timeval tv;
	int iRet;
	int iCliLen;
	struct sockaddr_in stSockAddr;
	fd_set ReadSet;

	FD_ZERO(&ReadSet);
	FD_SET(m_iSockfd, &ReadSet);

	tv.tv_sec = iSeconds;
	tv.tv_usec = 0;

	iRet = select(m_iSockfd+1, &ReadSet, NULL, NULL, &tv);
	if (iRet>0)
	{	
	
		if(FD_ISSET(m_iSockfd, &ReadSet))
		{
			iCliLen = sizeof(stSockAddr);
			iRet = recvfrom(m_iSockfd,pcMsgBuf,iBufLen,0,(SA*)&stSockAddr,(socklen_t*)&iCliLen);
		}
		else
			iRet = -2;
	}

	return iRet;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CUdpSvr::SendTo(ndUInt32 IP,unsigned short iPort,const char *pcMsgBuf,int iBufLen)
{
	struct sockaddr_in sSO;
	int iSendNum;

	memset(&sSO, 0, sizeof(SA));
	sSO.sin_family = AF_INET;
	sSO.sin_addr.s_addr = HTONL(IP);
	sSO.sin_port = HTONS(iPort);
	iSendNum = sendto(m_iSockfd,pcMsgBuf,iBufLen,0,(SA*)&sSO,sizeof(SA));	

	return iSendNum;	
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CUdpSvr::Close()
{
	return close(m_iSockfd);
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CUdpSvr::SetRecvBuffer(unsigned long ulBufferSize)
{
	//改变接收缓冲区大小
	setsockopt(m_iSockfd, SOL_SOCKET,SO_RCVBUF, &ulBufferSize, sizeof(ulBufferSize));
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CUdpCli::SendTo(char *pcIP,unsigned short iPort,char *pcMsgBuf,int iBufLen)
{
	struct sockaddr_in stSockAddr;
	int iSendNum;
	int iSockfd;	
	iSockfd= socket(AF_INET, SOCK_DGRAM, 0);

	memset(&stSockAddr, 0, sizeof(stSockAddr));
	stSockAddr.sin_family=AF_INET;
	//saAddr.sin_addr.s_addr=HTONL(INADDR_ANY);
	stSockAddr.sin_addr.s_addr=inet_addr(pcIP);
	stSockAddr.sin_port=htons(iPort);
	iSendNum=sendto(iSockfd,pcMsgBuf,iBufLen,0,(SA*)&stSockAddr,sizeof(stSockAddr));
	
	close(iSockfd);
	return iSendNum;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CUdpCli::SendTo(char *pcIP,unsigned short iPort,const void *pcMsgBuf,int iBufLen)
{
	struct sockaddr_in stSockAddr;
	int iSendNum;
	int iSockfd;	
	iSockfd= socket(AF_INET, SOCK_DGRAM, 0);

	memset(&stSockAddr, 0, sizeof(stSockAddr));
	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_addr.s_addr = inet_addr(pcIP);
	stSockAddr.sin_port = HTONS(iPort);
	iSendNum = sendto(iSockfd, pcMsgBuf, iBufLen, 0, (SA*)&stSockAddr, sizeof(stSockAddr));
	
	close(iSockfd);
	return iSendNum;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CUdpCli::Create(unsigned short iLocalPort)
{
	struct sockaddr_in sSA;
	int opt = 1;
	
	iSockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(iSockfd==-1) return false;

	int val = 0;
	val = fcntl(iSockfd, F_GETFL, 0) ;
	fcntl(iSockfd, F_SETFD, val|FD_CLOEXEC);	
	
	setsockopt(iSockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	
	sSA.sin_family = AF_INET;
	sSA.sin_port = HTONS(iLocalPort);
	sSA.sin_addr.s_addr = HTONL(INADDR_ANY);
	bzero(&(sSA.sin_zero), 8);
	if(bind(iSockfd, (struct sockaddr *)&sSA, sizeof(struct sockaddr_in))<0) return false;	

	return true;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CUdpCli::Create(unsigned long uIP,unsigned short iLocalPort)
{
	struct sockaddr_in sSA;
	int opt = 1;
	
	iSockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(iSockfd==-1) return false;

	int val = 0;
	val = fcntl(iSockfd, F_GETFL, 0) ;
	fcntl(iSockfd, F_SETFD, val|FD_CLOEXEC);	
	
	setsockopt(iSockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	
	sSA.sin_family = AF_INET;
	sSA.sin_port = HTONS(iLocalPort);
	sSA.sin_addr.s_addr = HTONL(uIP);
	if(bind(iSockfd, (struct sockaddr *)&sSA, sizeof(struct sockaddr_in))<0) return false;

	return true;
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CUdpCli::Close()
{
	close(iSockfd);
}

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CUdpCli::SendTo(ndUInt32 IP,unsigned short iPort,char *pcMsgBuf,int iBufLen)
{
	struct sockaddr_in sSO;
	int iSendNum;

	memset(&sSO,0,sizeof(SA));
	sSO.sin_family = AF_INET;
	sSO.sin_addr.s_addr = HTONL(IP);
	sSO.sin_port = HTONS(iPort);
	iSendNum = sendto(iSockfd, pcMsgBuf, iBufLen, 0, (SA*)&sSO, sizeof(SA));	

	return iSendNum;	
}

