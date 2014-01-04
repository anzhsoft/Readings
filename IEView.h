#if !defined(AFX_IEVIEW_H__C3413ABA_6343_4BC8_8B16_71703EF786C0__INCLUDED_)
#define AFX_IEVIEW_H__C3413ABA_6343_4BC8_8B16_71703EF786C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IEView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIEView html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CIEView : public CHtmlView
{
protected:
	CIEView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CIEView)

// html Data
public:
	//{{AFX_DATA(CIEView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CIEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CIEView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEVIEW_H__C3413ABA_6343_4BC8_8B16_71703EF786C0__INCLUDED_)
