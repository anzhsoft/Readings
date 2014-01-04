// ServerPg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ServerPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char CurrentPath[_MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CServerPg dialog


CServerPg::CServerPg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerPg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerPg)
	m_tranport = 0;
	m_sevport = 0;
	m_ctrport = 0;
	//}}AFX_DATA_INIT
}


void CServerPg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerPg)
	DDX_Control(pDX, IDC_IPADDRESS, m_ip);
	DDX_Text(pDX, IDC_EDIT_TRAN_PORT, m_tranport);
	DDX_Text(pDX, IDC_EDIT_SEV_PORT, m_sevport);
	DDX_Text(pDX, IDC_EDIT_CTR_PORT, m_ctrport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerPg, CDialog)
	//{{AFX_MSG_MAP(CServerPg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerPg message handlers

BOOL CServerPg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitialState();
	ChangeState();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerPg::InitialState()
{
	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);

	char ip[50] = {0};
	GetPrivateProfileString("server", "ip", NULL, ip, 50, iniPath);
/*//zuoss 20081217
	CString sIPAddress;
	struct hostent FAR *lpHostEnt=gethostbyname("readings.vicp.net");
	if(lpHostEnt == NULL)
	{
		sIPAddress = _T("");
	}
	LPSTR lpaddr = lpHostEnt->h_addr_list[0];
	if(lpaddr)
	{
		struct in_addr inAddr;
		memmove (&inAddr,lpaddr, 4);
		sIPAddress=inet_ntoa (inAddr);
		if(sIPAddress.IsEmpty())
			sIPAddress=_T("Not available");
	} 
	strcpy(ip,sIPAddress);
	//AfxMessageBox(sIPAddress);
//end*/
	m_ip.SetAddress(ntohl(inet_addr(ip)));

	char val[10] = {0};
	GetPrivateProfileString("server", "port", NULL, val, 10, iniPath);
	m_sevport = atoi(val);

	ZeroMemory(val, 10);
	GetPrivateProfileString("client", "ctrport", NULL, val, 10, iniPath);
	m_ctrport = atoi(val);

	ZeroMemory(val, 10);
	GetPrivateProfileString("client", "tranport", NULL, val, 10, iniPath);
	m_tranport = atoi(val);

	UpdateData(FALSE);
}

void CServerPg::ChangeState()
{

}

void CServerPg::SaveInfo()
{
	UpdateData(TRUE);

	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);

	char ip[50] = {0};
	BYTE i0,i1,i2,i3;
	m_ip.GetAddress(i0,i1,i2,i3);
	sprintf_s(ip,  50, "%d.%d.%d.%d", i0,i1,i2,i3);
	WritePrivateProfileString("server","ip",ip,iniPath);
	
	char val[10] = {0};
	sprintf_s(val, 10, "%d", m_sevport);
	WritePrivateProfileString("server","port",val,iniPath);

	ZeroMemory(val, 10);
	sprintf_s(val, 10, "%d", m_ctrport);
	WritePrivateProfileString("client","ctrport",val,iniPath);

	ZeroMemory(val, 10);
	sprintf_s(val, 10, "%d", m_tranport);
	WritePrivateProfileString("client","tranport",val,iniPath);
}
