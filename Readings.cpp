// Readings.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Readings.h"
#include "MainFrm.h"
#include "HttpDownLoadDlg.h"
#include "RegsterDialog.h"
#include "MainFrm.h"
#include "tlhelp32.h"
#include "UDiskAuth.h"
#include "ErrorExitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL offline = FALSE;
char CurrentPath[_MAX_PATH] = {0};
char UserName[50] = {0};
char Password[50] = {0};
char AspServer[1024] = {0};
char JspServer[1024] = {0};
char SimCardID[20] = {0};
char SDDisk[20] = {0};
char FavorPath[_MAX_PATH] = {0};
char ShareFileFolder[_MAX_PATH] ={0};
CMainFrame* pMainWnd = NULL;
_ConnectionPtr m_pConnection;
string RequestMail = "ReadingsResourceRequest";//请求邮件主题
string AgreeMail = "AgreeofReadingsRequest";//同意请求邮件主题
string RefuseMail = "RefuseofReadingsRequest";//拒绝请求邮件主题
string ChargingMail = "Chargingresource";//交换邮件主题

USB_DETAIL_INFO uDiskAuthInfo={0};

//add by yangxiao 2008.8.12
char hostOutIp[20] = {0};
char hostInerIp[20] = {0};

//Added by zhanganzhan on 20081023
bool gbOnSearching = FALSE;
//add by yangxiao 2008.8.13
CFile* currLogin;
/*
//add by yangxiao	2008.8.21 begin
enum COMMOPER {
	comm_oper_appendUser, //用户信息追加
	comm_oper_delUser,   //用户信息删除
	comm_oper_recvMsg, //接收聊天消息
	comm_oper_sentMSg, //发送聊天消息
	comm_oper_tranHotKey, //传送热词
	comm_oper_leftTree_search, //左树搜索
	comm_oper_search, //搜索
	
};
//add by yangxiao	2008.8.21 end
*/

/////////////////////////////////////////////////////////////////////////////
// CReadingsApp

BEGIN_MESSAGE_MAP(CReadingsApp, CWinApp)
	//{{AFX_MSG_MAP(CReadingsApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadingsApp construction

CReadingsApp::CReadingsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	SetUnhandledExceptionFilter(RDSExceptionFun); 
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CReadingsApp object

CReadingsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CReadingsApp initialization
LONG WINAPI RDSExceptionFun(_EXCEPTION_POINTERS * rdsexception)
{
//	AfxMessageBox(ExceptionInfo->ContextRecord);
//	AfxMessageBox((char*)ExceptionInfo->ExceptionRecord->ExceptionRecord->ExceptionInformation);
#if 0
	// 获得崩溃信息，发送给服务器
	char errorCode[1024];
	sprintf_s(errorCode,"error code %d,address %d",rdsexception->ExceptionRecord->ExceptionCode,
		      rdsexception->ExceptionRecord->ExceptionAddress,rdsexception->ExceptionRecord->ExceptionInformation[0]);
#endif
#if !_DEBUG
	CErrorExitDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
	//	AfxMessageBox("Readings 因为意外原因退出。");
	}
#endif
	//AfxMessageBox("error");
	return EXCEPTION_EXECUTE_HANDLER;
}
BOOL CReadingsApp::InitInstance()
{
/**
	zuoss 20090305
	开机检验是否已经有Readings正在运行。
	如果有则提示用户，可以退出当前的登陆，也可以退出先前已经运行的。
*/

//需要用到"tlhelp32.h"头文件

	PROCESSENTRY32   ProInfo;   
	HANDLE	hProSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL,0); 
	HANDLE  handle1 = NULL;
	HANDLE  handle2 = NULL;
	int clientcount = 0;
	if(::Process32First(hProSnap,&ProInfo))   
	{   
		for(;::Process32Next(hProSnap,&ProInfo);)   
		{   
			if(strcmp(ProInfo.szExeFile,"Readings.exe") == 0)   
			{   
				if (handle1 == NULL)
				{
					handle1 = ::OpenProcess(PROCESS_ALL_ACCESS,NULL, ProInfo.th32ProcessID);    
				}
				else 
				{	
					if (handle2 == NULL)
					{
						handle2 = ::OpenProcess(PROCESS_ALL_ACCESS,NULL, ProInfo.th32ProcessID);
					}
				}
				clientcount++;   
			}
		}
	} 
	if (clientcount > 1)
	{
		CString msg("已经有Reading同步阅读程序正在运行！您是否要重新登陆？\n\n选择“是”终止先前程序，重新登陆。\n选择“否”取消当前登陆。");
		if(AfxMessageBox(msg,MB_YESNO)==IDYES)
		{
			//终止先前进程
			TerminateProcess(handle1, 0);
			Sleep(2000);
		}
		else
		{
			//终止自身进程。exit(0);ExitProcess(0);
			ExitProcess(0);
		}
	}

