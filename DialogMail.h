#if !defined(AFX_DIALOGMAIL_H__958C09FF_A187_45B5_ABED_17A4EB2C8982__INCLUDED_)
#define AFX_DIALOGMAIL_H__958C09FF_A187_45B5_ABED_17A4EB2C8982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMail dialog

class CDialogMail : public CDialog
{
// Construction
public:
	CString resource;
	CDialogMail(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogMail)
	enum { IDD = IDD_DIALOG_MAIL };
	BOOL	issms;
	CString	m_resource_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMail)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnToolsReadwolf();
//	afx_msg void OnToolsMakewolf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGMAIL_H__958C09FF_A187_45B5_ABED_17A4EB2C8982__INCLUDED_)
