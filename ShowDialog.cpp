// ShowDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ShowDialog.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowDialog dialog
extern CMainFrame* pMainWnd;
extern char CurrentPath[_MAX_PATH];
extern char UserName[50];

CShowDialog::CShowDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CShowDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShowDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CShowDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowDialog)
	DDX_Control(pDX, IDC_EXPLORER, m_browser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowDialog, CDialog)
	//{{AFX_MSG_MAP(CShowDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowDialog message handlers

void CShowDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_browser.GetSafeHwnd())
		m_browser.MoveWindow(0,0,cx,cy);
	
}

BEGIN_EVENTSINK_MAP(CShowDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(CShowDialog)
	ON_EVENT(CShowDialog, IDC_EXPLORER, 250 /* BeforeNavigate2 */, OnBeforeNavigate2Explorer, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/*void CShowDialog::OnBeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) 
{
 	char WebPath[1024] = {0};
	BOOL bUsedDefaultChar;
	WideCharToMultiByte(CP_ACP, 0, URL->bstrVal, -1, WebPath, 1024, NULL, &bUsedDefaultChar);

	//共享资源下载
	char* tail = strstr(WebPath, "&option=download");
	if(tail){
		char* head = strstr(WebPath, "sid=");
		if(!head)
			return;
		long sid = atol(head+strlen("sid="));
		pMainWnd->PostMessage(WM_REQUEST_RESUOURCE_FROM_SERVER, 0, (LPARAM)sid);
		return;
	}	

	tail = strstr(WebPath,"operation=djxz");
	if(tail){
		CFile cf;
		CString path;
		path.Format("%s\\system\\name", CurrentPath);
		if(cf.Open(path,CFile::modeCreate|CFile::modeWrite)){
			cf.Write(UserName,strlen(UserName));
			cf.Close();
			ShellExecute(NULL,"open","./system/Readman.exe","",CurrentPath, SW_SHOW);
		}
		else{
			AfxMessageBox("程序被破坏，请您重新安装Readings程序！");
		}
	}
}*/
void CShowDialog::OnBeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) 
{
 	char WebPath[1024] = {0};
	BOOL bUsedDefaultChar;
	WideCharToMultiByte(CP_ACP, 0, URL->bstrVal, -1, WebPath, 1024, NULL, &bUsedDefaultChar);
	
	//
	CString theurl = WebPath;
	//if (theurl.Find("http://61.181.14.184:8084/ReadingStore/detail.jsp?download=") != -1)
	if (theurl.Find("http://61.181.14.184:8084/ReadingStore/detail.do?download=") != -1)
	{
		theurl.Replace("http://61.181.14.184:8084/ReadingStore/detail.do?download=","");
		CString filename = theurl;
		filename.Replace("app/","");
		if (filename.Find("null")==0)
		{
			return;
		}
		filename.Replace("app/","");
		filename = filename.Left(filename.Find("&username="));
		theurl = "app/" + filename;
		//AfxMessageBox(theurl);
		//AfxMessageBox(filename);
		theurl = "http://61.181.14.184:8084/ReadingStore/"+theurl;
		DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
		ZeroMemory(missionInfo, sizeof(DownLoadMissionInfo));
		CString DstPath;
		DstPath.Format("%s\\share\\%s",CurrentPath,filename);
		//AfxMessageBox(DstPath);
		int i = 1;
		while (PathFileExists(DstPath))
		{
			DstPath.Format("%s\\share\\(%d)%s",CurrentPath,filename);
			i ++;
		}
		strcpy_s(missionInfo->DstPath, _MAX_PATH, DstPath);
		strcpy_s(missionInfo->URL, 1024, theurl);
		strcpy_s(missionInfo->title, _MAX_PATH, filename);
		missionInfo->sourceType = dl_http;
		missionInfo->from = from_rstore;
		pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
		return;
	}
	//共享资源下载
	char* tail = strstr(WebPath, "&option=download");
	if(tail){
		char* head = strstr(WebPath, "sid=");
		if(!head)
			return;
		long sid = atol(head+strlen("sid="));
		pMainWnd->PostMessage(WM_REQUEST_RESUOURCE_FROM_SERVER, 0, (LPARAM)sid);
		return;
	}	

	tail = strstr(WebPath,"operation=djxz");
	if(tail){
		CFile cf;
		CString path;
		path.Format("%s\\system\\name", CurrentPath);
		if(cf.Open(path,CFile::modeCreate|CFile::modeWrite)){
			cf.Write(UserName,strlen(UserName));
			cf.Close();
			ShellExecute(NULL,"open","./system/Readman.exe","",CurrentPath, SW_SHOW);
		}
		else{
			AfxMessageBox("程序被破坏，请您重新安装Readings程序！");
		}
	}

	if(strstr(WebPath, "flag=6")){
		char* URL = strstr(WebPath, "&key=");
		if(!URL)
			return;
		URL+=strlen("&url=");
		if(WebPath+strlen(WebPath)<=URL)
			return;

		char* key = strstr(URL,"&keyword=");
		if(!key)
			return;

		char* escKey = (char*)malloc(key-URL+1);
		memset(escKey, 0, key-URL+1);
		memcpy(escKey, URL, key-URL);

		key+=strlen("&keyword=");
		if(WebPath+strlen(WebPath)<=key)
			return;

		char* keyWord = (char*)malloc(strlen(key)+1);
		memset(keyWord, 0, strlen(key)+1);
		strcpy_s(keyWord, strlen(key)+1, key);
		
	//	pMainWnd->m_listdlg->m_findlist.m_bTerminate =TRUE;
	//	TerminateFindListThread(&(pMainWnd->m_listdlg->m_findlist));
	//	BeginSearch(keyWord, escKey, false);
	//	pMainWnd->OnSearch(keyWord);
	//	pMainWnd->PostMessage(WM_SEARCH, (WPARAM)keyWord, NULL);
	
		pMainWnd->m_MainBar.Search(keyWord);
	}

//added by zuoss 20090730
	//Readman主页重登陆，触发整个界面刷新。
	if(strstr(WebPath, "restartWholeConsole="))
	{
		string wpath(WebPath);
		string name = wpath.substr(wpath.find("restartWholeConsole=")+20,wpath.find("&version") - wpath.find("restartWholeConsole=")- 20);
		sprintf_s(UserName, 50, "%s",name.c_str());

//更改标题
		CString title = "";
		title.Format("%sv%.2fBeta——%s", SOFTWARE_NAME, VERSION, UserName);
		pMainWnd->SetWindowText(title);
//刷新左树
		pMainWnd->isfreashing = TRUE;
		pMainWnd->m_DownloadCenterView->ShowTree();
		pMainWnd->isfreashing = FALSE;
//刷新锐客树
		pMainWnd->m_pCommDlg->PostMessage(WM_USER_REFRESH,NULL,NULL);
//开启新用户通信线程
	}
//end
}
