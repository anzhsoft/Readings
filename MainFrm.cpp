// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "Readings.h"
#include "MainFrm.h"
#include "FileCollect.h"
#include "HttpDownLoadDlg.h"
#include "Dbt.h"
#include "SYNDialog.h"
#include "RegsterDialog.h"
#include "tlhelp32.h"
#include "UDiskAuth.h"
#include "GetUserInfo.h"
#include "PushAdv.h"
#include "Push.h"
#include "ConfigDlg.h"
#include "UserListCtrl.h"
#include "PathDialog.h"
#include "BackupDialog.h"
#include "digChinaBook.h"
#include "EMailADO.h"
#include "Objbase.h"
#include "UserCommDlg.h"
#include "UDiskAuth.h"
#include "HttpUtl.h"
#include "UploadShareFileList.h"
#include "BTOperation.h"

#ifdef SWITCH_WATERMARKER
#include "WaterMaker.h"
PBTNODE root = NULL;
INPUT_STATE input_state = INPUT_STATE1;
#endif
#include <string>
#include <map>
#include <iostream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL offline;
extern char CurrentPath[_MAX_PATH];
extern char JspServer[1024];
extern char AspServer[1024];
extern char UserName[50];
extern char Password[50];
extern char SDDisk[20];
extern char FavorPath[_MAX_PATH];
extern CMainFrame* pMainWnd;
extern _ConnectionPtr m_pConnection;
extern bool gbOnSearching;// Added by zhanganzhan on 20081024
extern char hostOutIp[20];
extern CUserCommDlg* pDlg;
extern USB_DETAIL_INFO uDiskAuthInfo;
extern BOOL isChatroom1;
extern BOOL isChatroom2;
extern int chatRoomID;
extern int chatRoomID1;
extern int chatRoomID2;
//=========================modified by yangxiao 20080819 begin
//映射表改到数据库中
/*
#define NUM_IPMAP_COL 2
#define NUM_IPMAP_ROW 2
static _TCHAR* IPMAP [NUM_IPMAP_ROW][NUM_IPMAP_COL] = {
_T("202.113.16") ,_T("南开大学"),
_T("218.68.91.175"),_T("津科公司")
};
*/
//=========================modified by yangxiao 20080819 end


/////////////////////////////////////////////////////////////////////////////
// CMainFrame


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_WM_CREATE()
ON_MESSAGE(WM_ICON,OnIcon)
ON_WM_DESTROY()
ON_MESSAGE(WM_SEARCH, OnSearch)
ON_MESSAGE(WM_SEARCHCACHE, OnCacheSearch)
ON_MESSAGE(WM_SEARCH_FINISHED, OnSearchFinished)
ON_MESSAGE(WM_TREEITEM_SELECTED, OnTreeItemSelected)
ON_MESSAGE(WM_USER_TREE_CLICKED,OnUserTreeClicked)
ON_MESSAGE(WM_P2P_APPENDUSERINFO, OnAppendUserInfo)
ON_MESSAGE(WM_P2P_DELETEUSERINFO, OnDeleteUserInfo)
ON_MESSAGE(WM_FILEDOWNLOAD,OnFileDownload)
ON_MESSAGE(WM_NEW_DOWNLOAD, OnNewDownload)
ON_MESSAGE(WM_UPDATE_DOWNLOAD, OnUpdateDownload)
ON_UPDATE_COMMAND_UI(ID_HIDEUI, OnUpdateHideUI)
ON_UPDATE_COMMAND_UI(ID_SHOWUI, OnUpdateShowUI)
ON_COMMAND(ID_SHOWUI, OnShowUI)
ON_COMMAND(ID_HIDEUI, OnHideUI)
ON_UPDATE_COMMAND_UI(ID_HELP, OnUpdateHelp)
ON_WM_CLOSE()
ON_COMMAND(ID_APP_EXIT, OnAppExit)
ON_WM_TIMER()
ON_MESSAGE(WM_OPEN_FILE, OpenFile)
ON_MESSAGE(WM_OPEN_URL, OpenURL)
ON_MESSAGE(WM_OPEN_URL_NOT_SHOW,OpenURLNotShow)
ON_MESSAGE(WM_OPEN_TAG_URL, OpenTagURL)
ON_MESSAGE(WM_REQUEST_RESUOURCE_FROM_SERVER, OnRequestResource)
ON_MESSAGE(WM_ADD_ORDER_ITEM, OnAddOrderItem)
ON_MESSAGE(WM_THREAD_FINISHED, OnThreadFinished)
ON_COMMAND(IDC_BUTTON_HELP, OnHelp)
ON_BN_CLICKED(IDC_BUTTON_USERADV, OnButtonUseradv)
ON_BN_CLICKED(IDC_BUTTON_HISTORY, OnButtonHistory)
ON_BN_CLICKED(IDC_BUTTON_ORDERLIST, OnButtonOrderList)
ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
ON_BN_CLICKED(IDC_BUTTON_SYN, OnSyn)
ON_MESSAGE(WM_SYN_TO_SD, OnSynToSD)
ON_COMMAND(ID_MENUITEM_LIST, OnMenuitemList)
ON_COMMAND(ID_MENUITEM_REPORT, OnMenuitemReport)
ON_UPDATE_COMMAND_UI(ID_MENUITEM_LIST, OnUpdateMenuitemList)
ON_UPDATE_COMMAND_UI(ID_MENUITEM_REPORT, OnUpdateMenuitemReport)
ON_MESSAGE(WM_APPENDGROUPINFO,OnAppendGroupInfo)
ON_MESSAGE(WM_SHOW_GROUPRESOURCE,OnShowGroupResource)
ON_MESSAGE(WM_SHOW_USERSOURCE, OnShowUserResource)
ON_COMMAND(ID_MENUITEM_CONFIG, OnMenuitemConfig)
ON_MESSAGE(WM_CREATE_FAVORFOLDER, OnCreateNewFavorFolder)
ON_MESSAGE(WM_OPENWINDOW, OpenWindow)
ON_MESSAGE(WM_CLOSEWINDOW,CloseWindow)
ON_MESSAGE(WM_UPDATE_GUIDETREECOUNT,UpdateGuidTreeCount)
ON_COMMAND(ID_MENUITEM_COMMLOG, OnMenuitemCommlog)
ON_COMMAND(ID_MENUITEM_COMMANAL, OnMenuitemCommanal)
ON_MESSAGE(WM_RECORD_BEHAVIOR,RecordBehavior)
	ON_COMMAND(ID_TOOLS_READWOLF, OnToolsReadwolf)
	ON_COMMAND(ID_TOOLS_MAKEWOLF, OnToolsMakewolf)
	ON_COMMAND(ID_MENUITEM_MODIFY, OnMenuitemModify)
	ON_COMMAND(ID_MENUITEM_SCORE, OnMenuitemScore)
ON_MESSAGE(WM_MAINFRAME_REFRESH_USER,OnRefreshUser)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_quit = FALSE;	
	m_hItem = NULL;
	m_info = NULL;
	isfreashing = FALSE;
	

	RecvDlg = NULL;
	
	m_OrderCenterView = NULL;
	m_ReadOnLineView = NULL;
	m_ManBooksView = NULL;
	m_DownloadCenterView = NULL;
	m_HotKeyView = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	this->m_bAutoMenuEnable=false;
	pMainWnd = this;
	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_tipsDlg.Create(IDD_DIALOG_TIP);
	DigChinaBook * dlgCB ;//anzhsoftPPPP
	dlgCB = new DigChinaBook;
	dlgCB->Create(IDD_DIALOG_ZST);
	dlgCB->ShowWindow(SW_RESTORE);
	
	if(!InitialToolBar())
		return -1;
	
	AddIcon(IDR_MAINFRAME);

	//启动心跳,每5s心跳一次
	Heart = new CReadingsHeart;
	Heart->Start(5);
	Heart->Break();
	SetTimer(10,5*1000,NULL);

	return 0;
}

unsigned __stdcall CMainFrame::_GetReady(LPVOID pParam)
{
	CMainFrame* p = (CMainFrame*)pParam;
	int i = 0;
	if(p){
		while (i < 2)
		{
			if (p->m_quit)
			{
				return 0;
			}
			i ++;
			Sleep(500);
		}
		//p->ResumeDownload();
		p->m_pMainListView->ShowRecentlyRead();
	}

	while (i < 20)
	{
		if (p->m_quit)
		{
			return 0;
		}
			i ++;
			Sleep(500);
	}

	CBTOperation::OpenServer();

	HANDLE handle = (HANDLE)::GetCurrentThread();
	p->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	_endthreadex(0);
	return   0;  
}

unsigned __stdcall CMainFrame::_OnLineUserRegister(LPVOID pParam)
{
	CMainFrame* p = (CMainFrame*)pParam;
	if(p)
		p->OnLineUserRegister();
	HANDLE handle = (HANDLE)::GetCurrentThread();
	p->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	_endthreadex(0);
	return 0;  //deleted by zhanganzhan
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::InitialToolBar()
{
	
	if (!m_MainBar.Create(this, IDD_MAINBAR, CBRS_LEFT,	IDD_MAINBAR))
	{
		TRACE0("Failed to create dialog bar\n");
		return FALSE;      // fail to create
	}
	
	m_MainBar.SetBarStyle(CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_MainBar.EnableDocking(CBRS_ALIGN_ANY);
	m_ReBar.Create(this);
	m_ReBar.SetBarStyle(m_ReBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY);
	m_ReBar.AddBar(&m_MainBar);
	
	return TRUE;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	//将MainFrame分成三列窗口
	if(!offline)//能与服务器相连，联机状态
	{
		if(m_colWndSplitter.CreateStatic(this,1,3)==NULL) 
			return FALSE;
		
		CRect rect;
		GetClientRect(&rect);
		
		//将后2列分别划分两行窗口
		//v0
		//v1
		m_v1WndSplitter.CreateStatic(&m_colWndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_colWndSplitter.IdFromRowCol(0,1));
		//v2
		m_v2WndSplitter.CreateStatic(&m_colWndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_colWndSplitter.IdFromRowCol(0,2));
		
		//在这5个窗口中添加视图，依次为左树抽屉，主列表，显示区，对话，下载。
		m_colWndSplitter.CreateView(0,0,RUNTIME_CLASS(CDrawerView),CSize::CSize((int)(rect.Width()*0.15),(int)(rect.Height()*0.8)),pContext);
		
		m_v1WndSplitter.CreateView(0,0,RUNTIME_CLASS(CMainListView),CSize::CSize((int)(rect.Width()*0.7),(int)(rect.Height()*0.8)),pContext);
		m_v1WndSplitter.CreateView(1,0,RUNTIME_CLASS(CShowView),CSize::CSize((int)(rect.Width()*0.7),(int)(rect.Height()*0.2)),pContext);
		
		m_v2WndSplitter.CreateView(0,0,RUNTIME_CLASS(CCommView),CSize::CSize((int)(rect.Width()*0.15),(int)(rect.Height()*0.8)),pContext);
		m_v2WndSplitter.CreateView(1,0,RUNTIME_CLASS(CDownLoadListView),CSize::CSize((int)(rect.Width()*0.15),-1),pContext);
		
		//设置v0，v1，v2三列的最合适列宽和最小列宽。
		m_colWndSplitter.SetColumnInfo(0, 195, 0);
		m_colWndSplitter.SetColumnInfo(1, 1024-195-182, 0);
		m_colWndSplitter.SetColumnInfo(2, 182, 0);
		m_colWndSplitter.RecalcLayout();

		//对每列，分别设置行高。
		m_v1WndSplitter.SetRowInfo(0, 0, 0);
		m_v1WndSplitter.SetRowInfo(1, rect.Height(), 0);
		m_v1WndSplitter.RecalcLayout();
		
		m_v2WndSplitter.SetRowInfo(0, 800, 0);
		m_v2WndSplitter.SetRowInfo(1, 0, 0);
		m_v2WndSplitter.RecalcLayout();
		
		//将每一块视图面板赋值给相应类的对象。
		m_Drawers = (CDrawerView*)m_colWndSplitter.GetPane(0,0);
				
		m_pMainListView = (CMainListView*)m_v1WndSplitter.GetPane(0,0);
		m_pMainList = m_pMainListView->m_plist;
		
		m_pShowView = (CShowView*)m_v1WndSplitter.GetPane(1,0);
		
		m_pCommView = (CCommView*)m_v2WndSplitter.GetPane(0,0);
		m_pCommDlg = &(m_pCommView->m_CommDlg);
	
		m_pDownLoadListView = (CDownLoadListView*)m_v2WndSplitter.GetPane(1,0);
	}
	else
	{
		if(m_colWndSplitter.CreateStatic(this,1,3)==NULL) 
			return FALSE;
		
		CRect rect;
		GetClientRect(&rect);
		
		//将后2列分别划分两行窗口
		//v0
		//v1
		m_v1WndSplitter.CreateStatic(&m_colWndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_colWndSplitter.IdFromRowCol(0,1));
		//v2
		m_v2WndSplitter.CreateStatic(&m_colWndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_colWndSplitter.IdFromRowCol(0,2));
		
		//在这5个窗口中添加视图，依次为左树，主列表，显示区，对话，下载。
		m_colWndSplitter.CreateView(0,0,RUNTIME_CLASS(CDrawerView),CSize::CSize((int)(rect.Width()*0.15),(int)(rect.Height()*0.8)),pContext);	
		m_v1WndSplitter.CreateView(0,0,RUNTIME_CLASS(CMainListView),CSize::CSize((int)(rect.Width()*0.7),(int)(rect.Height()*0.8)),pContext);
		
		m_v1WndSplitter.CreateView(1,0,RUNTIME_CLASS(CShowView),CSize::CSize((int)(rect.Width()*0.7),(int)(rect.Height()*0.2)),pContext);
		m_v2WndSplitter.CreateView(0,0,RUNTIME_CLASS(CCommView),CSize::CSize((int)(rect.Width()*0.15),(int)(rect.Height()*0.8)),pContext);
		m_v2WndSplitter.CreateView(1,0,RUNTIME_CLASS(CDownLoadListView),CSize::CSize((int)(rect.Width()*0.15),-1),pContext);
		
		//设置v0，v1，v2三列的最合适列宽和最小列宽。
		m_colWndSplitter.SetColumnInfo(0, 195, 0);
		m_colWndSplitter.SetColumnInfo(1, 1024-195-182, 0);
		m_colWndSplitter.SetColumnInfo(2, 182, 0);
		m_colWndSplitter.RecalcLayout();
		
		//对每列，分别设置行高。
		m_v1WndSplitter.SetRowInfo(0, 0, 0);
		m_v1WndSplitter.SetRowInfo(1, rect.Height(), 0);
		m_v1WndSplitter.RecalcLayout();
		
		m_v2WndSplitter.SetRowInfo(0, 800, 0);
		m_v2WndSplitter.SetRowInfo(1, 0, 0);
		m_v2WndSplitter.RecalcLayout();
		
		//将每一块视图面板赋值给相应类的对象。
		m_Drawers = (CDrawerView*)m_colWndSplitter.GetPane(0,0);

		m_pMainListView = (CMainListView*)m_v1WndSplitter.GetPane(0,0);
		m_pMainList = m_pMainListView->m_plist;
		
		m_pShowView = (CShowView*)m_v1WndSplitter.GetPane(1,0);
		
		m_pCommView = (CCommView*)m_v2WndSplitter.GetPane(0,0);
		m_pCommDlg = &(m_pCommView->m_CommDlg);
		
		m_pDownLoadListView = (CDownLoadListView*)m_v2WndSplitter.GetPane(1,0);
	}
	HANDLE handle=(HANDLE)_beginthreadex(NULL,0,_GetReady,this,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
	m_threadList.AddTail(handle);
	
	return CFrameWnd::OnCreateClient(lpcs, pContext);
	
}

void CMainFrame::AddIcon(int IconID)
{
	NOTIFYICONDATA nid = {0};
	//填写一个图标数据结构
	nid.cbSize = sizeof(NOTIFYICONDATA);		//该数据结构的大小
	nid.hWnd = m_hWnd;					//当前窗口的句柄
	nid.uID = 10;								//用户ID
	strcpy_s(nid.szTip, 128, SOFTWARE_NAME);			//
	nid.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IconID));
	nid.uCallbackMessage = WM_ICON;
	nid.uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
	Shell_NotifyIcon(NIM_ADD,&nid);
}

void CMainFrame::ModifyIcon(int IconID)
{
	NOTIFYICONDATA nid = {0};
	//填写一个图标数据结构
	nid.cbSize = sizeof(NOTIFYICONDATA);		//该数据结构的大小
	nid.hWnd = m_hWnd;					//当前窗口的句柄
	nid.uID = 10;								//用户ID
	strcpy_s(nid.szTip, 128, SOFTWARE_NAME);			//
	nid.hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IconID));
	nid.uCallbackMessage = WM_ICON;
	nid.uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
	Shell_NotifyIcon(NIM_MODIFY,&nid);
}


