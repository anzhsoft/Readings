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
	CList<CString,CString&>* files;//��������Դ�б�
	string RequestSourceFilename;//������ļ���
	string RequesterEmail;//������Email
	string RequesterUser;//�������û���
	string MailTitle;//�ʼ�����
	string MailBody;//�ʼ�����
	string MailId;//�ʼ�id
	CMail();
	CMail(string title,string body,string id);
	virtual ~CMail();

};

#endif // !defined(AFX_MAIL_H__0073AB29_A4A8_4E0C_B3C9_113CD2CB9114__INCLUDED_)
