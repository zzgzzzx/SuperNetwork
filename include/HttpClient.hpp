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
���Ҫ�ڶ��̷߳�ʽ��ͬʱ���ö��CHttpClientʵ����
��Ҫ��App��ʼ����ʱ�����network::Init();
��App������ʱ�����network::Cleanup();
����libcurl�����zlib��־�����Ұ�OpenSSLҲ����ȥ��
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


		//�����URL
		ndString mSrvURL;
	    //���͵�����buff
	    ndString mSendBuf;
	    //���յ�����buff
	    ndString mRcvBuf;			


        //ҳ��ķ�����
        long mHttpRetcode;

        //������֤ͷ����Ϣ
        virtual ndString GenerateAuthHeader() = 0;

        virtual void AnalysisAuthHeader(ndString &head);

        //��ȡ������֤��·��
        virtual ndString GetCalPath();
        //��ȡ�ͻ���֤��·��
        virtual ndString GetClientCalPath();
        //��ȡ�ͻ���key·��
        virtual ndString GetClientKeyPath();

        //��ȡ�Ӵ�����
        bool GetSubString(string sou, string bFlag, string eFlag, string &out);

    public:
        /**
         * @brief HTTP/HTTPS POST/GET����
         * @param pUrl
         * @param pPost
         * @param strResponse
         * @return�����Ƿ�Post�ɹ�
         * 0  �ɹ�
         * 7  �޷�����
         * 28 ��ʱ
         * 58 �������֤�ͻ���֤��ʧ�ܡ�
         * 60 �ͻ�����֤�����֤��ʧ�ܡ�
         */
        CURLcode Post(const char *pUrl, const char *pPost, ndString &strResponse);

        /**
        * @brief HTTP/HTTPS POST/GET����
        * @param strUrl �������,�����Url��ַ,��:https://www.alipay.com
        * @param strPost �������,ʹ�����¸�ʽpara1=val1?2=val2&��
        * @param strCookie �������,Cookie�ļ��������� d:\temp\cookie.txt
        *                          ���Ϊ�գ�������Cookie.
        * @param strResponse �������,���ص�����
        * @param bPost �Ƿ�Post��ʽ��������Ĭ��Post��ʽ��������
        * @param pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��.
        * @param pClientCalPath ���������Ϊ�ͻ���֤���·��.�������ΪNULL,����֤�ͻ���֤�����Ч��.
        * @param pClientCalPassword ���������Ϊ�ͻ���֤��Ĵ�ȡ����.
        * @return �����Ƿ�Post�ɹ�
        * 0  �ɹ�
        * 7  �޷�����
        * 28 ��ʱ
        * 58 �������֤�ͻ���֤��ʧ�ܡ�
        * 60 �ͻ�����֤�����֤��ʧ�ܡ�
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
