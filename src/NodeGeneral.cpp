/*********************************************************
模块名       : 与服务器交互基类(Certificate\DP)
文件名       : ClientSrvBase.cpp
相关文件     : ClientSrvBase.h
文件实现功能 : 实现与服务器交互的功能
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeGeneral.hpp"
#include "HttpGeneral.hpp"
#include "NDFunc.hpp"


/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeGeneral::CNodeGeneral()
{
	mPHttpClient = NULL;
	AfxGetEthMac("eth0", mSNodeInform.sNodeMac);	
	//mSNodeInform.sNodeMac = "aa:bb:cc:dd:ee:ff";
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeGeneral::~CNodeGeneral()
{
	if(mPHttpClient != NULL) delete mPHttpClient;
}

/*********************************************************
函数说明：完成的事务后续处理内容
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeGeneral::InitData()
{
    return ND_SUCCESS;
}

/*********************************************************
函数说明：线程处理函数
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
void *CNodeGeneral::ThreadFunction(void *cmd)
{
    pthread_detach(pthread_self());

	char *ExecCMD = (char*)(cmd);
	AfxExecCmd(ExecCMD);
	free(ExecCMD);

    AfxWriteDebugLog("SuperVPN run at [%s] Thread End", __FUNCTION__);
    pthread_exit(0);
}

/*********************************************************
函数说明：节点初始化
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeGeneral::NodeInit()
{
	ndStatus ret = mPHttpClient->NodeInit();
	AfxWriteDebugLog("SuperVPN run at [CNodeGeneral::NodeEnvSet] Node init return=[%d]", ret);
	return ret;
}


/*********************************************************
函数说明：节点环境配置
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeGeneral::NodeEnvSet()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：清除所有的EDGE进程
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
void CNodeGeneral::CleanAllEdge()
{
	char cmd[512]={0};
	sprintf(cmd, "killall %s", EDGE_EXE_FILE_NAME);
	AfxExecCmd(cmd);

	strcpy(cmd, "killall edge");
	AfxExecCmd(cmd);	
}

/*********************************************************
函数说明：GetServerList
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndBool CNodeGeneral::GetServerList(list<SServerInfo> &mServers)
{
	return mPHttpClient->GetServerList(mServers);
}


