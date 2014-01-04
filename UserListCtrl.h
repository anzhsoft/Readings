#if !defined(AFX_USERLISTCTRL_H__9B235BBA_42CB_4BC3_A0A8_411BB5852096__INCLUDED_)
#define AFX_USERLISTCTRL_H__9B235BBA_42CB_4BC3_A0A8_411BB5852096__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserListCtrl window

class CUserListCtrl : public CListCtrl
{
// Construction
public:
	CUserListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserListCtrl)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void RemoveImage(int nItem);
	void DeleteImageList();
	BOOL CreateImageList();
	BOOL SetImage(UINT icon, int nItem);
	void Draw(CDC* pDC, int nItem, CRect rect, COLORREF txtColor, COLORREF bgColor);
	virtual ~CUserListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUserListCtrl)
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERLISTCTRL_H__9B235BBA_42CB_4BC3_A0A8_411BB5852096__INCLUDED_)
