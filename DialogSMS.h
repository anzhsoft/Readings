#if !defined(AFX_DIALOGSMS_H__29F1B964_78A0_4D61_B967_D598C6C6C808__INCLUDED_)
#define AFX_DIALOGSMS_H__29F1B964_78A0_4D61_B967_D598C6C6C808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSMS.h : header file
//
#include "mainlistview.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogSMS dialog

class CDialogSMS : public CDialog
{
// Construction
public:
	void init(CString userName,CString name,CString requestResource,CString AspServer,CMainListView *mainListView,CString path);
	CDialogSMS(CWnd* pParent = NULL);   // standard constructor

	CString userName;
	CString name;
	CString no;
	CString message;
	CString requestResource;
	CString myResource;

	CString server;
	CString object;
	CString dstPath;

	CString AspServer;

	CMainListView* mainListView;

	int port;


// Dialog Data
	//{{AFX_DATA(CDialogSMS)
	enum { IDD = IDD_DIALOG_SMS };
	CEdit	m_resource;
	CEdit	m_message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSMS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSMS)
	virtual void OnOK();
	afx_msg void OnSmsExplorer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSMS_H__29F1B964_78A0_4D61_B967_D598C6C6C808__INCLUDED_)
