// RegsterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "RegsterDialog.h"
#include "HttpDownLoadDlg.h"
#include "UDiskAuth.h"
#include "AutoLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char CurrentPath[_MAX_PATH];
extern char AspServer[1024];
extern char JspServer[1024];
extern char SimCardID[20];
extern char UserName[50];// added by zhanganzhan on 20090912
extern char Password[50];
extern USB_DETAIL_INFO uDiskAuthInfo;


/////////////////////////////////////////////////////////////////////////////
// CRegsterDialog dialog


CRegsterDialog::CRegsterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRegsterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegsterDialog)
	m_password = _T("");
	m_username = _T("");
	m_remember = FALSE;
	m_autolog = FALSE;
	m_offline = FALSE;
	m_SDCard = _T("");
	//}}AFX_DATA_INIT
	m_pServer = NULL;
}


void CRegsterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegsterDialog)
	DDX_Control(pDX, IDC_COMBO_USERNAME, m_users);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_servers);
	DDX_Control(pDX, IDC_COMBO_SEL_REMOVEDISK, m_rmDisk);
	DDX_Control(pDX, IDC_BUTTON_GETPWD, m_getPWD);
	DDX_Control(pDX, IDC_BUTTON_GUEST, m_guest);
	DDX_Control(pDX, IDC_BUTTON_REG, m_reg);
	DDX_Control(pDX, IDOK, m_login);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 20);
	DDX_CBString(pDX, IDC_COMBO_USERNAME, m_username);
	DDV_MaxChars(pDX, m_username, 30);
	DDX_Check(pDX, IDC_CHECK_SAVE, m_remember);
	DDX_Check(pDX, IDC_CHECK_AUTOLOG, m_autolog);
	DDX_Check(pDX, IDC_CHECK_OFFLINE, m_offline);
	DDX_CBString(pDX, IDC_COMBO_SEL_REMOVEDISK, m_SDCard);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegsterDialog, CDialog)
	//{{AFX_MSG_MAP(CRegsterDialog)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER, OnSelchangeComboServer)
	ON_CBN_SELCHANGE(IDC_COMBO_USERNAME, OnSelchangeComboUsername)
	ON_BN_CLICKED(IDOK, OnLogin)
	ON_BN_CLICKED(IDC_BUTTON_REG, OnButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_GUEST, OnButtonGuest)
	ON_BN_CLICKED(IDC_BUTTON_GETPWD, OnButtonGetpwd)
	ON_BN_CLICKED(IDC_CHECK_SAVE, OnCheckSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegsterDialog message handlers

BOOL CRegsterDialog::OnEraseBkgnd(CDC* pDC) 
{
	CRect	rect;
	GetWindowRect(&rect);

	CDC memDC;
	CBitmap			&cBitmap = bgBmp;
	CBitmap*		pOldMemBmp = NULL;
	
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	if(pOldMemBmp)
		memDC.SelectObject( pOldMemBmp );
	if(memDC.GetSafeHdc())
		memDC.DeleteDC();

	return TRUE;
}

BOOL CRegsterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CheckSDCard();		//���SD��
	LoadServerInfo();	//��ȡ������
	LoadUserInfo();		//��ȡ�û���Ϣ
	LoadAutoLogin();	//��ȡ�Զ���¼��Ϣ

	
	bgBmp.LoadBitmap(IDB_BITMAP_BGREGST);

	m_login.LoadBitmaps(IDB_BITMAP_LOGIN, 4, 1, 1, 1, 4);
	m_reg.LoadBitmaps(IDB_BITMAP_REG, 4, 1, 1, 1, 4);
	m_guest.LoadBitmaps(IDB_BITMAP_GUEST, 4, 1, 1, 1, 4);
	m_getPWD.LoadBitmaps(IDB_BITMAP_GETPWD, 4, 1, 1, 1, 4);

	m_login.SetCursor(IDC_CURSOR);
	m_reg.SetCursor(IDC_CURSOR);
	m_guest.SetCursor(IDC_CURSOR);
	m_getPWD.SetCursor(IDC_CURSOR);

//	OnLogin();
	
/*	CUDiskAuth * udiskAuth = new CUDiskAuth;
	
	int diskNum=m_rmDisk.GetWindowTextLength();
	CString cs;
	m_rmDisk.GetWindowText(cs);
//	AfxMessageBox(cs);
	if (!cs.IsEmpty())
	{
		char chDisk = cs.GetAt(0);
		int cnt=0;
		uDiskAuthInfo.flag = FALSE;
		strcpy(UserName,m_username);
		while (!udiskAuth->GetUSBDiskID(chDisk) && cnt<diskNum)
		{
			chDisk+=1;	
			cnt++;
		}
		if (uDiskAuthInfo.flag)
		{
			CAutoLoginDlg * dlgCB ;//anzhsoftPPPP
			dlgCB = new CAutoLoginDlg;
			dlgCB->Create(IDD_DIALOG_AUTO_LOGIN);
			dlgCB->ShowWindow(SW_RESTORE);
			OnLogin();
		}
	}
	*/ //zhanganzhan deleted for the new edition 091130
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/********************************************************
					����Ƿ���SD������
*********************************************************/
BOOL CRegsterDialog::CheckSDCard()
{
	DWORD driversbit = ::GetLogicalDrives();
	driversbit = driversbit >> 3;
	for(int i = 0; i < 29; i++){
		if(driversbit & 1){
			char driver[10] = {"a:"};
			driver[0] = 'a' + i + 3;
			if(::GetDriveType(driver)==DRIVE_REMOVABLE){
				m_rmDisk.AddString(driver);
			}
		}
		driversbit = driversbit >> 1;
	}
	m_rmDisk.SetCurSel(0);
	return TRUE;
}

/****************************************************
					��ȡ�������б�
*****************************************************/
void CRegsterDialog::LoadServerInfo()
{
	int servernum;
/*	char serverFile[_MAX_PATH] = {0};
	sprintf(serverFile, "%s\\system\\Servers.inf", CurrentPath);
	
	CFile cf;
	if(cf.Open(serverFile, CFile::modeRead))
	{
		servernum = cf.GetLength()/sizeof(DOWNLOADSERVERS);
		if(servernum>0)
		{
			m_pServer = (DOWNLOADSERVERS *)malloc(sizeof(DOWNLOADSERVERS)*servernum);
			ZeroMemory(m_pServer,sizeof(DOWNLOADSERVERS)*servernum);
			cf.Read(m_pServer,sizeof(DOWNLOADSERVERS)*servernum);
			cf.Close();
			for(int i = 0; i < servernum; i++)
			{
				m_servers.AddString(m_pServer[i].Description);
				if(m_pServer[i].DefState == 1)
				{
					m_servers.SetCurSel(i);
					//strcpy(JspServer, m_pServer[i].ServerIP);
					//strcpy(JspServer, "http://readings.vicp.net:8088");
					//strcpy(JspServer,"http://218.68.91.108:8088");
					
				}
			}
			return;
		}
	}*/
	servernum = 1;
	m_pServer = (DOWNLOADSERVERS *)malloc(sizeof(DOWNLOADSERVERS));
	ZeroMemory(m_pServer, sizeof(DOWNLOADSERVERS));
	m_pServer->DefState = 1;
	strcpy_s(m_pServer->Description, 64, "Ĭ�Ϸ�����");
	strcpy_s(m_pServer->ServerIP, 64, DefaultHost);

	//strcpy(JspServer, DefaultHost);
	m_servers.AddString(m_pServer->Description);
	m_servers.SetCurSel(0);
}

/****************************************************
					��ȡ�û���Ϣ
*****************************************************/
void CRegsterDialog::LoadUserInfo()
{
	char Userstr[_MAX_PATH] = {0};
	sprintf_s(Userstr, _MAX_PATH, "%s\\system\\userinfo.bin", CurrentPath);

	CFile cf;
	if(!cf.Open(Userstr,CFile::modeRead))
		return;
	int usernum = cf.GetLength()/sizeof(UserInfo);
	for(int i = 0; i < usernum; i++){
		UserInfo info={0};
		cf.Read(&info, sizeof(UserInfo));
		m_users.AddString(info.UserName);
			if(i == usernum-1){
				m_username = info.UserName;
				m_remember = info.RememberPw;
				if(m_remember){
					GetDisorString((char*)(info.Pasword), 19);
					m_password = info.Pasword;
				}
			}
	}
	cf.Close();
	UpdateData(FALSE);
}

/*****************************************************
				��ȡ�Զ���¼��Ϣ
******************************************************/
void CRegsterDialog::LoadAutoLogin()
{
	char Path[_MAX_PATH] = {0};
	char val[10] = {0};
	sprintf_s(Path,  _MAX_PATH, "%s/system/sysinfo.ini", CurrentPath);
	GetPrivateProfileString("default login", "auto", "", val, 10, Path);
	if(atoi(val)==1)
		m_autolog = TRUE;
	else
		m_autolog = FALSE;
}

/****************************************************
					����ӽ���
*****************************************************/
void CRegsterDialog::GetDisorString(char *str, long len)
{
	for(int i = 0; i < len; i++){
		str[i] ^= EncyStr[i];
	}
}

void CRegsterDialog::OnDestroy() 
{
	SaveServerFile();
	bgBmp.DeleteObject();
	if(m_pServer)
		free(m_pServer);
	CDialog::OnDestroy();
}

void CRegsterDialog::OnSelchangeComboServer() 
{
	int item = m_servers.GetCurSel();
	if(item < m_servers.GetCount()){
		//��ʱδ�ṩ��ѡ��ķ������б��������̶��������ļ���
		//strcpy(JspServer, m_pServer[item].ServerIP);
		//strcpy(JspServer, "readings.vicp.net:8088");
		//strcpy(JspServer, "218.68.91.108:8088");
	}	
}

void CRegsterDialog::OnSelchangeComboUsername() 
{
	char username[32] = {0};
	int cur = m_users.GetCurSel();
	m_users.GetLBText(cur, username);
	char Userstr[_MAX_PATH] = {0};
	sprintf_s(Userstr,  _MAX_PATH, "%s\\system\\userinfo.bin", CurrentPath);
	CFile cf;
	if(!cf.Open(Userstr, CFile::modeRead))
		return;
	m_password.Empty();
	int usernum = cf.GetLength()/sizeof(UserInfo);
	for(int i = 0; i < usernum; i++){
		UserInfo Info = {0};
		cf.Read(&Info, sizeof(UserInfo));
		if(strcmp(Info.UserName, username) == 0){
			m_remember = Info.RememberPw;
			if(m_remember){
				GetDisorString((char *)(Info.Pasword), 19);
				m_password = Info.Pasword;
			}
			break;
		}
	}
	cf.Close();
	m_username = username;
	UpdateData(FALSE);	
}


/*******************************************************
						��¼
*******************************************************/
void CRegsterDialog::OnLogin() 
{

	UpdateData(TRUE);

//��ȡ�����ļ�config.ini.��ֵAspServer,JspServer
//added by zuoss 20081222 
	char iniPath[_MAX_PATH] = {0};
	char aspAddr[_MAX_PATH] = {0};

	sprintf_s(iniPath,  _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("server", "ip", NULL, aspAddr, 50, iniPath);
	sprintf_s(AspServer, 1024, "http://%s/readman",aspAddr);
	sprintf_s(JspServer, 1024, "http://%s:8088",aspAddr);
	
// 	CHttpDownLoadDlg DownDlg;
// 	DownDlg.m_URL.Format("%s/info.asp",AspServer);
// 	DownDlg.m_Path.Format("%s/temp/readman.pth", CurrentPath);
// 	DeleteFile(DownDlg.m_Path);
// 	if(DownDlg.DoModal()!=IDOK)
// 	{
// 		sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
// 		GetPrivateProfileString("server", "dns", NULL, aspAddr, 50, iniPath);
// 		sprintf(AspServer,"http://%s/readman",aspAddr);
// 		sprintf(JspServer,"http://%s:8088",aspAddr);
// 
// 		WSADATA   wsaData;   
// 		int  iErrorCode = WSAStartup(MAKEWORD(2,2),&wsaData);  
// 
// 		CString sIPAddress;
// 		struct hostent FAR *lpHostEnt=gethostbyname(aspAddr);
// 		if(lpHostEnt == NULL)
// 		{
// 			sIPAddress = _T("");
// 			sprintf(AspServer,"http://readings.vicp.net/readman");
// 			sprintf(JspServer,"http://readings.vicp.net:8088");
// 			//AfxMessageBox("�޷����ӷ�������������ѡ�񡰹�䡱�����Ժ����ԡ�");
// 			//return;
// 		}
// 		else
// 		{
// 			LPSTR lpaddr = lpHostEnt->h_addr_list[0];
// 			if(lpaddr)
// 			{
// 				struct in_addr inAddr;
// 				memmove (&inAddr,lpaddr, 4);
// 				sIPAddress=inet_ntoa (inAddr);
// 				if(sIPAddress.IsEmpty())
// 				{
// 					sIPAddress=_T("Not available");
// 					sprintf(AspServer,"http://readings.vicp.net/readman");
// 					sprintf(JspServer,"http://readings.vicp.net:8088");
// 					//AfxMessageBox("�޷����ӷ�������������ѡ�񡰹�䡱�����Ժ����ԡ�");
// 					//return;
// 				}
// 				//write to config.ini					
// 				else
// 				{
// 					char serverip[_MAX_PATH] = {0};
// 					sprintf(serverip, "%s\\system\\config.ini", CurrentPath);
// 					WritePrivateProfileString("server", "ip", sIPAddress, iniPath);
// 					sprintf(AspServer,"http://%s/readman",sIPAddress);
// 					sprintf(JspServer,"http://%s:8088",sIPAddress);
// 				}
// 			}
// 		}
// 	}
//end
	if(!m_autolog)
	{
		char Path[_MAX_PATH] = {0};
		sprintf_s(Path,  _MAX_PATH, "%s/system/sysinfo.ini", CurrentPath);
		if(m_autolog)
		{
			WritePrivateProfileString("default login", "auto", "1", Path);
		}
		else
		{
			WritePrivateProfileString("default login", "auto", "0", Path);
		}
	}
	
	CHttpDownLoadDlg dlg;
	//dlg.m_URL.Format("http://%s/RetailWeb/docheckuser?username=%s&pwd=%s&version=3&validdate=1", 
	//	JspServer, m_username, m_password);

//jinke
	if (m_username == "")
	{
		AfxMessageBox("�û�������Ϊ�գ�");
		return;
	}
	dlg.m_URL.Format("%s/server/checkuser.asp?username=%s&pwd=%s",AspServer,m_username,m_password);
   //��¼���� by fangji
	sprintf_s(::Password, 50, "%s", m_password);
	//safelab
//	dlg.m_URL.Format("http://202.113.25.155/Readman/server/checkuser.asp?username=%s&pwd=%s",m_username,m_password);

	dlg.m_Path.Format("%s\\temp\\Regist.inf", CurrentPath);
	DeleteFile(dlg.m_Path);
	
	if(!m_offline){
		if(dlg.DoModal() != IDOK){
			ReceiveIP2();
			MessageBox("��������ԭ�����ӷ�����ʧ�ܣ������������ú����ԡ�", "����", MB_OK|MB_ICONERROR);
			m_offline = TRUE;
			EndDialog(IDOK);
			return;
		}

		/*if(!CheckUserRegInfo(dlg.m_Path)){
			MessageBox("�û���������������������ԣ�", "����", MB_OK|MB_ICONERROR);
			return;
		}*/

		//����ֵ��1-��½�ɹ���2-�û����������3-��Ҫ�û�������Ϣ��4-�û�δ����
		int checkresult = CheckUserRegInfo(dlg.m_Path);
		if(checkresult == 2)
		{
			MessageBox("�û���������������������ԣ�", "����", MB_OK|MB_ICONERROR);
			return;
		}
		else if (checkresult == 3)
		{
			AfxMessageBox("�����������������Ҫ�޸ĸ�����Ϣ���޸ĺ󼴿�ʹ�á���л����֧��!");
			CString userinfopath = "";
			userinfopath.Format("%s/server/changeUserInfo.asp?username=%s",AspServer,m_username);
			ShellExecute(NULL, "open", userinfopath,NULL, NULL, SW_SHOW);
			int re = 0;
			CRegsterDialog::EndDialog(re);

			return;
		}
		else if (checkresult == 4)
		{
			AfxMessageBox("�˻�δ���������ʼ�����ɼ���");
			return;
		}
		//���ӻ���
		//AfxMessageBox("���ӻ���");
		CString url;
		url.Format("%s/server/addscore.asp?username=%s",AspServer,m_username);
		if (!UpLoad(url))
		{
			//AfxMessageBox("��������ʧ��");
		}
	}

	SaveUserInfo();
	m_rmDisk.GetWindowText(m_SDCard);
	CDialog::OnOK();		
}

void CRegsterDialog::ReceiveIP2()
{
	//���շ�����IP��ַ
	CHttpDownLoadDlg udlg;
	udlg.m_URL = SERVERLISTURL;
    udlg.m_Path.Format("%s\\temp\\update.web", CurrentPath);
	//IP��ַ�����˿ڣ�#Ĭ��״̬#����������
	if(udlg.DoModal() == IDOK){
		CFile cf;
		if(cf.Open(udlg.m_Path, CFile::modeRead)){
			long flen = cf.GetLength();
			char* buf = (char*)malloc(flen+1);
			ZeroMemory(buf, flen+1);
			cf.Read(buf, flen);
			cf.Close();
			SaveServersInfo(buf);
			free(buf);
		}
	}
	else
		MessageBox("��ȡ��������ַʧ��", "����", MB_OK|MB_ICONWARNING);
	DeleteFile(udlg.m_Path);
}

int CRegsterDialog::CheckUserRegInfo(LPCSTR FileName)
{
	//�ļ��е�ֵ��1-��½�ɹ���2-�û����������3-��Ҫ�û�������Ϣ��4-�û�δ����

	CFile cf;
	if(!cf.Open(FileName, CFile::modeRead))
		return FALSE;
	char sflag[1];
	cf.Read(&sflag, 1);
	int iflag = atoi(sflag); 
	cf.Close();
	DeleteFile(FileName);
	return iflag;	
}

void CRegsterDialog::SaveUserInfo()
{
	UpdateData(TRUE);
	char Userstr[_MAX_PATH] = {0};
	sprintf_s(Userstr,  _MAX_PATH, "%s\\system\\userinfo.bin", CurrentPath);
	CFile cf;
	if(!cf.Open(Userstr, CFile::modeCreate|CFile::modeReadWrite))
		return;
	char username[32] = {0};
	strcpy_s(username, 32, m_username);
	int usernum = cf.GetLength()/sizeof(UserInfo);
	if(usernum != 0){
		UserInfo* Info = (UserInfo*)malloc(usernum*sizeof(UserInfo));
		ZeroMemory(Info, usernum*sizeof(UserInfo));
		cf.Read(Info, usernum*sizeof(UserInfo));
		int hasno = -1;
		for(int i=0; i<usernum; i++){
			UserInfo* tmp = Info+i;
			if(strcmp(tmp->UserName, username) == 0)
				hasno = i;
		}
		if(hasno == -1){
			if(usernum > 9){	//ֻ�ܴ�9��
				cf.SeekToBegin();
				cf.Write(Info+1, (usernum-1)*sizeof(UserInfo));
			}
			else
				cf.SeekToEnd();
		}
		else{
			cf.SeekToBegin();
			for(int i=0; i<usernum; i++){
				UserInfo* tmp = Info+i;
				if(i != hasno)
					cf.Write(tmp,sizeof(UserInfo));
			}
			cf.Seek(-(long)sizeof(UserInfo), CFile::end);
		}
		if(Info)
			free(Info);
	}
	UserInfo info = {0};
	strcpy_s(info.UserName, 32, username);
	info.RememberPw = m_remember;
	if(m_remember){
		strcpy_s((char*)info.Pasword, 20, m_password);
		GetDisorString((char*)(info.Pasword), 19);
	}
	srand((unsigned)time(NULL));
	for(int i = 0; i < 4; i++){
		info.Puzzle[i] = rand();
	}
	cf.Write(&info, sizeof(UserInfo));
	cf.Close();
}

void CRegsterDialog::SaveServersInfo(char *buf)
{
	char serverFile[_MAX_PATH] = {0};
	sprintf_s(serverFile,  _MAX_PATH, "%s\\system\\Servers.inf", CurrentPath);

	char DefServer[100] = {0};
	GetDefServer(serverFile, DefServer, 100);

	CFile cf;
	if(!cf.Open(serverFile, CFile::modeCreate|CFile::modeWrite))
		return;
	//IP��ַ�����˿ڣ�#Ĭ��״̬#����������
	char *tmp1 = buf;
	char *tmp2 = NULL;
	while(tmp1){
		char str[1000] = {0};
		tmp2 = strstr(tmp1, "\n");
		if(tmp2){
			strncpy_s(str, 1000, tmp1, tmp2 - tmp1);
			tmp1 = tmp2 + 1;
		}
		else{
			strcpy_s(str, 1000, tmp1);
			tmp1 = NULL;
		}
		if(strlen(str) > 10){
			//ÿһ�����������ļ�¼
			if(str[strlen(str) - 1] == 0x0d || str[strlen(str) - 1] == 0x0a)
				str[strlen(str) - 1] = 0;

			DOWNLOADSERVERS servers = {0};
			char *find1 = strstr(str, "#");
			if(find1){
				strncpy_s(servers.ServerIP, 64, str, find1 - str);
				char *find2 = strstr(find1 + 1, "#");
				if(find2){
					char state[10] = {0};
					if(find2 - find1 - 1 < 10)
						strncpy_s(state, 10, find1 + 1, find2 - find1 - 1);
					if(state[0] == '1')
						servers.DefState = 1;
					strcpy_s(servers.Description, 64, find2 + 1);
				}
			}
			if(strlen(servers.Description) > 0)
				cf.Write(&servers, sizeof(DOWNLOADSERVERS));
		}
	}
	cf.Close();

	SetDefServer(serverFile, DefServer);
}

void CRegsterDialog::SaveServerFile()
{
	int item = m_servers.GetCurSel();
	if(item < 0)
		return;

	char serverFile[_MAX_PATH] = {0};
	sprintf_s(serverFile,  _MAX_PATH, "%s\\system\\Servers.inf", CurrentPath);
	CFile cf;
	if(!cf.Open(serverFile, CFile::modeCreate|CFile::modeWrite))
		return;
	for(int i=0; i<m_servers.GetCount(); i++){
		m_pServer[i].DefState = 0;
	}
	m_pServer[item].DefState = 1;
	cf.Write(m_pServer, sizeof(DOWNLOADSERVERS)*m_servers.GetCount());
	cf.Close();
}

void CRegsterDialog::GetDefServer(char *FileName, char *serverip, int serverip_len)
{
	CFile cf;
	if(!cf.Open(FileName,CFile::modeRead))
		return;
	int servernum = cf.GetLength()/sizeof(DOWNLOADSERVERS);
	if(servernum == 0){
		cf.Close();
		return;
	}
	DOWNLOADSERVERS server = {0};
	for(int i = 0; i < servernum; i++){
		cf.Read(&server, sizeof(DOWNLOADSERVERS));
		if(server.DefState == 1)
			strcpy_s(serverip, serverip_len, server.ServerIP);
	}
	cf.Close();
}

void CRegsterDialog::SetDefServer(char *FileName, char *serverip)
{
	if(!serverip || strlen(serverip) == 0)
		return;

	CFile cf;
	if(!cf.Open(FileName, CFile::modeReadWrite|CFile::modeCreate))
		return;

	int servernum = cf.GetLength()/sizeof(DOWNLOADSERVERS);
	if(servernum == 0){
		cf.Close();
		return;
	}
	
	DOWNLOADSERVERS* server = (DOWNLOADSERVERS*)malloc(servernum*sizeof(DOWNLOADSERVERS));
	ZeroMemory(server, servernum*sizeof(DOWNLOADSERVERS));
	cf.Read(server, sizeof(DOWNLOADSERVERS)*servernum);
	int item = 0;
	for(item = 0; item< servernum; item++){
		if(strcmp(serverip, server[item].ServerIP) == 0)
			break;
	}
	int item1 = 0;
	for(item1=0; item1<servernum; item1++){
		if(server[item1].DefState == 1)
			break;
	}
	if(item != servernum){
		if(item1 != servernum){
			server[item1].DefState = 0;
		}
		server[item].DefState = 1;
	}
	else{
		if(item1 == servernum){
			server[0].DefState = 1;
		}
	}
	cf.SeekToBegin();
	cf.Write(server, sizeof(DOWNLOADSERVERS)*servernum);
	cf.Close();
	free(server);
}

/******************************************************
						�û�ע��
*******************************************************/
void CRegsterDialog::OnButtonRegister() 
{
	UpdateData(TRUE);
	//20081229
//��ȡ�����ļ�config.ini.���ע��ҳ���ַ
//added by zuoss 20090112 
	char iniPath[_MAX_PATH] = {0};
	char aspAddr[_MAX_PATH] = {0};

	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("server", "ip", NULL, aspAddr, 50, iniPath);
	sprintf_s(AspServer, 1024, "http://%s/readman",aspAddr);
/*	CHttpDownLoadDlg DownDlg;
	DownDlg.m_URL.Format("%s/info.asp",AspServer);
	DownDlg.m_Path.Format("%s/temp/readman.pth", CurrentPath);
	DeleteFile(DownDlg.m_Path);
	if(DownDlg.DoModal()!=IDOK)
	{
		sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
		GetPrivateProfileString("server", "dns", NULL, aspAddr, 50, iniPath);
		sprintf(AspServer,"http://%s/readman",aspAddr);

		WSADATA   wsaData;   
		int  iErrorCode = WSAStartup(MAKEWORD(2,2),&wsaData);  

		CString sIPAddress;
		struct hostent FAR *lpHostEnt=gethostbyname(aspAddr);
		if(lpHostEnt == NULL)
		{
			sIPAddress = _T("");
			sprintf(AspServer,"http://readings.vicp.net/readman");
		}
		else
		{
			LPSTR lpaddr = lpHostEnt->h_addr_list[0];
			if(lpaddr)
			{
				struct in_addr inAddr;
				memmove (&inAddr,lpaddr, 4);
				sIPAddress=inet_ntoa (inAddr);
				if(sIPAddress.IsEmpty())
				{
					sIPAddress=_T("Not available");
					sprintf(AspServer,"http://readings.vicp.net/readman");
				}
				//write to config.ini					
				else
				{
					char serverip[_MAX_PATH] = {0};
					sprintf(serverip, "%s\\system\\config.ini", CurrentPath);
					WritePrivateProfileString("server", "ip", sIPAddress, iniPath);
					sprintf(AspServer,"http://%s/readman",sIPAddress);
				}
			}
		}
	}
*/
	char WebPath[1024] = {0};
	sprintf_s(WebPath, 1024, "%s/server/register.asp",AspServer);
	ShellExecute(NULL, "open", WebPath,	NULL, NULL, SW_SHOW);
//end

// 
// 	UpdateData(TRUE);
// 	char WebPath[1024] = {0};
// 	sprintf(WebPath,"%s/server/register.asp",AspServer);
// 
//	ShellExecute(NULL, "open", WebPath,	NULL, NULL, SW_SHOW);
}

/*********************************************************
						�ο�
*********************************************************/
void CRegsterDialog::OnButtonGuest() 
{
//	UpdateData(TRUE);
	m_username = "Guest_Default";
	UpdateData(FALSE);

	//��ȡ�����ļ�config.ini.���ע��ҳ���ַ
//added by zuoss 20090116 
	char iniPath[_MAX_PATH] = {0};
	char aspAddr[_MAX_PATH] = {0};

	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("server", "ip", NULL, aspAddr, 50, iniPath);
	sprintf_s(AspServer, 1024, "http://%s/readman",aspAddr);
/*	CHttpDownLoadDlg DownDlg;
	DownDlg.m_URL.Format("%s/info.asp",AspServer);
	DownDlg.m_Path.Format("%s/temp/readman.pth", CurrentPath);
	DeleteFile(DownDlg.m_Path);
	if(DownDlg.DoModal()!=IDOK)
	{
		sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
		GetPrivateProfileString("server", "dns", NULL, aspAddr, 50, iniPath);
		sprintf(AspServer,"http://%s/readman",aspAddr);

		WSADATA   wsaData;   
		int  iErrorCode = WSAStartup(MAKEWORD(2,2),&wsaData);  

		CString sIPAddress;
		struct hostent FAR *lpHostEnt=gethostbyname(aspAddr);
		if(lpHostEnt == NULL)
		{
			sIPAddress = _T("");
			sprintf(AspServer,"http://readings.vicp.net/readman");
		}
		else
		{
			LPSTR lpaddr = lpHostEnt->h_addr_list[0];
			if(lpaddr)
			{
				struct in_addr inAddr;
				memmove (&inAddr,lpaddr, 4);
				sIPAddress=inet_ntoa (inAddr);
				if(sIPAddress.IsEmpty())
				{
					sIPAddress=_T("Not available");
					sprintf(AspServer,"http://readings.vicp.net/readman");
				}
				//write to config.ini					
				else
				{
					char serverip[_MAX_PATH] = {0};
					sprintf(serverip, "%s\\system\\config.ini", CurrentPath);
					WritePrivateProfileString("server", "ip", sIPAddress, iniPath);
					sprintf(AspServer,"http://%s/readman",sIPAddress);
				}
			}
		}
	}
//end
*/
	CDialog::OnOK();	
}

/**********************************************************
						�һ�����
***********************************************************/
void CRegsterDialog::OnButtonGetpwd() 
{
	/*
	char WebPath[1024] = {0};
	sprintf(WebPath, "%s/RetailWeb/member/lostpassword.jsp", JspServer);
	ShellExecute(NULL, "open", WebPath,	NULL, NULL, SW_SHOW);
	*/
	
//��ȡ�����ļ�config.ini.��ֵAspServer,JspServer
//added by zuoss 20081222 
	char iniPath[_MAX_PATH] = {0};
	char aspAddr[_MAX_PATH] = {0};

	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("server", "ip", NULL, aspAddr, 50, iniPath);
	sprintf_s(AspServer, 1024, "http://%s/readman",aspAddr);
	sprintf_s(JspServer, 1024, "http://%s:8088",aspAddr);
	
	CHttpDownLoadDlg DownDlg;
	DownDlg.m_URL.Format("%s/info.asp",AspServer);
	DownDlg.m_Path.Format("%s/temp/readman.pth", CurrentPath);
	DeleteFile(DownDlg.m_Path);
	if(DownDlg.DoModal()!=IDOK)
	{
		sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
		GetPrivateProfileString("server", "dns", NULL, aspAddr, 50, iniPath);
		sprintf_s(AspServer, 1024, "http://%s/readman",aspAddr);
		sprintf_s(JspServer, 1024, "http://%s:8088",aspAddr);

		WSADATA   wsaData;   
		int  iErrorCode = WSAStartup(MAKEWORD(2,2),&wsaData);  

		CString sIPAddress;
		struct hostent FAR *lpHostEnt=gethostbyname(aspAddr);
		if(lpHostEnt == NULL)
		{
			sIPAddress = _T("");
			strcpy_s(AspServer, 1024, "http://readings.vicp.net/readman");
			strcpy_s(JspServer, 1024, "http://readings.vicp.net:8088");
			//AfxMessageBox("�޷����ӷ�������������ѡ�񡰹�䡱�����Ժ����ԡ�");
			//return;
		}
		else
		{
			LPSTR lpaddr = lpHostEnt->h_addr_list[0];
			if(lpaddr)
			{
				struct in_addr inAddr;
				memmove (&inAddr,lpaddr, 4);
				sIPAddress=inet_ntoa (inAddr);
				if(sIPAddress.IsEmpty())
				{
					sIPAddress=_T("Not available");
					strcpy_s(AspServer, 1024, "http://readings.vicp.net/readman");
					strcpy_s(JspServer, 1024, "http://readings.vicp.net:8088");
					//AfxMessageBox("�޷����ӷ�������������ѡ�񡰹�䡱�����Ժ����ԡ�");
					//return;
				}
				//write to config.ini					
				else
				{
					char serverip[_MAX_PATH] = {0};
					sprintf_s(serverip, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
					WritePrivateProfileString("server", "ip", sIPAddress, iniPath);
					sprintf_s(AspServer, _MAX_PATH, "http://%s/readman",sIPAddress);
					sprintf_s(JspServer, _MAX_PATH, "http://%s:8088",sIPAddress);
				}
			}
		}
	}
//end

	CString userinfopath = "";
	userinfopath.Format("%s/server/GetPassword.asp",AspServer);
	ShellExecute(NULL, "open", userinfopath,	NULL, NULL, SW_SHOW);
	
	int re = 0;
	CRegsterDialog::EndDialog(re);

	return;
}

void CRegsterDialog::OnCheckSave() 
{
	UpdateData(TRUE);
	if(m_remember){
		GetDlgItem(IDC_CHECK_AUTOLOG)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_CHECK_AUTOLOG)->EnableWindow(FALSE);
	}
}
