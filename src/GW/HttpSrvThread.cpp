/*********************************************************
模块名       : 
文件名       :HttpSrvThread.cpp
相关文件     :HttpSrvThread.hpp
文件实现功能 :Web服务包处理实现文件
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "HttpSrvThread.hpp"
#include "NDFunc.hpp"
#include "microhttpd.h"
#include "NodeUser.hpp"

/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CHttpSrvThread::CHttpSrvThread()
{
	mStopRun = false;
}

/*********************************************************
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CHttpSrvThread::~CHttpSrvThread()
{
}

/*********************************************************
函数说明：发送数据包
入参说明：
出参说明：
返回值  ：
*********************************************************/
int CHttpSrvThread::SendTo(ndUInt32 IP,ndInt16 iPort,char *pBuf,int iBufLen)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Send Pkt To Hello Server SrvIP=[%s] port=[%d]!",AfxHostIPToStr(IP),iPort);
	return 0;
}

/*********************************************************
函数说明：线程处理函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CHttpSrvThread::ProcessVirtual(void)
{
	if (!StartService())
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Hello Rcv Server Start Error!!!\n");
		return;
	}
}

/*********************************************************
函数说明：http服务接收数据处理
入参说明：
出参说明：
返回值  ：
*********************************************************/
int  connectionHandler(
    void *cls,
    struct MHD_Connection *connection,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls)
{
/*    const char* pageBufferOK = "<html><body>Hello, I'm lgxZJ!</body></html>";
	const char* pageBufferERR = "<html><body>Hello, I'm lgxZJ!</body></html>";

	//接收HTTP数据包
	struct sockaddr *clientaddr = (struct sockaddr *)MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
	const char *value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "value");
	
	struct sockaddr_in *addr_in;
	addr_in= (struct sockaddr_in *)clientaddr;

	SBindInform sBindInform;
	//need complet by lewis
	//sBindInform.sDeviceIP = AfxNetIPToStr(addr_in->sin_addr);
	AfxWriteDebugLog( "MHD Request Client address=[%s]", sBindInform.sDeviceIP.c_str());
	AfxWriteDebugLog( "MHD Request URL=[%s]", value);
	AfxWriteDebugLog( "MHD Request sDeviceFlag=[%s]", sBindInform.sDeviceFlag.c_str());

	//===================================================================================================================
	// 1、下端用户标识请求数据包的处理(包括上线、下线通知)
	//===================================================================================================================	
	CNodeUser *pNode= dynamic_cast<CNodeUser *>(AfxGetVPNNode());
	pNode->BindIdentifyService(sBindInform);


	//===================================================================================================================
	// 2、中心服务业务数据包的处理(服务节点重启通知)
	//===================================================================================================================

	
	//回应数据包
    struct MHD_Response *response;
    response = MHD_create_response_from_buffer(strlen(pageBufferOK),
        (void*)pageBufferOK, MHD_RESPMEM_PERSISTENT);

    if (MHD_add_response_header(response, "Content-Type", "text/html") == MHD_NO) {
        AfxWriteDebugLog( "MHD_add_response_header error");
        return MHD_NO;
    }
    if (MHD_queue_response(connection, MHD_HTTP_OK, response) == MHD_NO) {
        AfxWriteDebugLog("MHD_queue_response error");
        return MHD_NO;
    }
    MHD_destroy_response(response);

    return MHD_YES;
    */
    return 0;
}


/*********************************************************
函数说明：启动服务socket
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CHttpSrvThread::StartService()
{
    const int port = 8888;
/*
    struct MHD_Daemon* daemon = 
        MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port
        , NULL, NULL, connectionHandler, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        AfxWriteDebugLog("Cannot start httpd server!");
        return false;
    }

	//循环等待保证不退出应用
	while(!mStopRun)
		sleep(60);

    MHD_stop_daemon(daemon);
    */
    
    return true;

}

