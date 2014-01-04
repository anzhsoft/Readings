#if !defined(AFX_ROWSPLITWND_H__1F3B8E71_B578_452B_8B6A_2613B2B7CC3A__INCLUDED_)
#define AFX_ROWSPLITWND_H__1F3B8E71_B578_452B_8B6A_2613B2B7CC3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RowSplitWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRowSplitWnd window

class CRowSplitWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(CRowSplitWnd)
// Construction
public:
	CRowSplitWnd();
	virtual ~CRowSplitWnd();

// Attributes
public:
	HCURSOR m_hand;
	CRect m_btnRect;
// Operations
public:
	BOOL IsUp();
	BOOL IsDown();
	void DrawLine(CDC *pDC);
	void OnDrawSplitter(CDC* pDC, ESplitType nType,const CRect& rectArg);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRowSplitWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ToBottom();
	void ToTop();
	void ToMid();
	void ToUp();
	void ToDown();
	// Generated message map functions
protected:
	//{{AFX_MSG(CRowSplitWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROWSPLITWND_H__1F3B8E71_B578_452B_8B6A_2613B2B7CC3A__INCLUDED_)
