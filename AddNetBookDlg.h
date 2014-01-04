#if !defined(AFX_ADDNETBOOKDLG_H__5B2945CA_0A25_4A79_B447_F8B9CFCB814E__INCLUDED_)
#define AFX_ADDNETBOOKDLG_H__5B2945CA_0A25_4A79_B447_F8B9CFCB814E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddNetBookDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddNetBookDlg dialog

class CAddNetBookDlg : public CDialog
{
// Construction
public:
	CAddNetBookDlg(CWnd* pParent = NULL);   // standard constructor
	CAddNetBookDlg(CString name,CString url,CWnd* pParent = NULL); 
//	CString m_name;
//	CString m_url;
// Dialog Data
	//{{AFX_DATA(CAddNetBookDlg)
	enum { IDD = IDD_DIALOG_ADDNETBOOK };
	CString	m_netBookName;
	CString	m_netBookURL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddNetBookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddNetBookDlg)
	afx_msg void OnButtonAddnetbook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDNETBOOKDLG_H__5B2945CA_0A25_4A79_B447_F8B9CFCB814E__INCLUDED_)
