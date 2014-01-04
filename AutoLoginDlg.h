#if !defined(AFX_AUTOLOGINDLG_H__8DDE9A1A_A4C1_41E2_9C35_5350F2829897__INCLUDED_)
#define AFX_AUTOLOGINDLG_H__8DDE9A1A_A4C1_41E2_9C35_5350F2829897__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoLoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoLoginDlg dialog

class CAutoLoginDlg : public CDialog
{
// Construction
public:
	CBitmap bgBmp;
	CAutoLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoLoginDlg)
	enum { IDD = IDD_DIALOG_AUTO_LOGIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoLoginDlg)
		// NOTE: the ClassWizard will add member functions here
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		virtual BOOL OnInitDialog();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOLOGINDLG_H__8DDE9A1A_A4C1_41E2_9C35_5350F2829897__INCLUDED_)
