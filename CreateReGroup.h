#if !defined(AFX_CREATEREGROUP_H__2AC72C7B_C9B0_4C87_B20E_1805D3CFACFC__INCLUDED_)
#define AFX_CREATEREGROUP_H__2AC72C7B_C9B0_4C87_B20E_1805D3CFACFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateReGroup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateReGroup dialog

class CCreateReGroup : public CDialog
{
// Construction
public:
	CString m_keywords;
	char * gname;
	CCreateReGroup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateReGroup)
	enum { IDD = IDD_DIALOG_CREATE_REGROUP };
	CString	m_des;
	CString	m_name;
	CString	m_key1;
	CString	m_key2;
	CString	m_key3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateReGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateReGroup)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEREGROUP_H__2AC72C7B_C9B0_4C87_B20E_1805D3CFACFC__INCLUDED_)
