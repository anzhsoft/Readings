#if !defined(AFX_WM_SHOW_DIALOG_H__BBA6C6F1_AEAE_4570_82E4_E46D7096AF51__INCLUDED_)
#define AFX_WM_SHOW_DIALOG_H__BBA6C6F1_AEAE_4570_82E4_E46D7096AF51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WM_Show_Dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WM_Show_Dialog dialog

class WM_Show_Dialog : public CDialog
{
// Construction
public:
	WM_Show_Dialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(WM_Show_Dialog)
	enum { IDD = IDD_DIALOG_WATERMARKLINK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WM_Show_Dialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(WM_Show_Dialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WM_SHOW_DIALOG_H__BBA6C6F1_AEAE_4570_82E4_E46D7096AF51__INCLUDED_)
