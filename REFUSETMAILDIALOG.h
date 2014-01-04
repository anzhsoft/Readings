#if !defined(AFX_REFUSETMAILDIALOG_H__086DBE12_5D07_402F_BFD7_E6DBC327727B__INCLUDED_)
#define AFX_REFUSETMAILDIALOG_H__086DBE12_5D07_402F_BFD7_E6DBC327727B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// REFUSETMAILDIALOG.h : header file
//
#include "RefuseMail.h"
#include "RECVMAILDIALOG.h"
/////////////////////////////////////////////////////////////////////////////
// CREFUSETMAILDIALOG dialog

class CREFUSETMAILDIALOG : public CDialog
{
// Construction
public:
	static unsigned __stdcall _deal(LPVOID pParam);
	CRECVMAILDIALOG* recvmaildlg;
	CRefuseMail refusemail;
	CREFUSETMAILDIALOG(CWnd* pParent = NULL);   // standard constructor
	CREFUSETMAILDIALOG(CRefuseMail mail, CRECVMAILDIALOG* dlg, CWnd* pParent = NULL);
// Dialog Data
	//{{AFX_DATA(CREFUSETMAILDIALOG)
	enum { IDD = IDD_REFUSETMAILDIALOG };
	CStatic	m_filename;
	CStatic	m_Reason;
	CStatic	m_Refuser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CREFUSETMAILDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CREFUSETMAILDIALOG)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REFUSETMAILDIALOG_H__086DBE12_5D07_402F_BFD7_E6DBC327727B__INCLUDED_)
