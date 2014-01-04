#if !defined(AFX_CLIENTLISTENERTHREAD_H__57324704_32B1_44EC_B985_BCA426B25FD3__INCLUDED_)
#define AFX_CLIENTLISTENERTHREAD_H__57324704_32B1_44EC_B985_BCA426B25FD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientListenerThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClientListenerThread thread

class CClientListenerThread : public CWinThread
{
	DECLARE_DYNCREATE(CClientListenerThread)
protected:
	CClientListenerThread();           // protected constructor used by dynamic creation

// Attributes
public:
	BOOL m_quit;
	CWnd* m_pWnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientListenerThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CClientListenerThread();

	// Generated message map functions
	//{{AFX_MSG(CClientListenerThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTLISTENERTHREAD_H__57324704_32B1_44EC_B985_BCA426B25FD3__INCLUDED_)
