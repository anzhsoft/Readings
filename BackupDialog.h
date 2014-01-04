#if !defined(AFX_BACKUPDIALOG_H__E53238C0_9F1C_49D2_AC3E_0AB50882319A__INCLUDED_)
#define AFX_BACKUPDIALOG_H__E53238C0_9F1C_49D2_AC3E_0AB50882319A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackupDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackupDialog dialog

class CBackupDialog : public CDialog
{
// Construction
public:
	static UINT _Thread(LPVOID pParam);
	CBackupDialog(CWnd* pParent = NULL);   // standard constructor
	void PowerCopy(CString src, CString dst, BOOL& quit);

	CString sourcePath;
	CString destPath;
	__int64 used;
	__int64 finished;
	BOOL q;
	CWinThread* th;
	DWORD start;

// Dialog Data
	//{{AFX_DATA(CBackupDialog)
	enum { IDD = IDD_DIALOG_BACKUP };
	CStatic	m_tm;
	CStatic	m_tip;
	CProgressCtrl	m_progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBackupDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnFinished(WPARAM,LPARAM);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKUPDIALOG_H__E53238C0_9F1C_49D2_AC3E_0AB50882319A__INCLUDED_)