void CMainFrame::DeleteIcon(int IconID)
{
	NOTIFYICONDATA nid = {0};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = m_hWnd;
	nid.uID = 10;
	strcpy_s(nid.szTip, 128, SOFTWARE_NAME);
	nid.hIcon=::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IconID));
	nid.uFlags=NIF_ICON|NIF_TIP;
	Shell_NotifyIcon(NIM_DELETE,&nid);
}

LRESULT CMainFrame::OnIcon(WPARAM wParam, LPARAM lParam)
{
	if(LOWORD(lParam)==WM_LBUTTONDBLCLK)
		ShowHide();
	if (LOWORD(lParam)==WM_RBUTTONDOWN)
		ShowMenu();
	return TRUE;
}

void CMainFrame::ShowHide()
{
	if(IsWindowVisible()){
		ShowWindow(SW_HIDE);
	}
	else{
		ShowWindow(SW_SHOWMAXIMIZED);
		SetForegroundWindow();
	}
}

void CMainFrame::OnDestroy() 
{
	TernminateAllThread(m_socketList,m_threadList);
//	POSITION pos = m_threadList.GetHeadPosition();
// 	while (pos)
// 	{
// 		HANDLE hd = m_threadList.GetNext(pos);
// 		TerminateThread(hd,0);
// 	}
	//TernminateAllThread(m_pDownLoadListView->m_sockList,m_pDownLoadListView->m_threadList);
	::SetEvent(Heart->hEvent);
	CFrameWnd::OnDestroy();
	DeleteIcon(IDR_MAINFRAME);
}

LRESULT CMainFrame::OnSearch(WPARAM w,LPARAM l)
{
	LPSTR keyword = (LPSTR)w;
	try{
		//m_colWndSplitter.Right2Right();
		m_v1WndSplitter.ToUp();//zuoss 20081031
		m_pMainListView->page = 1;
		m_pMainListView->page1 = 0;
		m_pMainListView->keyWord = keyword;
		m_pMainListView->Search(keyword);
	}
	catch(...){
#ifdef _DEBUG
		Tips("LRESULT CMainFrame::OnSearch(LPSTR keyword)");
#endif
	}
	return TRUE;
}
LRESULT CMainFrame::OnCacheSearch(WPARAM w,LPARAM l)
{
	TreeItemInfo* info = (TreeItemInfo*)w;
	LPSTR keyword = (LPSTR)l;
	if (info->Type==Is_NetGroup_Level)
	{
		//设定当前节点ID
		this->curID = info->BookNumber;
		m_pMainListView->SearchCache(info->Type,info->BookNumber,keyword, 1);
	}
	else
	{
		GROUPINFO* ginfo = (GROUPINFO*)info->BookNumber;
		this->curID = ginfo->gid;
		m_pMainListView->SearchCache(info->Type,ginfo->gid,keyword, 1);
	}
	return TRUE;
}
LRESULT CMainFrame::OnSearchFinished(WPARAM w,LPARAM l)
{
	m_MainBar.PostMessage(WM_SEARCH_FINISHED);
    gbOnSearching = FALSE;
	return TRUE;
}
// added by zhanganzhan on 20090410 for usertree clicked for showing resources.
LRESULT CMainFrame::OnUserTreeClicked(WPARAM w,LPARAM l)
{
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)w; 
	if (isfreashing)
	{
		return TRUE;
	}
	HTREEITEM hItem = NULL;

	TreeItemInfo* info = NULL;
	hItem = m_pCommDlg->m_tree.GetSelectedItem();
	info = (TreeItemInfo*)m_pCommDlg->m_tree.GetItemData(hItem);
	if (info->Type!=Is_NetFirend && info->Type!=Is_NetFriendList&&info->Type!=Is_Chatroom)
	{
		return FALSE;//当前树的双击只是响应右侧用户树的资源显示。
	}
		
	m_pMainListView->totalPage = 0;
	m_pMainListView->totalPage1 = 0;
	m_pMainListView->totalCount = 0;
	m_pMainListView->page = 1;
	m_pMainListView->page1 = 0;
	ShowResourceShareFiletype(info);
	return TRUE;
}

LRESULT CMainFrame::OnTreeItemSelected(WPARAM w,LPARAM l)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)w;
	int treetype = (int)l;
	if (isfreashing)
	{
		return TRUE;
	}
	HTREEITEM hItem = NULL;

	TreeItemInfo* info = NULL;
	if(thing_tree == treetype)
	{
		hItem = m_pSelectedTree->GetSelectedItem();
		info = (TreeItemInfo*)m_pSelectedTree->GetItemData(hItem);
	}
	else if(body_tree == treetype)
	{
		hItem = m_pCommDlg->m_tree.GetSelectedItem();
		info = (TreeItemInfo*)m_pCommDlg->m_tree.GetItemData(hItem);
	//	AfxMessageBox(CString(info->IndexID));//zhanganzhan0322
	}
	else
		return TRUE;
	
	m_pMainListView->totalPage = 0;
	m_pMainListView->totalPage1 = 0;
	m_pMainListView->totalCount = 0;
	m_pMainListView->page = 1;
	m_pMainListView->page1 = 0;
	Operation(hItem, info);

	return TRUE;
}

void CMainFrame::ShowFolder(HTREEITEM hItem, CString root)
{
	//m_colWndSplitter.Right2Right();
	m_v1WndSplitter.ToUp();
	
	CString relativePath;
	TreeItemInfo* info = NULL;
	HTREEITEM hCurItem = hItem;
	do{
		CString tempPath = relativePath;
		CString name = m_pManBooksTree->GetItemText(hCurItem);
		int index = -1;
		if((index = name.ReverseFind('('))<0)
			relativePath.Format("%s\\%s", name, tempPath);
		else
			relativePath.Format("%s\\%s", name.Left(index), tempPath);
		hCurItem = m_pManBooksTree->GetParentItem(hCurItem);
		if(!hCurItem)
			return;
		info = (TreeItemInfo*)m_pManBooksTree->GetItemData(hCurItem);
	}while(info->Type==Is_MyFavourate_Folder||info->Type==Is_SDCard_Folder);
	
	CString curFolderPath;
	curFolderPath.Format("%s\\%s", root, relativePath);
	
	if(info->Type == Is_MyFavourate)
		m_pMainListView->ShowFolder(curFolderPath,Is_FileList);
	else
		m_pMainListView->ShowFolder(curFolderPath,Is_SDDiskList);
}


void CMainFrame::ShowCollectYearFile(TreeItemInfo *info)
{
	//m_colWndSplitter.Right2Right();
	m_v1WndSplitter.ToUp();
	m_pMainListView->ShowCollectYearFile(info->BookNumber,atoi(info->IndexID), m_pMainListView->page);
}

LRESULT CMainFrame::OnAppendGroupInfo(WPARAM w,LPARAM l)
{
	LPSTR buffer = (LPSTR)w;
	//	CUserListCtrl& list = m_pCommDlg->m_grouplist;
	GROUPINFOHEAD* head = (GROUPINFOHEAD*)(buffer+1);
	for(int nIndex=0; nIndex<head->num; nIndex++){
		GROUPINFO* info = (GROUPINFO*)malloc(sizeof(GROUPINFO));
		long offset = sizeof(GROUPINFOHEAD)+sizeof(GROUPINFO)*nIndex+1;
		memcpy(info,(char*)(buffer+offset),	sizeof(GROUPINFO));
		if(strlen(info->groupname)){
		/*
		BOOL ext = FALSE;
		int ncount = list.GetItemCount();
		for(int index=0; index<ncount; index++){
		CString name = list.GetItemText(index,0);
		if(name.CompareNoCase(info->groupname)==0){
		ext = TRUE;
		break;
		}
		}
		if(!ext){
		list.InsertItem(ncount, info->groupname, ncount);
		list.SetImage(info->groupname[0], ncount);
		list.SetItemData(ncount, (DWORD)info);
		}
		else
		free(info);
			*/
			HTREEITEM hItem_temp = m_pCommDlg->m_tree.GetChildItem(m_pCommDlg->hGroup);
			BOOL ext = FALSE;
			DWORD count = 0;
			while(hItem_temp){
				CString str = m_pCommDlg->m_tree.GetItemText(hItem_temp);
				if(str.Compare(info->groupname)==0){
					ext = TRUE;
				}
				hItem_temp = m_pCommDlg->m_tree.GetNextSiblingItem(hItem_temp);
				count++;
			}
			if(!ext){
				CString title;
				title.Format("我的群落(%d个)", count+1);
				m_pCommDlg->m_tree.SetItemText(m_pCommDlg->hGroup, title);
				HTREEITEM hItem = m_pCommDlg->m_tree.InsertItem(info->groupname, 28, 28, m_pCommDlg->hGroup);
				m_pCommDlg->m_tree.SetTreeItemData(hItem,Is_NetGroup,NULL,0,(DWORD)info);
			}
			
		}
		else
			free(info);
	}
	free(buffer);
	m_pCommDlg->m_tree.Invalidate(FALSE);
	return TRUE;
}

//add by yangxiao 20080812
bool isFirst = true;

LRESULT CMainFrame::OnAppendUserInfo(WPARAM w,LPARAM l)
{
	//========================================== add by yangxiao 20080812 begin
	//for debug
	//CString tmp = ::UserName;
	LPSTR buffer = (LPSTR)w;
	if (isFirst)
	{
		STRUCTUSERINFOHEAD* head1 = (STRUCTUSERINFOHEAD*)(buffer+1);
		for(int nIndex1=0; nIndex1<head1->num; nIndex1++)
		{
			STRUCTUSERINFO* info1 = (STRUCTUSERINFO*)malloc(sizeof(STRUCTUSERINFO));
			long offset1 = sizeof(STRUCTUSERINFOHEAD)+sizeof(STRUCTUSERINFO)*nIndex1+1;
			memcpy(info1,(char*)(buffer+offset1),	sizeof(STRUCTUSERINFO));
			if((strlen(info1->UserName)!=0)&& (strcmp(info1->UserName ,::UserName) == 0))
			{
				//hostOutIp = info->IP;
				strcpy_s(hostOutIp, 20, info1->IP);
				isFirst = false;
				break;
			}
		}
	}
	//=============================== add by yangxiao 20080812 end
	//	CUserListCtrl& list = m_pCommDlg->m_userlist;
	STRUCTUSERINFOHEAD* head = (STRUCTUSERINFOHEAD*)(buffer+1);
	for(int nIndex=0; nIndex<head->num; nIndex++){
		STRUCTUSERINFO* info = (STRUCTUSERINFO*)malloc(sizeof(STRUCTUSERINFO));
		long offset = sizeof(STRUCTUSERINFOHEAD)+sizeof(STRUCTUSERINFO)*nIndex+1;
		memcpy(info,(char*)(buffer+offset),	sizeof(STRUCTUSERINFO));
		if(strlen(info->UserName)){
		/*
		BOOL ext = FALSE;
		int ncount = list.GetItemCount();
		for(int index=0; index<ncount; index++){
		CString name = list.GetItemText(index,0);
		if(name.CompareNoCase(info->UserName)==0){
		ext = TRUE;
		break;
		}
		}
		if(!ext){
		list.InsertItem(ncount, info->UserName, ncount);
		list.SetImage(info->UserName[0], ncount);
		list.SetItemData(ncount, (DWORD)info);
		}
		else
		free(info);
			*/
			//	if(strcmp(info->UserName, UserName)!=0){
			HTREEITEM hItem_temp = m_pCommDlg->m_tree.GetChildItem(m_pCommDlg->hFriend);
			BOOL ext = FALSE;
			DWORD count = 0;
			while(hItem_temp)
			{
				//zuoss 090310
				CString str = m_pCommDlg->m_tree.GetItemText(hItem_temp);
				CString sname = "";
				sname = str.Right(str.GetLength()-str.Find('-')-1);
				sname = sname.Left(sname.Find('('));
				//end
				if(sname.Compare(info->UserName)==0)
				{
					TreeItemInfo* tinfo = (TreeItemInfo*)m_pCommDlg->m_tree.GetItemData(hItem_temp);
					CString lsPath;
					free((STRUCTUSERINFO*)tinfo->BookNumber);
					m_pCommDlg->m_tree.DeleteItem(hItem_temp);
					//ext = TRUE;
				}
				hItem_temp = m_pCommDlg->m_tree.GetNextSiblingItem(hItem_temp);
				count++;
			}
			if(!ext){
				CString title;
				title.Format("锐客好友(%d人)", count+1);
				m_pCommDlg->m_tree.SetItemText(m_pCommDlg->hFriend, title);
				
				//modified by yangxiao 20080812 begin
				CString ftitle;
				
			
							
				ftitle.Format("%s-%s(%d)",info->NetName,info->UserName,info->shareFileNum);

				//modified by yangxiao 20080812 end
				
				HTREEITEM hItem = m_pCommDlg->m_tree.InsertItem(/*info->UserName,*/ftitle, 27, 27, m_pCommDlg->hFriend);
				TreeItemInfo* tinfo = (TreeItemInfo*)malloc(sizeof(TreeItemInfo));
				ZeroMemory(tinfo, sizeof(TreeItemInfo));
				tinfo->Type = Is_NetFirend;
				tinfo->BookNumber = (DWORD)info;
				m_pCommDlg->m_tree.SetItemData(hItem,(DWORD)tinfo);
			}
			//	}
			//	else
			//		free(info);
		}
		else
			free(info);
	}
	free(buffer);
	m_pCommDlg->m_tree.Invalidate(FALSE);
	return TRUE;
}

