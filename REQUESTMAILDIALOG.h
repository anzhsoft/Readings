#if !defined(AFX_REQUESTMAILDIALOG_H__11FC15E3_C7B0_4D91_8732_C3249FE007DB__INCLUDED_)
#define AFX_REQUESTMAILDIALOG_H__11FC15E3_C7B0_4D91_8732_C3249FE007DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// REQUESTMAILDIALOG.h : header file
//
#include "RequestMail.h"
#include "RECVMAILDIALOG.h"
/////////////////////////////////////////////////////////////////////////////
// CREQUESTMAILDIALOG dialog

class CREQUESTMAILDIALOG : public CDialog
{
// Construction
public:
	CRECVMAILDIALOG* recvmaildlg;
	CList<CString,CString&>* selecteditems;
	static unsigned __stdcall _SendRefuseEMail(LPVOID pParam);
	static unsigned __stdcall _SendEMail(LPVOID pParam);
	CRequestMail mailinfo;
	
	CREQUESTMAILDIALOG(CWnd* pParent = NULL);   // standard constructor
	CREQUESTMAILDIALOG(CRequestMail info, CRECVMAILDIALOG* dlg, CWnd* pParent = NULL);
// Dialog Data
	//{{AFX_DATA(CREQUESTMAILDIALOG)
	enum { IDD = IDD_REQUESTMAILDIALOG };
	CButton	m_agreeButton;
	CStatic	m_info;
	CButton	m_REFUSEBUTTON;
	CStatic	m_sourcename;
	CStatic	m_username;
	CListBox	m_SOURCTMEMBER;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CREQUESTMAILDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CREQUESTMAILDIALOG)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRefusebutton();
	afx_msg void OnAgreebutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REQUESTMAILDIALOG_H__11FC15E3_C7B0_4D91_8732_C3249FE007DB__INCLUDED_)
