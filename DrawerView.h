#if !defined(AFX_DRAWERVIEW_H__5999CE07_2E6E_4787_A2A4_7C5660C12028__INCLUDED_)
#define AFX_DRAWERVIEW_H__5999CE07_2E6E_4787_A2A4_7C5660C12028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawerView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawerView view

class CDrawerView : public CView
{
protected:
	CDrawerView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDrawerView)

// Attributes
public:

// Operations
public:
	CDrawers cd;
	static UINT _CreateDrawers(LPVOID pParam);
	CWinThread* thGetKey;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawerView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDrawerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawerView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWERVIEW_H__5999CE07_2E6E_4787_A2A4_7C5660C12028__INCLUDED_)
