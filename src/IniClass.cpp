
#include "IniClass.hpp"
#include "NDFunc.hpp"

/////////////////////////////////////////////////////////////////////////
//							CIniSect								   //
/////////////////////////////////////////////////////////////////////////
CIniSect::CIniSect()
{

}

CIniSect::~CIniSect()
{
	this->Clear();
}

int CIniSect::Size()
{
	return m_lstIniItems.size();
}

int CIniSect::Clear()
{
	list <CIniItem *>::iterator it;
	for (it=m_lstIniItems.begin();it!=m_lstIniItems.end();it++)
	{
		delete *it;
	}
	m_lstIniItems.clear();
	return 0;
}

int CIniSect::DelItem(char *pcItemName )
{
	CIniItem *poIniItem;
	poIniItem=GetItem(pcItemName);

	if (poIniItem==NULL) return 1;

	m_lstIniItems.remove(poIniItem);
	delete poIniItem;

	return 0;
}

int CIniSect::GetAllItem(char *pItem, char buf[][KEY_NAME_LEN])
{
	int		cnt =0;
	char *p;
	list <CIniItem *>::iterator it;
	CIniItem *poTmpIniItem=NULL;
	for (it=m_lstIniItems.begin();it!=m_lstIniItems.end();it++)
	{
		poTmpIniItem=*it;
		p = strstr(poTmpIniItem->m_acItemName, pItem);
		if (p != NULL)
		{
			strcpy(buf[cnt], poTmpIniItem->m_acItemName);
			cnt++;
		}
	}
	return cnt;
}

// 获取KEY里面最大的一个
int CIniSect::GetMaxKey(char *pItem)
{
	int		cnt = 0;
	int		max = 0;
	int		temp = 0;
	char *p;
	list <CIniItem *>::iterator it;
	CIniItem *poTmpIniItem;
	if (m_lstIniItems.size() == 0)
	{
		return 0;
	}
	for (it=m_lstIniItems.begin();it!=m_lstIniItems.end();it++)
	{
		poTmpIniItem=*it;
		// 如果不是和KEY一样，继续查找	
		if (strstr(poTmpIniItem->m_acItemName, pItem) == NULL)
		{
			continue;
		}
		p = strchr(poTmpIniItem->m_acItemName, '_');
		p++;
		if (cnt == 0)
		{
			max = atoi(p);
			cnt = 1;
			continue;
		}
		temp = atoi(p);
		if (temp > max)
		{
			max = temp;
		}			
	}
	return max;
}

CIniItem * CIniSect::GetItem( char *pcItemName )
{
	list <CIniItem *>::iterator it;
	CIniItem *poTmpIniItem;
	CIniItem *poIniItem=NULL;
	for (it=m_lstIniItems.begin();it!=m_lstIniItems.end();it++)
	{
		poTmpIniItem=*it;

		if (0==strcmp(pcItemName,poTmpIniItem->m_acItemName))
		{
			poIniItem=poTmpIniItem;
			break;
		}
	}
	return poIniItem;
}

int CIniSect::PutItem( char *pcItemName,const char *pcItemVal )
{
	int iRet;
	CIniItem *poTmpIniItem;
	list <CIniItem *>::iterator it;
	iRet=1;
	for (it=m_lstIniItems.begin();it!=m_lstIniItems.end();it++)
	{
		poTmpIniItem=*it;
		
		if (0==strcmp(pcItemName,poTmpIniItem->m_acItemName))
		{
			//AfxWriteDebugLog("SuperVPN run at [CIniSect::PutItem] found Name=[%s]", pcItemName);
			iRet=0;
			strcpy(poTmpIniItem->m_acVal,pcItemVal);
			break;
		}
	}
	if (0!=iRet)
	{
		poTmpIniItem=new CIniItem;
		//AfxWriteDebugLog("SuperVPN run at [CIniSect::PutItem] not found Name=[%s]", pcItemName);
		strcpy(poTmpIniItem->m_acItemName,pcItemName);
		strcpy(poTmpIniItem->m_acVal,pcItemVal);
		this->m_lstIniItems.push_back(poTmpIniItem);
	}
	return iRet;
}

/////////////////////////////////////////////////////////////////////////
//							CIniDoc									   //
/////////////////////////////////////////////////////////////////////////
CIniDoc::CIniDoc()
{
}

CIniDoc::~CIniDoc(void)
{
	this->Clear();
}

int CIniDoc::Clear()
{
	list <CIniSect *>::iterator it;
	for (it=this->m_lstIniSects.begin();it!=this->m_lstIniSects.end();it++)
		delete *it;
	m_lstIniSects.clear();
	return 0;
}