//end
// 	CUDiskAuth * uDiskAuth = new CUDiskAuth;
// 	if (uDiskAuth->GetUSBDiskID('H'))
// 	{
// 	//	AfxMessageBox(uDiskAuthInfo.DevName);
// 	//	AfxMessageBox(uDiskAuthInfo.ID);
// 	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)	return FALSE;

	if(!MakeSemaph())//设置标记
		return FALSE;

	GetModuleFileName(NULL, CurrentPath, _MAX_PATH);
	for(int i = strlen(CurrentPath) - 1; i > 0; i--){
		if(CurrentPath[i] == '\\'){
			CurrentPath[i] = 0;
			break;
		}
		CurrentPath[i] = 0;
	}
	char TmpPath[_MAX_PATH] = {0};
	sprintf_s(TmpPath, _MAX_PATH, "%s\\temp", CurrentPath);
	_mkdir(TmpPath);

	
	AfxInitRichEdit();//初始化RichEdit控件
	//::LoadLibrary("riched20.dll");
//	RegisterActiveX();//注册webfilter
	if(CheckNewVersion()){
		CDialog dlg(IDD_YESNODIALOG);
		if (dlg.DoModal() == IDOK)
		{
			::ShellExecute(NULL, "open", "upgrade.exe", NULL, CurrentPath, SW_SHOW);
			return FALSE;
		}
		
	}
	CoInitialize(NULL);

	//多于一个Readings启动时，socket初始化失败。zuoss090305
	if(!InitialSocket()) return FALSE;
	
	
	if(!ConnectToDB()){
		AfxMessageBox("数据库初始化失败，请您重新安装程序！");
		return FALSE;
	}

	CRegsterDialog regDlg;
	if(regDlg.DoModal()!=IDOK)	return FALSE;
	offline = regDlg.m_offline;
	strcpy_s(UserName, 50, regDlg.m_username);

	if(!regDlg.m_SDCard.IsEmpty())
	{	
		if (uDiskAuthInfo.diskFlag == regDlg.m_SDCard.GetAt(0))
		{
			strcpy_s(SDDisk, 20, regDlg.m_SDCard);
		}
		
	}
/*	if(!offline){
		CHttpDownLoadDlg DownDlg;
		DownDlg.m_URL.Format("http://www.readings.com.cn/readman/info.asp");
		DownDlg.m_Path.Format("%s/temp/readman.pth", CurrentPath);
		DeleteFile(DownDlg.m_Path);
		if(DownDlg.DoModal()!=IDOK)
			strcpy(AspServer, "http://61.181.14.184/readman");
		else{
			CFile cf;
			if(cf.Open(DownDlg.m_Path, CFile::modeRead)){
				if(cf.GetLength()<1024)
					cf.Read(AspServer,cf.GetLength());
				else
					strcpy(AspServer, "http://61.181.14.184/readman");
				cf.Close();
			}
			else
				strcpy(AspServer, "http://61.181.14.184/readman");
		}
		//safelab:
		//strcpy(AspServer, "http://202.113.25.155/readman");
		//strcpy(AspServer, "http://readings.vicp.net/readman");
		strcpy(AspServer,"http://218.68.91.108/readman");
	}
	*/
