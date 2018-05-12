//
// Created by lewis on 2017/9/3.
// FJ-FZ
//
#ifndef _HTTPCLIENT_H_
#define _HTTPCLIENT_H_


#include <curl.h>
#include "NDType.hpp"
#include "NDStruct.hpp"

/*
如果要在多线程方式下同时调用多个CHttpClient实例，
需要在App初始化的时候调用network::Init();
在App结束的时候调用network::Cleanup();
编译libcurl必须打开zlib标志，并且把OpenSSL也链进去。
*/

namespace network {
    void Init();

    void Cleanup();

    class CHttpClient {
    public:
        CHttpClient(void);
        ~CHttpClient(void);
        static bool mFirstCheckCert;

    protected:
        static ndString mNonce;
        static ndString mKey;
        static ndString mZToken;


		//服务端URL
		ndString mSrvURL;
	    //发送的数据buff
	    ndString mSendBuf;
	    //接收的数据buff
	    ndString mRcvBuf;			


        //页面的返回码
        long mHttpRetcode;

        //生成认证头部信息
        virtual ndString GenerateAuthHeader() = 0;

        virtual void AnalysisAuthHeader(ndString &head);

        //获取服务器证书路径
        virtual ndString GetCalPath();
        //获取客户端证书路径
        virtual ndString GetClientCalPath();
        //获取客户端key路径
        virtual ndString GetClientKeyPath();

    public:
        /**
         * @brief HTTP/HTTPS POST/GET请求
         * @param pUrl
         * @param pPost
         * @param strResponse
         * @return返回是否Post成功
         * 0  成功
         * 7  无法连接
         * 28 超时
         * 58 服务端验证客户端证书失败。
         * 60 客户端验证服务端证书失败。
         */
        CURLcode Post(const char *pUrl, const char *pPost, ndString &strResponse);

        /**
        * @brief HTTP/HTTPS POST/GET请求
        * @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
        * @param strPost 输入参数,使用如下格式para1=val1?2=val2&…
        * @param strCookie 输入参数,Cookie文件名，例如 d:\temp\cookie.txt
        *                          如果为空，不启用Cookie.
        * @param strResponse 输出参数,返回的内容
        * @param bPost 是否Post方式发送请求，默认Post方式发送请求。
        * @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
        * @param pClientCalPath 输入参数，为客户端证书的路径.如果输入为NULL,则不验证客户端证书的有效性.
        * @param pClientCalPassword 输入参数，为客户端证书的存取密码.
        * @return 返回是否Post成功
        * 0  成功
        * 7  无法连接
        * 28 超时
        * 58 服务端验证客户端证书失败。
        * 60 客户端验证服务端证书失败。
        */
        CURLcode Post(const char *pUrl,
                 const char *pPost,
                      ndString &strResponse,
                 bool bPost,
                 const char *pCookie = NULL,
                 const char *pCaPath = NULL,
                 const char *pClientCalPath = NULL,
                 const char *pClientCalPassword = NULL,
                 const char *pClientKeyPath = NULL,
                 const char *pClientKeyPassword = NULL);

        int MultipartFormdata(const char *pUrl,
                              const map<string, string> &mapFields,
                              const map< string, vector<string> > &mapFiles,
                              string &strResponse,
                              const char *pCookie = NULL,
                              const char *pCaPath = NULL,
                              const char *pClientCalPath = NULL,
                              const char *pClientCalPassword = NULL);

    public:
        long GetHttpReturnCode();

        void SetDebug(bool bDebug);

    private:
        bool m_bDebug;
    };
}
#endif
