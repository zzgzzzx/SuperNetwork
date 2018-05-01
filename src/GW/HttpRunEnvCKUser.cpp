/*********************************************************
模块名       : 节点http协议
文件名       : HttpUpdateCK.cpp
相关文件     : HttpUpdateCK.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpRunEnvCKUser.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"

/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpRunEvnCKUser::CHttpRunEvnCKUser(CNodeBase *node):CHttpRunEvnCK(node)
{
	mLocalVersion = SUPER_VPN_CLIENT_VER_USER;
}
