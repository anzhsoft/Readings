#if !defined(AFX_SYNLISTDLG_H__7DEAC260_A3C6_4EF6_890F_FF854AC9E6B4__INCLUDED_)
#define AFX_SYNLISTDLG_H__7DEAC260_A3C6_4EF6_890F_FF854AC9E6B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SynListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSynListDlg dialog

class CSynListDlg : public CDialog
{
// Construction
public:
	void InsertItem();
	CSynListDlg(CWnd* pParent = NULL);   // standard constructor
	CLocalList* m_synlist;

// Dialog Data
	//{{AFX_DATA(CSynListDlg)
	enum { IDD = IDD_DIALOG_SYN_LIST };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSynListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSynListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSelall();
	afx_msg void OnButtonCancelall();
	afx_msg void OnButtonSelhighlight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNLISTDLG_H__7DEAC260_A3C6_4EF6_890F_FF854AC9E6B4__INCLUDED_)
