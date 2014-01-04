#if !defined(AFX_RECVMAILDIALOG_H__26BDB86C_F371_4BE5_90CD_BBD1C961FDDA__INCLUDED_)
#define AFX_RECVMAILDIALOG_H__26BDB86C_F371_4BE5_90CD_BBD1C961FDDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RECVMAILDIALOG.h : header file
//
#include "RequestMail.h"
#include "ChargeMail.h"
#include "AgreeMail.h"
#include "RefuseMail.h"

/////////////////////////////////////////////////////////////////////////////
// CRECVMAILDIALOG dialog

class CRECVMAILDIALOG : public CDialog
{
// Construction
public:
	void resetscol();

	CRECVMAILDIALOG(CWnd* pParent = NULL);   // standard constructor
	//CRECVMAILDIALOG(CList<CRequestMail,CRequestMail&>* list, CWnd* pParent = NULL);
	//CRECVMAILDIALOG(CList<CString,CString&> list);
	// Dialog Data
	//{{AFX_DATA(CRECVMAILDIALOG)
	enum { IDD = IDD_RECVMAILDIALOG };
	CStatic	m_stat;
	CListBox	m_DownLoadFiles;
	CListBox	m_MailTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRECVMAILDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRECVMAILDIALOG)
	afx_msg void OnSelchangeMaillist();
	afx_msg void OnDblclkMaillist();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkMailLIST1();
	afx_msg void OnDblclkMailLIST2();
	afx_msg void OnDblclkMailLIST3();
	afx_msg void OnHistoryBUTTON();
	afx_msg void OnReflush();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECVMAILDIALOG_H__26BDB86C_F371_4BE5_90CD_BBD1C961FDDA__INCLUDED_)
