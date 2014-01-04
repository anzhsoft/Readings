#if !defined(AFX_CHARGETMAILDIALOG_H__BE9E954C_0895_4D8B_9EB8_1D0D8F767D5C__INCLUDED_)
#define AFX_CHARGETMAILDIALOG_H__BE9E954C_0895_4D8B_9EB8_1D0D8F767D5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CHARGETMAILDIALOG.h : header file
//
#include "ChargeMail.h"
#include "RECVMAILDIALOG.h"
/////////////////////////////////////////////////////////////////////////////
// CCHARGETMAILDIALOG dialog

class CCHARGETMAILDIALOG : public CDialog
{
// Construction
public:
	CRECVMAILDIALOG* recvmaildlg;
	static unsigned __stdcall  _deal(LPVOID pParam);
	CChargeMail chargemail;
	CCHARGETMAILDIALOG(CWnd* pParent = NULL);   // standard constructor
	CCHARGETMAILDIALOG(CChargeMail mail, CRECVMAILDIALOG* dlg, CWnd* pParent = NULL);
// Dialog Data
	//{{AFX_DATA(CCHARGETMAILDIALOG)
	enum { IDD = IDD_CHARGETMAILDIALOG };
	CListBox	m_attlist;
	CStatic	m_info;
	CListBox	m_filelist;
	CStatic	m_sender;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCHARGETMAILDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCHARGETMAILDIALOG)
	afx_msg void OnDestroy();
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARGETMAILDIALOG_H__BE9E954C_0895_4D8B_9EB8_1D0D8F767D5C__INCLUDED_)
