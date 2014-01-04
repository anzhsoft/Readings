#if !defined(AFX_NORMALPG_H__E510FCB2_3E68_4C6D_8DC7_FC5DF13206BE__INCLUDED_)
#define AFX_NORMALPG_H__E510FCB2_3E68_4C6D_8DC7_FC5DF13206BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NormalPg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNormalPg dialog

class CNormalPg : public CDialog
{
// Construction
public:
	void SaveReg();
	BOOL IsAutoRun();
	void ChangeState();
	void SaveInfo();
	void InitialState();
	CNormalPg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNormalPg)
	enum { IDD = IDD_CONFIGPAGE_NORMAL };
	CSpinButtonCtrl	m_spin;
	UINT	m_day;
	BOOL	m_bNewVer;
	BOOL	m_bAutoLogin;
	BOOL	m_bAutoRun;
	BOOL	m_bQuitAlert;
	BOOL	m_bShowMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNormalPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNormalPg)
	afx_msg void OnCheckNewversion();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NORMALPG_H__E510FCB2_3E68_4C6D_8DC7_FC5DF13206BE__INCLUDED_)
