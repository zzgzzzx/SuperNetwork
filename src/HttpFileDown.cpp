/*********************************************************
模块名       : 文件下载类封装
文件名       : HttpFileDown.cpp
相关文件     : HttpFileDown.h
文件实现功能 : 文件下载类的功能实现
作者         :lewis
创建时间     :2017-03
**********************************************************/
#include "HttpFileDown.hpp"
#include "NDFunc.hpp"

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CHttpFileDown::CHttpFileDown(void)
{
    mIFAbort = false;
    mDownRsp.sUrl = "";
    mFile = NULL;
    mDownRsp.bIsDownloadComplete = ND_FALSE;
    mDownRsp.uiChunkOffset = 0;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CHttpFileDown::~CHttpFileDown(void) {

}

/*********************************************************
函数说明：读取下载请求数据
入参说明：无
出参说明：无
返回值  ：下载请求数据
*********************************************************/
SDownloadFileReqSt CHttpFileDown::GetDownFileReq()
{
    return mDownReq;
}

/*********************************************************
函数说明：获取下载应答数据
入参说明：无
出参说明：无
返回值  ：下载应答数据
*********************************************************/
SDownloadFileRspSt CHttpFileDown::GetDownFileRsp()
{
    return mDownRsp;
}

/*********************************************************
函数说明：停止下载
入参说明：无
出参说明：无
返回值  ：true-中断下载成功
          false-中断下载失败
*********************************************************/
void CHttpFileDown::AbortDownload()
{
    mIFAbort = true;
}

/*********************************************************
函数说明：获取是否中断下载
入参说明：无
出参说明：无
返回值  ：true-已中断下载
          false-未中断下载
*********************************************************/
bool CHttpFileDown::IFAbortDownload()
{
    return mIFAbort;
}

/*********************************************************
函数说明：设置文件下载的应答信息
入参说明：uiChunkOffset-下载偏移量
出参说明：无
返回值  ：无
*********************************************************/
ndBool CHttpFileDown::SetDownFileRsp(ndUInt32 uiChunkOffset)
{
    mDownRsp.uiChunkOffset += uiChunkOffset;

    if (mDownReq.iSize <=  mDownRsp.uiChunkOffset){
        mDownRsp.bIsDownloadComplete = ND_TRUE;
        //AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::SetDownFileRsp] Complete...\n");
    }
    else{
        mDownRsp.bIsDownloadComplete = ND_FALSE;
        //AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::SetDownFileRsp] Not Complete...\n");
    }

    return mDownRsp.bIsDownloadComplete;
}

/*********************************************************
函数说明：设置文件下载请求信息
入参说明：req-下载请求数据
出参说明：无
返回值  ：true-设置成功
          false-设置失败
*********************************************************/
bool CHttpFileDown::SetDownFileReq(SDownloadFileReqSt req)
{
    mDownReq = req;
    mFile = req.fFile;

    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] FilePath=[%s]\n", mDownReq.sLocalFile.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] offset=[%lu]\n", mDownReq.iOffset);

    //以传文件路径的优先级高
    if(!mDownReq.sLocalFile.empty())
    {
        //如果文件存在并且下载的偏移量等于文件大小，表示断点续传
        if(AfxFileSize(mDownReq.sLocalFile.c_str()) == mDownReq.iOffset){
            AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Create File a+\n");
            mFile = fopen(mDownReq.sLocalFile.c_str(), "a+");
        }else{
            AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Create File wb\n");
            mFile = fopen(mDownReq.sLocalFile.c_str(), "wb");
        }
    }

    if(NULL == mFile)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] File Is NULL ERROR\n");
        return false;
    }

    if(mDownReq.iOffset < 0){
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Offset =[%lu] Err\n", mDownReq.iOffset);
        return false;
    }
    mDownRsp.uiChunkOffset = mDownReq.iOffset;

    return true;
}