LRESULT CMainFrame::OnDeleteUserInfo(WPARAM w,LPARAM l)
{
	//	CUserListCtrl& list = m_pCommDlg->m_userlist;
	char * buffer = (char*)w;
	STRUCTUSERINFOHEAD* head = (STRUCTUSERINFOHEAD*)(buffer+1);
	for(int nIndex=0; nIndex<head->num; nIndex++){
		long offset = sizeof(STRUCTUSERINFOHEAD)+sizeof(STRUCTUSERINFO)*nIndex+1;
		STRUCTUSERINFO* info = (STRUCTUSERINFO*)(buffer+offset);
		if(info->UserName){
		/*
		BOOL ext = FALSE;
		int ncount = list.GetItemCount();
		int index;
		for(index=0; index<ncount; index++){
		CString name = list.GetItemText(index,0);
		if(name.CompareNoCase(info->UserName)==0){
		ext = TRUE;
		break;
		}
		}
		if(ext){
		list.DeleteItem(index);
		list.RemoveImage(index);
		}
			*/
			HTREEITEM hItem = m_pSelectedTree->GetChildItem(m_pCommDlg->hFriend);
			BOOL ext = FALSE;
			DWORD count = 0;
			while(hItem)
			{
				CString str = m_pCommDlg->m_tree.GetItemText(hItem);
				//zuoss 090310
				CString sname = "";
				sname = str.Right(str.GetLength()-str.Find('-')-1);
				//AfxMessageBox(sname);
				sname = sname.Left(sname.Find('('));
				//AfxMessageBox(sname);

				//end 
				if(sname.Compare(info->UserName)==0){
					TreeItemInfo* tinfo = (TreeItemInfo*)m_pCommDlg->m_tree.GetItemData(hItem);
					CString lsPath;
					lsPath.Format("%s\\temp\\listCache\\%s.ls", CurrentPath, info->UserName);
					DeleteFile(lsPath);
					free((STRUCTUSERINFO*)tinfo->BookNumber);
					m_pCommDlg->m_tree.DeleteItem(hItem);
				}
				else
					count++;
				hItem = m_pCommDlg->m_tree.GetNextSiblingItem(hItem);
			}
			CString title;
			title.Format("锐客好友(%d人)", count);
			m_pCommDlg->m_tree.SetItemText(m_pCommDlg->hFriend, title);
		}
	}
	free(buffer);
	return TRUE;
}

void CMainFrame::ConnectNetFriend(TreeItemInfo *info, CString FriendName)
{
	STRUCTUSERINFO* uinfo = (STRUCTUSERINFO*)(info->BookNumber);
	if(!uinfo)
		return;
	
	STRUCTUSERINFO* ui = (STRUCTUSERINFO*)malloc(sizeof(STRUCTUSERINFO));
	memcpy(ui, uinfo, sizeof(STRUCTUSERINFO));
	
	LPSTR fn = (LPSTR)malloc(FriendName.GetLength()+1);
	ZeroMemory(fn, FriendName.GetLength()+1);
	strcpy_s(fn, FriendName.GetLength()+1, FriendName);
	
	m_pCommDlg->PostMessage(WM_P2P_CONNECT_FRIEND, (WPARAM)ui, (LPARAM)fn);
}

LRESULT CMainFrame::OnFileDownload(WPARAM w,LPARAM l)
{
	DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)w;
	m_pDownLoadListView->PostMessage(WM_FILEDOWNLOAD, (WPARAM)missionInfo);
	return TRUE;
}
LRESULT CMainFrame::OnNewDownload(WPARAM w,LPARAM l){
	m_pDownLoadListView->PostMessage(WM_NEW_DOWNLOAD, w);
	return true;
}
LRESULT CMainFrame::OnUpdateDownload(WPARAM w,LPARAM l){
    m_pDownLoadListView->PostMessage(WM_UPDATE_DOWNLOAD, w);
	return true;
}
LRESULT CMainFrame::UpdateGuidTreeCount(WPARAM w,LPARAM l)
{ //anzhsoftcnt
	CString title = m_pDownloadCenterTree->GetItemText(m_hItem);
	title = title.Left(title.Find('('));
	CString newtitle;
	newtitle.Format("%s(%d)",title,(int)w);
	m_pDownloadCenterTree->SetItemText(m_hItem,newtitle);
	return TRUE;
}
void CMainFrame::ShowMenu()
{
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = 10;
	strcpy_s(nid.szTip, 128, SOFTWARE_NAME);
	nid.hIcon=::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	nid.uCallbackMessage =WM_ICON;
	nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
	
	CMenu menu;
	menu.LoadMenu(IDR_TRAYMENU);
	CMenu* submenu = menu.GetSubMenu(0);
	
	if(IsWindowVisible())
		submenu->DeleteMenu(0, MF_BYPOSITION);
	else
		submenu->DeleteMenu(1, MF_BYPOSITION);
	
	submenu->SetDefaultItem(0, TRUE);
	
	CPoint pt;
	GetCursorPos(&pt);
	::SetForegroundWindow(nid.hWnd);  
	::TrackPopupMenu(submenu->m_hMenu,TPM_LEFTALIGN,pt.x,pt.y,0,nid.hWnd,NULL);
	::PostMessage(nid.hWnd, WM_NULL, 0, 0);
	menu.DestroyMenu();
	return;
	
}

void CMainFrame::OnUpdateHideUI(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateShowUI(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);	
}

void CMainFrame::OnShowUI() 
{
	ShowHide();
}

void CMainFrame::OnHideUI() 
{
	ShowHide();
}

void CMainFrame::OnUpdateHelp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnClose() 
{
	if(!m_quit)
		ShowHide();
	else
		CFrameWnd::OnClose();
}

void CMainFrame::OnAppExit() 
{
	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	char val[50] = {0};
	GetPrivateProfileString("normal", "quitAlert", NULL, val, 50, iniPath);
	if(atoi(val)){
		if(IDYES != MessageBox("是否要关闭Readings?","提示",MB_YESNO))
			return;
	}
	m_quit = TRUE;
//	pDlg->quit = TRUE;
// 	POSITION pos = m_threadList.GetHeadPosition();
// 	while (pos)
// 	{
// 		HANDLE hd = m_threadList.GetNext(pos);
// 		TerminateThread(hd,0);
// 	}
	PostMessage(WM_CLOSE);
}

UINT CMainFrame::OnLineUserRegister()
{
	CString URL, Tadd;
	URL.Format("%s/logon/userlogon.asp?username=%s", AspServer, UserName);
	Tadd.Format("%s/temp/logon.tmp", CurrentPath);
	if(DownLoad(URL, Tadd, m_quit, m_socketList))
		DeleteFile(Tadd);
	return 0;
}

