#if !defined(AFX_DIGCHINABOOK_H__CB20E08B_5678_402F_A3FD_D6DCDFA16DA4__INCLUDED_)
#define AFX_DIGCHINABOOK_H__CB20E08B_5678_402F_A3FD_D6DCDFA16DA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DigChinaBook.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DigChinaBook dialog

class DigChinaBook : public CDialog
{
// Construction
public:
	CBitmap bgBmp;
	DigChinaBook(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DigChinaBook)
	enum { IDD = IDD_DIALOG_ZST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DigChinaBook)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DigChinaBook)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGCHINABOOK_H__CB20E08B_5678_402F_A3FD_D6DCDFA16DA4__INCLUDED_)
