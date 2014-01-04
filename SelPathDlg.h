#if !defined(AFX_SELPATHDLG_H__638EBA6C_EDAE_4F4E_A648_54B7A12C276D__INCLUDED_)
#define AFX_SELPATHDLG_H__638EBA6C_EDAE_4F4E_A648_54B7A12C276D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelPathDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelPathDlg dialog

class CSelPathDlg : public CDialog
{
// Construction
public:
	CSelPathDlg(CWnd* pParent = NULL);   // standard constructor
	CString strPath;
	BOOL flag;
	DWORD defSel;

	static void CountFile(LPCSTR Path, WIN32_FIND_DATA* pData, DWORD& data);
	static LPVOID InsertFolder(LPCSTR Path, WIN32_FIND_DATA* pData,
							 LPVOID pfParam, DWORD& subdata);
	static LPVOID InsertSDFolder(LPCSTR Path, WIN32_FIND_DATA* pData,
							 LPVOID pfParam, DWORD& subdata);
	static void SetFolderStatics(LPCSTR Path, WIN32_FIND_DATA* pData,
						   LPVOID pParam, DWORD subdata, DWORD& data);
	
// Dialog Data
	//{{AFX_DATA(CSelPathDlg)
	enum { IDD = IDD_SELPATH_DIALOG };
	CTreeCtrl	m_ctrTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelPathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HTREEITEM removeableItem;
	HTREEITEM localItem;

	// Generated message map functions
	//{{AFX_MSG(CSelPathDlg)
	afx_msg void OnSelchangedTreeDownload(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonNewtype();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELPATHDLG_H__638EBA6C_EDAE_4F4E_A648_54B7A12C276D__INCLUDED_)