void CMainFrame::StopSearch()
{
	m_pMainListView->StopThread();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1){
		//状态保持
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_OnLineUserRegister,this,CREATE_SUSPENDED,NULL);
		m_threadList.AddTail(handle);
		ResumeThread(handle);
	}
	else if(nIDEvent==4){
		//Removeable Disk
		KillTimer(4);
		InsertSDDisk();
	}
	else if(nIDEvent==5){
		static BOOL flag = TRUE;
		if(flag)
			pMainWnd->ModifyIcon(IDI_ICON_EMPTY);
		else
			pMainWnd->ModifyIcon(IDI_ICON_MSG);
		flag = !flag;
		if(IsWindowVisible()){
			pMainWnd->ModifyIcon(IDR_MAINFRAME);
			KillTimer(5);
		}
	}

	//added by zhanganzhan on 20090319 for web tech.
	
	else if (nIDEvent == 10)
	{
		Heart->Break();
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

LRESULT CMainFrame::OpenFile(WPARAM w,LPARAM l)
{
	LPSTR FilePath = (LPSTR)w;
	int FileType = GetFileTypeFromFull(FilePath);
	switch(FileType){
	case wolFileType:
	case txtFileType:
		m_v1WndSplitter.ToTop();
		//		m_colWndSplitter.Right2Right();
	}
	m_pShowView->OpenFile(FilePath);
	free(FilePath);
	return TRUE;
}

LRESULT CMainFrame::OpenURL(WPARAM w,LPARAM l)
{
	//anzhsoft
	LPSTR URL = (LPSTR)w;
	m_v1WndSplitter.ToUp();
	m_pShowView->OpenURL(URL);
	free(URL);
	return TRUE;
}
LRESULT CMainFrame::OpenURLNotShow(WPARAM w,LPARAM l)
{
	LPSTR URL = (LPSTR)w;
	m_pShowView->OpenURL(URL);
	free(URL);
	return TRUE;
}
LRESULT CMainFrame::OpenTagURL(WPARAM w,LPARAM l)
{
	LPSTR URL = (LPSTR)w;
	m_v1WndSplitter.ToUp();
	m_pShowView->OpenURL(URL);
	free(URL);
	return TRUE;
}
LRESULT CMainFrame::OnRefreshUser(WPARAM wParam,LPARAM lParam)
{
	Heart->RefreshUser();
	return true;
}
LRESULT CMainFrame::OnRequestResource(WPARAM reserve, long sid)
{
	m_pCommDlg->PostMessage(WM_REQUEST_RESUOURCE_FROM_SERVER, (WPARAM)reserve, (LPARAM)sid);
	return TRUE;
}

void CMainFrame::ShowOrderPaper(TreeItemInfo *info, HTREEITEM hItem)
{
	char BinFileName[_MAX_PATH] = {0};
	sprintf_s(BinFileName, _MAX_PATH, "%s/temp/order_%d.bin", CurrentPath, info->TotlePage);
	char IbxFileName[_MAX_PATH] = {0};
	sprintf_s(IbxFileName, _MAX_PATH, "%s/temp/order_%d.ibx", CurrentPath, info->TotlePage);
	char WebPath[_MAX_PATH] = {0};
	sprintf_s(WebPath, _MAX_PATH, "%s/order/newsection1.asp?fid=%d&userid=%s", AspServer, info->TotlePage, UserName);
	CHttpDownLoadDlg dlg;
	dlg.m_URL = WebPath;
	dlg.m_Path = IbxFileName;
	if(dlg.DoModal()!=IDOK)
		return;
	if(!SaveOrderType(IbxFileName, BinFileName))
		return;

	m_pMainListView->ShowOrderItem(BinFileName);
}

BOOL CMainFrame::SaveOrderType(LPCSTR IbxFileName, LPCSTR BinFileName)
{
	FILE* rfp = fopen(IbxFileName,"rb");
	if(!rfp)
		return FALSE;
	
	fseek(rfp, 0, SEEK_END);
	long len = ftell(rfp);
	if(len<=10){
		fclose(rfp);
		return FALSE;
	}
	
	fseek(rfp, -10, SEEK_END);
	
	char tmp[20]= {0};
	int n = fread(tmp, 1, 10, rfp);
	if(atol(tmp)!=len-10){
		fclose(rfp);
		return FALSE;
	}
	
	fseek(rfp, 0, SEEK_SET);
	
	char* buf = (char*)malloc(len-10+1);
	memset(buf, 0, len-10+1);
	fread(buf, 1, len-10, rfp);
	fclose(rfp);
	
	FILE* wfp = fopen(BinFileName, "wb");
	char* p = buf;
	while(p){
		char type[7][210] = {0};
		int i = 0;
		int num = 0;
		char* chr = p;
		while(i < 12){
			if(strlen(chr) == 0)
				break;
			if(chr[0] == ','){
				i++;
				num = 0;
				chr++;
				continue;
			}
			if(chr[0] == ';')
				break;
			type[i][num++] = chr[0];
			chr++;
		}
		p = chr+1;
		if(*p==0 || *chr==0)
			p=NULL;
		OrderDetailInfo info = {0};
		strcpy_s(info.name, 200, type[0]);
		strcpy_s(info.id, 50, type[1]);
		strcpy_s(info.Last, 50, type[2]);
		strcpy_s(info.Cycle, 50, type[3]);
		info.type = atoi(type[4]);
		if(atoi(type[5])>0)
			info.checked = true;
		else
			info.checked = false;
		
		fwrite(&info, 1, sizeof(OrderDetailInfo), wfp);
	}
	fclose(wfp);
	free(buf);
	return TRUE;
}

LRESULT CMainFrame::OnAddOrderItem(WPARAM w,LPARAM l)
{
	FINDINFO* info = (FINDINFO*)w; 
	if(info->abst3[0]=='3'){
		HTREEITEM hItem = m_pOrderCenterTree->GetChildItem(m_OrderCenterView->hpaperItem);
		BOOL ext = FALSE;
		while(hItem){
			CString str = m_pOrderCenterTree->GetItemText(hItem);
			if(str.Compare(info->title)==0){
				ext = TRUE;
				break;
			}
			hItem = m_pOrderCenterTree->GetNextSiblingItem(hItem);
		}
		if(!ext){
			hItem = m_pOrderCenterTree->InsertItem(info->title, 12, 12, m_OrderCenterView->hpaperItem);
			m_OrderCenterView->SetTreeItemData(hItem,Is_OrderPaper_Node,NULL,(WORD)atoi(info->abst3+1));
			m_OrderCenterView->Invalidate(FALSE);
		}
	}
	else if(info->abst3[0]=='4'){
		char* title = strrchr(info->abst3, ';');
		if(title){
			title = title+1;
			HTREEITEM hItem = m_pOrderCenterTree->GetChildItem(m_OrderCenterView->hmagItem);
			BOOL ext = FALSE;
			while(hItem){
				CString str = m_pOrderCenterTree->GetItemText(hItem);
				if(str.Compare(title)==0){
					ext = TRUE;
					break;
				}
				hItem = m_pOrderCenterTree->GetNextSiblingItem(hItem);
			}
			if(!ext){
				hItem = m_pOrderCenterTree->InsertItem(title, 12, 12,m_OrderCenterView->hmagItem);
				m_OrderCenterView->SetTreeItemData(hItem,Is_OrderMagzine_Node,NULL,(WORD)atoi(info->abst3+1));
				m_OrderCenterView->Invalidate(FALSE);
			}
		}
	}
	free(info);
	return TRUE;
}


//清理更新缓存
unsigned __stdcall _ConductCache(LPVOID pParam)
{
	
	return 0;
}

LRESULT CMainFrame::OnThreadFinished(WPARAM w,LPARAM l)
{
	HANDLE handle = (HANDLE)w;
	try{
		POSITION pos = m_threadList.Find(handle);
		if(pos)
			m_threadList.RemoveAt(pos);
	}
	catch(...){
#ifdef _DEBUG
		AfxMessageBox("OnThreadFinished");
#endif
	}
	return TRUE;
}

void CMainFrame::ShowResourceShareFiletype(TreeItemInfo* info)
{
	//需要修改
	m_v1WndSplitter.ToUp();
	//m_colWndSplitter.Right2Right();
	CHttpDownLoadDlg* dlg = new CHttpDownLoadDlg;
	if(info->Type == Is_ResourceShare_FileType){
		dlg->m_URL.Format("%s/share/share.asp?filetype=%d&page=%d&order=%d&basc=%d",AspServer,info->BookNumber,m_pMainListView->page,m_pMainListView->orderstyle,m_pMainListView->basc);
		dlg->m_Path.Format("%s\\temp\\share.lst", CurrentPath);
	}
	else if(info->Type ==Is_ResourceShare_Hot){
		dlg->m_URL.Format("%s/share/hot.asp?page=%d&order=%d&basc=%d", AspServer, m_pMainListView->page,m_pMainListView->orderstyle,m_pMainListView->basc);
		dlg->m_Path.Format("%s\\temp\\share.lst", CurrentPath);
	}
	else if(info->Type == Is_NetGroup)
	{
		GROUPINFO* ginfo = (GROUPINFO*)info->BookNumber;
		dlg->m_URL.Format("%s/share/groupxml.asp?gid=%d&page=%d&order=%d&basc=%d",AspServer,ginfo->gid, m_pMainListView->page,m_pMainListView->orderstyle,m_pMainListView->basc);
		LPSTR url = (LPSTR)calloc(1024,sizeof(char));
		sprintf_s(url, 1024, "%s/share/group.asp?gid=%d", AspServer, ginfo->gid);
		OpenURL((WPARAM)url);
	}
	else if(info->Type == Is_NetFirend)
	{
		STRUCTUSERINFO* uinfo = (STRUCTUSERINFO*)info->BookNumber;
		//pMainWnd->m_pMainListView->ShowFriendShareList(uinfo->UserName);
		//return;
		dlg->m_URL.Format("http://61.181.14.184:6969/ClientLogin.html?uploader=%s&page=%d",uinfo->UserName, m_pMainListView->page);
		//dlg->m_URL.Format("http://61.181.14.184:6969/SearchUser.html?user=%s&page=%d", uinfo->UserName,m_pMainListView->page);
		dlg->m_Path.Format("%s\\temp\\share_netfriend.xml", CurrentPath);
	}
	//added by zhanganzhan on 20090414 for proxy resources.
	else if (info->Type == Is_NetFriendList)
	{
		int tmp;
		if (TRUE == isChatroom1)
		{
			tmp=chatRoomID1;
		}
		else if (TRUE == isChatroom2)
		{
			tmp= chatRoomID2;
		}
		else
			tmp = MAX_CHATROOM_ID;
		
		dlg->m_URL.Format("%s/share/sharexml_accord_chatroomid.asp?chatroomid=%d&page=%d&order=%d&basc=%d",AspServer,chatRoomID1, m_pMainListView->page,m_pMainListView->orderstyle,m_pMainListView->basc);
		dlg->m_Path.Format("%s\\temp\\share_chatroom.lst", CurrentPath);
	}
	else if (info->Type == Is_Chatroom)
	{
		dlg->m_URL.Format("%s/share/sharexml_accord_chatroomid.asp?chatroomid=%d&page=%d&order=%d&basc=%d",AspServer,chatRoomID2, m_pMainListView->page,m_pMainListView->orderstyle,m_pMainListView->basc);
		dlg->m_Path.Format("%s\\temp\\share_chatroom.lst", CurrentPath);
	}
	else
		return;
	
	DeleteFile(dlg->m_Path);
	
	HANDLE handle=(HANDLE)_beginthreadex(NULL,0,_ShowDownInfo,dlg,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
	m_threadList.AddTail(handle);
}

//下载文件并解析显示
unsigned __stdcall CMainFrame::_ShowDownInfo(LPVOID pParam)
{
	try{
		CHttpDownLoadDlg* dlg = (CHttpDownLoadDlg *)pParam;
		char* buffer;// = new char[1000000];
		HttpUtl utl;
		if(utl.request(dlg->m_URL.GetBuffer(0),buffer)==0)//,1000000);
		{
			Tips("获取共享信息失败，请检查当前网络链接！");
			return 0;
		}
		if (!WriteToFile(dlg->m_Path, buffer, strlen(buffer)))
		{
			Tips("获取共享信息失败，请检查当前网络链接！");
			return 0;
		}
		//AfxMessageBox("获取信息成功!");
		pMainWnd->m_pMainListView->ShowResourceShareFiletype(dlg->m_Path);
	}catch(...){}
	HANDLE handle = (HANDLE)::GetCurrentThread();
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	return 0;
}

//added by zuoss 090526 显示网络读物列表
void CMainFrame::ShowNetBookResource(TreeItemInfo* info)
{
	m_v1WndSplitter.ToUp();
	CHttpDownLoadDlg dlg;
	if(info->Type == Is_NetBook)
	{
		DWORD id = info->BookNumber;
		dlg.m_URL.Format("%s/share/getNetBookItem.asp?id=%d&page=%d",AspServer,id,m_pMainListView->page);
		dlg.m_Path.Format("%s\\temp\\netbookItem_%d_%d.xml", CurrentPath,id,m_pMainListView->page);
	
/*		//每天更新一次
		CTime ct = CTime::GetCurrentTime();
		CFileStatus rStatus;
		CFile cf;
		if (cf.Open(dlg.m_Path,CFile::modeRead))
		{
			cf.GetStatus(rStatus);
			CTime filetime = rStatus.m_ctime;
			if (filetime.GetYear() != ct.GetYear() || filetime.GetMonth() != ct.GetMonth() || filetime.GetDay() != ct.GetDay())
			{	
				if(dlg.DoModal()!=IDOK)
					return;
			}
			cf.Close();
		}
		else
		{*/
			if(dlg.DoModal()!=IDOK)
				return;
		//}
		m_pMainListView->ShowNetBookResource(dlg.m_Path);
	}
}
//end 

//begin istation点击类别节点显示所属书籍
void CMainFrame::ShowiBook(int typeID,int channelID)
{
	m_v1WndSplitter.ToUp();
	CHttpDownLoadDlg dlg;

	dlg.m_URL.Format("%s/bookstore/getBookinfoList.asp?typeid=%d&channelid=%d&page=%d",AspServer,typeID,channelID,m_pMainListView->page);
	dlg.m_Path.Format("%s\\temp\\iTypeBooks_%d_%d_%d.xml", CurrentPath,typeID,channelID,m_pMainListView->page);
	
	m_pMainListView->ichannelid = channelID;
	m_pMainListView->itypeid = typeID;
	
/*	//每天更新一次
	CTime ct = CTime::GetCurrentTime();
	CFileStatus rStatus;
	CFile cf;
	if (cf.Open(dlg.m_Path,CFile::modeRead))
	{
		cf.GetStatus(rStatus);
		CTime filetime = rStatus.m_ctime;
		if (filetime.GetYear() != ct.GetYear() || filetime.GetMonth() != ct.GetMonth() || filetime.GetDay() != ct.GetDay())
		{	
			if(dlg.DoModal()!=IDOK)
				return;
		}
		cf.Close();
	}
	else
	{*/
		if(dlg.DoModal()!=IDOK)
			return;
	//}
	m_pMainListView->ShowiBook(dlg.m_Path);
}
//end

void CMainFrame::ShowTip(UINT icon, CString tip, DWORD dwseconds)
{
	if(m_tipsDlg.IsWindowVisible()){
		if(m_tipsDlg.style==0){
			m_tipsDlg.KillTimer(1);
			m_tipsDlg.ShowWindow(SW_HIDE);	
		}
		else{
			return;
		}
	}
	m_tipsDlg.btn.ShowWindow(SW_HIDE);
	m_tipsDlg.chk.ShowWindow(SW_HIDE);
	m_tipsDlg.style = 0;
	UINT id = IDB_TIPDLG_MESSAGE;
	switch(icon){
	case td_msg:
		m_tipsDlg.bg = IDB_TIPDLG_MESSAGE;
		break;
	case td_alert:
		m_tipsDlg.bg = IDB_TIPDLG_ALERT;
		break;
	case td_tips:
		m_tipsDlg.bg = IDB_TIPDLG_TIP;
		break;
	case td_qst:
		m_tipsDlg.bg = IDB_TIPDLG_QUESTION;
		break;
	}
	m_tipsDlg.m_tip = tip;
	m_tipsDlg.icon = icon;
	m_tipsDlg.SetTimer(1, dwseconds, NULL);
	m_tipsDlg.ShowWindow(SW_SHOW);
}

void CMainFrame::OnHelp() 
{
	char WebPath[1024] = {0};
	sprintf_s(WebPath, 1024, "%s/readingshelp/1_1.html", AspServer);
	ShellExecute(NULL, "open", WebPath,	NULL, NULL, SW_SHOWMAXIMIZED);	
}

void CMainFrame::OnButtonUseradv() 
{
	m_v1WndSplitter.ToTop();
	//m_colWndSplitter.Right2Right();
	char WebPath[1024] = {0};
	sprintf_s(WebPath, 1024, "http://%s/RetailWeb/hanlinclient/useradvice.jsp?username=%s", JspServer, UserName);
	m_pShowView->OpenURL(WebPath);	
}

void CMainFrame::OnButtonHistory() 
{
	m_v1WndSplitter.ToTop();
	//m_colWndSplitter.Right2Right();
	char WebPath[1024] = {0};
	sprintf_s(WebPath, 1024, "%s/search/searchhistory.asp?username=%s", AspServer, UserName);
	m_pShowView->OpenURL(WebPath);	
}

void CMainFrame::ShowOrderList()
{
	char BinFileName[_MAX_PATH] = {0};
	sprintf_s(BinFileName, 1024, "%s\\temp\\order_list.bin", CurrentPath);
	char IbxFileName[_MAX_PATH] = {0};
	sprintf_s(IbxFileName, _MAX_PATH, "%s\\temp\\order_list.ibx", CurrentPath);
	char WebPath[_MAX_PATH] = {0};
	sprintf_s(WebPath, _MAX_PATH, "%s/order/newlistorderitem.asp?id=%s", AspServer, UserName);
	CHttpDownLoadDlg dlg;
	dlg.m_URL = WebPath;
	dlg.m_Path = IbxFileName;
	if(dlg.DoModal()!=IDOK)
		return;
	if(!SaveOrderType(IbxFileName, BinFileName))
		return;
	m_pMainListView->ShowOrderItem(BinFileName);
}

void CMainFrame::OnButtonOrderList()
{
	m_v1WndSplitter.ToUp();
	ShowOrderList();
}

void CMainFrame::OnButtonRefresh() 
{
	isfreashing = TRUE;
	if (m_OrderCenterView != NULL && m_OrderCenterView->IsWindowVisible())
	{
		m_OrderCenterView->ShowTree();
	}
	if (m_ManBooksView != NULL && m_ManBooksView->IsWindowVisible())
	{
		m_ManBooksView->ShowTree();
	}
	if (m_DownloadCenterView != NULL && m_DownloadCenterView->IsWindowVisible())
	{
		m_DownloadCenterView->ShowTree();
	}
	if (m_ReadOnLineView != NULL && m_ReadOnLineView->IsWindowVisible())
	{
		m_ReadOnLineView->ShowTree();
	}
	if (m_HotKeyView != NULL && m_HotKeyView->IsWindowVisible())
	{
		m_HotKeyView->ShowTree();
	}
	//if (m_PersonInfoView != NULL && m_PersonInfoView->IsWindowVisible())
	//{
	//	m_PersonInfoView->OnRefresh();
	//}
	isfreashing = FALSE;
}

void CMainFrame::ShowReadman(TreeItemInfo *info)
{
	CString curFolderPath;
	switch(info->Type)
	{
	case Is_Readman_News:
		curFolderPath.Format("%s\\Readman\\News", FavorPath);
		m_pMainListView->ShowFolder(curFolderPath, Is_FileList);
		break;
	case Is_Readman_Known:
		curFolderPath.Format("%s\\Readman\\Known", CurrentPath);
		m_pMainListView->ShowFolder(curFolderPath, Is_FileList);
		break;
	case Is_Readman_Method:
		curFolderPath.Format("%s\\Readman\\Method", CurrentPath);
		m_pMainListView->ShowFolder(curFolderPath, Is_FileList);
		break;
	case Is_Readman_Help:
		curFolderPath.Format("%s\\Readman\\Helper", CurrentPath);
		m_pMainListView->ShowFolder(curFolderPath, Is_FileList);
		break;
	case Is_Readman_Advertise:
		curFolderPath.Format("%s\\Readman\\Advertisement", CurrentPath);
		m_pMainListView->ShowFolder(curFolderPath, Is_FileList);
		break;
	case Is_Readman_Fonts:
		curFolderPath.Format("%s\\Readman\\Fonts", CurrentPath);
		m_pMainListView->ShowFolder(curFolderPath, Is_FileList);
		break;
	case Is_Readman_Tool:
		// 显示工具
		curFolderPath.Format("%s\\Readman\\Tools", CurrentPath);
		m_pMainListView->ShowFolder(curFolderPath,Is_FileList);
		break;
	}
	// 	if(info->Type == Is_Readman_News){
	// 		CString curFolderPath;
	// 		curFolderPath.Format("%s\\Readman\\News", FavorPath);
	// 		m_pMainListView->ShowFolder(curFolderPath, Is_FileList);
	//	}
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_DEVICECHANGE){
		if(DBT_DEVICEREMOVECOMPLETE == wParam)
			ReMoveSDDisk();
		else if(DBT_DEVICEARRIVAL == wParam)
			SetTimer(4, 2000, NULL);
	}
	if(message == WM_SYSCOMMAND){
		if(wParam==SC_MINIMIZE){ 
			ShowHide();
			return 0;
		}
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::InsertSDDisk()
{
	if(strlen(SDDisk) != 0)
		return;
	Sleep(500);
	
	char Disk[10] = {0};
	
	DWORD driversbit=GetLogicalDrives();
	driversbit = driversbit >> 3;
	for(int i = 0; i < 24; i++){
		if(driversbit & 1){
			char driver[10] = {"a:"};
			driver[0] = 'a' + i + 3;
			if(GetDriveType(driver)==DRIVE_REMOVABLE && GetDiskFree(driver) != 0){
				strcpy_s(Disk, 10, driver);
				break;//可能只能识别一个USB设备
			}
		}
		driversbit = driversbit >> 1;
	}
	
	if(strlen(Disk) == 0)
		return;
	//added by zhanganzhan on 20090920
	
	//插入项目
// 	if (!uDiskAuthInfo.flag)
// 	{
// 		CUDiskAuth *uDisk = new CUDiskAuth;
// 		uDisk->GetUSBDiskID(Disk[0]);
// 	}
// 
// 
// 	if (uDiskAuthInfo.flag && uDiskAuthInfo.diskFlag == Disk[0])
 	{
	// end added by zhanganzhan on 20090920
	
		char Msg[100] = {0};
		sprintf_s(Msg, 100, "插入盘符为“%s”的阅读器", Disk);
		strcpy_s(SDDisk, 20, Disk);	
		if(strlen(SDDisk)==0)
			return;	
		char Txt[100] = "我的阅读器";
		HTREEITEM hItem = m_pManBooksTree->InsertItem(Txt, 7, 7, NULL);
		m_ManBooksView->SetTreeItemData(hItem, Is_SDCard, NULL);
		m_ManBooksView->InsertSDFolderItem(hItem);
		Tips(Msg,td_tips);
		GetUserInfo();
	}
	
}

unsigned _int64 CMainFrame::GetDiskFree(char *disk)
{
	ULARGE_INTEGER lpfreetouse = {0},lpfreeclunum = {0},lpclunum = {0};
	
	GetDiskFreeSpaceEx(disk,&lpfreetouse,&lpclunum,&lpfreeclunum);
	
	return lpfreetouse.QuadPart;
}

void CMainFrame::ReMoveSDDisk()
{
	if(strlen(SDDisk) == 0)
		return;
	if(GetDiskFree(SDDisk) != 0)
		return;
	// if (uDiskAuthInfo.flag && uDiskAuthInfo.diskFlag == SDDisk[0])
	{
		char Msg[100] = {0};
		sprintf_s(Msg, 100, "系统已经删除了盘符为“%s”的阅读器", SDDisk);
		memset(SDDisk, 0, 20);
		uDiskAuthInfo.flag = FALSE;
		uDiskAuthInfo.DevName[0] = '\0';
		uDiskAuthInfo.diskFlag = '\0';
		//删除相关项
		m_ManBooksView->FreeTreeItem(m_ManBooksView->hSDDisk);
		m_pManBooksTree->DeleteItem(m_ManBooksView->hSDDisk);
		m_ManBooksView->hSDDisk = NULL;
		Tips(Msg, td_tips);
	}

}

void CMainFrame::OnSyn()
{
	try{
		CSYNDialog synDialog;
		if(m_pMainListView->showtype == Is_SDDiskList)
			synDialog.dire = SD2PC;
		else
			synDialog.dire = PC2SD;
		synDialog.DoModal();
		m_pMainListView->Invalidate(FALSE);
	}
	catch(...){
#ifdef _DEBUG
		Tips("LRESULT CMainFrame::OnSyn()", td_alert);
#endif
	}
	
}

LRESULT CMainFrame::OnSynToSD(WPARAM w,LPARAM l)
{
	try{
		if(strlen(SDDisk)==0)
			return TRUE;
		
		CSYNDialog synDialog;
		synDialog.dire = PC2SD;
		synDialog.DoModal();
		m_pMainListView->Invalidate(FALSE);
	}
	catch(...){
#ifdef _DEBUG
		Tips("LRESULT CMainFrame::OnSynToSD()", td_alert);
#endif
	}
	return TRUE;
}

unsigned __stdcall CMainFrame::_RecordUserBehavior(LPVOID behav)
{
    CHttpDownLoadDlg dlg;
		dlg.m_URL.Format("%s/search/adduserbehavior.asp?userid=%s&behavior=%s",AspServer,UserName,behav);
		dlg.m_Path.Format("%s\\temp\\adduserbehavior.txt", CurrentPath);
		if(dlg.DoModal()!=IDOK){
			Tips("搜索记录跟踪失败！",td_alert,5);
				return FALSE;
		}
		free(behav);
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}
unsigned __stdcall CMainFrame::_GetUserInfo(LPVOID pParam)
{
	if(strlen(SDDisk)==0)
		return 1;
	
	CPushAdv::ToSDCardAdv();
	
	CGetUserInfo gui;
	gui.GetWriting(pMainWnd->m_quit);
	gui.GetRestore(pMainWnd->m_quit);
	gui.GetComment(pMainWnd->m_quit,pMainWnd->m_socketList,pMainWnd->m_mutex);
	gui.UpBugLib(pMainWnd->m_quit,pMainWnd->m_socketList,pMainWnd->m_mutex);
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	_endthreadex(0);
	return 0;  //deleted by zhanganzhan
}

void CMainFrame::GetUserInfo()
{
	/*格式采集*/
	HANDLE handle = (HANDLE)::_beginthreadex(NULL,0,_GetUserInfo,this,CREATE_SUSPENDED,NULL);
	m_threadList.AddTail(handle);
	::ResumeThread(handle);
	
	if(strlen(SDDisk)>0)
		PostMessage(WM_SYN_TO_SD);
}

void CMainFrame::OnMenuitemList() 
{
	m_pMainListView->ChangeToList();
	m_pMainListView->style = style_list;
	char iniPath[_MAX_PATH] = {0};
	char val[10] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\option.ini", CurrentPath);
	sprintf_s(val, 10, "%d",style_list);
	WritePrivateProfileString(UserName,"list-style",val,iniPath);
	Operation(m_hItem, m_info);
}

void CMainFrame::OnMenuitemReport() 
{
	m_pMainListView->ChangeToReport();
	m_pMainListView->style = style_report;
	char iniPath[_MAX_PATH] = {0};
	char val[10] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\option.ini", CurrentPath);
	sprintf_s(val, 10, "%d",style_report);
	WritePrivateProfileString(UserName,"list-style",val,iniPath);
	Operation(m_hItem, m_info);
}

void CMainFrame::OnUpdateMenuitemList(CCmdUI* pCmdUI) 
{
	if(m_pMainListView->style == style_list)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateMenuitemReport(CCmdUI* pCmdUI) 
{
	if(m_pMainListView->style == style_report)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

LRESULT CMainFrame::OnShowGroupResource(WPARAM reserve, LONG gid)
{
	m_v1WndSplitter.ToUp();
	CHttpDownLoadDlg dlg;
	dlg.m_URL.Format("%s/share/groupxml.asp?gid=%d",AspServer,gid);
	dlg.m_Path.Format("%s\\temp\\share.lst", CurrentPath);
	DeleteFile(dlg.m_Path);
	
	LPSTR url = (LPSTR)calloc(1024,sizeof(char));
	sprintf_s(url, 1024, "%s/share/group.asp?gid=%d", AspServer, gid);
	OpenURL((WPARAM)url);
	
	if(dlg.DoModal()!=IDOK)
		return TRUE;
	m_pMainListView->ShowResourceShareFiletype(dlg.m_Path);
	
	return TRUE;
}

LRESULT CMainFrame::OnShowUserResource(WPARAM w,LPARAM l)
{
	LPSTR UN = (LPSTR)w;
	m_v1WndSplitter.ToUp();
	CHttpDownLoadDlg dlg;
	dlg.m_URL.Format("%s/share/sharexml_accord_uid.asp?username=%s",AspServer,UN);
	dlg.m_Path.Format("%s\\temp\\share.lst", CurrentPath);
	DeleteFile(dlg.m_Path);
	if(dlg.DoModal()!=IDOK)
		return TRUE;
	m_pMainListView->ShowResourceShareFiletype(dlg.m_Path);
	free(UN);
	return TRUE;
}

void CMainFrame::OnMenuitemConfig() 
{
	CConfigDlg dlg;
	dlg.DoModal();
}

void CMainFrame::RefreshBookList(HTREEITEM hItem)
{
	HTREEITEM hPItem = m_pSelectedTree->GetParentItem(hItem);
	TreeItemInfo *info = (TreeItemInfo *)m_pSelectedTree->GetItemData(hItem);
	TreeItemInfo *Pinfo = (TreeItemInfo *)m_pSelectedTree->GetItemData(hPItem);
	if(!info || !Pinfo)
		return;

	char FileName[_MAX_PATH] = {0};
	sprintf_s(FileName, _MAX_PATH, "%s\\temp\\%s_%s_%d.bin", CurrentPath, Pinfo->IndexID, info->IndexID, info->CurrentPage);
	char ID[20] = {0};
	sprintf_s(ID, 20, "%s_%s_%d", Pinfo->IndexID, info->IndexID, info->CurrentPage);

	if(!PathFileExists(FileName) || atoi(Pinfo->IndexID)==19)
	{
		//接收
		CHttpDownLoadDlg dlg;
		char TmpFileName[_MAX_PATH] = {0};

		sprintf_s(TmpFileName, _MAX_PATH, "%s\\temp\\%s_%s_%d.ibk", CurrentPath, Pinfo->IndexID, info->IndexID, info->CurrentPage);
		char WebPath[1024] = {0};
		sprintf_s(WebPath, 1024, "%s/RetailWeb/getbookinfo?channelid=%s&typeid=%s&pageno=%d",
			JspServer, Pinfo->IndexID, info->IndexID, info->CurrentPage);
		dlg.m_URL = WebPath;
		dlg.m_Path = TmpFileName;
		if(dlg.DoModal() == IDOK)
		{
			SaveBookTypeFile(TmpFileName, FileName);
		}
		m_pMainListView->SetFocus();
	}
	if(!PathFileExists(FileName))
	{
		return;
	}
	m_pMainListView->RefreshHanlinBook(info,Pinfo,FileName);

}

void CMainFrame::SaveBookTypeFile(LPCTSTR ResName, LPCTSTR DesName)
{
	FILE *rfp = fopen(ResName, "rb");
	if(!rfp)
		return;

	fseek(rfp, 0, 2);
	long flen = ftell(rfp);
	fseek(rfp, 0, 0);
	if(flen <= 10){
		fclose(rfp);
		return;
	}

	char len[20] = {0};
	fread(len, 1, 10, rfp);
/*	if(flen != atoi(len) + 30){
		fclose(rfp);
		DeleteFile(ResName);
		return;
	}*/

	char T[20] = {0};
	fread(T, 1, 10, rfp);
	DWORD Time = atoi(T);
	
	char *buf = (char *)malloc(flen);
	memset(buf, 0, flen);
	fseek(rfp, 30, 0);
	fread(buf, 1, flen - 30, rfp);
	fclose(rfp);
	
	FILE *wfp = fopen(DesName, "wb");
	if(!wfp){
		free(buf);
		return;
	}
	BookInfoFileHead head = {0};
	head.UpdateBookListTime = Time;
	CTime Now = CTime::GetCurrentTime();
	head.UpdateFileTime = Now.GetYear() * 1000000 + Now.GetMonth() * 10000 + Now.GetDay() * 100 + Now.GetHour();
	
	fwrite(&head, 1, sizeof(BookInfoFileHead), wfp);
	char *tmp = buf;
	while(tmp){
		char type[16][210] = {0};
		int i = 0;
		long num = 0;
		char *chr = tmp;
		
		while(i < 16){
			if(strlen(chr) == 0)
				break;
			if(chr[0] == 0x02){
				i++;
				num = 0;
				chr++;
				continue;
			}
			if(chr[0] == 0x03)
				break;
			type[i][num++] = chr[0];
			chr++;
		}

		BookInfo info = {0};
		strcpy_s(info.BookId, 10, type[0]);
		strcpy_s(info.BookName, 88, type[1]);
		strcpy_s(info.HanlinCode, 20, type[2]);
		strcpy_s(info.Author, 40, type[3]);
		strcpy_s(info.Publisher, 50, type[4]);
		strcpy_s(info.Size, 20, type[5]);
		strcpy_s(info.Price, 20, type[6]);
		strcpy_s(info.Quality, 16, type[7]);
		strcpy_s(info.Path, 10, type[8]);
		strcpy_s(info.RefreshData, 12, type[9]);
		strcpy_s(info.VisitNumber, 10, type[11]);
		strcpy_s(info.Abstract, 200, type[14]);

		if(strlen(type[15])>0)
			info.operation = type[15][0];

		if(strlen(info.BookName) != 0)
			fwrite(&info, 1, sizeof(BookInfo), wfp);
	
		tmp = strchr(tmp+1, 0x03);
		if(tmp)
			tmp++;
	}
	free(buf);
	fclose(wfp);
//	DeleteFile(ResName);
}

void CMainFrame::Operation(HTREEITEM hItem, TreeItemInfo* info)
{
	if(!info)
		return;
	
	if(!hItem)
		return;
	
	m_hItem = hItem; 
	m_info = info;
	
	char WebPath[1024] = {0};
	char* buf;
	char* key;
	TreeItemInfo * treeItem;
	int tid,cid;//istation中TypeID，ChannelID

	CString cashfilepath;//add by Lins 2008/08/22
	GROUPINFO* ginfo;
	this->curID = 0;
	this->curCondition = info->Type;
	
	m_pMainListView->curcondition = 0;
	
	switch(info->Type){
	case Is_NetFriendList://added by zhanganzhan on 20090326
		m_pCommDlg->PostMessage(WM_SEND_CHAT_INFO1,NULL);
	//	AfxMessageBox("ruike");
		break;
	case Is_Chatroom://added by zhanganzhan on 20090326
		m_pCommDlg->PostMessage(WM_SEND_CHAT_INFO2,NULL);
	//	AfxMessageBox("chatroom");
		break;
	case Is_MyFavourate_Folder:
		ShowFolder(hItem, FavorPath);
		break;
	case Is_SDCard_Folder:
		ShowFolder(hItem, SDDisk);
		break;
	case Is_SDCard:
		m_v1WndSplitter.ToUp();
		m_pMainListView->ShowFolder(SDDisk,Is_SDDiskList);
		break;
	case Is_Collect_FileType_Year:
		ShowCollectYearFile(info);
		break;
	case Is_NetFirend:
		//ShowResourceShareFiletype(info);
		break;
	case Is_OrderPaper_Node:
	case Is_OrderMagzine_Node:
		m_v1WndSplitter.ToUp();
		ShowOrderPaper(info, hItem);
		break;
	case Is_OrderList:
		m_v1WndSplitter.ToUp();
		ShowOrderList();
		break;
	case Is_PushFolder://推送箱
		m_v1WndSplitter.ToUp();
		m_pMainListView->ShowPushFolder();
		break;
	case Is_RecentlyRead://最近阅读
		m_v1WndSplitter.ToUp();
		m_pMainListView->ShowRecentlyRead();
		break;
	case Is_OrderPaper_Search:
		m_v1WndSplitter.ToUp();
		m_MainBar.Search("数字报纸");
		break;
	case Is_OrderMagzine_Search:
		m_v1WndSplitter.ToUp();
		m_MainBar.Search("电子杂志");
		break;
	case Is_ResourceShare_FileType:
	case Is_ResourceShare_Hot:
		ShowResourceShareFiletype(info);
		break;
	case Is_MyResource:
/*
//zuoss 090312
	case Is_PublishCom:
		treeItem = (TreeItemInfo *)malloc(1);
		treeItem = (TreeItemInfo *)m_pTree->GetItemData(hItem);
		
		buf = (LPSTR)calloc(1024, sizeof(char));
		strcpy(buf,treeItem->url);
		OpenURL(buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Hanlin:
		treeItem = (TreeItemInfo *)malloc(1);
		treeItem = (TreeItemInfo *)m_pTree->GetItemData(hItem);
		if(strcmp(treeItem->IndexID,"")==0)
		{
			return;
		}
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf(buf, "%s/RetailWeb/hanlinclient/ontype.jsp?typeid=%s",JspServer,info->IndexID);
		OpenURL(buf);

		RefreshBookList(hItem);
		return;
	case Is_HanlinChannel:
		treeItem = (TreeItemInfo *)malloc(1);
		treeItem = (TreeItemInfo *)m_pTree->GetItemData(hItem);
		if(strcmp(treeItem->IndexID,"")==0)
		{
			return;
		}
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf(buf, "%s/RetailWeb/hanlinclient/onchannel.jsp?channelid=%s",JspServer,info->IndexID);
		OpenURL(buf);
		m_v1WndSplitter.ToTop();
		return;
*/
	case Is_iType:
		treeItem = (TreeItemInfo *)malloc(1);
		treeItem = (TreeItemInfo *)m_pSelectedTree->GetItemData(hItem);
		tid = treeItem->BookNumber;
		treeItem = (TreeItemInfo *)m_pSelectedTree->GetItemData(m_pSelectedTree->GetParentItem(hItem));
		cid = treeItem->BookNumber;
		ShowiBook(tid,cid);
		return;
	case Is_NetBook:
		buf = (LPSTR)calloc(1024, sizeof(char));
		strcpy_s(buf, 1024, info->url);	
		OpenURL((WPARAM)buf);
		ShowNetBookResource(info);
		return;
	case Is_NetBookLeaf:
		buf = (LPSTR)calloc(1024, sizeof(char));
		strcpy_s(buf, 1024, info->url);
		OpenURL((WPARAM)buf);
		ShowNetBookResource(info);
		return;
	case Is_FanShu:
		buf = (LPSTR)calloc(1024, sizeof(char));
		strcpy_s(buf, 1024, "http://www.fanshu.com/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_FanShuLeaf:
		buf = (LPSTR)calloc(1024, sizeof(char));
		strcpy_s(buf, 1024, info->url);	
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
//end
	case Is_NetGroup:
		//ShowResourceShareFiletype(info);
		/*
		key =(LPSTR)calloc(1024,sizeof(char));
		ginfo = (GROUPINFO*)info->BookNumber; 
		sprintf(key,"$$%d$%d",Is_NetGroup,ginfo->gid);
		m_MainBar.Search(key);
		m_MainBar.m_keyword = "";
		UpdateData(TRUE);
		*/
		//add by dongzhenhua 20080602
		
 		if (gbOnSearching == TRUE)
 		{
 			MessageBox("正在搜索，请稍候...");
 			return ;
 		}//added by zhanganzhan on 20081023
// 		gbOnSearching = TRUE;
		key= (char*)malloc(m_pSelectedTree->GetItemText(hItem).GetLength()+1);
		ZeroMemory(key, m_pSelectedTree->GetItemText(hItem).GetLength()+1);
		strcpy_s(key, m_pSelectedTree->GetItemText(hItem).GetLength()+1, m_pSelectedTree->GetItemText(hItem).Left(m_pSelectedTree->GetItemText(hItem).Find('(')));
		currentkeyword = key;
		ginfo = (GROUPINFO*)info->BookNumber; 
		//	if (ExistFile(Is_NetGroup,ginfo->gid))//如果缓存中有该文件则直接显示
		//	{
		//	}
		//	else									//如果缓存中无该文件
		m_pMainListView->curcondition = 1;	//当前状态赋初值
		pMainWnd->PostMessage(WM_SEARCHCACHE, (WPARAM)info,(WPARAM)key);
		//	m_pMainListView->SearchCache(Is_NetGroup,ginfo->gid,m_pTree->GetItemText(hItem)); 
		//	ShowResourceShareFiletype(info);
		buf = (LPSTR)calloc(1024,sizeof(char));
		sprintf_s(buf, 1024, "%s/share/group.asp?gid=%d", AspServer, ginfo->gid);
		OpenURL((WPARAM)buf);
		break;
	case Is_Readman_News:
	case Is_Readman_Known:
	case Is_Readman_Method:
	case Is_Readman_Help:
	case Is_Readman_Fonts:
	case Is_Readman_Advertise:
	case Is_Readman_Tool:
		ShowReadman(info);
		break;
	case Is_Readman://Readman根节点
		buf = (LPSTR)calloc(1024, sizeof(char));
		if (strcmp(UserName,"Guest_Default") == 0)
		{
			sprintf_s(buf, 1024, "%s/index.asp", AspServer);
		}
		else
			sprintf_s(buf, 1024, "%s/index.asp?userid=%s&pwd=%s",AspServer,UserName,Password);
		OpenURL((WPARAM)buf);
		return;
	case Is_AllNetGroupList:
		buf = (LPSTR)calloc(1024, sizeof(char));
// 		sprintf(buf, "%s/share/discover.asp", AspServer);
// 		OpenURL(buf);
		if (strcmp(UserName,"Guest_Default") == 0)
		{
			sprintf_s(buf, 1024, "%s/index.asp", AspServer);
		}
		else
			sprintf_s(buf, 1024, "%s/index.asp?userid=%s&pwd=%s", AspServer, UserName, Password);
		OpenURL((WPARAM)buf);
		return;
	case Is_NetGroup_Level:
 		if (gbOnSearching == TRUE)
 		{
 			MessageBox("正在搜索，请稍候...");
 			return ;
 		}//added by zhanganzhan on 20081023
// 		gbOnSearching = TRUE;
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "%s/share/grouplevel.asp?grouplevel=%d", AspServer, info->BookNumber);	
		OpenURL((WPARAM)buf);
		
		
		//搜索类别名称
		/*
		if (info->KeyWords==NULL)
		m_MainBar.Search(m_pTree->GetItemText(hItem).Left(m_pTree->GetItemText(hItem).Find('(')));
		else{
		key =(LPSTR)calloc(1024,sizeof(char));
		sprintf(key,"$$%d$%d",Is_NetGroup_Level,info->BookNumber);
		m_MainBar.Search(key);
		}*/
		key= (char*)malloc(m_pSelectedTree->GetItemText(hItem).Left(m_pSelectedTree->GetItemText(hItem).Find('(')).GetLength()+1);
		ZeroMemory(key, m_pSelectedTree->GetItemText(hItem).Left(m_pSelectedTree->GetItemText(hItem).Find('(')).GetLength()+1);
		strcpy_s(key, m_pSelectedTree->GetItemText(hItem).Left(m_pSelectedTree->GetItemText(hItem).Find('(')).GetLength()+1, m_pSelectedTree->GetItemText(hItem).Left(m_pSelectedTree->GetItemText(hItem).Find('(')));
		currentkeyword = key;
		m_pMainListView->curcondition = 1;
		//add by Lins 2008/08/22
		cashfilepath = RecordCourse(1,info);
		if (cashfilepath == "")
		{
			pMainWnd->PostMessage(WM_SEARCHCACHE, (WPARAM)info,(WPARAM)key);
		}
		else{
			char* ccashfilepath;
			ccashfilepath = new char[cashfilepath.GetLength()+1];
			ZeroMemory(ccashfilepath,cashfilepath.GetLength()+1);
			strcpy_s(ccashfilepath, cashfilepath.GetLength()+1, cashfilepath);
			//	sprintf(ccashfilepath,"%d%d",CurrentPath,ccashfilepath);
			//	sprintf(ccashfilepath,"%s\\temp\\%s",CurrentPath,cashfilepath);
			
			//	AfxMessageBox(ccashfilepath);
			m_pMainListView->ShowOneClass(ccashfilepath);
			//AfxMessageBox("gbOnSearching = FALSE");
			//gbOnSearching = FALSE;//zhanganzhan
		}
		//pMainWnd->PostMessage(WM_SEARCHCACHE, (WPARAM)info,(WPARAM)key);
		return;
	case Is_ResourceShare:
		m_v1WndSplitter.ToTop();
		sprintf_s(WebPath, 1024, "%s/share/index.asp", AspServer);
		m_pShowView->OpenURL(WebPath);
		return;
	case Is_RickerGroup_Leaf:
	case Is_RickerGroup_Node:
		//AfxMessageBox(CString(info->IndexID));
		if (info->IndexID != NULL)
		{
			m_pCommDlg->PostMessage(WM_SEND_CHAT_INFO,(WPARAM)info->IndexID);
		}
		return;
	case Is_Hall:
		m_v1WndSplitter.ToTop();
		m_colWndSplitter.Left2Left();
		//sprintf(WebPath, "http://218.68.91.108:8088/room/room.jsp?roomid=%d&username=%s&roomname=%s",info->BookNumber,UserName,m_pTree->GetItemText(hItem));
		sprintf_s(WebPath, 1024, "%s/room/room.do?roomId=%d&username=%s&roomname=%s",JspServer,info->BookNumber,UserName,m_pSelectedTree->GetItemText(hItem));
		m_pShowView->OpenURL(WebPath);
		return;

	//add by Lins 20090103
	case Is_Zgsztug:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.d-library.com.cn/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		m_v1WndSplitter.ToTop();
		return;
	case Is_Tianya:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.tianyabook.com/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Rdcxzx:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.rol.cn.net/library");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Qjzw:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.cnovel.com/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Essk:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.eshunet.com/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Dtzw:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www8.silversand.net/com/dtbook/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Sxmd:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.bookhome.net/");
		OpenURL((WPARAM)buf);
		return;
	case Is_Rmsc:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.peopledaily.com.cn/bookshop");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Kyec:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://fastbook.9797.com/Learning/ZheXueZongJiao");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Sb:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.book8.com/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Fhsc:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://lwwww.sd.cninfo.net/library");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Sjsc:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.52read.com/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Zhsk:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://hff.21cn.com:83/book/bg/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Sh:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.booksea.com/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Dnsc:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://computerbooks.yeah.net/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_Dssk:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://book.szonline.net/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_HotKeyleaf:
// 		if(gbOnSearching==TRUE)
// 		{
// 			MessageBox("正在进行搜索，请稍候...");
// 			return;
// 		}
// 		gbOnSearching = TRUE;
		treeItem = (TreeItemInfo *)malloc(1);
		treeItem = (TreeItemInfo *)m_pSelectedTree->GetItemData(hItem);
		m_MainBar.Search(treeItem->KeyWords);
		return;
	case Is_FanShuHotBook:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://www.fanshu.com/");
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_FanShuHotBookleaf:
		buf = (LPSTR)calloc(1024, sizeof(char));
		strcpy_s(buf, 1024, info->url);	
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	case Is_RStore:
		buf = (LPSTR)calloc(1024, sizeof(char));
		sprintf_s(buf, 1024, "http://61.181.14.184:8084/ReadingStore/index.jsp?name=%s",UserName);
		OpenURL((WPARAM)buf);
		m_v1WndSplitter.ToTop();
		return;
	}
	
	m_hItem = hItem; 
	m_info = info;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
#ifdef SWITCH_WATERMARKER
	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_CONTROL && input_state==INPUT_STATE1){
			root=BuildBinaryTree();
			input_state=INPUT_STATE2;
			MessageBox("control");
		}
		else if(pMsg->wParam == VK_SHIFT&& input_state==INPUT_STATE2){
			DWORD num = 1;
			InitPreOrderWalk(root,num);
			input_state=INPUT_STATE3;
			MessageBox("shift");
		}
		else if(pMsg->wParam == VK_INSERT&& input_state==INPUT_STATE3){
			DWORD num = 1;
			CList<PBTNODE,PBTNODE&> nl;
			nl.AddTail(root);
			InitLevelOrderWalk(&nl,num);
			input_state=INPUT_STATE_END;
			MessageBox("VK_INSERT");
		}
		else if(pMsg->wParam == VK_F6 && input_state==INPUT_STATE_END){
			if(::GetKeyState(VK_CONTROL)<0){
				__int64 n = Extract(root);
				CString str;
				str.Format("动态图水印信息为:%I64d", n);
				MessageBox(str,SOFTWARE_NAME);
			}
		}
	}
#endif
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::MessageAlert()
{
	pMainWnd->ModifyIcon(IDI_ICON_MSG);
	SetTimer(5,400,NULL);
}

LRESULT CMainFrame::OnCreateNewFavorFolder(WPARAM w,LPARAM l)
{
	LPSTR name = (LPSTR)w;
	m_ManBooksView->PostMessage(WM_CREATE_FAVORFOLDER,(WPARAM)name);
	return TRUE;
}
LRESULT CMainFrame::RecordBehavior(WPARAM w,LPARAM l)
{
	LPSTR behav = (LPSTR)w;
	//记录用户行为
	HANDLE handle = (HANDLE)::_beginthreadex(NULL,0,_RecordUserBehavior,(LPVOID)behav,CREATE_SUSPENDED,NULL);
	m_threadList.AddTail(handle);
	::ResumeThread(handle);
// 	CHttpDownLoadDlg dlg;
// 		dlg.m_URL.Format("%s/search/adduserbehavior.asp?userid=%s&behavior=%s",AspServer,UserName,behav);
// 		dlg.m_Path.Format("%s\\temp\\adduserbehavio.asp", CurrentPath);
// 		if(dlg.DoModal()!=IDOK){
// 			Tips("搜索记录跟踪失败！",td_alert,5);
// 				return FALSE;
// 		}
	return TRUE;
}
LRESULT CMainFrame::OpenWindow(WPARAM w,LPARAM l)
{
	//	m_pGuidTreeView->PostMessage(WM_CREATE_FAVORFOLDER,(WPARAM)name);
	//	AfxMessageBox("Change");
	m_colWndSplitter.Right2Right();
	m_colWndSplitter.Left2Left();
	m_v1WndSplitter.ToBottom();
	m_v1WndSplitter.ToUp();
	
	return TRUE;
}

LRESULT CMainFrame::CloseWindow(WPARAM w,LPARAM l)
{
	//	AfxMessageBox("Change");
	m_colWndSplitter.Right2Left();
	m_colWndSplitter.Left2Right();
	m_v1WndSplitter.ToDown();
	m_v1WndSplitter.ToUp();
	return TRUE;
}

BOOL CMainFrame::ExistFile(int flag, int id)
{
	char Tadd[_MAX_PATH] = {0};
	sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\groupcache\\baidu\\findres%d_%d.xml",CurrentPath,flag,id);
	if((fopen(Tadd,"r"))==NULL)			/*要打开的文件名*/ 
		return FALSE;	//xml 下载失败
	return TRUE;
}

void CMainFrame::BackupSDCard()
{
	if(strlen(SDDisk)==0)
		return;
	
	CPathDialog dlg("备份到","文件夹选择", NULL, this);
	if(dlg.DoModal()!=IDOK)
		return;
	CString path = dlg.GetPathName();
	CString name;
	CTime tm = CTime::GetCurrentTime();
	name.Format("SD卡备份_%02d年%02d月%02d日", tm.GetYear(),tm.GetMonth(),tm.GetDay());
	path += "\\";
	path += name;
	_mkdir(path);
	
	CBackupDialog bdlg;
	bdlg.sourcePath = SDDisk;
	bdlg.destPath = path;
	if(bdlg.DoModal()!=IDOK)
		return;
}
// add by yangxiao 2008.8.12
CString CMainFrame::compareIp(CString ipHost)
{	
	CString tmpStr1;
	CString network;
	tmpStr1 = ipHost.Left(ipHost.ReverseFind('.'));
	//AfxMessageBox(tmpStr1);
	
	/* modified by yangxiao 20080818
	if (tmpStr1.Compare(tmpStr2)==0)
	{	
	return "本地";
	}
	else 
	{	
	if (tmpStr3.Compare("202.113.16")==0)
	{
	return "南开大学";
	}
	
	  return "外部网络";
	  
		}
	*/
	
	/*
	for (int i=0; i<NUM_IPMAP_ROW;i++)
	{
	if ((tmpStr1.Compare(IPMAP[i][0]) ==0)||(ipHost.Compare(IPMAP[i][0]) ==0))
	{
	return IPMAP[i][1];
	}
	}
	
	  return "未知网络";
	*/
	
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
		{
			throw 0;
		}
		char SQLSEL[1024] = {0};
		sprintf_s(SQLSEL, 1024, "SELECT [network] from ipmap where netNum = \"%s\"",tmpStr1);
		hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
			adOpenStatic, adLockOptimistic, adCmdText);
		if (!m_pRecordset->EndOfFile)
		{
			network = GetCollectString(m_pRecordset,"network");
			return network;
		}
		
		
	}
	catch(_com_error &e){
		if(SUCCEEDED(hr)){
			if(m_pRecordset->State)
				m_pRecordset->Close();
		//	m_pRecordset.Release();
		}
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();
	
	return "";
	
}

void CMainFrame::OnMenuitemCommlog() 
{
	// TODO: Add your command handler code here
	
	CString MsgRecord;
	MsgRecord.Format("%s\\system\\COMMLOG.txt",CurrentPath);
	CFile cf;
	if(!cf.Open(MsgRecord,CFile::modeCreate|CFile::modeWrite)){
		Tips("打开通讯日志文件失败");
		return;
	}
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			throw 0;
		char SQLSEL[1024] = {0};
		sprintf_s(SQLSEL, 1024, "SELECT [operation],[filesize],[upDownFlag],[year],[month],[day],[hour],[minute],[second],[weekDay] FROM currLog order by id DESC");
		hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
			adOpenStatic, adLockOptimistic, adCmdText);
		while(!m_pRecordset->EndOfFile){
			CString operation = GetCollectString(m_pRecordset, "operation");
			long filesize = GetCollectLong(m_pRecordset, "filesize");
			
			
			BYTE upDownFlag = 0;
			upDownFlag = GetCollectByte(m_pRecordset, "upDownFlag");
			int year = 0;
			year =GetCollectInt(m_pRecordset, "year");
			BYTE month = 0;
			month = GetCollectByte(m_pRecordset, "month");
			BYTE day = 0;
			day = GetCollectByte(m_pRecordset, "day");
			BYTE hour = 0;
			hour = GetCollectByte(m_pRecordset, "hour");
			BYTE minute = 0;
			minute = GetCollectByte(m_pRecordset, "minute");
			BYTE second = 0;
			second = GetCollectByte(m_pRecordset, "second");
			BYTE weekDay = 0;
			weekDay = GetCollectByte(m_pRecordset, "weekDay");
			
			
			CString date;
			date.Format("日期:%d-%d-%d",year,month,day);
			CString time;
			time.Format("时间:%d:%d:%d",hour,minute,second);
			CString logOper;
			logOper.Format("操作:  %s\t",operation);
			CString logFileSize;
			logFileSize.Format("传输字节数: %d",filesize);
			CString upDown ;
			if (upDownFlag == 0)
			{
				upDown = "上传";
			}
			if (upDownFlag == 1)
			{
				upDown = "下载";
			}
			
			CString logItem;
			logItem.Format("%s \t %s \t %s \t %s \t %s\r\n",date,time,logOper,logFileSize,upDown);
			
			cf.SeekToEnd();
			cf.Write((LPCSTR)logItem,logItem.GetLength());
			
			m_pRecordset->MoveNext();
		}
		
	}
	catch(_com_error &e){
		if(SUCCEEDED(hr)){
			if(m_pRecordset->State)
				m_pRecordset->Close();
		//	m_pRecordset.Release();
		}
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();
	cf.Close();
	::ShellExecute(NULL, "open", MsgRecord, NULL, NULL, SW_SHOW);
	
	/*********** for test
	CString COMMEXE;
	COMMEXE.Format("%s\\system\\CommAnalyzer.exe",CurrentPath);
	::ShellExecute(NULL, "open", COMMEXE, NULL, NULL, SW_SHOW);
	***********/
}

