// DialogMail.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "DialogMail.h"
#include "sms.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMail dialog


CDialogMail::CDialogMail(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMail)
	issms = FALSE;
	m_resource_name = _T("");
	//}}AFX_DATA_INIT
}


void CDialogMail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMail)
	DDX_Check(pDX, IDC_ISSMS, issms);
	DDX_Text(pDX, IDC_RESOURCE_NAME, m_resource_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMail, CDialog)
	//{{AFX_MSG_MAP(CDialogMail)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMail message handlers

void CDialogMail::OnOK() 
{	
	this->UpdateData();
	CDialog::OnOK();
}

BOOL CDialogMail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_resource_name=resource;
	this->UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


