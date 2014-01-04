#if !defined(AFX_HOTKEYNEW_H__90C2782B_E66C_4A06_A6AC_46EF31172FFC__INCLUDED_)
#define AFX_HOTKEYNEW_H__90C2782B_E66C_4A06_A6AC_46EF31172FFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HotkeyNew.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHotkeyNew dialog

class CHotkeyNew : public CDialog
{
// Construction
public:
	CHotkeyNew(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHotkeyNew)
	enum { IDD = IDD_DIALOG_TABNEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHotkeyNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHotkeyNew)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOTKEYNEW_H__90C2782B_E66C_4A06_A6AC_46EF31172FFC__INCLUDED_)
