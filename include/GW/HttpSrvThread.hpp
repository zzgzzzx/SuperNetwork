/****************** Copyright (c) 2008 **************
ģ����       : Http�������ݾݽ��մ�����
�ļ���       : HttpSrv.hpp
����ļ�	    : HttpSrv.cpp
�ļ�ʵ�ֹ��� : ���մ���Http Server���͵���Ϣ
����         : zhangzx 
�汾         : 1.0 
**********************************************************/
#ifndef __HTTP_SRV_HPP__
#define __HTTP_SRV_HPP__

#include "BThread.hpp"
#include "Udp.hpp"

class CHttpSrvThread: public CBaseThread
{
private:
	//�Ƿ���Ҫ�˳���ʶ
	ndBool mStopRun;	
	//��������socket
	bool StartService();
protected:
	//�̴߳�����
	virtual void ProcessVirtual(void);
public:
	//���캯��
	CHttpSrvThread();
	//��������
	~CHttpSrvThread();
	//�������ݰ�
	int SendTo(ndUInt32 IP,ndInt16 iPort,char *pBuf,int iBufLen);
};

#endif
