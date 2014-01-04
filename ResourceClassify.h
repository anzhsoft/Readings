#if !defined(AFX_RESOURCECLASSIFY_H__C08A5C60_7A45_45D5_B3D8_99FBC6389A75__INCLUDED_)
#define AFX_RESOURCECLASSIFY_H__C08A5C60_7A45_45D5_B3D8_99FBC6389A75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ResourceClassify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ResourceClassify dialog

class ResourceClassify : public CDialog
{
// Construction
public:
		HINSTANCE hDll;
	BOOL UpdateGroupFile();
	int gID[3];
	BOOL CopyTree(HTREEITEM Or, HTREEITEM New);
	BOOL InitialImageList();

	int sid;
	char * class1;
	HTREEITEM GroupRoot;
	ResourceClassify(CWnd* pParent = NULL);   // standard constructor
	ResourceClassify(const char * group, int id, HTREEITEM hAllGroup, CWnd* pParent=NULL);

// Dialog Data
	//{{AFX_DATA(ResourceClassify)
	enum { IDD = IDD_DIALOG_CLASSIFY };
	CListBox	m_typelist;
	CButton	m_del;
	CTreeCtrl	m_pTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResourceClassify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ResourceClassify)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnButtonDel();
	afx_msg void OnSelchangedTreeGroup(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCECLASSIFY_H__C08A5C60_7A45_45D5_B3D8_99FBC6389A75__INCLUDED_)
