/*********************************************************
ģ����       : 
�ļ���       :ARPSet.cpp
����ļ�     :Set.hpp
�ļ�ʵ�ֹ��� :ARP������
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "ARPSet.hpp"
#include "NDFunc.hpp"
#include "MyDB.hpp"
#include "UserVPNApp.hpp"

extern CSuperVPNApp *gPSuperVPNApp;

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CARPSet::CARPSet()
{
	mBArpCheckRun = ND_FALSE;
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CARPSet::~CARPSet()
{

}

/*********************************************************
����˵������ȡ��ǰarp�б���Ϣ
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CARPSet::GetCurrentARP(list<SArp> &liarp)
{
	FILE *fARP;
	char buf[1024];
	SArp sARP;
	char ip[16];  
	char mac[18]; 	

	if ((fARP = fopen(PATH_PROCNET_ARP, "r")) == NULL) return 0;

	fgets(buf, sizeof buf, fARP);	/* eat line */

	while (!feof(fARP) && (fscanf(fARP, " %15[0-9.] %*s %*s %17[A-Fa-f0-9:] %*s %*s", ip, mac) == 2)) 
	{  
		sARP.ip = ip;
		sARP.mac = mac;
		//AfxWriteDebugLog("SuperVPN run at [CARPSet::GetCurrentARP] mac=[%s] ip=[%s]", mac, ip);
		liarp.push_back(sARP);
	} 
	fclose(fARP);

	return true;
}

/*********************************************************
����˵�������±�ʶ����Ϊfalse
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CARPSet::RestUpdateFalse()
{
	SBindInform *pSBI;
	bool bResult = GetBegin(pSBI);
	while(bResult)
	{
		pSBI->ifUpdate = ND_FALSE;
		bResult = GetNext(pSBI);
	}
}


/*********************************************************
����˵��:���ARP
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CARPSet::ARPCheck()
{
	list<SArp> liarp;
	SArp sARP;
	SBindInform *pSBI;

	if (!GetCurrentARP(liarp))
	{
		AfxWriteDebugLog("SuperVPN run at [CARPSet::ARPCheck] GetCurrentARP ERROR");
		return false;
	}

	if(mBArpCheckRun == ND_TRUE)
	{
		AfxWriteDebugLog("SuperVPN run at [CARPSet::ARPCheck] Already Running...");
		return false;
	}

	mBArpCheckRun = ND_TRUE;

	//�Ƿ���ARP�б���ı�ʶȫ����Ϊfalse
	RestUpdateFalse();

	//���߹��̵Ĵ���
	list<SArp>::iterator iter = liarp.begin();
	while(iter != liarp.end())
	{
	    sARP = *iter;
		pSBI = FindItem(sARP.mac);
		if(pSBI != NULL)
		{
			pSBI->ifUpdate = ND_TRUE;
			if(pSBI->ifOnline == ND_FALSE)
			{
				//֪ͨ�󶨳��ڴ�������ɹ������߱�ʶ��Ϊtrue
				CUserVPNApp *pSuperVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp);
				if (pSuperVPNApp->GetIdentifySet()->BindIdentifyService(*pSBI) == ND_SUCCESS)
				{
					pSBI->ifOnline = ND_TRUE;
				}			
			}else if(pSBI->sDeviceIP != sARP.ip)
			{
				//ip��ַ�б仯�Ĵ������
			}
		}
		iter++;
	}	

	//�Ѳ���ARP����ģ��������ߴ���
	bool bResult = GetBegin(pSBI);
	while(bResult)
	{
		if(pSBI->ifUpdate == ND_FALSE)
		{
			//֪ͨ�󶨳��ڴ�������ɹ������߱�ʶ��Ϊtrue
			CUserVPNApp *pSuperVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp);
			if (pSuperVPNApp->GetIdentifySet()->UnBindIdentifyService(*pSBI) == ND_SUCCESS)
			{
				pSBI->ifOnline = ND_FALSE;
			}			
		}
		bResult = GetNext(pSBI);
	}

	mBArpCheckRun = ND_FALSE;
	
	return true;
}

/*********************************************************
����˵��:�������������ݿ��ж�ȡMAC����ݵ�ʶ���벢����ҵ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CARPSet::InitIdentifyFromGW()
{

	MyDB myDB;
	list<SBindInform> ltSI;

	if (!myDB.initDB())
	{
		AfxWriteDebugLog("SuperVPN run at [CARPSet::InitIdentifyFromGW] InitDB Error");
		return false;
	}
	
	bool ret = myDB.GetIndetifyMac(ltSI);
	if(!ret) return ret;

	SBindInform sBI, *pBI;

	BindInformItr iter = ltSI.begin();
	while(iter != ltSI.end())
	{
	    sBI = *iter;
		pBI = new SBindInform(); *pBI = sBI;
		AddItem(sBI.sDeviceMac, pBI);
		iter++;
	}	

	return true;
}


/*********************************************************
����˵�������ӽڵ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CARPSet::AddItem(ndString mac, SBindInform *pBI)
{
	return insert(mac, pBI);
}

/*********************************************************
����˵����ɾ���ڵ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CARPSet::DelItem(ndString mac)
{
	return nlerase(mac);
}

/*********************************************************
����˵�������ҽڵ�(������)
���˵����
����˵����
����ֵ  ��
*********************************************************/
SBindInform *CARPSet::FindItemNL(ndString mac)
{
	SBindInform *pBI;
	if (!nlfind(mac, pBI)) return NULL;

	return pBI;

}

/*********************************************************
����˵�������ҽڵ�(����)
���˵����
����˵����
����ֵ  ��
*********************************************************/
SBindInform *CARPSet::FindItem(ndString mac)
{
	SBindInform *pBI;
	if (!find(mac, pBI)) return NULL;

	return pBI;
}