/*********************************************************
函数说明：curl库接收数据处理函数
入参说明：prt-接收到的数据内容指针
          size-接收数据每块的长度
          nmemb-总共接收的数据块
          stream-函数参数传递指针
出参说明：无
返回值  ：成功处理的的内容长度，如果返回的长度跟传入的总长
          度不一致，下载将会中断退出
*********************************************************/
size_t OnWriteData_GetFile(void *ptr, size_t size, size_t nmemb, void *stream) {
    CHttpFileDown *FileDown = reinterpret_cast<CHttpFileDown *>(stream);
    //如果中断下载，则退出
    if(FileDown->IFAbortDownload())
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::OnWriteData_GetFile] Write File Abort...\n");
        return 0;
    }

    size_t written = fwrite(ptr, size, nmemb, FileDown->GetDownLoadFile());
    if(written <= 0){
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::OnWriteData_GetFile] Write File Error\n");
        return 0;
    }

    //AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::OnWriteData_GetFile] Write File Written Len=[%d]\n", written);
    //AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::OnWriteData_GetFile] Write File iOffset=[%d]\n", FileDown->GetDownFileReq().iOffset);

    //增加判断是不是已下载完成，如果下载完成了就关闭文件
    if (FileDown->SetDownFileRsp(written) == ND_TRUE){
        if(!FileDown->GetDownFileReq().sLocalFile.empty())
            FileDown->FileClose();
    }

    //AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::OnWriteData_GetFile] Write File...\n");
    return written;
}

/*********************************************************
函数说明：获取下载文件的大小
入参说明：prt-接收到的数据内容指针
          size-接收数据每块的长度
          nmemb-总共接收的数据块
          data-函数参数传递指针
出参说明：无
返回值  ：成功处理的的内容长度，如果返回的长度跟传入的总长
          度不一致，下载将会中断退出
*********************************************************/
size_t get_size_struct(void* ptr, size_t size, size_t nmemb, void* data){
    return (size_t)(size * nmemb);
}

double CHttpFileDown::GetDownloadFileSize()
{
    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetDownloadFileSize] Begin Get FileSize Url=[%s]\n", mDownReq.sUrl.c_str());
    if(mDownReq.sUrl.empty()){
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetDownloadFileSize] Url Empty\n");
        return ND_ERROR_INVALID_REQUEST;
    }

    double downloadFileLenth = 0;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, mDownReq.sUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, get_size_struct);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, VALUE_CURLOPT_CONNECTTIMEOUT);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, VALUE_CURLOPT_LOW_SPEED_TIME);
    //打开详细的调试信息
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    //if(fErr != NULL) curl_easy_setopt( curl, CURLOPT_STDERR, fErr);

    int iReperform = VALUE_CURLOPT_DOWNLOAD_RETRY_TIMES;
    int iCouldNotConnect = 1;
RePerform:
    CURLcode ret = curl_easy_perform(curl);
    if (ret == CURLE_OK)
    {
        long retcode = 0;
        CURLcode code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode);
        if ( (code == CURLE_OK) && (retcode == 200) )
        {
            curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &downloadFileLenth);
            AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetDownloadFileSize] Begin Get downloadFileLenth=[%f]\n", downloadFileLenth);
        }else{
            AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetDownloadFileSize] CURLCode=[%d] retcode=[%d]\n", code, retcode);
        }
    }
    else
    {
        //增加ipv4与ipv6的判断
        if( ret == CURLE_COULDNT_CONNECT){
            if(iCouldNotConnect >= 2) curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
            iCouldNotConnect ++;
        }
        //增加网络故障retry
        if ((ret == CURLE_OPERATION_TIMEDOUT ||
                ret == CURLE_SEND_ERROR ||
                ret == CURLE_RECV_ERROR ||
                ret == CURLE_PARTIAL_FILE ||
                ret == CURLE_COULDNT_CONNECT ||
                ret == CURLE_COULDNT_RESOLVE_HOST) && (iReperform > 0)){

            AfxWriteDebugLog("dpclient run at [CHttpFileDown::GetDownloadFileSize] perform return =[%d], reperform=[%d]\n", ret, iReperform);
            iReperform--;
            goto RePerform;
        }

        downloadFileLenth = -1;
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetDownloadFileSize] Begin Get downloadFileLenth ERROR Curl=[%d]\n", ret);
    }
    curl_easy_cleanup(curl);
    //if(fErr != NULL) fclose(fErr);

    return downloadFileLenth;
}

