// RefuseMail.h: interface for the CRefuseMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REFUSEMAIL_H__C132378C_757F_468C_8017_495B1112B154__INCLUDED_)
#define AFX_REFUSEMAIL_H__C132378C_757F_468C_8017_495B1112B154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// #import  "jmail.dll"
// using namespace jmail;
class CRefuseMail  
{
public:
	string filename;
	string Reason;
	string Replyer;
	string body;
	string MailID;
	CRefuseMail();
	jmail::IMessagePtr pMessage;
	CRefuseMail(jmail::IMessagePtr message);
	virtual ~CRefuseMail();

};

#endif // !defined(AFX_REFUSEMAIL_H__C132378C_757F_468C_8017_495B1112B154__INCLUDED_)
