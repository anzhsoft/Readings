#if !defined(AFX_ADDUNIVERSITY_H__474A075B_148C_48EB_BF3E_92B901A5123B__INCLUDED_)
#define AFX_ADDUNIVERSITY_H__474A075B_148C_48EB_BF3E_92B901A5123B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddUniversity.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AddUniversity dialog

class AddUniversity : public CDialog
{
// Construction
public:
	AddUniversity(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(AddUniversity)
	enum { IDD = IDD_DIALOGADDUNIVERSITY };
	CString	m_desc;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AddUniversity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AddUniversity)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDUNIVERSITY_H__474A075B_148C_48EB_BF3E_92B901A5123B__INCLUDED_)
