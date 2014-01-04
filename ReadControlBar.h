#if !defined(AFX_READCONTROLBAR_H__2C8BE343_7741_46BA_B69F_900141AF4D19__INCLUDED_)
#define AFX_READCONTROLBAR_H__2C8BE343_7741_46BA_B69F_900141AF4D19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReadControlBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReadControlBar dialog
#include "control/WBButton.h"

class CReadControlBar : public CDialog
{
// Construction
public:
	CReadControlBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReadControlBar)
	enum { IDD = IDD_CONTROLBAR };
	CWBButton	m_zoom;
	CWBButton	m_menu;
	CWBButton	m_right;
	CWBButton	m_left;
	CString	m_page;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReadControlBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReadControlBar)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpPage();
	afx_msg void OnDownPage();
	afx_msg void OnZoom();
	afx_msg void OnMenu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READCONTROLBAR_H__2C8BE343_7741_46BA_B69F_900141AF4D19__INCLUDED_)
