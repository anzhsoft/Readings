#if !defined(AFX_COMMLOGDLG_H__5BEFBFB9_B310_4F5B_99FD_20BBF86415EE__INCLUDED_)
#define AFX_COMMLOGDLG_H__5BEFBFB9_B310_4F5B_99FD_20BBF86415EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommLogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommLogDlg dialog

class CCommLogDlg : public CDialog
{
// Construction
public:
	long countFlowAll(char upDown);
	long countFlowSel(char operNumber);
//	CString m_strNodeName;
	void showLogSel(char operNumber);
	void showLogAll();
	CCommLogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommLogDlg)
	enum { IDD = IDD_DIALOG_COMMLOG };
	CTreeCtrl	m_treeOper;
	CListCtrl	m_listPubInfo;
	CListCtrl	m_listCommLog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommLogDlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeOper(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMLOGDLG_H__5BEFBFB9_B310_4F5B_99FD_20BBF86415EE__INCLUDED_)
