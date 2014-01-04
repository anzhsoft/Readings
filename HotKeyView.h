#if !defined(AFX_HOTKEYVIEW_H__CD9E2ACD_2AB8_488D_9CDA_5D340D7FA399__INCLUDED_)
#define AFX_HOTKEYVIEW_H__CD9E2ACD_2AB8_488D_9CDA_5D340D7FA399__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HotKeyView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHotKeyView view
#include "DigHotkey.h"

class CHotKeyView : public CView
{
protected:
	           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHotKeyView)

// Attributes
public:

// Operations
public:
	CHotKeyView();
	CDigHotkey m_hotDlg;
	BOOL Create(CWnd *pWnd,CRect rect);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotKeyView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHotKeyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHotKeyView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTKEYVIEW_H__CD9E2ACD_2AB8_488D_9CDA_5D340D7FA399__INCLUDED_)