CIniSect * CIniDoc::GetSect(char *pcSect)
{
	CIniSect * poTmpIniSect;
	CIniSect * poIniSect=NULL;

	list <CIniSect *>::iterator it;
	for (it=this->m_lstIniSects.begin();it!=this->m_lstIniSects.end();it++)
	{
		poTmpIniSect=*it;
		if (0==strcmp(pcSect,poTmpIniSect->acSectName))
		{
			poIniSect=poTmpIniSect;
			break;
		}
	}

	return poIniSect;
}

// 修改SET的名称
int CIniDoc::RenameSet(char *pOldSet, char *pNewName)
{
	CIniSect *pSet;

	pSet = GetSect(pOldSet);
	if (NULL == pSet)
		return -1;

	strcpy(pSet->acSectName, pNewName);

	return 0;
}

char *CIniDoc::GetItemString(char *pcSect,char *pcItem)
{
	CIniSect * poSect;
	CIniItem * poItem;
	char *pcRet=NULL;
	
	poSect=GetSect(pcSect);
	if (NULL!=poSect)
	{
		poItem=	poSect->GetItem(pcItem);
		if (NULL!=poItem)
			pcRet=poItem->m_acVal;
	}
	if (NULL == pcRet)
	{
		return "";
	}
	return pcRet;
}

// 检测某个选项是否存在
bool CIniDoc::ifExistsItem(char *pcSect,char *pcItem)
{
	CIniSect * poSect;
	CIniItem * poItem;
	char *pcRet=NULL;
	
	poSect = GetSect(pcSect);
	if (NULL == poSect) return false;

	poItem=	poSect->GetItem(pcItem);
	if (NULL == poItem) return false;

	return true;
}


int CIniDoc::DelSect(char *pcSect)
{
	CIniSect * poTmpIniSect;
	
	poTmpIniSect=GetSect(pcSect);
	if (NULL==poTmpIniSect) return 0;

	poTmpIniSect->Clear();
	m_lstIniSects.remove(poTmpIniSect);  
	delete poTmpIniSect;

	return 0;
}

int CIniDoc::DelItem(char *pcSect,char *pcItem)
{
	CIniSect * poTmpIniSect;
	poTmpIniSect=GetSect(pcSect);
	if (NULL==poTmpIniSect) return 1;

	poTmpIniSect->DelItem(pcItem);

	return 0;
}

int CIniDoc::PutSect(char *pcSect)
{
	CIniSect * poTmpIniSect;
	poTmpIniSect=GetSect(pcSect);
	if (NULL==poTmpIniSect)
	{
		poTmpIniSect=new CIniSect;
		strcpy(poTmpIniSect->acSectName,pcSect);
		this->m_lstIniSects.push_back(poTmpIniSect);
	}
	return 0;
}

int CIniDoc::PutItemString(char *pcSect,char *pcItem,const char *pcValue)
{
	CIniSect * poTmpIniSect;
	poTmpIniSect=GetSect(pcSect);
	if (NULL == poTmpIniSect)
	{
		poTmpIniSect=new CIniSect;
		strcpy(poTmpIniSect->acSectName,pcSect);
		this->m_lstIniSects.push_back(poTmpIniSect);

	}
	poTmpIniSect->PutItem(pcItem,pcValue);
	return 0;
}

void CIniDoc::vTrim(char *pcBuf)
{
    vRTrim(pcBuf);
    vLTrim(pcBuf);
}

void CIniDoc::vLTrim(char *pcBuf)
{
	char tmp[MAX_LINE_LEN]={0},*pos;
    int iLen=strlen(pcBuf);
	pos = pcBuf;
    while( iLen>0 )
    {
        if ((' '== *pos)||('\t'== *pos))
        {
            pos++;
            iLen--;
        }
        else
            break;
    }
	strcpy(tmp, pos);
	strcpy(pcBuf, tmp);
}

void CIniDoc::vRTrim(char *pcBuf)
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
	
int CIniDoc::SaveToFile(char *pcFileName)
{
	FILE *pFile;
	
	CIniSect* poSect;
	CIniItem* poItem;
	
	list <CIniSect *>::iterator itSect;
	list <CIniItem *>::iterator itItem;

	char	buf[MAX_NAME_LEN] = {0};

	pFile=::fopen(pcFileName,"wt");

	if (NULL==pFile)
		return -1;
	
	for (itSect=m_lstIniSects.begin();itSect!=m_lstIniSects.end();itSect++)
	{
		poSect=*itSect;
		fputc('[',pFile);
		fputs(poSect->acSectName,pFile);
		fputc(']',pFile);
		fputc('\n',pFile);

		for (itItem=poSect->m_lstIniItems.begin();itItem!=poSect->m_lstIniItems.end();itItem++)
		{
			poItem=*itItem;
			fputc('	',pFile);
			strcpy(buf, poItem->m_acItemName);
			strcat(buf, "	");
			fputs(buf,pFile);
			memset(buf, 0, MAX_NAME_LEN);
			fputc('=',pFile);
			fputc(' ',pFile);
			fputs(poItem->m_acVal,pFile);
			fputc('\n',pFile);			
		}
	}
	::fclose(pFile);
	system("sync");
	return 0;
}

