//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef _HTTPFILEDOWN_H_
#define _HTTPFILEDOWN_H_

#include <curl.h>
#include "NDType.hpp"
#include "NDStruct.hpp"


class CHttpFileDown{
private:
    //文件句柄
    FILE *mFile;
    //下载的文件
    SDownloadFileReqSt mDownReq;
    //下载文件的返回
    SDownloadFileRspSt mDownRsp;
    //是否停止下载
    bool mIFAbort;
    double GetDownloadFileSize();

protected:
    CURLcode GetFile(const char *pCookie = NULL,
                     const char *pCaPath = NULL,
                     const char *pClientCalPath = NULL,
                     const char *pClientCalPassword = NULL);

public:
    CHttpFileDown(void);
    ~CHttpFileDown(void);

    //设置下载的请求数据
    bool SetDownFileReq(SDownloadFileReqSt req);
    //读取下载请求数据
    SDownloadFileReqSt GetDownFileReq();

    //读取下载应答数据
    SDownloadFileRspSt GetDownFileRsp();
    //设置下载应答数据
    ndBool SetDownFileRsp(ndUInt32 uiChunkOffset);

    //处理下载事务
    ndStatus BeginDownload();
	
    //停止下载
    void AbortDownload();
    //获取是否中断下载
    bool IFAbortDownload();
    //获取下载文件的句柄
    FILE *GetDownLoadFile();
    //关闭下载文件
    void FileClose();
};
#endif
