/*********************************************************
ģ����       : 
�ļ���       :Packet.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :���ݰ���
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include "NDStruct.hpp"

//���ݰ����ȶ���(��̫����MTU)
#define PACKET_BUFF_LEN 1472

//////////////////////////////////////////////////////////
//���ݰ�����                     
/////////////////////////////////////////////////////////
class CPacket
{
protected:
    //���͵�����buff
    ndString mSendBuf;
    //���յ�����buff
    ndString mRcvBuf;

	//���ݰ�������Ϣ
	SNetParam mRcvNP,mSndNP;				
					
	//������ݰ�
	virtual bool CheckPkt();		
	//�������ݰ�
	virtual bool AnalysePkt();	
	//�������ݰ�
	virtual bool DealWithPkt();	
public:	
	//���캯��
	CPacket();
	//��������
	virtual ~CPacket();					

	//��ȡ���������Ϣ(���صĶ���������)
	SNetParam GetRcvNetParam();	
	
	//�豸���������Ϣ
	void SetRcvNetParam(const SNetParam &sNP);	
	//�������ݰ�
	void SetRcvPktBuf(char *buf,int len);	

	//�������������Ϣ
	void SetSndNetParam(const SNetParam &sNP);		

	//�������ݰ�
	bool DisposePkt();	

	//��¡���ݰ�
	virtual CPacket *Clone();

	//����UDP���ݰ�
	bool SendTo();								
};


#endif
