// Mail.h: interface for the CMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAIL_H__0073AB29_A4A8_4E0C_B3C9_113CD2CB9114__INCLUDED_)
#define AFX_MAIL_H__0073AB29_A4A8_4E0C_B3C9_113CD2CB9114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

class CMail  
{
public:
	static CList<CString,CString&>* parseResourceList(string body);
	CList<CString,CString&>* files;//请求者资源列表
	string RequestSourceFilename;//请求的文件名
	string RequesterEmail;//请求者Email
	string RequesterUser;//请求者用户名
	string MailTitle;//邮件标题
	string MailBody;//邮件正文
	string MailId;//邮件id
	CMail();
	CMail(string title,string body,string id);
	virtual ~CMail();

};

#endif // !defined(AFX_MAIL_H__0073AB29_A4A8_4E0C_B3C9_113CD2CB9114__INCLUDED_)