void CMainFrame::OnMenuitemCommanal() 
{
	// TODO: Add your command handler code here
	//修改实现方式
	
	//	CString COMMEXE;
	//	COMMEXE.Format("%s\\system\\CommAnalyzer.exe",CurrentPath);
	//	::ShellExecute(NULL, "open", COMMEXE, NULL, NULL, SW_SHOW);
	
	//CCommLogDlg dlg;
	m_commLogDlg.DoModal();
	
}
//===================add by Lins 2008/08/22======================//
//======保存课程查询记录，如果未查询，则插入记录后执行搜索=======//
//===============================================================//
CString CMainFrame::RecordCourse(int treetype, TreeItemInfo* info)
{
    int count;
	//查询
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			throw 0;
		char SQLSEL[1000] = {0};
		sprintf_s(SQLSEL, 1000, "SELECT count(0) AS cnt FROM courseRecord WHERE treetype=%d AND treeid=%d",treetype,(int)info->BookNumber);
		//	AfxMessageBox(SQLSEL);
		hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
			adOpenStatic, adLockOptimistic, adCmdText);
		count = GetCollectLong(m_pRecordset, "cnt");
	}
	catch(_com_error &e){
		if(SUCCEEDED(hr)){
			if(m_pRecordset->State)
				m_pRecordset->Close();
		//	m_pRecordset.Release();
		}
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();
	CString cashfilepath;
	cashfilepath.Format("course_%d_%d.xml",treetype,(int)info->BookNumber);
	//无重复信息则，插入数据库
	if(count<=0){	
		char SQL[1000]={0};
		CTime Now = CTime::GetCurrentTime();
		CString nowtime = Now.Format("%Y-%m-%d %H:%M:%S");
		sprintf_s(SQL, 1000, "INSERT INTO courseRecord ([treetype],[treeid],[cashfilepath],[time]) values(%d,%d,\"%s\",\"%s\")", treetype,(int)info->BookNumber,cashfilepath,nowtime);
		//		AfxMessageBox(SQL);
		m_pConnection->Execute(SQL, NULL, adCmdText);
		return "";
	}
	else{
        return cashfilepath;  
	}
}

