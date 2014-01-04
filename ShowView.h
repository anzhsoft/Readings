#if !defined(AFX_SHOWVIEW_H__CDB99B6E_32A4_4D67_94FC_F75ADF524020__INCLUDED_)
#define AFX_SHOWVIEW_H__CDB99B6E_32A4_4D67_94FC_F75ADF524020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowView view
#include "ShowDialog.h"
#include "ReadDlg.h"

class CShowView : public CView
{
protected:
	CShowView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CShowView)

// Attributes
public:
	CShowDialog m_showDialog;
	CReadDlg m_readDialog;

// Operations
public:
	void AddToRecentlyRecord(CString FilePath, int filetype);
	void OpenURL(CString URL);
	void OpenFile(CString FilePath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CShowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CShowView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnUpPage(WPARAM w,LPARAM l);
	afx_msg LRESULT OnDownPage(WPARAM w,LPARAM l);
	afx_msg LRESULT OnZoom(WPARAM w,LPARAM l);
	afx_msg LRESULT OnMenu(WPARAM w,LPARAM l);
	afx_msg void OnSelchangedTreeCatalog(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWVIEW_H__CDB99B6E_32A4_4D67_94FC_F75ADF524020__INCLUDED_)
