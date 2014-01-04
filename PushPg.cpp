// PushPg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "PushPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char CurrentPath[_MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CPushPg dialog


CPushPg::CPushPg(CWnd* pParent /*=NULL*/)
	: CDialog(CPushPg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPushPg)
	m_bPush = FALSE;
	m_bReadman = FALSE;
	//}}AFX_DATA_INIT
}


void CPushPg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPushPg)
	DDX_Check(pDX, IDC_CHECK_PUSH, m_bPush);
	DDX_Check(pDX, IDC_CHECK_READMAN, m_bReadman);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPushPg, CDialog)
	//{{AFX_MSG_MAP(CPushPg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPushPg message handlers

void CPushPg::InitialState()
{
	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);

	char val[50] = {0};
	GetPrivateProfileString("push", "Readman", NULL, val, 50, iniPath);
	m_bReadman = !atoi(val);

	ZeroMemory(val,50);
	GetPrivateProfileString("push", "resource", NULL, val, 50, iniPath);
	m_bPush = !atoi(val);

	UpdateData(FALSE);
}

BOOL CPushPg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitialState();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPushPg::SaveInfo()
{
	UpdateData(TRUE);

	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);

	char yes[10] = {0};
	char no[10] = {0};
	strcpy_s(yes, 10, "1");
	strcpy_s(no, 10, "0");

	WritePrivateProfileString("push", "Readman", !m_bReadman?yes:no, iniPath);
	WritePrivateProfileString("push", "resource", !m_bPush?yes:no, iniPath);
}