void CMainFrame::OnToolsReadwolf() 
{
	// TODO: Add your command handler code here
		// TODO: Add your command handler code here
	HKEY hKEY;   //定义有关的   hKEY,   在查询结束时要关闭。     
	try{
		LPCTSTR data_Set = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\eBook.exe";
		//打开与路径 data_Set 相关的 hKEY，第一个参数为根键名称，第二个参数表。
		//表示要访问的键的位置，第三个参数必须为0，KEY_READ表示以查询的方式。     
		//访问注册表，hKEY则保存此函数所打开的键的句柄。    
		long ret= RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set, 0, KEY_READ, &hKEY);
		if(ret!=ERROR_SUCCESS){//如果无法打开hKEY，则终止程序的执行     
			throw 0;
		}
		LPBYTE setupPath = new BYTE [_MAX_PATH];
		DWORD type = REG_SZ;
		DWORD len = _MAX_PATH;
		//hKEY为刚才RegOpenKeyEx()函数所打开的键的句柄，"RegisteredOwner"
		//表示要查   询的键值名，type_1表示查询数据的类型，owner_Get保存所。     
		//查询的数据，cbData_1表示预设置的数据长度。     
		ret = ::RegQueryValueEx(hKEY, "Path", NULL, &type, setupPath, &len);
		if(ret!=ERROR_SUCCESS)	throw 0;
		CString path(setupPath); 
		delete [] setupPath;
		path += "SystemFunction\\Reader.exe";
		if(PathFileExists(path))
			ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOW);
		else
			throw 0;
	}
	catch(int err){
		if(err==0)
			DownLoadTools();
	}
	//程序结束前要关闭已经打开的hKEY。     
	RegCloseKey(hKEY);    
}

