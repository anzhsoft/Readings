#if !defined(AFX_SPLITWND_H__E4AF8509_37FD_4894_BC3D_3FD9CE4C1E79__INCLUDED_)
#define AFX_SPLITWND_H__E4AF8509_37FD_4894_BC3D_3FD9CE4C1E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplitWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitWnd frame

class CSplitWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(CSplitWnd)
public:
	CSplitWnd();           // protected constructor used by dynamic creation
	virtual ~CSplitWnd();

// Attributes
public:
	CRect m_btnRect[2];
	HCURSOR m_hand;

// Operations
public:
	void Right2Right();
	void Right2Left();
	void Left2Right();
	void Left2Left();
	UINT GetSplitBar(CRect rect);
	BOOL IsLeft();
	BOOL IsRight();
	void DrawLine(CDC *pDC, UINT nBar);
	void OnDrawSplitter(CDC* pDC, ESplitType nType,const CRect& rectArg);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSplitWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITWND_H__E4AF8509_37FD_4894_BC3D_3FD9CE4C1E79__INCLUDED_)