/*********************************************************
函数说明：处理下载事务
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CHttpFileDown::BeginDownload()
{
    if(mDownReq.sUrl.empty()){
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::DealActionWithModel] Download Url Empty\n");
        return ND_ERROR_INVALID_REQUEST;
    }

    if(mDownReq.sLocalFile.empty() && mDownReq.fFile == NULL){
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::DealActionWithModel] Local Or File handler Empty\n");
        return ND_ERROR_INVALID_REQUEST;
    }

    if(mDownReq.iSize <= 0){
        mDownReq.iSize = GetDownloadFileSize();
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::DealActionWithModel] FileSize=[%f]\n", mDownReq.iSize);
        if(mDownReq.iSize <= 0){
            return ND_ERROR_DOWNLOAD_ABORTED;
        }
    }

    CURLcode ret = GetFile();
    if (ret != CURLE_OK){
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::DealActionWithModel] Curl Retcode=[%d]\n", ret);
        return ND_ERROR_DOWNLOAD_ABORTED;
    }

    return ND_SUCCESS;
}

/*********************************************************
函数说明：获取下载文件存储的文件指针
入参说明：无
出参说明：无
返回值  ：文件指针
*********************************************************/
FILE *CHttpFileDown::GetDownLoadFile()
{
    return  mFile;
}

/*********************************************************
函数说明：关闭下载文件
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
void CHttpFileDown::FileClose()
{
    if(mFile != NULL)
        fclose(mFile);
    mFile = NULL;
}

/*********************************************************
函数说明：下载文件主函数
入参说明：pCookie-cookie信息
          pCaPath-服务器证书
          pClientCalPath-客户端证书
          pClientCalPassword-客户端key
出参说明：
返回值  ：
*********************************************************/
CURLcode CHttpFileDown::GetFile(const char *pCookie,
                         const char *pCaPath,
                         const char *pClientCalPath,
                         const char *pClientCalPassword) {
    CURL *curl = NULL;
    CURLcode code;
    bool bFirstPerform = true;
    char bufError[CURL_ERROR_SIZE];
    int iReperform = VALUE_CURLOPT_DOWNLOAD_RETRY_TIMES;
    int iCouldNotConnect = 1;

    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Begin Download FIle\n");
    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Url=[%s]\n", mDownReq.sUrl.c_str());

    if(mDownReq.sUrl.empty()){
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Url Empty\n");
        return CURLE_FAILED_INIT;
    }

    curl = curl_easy_init();
    if (curl == NULL) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Curl init Err\n");
        return CURLE_FAILED_INIT;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: *.*");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    //AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_ERRORBUFFER\n");
    code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, bufError);
    if (code != CURLE_OK) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_ERRORBUFFER Err\n");
        goto _END;
    }

    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_RESUME_FROM_LARGE Offset=[%d]\n", mDownReq.iOffset);
    code =curl_easy_setopt(curl, CURLOPT_RESUME_FROM, mDownReq.iOffset);
    if (code != CURLE_OK) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_RESUME_FROM_LARGE Err\n");
        goto _END;
    }

    code = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, VALUE_CURLOPT_CONNECTTIMEOUT);
    if (code != CURLE_OK) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_TIMEOUT Err\n");
        goto _END;
    }

    //code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    //if (code != CURLE_OK) {
    //    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_TIMEOUT Err\n");
    //    goto _END;
    //}

    code =curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    if (code != CURLE_OK) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_NOSIGNAL Err\n");
        goto _END;
    }

    //curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 60 * 60);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, VALUE_CURLOPT_LOW_SPEED_TIME);

    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_URL\n");
    code = curl_easy_setopt(curl, CURLOPT_URL, mDownReq.sUrl.c_str());
    if (code != CURLE_OK) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_URL Err\n");
        goto _END;
    }

    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    if (code != CURLE_OK) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_FOLLOWLOCATION Err\n");
        goto _END;
    }

    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData_GetFile);
    if (code != CURLE_OK) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_WRITEFUNCTION Err\n");
        goto _END;
    }

    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    if (code != CURLE_OK) {
        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CURLOPT_WRITEDATA Err\n");
        goto _END;
    }

    //AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Cookie\n");
    if (pCookie != 0) {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, (void *) pCookie);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, (void *) pCookie);
    }

    //单向认证用
    //AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] CaPath\n");
    if (pCaPath != 0) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
    }

    //服务端需要认证客户端的真实性，即双向认证。
    if (pClientCalPath != 0 && pClientCalPassword != 0) {
        curl_easy_setopt(curl, CURLOPT_SSLCERT, pClientCalPath);
        curl_easy_setopt(curl, CURLOPT_SSLCERTPASSWD, pClientCalPassword);
        curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
        curl_easy_setopt(curl, CURLOPT_SSLKEY, pClientCalPath);
        curl_easy_setopt(curl, CURLOPT_SSLKEYPASSWD, pClientCalPassword);
        curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");
    }

    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] curl_easy_perform\n");

