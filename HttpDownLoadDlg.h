#if !defined(AFX_HTTPDOWNLOADDLG_H__DAB66AB4_AD3C_4511_BABF_F92E042C8309__INCLUDED_)
#define AFX_HTTPDOWNLOADDLG_H__DAB66AB4_AD3C_4511_BABF_F92E042C8309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HttpDownLoadDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHttpDownLoadDlg dialog

class CHttpDownLoadDlg : public CDialog
{
// Construction
public:
	BOOL DownLoadThread();
	static UINT _DownLoadThread(LPVOID pParam);
	CHttpDownLoadDlg(CWnd* pParent = NULL);   // standard constructor

	CString m_URL;
	CString m_Path;
	SOCKET sock;
	int nDot;
	BOOL quit;

// Dialog Data
	//{{AFX_DATA(CHttpDownLoadDlg)
	enum { IDD = IDD_DIALOG_HTTPDOWN };
	CStatic	m_ctrTip;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpDownLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHttpDownLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnCloseDialog(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPDOWNLOADDLG_H__DAB66AB4_AD3C_4511_BABF_F92E042C8309__INCLUDED_)