void CMainFrame::OnToolsMakewolf() 
{
	// TODO: Add your command handler code here
		HKEY hKEY;
	try{
		LPCTSTR data_Set = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\eBook.exe";
		long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKEY);
		if(ret!=ERROR_SUCCESS)	throw 0;
		LPBYTE setupPath = new BYTE [_MAX_PATH];
		DWORD type = REG_SZ;
		DWORD len = _MAX_PATH;
		ret = ::RegQueryValueEx(hKEY, "Path", NULL, &type, setupPath, &len);
		if(ret!=ERROR_SUCCESS)	throw 0;
		CString path(setupPath); 
		delete [] setupPath;
		CString exePath;
		exePath.Format("%seBook.exe", path);
		if(PathFileExists(path))
			ShellExecute(NULL, "open", exePath, NULL, path, SW_SHOW);
		else
			throw 0;
	}
	catch(int err){
		if(err==0)
			DownLoadTools();
	}
	RegCloseKey(hKEY);   
}

void CMainFrame::DownLoadTools()
{
	if(MessageBox("您所要使用的工具并没有安装,是否需要下载?", "提示", MB_YESNO)!=IDYES)
		return;
	FILE* fp;
	char* buf;
	try{
		char WebPath[1024] = {0};
		char Tadd[_MAX_PATH] = {0};
		BOOL flag = FALSE;
		sprintf_s(WebPath, 1024, "http://www.jinke.com.cn/gettools.asp");
		sprintf_s(Tadd, _MAX_PATH, "%s/temp/toolsurl.tmp", CurrentPath);
		DeleteFile(Tadd);
		if(!DownLoad(WebPath, Tadd, m_quit, m_socketList))
			throw 0;
	//	if(!DownloadEx(WebPath, Tadd, &flag))
		if (pMainWnd->m_quit)
		{
			return;
		}
		fp = fopen(Tadd, "rb");
		if(!fp)
			throw 0;
		fseek(fp, 0, 2);
		long len = ftell(fp);
		fseek(fp, 0, 0);
		if(len==0)
			throw 1;
		buf = (char*)malloc(len+1);
		ZeroMemory(buf, len+1);
		fread(buf, 1, len, fp);
		fclose(fp);
		char* tmp = buf+strlen(buf)-4;
		if(_strnicmp(buf+strlen(buf)-3, "rar", 3) != 0)
			throw 2;
		ShellExecute(NULL, "open", buf, NULL, NULL, SW_SHOW);
		free(buf);
	}
	catch(int err){
		switch(err){
		case 2:
			free(buf);
		case 1:
			fclose(fp);
		case 0:
			MessageBox("下载失败，请检查您的网络连接!", "错误", MB_OK);
		}
	}
}

