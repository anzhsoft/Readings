#if !defined(AFX_MIDROWSPLITWND_H__588E49A4_8B35_414D_B658_2FBCB2C248C1__INCLUDED_)
#define AFX_MIDROWSPLITWND_H__588E49A4_8B35_414D_B658_2FBCB2C248C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidRowSplitWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMidRowSplitWnd window

class CMidRowSplitWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(CMidRowSplitWnd)	
// Construction
public:
	CMidRowSplitWnd();
	virtual ~CMidRowSplitWnd();

// Attributes
public:	
	HCURSOR m_hand;
	CRect m_btnRect;
	CRect m_btnRect2;
// Operations
public:
	BOOL IsUp();
	BOOL IsDown();
	void DrawLine(CDC *pDC);
	void OnDrawSplitter(CDC* pDC, ESplitType nType,const CRect& rectArg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidRowSplitWnd)
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
	//{{AFX_MSG(CMidRowSplitWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDROWSPLITWND_H__588E49A4_8B35_414D_B658_2FBCB2C248C1__INCLUDED_)
