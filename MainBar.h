#if !defined(AFX_MAINBAR_H__167749E9_25B4_4A66_83BA_4DFF7CCF14AE__INCLUDED_)
#define AFX_MAINBAR_H__167749E9_25B4_4A66_83BA_4DFF7CCF14AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainBar dialog
#include "control/AdvancedBitmapButton.h"
#include "RequestMail.h"
#include <afxtempl.h>
class CMainBar : public CDialogBar
{
// Construction
public:
	void Search(CString key);
	CMainBar();   // standard constructor

	BOOL m_bSearch;
	BOOL m_bopen;
// Dialog Data
	//{{AFX_DATA(CMainBar)
	enum { IDD = IDD_MAINBAR };
	CAdvancedBitmapButton	m_mail;
	CAdvancedBitmapButton	m_open_close;
	CAdvancedBitmapButton	m_userAdv;
	CAdvancedBitmapButton	m_history;
	CAdvancedBitmapButton	m_help;
	CAdvancedBitmapButton	m_search;
	CAdvancedBitmapButton	m_searchstr;
	CAdvancedBitmapButton	m_orderlist;
	CAdvancedBitmapButton	m_syn;
	CAdvancedBitmapButton	m_refresh;
	CString	m_keyword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainBar)
	afx_msg LRESULT OnInitDialog(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateRefresh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOrderList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSyn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHistory(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUserAdv(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHelp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSearch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOpenClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMail(CCmdUI* pCmdUI);
	afx_msg void OnButtonSearch();
	afx_msg LRESULT OnSearchFinished(WPARAM w,LPARAM l);
	afx_msg void OnButtonOpenClose();
	afx_msg void OnButtonMail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINBAR_H__167749E9_25B4_4A66_83BA_4DFF7CCF14AE__INCLUDED_)
