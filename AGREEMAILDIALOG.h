#if !defined(AFX_AGREEMAILDIALOG_H__6DF0BDEC_5EFA_4D57_8CC0_D28E3CCF3485__INCLUDED_)
#define AFX_AGREEMAILDIALOG_H__6DF0BDEC_5EFA_4D57_8CC0_D28E3CCF3485__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AGREEMAILDIALOG.h : header file
//
#include "AgreeMail.h"
#include "RECVMAILDIALOG.h"
/////////////////////////////////////////////////////////////////////////////
// CAGREEMAILDIALOG dialog

class CAGREEMAILDIALOG : public CDialog
{
// Construction
public:
	CRECVMAILDIALOG* recvmaildlg;
	static unsigned __stdcall _deal(LPVOID pParam);
	CAgreeMail agreemail;
	CAGREEMAILDIALOG(CWnd* pParent = NULL);   // standard constructor
	CAGREEMAILDIALOG(CAgreeMail mail, CRECVMAILDIALOG* dlg, CWnd* pParent = NULL);
// Dialog Data
	//{{AFX_DATA(CAGREEMAILDIALOG)
	enum { IDD = IDD_AGREEMAILDIALOG };
	CListBox	m_chargefilelist;
	CStatic	m_info;
	CStatic	m_filename;
	CStatic	m_Agreeer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAGREEMAILDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAGREEMAILDIALOG)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGREEMAILDIALOG_H__6DF0BDEC_5EFA_4D57_8CC0_D28E3CCF3485__INCLUDED_)