RePerform:
    code = curl_easy_perform(curl);
    if (code != CURLE_OK) {
        if(code == CURLE_RANGE_ERROR && bFirstPerform)
        {
            AfxWriteDebugLog("SuperVPN run at [%s] Error End CURL Code=[%d] RePerform^_^\n", __FUNCTION__, code);
            curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, mDownReq.iOffset);
            bFirstPerform = false;
            iReperform = VALUE_CURLOPT_DOWNLOAD_RETRY_TIMES;
            goto RePerform;
        }

        //增加ipv4与ipv6的判断
        if( code == CURLE_COULDNT_CONNECT){
            if(iCouldNotConnect >= 2) curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
            iCouldNotConnect ++;
        }
        //增加网络故障retry
        if ((code == CURLE_OPERATION_TIMEDOUT ||
             code == CURLE_SEND_ERROR ||
             code == CURLE_RECV_ERROR ||
             code == CURLE_PARTIAL_FILE ||
             code == CURLE_COULDNT_CONNECT ||
             code == CURLE_COULDNT_RESOLVE_HOST) && (iReperform > 0)){

            bFirstPerform = true;
            mDownReq.iOffset = mDownRsp.uiChunkOffset;
            curl_easy_setopt(curl, CURLOPT_RESUME_FROM, mDownReq.iOffset);

            AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] perform return =[%d], reperform=[%d] Offset=[%u]\n", code, iReperform, mDownReq.iOffset);
            iReperform--;
            goto RePerform;
        }

        AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] curl_easy_perform Return Err code=[%d]\n", code);
        goto _END;
    }

    long retcode;
    code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode);
    if (code == CURLE_OK){
        if  (retcode =! 200){
            if( !mDownReq.sLocalFile.empty() && remove(mDownReq.sLocalFile.c_str()) != 0 )
                AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Remove File Err=[%s]\n", mDownReq.sLocalFile.c_str());
            else
                AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Remove File Success=[%s]\n", mDownReq.sLocalFile.c_str());
        }
    }else{
        if( !mDownReq.sLocalFile.empty() && remove(mDownReq.sLocalFile.c_str()) != 0 )
            AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Remove File Err=[%s]\n", mDownReq.sLocalFile.c_str());
        else
            AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] Remove File Success=[%s]\n", mDownReq.sLocalFile.c_str());
    }

    _END:
    curl_easy_cleanup(curl);

    if(!mDownReq.sLocalFile.empty())
        FileClose();

    AfxWriteDebugLog("SuperVPN run at [CHttpFileDown::GetFile] End>>>\n");

    return code;
}
