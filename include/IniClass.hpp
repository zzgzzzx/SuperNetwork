#ifndef __INICLASS_H__
#define __INICLASS_H__
#include <iostream>
#include <list>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

/*********************************************************************************
*							INI  �ļ���										     *
*********************************************************************************/
// INI�ļ���
#define MAX_FILENAME_LEN	50
#define MAX_LINE_LEN		1024
#define MAX_NAME_LEN		50
#define MAX_VAL_LEN			1024

// SECT���ֵ���󳤶�
#define SECT_NAME_LEN		50
// KEY���ֵ���󳤶�
#define KEY_NAME_LEN		50 
// KEY ��һ��VALUE����󳤶� �� | | ֮�����ݵĳ���
#define KEY_VALUE_LEN		64

class CIniItem
{
public:	
	char m_acItemName[MAX_NAME_LEN];
	char m_acVal[MAX_VAL_LEN];
};

class CIniSect
{
public:
	list <CIniItem *> m_lstIniItems;
	char acSectName[MAX_NAME_LEN];
public:
	CIniSect();
	~CIniSect();
	// ��ȡSECT�Ĵ�С�����ж��ٸ�KEY
	int Size();
	// ���˵�е�KEY
	int Clear();
	// ���һ��KEY
	int PutItem( char *pcItemName,const char *pcItemVal );
	// ��ȡһ��KEY
	CIniItem * GetItem( char *pcItemName );
	// ɾ��һ��KEY
	int DelItem(char *pcItemName );
	// ��ȡ��ָ��KEY(pItem) ���Ƶ�KEY
	int GetAllItem(char *pItem, char buf[][KEY_NAME_LEN]);
	// ��ȡKEY��������һ��
	int GetMaxKey(char *pitem);
};

class CIniDoc
{
public:	
	list <CIniSect *> m_lstIniSects;
public:
	CIniDoc();
	~CIniDoc(void);
	// ��ȡָ��SECT��ָ��
	CIniSect * GetSect(char *pcSect);
	// �޸�SET������
	int RenameSet(char *pOldSet, char *pNewName);
	// ��ȡSECT��ĳһKEY��ֵ
	char *GetItemString(char *pcSect,char *pcItem);
	// ���ĳ��ѡ���Ƿ����
	bool ifExistsItem(char *pcSect,char *pcItem);
	// ��ȡSECT�������Ƶ���
	int GetRelationSectInform(const char  *pSect,char Sect[][SECT_NAME_LEN]);	
	// ��ȡָ��SECTION�����Ƶ�KEY
	int GetItemInfo(char *pSect, char *pItem, char buf[][KEY_NAME_LEN]);
	// ��ȡSECT��һ��KEY�������:USER1 USER 3 USER4 ȡ4
	int GetMaxKey(char *pSect, char *pItem);
	// ��ȡһ��SETT�������:GOURP_1 
	int GetMaxSect(char *pSect);
	// ��ȡRuleList�������1_RuleList_2
	int GetMaxRuleList(char *pSect);
	// ��ȡKEY��IDֵ
	int GetItemID(char *pItem);
	// ��ȡSECT��ID
	int GetSectID(char *pSect);
	/*	
	����˵�����ƶ�һ��SECT��ĳ��֮ǰ��֮�� 
	����������Ҫ�ƶ���:pMoveSect �ƶ�����pSect Flag=0->֮ǰ Flag=1->֮��
	�������أ��ƶ��ɹ����� 1��ʧ�ܷ���0
	*/
	int Move(char *pMoveSect, char *pSect, int Flag);
	// ���һ��SECTһ��KEYһ��VALUE
	int PutItemString(char *pcSect,char *pcItem,const char *pcValue);
	// ���һ��SECT
	int PutSect(char *pcSect);
	// ɾ��һ��SECT
	int DelSect(char *pcSect);
	// ɾ��ָ��SECT�µ�һ��KEY
	int DelItem(char *pcSect,char *pcItem);
	// ���m_lstIniSects��
	int Clear();
	// ����INI�ļ�
	int LoadFromFile(char *pcFileName);
	// ���浽INI�ļ�
	int SaveToFile(char *pcFileName);
private:
	void vTrim(char *pcBuf);
	void vLTrim(char *pcBuf);
	void vRTrim(char *pcBuf);
};

#endif
