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
#include "MyDB.hpp"

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
bool CIdentifySet::DealRcvPkt(CHelloPkt *pkt)
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
函数说明:从网关配置数据库中读取MAC与身份的识别码
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CIdentifySet::ReadMacIdentifyFromGW(list<SBindInform> &ltSI)
{
	MyDB myDB;
	return myDB.GetIndetifyMac(ltSI);
}

/*********************************************************
函数说明:从网关配置数据库中读取MAC与身份的识别码并处理业务
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CIdentifySet::InitIdentifyFromGW()
{
	list<SBindInform> ltSI;

	//=============================================================================
	//1、获取下游设备MAC与身份ID对应的关系(从网关数据库获取),并进行数据的初始化
	//读出Mac与身份ID
	//=============================================================================
	ReadMacIdentifyFromGW(ltSI);

	//=============================================================================
	//2、根据获取的MAC与身份ID，向中心请求身份ID与出口的对应关系(向中心服务器获取)
	//=============================================================================
	CNodeBase *pBase = AfxGetVPNNode();
	if(pBase == NULL) return false;
	
	CNodeUser *pNode= dynamic_cast<CNodeUser *>(pBase);
	if(pNode == NULL) return false;
	pNode->BindIdentifyService(ltSI);

	return true;
}

/*********************************************************
函数说明:从ARP表中读取MAC与IP对应关系
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool GetSubString(string sou, string bFlag, string eFlag, string &out)
{
	int bPos = sou.find(bFlag);
	if (bPos == string::npos) return false;

	string tmp = sou.substr(bPos+bFlag.length(), sou.length()-bPos-bFlag.length());
	int ePos = tmp.find(eFlag);
	if (ePos == string::npos) return false;

	out = tmp.substr(0, ePos);

	return true;
}

bool CIdentifySet::ReadARP(list<SBindInform> &ltBSer)
{
	//bogon (192.168.204.254) at 00:50:56:f0:93:b4 [ether] on eth0
	ndString ip, mac, arptable;

	if (!AfxRunCmdGetResult("arp -a", arptable))
	{
		return false;
	}
	
	//GetSubString();
	return false;
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
				//移除旧的
				pNode->RemoveEdgeAndRoute(*iter);
				//增加新的
				pNode->BindIdentifyService(sBI);
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
		CHelloPkt hellPkt;
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


