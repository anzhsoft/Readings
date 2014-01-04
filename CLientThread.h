#if !defined(AFX_CLIENTTHREAD_H__900E73B0_1073_4BC0_8695_65CEF799F08C__INCLUDED_)
#define AFX_CLIENTTHREAD_H__900E73B0_1073_4BC0_8695_65CEF799F08C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CLientThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CCLientThread thread

class CCLientThread : public CWinThread
{
	DECLARE_DYNCREATE(CCLientThread)
protected:
	CCLientThread();           // protected constructor used by dynamic creation

// Attributes
public:
	BOOL* m_pQuit;
	CWnd* m_pWnd;
	UDTSOCKET m_s;
	UDTSOCKET m_sl;
	CString m_UserName;
	CString m_Msg;
	CString m_ip;
	int m_tranport;
	

// Operations
public:
	void MessagePostToDialog(UINT msg, LPCSTR buffer);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLientThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCLientThread();
	void SendShareInfo(LPSTR buffer);
	BOOL SendUserShareInfoQuery();
	// Generated message map functions
	//{{AFX_MSG(CCLientThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTTHREAD_H__900E73B0_1073_4BC0_8695_65CEF799F08C__INCLUDED_)
