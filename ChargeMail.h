// ChargeMail.h: interface for the CChargeMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARGEMAIL_H__2842C7C0_E2EF_4CEB_BB95_F654CB1CF17B__INCLUDED_)
#define AFX_CHARGEMAIL_H__2842C7C0_E2EF_4CEB_BB95_F654CB1CF17B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// #import  "jmail.dll"
// using namespace jmail;
class CChargeMail  
{
public:
	CList<CString,CString&>* attlist;
	string Replyer;
	CList<CString,CString&>* filenotfind;
	string body;
	string MailID;
	CChargeMail();
	jmail::IMessagePtr pMessage;
	CChargeMail(jmail::IMessagePtr message);
	virtual ~CChargeMail();

};

#endif // !defined(AFX_CHARGEMAIL_H__2842C7C0_E2EF_4CEB_BB95_F654CB1CF17B__INCLUDED_)
