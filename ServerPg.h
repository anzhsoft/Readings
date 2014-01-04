#if !defined(AFX_SERVERPG_H__973783CC_A414_46CA_A843_DD0A149D7CA6__INCLUDED_)
#define AFX_SERVERPG_H__973783CC_A414_46CA_A843_DD0A149D7CA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerPg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerPg dialog

class CServerPg : public CDialog
{
// Construction
public:
	void SaveInfo();
	void ChangeState();
	void InitialState();

	CServerPg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerPg)
	enum { IDD = IDD_CONFIGPAGE_SERVER };
	CIPAddressCtrl	m_ip;
	UINT	m_tranport;
	UINT	m_sevport;
	UINT	m_ctrport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerPg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERPG_H__973783CC_A414_46CA_A843_DD0A149D7CA6__INCLUDED_)