int CIniDoc::GetRelationSectInform(const char  *pSect,char Sect[][SECT_NAME_LEN])
{
	int iCount=0;
	char *p;

	list <CIniSect *>::iterator it;

	for (it=m_lstIniSects.begin();it!=m_lstIniSects.end();it++)
	{
		p=strstr((*it)->acSectName,pSect);   
		if(p!=NULL) 
		{
			strcpy(Sect[iCount],(*it)->acSectName);
			iCount++;
		}   
	}
	return iCount;
}

int CIniDoc::GetItemInfo(char *pSect, char *pItem, char buf[][KEY_NAME_LEN])
{
	int		cnt = 0;
	CIniSect	*pIniSect;		
	pIniSect = GetSect(pSect);
	
	if (NULL == pIniSect) return 0;

	cnt = pIniSect->GetAllItem(pItem, buf);
	return cnt;
}

// 获取SECT下一个KEY的最大项:USER_1 USER_3 USER_4 取4
int CIniDoc::GetMaxKey(char *pSect, char *pItem)
{
	int		cnt = 0;
	CIniSect	*pIniSect;		
	pIniSect = GetSect(pSect);

	if (NULL == pIniSect) return 0;

	cnt = pIniSect->GetMaxKey(pItem);
	return cnt;
}
// 获取一个SETT的最大项:GOURP_1 
int CIniDoc::GetMaxSect(char *pSect)
{
	int		cnt = 0;
	int		max =0;
	int		temp;
	char	*p;
	list <CIniSect *>::iterator it;
	CIniSect *poTmpIniSect;
	if (m_lstIniSects.size() == 0)
	{
		return 0;
	}
	for (it=m_lstIniSects.begin();it!=m_lstIniSects.end();it++)
	{
		poTmpIniSect=*it;
		// 如果不是和pSect一样，继续查找
		if (strstr(poTmpIniSect->acSectName, pSect) == NULL)
		{
			continue;
		}
		p = strchr(poTmpIniSect->acSectName, '_');
		p++;
		if (cnt == 0)
		{
			max = atoi(p);
			cnt = 1;
			continue;
		}
		temp = atoi(p);
		if (temp > max)
		{
			max = temp;
		}			
	}
	return max;
}
// 获取RuleList项的最大项：1_RuleList_2
int  CIniDoc::GetMaxRuleList(char *pSect)
{
	int		cnt = 0;
	int		max = 0;
	int		temp;
	char	*p;
	char	*value;
	char	buf[50] = {0};
	list <CIniSect *>::iterator it;
	CIniSect *poTmpIniSect;
	if (m_lstIniSects.size() == 0)
	{
		return 0;
	}
	for (it=m_lstIniSects.begin();it!=m_lstIniSects.end();it++)
	{
		poTmpIniSect=*it;
		// 如果不是和pSect一样，继续查找
		if (strstr(poTmpIniSect->acSectName, pSect) == NULL)
		{
			continue;
		}
		strcpy(buf, poTmpIniSect->acSectName);
		p = strchr(buf, '_');
		p++;
		value = strchr(p, '_');
		value++;
		if (cnt == 0)
		{
			max = atoi(value);
			cnt = 1;
			continue;
		}
		temp = atoi(value);
		if (temp > max)
		{
			max = temp;
		}			
	}
	return max;
}
// 获取KEY的ID值
int CIniDoc::GetItemID(char *pItem)
{
	int cnt = 0;
	char *value;
	value = strchr(pItem, '_');
	value++;
	cnt = atoi(value);
	return cnt;
}
// 获取SECT的ID
int CIniDoc::GetSectID(char *pSect)
{
	int cnt = 0;
	char *value;
	value = strchr(pSect, '_');
	value++;
	cnt = atoi(value);
	return cnt;
}
/*	
	函数说明：移动一个SECT到某项之前或之后 
	函数参数：要移动项:pMoveSect 移动到：pSect  Flag=0->之前 Flag=1->之后
	函数返回：移动成功返回 1，失败返回0
*/
int CIniDoc::Move(char *pMoveSect, char *pSect, int Flag)
{
	if (strcmp(pMoveSect, pSect) == 0)
	{
		return 1;
	}
	list <CIniSect *>::iterator it;
	list <CIniSect *>::iterator move;
	CIniSect *poTmpIniSect;
	CIniSect *pMove;
	CIniSect *buf;
	if (m_lstIniSects.size() == 0)
	{
		return 0;
	}
	for (it=m_lstIniSects.begin();it!=m_lstIniSects.end();it++)
	{
		poTmpIniSect=*it;
		// 找到参照目标
		if (strcmp(poTmpIniSect->acSectName, pSect) == 0)
		{
			// 移动到pSect之前
			if (Flag == 0)
			{
				for (move = m_lstIniSects.begin(); move != m_lstIniSects.end(); move++)
				{
					pMove = *move;
					// 找到要移动的项
					if (strcmp(pMove->acSectName, pMoveSect) == 0)
					{
						m_lstIniSects.insert(it, pMove);
						m_lstIniSects.erase(move);
						break;
					}		
				}
			}	
			else
			{
				for (move = m_lstIniSects.begin(); move != m_lstIniSects.end(); move++)
				{
					pMove = *move;
					// 找到要移动的项
					if (strcmp(pMove->acSectName, pMoveSect) == 0)
					{
						it++;
						// 已经到了列表的最后,直接添加
						if (it == m_lstIniSects.end())
						{
							m_lstIniSects.push_back(pMove);
							m_lstIniSects.erase(move);
							break;
						}
						buf = *it;
						if (strcmp(buf->acSectName, pMove->acSectName) == 0)
						{
							break;
						}
						m_lstIniSects.insert(it, pMove);
						m_lstIniSects.erase(move);
						break;
					}		
				}
			}
		}
	}		
	return 1;
}


