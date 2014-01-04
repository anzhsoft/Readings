// CreateRGDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "CreateRGDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateRGDlg dialog


CCreateRGDlg::CCreateRGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateRGDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateRGDlg)
	m_title = _T("");
	//}}AFX_DATA_INIT
}


void CCreateRGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateRGDlg)
	DDX_Text(pDX, IDC_EDIT_RGTITLE, m_title);
	DDV_MaxChars(pDX, m_title, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateRGDlg, CDialog)
	//{{AFX_MSG_MAP(CCreateRGDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateRGDlg message handlers
