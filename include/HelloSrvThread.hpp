/****************** Copyright (c) 2008 **************
ģ����       : Hello��������
�ļ���       : HelloSrv.hpp
����ļ�	    : HelloSrv.cpp
�ļ�ʵ�ֹ��� : ���մ���Hello Server���͵���Ϣ
����         : zhangzx 
�汾         : 1.0 
**********************************************************/
#ifndef __HELLOSRV_HPP__
#define __HELLOSRV_HPP__

#include "BThread.hpp"
#include "Udp.hpp"
#include "NDStruct.hpp"

class CHelloSrvThread: public CBaseThread
{
private:
	//������socket
	CUdpSvr mSrvSkt;
	//��������socket
	bool StartService();
	//���ղ���������
	void RcvAndDealMsg();
	//�ڵ�hello
	static void NodeHelloFunc(ndULong param);		
protected:
	//�̴߳�����
	virtual void ProcessVirtual(void);
public:
	//���캯��
	CHelloSrvThread();
	//��������
	~CHelloSrvThread();
	//�������ݰ�
	int SendTo(ndUInt32 IP, ndInt16 iPort, const char *pBuf, int iBufLen);
};

#endif
