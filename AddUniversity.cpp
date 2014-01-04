// AddUniversity.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "AddUniversity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AddUniversity dialog


AddUniversity::AddUniversity(CWnd* pParent /*=NULL*/)
	: CDialog(AddUniversity::IDD, pParent)
{
	//{{AFX_DATA_INIT(AddUniversity)
	m_desc = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT
}


void AddUniversity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AddUniversity)
	DDX_Text(pDX, IDC_UDESC_EDIT, m_desc);
	DDX_Text(pDX, IDC_UNAME_EDIT, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AddUniversity, CDialog)
	//{{AFX_MSG_MAP(AddUniversity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AddUniversity message handlers

void AddUniversity::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
