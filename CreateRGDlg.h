#if !defined(AFX_CREATERGDLG_H__CA4E789A_9DD5_48C0_B66E_8CB5FE5E9E3D__INCLUDED_)
#define AFX_CREATERGDLG_H__CA4E789A_9DD5_48C0_B66E_8CB5FE5E9E3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateRGDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateRGDlg dialog

class CCreateRGDlg : public CDialog
{
// Construction
public:
	CCreateRGDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateRGDlg)
	enum { IDD = IDD_DIALOG_NEWRG };
	CString	m_title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateRGDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateRGDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATERGDLG_H__CA4E789A_9DD5_48C0_B66E_8CB5FE5E9E3D__INCLUDED_)
