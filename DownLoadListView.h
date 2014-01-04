#if !defined(AFX_DOWNLOADLISTVIEW_H__85091D95_A4F3_4549_B4B5_445D801788E6__INCLUDED_)
#define AFX_DOWNLOADLISTVIEW_H__85091D95_A4F3_4549_B4B5_445D801788E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownLoadListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownLoadListView view

class CDownLoadListView : public CListView
{
protected:
	CDownLoadListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDownLoadListView)

// Attributes
public:
	CList<DownLoadMissionInfo*,DownLoadMissionInfo*&> missioninfolist;
	CListCtrl* m_pList;
	BOOL m_quit;
	HANDLE thSev;
	
	CThreadList m_threadList;
	CSocketList m_sockList;
	CMenu m_menu;
	int  listitem;//list½Úµã

// Operations
public:
	static unsigned __stdcall _DownLoadNetBookByBT(LPVOID pParam);
	BOOL DeleteFromMissionsave(DownLoadMissionInfo* minfo);
	int SendRequest(DownLoadMissionInfo* missoinInfo);

	static unsigned __stdcall _DownLoadNetBookByEmail(LPVOID pParam);
	static unsigned __stdcall _HttpDown(LPVOID pParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownLoadListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDownLoadListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDownLoadListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnFileDownload(WPARAM w,LPARAM l);
	afx_msg LRESULT OnNewDownload(WPARAM w,LPARAM l);
	afx_msg LRESULT OnUpdateDownload(WPARAM w,LPARAM l);
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnDownloadFinished(WPARAM w,LPARAM l);
	afx_msg LRESULT OnThreadFinished(WPARAM w,LPARAM l);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnP2PFileDownloadError(WPARAM w,LPARAM l);
	afx_msg LRESULT OnSocketFinished(UINT socket, LONG type);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuStopdownload();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADLISTVIEW_H__85091D95_A4F3_4549_B4B5_445D801788E6__INCLUDED_)
