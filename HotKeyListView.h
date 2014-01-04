#if !defined(AFX_HOTKEYLISTVIEW_H__4E055272_2017_46E0_B5CA_3B6ADDF89BCC__INCLUDED_)
#define AFX_HOTKEYLISTVIEW_H__4E055272_2017_46E0_B5CA_3B6ADDF89BCC__INCLUDED_

#include "CommLog.h"	// Added by ClassView


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HotKeyListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHotKeyListView view

class CHotKeyListView : public CListView
{
protected:
	DECLARE_DYNCREATE(CHotKeyListView)
public:
	CHotKeyListView();           // protected constructor used by dynamic creation

// Attributes
public:
	CSocketList socklist;
	BOOL quit;
	CWinThread* thGetKey;
//	CDigHotkey m_hotDlg;

	int m_mouseOn;
	int m_mouseOn_old;

// Operations
public:
	CCommLog m_hotKeyLog;
	void GetHotKeyWord();
	static UINT _GetHotKeyWord(LPVOID pParam);
	void DrawRow(CDC* pDC, int nItem, CRect rect, COLORREF textColor, COLORREF bgColor, DWORD data);
	BOOL Create(CWnd *pWnd,CRect rect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotKeyListView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void RefreshHotkey();
	int showFlag;		//0: 最新 1：最热 2：推荐
	virtual ~CHotKeyListView();
protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHotKeyListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CFont ctlfont;
	CFont strfont;
	CFont selfont;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTKEYLISTVIEW_H__4E055272_2017_46E0_B5CA_3B6ADDF89BCC__INCLUDED_)
