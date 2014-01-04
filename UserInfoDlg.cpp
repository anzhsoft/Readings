// UserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "UserInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg dialog


CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserInfoDlg)
	m_city = _T("");
	m_ip = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT
}


CUserInfoDlg::CUserInfoDlg(STRUCTUSERINFO info,CWnd* pParent /* = NULL */)
		:CDialog(CUserInfoDlg::IDD,pParent)
{
	userinfo = info;
	m_city = _T("");
	m_ip = _T("");
	m_name = _T("");
}
void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserInfoDlg)
	DDX_Text(pDX, IDC_STATIC_CITY, m_city);
	DDX_Text(pDX, IDC_STATIC_IP, m_ip);
	DDX_Text(pDX, IDC_STATIC_NAME, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CUserInfoDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg message handlers


int CUserInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CUserInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_name = userinfo.UserName;
	m_city = userinfo.NetName;
	m_ip = userinfo.IP;

//	m_name.Format("%s",userinfo.UserName);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