void CMainFrame::OnMenuitemModify() 
{
	// TODO: Add your command handler code here
	CString userinfopath = "";
	userinfopath.Format("%s/server/changeUserInfo.asp?username=%s",AspServer,UserName);
	ShellExecute(NULL, "open", userinfopath,	NULL, NULL, SW_SHOW);
}

void CMainFrame::OnMenuitemScore() 
{
	// TODO: Add your command handler code here
	char* userinfopath;
	userinfopath = (LPSTR)calloc(1024, sizeof(char));
	sprintf_s(userinfopath, 1024, "%s/server/getuserscore.asp?username=%s",AspServer,UserName);
	OpenURL((WPARAM)userinfopath);
}
/*
bool CMainFrame::GetAllInfor()
{
	string WebPath = string(AspServer) + "/share/getemail.asp?username=" + string(UserName);
	string TmpPath = string(CurrentPath) + "\\myemailinfo.txt";
	if(!DownLoad(WebPath.c_str(), TmpPath.c_str(), pMainWnd->m_quit, pMainWnd->m_socketList)) 
	{
		//AfxMessageBox("获取用户邮箱信息失败！");
		return false;
	} 
	else
	{
		if (pMainWnd->m_quit)
		{
			return false;
		}
		if (pMainWnd->m_quit)
		{
			return false;
		}
		ifstream myemailinfo;
		myemailinfo.open(TmpPath.c_str(),ios::in);	
		char * Mailtemp = new char [1024];
		while(myemailinfo.getline(Mailtemp,1024))
		{
			string Maildata(Mailtemp);
			if(Maildata.compare(0,6,"email:") == 0)
			{
				MyEMailUserName = Maildata.substr(7,Maildata.find_first_of('@') - 7);
				MyEMailServer = Maildata.substr(Maildata.find_first_of('@') + 1);
			}
			if(Maildata.compare(0,9,"password:") == 0)
			{
				MyEMailPassWord = Maildata.substr(10);
			}
		}
		delete[] Mailtemp;
		myemailinfo.close();
		CFile temfile;
		temfile.Remove(TmpPath.c_str());
		if (pMainWnd->m_quit)
		{
			return false;
		}
		if (MyEMailServer.compare("sina.com")==0)
		{
			SmtpServer = "smtp.sina.com.cn";
			PopServer = "pop.sina.com.cn";
		}else if (MyEMailServer.compare("sohu.com")==0)
		{
			SmtpServer = "smtp.sohu.com";
			PopServer = "pop3.sohu.com";
		}else if (MyEMailServer.compare("qq.com")==0)
		{
			SmtpServer = "smtp.qq.com";
			PopServer = "pop.qq.com";
		}else{
			//Tips("您的SMTP,Pop3服务暂不可用");
			return false;
		}
		//只有当用户邮箱信息不为空时才能轮询
		if (MyEMailServer.length() == 0)
		{
			//Tips("邮箱服务器为空!");
			return false;
		}
		if (MyEMailUserName.length() == 0)
		{
			//Tips("邮箱用户名为空!");
			return false;
		}
		if (MyEMailPassWord.length() == 0)
		{
			//Tips("邮箱密码为空!");
			return false;
		}
	}
	return true;
}
*/
void CMainFrame::ReflushMail()
{
	Heart->ReflushMail();
}

BOOL CMainFrame::AddToReadings(CString filepath)
{
	CString fname;
	CString temp;
	int pos;
	temp.Format("%s",filepath);
	pos = temp.GetLength() - temp.ReverseFind('\\') - 1;
	fname = temp.Right(pos);

	CString sharepath;
	sharepath.Format("%s\\share\\%s",CurrentPath,fname);
	//存储到桌面读物本地数据库
	CString ext;
	ext = fname.Right(fname.GetLength() - fname.ReverseFind('.') - 1);

	char * extension;
	extension = ext.GetBuffer(ext.GetLength());
	ext.ReleaseBuffer();
	int filetype = GetFileTypeFromExt(extension);
	
	CString name;
	name = fname.Left(fname.ReverseFind('.'));
	
	CTime tm = CTime::GetCurrentTime();
	CString strTime;//修改时间	
	strTime.Format("%d-%d-%d %d:%d", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute());

	long filesize = 0;
	CFile cf;
	CFileStatus fstatus;
	if (cf.GetStatus(filepath,fstatus))
	{
		filesize = (long)fstatus.m_size;
	}
	long year = tm.GetYear();

//  char SQL[2048] = {0};
//	sprintf(SQL, "INSERT INTO loc ([addr],[type],[cdate],[title],[size],[year]) values(\"%s\",%d,\"%s\",\"%s\",%d,%d)",info->DstPath,filetype,strTime,name,filesize,year);
//	m_pConnection->Execute(SQL, NULL, adCmdText);

//  先判断记录不存在再插入!
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try
	{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))	//数据集打开不成功
			return TRUE;
		char SQL[1000]={0};
		sprintf_s(SQL, 1000, "SELECT [addr],[cdate],[type],[title],[size],[year],[nlist] FROM loc WHERE type=%d and addr=\"%s\"", filetype, filepath);
			m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
				adOpenKeyset, adLockOptimistic, adCmdText);
			if(m_pRecordset->EndOfFile)
			{
				m_pRecordset->AddNew();
				m_pRecordset->PutCollect("addr", _variant_t(filepath));
				m_pRecordset->PutCollect("cdate", _variant_t(strTime));
				m_pRecordset->PutCollect("type", _variant_t((long)filetype));
				m_pRecordset->PutCollect("title", _variant_t(name));
				m_pRecordset->PutCollect("size", _variant_t((long)filesize));
				m_pRecordset->PutCollect("year", _variant_t((long)year));
				long nlist=255;
				switch(filetype)
				{
				case wolFileType:
					nlist = 1;
					break;
				case pdfFileType:
					nlist = 2;
					break;
				case docFileType:
					nlist = 3;
					break;
				case chmFileType:
					nlist = 4;
					break;
				case htmFileType:
					nlist = 5;
					break;
				case txtFileType:
					nlist = 6;
					break;
				default:
					nlist = 7;
				}
				m_pRecordset->PutCollect("nlist", _variant_t((long)nlist));
				m_pRecordset->Update();
			}
			m_pRecordset->Close();
		//	m_pRecordset.Release();
		}
		catch(_com_error &e){
			if(SUCCEEDED(hr)){
				if(m_pRecordset->State)
					m_pRecordset->Close();
		//		m_pRecordset.Release();
			}
		}


	//显示在“最近阅读”中
	_RecordsetPtr m_pRecordset1;
	HRESULT hr1;
	try
	{
		hr1 = m_pRecordset1.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr1))	//数据集打开不成功
			return TRUE;
		char reSQL[1000]={0};
		sprintf_s(reSQL, 1000, "SELECT [addr],[cdate],[type],[title] FROM recently WHERE type=%d and addr=\"%s\"", filetype, filepath);
			m_pRecordset1->Open(reSQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
				adOpenKeyset, adLockOptimistic, adCmdText);
			if(m_pRecordset1->EndOfFile)
			{
				m_pRecordset1->AddNew();
				m_pRecordset1->PutCollect("addr", _variant_t(filepath));
				m_pRecordset1->PutCollect("cdate", _variant_t(strTime));
				m_pRecordset1->PutCollect("type", _variant_t((long)filetype));
				m_pRecordset1->PutCollect("title", _variant_t(name));
				m_pRecordset1->Update();
			}
			m_pRecordset1->Close();
		//	m_pRecordset1.Release();
		}
		catch(_com_error &e){
			if(SUCCEEDED(hr1)){
				if(m_pRecordset1->State)
					m_pRecordset1->Close();
		//		m_pRecordset1.Release();
			}
		}

	//上传到用户个人共享。未完成
// 	CString updatePath;
// 	updatePath.Format("%s/share/updateShareAfterDownload.asp?username=%s&lastwritetime=%s&nfilesize=%d&filetype=%d&filepath=s%",AspServer,UserName,strTime,filesize,filetype,filepath);
//	UpLoad(updatePath);


	//询问是否同步到阅读器	
//	if (AfxMessageBox("收到邮件文件\"" + filepath +"\",是否将文件同步到阅读器？",MB_YESNO) == IDYES)
//	{
		if (strcmp(SDDisk,"") == 0)
		{
			Tips("您的文件已存储。请您确定阅读器已连接后，点击菜单“同步按钮”即可同步到阅读器。");
			//存储到本地数据库
			char updateSQL[2048] = {0};
			sprintf_s(updateSQL, 2048, "update loc set SynFlag = 1 where addr = \"%s\"",filepath);
			m_pConnection->Execute(updateSQL, NULL, adCmdText);
		}
		else
		{
			//BIG BUG!!!!!!!!!!!!!!!!!
			//ReadingsSync 邮件
			char path[_MAX_PATH];
			sprintf_s(path, _MAX_PATH, "%s\\Readings同步中心", SDDisk);
			if(!PathFileExists(path))
				_mkdir(path);
			CString readerPath;
			readerPath.Format("%s\\Readings同步中心\\%s",SDDisk,fname);
			CopyFile(filepath,readerPath,TRUE);
		}
//	}
//end
	return TRUE;
}

void CMainFrame::DeleteFromDownLoadListView(DownLoadMissionInfo* info)
{
	CListCtrl* m_pList = m_pDownLoadListView->m_pList;
	LVFINDINFO itemInfo = {0};
	itemInfo.flags = LVFI_PARAM;
	itemInfo.lParam = (LPARAM)info;
	int n = m_pList->FindItem(&itemInfo);
	if(n>=0)
		m_pList->DeleteItem(n);
	//m_pDownLoadListView->PostMessage(WM_P2P_FILEDOWNLOAD_FINISHED,(WPARAM)info);
}

void CMainFrame::ResumeDownload()
{
	CList<DownLoadMissionInfo*,DownLoadMissionInfo*&> infolist;
	CString missionsavepath;
	missionsavepath = CurrentPath + CString("\\system\\") + UserName;
	_mkdir(missionsavepath);
	missionsavepath += "\\missionsave.txt";
	CFile missionsavefile;
	if(!PathFileExists(missionsavepath))
	{
		missionsavefile.Open(missionsavepath,CFile::modeCreate);
		missionsavefile.Close();
		return;
	}
	ifstream info;
	info.open(missionsavepath);
	int i = 0;
	char lineinfo[MAX_PATH];
	
	while (info.getline(lineinfo,MAX_PATH))
	{
		DownLoadMissionInfo* miinfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
		ZeroMemory(miinfo, sizeof(DownLoadMissionInfo));
		strcpy_s(miinfo->title, _MAX_PATH, lineinfo);
		if(info.getline(lineinfo,1024))
			strcpy_s(miinfo->URL, 1024, lineinfo);
		else
			break;
		if(info.getline(lineinfo,_MAX_PATH))
			strcpy_s(miinfo->DstPath, _MAX_PATH, lineinfo);
		else
			break;
		if(info.getline(lineinfo,_MAX_PATH))
			strcpy_s(miinfo->IP, 20, lineinfo);
		else
			break;
		if(info.getline(lineinfo,_MAX_PATH))
			strcpy_s(miinfo->MD5, MD5LEN, lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			miinfo->fileType = atoi(lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			miinfo->sourceType = atoi(lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			miinfo->port = atoi(lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			miinfo->fileLen = atol(lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			miinfo->finishedSize = atol(lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			miinfo->speed = atol(lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			miinfo->passTime = atol(lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			miinfo->restTime = atol(lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			strcpy_s(miinfo->UserName, 30, lineinfo);
		else
			break;
		infolist.AddTail(miinfo);
	}
	info.close();
	//CFile missionsavefile;
	missionsavefile.Open(missionsavepath,CFile::modeCreate);
	missionsavefile.Close();
	POSITION pos = infolist.GetHeadPosition();
	while (pos)
	{
		DownLoadMissionInfo* mis = infolist.GetNext(pos);
		pMainWnd->PostMessage(WM_FILEDOWNLOAD, (WPARAM)mis);
	}
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	//AfxMessageBox("Receive Msg");
	try{
	DownLoadMissionInfo *info=new DownLoadMissionInfo;
	char *msg=(char *)pCopyDataStruct->lpData;
	char *p;   
	if(msg && !pMainWnd->m_quit){
		//AfxMessageBox(msg);
		//格式：process:filename;torrent名字;总大小;已下载大小;进度(0.0-1);目标路径
		if(strstr(msg, "process:")){//进度
			char *submsg=msg+8;
			//filename
			p = strtok(submsg,";"); 
			if(p){
				strcpy_s(info->title, _MAX_PATH, p);
				//AfxMessageBox(p);
			}
			//torrent名
			p = strtok(NULL, ";"); 
			if(p){
				strcpy_s(info->URL, 1024, p);
				//AfxMessageBox(p);
			}
			//总大小
			p=strtok(NULL,";");
			if(p){
				info->fileLen=atol(p);
				//AfxMessageBox(p);
			}
			//已下载大小
			p=strtok(NULL,";");
			if(p){
				info->finishedSize=atol(p);
				//AfxMessageBox(p);
			}
			//进度百分比
			p=strtok(NULL,";");
			if(p){
				info->process=atof(p);
				//AfxMessageBox(p);
			}
			//目标路径
			p=strtok(NULL,";");
			if(p){
				strcpy_s(info->DstPath, _MAX_PATH, p);
				//AfxMessageBox(p);
			}else
			{
				return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
			}
			info->sourceType=dl_bt;
			//if(info->process < 1.0)
			//{
				pMainWnd->PostMessage(WM_UPDATE_DOWNLOAD, (WPARAM)info);
			//}
		}/*else if(strstr(msg,"begindownload:")){
			char *submsg=msg+14;
			p = strtok(submsg,";"); 
			if(p){
				strcpy(info->bookID,p);
			}
			
			p = strtok(NULL, ";"); 
			if(p){
				strcpy(info->MD5,p);
			}
			info->finishedSize=0;
			info->fileLen=0;
			info->speed=0;
			pMainWnd->PostMessage(WM_NEW_DOWNLOAD, (WPARAM)info);
		}*/
	}
	}catch(...){
		AfxMessageBox("throw error!");
	}
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}
