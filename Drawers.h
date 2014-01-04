#if !defined(AFX_DRAWERS_H__6861D5AF_EE67_4A08_851A_5A99888B4D4F__INCLUDED_)
#define AFX_DRAWERS_H__6861D5AF_EE67_4A08_851A_5A99888B4D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Drawers.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawers dialog

class CDrawers : public CDialog
{
	
// Construction
public:
	
	CDrawers(CWnd* pParent = NULL);   // standard constructor
	static unsigned __stdcall _CreateDrawers(LPVOID pParam);
	CGfxOutBarCtrl	wndBar;
	

// Dialog Data
	//{{AFX_DATA(CDrawers)
	enum { IDD = IDD_drawers };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawers)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawers)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWERS_H__6861D5AF_EE67_4A08_851A_5A99888B4D4F__INCLUDED_)
