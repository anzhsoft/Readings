// AddNetBookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "AddNetBookDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddNetBookDlg dialog


CAddNetBookDlg::CAddNetBookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddNetBookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddNetBookDlg)
	m_netBookName = _T("");
	m_netBookURL = _T("");
	//}}AFX_DATA_INIT
}

void CAddNetBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddNetBookDlg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_netBookName);
	DDX_Text(pDX, IDC_EDIT_URL, m_netBookURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddNetBookDlg, CDialog)
	//{{AFX_MSG_MAP(CAddNetBookDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADDNETBOOK, OnButtonAddnetbook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddNetBookDlg message handlers

CAddNetBookDlg::CAddNetBookDlg(CString name,CString url,CWnd* pParent /*=NULL*/)
	: CDialog(CAddNetBookDlg::IDD, pParent)
{
	m_netBookName = _T(name);
	m_netBookURL = _T(url);
}

void CAddNetBookDlg::OnButtonAddnetbook() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	m_name =  m_netBookName;
//	m_url = m_netBookURL;

	CDialog::OnOK();
}

