// AgreeMail.h: interface for the CAgreeMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AGREEMAIL_H__998F764C_38D7_4EE9_9043_7580A29BDADE__INCLUDED_)
#define AFX_AGREEMAIL_H__998F764C_38D7_4EE9_9043_7580A29BDADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// #import  "jmail.dll"
// using namespace jmail;

class CAgreeMail  
{
public:
	string attname;
	string desEMail;
	string AgreeReplyer;
	CList<CString,CString&>* FCResource;
	string body;
	string MailID;
	CAgreeMail();
	jmail::IMessagePtr pMessage;
	//CString MailId;
	CAgreeMail(jmail::IMessagePtr message);
	virtual ~CAgreeMail();

};

#endif // !defined(AFX_AGREEMAIL_H__998F764C_38D7_4EE9_9043_7580A29BDADE__INCLUDED_)