int  CIniDoc::LoadFromFile(char *pcFileName)
{
	FILE *pFile;
	char *pcPosEnd;
	int iLen;
	bool bFirst=true;
	char acBuf[MAX_LINE_LEN];
	char acSect[MAX_NAME_LEN];
	char acItemName[MAX_NAME_LEN];
	char acItemVal[MAX_VAL_LEN];
	
	Clear();
	pFile=::fopen(pcFileName,"rt");
	if (NULL==pFile)
	{
		pFile=::fopen(pcFileName,"w+");
		if(pFile==NULL) return -1;
		return 0;
	}

	while (NULL!=fgets(acBuf,MAX_LINE_LEN,pFile))
	{		
		pcPosEnd=::strstr(acBuf,"##");//chr(acBuf,'&');
		if (NULL!=pcPosEnd)
			*pcPosEnd=0;
		vTrim(acBuf);
		if (strlen(acBuf)==0)
		{
			continue;
		}
		if ('#' == acBuf[0])
		{
			continue;
		}

		//AfxWriteDebugLog("SuperVPN run at [CIniSect::LoadFromFile] content=[%s]======", acBuf);
		
		if (bFirst)
		{
			//先sect,否则文件错误
			pcPosEnd=strchr(acBuf,']');		
			if (('['==acBuf[0])&&(NULL!=pcPosEnd))
			{
				iLen=pcPosEnd-acBuf-1;
				strncpy(acSect,acBuf+1,iLen);
				acSect[iLen]=0;
				
				//AfxWriteDebugLog("SuperVPN run at [CIniSect::LoadFromFile] sect=[%s]", acSect);

				PutSect(acSect);
			}
			else //error
			{
				fclose(pFile);
				return -1;
			}
			bFirst=false;
		}
		else
		{
			pcPosEnd=strchr(acBuf,']');		
			if (('['==acBuf[0])&&(NULL!=pcPosEnd))//sect
			{
				iLen=pcPosEnd-acBuf-1;
				strncpy(acSect,acBuf+1,iLen);
				acSect[iLen]=0;
				//AfxWriteDebugLog("SuperVPN run at [CIniSect::LoadFromFile] sect=[%s]", acSect);

				PutSect(acSect);
			}
			else //item
			{				
				//AfxWriteDebugLog("SuperVPN run at [CIniSect::LoadFromFile] item content=[%s]", acBuf);
				pcPosEnd=strchr(acBuf,'=');	
				if (NULL!=pcPosEnd)
				{
					//itemname
					iLen=pcPosEnd-acBuf;
					strncpy(acItemName,acBuf,iLen);
					acItemName[iLen]=0;
					vRTrim(acItemName);
					//itemval
					strcpy(acItemVal,pcPosEnd+1);
					vLTrim(acItemVal);

					//AfxWriteDebugLog("SuperVPN run at [CIniSect::LoadFromFile] item Name=[%s] Value=[%s]", acItemName, acItemVal);
					
					PutItemString(acSect,acItemName,acItemVal);
				}
				/*else //err
				{
					fclose(pFile);
					return -2;
				}*/
			}
		}
	}
	fclose(pFile);
	return 0;
}

