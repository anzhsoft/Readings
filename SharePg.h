#if !defined(AFX_SHAREPG_H__E0E3B5C9_F1E7_4673_912C_BB5840CBA27E__INCLUDED_)
#define AFX_SHAREPG_H__E0E3B5C9_F1E7_4673_912C_BB5840CBA27E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SharePg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSharePg dialog

class CSharePg : public CDialog
{
// Construction
public:
	CSharePg(CWnd* pParent = NULL);   // standard constructor
	void SetItemDataAndCheck(HTREEITEM hItem, int filetype);
	BOOL IsShareFormat(int filetype);
	void LoadShareList();
	void SaveShareFormat(LPCSTR iniPath);
	BOOL SaveShareFolder();
	void SaveInfo();
	void InitialFormatTree();
	void InitialState();
	void StateChanged();

// Dialog Data
	//{{AFX_DATA(CSharePg)
	enum { IDD = IDD_CONFIGPAGE_SHARE };
	BOOL	m_bsharefolder;
	BOOL	m_bshareformat;
	CTreeCtrl	m_shareFormat;
	CButton	m_delbtn;
	CButton	m_addbtn;
	CListBox	m_sharefolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSharePg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSharePg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAddshare();
	afx_msg void OnCheckSharefolder();
	afx_msg void OnCheckShareformat();
	afx_msg void OnSelchangeListSharefolder();
	afx_msg void OnDestroy();
	afx_msg void OnButtonDelshare();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAREPG_H__E0E3B5C9_F1E7_4673_912C_BB5840CBA27E__INCLUDED_)
