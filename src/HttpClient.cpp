/*********************************************************
ģ����       : HttpЭ���װ��
�ļ���       : HttpClient.cpp
����ļ�     : HttpClient.h
�ļ�ʵ�ֹ��� : HttpЭ���װ��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpClient.hpp"
#include "NodeSrv.hpp"
#include "NDFunc.hpp"

namespace network {
    CHttpClient::CHttpClient(void) :
            m_bDebug(false) {
        mHttpRetcode = 0;
    }

    CHttpClient::~CHttpClient(void) {

    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    static int OnDebug(CURL *, curl_infotype itype, char *pData, size_t size, void *) {
        if (itype == CURLINFO_TEXT) {
            AfxWriteDebugLog("SuperVPN run at [%s] TEXT=[%s]", __func__, pData);
        } else if (itype == CURLINFO_HEADER_IN) {
            AfxWriteDebugLog("SuperVPN run at [%s] HEADER_IN=[%s]", __func__, pData);
        } else if (itype == CURLINFO_HEADER_OUT) {
            AfxWriteDebugLog("SuperVPN run at [%s] HEADER_OUT=[%s]", __func__, pData);
        } else if (itype == CURLINFO_DATA_IN) {
            AfxWriteDebugLog("SuperVPN run at [%s] DATA_IN=[%s]", __func__, pData);
        } else if (itype == CURLINFO_DATA_OUT) {
            AfxWriteDebugLog("SuperVPN run at [%s] DATA_OUT=[%s]", __func__, pData);
        }
        return 0;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    size_t OnWriteData_Post(void *buffer, size_t size, size_t nmemb, void *lpVoid) {
        ndString *str = reinterpret_cast<ndString *>(lpVoid);
        if (NULL == str || NULL == buffer) {
            return -1;
        }

        char *pData = reinterpret_cast<char *>(buffer);
        str->append(pData, size * nmemb);
        return size * nmemb;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    size_t
    OnWriteData_MultipartFormdata(void *inBuffer, size_t size, size_t nmemb, void *outBuffer) {
        int len = size * nmemb;
        char *temp = new char[len + 1];
        memcpy(temp, inBuffer, len);
        temp[len] = 0;
        reinterpret_cast<ndString *>(outBuffer)->append(temp);
        delete temp;
        return len;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    size_t OnWriteData_GetFile(void *inBuffer, int size, int nmemb, ndString &content) {
        long len = size * nmemb;
        ndString temp((char *) inBuffer, len);
        content += temp;
        return len;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    long CHttpClient:: GetHttpReturnCode(){
        return mHttpRetcode;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    ndString CHttpClient::GetCalPath()
    {
        return "";
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    ndString CHttpClient::GetClientCalPath()
    {
        return "";
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    ndString CHttpClient::GetClientKeyPath()
    {
        return "";
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    CURLcode CHttpClient::Post(const char *pUrl, const char *pPost, ndString &strResponse)
    {
//        if(GetCalPath().empty()){
//            AfxWriteDebugLog("SuperVPN run at [%s] Server PEM IS NULL", __func__);
//            return CURLE_FAILED_INIT;
//        }

        return Post(pUrl,
                    pPost,
                    strResponse,
                    true,
                    NULL,
                    GetCalPath().empty()?NULL:GetCalPath().c_str(),
                    GetClientCalPath().empty()?NULL:GetClientCalPath().c_str(),
                    NULL,
                    GetClientKeyPath().empty()?NULL:GetClientKeyPath().c_str(),
                    NULL);
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    CURLcode CHttpClient::Post(const char *pUrl,
                          const char *pPost,
                          ndString &strResponse,
                          bool bPost,
                          const char *pCookie,
                          const char *pCaPath,
                          const char *pClientCalPath,
                          const char *pClientCalPassword,
                          const char *pClientKeyPath,
                          const char *pClientKeyPassword) {
        strResponse = "";
        ndString strHeadData;

        CURLcode res;
        CURL *curl = curl_easy_init();
        if (NULL == curl) {
            AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Curl Init Error");
            return CURLE_FAILED_INIT;
        }

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: *.*");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        //headers = curl_slist_append(headers, GenerateAuthentication().c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Curl Easy Setopt URL=[%s]", pUrl);
        if (m_bDebug) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
        }
        curl_easy_setopt(curl, CURLOPT_URL, pUrl);
        if (bPost) {
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pPost);
        }
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData_Post);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &strResponse);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, OnWriteData_Post);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *) &strHeadData);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

        AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Curl Add Header Inform");

        if (pCookie != NULL) {
            AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Curl Set Cookie");
            curl_easy_setopt(curl, CURLOPT_COOKIEFILE, (void *) pCookie);
            curl_easy_setopt(curl, CURLOPT_COOKIEJAR, (void *) pCookie);
        }

        if (NULL == pCaPath) {
            AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Curl Set Not Support PEM");
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);

            //��Ҫ�ڱ���curl��ʱ��һͬ����zlib��־��Ҫ��Ȼ�Ҳ��������־.
            //��Ȼǰ�������Ѿ��������zlib.
            //��������gzipѹ�����ݵ��������������֧��gzip���ݣ��᷵��ѹ��������ݡ�
            //���Http��������֧��gzip encodingҲ��Ӱ��libcurl����������
            //�������ݵ�ʱ��������ص���ѹ�����ݣ�libcurl���Զ���ѹ���ݡ�
            //curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
        } else {
            //ȱʡ�������PEM�������������ã�����֧��DER
            //curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
            AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Curl Set PEM=[%s]", pCaPath);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
            curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);

            //�������Ҫ��֤�ͻ��˵���ʵ�ԣ���˫����֤��
            if (pClientCalPath != NULL) {
                AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Curl Set Client PEM");
                curl_easy_setopt(curl, CURLOPT_SSLCERT, pClientCalPath);
                if(pClientCalPassword != NULL)
                    curl_easy_setopt(curl, CURLOPT_SSLCERTPASSWD, pClientCalPassword);
                curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
                curl_easy_setopt(curl, CURLOPT_SSLKEY, pClientKeyPath);
                if(pClientKeyPassword != NULL)
                    curl_easy_setopt(curl, CURLOPT_SSLKEYPASSWD, pClientKeyPassword);
                curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");
            }
        }
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 16);

        //Web������һ����ض������ӣ��������http:/xxx/x1.do�Զ�ת��http:/xxx/x2.do
        //����һ��Ҫ����CURLOPT_FOLLOWLOCATIONΪ1,�����ض��������ݲ��᷵�ء�
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

        AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Begin send to server");
        res = curl_easy_perform(curl);
        //Router������Ҫ������ȡNone��zKey��ZToken, ����ֻ����None
        if(res != CURLE_OK)
            AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Curl Perform Err=[%d]???", res);

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &mHttpRetcode);
        AfxWriteDebugLog("SuperVPN run at [CHttpClient::Post] Http RetCode=[%d]", mHttpRetcode);

        curl_easy_cleanup(curl);

        return res;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    bool CHttpClient::GetSubString(string sou, string bFlag, string eFlag, string &out)
    {
        int bPos = sou.find(bFlag);
        if (bPos == string::npos) return false;

        //AfxWriteDebugLog("SuperVPN run at [CHttpClient::GetSubString] SOU=[%s] bFlag=[%s] eFlag=[%s] bPOS=[%d]", sou.c_str(), bFlag.c_str(), eFlag.c_str(), bPos);

        string tmp = sou.substr(bPos+bFlag.length(), sou.length()-bPos-bFlag.length());
        int ePos = tmp.find(eFlag);
        if (ePos == string::npos) return false;

        out = tmp.substr(0, ePos);
        //AfxWriteDebugLog("SuperVPN run at [CHttpClient::GetSubString] out=[%s]", out.c_str());

        return true;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    void CHttpClient::AnalysisAuthHeader(ndString &head)
    {

    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    int CHttpClient::MultipartFormdata(const char *pUrl,
                                       const map<ndString, ndString> &mapFields,
                                       const map< ndString, vector<ndString> > &mapFiles,
                                       ndString &strResponse,
                                       const char *pCookie, const char *pCaPath,
                                       const char *pClientCalPath, const char *pClientCalPassword) {
        CURL *curl;
        CURLcode res;

        curl = curl_easy_init();
        strResponse = "";

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData_MultipartFormdata);//write_data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponse);

        struct curl_httppost *formpost = 0;
        struct curl_httppost *lastptr = 0;

        map<ndString, ndString>::const_iterator iterFields = mapFields.begin();
        while (iterFields != mapFields.end()) {
            curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, iterFields->first.c_str(),
                         CURLFORM_COPYCONTENTS, iterFields->second.c_str(), CURLFORM_END);
            iterFields++;
        }

        map< ndString, vector<ndString> >::const_iterator iterFiles = mapFiles.begin();
        while (iterFiles != mapFiles.end()) {
            curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME,
                         iterFiles->first.c_str(), CURLFORM_FILE,
                         iterFiles->second[0].c_str(), CURLFORM_CONTENTTYPE,
                         iterFiles->second[1].c_str(), CURLFORM_END);
            iterFiles++;
        }

        curl_easy_setopt(curl, CURLOPT_URL, pUrl);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        if (pCookie != 0) {
            curl_easy_setopt(curl, CURLOPT_COOKIEFILE, (void *) pCookie);
            curl_easy_setopt(curl, CURLOPT_COOKIEJAR, (void *) pCookie);
        }

        //������֤��
        if (pCaPath != 0) {
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
            curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
        }

        //�������Ҫ��֤�ͻ��˵���ʵ�ԣ���˫����֤��
        if (pClientCalPath != 0 && pClientCalPassword != 0) {
            curl_easy_setopt(curl, CURLOPT_SSLCERT, pClientCalPath);
            curl_easy_setopt(curl, CURLOPT_SSLCERTPASSWD, pClientCalPassword);
            curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
            curl_easy_setopt(curl, CURLOPT_SSLKEY, pClientCalPath);
            curl_easy_setopt(curl, CURLOPT_SSLKEYPASSWD, pClientCalPassword);
            curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");
        }

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        curl_formfree(formpost);
        return res;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    void CHttpClient::SetDebug(bool bDebug) {
        m_bDebug = bDebug;
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    void Init() {
        //the function is not thread safe.
        curl_global_init(CURL_GLOBAL_ALL);
    }

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
    void Cleanup() {
        curl_global_cleanup();
    }
}

