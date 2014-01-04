#if !defined(AFX_TALKDIALOG_H__D5D95172_CE2E_4FC7_9B9E_CB77670F9DD9__INCLUDED_)
#define AFX_TALKDIALOG_H__D5D95172_CE2E_4FC7_9B9E_CB77670F9DD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TalkDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTalkDialog dialog

class CTalkDialog : public CDialog
{
// Construction
public:
	CTalkDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTalkDialog)
	enum { IDD = IDD_DIALOG_TALK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTalkDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBitmap bgBmp;
	// Generated message map functions
	//{{AFX_MSG(CTalkDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TALKDIALOG_H__D5D95172_CE2E_4FC7_9B9E_CB77670F9DD9__INCLUDED_)
