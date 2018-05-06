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
*							INI  文件类										     *
*********************************************************************************/
// INI文件类
#define MAX_FILENAME_LEN	50
#define MAX_LINE_LEN		1024
#define MAX_NAME_LEN		50
#define MAX_VAL_LEN			1024

// SECT名字的最大长度
#define SECT_NAME_LEN		50
// KEY名字的最大长度
#define KEY_NAME_LEN		50 
// KEY 中一个VALUE的最大长度 及 | | 之间数据的长度
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
	// 获取SECT的大小，既有多少个KEY
	int Size();
	// 清空说有的KEY
	int Clear();
	// 添加一个KEY
	int PutItem( char *pcItemName,const char *pcItemVal );
	// 获取一个KEY
	CIniItem * GetItem( char *pcItemName );
	// 删除一个KEY
	int DelItem(char *pcItemName );
	// 获取和指定KEY(pItem) 类似的KEY
	int GetAllItem(char *pItem, char buf[][KEY_NAME_LEN]);
	// 获取KEY里面最大的一个
	int GetMaxKey(char *pitem);
};

class CIniDoc
{
public:	
	list <CIniSect *> m_lstIniSects;
public:
	CIniDoc();
	~CIniDoc(void);
	// 获取指定SECT的指针
	CIniSect * GetSect(char *pcSect);
	// 修改SET的名称
	int RenameSet(char *pOldSet, char *pNewName);
	// 获取SECT下某一KEY的值
	char *GetItemString(char *pcSect,char *pcItem);
	// 检测某个选项是否存在
	bool ifExistsItem(char *pcSect,char *pcItem);
	// 获取SECT名称相似的组
	int GetRelationSectInform(const char  *pSect,char Sect[][SECT_NAME_LEN]);	
	// 获取指定SECTION下相似的KEY
	int GetItemInfo(char *pSect, char *pItem, char buf[][KEY_NAME_LEN]);
	// 获取SECT下一个KEY的最大项:USER1 USER 3 USER4 取4
	int GetMaxKey(char *pSect, char *pItem);
	// 获取一个SETT的最大项:GOURP_1 
	int GetMaxSect(char *pSect);
	// 获取RuleList项的最大项：1_RuleList_2
	int GetMaxRuleList(char *pSect);
	// 获取KEY的ID值
	int GetItemID(char *pItem);
	// 获取SECT的ID
	int GetSectID(char *pSect);
	/*	
	函数说明：移动一个SECT到某项之前或之后 
	函数参数：要移动项:pMoveSect 移动到：pSect Flag=0->之前 Flag=1->之后
	函数返回：移动成功返回 1，失败返回0
	*/
	int Move(char *pMoveSect, char *pSect, int Flag);
	// 添加一个SECT一个KEY一个VALUE
	int PutItemString(char *pcSect,char *pcItem,const char *pcValue);
	// 添加一个SECT
	int PutSect(char *pcSect);
	// 删除一个SECT
	int DelSect(char *pcSect);
	// 删除指定SECT下的一个KEY
	int DelItem(char *pcSect,char *pcItem);
	// 清空m_lstIniSects表
	int Clear();
	// 导入INI文件
	int LoadFromFile(char *pcFileName);
	// 保存到INI文件
	int SaveToFile(char *pcFileName);
private:
	void vTrim(char *pcBuf);
	void vLTrim(char *pcBuf);
	void vRTrim(char *pcBuf);
};

#endif
