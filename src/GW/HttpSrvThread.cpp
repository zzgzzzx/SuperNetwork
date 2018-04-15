/*********************************************************
ģ����       : 
�ļ���       :HttpSrvThread.cpp
����ļ�     :HttpSrvThread.hpp
�ļ�ʵ�ֹ��� :Web���������ʵ���ļ�
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "HttpSrvThread.hpp"
#include "NDFunc.hpp"
#include "microhttpd.h"
#include "NodeUser.hpp"

/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHttpSrvThread::CHttpSrvThread()
{
	mStopRun = false;
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CHttpSrvThread::~CHttpSrvThread()
{
}

/*********************************************************
����˵�����������ݰ�
���˵����
����˵����
����ֵ  ��
*********************************************************/
int CHttpSrvThread::SendTo(ndUInt32 IP,ndInt16 iPort,char *pBuf,int iBufLen)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Send Pkt To Hello Server SrvIP=[%s] port=[%d]!",AfxHostIPToStr(IP),iPort);
	return 0;
}

/*********************************************************
����˵�����̴߳�����
���˵����
����˵����
����ֵ  ��
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
����˵����http����������ݴ���
���˵����
����˵����
����ֵ  ��
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

	//����HTTP���ݰ�
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
	// 1���¶��û���ʶ�������ݰ��Ĵ���(�������ߡ�����֪ͨ)
	//===================================================================================================================	
	CNodeUser *pNode= dynamic_cast<CNodeUser *>(AfxGetVPNNode());
	pNode->BindIdentifyService(sBindInform);


	//===================================================================================================================
	// 2�����ķ���ҵ�����ݰ��Ĵ���(����ڵ�����֪ͨ)
	//===================================================================================================================

	
	//��Ӧ���ݰ�
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
����˵������������socket
���˵����
����˵����
����ֵ  ��
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

	//ѭ���ȴ���֤���˳�Ӧ��
	while(!mStopRun)
		sleep(60);

    MHD_stop_daemon(daemon);
    */
    
    return true;

}

