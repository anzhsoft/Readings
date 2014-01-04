#if !defined(AFX_PUSHPG_H__246561D1_CB4D_475D_A06B_0028F40E51CB__INCLUDED_)
#define AFX_PUSHPG_H__246561D1_CB4D_475D_A06B_0028F40E51CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PushPg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPushPg dialog

class CPushPg : public CDialog
{
// Construction
public:
	void SaveInfo();
	void InitialState();
	CPushPg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPushPg)
	enum { IDD = IDD_CONFIGPAGE_PUSH };
	BOOL	m_bPush;
	BOOL	m_bReadman;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPushPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPushPg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PUSHPG_H__246561D1_CB4D_475D_A06B_0028F40E51CB__INCLUDED_)
