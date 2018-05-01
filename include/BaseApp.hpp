/****************** Copyright (c) 2017 **************
ģ����       : 
�ļ���       :CBaseApp.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __BASEAPP_HPP__
#define __BASEAPP_HPP__

#include "MsgQueue.hpp"
#include "TimerManager.hpp"
#include "NodeGeneral.hpp"
#include "Packet.hpp"
#include "HelloSrvThread.hpp"

//**********************************
//Ӧ�ù��̻���                       
//**********************************
class CBaseApp
{
private:
	//��ʱ����Ա
	CTimerManager mTimer;

protected:
	//Ӧ�ó����ʼ������
	virtual bool InitApplication(int argc,char *argv[]);
public:	
	//���뵥�ض�ʱ��	
	bool InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));
	//����ѭ����ʱ��	
	bool InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));

	//ɾ��ѭ����ʱ��
	bool DeleteCircleTimer(unsigned long callid);
	//ɾ��������ʱ��
	bool DeleteSingleTimer(unsigned long callid);

	//Ӧ�ó�����������
	void Run(int argc,char *argv[]);

	//���캯��
	CBaseApp();
	//��������
	virtual ~CBaseApp();
};

//**********************************
//SuperVPN������                       
//**********************************
class CSuperVPNApp: public CBaseApp
{
private:
	//��Ϣ������
	CMsgFIFO<CPacket*> mPktQueue;		

	//�������б���
	ndStatus ServerListCheck();
 	//ϵͳ���л������
 	virtual ndStatus RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade)=0;

	//�ڵ��ʼ����ż��
	ndStatus NodeInitCheck();
	//����http������
	ndStatus StartHttpd();
	//���ݰ�����ѭ������
	void TranslatePkt(void);
	//������(��������������)
	ndBool CKArgvAndLock(int argc,char *argv[], ndBool &ifOnlyCheckUpgrade);
	//�������ƻ���
	ndBool GetRunLock();
	//�����������ƻ���
	ndBool GetUpgradeLock();	
	
protected:
	//ϵͳ���ݳ�ʼ��
	bool InitSystem(char *appname, bool ifOnlyCheckUpgrade);	
	//Ӧ�ó����ʼ������
	bool InitApplication(int argc,char *argv[]);

public:
	//�ڵ�
	CNodeGeneral *mPNode;
	//hello����
	CHelloSrvThread *mPHelloSrv;
	//��ʱ���������ʱ��
	int mCheckTime;

	//��ȡ���ʱ��
	int GetCheckTime();
	//���ü��ʱ��
	void SetCheckTime(int time);

	//��ʾ�汾��Ϣ
	virtual void ShowVersion()=0;
	//��ȡ�豸����
	virtual ndString GetDeviceType()=0;	
	
	//���캯��
	CSuperVPNApp();
	//��������
	~CSuperVPNApp();	

	//�ڵ�����
	static void NodeRestartFunc(ndULong param);	

	//��Ϣ���ݰ����
	void InsertPktToQueue(CPacket *pkt);	
};

//**********************************
//SuperVPN��⹤����                       
//**********************************
class CCheckSuperVPNApp: public CBaseApp
{
private:	
	//ϵͳ���ݳ�ʼ��
	bool InitSystem(void);
	void StopSuperVPNAndUpgrade();
	void CheckVPNAndRun();

protected:
	//Ӧ�ó����ʼ������
	bool InitApplication(int argc,char *argv[]);

public:
	
	//���캯��
	CCheckSuperVPNApp();
	//��������
	~CCheckSuperVPNApp();	
};

#endif
