// NormalPg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "NormalPg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char CurrentPath[_MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CNormalPg dialog

CNormalPg::CNormalPg(CWnd* pParent /*=NULL*/)
	: CDialog(CNormalPg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNormalPg)
	m_day = 1;
	m_bNewVer = FALSE;
	m_bAutoLogin = FALSE;
	m_bAutoRun = FALSE;
	m_bQuitAlert = FALSE;
	m_bShowMin = FALSE;
	//}}AFX_DATA_INIT
}

void CNormalPg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNormalPg)
	DDX_Control(pDX, IDC_SPIN, m_spin);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_day);
	DDV_MinMaxUInt(pDX, m_day, 1, 30);
	DDX_Check(pDX, IDC_CHECK_NEWVERSION, m_bNewVer);
	DDX_Check(pDX, IDC_CHECK_AUTOLOGIN, m_bAutoLogin);
	DDX_Check(pDX, IDC_CHECK_AUTORUN, m_bAutoRun);
	DDX_Check(pDX, IDC_CHECK_CONFIRM_QUIT, m_bQuitAlert);
	DDX_Check(pDX, IDC_CHECK_SHOWMIN, m_bShowMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNormalPg, CDialog)
	//{{AFX_MSG_MAP(CNormalPg)
	ON_BN_CLICKED(IDC_CHECK_NEWVERSION, OnCheckNewversion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNormalPg message handlers

void CNormalPg::OnCheckNewversion() 
{
	UpdateData(TRUE);
	ChangeState();	
}

BOOL CNormalPg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_spin.SetRange(30,1);
	InitialState();
	ChangeState();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNormalPg::InitialState()
{
	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	char val[50] = {0};
	GetPrivateProfileString("normal", "auto-update", NULL, val, 50, iniPath);
	m_bNewVer = atoi(val);
	if(m_bNewVer){
		ZeroMemory(val, 50);
		GetPrivateProfileString("normal", "update-interval", NULL, val, 50, iniPath);
		m_day = atoi(val);
	}
	ZeroMemory(val, 50);
	GetPrivateProfileString("normal", "showMin", NULL, val, 50, iniPath);
	m_bShowMin = atoi(val);

	ZeroMemory(val, 50);
	GetPrivateProfileString("normal", "autoLogin", NULL, val, 50, iniPath);
	m_bAutoLogin = atoi(val);

	ZeroMemory(val, 50);
	GetPrivateProfileString("normal", "quitAlert", NULL, val, 50, iniPath);
	m_bQuitAlert = atoi(val);

	m_bAutoRun = IsAutoRun();

	UpdateData(FALSE);
}

void CNormalPg::SaveInfo()
{
	UpdateData(TRUE);

	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);

	char yes[10] = {0};
	char no[10] = {0};
	strcpy_s(yes, 10, "1");
	strcpy_s(no, 10, "0");

	WritePrivateProfileString("normal","auto-update",m_bNewVer?yes:no,iniPath);
	if(m_bNewVer){
		char val[10] = {0};
		sprintf_s(val, 10, "%d", m_day);
		WritePrivateProfileString("normal","update-interval",
			m_bNewVer?yes:no,iniPath);
	}

	WritePrivateProfileString("normal","showMin",m_bShowMin?yes:no,iniPath);
	WritePrivateProfileString("normal","autoLogin",m_bAutoLogin?yes:no,iniPath);
	WritePrivateProfileString("normal","quitAlert",m_bQuitAlert?yes:no,iniPath);
	SaveReg();
}

void CNormalPg::ChangeState()
{
	if(m_bNewVer){
		GetDlgItem(IDC_STATIC_TIPS)->EnableWindow();
		GetDlgItem(IDC_STATIC_TIPS2)->EnableWindow();
		GetDlgItem(IDC_EDIT_INTERVAL)->EnableWindow();
		GetDlgItem(IDC_SPIN)->EnableWindow();
	}
	else{
		GetDlgItem(IDC_STATIC_TIPS)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIPS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INTERVAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN)->EnableWindow(FALSE);
	}
}

BOOL CNormalPg::IsAutoRun()
{
	BOOL res = FALSE;
	HKEY hKEY;
	LPCTSTR data_Set = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKEY);
	if(ret==ERROR_SUCCESS){
		LPBYTE setupPath = new BYTE [_MAX_PATH];
		DWORD type = REG_SZ;
		DWORD len = _MAX_PATH;
		ret = ::RegQueryValueEx(hKEY, "Readings", NULL, &type, setupPath, &len);
		if(ret!=ERROR_SUCCESS)
			res = FALSE;
		else
			res = TRUE;
	}
	else
		res = FALSE;
	RegCloseKey(hKEY);    
	return res;
}

void CNormalPg::SaveReg()
{
	HKEY hKEY;
	LPCTSTR data_Set = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	long ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_ALL_ACCESS, &hKEY);
	if(ret==ERROR_SUCCESS){
		char setupPath[_MAX_PATH] = {0};
		sprintf_s(setupPath, _MAX_PATH, "%s\\Readings.exe", CurrentPath);
		DWORD type = REG_SZ;
		DWORD len = _MAX_PATH;
		if(m_bAutoRun){
			ret = ::RegSetValueEx(hKEY, "Readings", 0, type, (CONST BYTE*)setupPath, strlen(setupPath));
		}
		else{
			ret = ::RegDeleteValue(hKEY, "Readings");
		}
	}
	RegCloseKey(hKEY);    
}
