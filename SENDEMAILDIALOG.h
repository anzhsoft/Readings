#if !defined(AFX_SENDEMAILDIALOG_H__B1395859_CEDE_452A_9F17_D04FA35E2884__INCLUDED_)
#define AFX_SENDEMAILDIALOG_H__B1395859_CEDE_452A_9F17_D04FA35E2884__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SENDEMAILDIALOG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSENDEMAILDIALOG dialog

class CSENDEMAILDIALOG : public CDialog
{
// Construction
public:
	CSENDEMAILDIALOG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSENDEMAILDIALOG)
	enum { IDD = IDD_SENDEMAILDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSENDEMAILDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSENDEMAILDIALOG)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDEMAILDIALOG_H__B1395859_CEDE_452A_9F17_D04FA35E2884__INCLUDED_)
