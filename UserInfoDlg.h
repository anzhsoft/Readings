#if !defined(AFX_USERINFODLG_H__C71C37D2_7721_41FD_B2FB_1541D4954816__INCLUDED_)
#define AFX_USERINFODLG_H__C71C37D2_7721_41FD_B2FB_1541D4954816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg dialog

class CUserInfoDlg : public CDialog
{
// Construction
public:
	STRUCTUSERINFO userinfo;
	CUserInfoDlg(CWnd* pParent = NULL);   // standard constructor
	CUserInfoDlg(STRUCTUSERINFO info,CWnd* pParent = NULL);
// Dialog Data
	//{{AFX_DATA(CUserInfoDlg)
	enum { IDD = IDD_DIALOG_USERINFO };
	CString	m_city;
	CString	m_ip;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserInfoDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERINFODLG_H__C71C37D2_7721_41FD_B2FB_1541D4954816__INCLUDED_)
