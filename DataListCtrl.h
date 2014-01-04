#if !defined(AFX_DATALISTCTRL_H__EB9B7431_5F28_49E9_87D2_C2B0FBCA1611__INCLUDED_)
#define AFX_DATALISTCTRL_H__EB9B7431_5F28_49E9_87D2_C2B0FBCA1611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataListCtrl window

class CDataListCtrl : public CListCtrl
{
// Construction
public:
	CDataListCtrl();

// Attributes
public:

// Operations
public:
	BOOL DeleteAllItems();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDataListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALISTCTRL_H__EB9B7431_5F28_49E9_87D2_C2B0FBCA1611__INCLUDED_)
