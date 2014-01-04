#if !defined(AFX_REGSTERDIALOG_H__FF369105_1B6D_4387_81C6_CA0FA181FBF7__INCLUDED_)
#define AFX_REGSTERDIALOG_H__FF369105_1B6D_4387_81C6_CA0FA181FBF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegsterDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegsterDialog dialog
#include "control/WBButton.h"

class CRegsterDialog : public CDialog
{
// Construction
public:
	CRegsterDialog(CWnd* pParent = NULL);   // standard constructor
	CBitmap bgBmp;
	DOWNLOADSERVERS *m_pServer;
	BOOL CheckSDCard();
	void LoadServerInfo();
	void LoadUserInfo();
	void LoadAutoLogin();
	void GetDisorString(char *str, long len);
	void ReceiveIP2();
	int CheckUserRegInfo(LPCSTR FileName);
	void SaveUserInfo();
	void SaveServerFile();
	void SaveServersInfo(char *buf);
	void GetDefServer(char *FileName, char *serverip, int serverip_len);
	void SetDefServer(char *FileName, char *serverip);

// Dialog Data
	//{{AFX_DATA(CRegsterDialog)
	enum { IDD = IDD_DIALOG_REG };
	CComboBox	m_users;
	CComboBox	m_servers;
	CComboBox	m_rmDisk;
	CWBButton	m_getPWD;
	CWBButton	m_guest;
	CWBButton	m_reg;
	CWBButton	m_login;
	CString	m_password;
	CString	m_username;
	BOOL	m_remember;
	BOOL	m_autolog;
	BOOL	m_offline;
	CString	m_SDCard;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegsterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegsterDialog)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeComboServer();
	afx_msg void OnSelchangeComboUsername();
	afx_msg void OnLogin();
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonGuest();
	afx_msg void OnButtonGetpwd();
	afx_msg void OnCheckSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGSTERDIALOG_H__FF369105_1B6D_4387_81C6_CA0FA181FBF7__INCLUDED_)
