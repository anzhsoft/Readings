#if !defined(AFX_NAMETABCTRL_H__7990B52A_5F1D_4056_9031_C57F8099409A__INCLUDED_)
#define AFX_NAMETABCTRL_H__7990B52A_5F1D_4056_9031_C57F8099409A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NameTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNameTabCtrl window

class CNameTabCtrl : public CTabCtrl
{
// Construction
public:
	CNameTabCtrl();

// Attributes
public:

// Operations
public:
	BOOL InsertItem(LPCTSTR lpszItem);
	CString GetItemText(int nItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNameTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	int Find(LPCSTR name);
	virtual ~CNameTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNameTabCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMETABCTRL_H__7990B52A_5F1D_4056_9031_C57F8099409A__INCLUDED_)
