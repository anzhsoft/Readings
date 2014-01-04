#if !defined(AFX_TIPSDIALOG_H__581D8CFB_DD41_43F2_A39B_3FDC73DD6E8A__INCLUDED_)
#define AFX_TIPSDIALOG_H__581D8CFB_DD41_43F2_A39B_3FDC73DD6E8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TipsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTipsDialog dialog
#include "control/WBButton.h"

class CTipsDialog : public CDialog
{
// Construction
public:
	CTipsDialog(CWnd* pParent = NULL);   // standard constructor
	CFont font;
	CSize sz;
	UINT icon;
	UINT bg;
	UINT style;	//0 tip; 1 alert; 2 confirm
	CWBButton btn;
	CButton chk;
	CString key;
	CString	m_tip;
// Dialog Data
	//{{AFX_DATA(CTipsDialog)
	enum { IDD = IDD_DIALOG_TIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTipsDialog)
	afx_msg void OnClickOk();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPSDIALOG_H__581D8CFB_DD41_43F2_A39B_3FDC73DD6E8A__INCLUDED_)
