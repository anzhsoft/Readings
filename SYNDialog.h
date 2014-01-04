#if !defined(AFX_SYNDIALOG_H__9040218D_1638_4A91_B625_6EF3EEB9F625__INCLUDED_)
#define AFX_SYNDIALOG_H__9040218D_1638_4A91_B625_6EF3EEB9F625__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SYNDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSYNDialog dialog

class CSYNDialog : public CDialog
{
// Construction
public:
	CSYNDialog(CWnd* pParent = NULL);   // standard constructor
	UINT dire;

	CLocalList list;

	UINT SynToPC();
	UINT SynToSD();

	static UINT _SynToPC(LPVOID pParam);
	static UINT _SynToSD(LPVOID pParam);
	
	void SearchDB();
	void Search(CString findPath, CString title);

// Dialog Data
	//{{AFX_DATA(CSYNDialog)
	enum { IDD = IDD_DIALOG_SYN };
	CProgressCtrl	m_ctrPro;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSYNDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSYNDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNDIALOG_H__9040218D_1638_4A91_B625_6EF3EEB9F625__INCLUDED_)
