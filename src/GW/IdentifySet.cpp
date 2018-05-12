/*********************************************************
模块名       : 
文件名       :IdentifySet.cpp
相关文件     :Set.hpp
文件实现功能 :用户标识集合类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "IdentifySet.hpp"
#include "NDFunc.hpp"
#include "NodeUser.hpp"

/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CIdentifySet::CIdentifySet()
{

}

/*********************************************************
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CIdentifySet::~CIdentifySet()
{

}

/*********************************************************
函数说明：接收处理数据包(只有网关才会触发)
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CIdentifySet::DealRcvPkt(CUserHelloPkt *pkt)
{
	//接收到服务节点的数据包，针对服务节点的IP进行查找
	ServiceInform *pSI;
	if (mServiceMap.find(AfxHostIPToStr(pkt->GetRcvNetParam().lSrcIP), pSI))
	{
		pSI->iHelloNum = MAX_VALUE_HELLO_CHECK_TIMES;
		return true;
	}
	return false;
}

/*********************************************************
函数说明:向中心服务器请求下游设备身份对应的出口信息(上线)
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndStatus CIdentifySet::BindIdentifyService(SBindInform sBindInform)
{
	CNodeBase *pBase = AfxGetVPNNode();
	if(pBase == NULL) return ND_ERROR_INVALID_PARAM;
	
	CNodeUser *pNode= dynamic_cast<CNodeUser *>(pBase);
	if(pNode == NULL) return ND_ERROR_INVALID_PARAM;

	ndStatus ret = pNode->BindIdentifyService(sBindInform);
	if(ret == ND_SUCCESS)
	{
		SBindInform *pBI = new SBindInform();
		*pBI = sBindInform;
		AddItem(sBindInform.sDeviceFlag, pBI);
	}

	return ret;	
}

/*********************************************************
函数说明:向中心服务器通知下游设备身份对应的出口信息释放(下线)
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndStatus CIdentifySet::UnBindIdentifyService(SBindInform sBindInform)
{
	CNodeBase *pBase = AfxGetVPNNode();
	if(pBase == NULL) return ND_ERROR_INVALID_PARAM;
	
	CNodeUser *pNode= dynamic_cast<CNodeUser *>(pBase);
	if(pNode == NULL) return ND_ERROR_INVALID_PARAM;

	ndStatus ret = pNode->UnBindIdentifyService(sBindInform);
	if(ret == ND_SUCCESS)
	{
		DelItem(sBindInform.sDeviceFlag);
	}

	return ret;		
}

/*********************************************************
函数说明：发送Hello包并检测服务是否掉线
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CIdentifySet::SendHelloAndCheck()
{
	ServiceInform *pService;
	SBindInform sBI;
	ndStatus ret;

	CNodeBase *pBase = AfxGetVPNNode();
	if(pBase == NULL) return;	
	CNodeUser *pNode= dynamic_cast<CNodeUser *>(pBase);
	if(pNode == NULL) return;
	
	bool have = mServiceMap.GetBegin(pService);
	while(have)
	{
		//服务器已失联，通知服务器更换
		if(pService->iHelloNum <= 0)
		{	
			//所有服务相关联的设备通道与路由进行释放
			BindInformItr iter = pService->ltBindInform.begin();
			while(iter != pService->ltBindInform.end())
			{
				//通知服务器变更出口信息，如果失败
				sBI = *iter;
				ret = pNode->ServiceErrorNotify(sBI);
				if (ret != ND_SUCCESS)
				{
					AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::SendHelloAndCheck] ServiceErrorNotify Err=[%d]", ret);
					iter++; 
					continue;
				}
				BindInformItr iterBak = iter;
				iterBak++;
				pService->ltBindInform.erase(iter);
				iter = iterBak;
			}
			
			//判断是否下游设备都更新出口服务完成，如果全部完成，进行移除出口服务
			if(pService->ltBindInform.size() <= 0)
			{
				mServiceMap.EraseCurMNext();
				have = mServiceMap.GetCurrent(pService);
				continue;
			}
		}

		//发送Hello
		CUserHelloPkt hellPkt;
		SNetParam sNP;
		sNP.lDesIP = inet_addr(pService->sServiceIP.c_str());
		sNP.uDesPort = HELLO_SRV_LOCAL_PORT;
		hellPkt.SetSndNetParam(sNP);
		hellPkt.SendHelloPkt();

		//Hello剩余次数--
		pService->iHelloNum--;
		
		have = mServiceMap.GetNext(pService);
	}
}

/*********************************************************
函数说明：增加节点
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CIdentifySet::AddItem(ndString Key, SBindInform *pBI)
{
	mMacMap.insert(pBI->sDeviceMac, pBI);

	ServiceInform *pSI;
	bool bFind = mServiceMap.find(pBI->sServiceIP, pSI);
	if(!bFind)
	{
		pSI = new ServiceInform();
		pSI->iHelloNum = MAX_VALUE_HELLO_CHECK_TIMES;
		pSI->ltBindInform.push_back(*pBI);
		pSI->sServiceIP = pBI->sServiceIP;
		mServiceMap.insert(pBI->sServiceIP, pSI);	
	}
	else
	{
		pSI->ltBindInform.push_back(*pBI);
	}
	
	return insert(Key,pBI);
}

/*********************************************************
函数说明：删除节点
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CIdentifySet::DelItem(ndString Key)
{
	SBindInform *pBI, sBI;
	if (!nlfind(Key, pBI)) return false;

	mMacMap.erase(pBI->sDeviceMac);

	ServiceInform *pSI;
	bool bFind = mServiceMap.find(pBI->sServiceIP, pSI);
	if(bFind)
	{
		BindInformItr iter = pSI->ltBindInform.begin();
		while(iter != pSI->ltBindInform.end())
		{
			sBI = *iter;
			if(sBI.sDeviceFlag == pBI->sDeviceFlag)
			{
				pSI->ltBindInform.erase(iter);
				break;
			}
			iter++;
		}
		if(pSI->ltBindInform.empty())
			mServiceMap.erase(pBI->sServiceIP);
	}	
	delete pBI;
	
	return nlerase(Key);
}

/*********************************************************
函数说明：查找节点(不加锁)
入参说明：
出参说明：
返回值  ：
*********************************************************/
SBindInform *CIdentifySet::FindItemNL(ndString Key)
{
	SBindInform *pBI;

	if (nlfind(Key,pBI)) return pBI;

	return NULL;
}

/*********************************************************
函数说明：查找节点(加锁)
入参说明：
出参说明：
返回值  ：
*********************************************************/
SBindInform *CIdentifySet::FindItem(ndString Key)
{
	SBindInform *pBI;

	if (find(Key,pBI)) return pBI;

	return NULL;
}

/*********************************************************
函数说明：根据mac查找结点
入参说明：
出参说明：
返回值  ：
*********************************************************/
SBindInform *CIdentifySet::FindItemByMac(ndString mac)
{
	SBindInform *pBI;
	if (mMacMap.find(mac, pBI)) return pBI;

	return NULL;
}


