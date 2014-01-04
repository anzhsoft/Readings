// DlgAddStudyBuilding.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "DlgAddStudyBuilding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddStudyBuilding dialog


CDlgAddStudyBuilding::CDlgAddStudyBuilding(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddStudyBuilding::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddStudyBuilding)
	m_desc = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT
}


void CDlgAddStudyBuilding::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddStudyBuilding)
	DDX_Text(pDX, IDC_EDIT_DESC, m_desc);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddStudyBuilding, CDialog)
	//{{AFX_MSG_MAP(CDlgAddStudyBuilding)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddStudyBuilding message handlers

void CDlgAddStudyBuilding::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
