//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_SHOWDIALOG_H__2F67D9DA_4352_4DCF_B8D2_39488E7D89E5__INCLUDED_)
#define AFX_SHOWDIALOG_H__2F67D9DA_4352_4DCF_B8D2_39488E7D89E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowDialog dialog

class CShowDialog : public CDialog
{
// Construction
public:
	CShowDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShowDialog)
	enum { IDD = IDD_DIALOG_SHOW };
	CWebBrowser2	m_browser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowDialog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowDialog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWDIALOG_H__2F67D9DA_4352_4DCF_B8D2_39488E7D89E5__INCLUDED_)
