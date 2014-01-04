#if !defined(AFX_DIGHOTKEY_H__8356F99F_FFB6_4C23_AED9_B24DF10950F3__INCLUDED_)
#define AFX_DIGHOTKEY_H__8356F99F_FFB6_4C23_AED9_B24DF10950F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DigHotkey.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDigHotkey dialog
#include "HotkeyNew.h"
#include "HotKeyListView.h"	// Added by ClassView

class CDigHotkey : public CDialog
{
// Construction
public:
	CDigHotkey(CWnd* pParent = NULL);   // standard constructor
//	BOOL Create(CWnd *pWnd,CRect rect);
	CHotKeyListView* m_hotlistview;
// Dialog Data
	//{{AFX_DATA(CDigHotkey)
	enum { IDD = IDD_DIALOG_HOTKEY };
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigHotkey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDigHotkey)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTabMostnew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGHOTKEY_H__8356F99F_FFB6_4C23_AED9_B24DF10950F3__INCLUDED_)