/*
	//added by zuoss 20081222 
	char iniPath[_MAX_PATH] = {0};
	char aspAddr[_MAX_PATH] = {0};

	sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("server", "ip", NULL, aspAddr, 50, iniPath);
	sprintf(AspServer,"http://%s/readman",aspAddr);
	
	CHttpDownLoadDlg DownDlg;
	DownDlg.m_URL.Format("%s/main.asp",AspServer);
	DownDlg.m_Path.Format("%s/temp/readman.pth", CurrentPath);
	DeleteFile(DownDlg.m_Path);
	if(DownDlg.DoModal()!=IDOK)
	{
		sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
		GetPrivateProfileString("server", "dns", NULL, aspAddr, 50, iniPath);
		sprintf(AspServer,"http://%s/readman",aspAddr);
		CString sIPAddress;
		struct hostent FAR *lpHostEnt=gethostbyname(aspAddr);
		if(lpHostEnt == NULL)
		{
			sIPAddress = _T("");
			AfxMessageBox("无法连接服务器！");
			return false;
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
					AfxMessageBox("无法连接服务器！");
					return false;
				}
				//write to config.ini					
				char serverip[_MAX_PATH] = {0};
				sprintf(serverip, "%s\\system\\config.ini", CurrentPath);
				WritePrivateProfileString("server", "ip", sIPAddress, iniPath);
			}
		}
	}
//end*/

	sprintf_s(FavorPath, _MAX_PATH, "%s\\temp\\%s", CurrentPath, UserName);
	_mkdir(FavorPath);

		//add by dongzhenhua 20080605
	char CachePath[_MAX_PATH]={0};
	char OneCache[_MAX_PATH]={0};

	sprintf_s(CachePath, _MAX_PATH, "%s\\temp\\groupcache",CurrentPath);
	_mkdir(CachePath);

	sprintf_s(OneCache, _MAX_PATH, "%s\\baidu",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\baiduentity",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\dangdang",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\fanshu",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\duxiu",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\Google",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\du8",CachePath);
	_mkdir(OneCache);

// 	memset(OneCache,0,_MAX_PATH);
// 	sprintf(OneCache,"%s\\OpenCourse",CachePath);
// 	_mkdir(OneCache);
	
	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\local",CachePath);
	_mkdir(OneCache);
	
	memset(OneCache, 0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\order",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\readman",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf(OneCache,"%s\\share",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\sina",CachePath);
	_mkdir(OneCache);

	memset(OneCache,0,_MAX_PATH);
	sprintf_s(OneCache, _MAX_PATH, "%s\\VeryCD",CachePath);
	_mkdir(OneCache);

// 	memset(OneCache,0,_MAX_PATH);
// 	sprintf(OneCache,"%s\\Huntmine",CachePath);
// 	_mkdir(OneCache);


// 	memset(OneCache,0,_MAX_PATH);
// 	sprintf(OneCache,"%s\\zcom",CachePath);
// 	_mkdir(OneCache);
	
	sprintf_s(ShareFileFolder, _MAX_PATH, "%s\\share", CurrentPath);
	
	
	CMainFrame* pMainFrame = new CMainFrame;

	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		return FALSE;
	}
 
	CString title;
	if (strcmp(UserName,"Guest_Default") == 0)
	{
		CMenu *menu = pMainFrame->GetMenu();     
		CMenu *SubMenu = menu->GetSubMenu(1); 
		SubMenu->EnableMenuItem(1,MF_GRAYED|MF_BYPOSITION);	
		title.Format("%sv%.2f——Guest", SOFTWARE_NAME, VERSION);
	}
	else
	{
		title.Format("%sv%.2fBeta——%s", SOFTWARE_NAME, VERSION, UserName);
	}
	
	pMainFrame->SetWindowText(title);
		

	// add by yangxiao 20080813 begin
// 	CString currLogFile;
// 	currLogFile.Format("%s\\system\\currLog.log",CurrentPath);
// 	currLogin = new CFile();
// 	if(!currLogin->Open(currLogFile,CFile::modeCreate|CFile::modeReadWrite))
// 		Tips("打开日志文件失败");
	// add by yangxiao 20080813 end
	

	m_pMainWnd = pMainFrame;

	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	char val[50] = {0};
	GetPrivateProfileString("normal", "showMin", NULL, val, 50, iniPath);
	if(atoi(val))
		m_pMainWnd->ShowWindow(SW_HIDE);
	else
		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}
