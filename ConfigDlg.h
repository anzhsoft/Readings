#if !defined(AFX_CONFIGDLG_H__9C1D3536_432E_4BCF_8267_5D97F3C16E9D__INCLUDED_)
#define AFX_CONFIGDLG_H__9C1D3536_432E_4BCF_8267_5D97F3C16E9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog
#include "SharePg.h"
#include "NormalPg.h"
#include "FavorPg.h"
#include "ServerPg.h"
#include "PushPg.h"

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	void InitialList();
// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_DIALOG_CONFIG };
	CListCtrl	m_list;
	CSharePg m_sharePg;
	CNormalPg m_normalPg;
	CFavorPg m_favorPg;
	CServerPg m_serverPg;
	CPushPg m_pushPg;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListSel(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLG_H__9C1D3536_432E_4BCF_8267_5D97F3C16E9D__INCLUDED_)
