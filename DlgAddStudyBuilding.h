#if !defined(AFX_DLGADDSTUDYBUILDING_H__C2F95847_1282_4DCD_A4AD_C74569459721__INCLUDED_)
#define AFX_DLGADDSTUDYBUILDING_H__C2F95847_1282_4DCD_A4AD_C74569459721__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddStudyBuilding.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddStudyBuilding dialog

class CDlgAddStudyBuilding : public CDialog
{
// Construction
public:
	CDlgAddStudyBuilding(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddStudyBuilding)
	enum { IDD = IDD_DIALOG_ADDSTUDYBUILDING };
	CString	m_desc;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddStudyBuilding)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddStudyBuilding)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDSTUDYBUILDING_H__C2F95847_1282_4DCD_A4AD_C74569459721__INCLUDED_)