#if 0
LONG WINAPI CReadingsApp::RDSExceptionFun(_EXCEPTION_POINTERS   *ExceptionInfo   )
{
	AfxMessageBox("error");
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CReadingsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CReadingsApp message handlers


BOOL CReadingsApp::InitialSocket()
{
	//初始化套结字
	WSADATA wsaData = {0};
	int err;
	err = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(err!=0)
		return FALSE;
/*
	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)
		return FALSE; 
*/
	return TRUE;
}

int CReadingsApp::ExitInstance() 
{
	//added by zuoss 20090116 对于“逛逛”用户退出时删除OnLineUser表中的记录。
/*	if (!offline && (strcmp(UserName,"") == 0))
	{
		CHttpDownLoadDlg DownDlg;
		DownDlg.m_URL.Format("%s/logon/userlogout.asp?username=%s", AspServer, "Guest_Default");
		DownDlg.m_Path.Format("%s/temp/logout.tmp", CurrentPath);
		if(DownDlg.DoModal()!=IDOK)
		{
			;//OnLineUser表中Guest_Default未删除成功。
		}
	}*/
//end
	::WSACleanup();
	if (m_pConnection != NULL)
	{
		m_pConnection->Close();
	}
	
//	AfxMessageBox("m_pConnection->Close();");
//	m_pConnection.Release();

//	CoUninitialize();
	RemoveTempFiles();

	return CWinApp::ExitInstance();
}

BOOL CReadingsApp::ConnectToDB()
{ 
	HRESULT chr;
	HRESULT ohr;
	try{
		char conn[1000] = {0};
		sprintf_s(conn, 1000, "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s\\system\\db\\db.mdb", CurrentPath);
		chr = m_pConnection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(chr)){
			ohr = m_pConnection->Open(conn,"","",adModeUnknown);
			if(SUCCEEDED(ohr))
				return TRUE;
			else{
				//m_pConnection.Release();
				m_pConnection = NULL;
				return FALSE;
			}
		}
	}
	catch(_com_error& e){
		if(SUCCEEDED(chr))
			//m_pConnection.Release();
		m_pConnection = NULL;
		return FALSE;
	}
	return FALSE;
}

BOOL CReadingsApp::MakeSemaph()
{
	HANDLE hSem = CreateSemaphore(NULL, 1, 1, m_pszExeName);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hSem);
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(),GW_CHILD);
		while (IsWindow(hWndPrevious))
		{
			if (GetProp(hWndPrevious, m_pszExeName))
			{ 
				ShowWindow(hWndPrevious,SW_SHOWMAXIMIZED);
				SetForegroundWindow(hWndPrevious); 
				SetForegroundWindow(::GetLastActivePopup(hWndPrevious)); 

				return FALSE; 
			} 
			hWndPrevious = ::GetWindow(hWndPrevious,GW_HWNDNEXT);
		}
		return FALSE;
	}
	return TRUE;
}

void CReadingsApp::RemoveTempFiles()
{
	CString path;
	path.Format("%s\\temp\\listCache\\*", CurrentPath);
	CFileFind filefind;
	BOOL ext = filefind.FindFile(path);
	while(ext){
		ext = filefind.FindNextFile();
		if(filefind.IsDots())
			continue;
		if(filefind.IsDirectory())
			continue;
		DeleteFile(filefind.GetFilePath());
	}
	filefind.Close();
}

BOOL CReadingsApp::CheckNewVersion()
{
	CHttpDownLoadDlg DownDlg;
	DownDlg.m_URL = UPGRADEURL;
	DownDlg.m_Path.Format("%s\\temp\\newVersion.info", CurrentPath);
	//DeleteFile(DownDlg.m_Path);
	if(DownDlg.DoModal()!=IDOK)
		return FALSE;
	CFile cf;
	if(!cf.Open(DownDlg.m_Path,CFile::modeRead)){
		DeleteFile(DownDlg.m_Path);
		return FALSE;
	}
	//buf 存储
	char* buf = new char[cf.GetLength()+1];
	ZeroMemory(buf, cf.GetLength()+1);
	cf.Read(buf, cf.GetLength());
	cf.Close();
	//AfxMessageBox(buf);
	double ver = atof(buf);
	delete[] buf;
	if(ver>VERSION)
		return TRUE;
	return FALSE;
}

BOOL CReadingsApp::RegisterActiveX()
{
	CString FilePath;
	FilePath.Format("%s\\UrlToWol\\dll\\WebFilter.dll",CurrentPath);
	HINSTANCE hLib = LoadLibrary((LPCSTR)FilePath);
	if(!hLib){ 
		MessageBox(NULL,"不能载入Dll文件!","结果",MB_OK); 
		return FALSE; 
	} 
	//获取注册函数DllRegisterServer地址 
	FARPROC lpDllEntryPoint;
	lpDllEntryPoint = GetProcAddress(hLib,_T("DllRegisterServer")); 
	//调用注册函数DllRegisterServer 
	if(lpDllEntryPoint!=NULL){ 
		if(FAILED((*lpDllEntryPoint)())){ 
			MessageBox(NULL,"调用DllRegisterServer失败！","结果",MB_OK); 
			FreeLibrary(hLib);
			return FALSE; 
		} 
		MessageBox(NULL,"注册成功","结果",MB_OK);
		return TRUE; 
	} 
	else
		MessageBox(NULL,"调用DllRegisterServer失败!","结果",MB_OK); 
	return FALSE; 
} 