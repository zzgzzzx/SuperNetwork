/*********************************************************
模块名       : 
文件名       :ARPSet.cpp
相关文件     :Set.hpp
文件实现功能 :ARP集合类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "ARPSet.hpp"
#include "NDFunc.hpp"
#include "MyDB.hpp"
#include "UserVPNApp.hpp"

extern CSuperVPNApp *gPSuperVPNApp;

/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CARPSet::CARPSet()
{
	mBArpCheckRun = ND_FALSE;
}

/*********************************************************
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CARPSet::~CARPSet()
{

}

/*********************************************************
函数说明：获取当前arp列表信息
入参说明：
出参说明：
返回值  ：
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
函数说明：更新标识重置为false
入参说明：
出参说明：
返回值  ：
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
函数说明:检测ARP
入参说明：
出参说明：
返回值  ：
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

	//是否还在ARP列表里的标识全部置为false
	RestUpdateFalse();

	//上线过程的处理
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
				//通知绑定出口处理，如果成功，上线标识置为true
				CUserVPNApp *pSuperVPNApp = dynamic_cast<CUserVPNApp*> (gPSuperVPNApp);
				if (pSuperVPNApp->GetIdentifySet()->BindIdentifyService(*pSBI) == ND_SUCCESS)
				{
					pSBI->ifOnline = ND_TRUE;
				}			
			}else if(pSBI->sDeviceIP != sARP.ip)
			{
				//ip地址有变化的处理过程
			}
		}
		iter++;
	}	

	//已不在ARP表里的，进行下线处理
	bool bResult = GetBegin(pSBI);
	while(bResult)
	{
		if(pSBI->ifUpdate == ND_FALSE)
		{
			//通知绑定出口处理，如果成功，上线标识置为true
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
函数说明:从网关配置数据库中读取MAC与身份的识别码并处理业务
入参说明：
出参说明：
返回值  ：
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
函数说明：增加节点
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CARPSet::AddItem(ndString mac, SBindInform *pBI)
{
	return insert(mac, pBI);
}

/*********************************************************
函数说明：删除节点
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CARPSet::DelItem(ndString mac)
{
	return nlerase(mac);
}

/*********************************************************
函数说明：查找节点(不加锁)
入参说明：
出参说明：
返回值  ：
*********************************************************/
SBindInform *CARPSet::FindItemNL(ndString mac)
{
	SBindInform *pBI;
	if (!nlfind(mac, pBI)) return NULL;

	return pBI;

}

/*********************************************************
函数说明：查找节点(加锁)
入参说明：
出参说明：
返回值  ：
*********************************************************/
SBindInform *CARPSet::FindItem(ndString mac)
{
	SBindInform *pBI;
	if (!find(mac, pBI)) return NULL;

	return pBI;
}


