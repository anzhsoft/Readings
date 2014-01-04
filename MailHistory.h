#if !defined(AFX_MAILHISTORY_H__85A7A711_EC12_4073_9151_7A8E5AD19D03__INCLUDED_)
#define AFX_MAILHISTORY_H__85A7A711_EC12_4073_9151_7A8E5AD19D03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MailHistory.h : header file
//
#include "ChargeMail.h"
#include "AgreeMail.h"
#include "RefuseMail.h"
#include "RequestMail.h"
/////////////////////////////////////////////////////////////////////////////
// CMailHistory dialog

class CMailHistory : public CDialog
{
// Construction
public:
	CMailHistory(CWnd* pParent = NULL);   // standard constructor
	CList<CChargeMail,CChargeMail&>* ChargeMailList;
	CList<CAgreeMail,CAgreeMail&>* AgreeMailList;
	CList<CRefuseMail,CRefuseMail&>* RefuseMailList;
	CList<CRequestMail,CRequestMail&>* RequestMailList;
// Dialog Data
	//{{AFX_DATA(CMailHistory)
	enum { IDD = IDD_MailHistory };
	CListBox	m_HasReadedRefuseMailList;
	CListBox	m_HasReadedChargeMailList;
	CListBox	m_HasReadedAgreeMailList;
	CListBox	m_HasReadedRequestMailList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailHistory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMailHistory)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkMailLIST1();
	afx_msg void OnDblclkMailLIST2();
	afx_msg void OnDblclkMailLIST3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILHISTORY_H__85A7A711_EC12_4073_9151_7A8E5AD19D03__INCLUDED_)
