#if !defined(AFX_FAVORPG_H__09454DA0_4153_419C_BB05_C0D05D59AA45__INCLUDED_)
#define AFX_FAVORPG_H__09454DA0_4153_419C_BB05_C0D05D59AA45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FavorPg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFavorPg dialog

class CFavorPg : public CDialog
{
// Construction
public:
	void SaveInfo();
	void ChangeState();
	void InitialState();
	CFavorPg(CWnd* pParent = NULL);   // standard constructor


	UINT collect_style;
	UINT clear_style;

	enum { not_clear=0, clear_pdf, clear_all, };
	enum { collect_all=0, collect_spec, };

// Dialog Data
	//{{AFX_DATA(CFavorPg)
	enum { IDD = IDD_CONFIGPAGE_FAVOR };
	CListBox	m_collectList;
	CString	m_favorFolder;
	UINT	m_allday;
	UINT	m_papday;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFavorPg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFavorPg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFavorfolderBrowse();
	afx_msg void OnRadioNever();
	afx_msg void OnRadioPdfAuto();
	afx_msg void OnRadioAllAuto();
	afx_msg void OnRadioAlldisk();
	afx_msg void OnRadioSpecdisk();
	afx_msg void OnButtonAddCollectfolder();
	afx_msg void OnSelchangeListCollect();
	afx_msg void OnButtonDelCollectfolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAVORPG_H__09454DA0_4153_419C_BB05_C0D05D59AA45__INCLUDED_)
