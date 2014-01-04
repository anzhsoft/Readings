#if !defined(AFX_COMMVIEW_H__317F706B_54D5_46A0_B9CB_3F1306AFE36A__INCLUDED_)
#define AFX_COMMVIEW_H__317F706B_54D5_46A0_B9CB_3F1306AFE36A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommView view
#include "UserCommDlg.h"

class CCommView : public CView
{
protected:
	CCommView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCommView)

// Attributes
public:
	CUserCommDlg m_CommDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCommView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCommView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMVIEW_H__317F706B_54D5_46A0_B9CB_3F1306AFE36A__INCLUDED_)
