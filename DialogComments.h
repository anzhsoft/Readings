#if !defined(AFX_DIALOGCOMMENTS_H__10C9FDA2_C4F7_48EB_835F_13FD63F9F1DD__INCLUDED_)
#define AFX_DIALOGCOMMENTS_H__10C9FDA2_C4F7_48EB_835F_13FD63F9F1DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogComments.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogComments dialog

class CDialogComments : public CDialog
{
// Construction
public:
	BOOL OtherComments();
	CString resourcename;
	CString name;
	long sid;
	CDialogComments(CWnd* pParent = NULL);   // standard constructor
	CDialogComments(CString title, int id, CString username, LPCTSTR lpszTemplateName="",CWnd* pParent = NULL);
	CDialogComments(int id,CWnd* pParent = NULL);
	// Dialog Data
	//{{AFX_DATA(CDialogComments)
	enum { IDD = IDD_DIALOG_COMMENTS };
	CString	m_comments;
	CString	m_mycomments;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogComments)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogComments)
	afx_msg void OnButtonSubmit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCOMMENTS_H__10C9FDA2_C4F7_48EB_835F_13FD63F9F1DD__INCLUDED_)
