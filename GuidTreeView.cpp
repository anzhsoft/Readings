// GuidTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "GuidTreeView.h"
#include "HttpDownLoadDlg.h"
#include "FileFindEx.h"
#include "MainFrm.h"
#include "InputDlg.h"
#include "PushAdv.h"
#include "SetRGroupKeywords.h"
#include "base64.h"
#include "Transcoding.h"
#include <iostream>
#include "UDiskAuth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char SDDisk[20];
extern char UserName[50];
extern char CurrentPath[_MAX_PATH];
extern char AspServer[1024];
extern char JspServer[1024];
extern BOOL offline;
extern char FavorPath[_MAX_PATH];
extern CMainFrame* pMainWnd;
extern _ConnectionPtr m_pConnection;
extern USB_DETAIL_INFO uDiskAuthInfo;

/////////////////////////////////////////////////////////////////////////////
// CGuidTreeView
CGuidTreeView* pTreeView = NULL;
CTreeCtrl* pTree = NULL;
COrderCenterView* m_OCView = NULL;
CReadOnLine* m_RLView = NULL;
CManBooks* m_MBView = NULL;
CDownloadCenterTreeView* m_DCView = NULL;
CHotKeyTree* m_HKView = NULL;
CTreeCtrl* m_pOCTree = NULL;
CTreeCtrl* m_pRLTree = NULL;
CTreeCtrl* m_pMBTree = NULL;
CTreeCtrl* m_pDCTree = NULL;
CTreeCtrl* m_pHKTree = NULL;
char Tip[30][30] = {0};
int treeicowidth=16;
int treeicoheight=16;


IMPLEMENT_DYNCREATE(CGuidTreeView, CTreeView)

CGuidTreeView::CGuidTreeView()
{
	quit = FALSE;
	hit_Item = NULL;

	m_MyBookcaseItem = NULL;
	hNetLibrary = NULL;
	hpaperItem = NULL;
	hmagItem = NULL;
	hNetResource = NULL;
	hOrderItem = NULL;
	collectItemRead = NULL;
	collectItemPic = NULL;
	collectItemMeida = NULL;
	hFriend = NULL;
	hSDDisk = NULL;
}

CGuidTreeView::~CGuidTreeView()
{
}


BEGIN_MESSAGE_MAP(CGuidTreeView, CTreeView)
	//{{AFX_MSG_MAP(CGuidTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_MESSAGE(WM_THREAD_FINISHED, OnThreadFinished)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteitem)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENUITEM_OPENFOLDER, OnMenuitemOpenfolder)
	ON_COMMAND(ID_MENUITEM_ADDFOLDER, OnMenuitemAddfolder)
	ON_COMMAND(ID_MENUITEM_DELFOLDER, OnMenuitemDelfolder)
	ON_MESSAGE(WM_CREATE_FAVORFOLDER, OnCreateNewFavorFolder)
	ON_COMMAND(ID_MENU_CREATE_REGROUP, OnMenuCreateRegroup)
	ON_COMMAND(ID_MENU_CREATE_LEVELGROUP, OnMenuCreateLevelgroup)
	ON_COMMAND(ID_MENU_DELETE, OnMenuDelete)
	ON_COMMAND(ID_MENU_KEYWORDS, OnMenuKeywords)
	ON_COMMAND(ID_MENU_TOMYFAVOR, OnMenuTomyfavor)
	ON_COMMAND(ID_MENUITEM_BACKUP, OnMenuitemBackup)
	ON_COMMAND(ID_MENU_SHOWTAG, OnMenuShowtag)
	ON_COMMAND(ID_MENU_ADDNETBOOK, OnMenuAddnetbook)
	ON_COMMAND(ID_MENU_SETNETBOOK, OnMenuSetnetbook)
	ON_COMMAND(ID_MENU_DELNETBOOK, OnMenuDelnetbook)
	ON_COMMAND(ID_MENU_MOVEUP, OnMenuMoveup)
	ON_COMMAND(ID_MENUITEM_REMOVE, OnMenuitemRemove)
	ON_COMMAND(ID_MENU_MAKEBOOK, OnMenuMakebook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuidTreeView drawing

void CGuidTreeView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CGuidTreeView diagnostics

#ifdef _DEBUG
void CGuidTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CGuidTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGuidTreeView message handlers

int CGuidTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;


	if(!InitialImageList())
		return FALSE;

	ShowTree();

	return 0;
}

BOOL CGuidTreeView::InitialImageList()
{
	try{
		m_pTree->ModifyStyle(0,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS);
		CBitmap bitmap;
		if(!bitmap.LoadBitmap(IDB_IMGLIST_TREE))
			return FALSE;
		CImageList imgList;
		if(!imgList.Create(treeicowidth, treeicoheight, ILC_COLORDDB|ILC_MASK, 11, 1))
			return FALSE;
		if(imgList.Add(&bitmap, RGB(236,233,214))==-1)
			return FALSE;
		m_pTree->SetImageList(&imgList, TVSIL_NORMAL);
		bitmap.DeleteObject();
		imgList.Detach();
	}
	catch(...){
#ifdef _DEBUG
		Tips("BOOL CGuidTreeView::InitialImageList()");
#endif
	}
	return TRUE;
}

BOOL CGuidTreeView::RefreshTree()
{	
	InsertLocalFolder(NULL);
	if(!offline)
	{
		InsertResourceShare(NULL);//共享资源
#ifdef SWITCH_ORDER
		InsertOrderCenter(NULL);//订阅中心
		GetOrderInfo();
		//GetNetStatics();
#endif
#ifdef SWITCH_NETRESOURCE
		InsertDownloadCenter(NULL);//网络读物,公告栏,免费数图
		GetNetLibraryInfo();
#endif
		//		InsertSearchHistory(NULL);
		//		InsertDownload(NULL);
		//		InsertFriendList(NULL);


		HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetNetInfo,(LPVOID)this,CREATE_SUSPENDED,NULL);
		if(handle)
		{
			threadList.AddTail(handle);
			ResumeThread(handle);
		}
		//add by dongzhenhua 20080604
		InsertMyResourceGroup();//我的读物群落
		InsertResourceGroup();//高教课程读物部落
		InsertNetBook();//网络原创读物
		InsertPublishCom();//读书联盟

		//InsertBookstore(NULL);
	}

	InsertRemoveableDisk();
	Invalidate(FALSE);

	return TRUE;
}

BOOL CGuidTreeView::InsertLocalFolder(HTREEITEM hItem)
{
	try{
		HTREEITEM hRecentlyRead = m_pTree->InsertItem("最近阅读", 0, 0, hItem);
		SetTreeItemData(hRecentlyRead, Is_RecentlyRead, NULL, 2, 1);

		InsertReadman(hItem);

		HTREEITEM PushFolder = m_pTree->InsertItem("推送箱", 15, 15, hItem);
		SetTreeItemData(PushFolder, Is_PushFolder, NULL, 2, 0);

		InsertMyBookCase(hItem);


		try{
			_RecordsetPtr m_pRecordset;
			HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
			if(!SUCCEEDED(hr))
				return FALSE;
			{
				CString SQL;
				SQL.Format("SELECT count(title) AS cnt FROM recently");
				hr = m_pRecordset->Open((LPCSTR)SQL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
				if(SUCCEEDED(hr)){
					long total = GetCollectLong(m_pRecordset, "cnt");
					m_pRecordset->Close();
					SQL.Format("SELECT count(title) AS cnt FROM recently WHERE DATEDIFF('d', record, '%s')<=1", GetDate(0,FALSE));
					hr = m_pRecordset->Open((LPCSTR)SQL, m_pConnection.GetInterfacePtr(),
						adOpenStatic, adLockOptimistic, adCmdText);
					long cur = 0;
					if(SUCCEEDED(hr)){
						cur = GetCollectLong(m_pRecordset, "cnt");
					}
					m_pRecordset->Close();
					CString str;
					str.Format("最近阅读[%d/%d]", cur, total);
					m_pTree->SetItemText(hRecentlyRead, str);
				}
			}
			{
				CString SQL;
				SQL.Format("SELECT count(title) AS cnt FROM push");
				hr = m_pRecordset->Open((LPCSTR)SQL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
				if(SUCCEEDED(hr)){
					long total = GetCollectLong(m_pRecordset, "cnt");
					m_pRecordset->Close();
					SQL.Format("SELECT count(title) AS cnt FROM push WHERE DATEDIFF('d', cdate, '%s')<=1", GetDate(0,FALSE));
					hr = m_pRecordset->Open((LPCSTR)SQL, m_pConnection.GetInterfacePtr(),
						adOpenStatic, adLockOptimistic, adCmdText);
					long cur = 0;
					if(SUCCEEDED(hr)){
						cur = GetCollectLong(m_pRecordset, "cnt");
					}
					m_pRecordset->Close();
					CString str;
					str.Format("推送箱[%d/%d]", cur, total);
					m_pTree->SetItemText(PushFolder, str);
				}
			}
			//	m_pRecordset.Release();
		}
		catch(_com_error& e){

		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("BOOL CGuidTreeView::InsertLocalFolder(HTREEITEM hItem)");
#endif
	}

	return TRUE;
}

void CGuidTreeView::SetTreeItemData(HTREEITEM hItem, BYTE type, char *ID, WORD Pages, DWORD Numbers,CString KeyWords,CString url)
{
	try{
		TreeItemInfo* info = (TreeItemInfo*)malloc(sizeof(TreeItemInfo));
		memset(info, 0, sizeof(TreeItemInfo));
		info->Type = type;
		if(ID)
			strcpy_s(info->IndexID, 10, ID);
		if(Pages != 0){
			info->TotlePage = Pages;
			info->CurrentPage = 1;
		}
		info->BookNumber = Numbers;
		//add by dongzhenhua 0530
		if(KeyWords.Compare("")!=0){
			info->KeyWords = (char*)malloc(KeyWords.GetLength()+1);
			memset(info->KeyWords,0,KeyWords.GetLength()+1);
			memcpy(info->KeyWords,KeyWords,KeyWords.GetLength());
			//			info->KeyWords =(LPCSTR)KeyWords;
		}
		//zuoss 090318
		if(url.Compare("")!=0){
			info->url = (char*)malloc(url.GetLength()+1);
			memset(info->url,0,url.GetLength()+1);
			memcpy(info->url,url,url.GetLength());
		}
		//end		
		m_pTree->SetItemData(hItem, (DWORD)info);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::SetTreeItemData(HTREEITEM hItem, BYTE type, char *ID, WORD Pages, DWORD Numbers)");
#endif
	}
}

void CGuidTreeView::InsertReadman(HTREEITEM hItem)
{
	try{
		int filecount = 0;
		CString curFolderPath;
		CString ftitle;

		HTREEITEM readmanNews = m_pTree->InsertItem("Readman      ", 18, 18, hItem);
		SetTreeItemData(readmanNews, Is_Readman, NULL);


		curFolderPath.Format("%s\\Readman\\News", FavorPath);
		filecount = GetFileNum(curFolderPath);
		ftitle.Format("新闻(%d)         ", filecount);
		HTREEITEM readmanXW = m_pTree->InsertItem(ftitle, 18, 18, readmanNews);
		SetTreeItemData(readmanXW, Is_Readman_News, NULL);


		HTREEITEM readmanTG = m_pTree->InsertItem("通告(0)         ", 18, 18, readmanNews);
		SetTreeItemData(readmanTG, Is_Readman_Notice, NULL);

		curFolderPath.Format("%s\\Readman\\Helper", CurrentPath);
		filecount = GetFileNum(curFolderPath);
		ftitle.Format("帮助(%d)         ", filecount);	
		HTREEITEM readmanBZ = m_pTree->InsertItem(ftitle, 18, 18, readmanNews);
		SetTreeItemData(readmanBZ, Is_Readman_Help, NULL);

		curFolderPath.Format("%s\\Readman\\Method", CurrentPath);
		filecount = GetFileNum(curFolderPath);
		ftitle.Format("读法(%d)         ", filecount);
		HTREEITEM readmanDF = m_pTree->InsertItem(ftitle, 18, 18, readmanNews);
		SetTreeItemData(readmanDF, Is_Readman_Method, NULL);

		curFolderPath.Format("%s\\Readman\\Known", CurrentPath);
		filecount = GetFileNum(curFolderPath);
		ftitle.Format("知道(%d)         ", filecount);
		HTREEITEM readmanZD = m_pTree->InsertItem(ftitle, 18, 18, readmanNews);
		SetTreeItemData(readmanZD, Is_Readman_Known, NULL);

		HTREEITEM readmanDD = m_pTree->InsertItem("导读(0)         ", 18, 18, readmanNews);
		SetTreeItemData(readmanDD, Is_Readman_Guide, NULL);

		HTREEITEM readmanYD = m_pTree->InsertItem("友读(0)         ", 18, 18, readmanNews);
		SetTreeItemData(readmanYD, Is_Readman_Friend, NULL);

		HTREEITEM readmanYY = m_pTree->InsertItem("音乐(0)         ", 18, 18, readmanNews);
		SetTreeItemData(readmanYY, Is_Readman_Music, NULL);

		curFolderPath.Format("%s\\Readman\\Advertisement", CurrentPath);
		filecount = GetFileNum(curFolderPath);
		ftitle.Format("广告(%d)         ", filecount);
		HTREEITEM readmanGG = m_pTree->InsertItem(ftitle, 18, 18, readmanNews);
		SetTreeItemData(readmanGG, Is_Readman_Advertise, NULL);

		curFolderPath.Format("%s\\Readman\\Fonts", CurrentPath);
		filecount = GetFileNum(curFolderPath);
		ftitle.Format("字库(%d)         ", filecount);
		HTREEITEM readmanZK = m_pTree->InsertItem(ftitle, 18, 18, readmanNews);
		SetTreeItemData(readmanZK,Is_Readman_Fonts,NULL);

		curFolderPath.Format("%s\\Readman\\Tools", CurrentPath);
		filecount = GetFileNum(curFolderPath);
		ftitle.Format("工具(%d)         ", filecount);
		HTREEITEM readmanGJ = m_pTree->InsertItem(ftitle, 18, 18, readmanNews);
		SetTreeItemData(readmanGJ,Is_Readman_Tool,NULL);

		HTREEITEM readmanAZ = m_pTree->InsertItem("安装(0)         ", 18, 18, readmanNews);
		SetTreeItemData(readmanAZ,Is_Readman_Install,NULL);

	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::InsertReadman(HTREEITEM hItem)");
#endif
	}
}

void CGuidTreeView::InsertMyBookCase(HTREEITEM hItem)
{
	try{
		char Path[_MAX_PATH] = {0};
		sprintf_s(Path, _MAX_PATH, "%s/system/config.ini", CurrentPath);
		char collectName[100] = {0};
		char collect_style[100] = {0};
		GetPrivateProfileString("favor","collect-style", "", collect_style, 100, Path);
		if (strlen(collect_style)>0 && atoi(collect_style) == 1) {
			char precent[100] = {0};
			GetPrivateProfileString("favor","spec-precent", "", precent, 100, Path);
			int iprecent = 0;
			if (strlen(precent) > 0) {
				iprecent = atoi(precent);
				sprintf_s(collectName, 100, "桌面读物(已完成%d.0%c)", ((iprecent>99)?99:iprecent), '%');
			}
			else
				strcpy_s(collectName,  100, "桌面读物");

			memset(precent, 0, 100);
			sprintf_s(precent, 100, "%d", iprecent);
			WritePrivateProfileString("favor", "spec-precent", precent, Path);
		}
		else {
			memset(Path, 0, _MAX_PATH);
			sprintf_s(Path, _MAX_PATH, "%s/system/sysinfo.ini", CurrentPath);
			char tFSize[100] = {0};
			GetPrivateProfileString("UpdateDB","FinishSize", "", tFSize, 100, Path);
			char tTSize[100] = {0};
			GetPrivateProfileString("UpdateDB","TotalSize", "", tTSize, 100, Path);

			if(strlen(tTSize)>0){
				__int64 tsize = _atoi64(tTSize);
				__int64 fsize = 0;
				if(strlen(tFSize)>0)
					fsize = _atoi64(tFSize);
				if((float)fsize/(float)tsize*100.0>99)
					sprintf_s(collectName, 100, "桌面读物(已完成99.9%c)", '%');
				else {
					sprintf_s(collectName, 100, "桌面读物(已完成%0.1f%c)", (float)fsize/(float)tsize*100.0, '%');
				}
			}
			else
				strcpy_s(collectName, 100, "桌面读物");
		}

		m_MyBookcaseItem = m_pTree->InsertItem(collectName, 17, 17, hItem);
		SetTreeItemData(m_MyBookcaseItem, Is_MyBookCase, NULL);
		InsertMyFavor(m_MyBookcaseItem);
		InsertCollect(m_MyBookcaseItem);
		SetTimer(1, 60*1000, NULL);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::InsertMyBookCase(HTREEITEM hItem)");
#endif
	}
}

void CGuidTreeView::InsertMyFavor(HTREEITEM hItem)
{
	try{
		hMyFavor = m_pTree->InsertItem("收藏夹",2,2,hItem);
		SetTreeItemData(hMyFavor, Is_MyFavourate, NULL);

		DWORD data = 0;
		CFileFindEx find(InsertFolder, SetFolderStatics, CountFile);
		find.Find(FavorPath, &hMyFavor, data);
		CString title;
		title.Format("%s(%d本)", m_pTree->GetItemText(hMyFavor), data);
		m_pTree->SetItemText(hMyFavor, title);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::InsertMyFavor(HTREEITEM hItem)");
#endif
	}
}

LPVOID CGuidTreeView::InsertFolder(LPCSTR Path, WIN32_FIND_DATA* pData,
								   LPVOID pfParam, DWORD& subdata)
{
	try{
		if(_stricmp(pData->cFileName,"Readman") == 0)
			return NULL;
		else if(_stricmp(pData->cFileName,"_Adv") == 0)
			return NULL;

		HTREEITEM* pfItem = (HTREEITEM*)pfParam;
		HTREEITEM* pcurItem = new HTREEITEM;
		HTREEITEM curItem = pTree->InsertItem(pData->cFileName, 4, 4, *pfItem);
		memcpy(pcurItem, &curItem, sizeof(HTREEITEM));
		pTreeView->SetTreeItemData(*pcurItem, Is_MyFavourate_Folder, NULL, 2, 1);
		subdata = 0;
		return (LPVOID)pcurItem;
	}
	catch(...){
#ifdef _DEBUG
		Tips("LPVOID CGuidTreeView::InsertFolder(LPCSTR Path, WIN32_FIND_DATA* pData, LPVOID pfParam, DWORD& subdata)");
#endif
	}
	return NULL;
}

LPVOID CGuidTreeView::InsertSDFolder(LPCSTR Path, WIN32_FIND_DATA* pData,
									 LPVOID pfParam, DWORD& subdata)
{
	try{
		HTREEITEM* pfItem = (HTREEITEM*)pfParam;
		HTREEITEM* pcurItem = new HTREEITEM;
		HTREEITEM curItem = m_pMBTree->InsertItem(pData->cFileName, 4, 4, *pfItem);
		memcpy(pcurItem, &curItem, sizeof(HTREEITEM));
		m_MBView->SetTreeItemData(*pcurItem, Is_SDCard_Folder, NULL, 2, 1);
		subdata = 0;
		return (LPVOID)pcurItem;
	}
	catch(...){
#ifdef _DEBUG
		Tips("LPVOID CGuidTreeView::InsertSDFolder(LPCSTR Path, WIN32_FIND_DATA* pData, LPVOID pfParam, DWORD& subdata)");
#endif
	}
	return NULL;
}

void CGuidTreeView::SetFolderStatics(LPCSTR Path, WIN32_FIND_DATA* pData,
									 LPVOID pParam, DWORD subdata, DWORD& data)
{
	try{
		HTREEITEM* pcurItem = (HTREEITEM*)pParam;
		CString title;
		title.Format("%s(%d本)", m_pMBTree->GetItemText(*pcurItem), subdata);
		m_pMBTree->SetItemText(*pcurItem, title);
		delete pcurItem;
		data += subdata;
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::SetFolderStatics(LPCSTR Path, WIN32_FIND_DATA* pData, LPVOID pParam, DWORD subdata, DWORD& data)");
#endif
	}
}

void CGuidTreeView::CountFile(LPCSTR Path, WIN32_FIND_DATA* pData, DWORD& data)
{
	data++;
}

unsigned __stdcall CGuidTreeView::_ShowTree(LPVOID pParam)
{

	try{
		CGuidTreeView* p = (CGuidTreeView*)pParam;
		if(!p)
			return 1;

		pTreeView->RefreshTree();
		HANDLE handle = ::GetCurrentThread();
		p->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
		_endthreadex(0);
	}
	catch(...)
	{
#ifdef _DEBUG
		AfxMessageBox("ShowTree");
#endif
	}

	return 0;
}

void CGuidTreeView::InsertCollect(HTREEITEM hItem)
{
	try{
		collectItemRead = m_pTree->InsertItem("我的读物", 2, 2, hItem);
		SetTreeItemData(collectItemRead, Is_Collect_Read, NULL);

		CTreeItemList* treeList_r = new CTreeItemList;
		HTREEITEM hWOL = m_pTree->InsertItem("WOL", 4, 4, collectItemRead);
		SetTreeItemData(hWOL, Is_Collect_FileType, NULL, 2, wolFileType);
		treeList_r->AddTail(hWOL);

		HTREEITEM hTXT = m_pTree->InsertItem("TXT", 4, 4, collectItemRead);
		SetTreeItemData(hTXT, Is_Collect_FileType, NULL, 2, txtFileType);
		treeList_r->AddTail(hTXT);

		HTREEITEM hHTM = m_pTree->InsertItem("HTML", 4, 4, collectItemRead);
		SetTreeItemData(hHTM, Is_Collect_FileType, NULL, 2, htmFileType);
		treeList_r->AddTail(hHTM);

		HTREEITEM hDOC = m_pTree->InsertItem("DOC", 4, 4, collectItemRead);
		SetTreeItemData(hDOC, Is_Collect_FileType, NULL, 2, docFileType);
		treeList_r->AddTail(hDOC);

		HTREEITEM hPPT = m_pTree->InsertItem("PPT", 4, 4, collectItemRead);
		SetTreeItemData(hPPT, Is_Collect_FileType, NULL, 2, pptFileType);
		treeList_r->AddTail(hPPT);

		HTREEITEM hPDF = m_pTree->InsertItem("PDF", 4, 4, collectItemRead);
		SetTreeItemData(hPDF, Is_Collect_FileType, NULL, 2, pdfFileType);
		treeList_r->AddTail(hPDF);

		HTREEITEM hCHM = m_pTree->InsertItem("CHM", 4, 4, collectItemRead);
		SetTreeItemData(hCHM, Is_Collect_FileType, NULL, 2, chmFileType);
		treeList_r->AddTail(hCHM);

		HTREEITEM hRAR = m_pTree->InsertItem("RAR\\ZIP", 4, 4, collectItemRead);
		SetTreeItemData(hRAR, Is_Collect_FileType, NULL, 2, rarFileType);
		treeList_r->AddTail(hRAR);

		HANDLE handle_r = (HANDLE)_beginthreadex(NULL,0,_GetLocalInfo,(LPVOID)treeList_r,CREATE_SUSPENDED,NULL);
		if(handle_r){
			threadList.AddTail(handle_r);
			ResumeThread(handle_r);
		}

		collectItemPic = m_pTree->InsertItem("我的图片", 2, 2, hItem);
		SetTreeItemData(collectItemPic, Is_Collect_Pic, NULL);

		CTreeItemList* treeList_p = new CTreeItemList;
		HTREEITEM hJPG = m_pTree->InsertItem("JPG", 4, 4, collectItemPic);
		SetTreeItemData(hJPG, Is_Collect_FileType, NULL, 2, jpgFileType);
		treeList_p->AddTail(hJPG);

		HTREEITEM hBMP = m_pTree->InsertItem("BMP", 4, 4, collectItemPic);
		SetTreeItemData(hBMP, Is_Collect_FileType, NULL, 2, bmpFileType);
		treeList_p->AddTail(hBMP);

		HTREEITEM hGIF = m_pTree->InsertItem("GIF", 4, 4, collectItemPic);
		SetTreeItemData(hGIF, Is_Collect_FileType, NULL, 2, gifFileType);
		treeList_p->AddTail(hGIF);

		HANDLE handle_p = (HANDLE)_beginthreadex(NULL,0,_GetLocalInfo,(LPVOID)treeList_p,CREATE_SUSPENDED,NULL);
		if(handle_p){
			threadList.AddTail(handle_p);
			ResumeThread(handle_p);
		}

		collectItemMeida = m_pTree->InsertItem("我的媒体", 2, 2, hItem);
		SetTreeItemData(collectItemMeida, Is_Collect_Media, NULL);

		CTreeItemList* treeList_m = new CTreeItemList;
		HTREEITEM hWAV = m_pTree->InsertItem("WAV", 4, 4, collectItemMeida);
		SetTreeItemData(hWAV, Is_Collect_FileType, NULL, 2, wavFileType);
		treeList_m->AddTail(hWAV);

		HTREEITEM hMP3 = m_pTree->InsertItem("MP3", 4, 4, collectItemMeida);
		SetTreeItemData(hMP3, Is_Collect_FileType, NULL, 2, mp3FileType);
		treeList_m->AddTail(hMP3);

		HTREEITEM hWMV = m_pTree->InsertItem("WMV\\WMA", 4, 4, collectItemMeida);
		SetTreeItemData(hWMV, Is_Collect_FileType, NULL, 2, wmvFileType);
		treeList_m->AddTail(hWMV);

		HTREEITEM hRM = m_pTree->InsertItem("RM\\RMVB", 4, 4, collectItemMeida);
		SetTreeItemData(hRM, Is_Collect_FileType, NULL, 2, rmFileType);
		treeList_m->AddTail(hRM);

		HTREEITEM hAVI = m_pTree->InsertItem("AVI", 4, 4, collectItemMeida);
		SetTreeItemData(hAVI, Is_Collect_FileType, NULL, 2, aviFileType);
		treeList_m->AddTail(hAVI);

		HTREEITEM hASF = m_pTree->InsertItem("ASF", 4, 4, collectItemMeida);
		SetTreeItemData(hASF, Is_Collect_FileType, NULL, 2, asfFileType);
		treeList_m->AddTail(hASF);

		HTREEITEM h3GP = m_pTree->InsertItem("3GP", 4, 4, collectItemMeida);
		SetTreeItemData(h3GP, Is_Collect_FileType, NULL, 2, _3gpFileType);
		treeList_m->AddTail(h3GP);

		HANDLE handle_m = (HANDLE)_beginthreadex(NULL,0,_GetLocalInfo,(LPVOID)treeList_m,CREATE_SUSPENDED,NULL);
		if(handle_m){
			threadList.AddTail(handle_m);
			ResumeThread(handle_m);
		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::InsertCollect(HTREEITEM hItem)");
#endif
	}

}

unsigned __stdcall CGuidTreeView::_GetLocalInfo(LPVOID pParam)//获取本地信息，我的读物，我的图片，我的媒体
{
	try{
		CTreeItemList* treeList = (CTreeItemList*)pParam;
		if(!treeList)
			return 1;

		long rootTotal = 0;

		_RecordsetPtr m_pRecordset;
		try{
			HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
			if(!SUCCEEDED(hr))
				throw 0;

			POSITION pos = treeList->GetHeadPosition();
			while(pos){
				HTREEITEM hItem = treeList->GetNext(pos);
				long total = 0;
				TreeItemInfo* info = (TreeItemInfo*)m_pMBTree->GetItemData(hItem);
				char SQL[1024] = {0};
				sprintf_s(SQL, 1024, "SELECT count(0) AS cnt,year FROM loc WHERE type=%d GROUP BY year ORDER BY year DESC",
					info->BookNumber);
				hr = m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
				while(!m_pRecordset->EndOfFile){
					try{
						total += GetCollectLong(m_pRecordset, "cnt");
						CString title;
						title.Format("%d年(%d)", GetCollectLong(m_pRecordset, "year"),
							GetCollectLong(m_pRecordset, "cnt"));
						HTREEITEM hSubItem = m_pMBTree->InsertItem(title,2,2,hItem);
						char Year[20] = {0};
						sprintf_s(Year, 20, "%d", GetCollectLong(m_pRecordset, "year"));
						m_MBView->SetTreeItemData(hSubItem, Is_Collect_FileType_Year, Year,
							(WORD)GetCollectLong(m_pRecordset, "cnt")/LOCALSEARCHMAXIUM+1,
							info->BookNumber);
					}
					catch(_com_error& e){
#ifdef _DEBUG
						Tips("unsigned __stdcall CGuidTreeView::_GetLocalInfo(LPVOID pParam):: while()");
						Tips(e.ErrorMessage());
#endif
					}
					m_pRecordset->MoveNext();
				}
				m_pRecordset->Close();

				CString ftitle;
				ftitle.Format("%s(%d)",m_pMBTree->GetItemText(hItem),total);
				m_pMBTree->SetItemText(hItem, ftitle);

				rootTotal += total;
				HTREEITEM hParentItem = m_pMBTree->GetParentItem(hItem);
				CString pTitle;
				TreeItemInfo* pinfo = (TreeItemInfo*)m_pMBTree->GetItemData(hParentItem);
				if(pinfo->Type == Is_Collect_Read)
					pTitle.Format("我的读物(%d)", rootTotal);
				else if(pinfo->Type == Is_Collect_Pic)
					pTitle.Format("我的图片(%d)", rootTotal);
				else
					pTitle.Format("我的媒体(%d)", rootTotal);
				m_pMBTree->SetItemText(hParentItem, pTitle);
			}
			//	m_pRecordset.Release();
		}
		catch(_com_error& e){
#ifdef _DEBUG
			Tips("unsigned __stdcall CGuidTreeView::_GetLocalInfo(LPVOID pParam)");
			Tips(e.ErrorMessage());
#endif
		}
		catch(int err){
#ifdef _DEBUG
			Tips("unsigned __stdcall CGuidTreeView::_GetLocalInfo(LPVOID pParam)");
#endif
		}
		treeList->RemoveAll();
		delete treeList;
	}
	catch(...){
#ifdef _DEBUG
		Tips("unsigned __stdcall CGuidTreeView::_GetLocalInfo(LPVOID pParam)");
#endif
	}

	HANDLE handle = ::GetCurrentThread();
	m_MBView->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	_endthreadex(0);

	return 0;  //deleted by zhanganzhan
}

void CGuidTreeView::InsertOrderCenter(HTREEITEM hItem)
{
	try{
		hOrderItem = m_pTree->InsertItem("订阅中心", 21, 21, hItem);
		SetTreeItemData(hOrderItem, Is_OrderCenter,NULL);
		HTREEITEM hlistItem = m_pTree->InsertItem("订阅清单", 11, 11, hOrderItem);
		SetTreeItemData(hlistItem, Is_OrderList,NULL);

		hpaperItem = m_pTree->InsertItem("数字报纸", 22, 22, hOrderItem);
		SetTreeItemData(hpaperItem,Is_OrderPaper,NULL);
		HTREEITEM hsearchPaperItem = m_pTree->InsertItem("搜索", 14, 14, hpaperItem);
		SetTreeItemData(hsearchPaperItem,Is_OrderPaper_Search,NULL);


		hmagItem = m_pTree->InsertItem("电子杂志", 2, 2, hOrderItem);
		SetTreeItemData(hmagItem,Is_OrderMagzine,NULL);
		HTREEITEM hsearchMagItem = m_pTree->InsertItem("搜索", 14, 14, hmagItem);
		SetTreeItemData(hsearchMagItem,Is_OrderMagzine_Search,NULL);

	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::InsertOrderCenter(HTREEITEM hItem)");
#endif
	}
}

void CGuidTreeView::InsertDownloadCenter(HTREEITEM hItem)
{
	try{
		hNetResource = m_pTree->InsertItem("网络读物", 3, 3, hItem);
		SetTreeItemData(hNetResource, Is_NetResource, NULL);

		HTREEITEM hPubInfo = m_pTree->InsertItem("公告栏", 0,0, hNetResource);
		SetTreeItemData(hPubInfo, Is_PubInfo, NULL);

		hNetLibrary = m_pTree->InsertItem("免费数图", 3, 3, hNetResource);
		SetTreeItemData(hNetLibrary, Is_NetLibrary, NULL);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::InsertDownloadCenter(HTREEITEM hItem)");
#endif
	}
}

void CGuidTreeView::InsertSearchHistory(HTREEITEM hItem)
{
	try{
		HTREEITEM hSearchHistory = m_pTree->InsertItem("寻找读物", 14, 14, hItem);
		SetTreeItemData(hSearchHistory, Is_SearchHistory, NULL);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::InsertSearchHistory(HTREEITEM hItem)");
#endif
	}
}

void CGuidTreeView::InsertResourceShare(HTREEITEM hItem)
{
	CTreeItemList* treeList = new CTreeItemList;
	CTreeItemList* treeTotalList = new CTreeItemList;

	HTREEITEM hShareResource = m_pTree->InsertItem("共享资源", 0, 0, hItem);
	SetTreeItemData(hShareResource, Is_ResourceShare, NULL);

	HTREEITEM hHot = m_pTree->InsertItem("热门共享", 23, 23, hShareResource);
	SetTreeItemData(hHot, Is_ResourceShare_Hot, NULL);

	HTREEITEM hRead = m_pTree->InsertItem("我读", 20, 20, hShareResource);
	SetTreeItemData(hRead, Is_ResourceShare_Read, NULL, 2, Is_MyRead);
	treeTotalList->AddTail(hRead);

	HTREEITEM hWOL = m_pTree->InsertItem("WOL", 20, 20, hRead);
	SetTreeItemData(hWOL, Is_ResourceShare_FileType, NULL, 2, wolFileType);
	treeList->AddTail(hWOL);

	HTREEITEM hTXT = m_pTree->InsertItem("TXT", 20, 20, hRead);
	SetTreeItemData(hTXT, Is_ResourceShare_FileType, NULL, 2, txtFileType);
	treeList->AddTail(hTXT);

	HTREEITEM hHTM = m_pTree->InsertItem("HTML", 20, 20, hRead);
	SetTreeItemData(hHTM, Is_ResourceShare_FileType, NULL, 2, htmFileType);
	treeList->AddTail(hHTM);

	HTREEITEM hDOC = m_pTree->InsertItem("DOC", 20, 20, hRead);
	SetTreeItemData(hDOC, Is_ResourceShare_FileType, NULL, 2, docFileType);
	treeList->AddTail(hDOC);

	HTREEITEM hPPT = m_pTree->InsertItem("PPT", 20, 20, hRead);
	SetTreeItemData(hPPT, Is_ResourceShare_FileType, NULL, 2, pptFileType);
	treeList->AddTail(hPPT);

	HTREEITEM hPDF = m_pTree->InsertItem("PDF", 20, 20, hRead);
	SetTreeItemData(hPDF, Is_ResourceShare_FileType, NULL, 2, pdfFileType);
	treeList->AddTail(hPDF);

	HTREEITEM hCHM = m_pTree->InsertItem("CHM", 20, 20, hRead);
	SetTreeItemData(hCHM, Is_ResourceShare_FileType, NULL, 2, chmFileType);
	treeList->AddTail(hCHM);

	HTREEITEM hRAR = m_pTree->InsertItem("RAR\\ZIP", 20, 20, hRead);
	SetTreeItemData(hRAR, Is_ResourceShare_FileType, NULL, 2, rarFileType);
	treeList->AddTail(hRAR);

	HTREEITEM hJPG = m_pTree->InsertItem("JPG", 20, 20, hRead);
	SetTreeItemData(hJPG, Is_ResourceShare_FileType, NULL, 2, jpgFileType);
	treeList->AddTail(hJPG);

	HTREEITEM hBMP = m_pTree->InsertItem("BMP", 20, 20, hRead);
	SetTreeItemData(hBMP, Is_ResourceShare_FileType, NULL, 2, bmpFileType);
	treeList->AddTail(hBMP);

	HTREEITEM hGIF = m_pTree->InsertItem("GIF", 20, 20, hRead);
	SetTreeItemData(hGIF, Is_ResourceShare_FileType, NULL, 2, gifFileType);
	treeList->AddTail(hGIF);

	HTREEITEM hPic = m_pTree->InsertItem("我看", 24, 24, hShareResource);
	SetTreeItemData(hPic, Is_ResourceShare_Pic, NULL, 2, Is_MyLook);
	treeTotalList->AddTail(hPic);

	HTREEITEM hWMV = m_pTree->InsertItem("WMV\\WMA", 24, 24, hPic);
	SetTreeItemData(hWMV, Is_ResourceShare_FileType, NULL, 2, wmvFileType);
	treeList->AddTail(hWMV);

	HTREEITEM hRM = m_pTree->InsertItem("RM\\RMVB", 24, 24, hPic);
	SetTreeItemData(hRM, Is_ResourceShare_FileType, NULL, 2, rmFileType);
	treeList->AddTail(hRM);

	HTREEITEM hAVI = m_pTree->InsertItem("AVI", 24, 24, hPic);
	SetTreeItemData(hAVI, Is_ResourceShare_FileType, NULL, 2, aviFileType);
	treeList->AddTail(hAVI);

	HTREEITEM hASF = m_pTree->InsertItem("ASF", 24, 24, hPic);
	SetTreeItemData(hASF, Is_ResourceShare_FileType, NULL, 2, asfFileType);
	treeList->AddTail(hASF);

	HTREEITEM h3GP = m_pTree->InsertItem("3GP", 24, 24, hPic);
	SetTreeItemData(h3GP, Is_ResourceShare_FileType, NULL, 2, _3gpFileType);
	treeList->AddTail(h3GP);

	HTREEITEM hMedia = m_pTree->InsertItem("我听", 25, 25, hShareResource);
	SetTreeItemData(hMedia, Is_ResourceShare_Media, NULL, 2, Is_MyListen);
	treeTotalList->AddTail(hMedia);

	HTREEITEM hWAV = m_pTree->InsertItem("WAV", 25, 25, hMedia);
	SetTreeItemData(hWAV, Is_ResourceShare_FileType, NULL, 2, wavFileType);
	treeList->AddTail(hWAV);

	HTREEITEM hMP3 = m_pTree->InsertItem("MP3", 25, 25, hMedia);
	SetTreeItemData(hMP3, Is_ResourceShare_FileType, NULL, 2, mp3FileType);
	treeList->AddTail(hMP3);

	HTREEITEM hOther = m_pTree->InsertItem("其他", 26, 26, hShareResource);
	SetTreeItemData(hOther, Is_ResourceShare_FileType, NULL, 2, unknownFileType);
	treeList->AddTail(hOther);

	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetShareInfo,(LPVOID)treeList,CREATE_SUSPENDED,NULL);
	if(handle){
		threadList.AddTail(handle);
		ResumeThread(handle);
	}

	HANDLE handle1 = (HANDLE)_beginthreadex(NULL,0,_GetShareInfoTotal,(LPVOID)treeTotalList,CREATE_SUSPENDED,NULL);
	if(handle1){
		threadList.AddTail(handle1);
		ResumeThread(handle1);
	}

	HANDLE handle2 = (HANDLE)_beginthreadex(NULL,0,_GetShareInfoRoot,(LPVOID)hShareResource,CREATE_SUSPENDED,NULL);
	if(handle2){
		threadList.AddTail(handle2);
		ResumeThread(handle2);
	}
}

////////////////20090103 add by Lins//////////////////////
void CGuidTreeView::InsertBookstore(HTREEITEM hItem)
{
	CTreeItemList* treeList = new CTreeItemList;
	CTreeItemList* treeTotalList = new CTreeItemList;

	HTREEITEM hBookChannel = m_pTree->InsertItem("图书频道(24/24)", 0, 0, hItem);
	SetTreeItemData(hBookChannel, Is_Bookstore, NULL);

	HTREEITEM hZgsztsg = m_pTree->InsertItem("中国数字图书馆", 20, 20, hBookChannel);
	SetTreeItemData(hZgsztsg, Is_Zgsztug, NULL);

	HTREEITEM hTianya = m_pTree->InsertItem("天涯在线书库", 20, 20, hBookChannel);
	SetTreeItemData(hTianya, Is_Tianya, NULL);

	HTREEITEM hRdcxzx = m_pTree->InsertItem("瑞德超星在线图书馆", 20, 20, hBookChannel);
	SetTreeItemData(hRdcxzx, Is_Rdcxzx, NULL);

	HTREEITEM hQjzw = m_pTree->InsertItem("全景中文图书", 20, 20, hBookChannel);
	SetTreeItemData(hQjzw, Is_Qjzw, NULL);

	HTREEITEM hEssk = m_pTree->InsertItem("E书时空", 20, 20, hBookChannel);
	SetTreeItemData(hEssk, Is_Essk, NULL);

	HTREEITEM hDtzw = m_pTree->InsertItem("大唐中文书库", 20, 20, hBookChannel);
	SetTreeItemData(hDtzw, Is_Dtzw, NULL);

	HTREEITEM hSxmd = m_pTree->InsertItem("书香门第", 20, 20, hBookChannel);
	SetTreeItemData(hSxmd, Is_Sxmd, NULL);

	HTREEITEM hRmsc = m_pTree->InsertItem("人民书城", 20, 20, hBookChannel);
	SetTreeItemData(hRmsc, Is_Rmsc, NULL);

	HTREEITEM hKyec = m_pTree->InsertItem("快意恩仇书社", 20, 20, hBookChannel);
	SetTreeItemData(hKyec, Is_Kyec, NULL);

	HTREEITEM hSb = m_pTree->InsertItem("书吧", 20, 20, hBookChannel);
	SetTreeItemData(hSb, Is_Sb, NULL);

	HTREEITEM hFhsc= m_pTree->InsertItem("凤凰书城", 20, 20, hBookChannel);
	SetTreeItemData(hFhsc, Is_Fhsc, NULL);

	HTREEITEM hSjsc = m_pTree->InsertItem("世纪书城", 20, 20, hBookChannel);
	SetTreeItemData(hSjsc, Is_Sjsc, NULL);

	HTREEITEM hZhsk = m_pTree->InsertItem("中华书库", 20, 20, hBookChannel);
	SetTreeItemData(hZhsk, Is_Zhsk, NULL);

	HTREEITEM hSh = m_pTree->InsertItem("书海", 20, 20, hBookChannel);
	SetTreeItemData(hSh, Is_Sh, NULL);

	HTREEITEM hDnsc = m_pTree->InsertItem("电脑书城", 20, 20, hBookChannel);
	SetTreeItemData(hDnsc, Is_Dnsc, NULL);

	HTREEITEM hDssk = m_pTree->InsertItem("读书时空", 20, 20, hBookChannel);
	SetTreeItemData(hDssk, Is_Dssk, NULL);

}

///////////////////////////////////////////////////////////
unsigned __stdcall CGuidTreeView::_GetShareInfoRoot(LPVOID pParam)//共享资源
{
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	HTREEITEM hItem = (HTREEITEM)pParam;
	if(!hItem)
		return 1;
	CString URL, Path;
	URL.Format("%s/share/shareinfo.asp", AspServer);
	Path.Format("%s\\system\\shareinfo_root.inf", CurrentPath);
	if(!DownLoad(URL, Path, m_DCView->quit, m_DCView->socketList, m_DCView->m_mutex))
		return 1;
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	CFile cf;
	if(!cf.Open(Path, CFile::modeRead))
		return 1;
	LPSTR buf = (LPSTR)malloc((int)cf.GetLength()+1);
	ZeroMemory(buf, (int)cf.GetLength()+1);
	cf.Read(buf, (UINT)cf.GetLength());
	cf.Close();
	CString title = m_pDCTree->GetItemText(hItem);
	title += "[";
	title += buf;
	title += "]";
	m_pDCTree->SetItemText(hItem, title);
	free(buf);
	DeleteFile(Path);
	return 0;
}

unsigned __stdcall CGuidTreeView::_GetShareInfoTotal(LPVOID pParam)//共享资源
{
	CTreeItemList* treeList = (CTreeItemList*)pParam;
	if(!treeList)
		return 1;
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	POSITION pos = treeList->GetHeadPosition();
	while(pos){
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		HTREEITEM hItem = treeList->GetNext(pos);
		TreeItemInfo* info = (TreeItemInfo*)m_pDCTree->GetItemData(hItem);
		CString URL, Path;
		URL.Format("%s/share/shareinfo.asp?mode=%d", AspServer, info->BookNumber);
		Path.Format("%s\\temp\\shareinfo%d.inf", CurrentPath, info->BookNumber);
		if(!DownLoad(URL, Path, m_DCView->quit, m_DCView->socketList, m_DCView->m_mutex))
			continue;
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		CFile cf;
		if(!cf.Open(Path, CFile::modeRead))
			continue;
		LPSTR buf = (LPSTR)malloc((size_t)cf.GetLength()+1);
		ZeroMemory(buf, (size_t)cf.GetLength()+1);
		cf.Read(buf,(size_t) cf.GetLength());
		cf.Close();
		CString title = m_pDCTree->GetItemText(hItem);
		title += "[";
		title += buf;
		title += "]";
		m_pDCTree->SetItemText(hItem, title);
		free(buf);
		DeleteFile(Path);
	}
	delete treeList;
	return 0;
}

unsigned __stdcall CGuidTreeView::_GetShareInfo(LPVOID pParam)//共享资源
{
	CTreeItemList* treeList = (CTreeItemList*)pParam;
	if(!treeList)
		return 1;
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	POSITION pos = treeList->GetHeadPosition();
	while(pos){
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		HTREEITEM hItem = treeList->GetNext(pos);
		TreeItemInfo* info = (TreeItemInfo*)m_pDCTree->GetItemData(hItem);
		CString URL, Path;
		URL.Format("%s/share/shareinfo.asp?filetype=%d", AspServer, info->BookNumber);
		Path.Format("%s\\system\\shareinfo%d.inf", CurrentPath);
		if(!DownLoad(URL, Path, m_DCView->quit, m_DCView->socketList, m_DCView->m_mutex))
			continue;
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		CFile cf;
		if(!cf.Open(Path, CFile::modeRead))
			continue;
		LPSTR buf = (LPSTR)malloc((size_t)cf.GetLength()+1);
		ZeroMemory(buf, (size_t)cf.GetLength()+1);
		cf.Read(buf, (size_t)cf.GetLength());
		cf.Close();
		CString title = m_pDCTree->GetItemText(hItem);
		title += "[";
		title += buf;
		title += "]";
		m_pDCTree->SetItemText(hItem, title);
		free(buf);
		DeleteFile(Path);
	}
	delete treeList;
	return 0;
}


void CGuidTreeView::InsertDownload(HTREEITEM hItem)
{
	HTREEITEM hDownload = m_pTree->InsertItem("正在下载", 8, 8, hItem);
	SetTreeItemData(hDownload, Is_Download, NULL);
}

unsigned __stdcall CGuidTreeView::_GetNetInfo(LPVOID pParam)
{
	CGuidTreeView* p = (CGuidTreeView*)pParam;
	if(!p)
		return 1;

	try{
#ifdef SWITCH_ORDER
		//p->GetOrderInfo();
#endif
#ifdef SWITCH_NETRESOURCE
		//p->GetNetLibraryInfo();
#endif
		//p->GetNetStatics();
	}
	catch(...){
#ifdef _DEBUG
		Tips("unsigned __stdcall CGuidTreeView::_GetNetInfo(LPVOID pParam)");
#endif
	}

	HANDLE handle = ::GetCurrentThread();
	p->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	_endthreadex(0);

	return 0;  //deleted by zhanganzhan
}

void CGuidTreeView::GetNetLibraryInfo()
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetNetLibraryInfo,NULL,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}
unsigned __stdcall CGuidTreeView::_GetNetLibraryInfo(LPVOID pParam)//"我的读物群落"
{
	Sleep(1000);
	HANDLE handle = ::GetCurrentThread();
	try{
		char TmpFileName[_MAX_PATH] = {0};
		char ChannelFileName[_MAX_PATH ] = {0};
		sprintf_s(TmpFileName, _MAX_PATH, "%s\\temp\\tmp.web", CurrentPath);
		sprintf_s(ChannelFileName, _MAX_PATH, "%s\\temp\\channel.bin", CurrentPath);
		CString URL;
		CString Path;
		if(!offline){
			CHttpDownLoadDlg dlg;
			if(!m_DCView->ChannelIsNew(ChannelFileName)){
				URL.Format("%s/RetailWeb/getstorechanneltype?regname=%s&version=readings", JspServer, UserName);
				Path.Format("%s\\temp\\tmp.web", CurrentPath);
				if(!DownLoad(URL, Path, pMainWnd->m_quit, m_DCView->socketList))
				{
					if (pMainWnd->m_quit)
					{
						return 0;
					}
					Tips("CGuidTreeView::GetNetLibraryInfo");
					m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
					return 0;
				}
				if (pMainWnd->m_quit)
				{
					//m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
					return 0;
				}
				else
					m_DCView->SaveChannelInfo(TmpFileName, ChannelFileName);
			}
			if(!PathFileExists(ChannelFileName)){
				Tips("接收列表文件失败，请确认网络是否可用");
				m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
				return 0;
			}
		}
		m_DCView->InsertNetLibrary(ChannelFileName, m_DCView->hNetLibrary);
		m_DCView->Invalidate(FALSE);
		CFile cf;
		char WebPath[1024] = {0};
		char TempFilePath[_MAX_PATH] = {0};
		sprintf_s(TempFilePath, _MAX_PATH, "%s\\temp\\sta.tmp", CurrentPath);
		sprintf_s(WebPath, 1024, "http://%s/RetailWeb/getstorechanneltype?regname=%s&stat=1", JspServer, UserName);
		if(!DownLoad(WebPath, TempFilePath, pMainWnd->m_quit, m_DCView->socketList))
		{
			if (pMainWnd->m_quit)
			{
				return 0;
			}
			m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
			return 0;
		}
		if (pMainWnd->m_quit)
		{
			//m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
			return 0;
		}
		else{
			if(cf.Open(TempFilePath,CFile::modeRead))
			{
				long len = (long)cf.GetLength();
				if(len>0){
					char* buf = (char*)malloc(len+1);
					ZeroMemory(buf, len+1);
					cf.Read(buf, len);
					CString itemTitle;
					CString temp = m_pDCTree->GetItemText(m_DCView->hNetResource);
					if(temp.ReverseFind('[')>0)
						itemTitle = temp.Left(temp.ReverseFind('['));
					else
						itemTitle = m_pDCTree->GetItemText(m_DCView->hNetResource);
					CString nstr;
					nstr.Format("%s[%s]", itemTitle, buf),
						m_pDCTree->SetItemText(m_DCView->hNetResource,nstr);
					free(buf);
				}
				cf.Close();
			}
			DeleteFile(TempFilePath);
		}
	}
	catch(...){
#ifdef _DEBUG
		//	AfxMessageBox("void CGuidTreeView::GetNetLibraryInfo()");
#endif
	}
	m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	return 0;
}
BOOL CGuidTreeView::ChannelIsNew(char *FileName)
{
	try{
		CFile cf;
		if(!cf.Open(FileName, CFile::modeRead))
			return FALSE;
		ChannelFileHead head = {0};
		cf.Read(&head,sizeof(ChannelFileHead));
		cf.Close();
		char URL[1024]={0};
		sprintf_s(URL, 1024, "http://%s/RetailWeb/getstorechanneltype?regname=%s&refreshdate=1", JspServer, UserName);
		char TmpFileName[1024] = {0};
		sprintf_s(TmpFileName, 1024, "%s/temp/nowchannelupdatetime.tmp", CurrentPath);
		if(!DownLoad(URL, TmpFileName, quit, socketList))
			return FALSE;
		char T[11] = {0};
		if(!cf.Open(TmpFileName, CFile::modeRead))
			return FALSE;
		if(cf.GetLength()<10){
			cf.Close();
			return FALSE;
		}
		cf.Read(T, 10);
		cf.Close();
		DeleteFile(TmpFileName);
		DWORD time = (DWORD)atoi(T);
		if(head.UpdateChannelTime >= time)
			return TRUE;
	}
	catch(...){
#ifdef _DEBUG
		Tips("BOOL CGuidTreeView::ChannelIsNew(char *FileName)");
#endif
	}
	return FALSE;
}

HTREEITEM CGuidTreeView::InsertChannelItem(HTREEITEM hRoot, char *FileName)
{
	/*	bool isone = true;

	FILE *fp = fopen(FileName, "rb");
	if(!fp)
	return NULL;
	fseek(fp, 0, 2);
	long flen = ftell(fp);
	fseek(fp, 0, 0);

	HTREEITEM Itemlist[100] = {0};
	int itemnum = 0;
	ChannelFileHead head = {0};
	fread(&head, 1, sizeof(ChannelFileHead), fp);
	HTREEITEM tmpItem = NULL;
	HTREEITEM hItem = NULL;

	int num = (flen - sizeof(ChannelFileHead))/sizeof(ChannelInfo);

	int cnt = 0;//ToolTip
	UINT expandflag = 0;

	for (int i = 0; i < num; i++)
	{
	ChannelInfo info = {0};
	fread(&info, 1, sizeof(ChannelInfo), fp);
	if(info.Type == 3)
	{
	hItem = pTree->InsertItem(info.Name, 0, 0, hRoot);
	SetTreeItemData(hItem, Is_HanlinChannel, NULL);
	}
	else if(info.Type == 1)
	{
	if(!hItem)
	continue;
	BOOL flag = FALSE;
	char Name[60] = {0};
	char *head, *tail;
	head = strchr(info.Name, '(');

	if(head)
	{
	tail = strchr(head, ')');
	memcpy(Name, info.Name, head-info.Name);
	memcpy(Tip[cnt], head+1, tail-head-1);
	flag = TRUE;
	}
	else
	{
	strcpy(Name, info.Name);
	}
	expandflag = 0;

	if(info.Numbers == 0)
	{
	tmpItem = pTree->InsertItem(Name, 2, 2, hItem);
	}
	else if(info.Numbers == 1){
	tmpItem = pTree->InsertItem(Name, 12, 12, hItem);
	}
	else if(info.Numbers == 2){
	tmpItem = pTree->InsertItem(Name, 13, 13, hItem);
	}
	if(expandflag)
	pTree->Expand(tmpItem, TVE_EXPAND);


	SetTreeItemData(tmpItem, Is_HanlinChannel, info.ID);

	if(info.Pages == 1)
	{
	Itemlist[itemnum] = tmpItem;
	itemnum++;
	}

	if(isone)
	{
	m_firstChannelItem = tmpItem;
	m_curChannelItem = tmpItem;
	isone = false;
	}
	}
	else if(info.Type == 2)
	{
	if(!hItem)
	continue;

	char Show[300] = {0};
	sprintf(Show, "%s(%d册)", info.Name, info.Numbers);
	HTREEITEM itm = pTree->InsertItem(Show, 4, 4, tmpItem);
	SetTreeItemData(itm, Is_Hanlin, info.ID, info.Pages, info.Numbers);
	}
	}
	fclose(fp);

	return hItem;*/
	return NULL;
}

void CGuidTreeView::SaveChannelInfo(char *FileName, char *ChannelFileName)
{
	try{
		CFile cf;
		if(!cf.Open(FileName,CFile::modeRead))
			return;
		long flen = (long)cf.GetLength();

		if(flen <= 10){
			cf.Close();
			return;
		}
		char len[20] = {0};
		cf.Read(len, 10);

		// 	if(flen != atoi(len) + 30){
		// 		cf.Close();
		// 		return;
		// 	}
		char T[20] = {0};
		cf.Read(T, 10);
		DWORD Time = atoi(T);
		char Ver[10] = {0};
		cf.Seek(25, CFile::begin);
		cf.Read(Ver, 5);	//版本信息已废止
		char *buf = (char *)malloc(flen-30);
		ZeroMemory(buf, flen-30);
		cf.Seek(30, CFile::begin);
		cf.Read(buf, flen-30);
		cf.Close();

		if(!cf.Open(ChannelFileName, CFile::modeWrite|CFile::modeCreate))
		{
			free(buf);
			return;
		}
		ChannelFileHead head = {0};
		head.UpdateChannelTime = Time;
		CTime Now = CTime::GetCurrentTime();
		head.UpdateFileTime = Now.GetYear() * 1000000 + Now.GetMonth() * 10000 + Now.GetDay() * 100 + Now.GetHour();
		cf.Write(&head, sizeof(ChannelFileHead));
		char *tmp = buf;
		while(tmp){
			char type[4][200] = {0};
			int i = 0;
			long num = 0;
			char *chr = tmp;
			while(i < 4){
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
			ChannelInfo cInfo = {0};
			cInfo.Type = atoi(type[0]);
			strcpy_s(cInfo.Name, 60, type[2]);
			strcpy_s(cInfo.ID, 20, type[1]);
			cInfo.Pages = atoi(type[3]);
			char *Nt = strstr(type[3], "-");
			if(Nt){
				cInfo.Numbers = atoi(Nt + 1);
			}
			if(strlen(cInfo.Name) != 0)
				cf.Write(&cInfo, sizeof(ChannelInfo));

			tmp = strchr(tmp+1, 0x03);
			if(tmp)
				tmp++;
		}
		free(buf);
		cf.Close();
		//	DeleteFile(FileName);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::SaveChannelInfo(char *FileName, char *ChannelFileName)");
#endif
	}
}

void CGuidTreeView::InsertNetLibrary(char *FileName, HTREEITEM hItem)
{
	try{
		CFile cf;
		if(!cf.Open(FileName,CFile::modeRead))
			return;
		long flen = (long)cf.GetLength();
		ChannelFileHead head = {0};
		cf.Read(&head,sizeof(ChannelFileHead));
		int num = (flen - sizeof(ChannelFileHead))/sizeof(ChannelInfo);

		HTREEITEM hcurrItem = hItem;
		for (int i = 0; i < num; i++){
			ChannelInfo info = {0};
			cf.Read(&info, sizeof(ChannelInfo));
			if(info.Type == 3){
				//根节点
				continue;
			}
			else if(info.Type == 1){
				if(!hItem)
					continue;
				BOOL flag = FALSE;
				char Name[60] = {0};
				char *head, *tail;
				head = strchr(info.Name, '(');
				if(head){
					tail = strchr(head, ')');
					memcpy(Name, info.Name, head-info.Name);
					flag = TRUE;
				}
				else{
					strcpy_s(Name, 60, info.Name);
				}
				HTREEITEM tmpItem = NULL;

				if(info.Numbers == 0){
					tmpItem = m_pTree->InsertItem(Name, 2, 2, hItem);
				}
				else if(info.Numbers == 1){
					tmpItem = m_pTree->InsertItem(Name, 12, 12, hItem);
				}
				else if(info.Numbers == 2){
					tmpItem = m_pTree->InsertItem(Name, 13, 13, hItem);
				}
				else
					tmpItem = m_pTree->InsertItem(Name, 2, 2, hItem);

				SetTreeItemData(tmpItem, Is_Channel, info.ID);
				hcurrItem = tmpItem;
			}
			else if(info.Type == 2){
				if(!hcurrItem)
					continue;
				char Show[300] = {0};
				sprintf_s(Show, 300, "%s(%d册)", info.Name, info.Numbers);
				HTREEITEM itm = m_pTree->InsertItem(Show, 4, 4, hcurrItem);
				SetTreeItemData(itm, Is_BookType, info.ID, info.Pages, info.Numbers);
			}
		}
		cf.Close();
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::InsertNetLibrary(char *FileName, HTREEITEM hItem)");
#endif
	}
}

void CGuidTreeView::GetOrderInfo()
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetOrderInfo,NULL,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}

}
unsigned __stdcall CGuidTreeView::_GetOrderInfo(LPVOID pParam)//"我的读物群落"
{
	HANDLE handle = ::GetCurrentThread();
	try{
		char orderPath[_MAX_PATH] = {0};
		sprintf_s(orderPath, _MAX_PATH, "%s\\temp\\orderchannel.bin", CurrentPath);

		char orderTmpPath[_MAX_PATH] = {0};
		sprintf_s(orderTmpPath,  _MAX_PATH, "%s\\temp\\orderchannel.ibx", CurrentPath);

		char WebPath[1024] = {0};
		sprintf_s(WebPath, 1024, "%s/order/ordercenter.asp?id=%s", AspServer, UserName);

		if(!DownLoad(WebPath,orderTmpPath, pMainWnd->m_quit, m_OCView->socketList))
		{
			if (pMainWnd->m_quit)
			{
				return 0;
			}
			m_OCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
			return 0;
		}
		if (pMainWnd->m_quit)
		{
			//m_OCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
			return 0;
		}
		m_OCView->SaveOrderItem(orderTmpPath,orderPath);
		m_OCView->InsertOrderItem(orderPath);

		CFile cf;
		char TempFilePath[_MAX_PATH] = {0};
		sprintf_s(TempFilePath, _MAX_PATH,  "%s\\temp\\sta.tmp", CurrentPath);
		ZeroMemory(WebPath, 1024);
		sprintf_s(WebPath, 1024, "%s/order/getstatics.asp", AspServer);
		if(!DownLoad(WebPath, TempFilePath, pMainWnd->m_quit, m_OCView->socketList))
		{
			if (pMainWnd->m_quit)
			{
				return 0;
			}
			m_OCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
			return 0;
		}
		if (pMainWnd->m_quit)
		{
			//m_OCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
			return 0;
		}
		else{
			if(cf.Open(TempFilePath,CFile::modeRead)){
				long len = (long)cf.GetLength();
				if(len>0){
					char* buf = (char*)malloc(len+1);
					ZeroMemory(buf, len+1);
					cf.Read(buf,len);
					CString itemTitle;
					CString temp = m_pOCTree->GetItemText(m_OCView->hOrderItem);
					if(temp.ReverseFind('[')>0)
						itemTitle = temp.Left(temp.ReverseFind('['));
					else
						itemTitle = m_pOCTree->GetItemText(m_OCView->hOrderItem);
					CString nstr;
					nstr.Format("%s[%s]", itemTitle, buf);
					m_pOCTree->SetItemText(m_OCView->hOrderItem,nstr);
					free(buf);
				}
				cf.Close();
			}
			DeleteFile(TempFilePath);
		}
	}
	catch(...){
#ifdef _DEBUG
		//	AfxMessageBox("void CGuidTreeView::GetOrderInfo()");
#endif
	}
	m_OCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}
void CGuidTreeView::SaveOrderItem(char* FileName,char* destPath)
{
	try{
		CFile cf;
		if(!cf.Open(FileName, CFile::modeRead))
			return;
		long len = (long)cf.GetLength();
		if(len<=10){
			cf.Close();
			return;
		}

		char tmp[20]= {0};
		cf.Read(tmp,10);
		if(atol(tmp)!=len-10){
			cf.Close();
			return;
		}

		char* buf = (char*)malloc(len-10+1);
		ZeroMemory(buf, len-10+1);
		cf.Read(buf, len-10);
		cf.Close();

		if(!cf.Open(destPath,CFile::modeCreate|CFile::modeWrite)){
			free(buf);
			cf.Close();
		}
		char* p = buf;
		while(p){
			char type[4][210] = {0};
			int i = 0;
			int num = 0;
			char* chr = p;
			while(i < 4){
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
			OrderChannelInfo info = {0};
			info.clevel = atoi(type[0]);
			info.id = atoi(type[2]);
			strcpy_s(info.name, 200, type[1]);
			cf.Write(&info, sizeof(OrderChannelInfo));
		}
		cf.Close();
		free(buf);
		DeleteFile(FileName);
	}
	catch(...)
	{
#ifdef _DEBUG
		//	AfxMessageBox("void CGuidTreeView::SaveOrderItem(char* FileName,char* destPath)");
#endif
	}
	return;
}

void CGuidTreeView::InsertOrderItem(char *FilePath)
{
	try{
		CFile cf;
		if(!cf.Open(FilePath, CFile::modeRead))
			return;
		long len = (long)cf.GetLength();

		char* buf = (char*)malloc(len+1);
		memset(buf, 0, len+1);
		cf.Read(buf, len);
		cf.Close();

		for(int lp=0; lp<(int)(len/sizeof(OrderChannelInfo)); lp++){
			OrderChannelInfo* info = (OrderChannelInfo*)(buf+lp*sizeof(OrderChannelInfo));
			if(info->clevel == 0){ //数字报纸
				HTREEITEM hItem = m_pTree->InsertItem(info->name,22,22,hpaperItem);
				SetTreeItemData(hItem,Is_OrderPaper_Node,NULL,info->id);
			}
			else if(info->clevel == 1){	//电子杂志
				HTREEITEM hItem = m_pTree->InsertItem(info->name,12,12,hmagItem);
				SetTreeItemData(hItem,Is_OrderMagzine_Node,NULL,info->id);
			}
		}
	}
	catch(...){
#ifdef _DEBUG
		//	AfxMessageBox("void CGuidTreeView::InsertOrderItem(char *FilePath)");
#endif
	}
}

void CGuidTreeView::GetNetStatics()
{
	try{
		CFile cf;
		char WebPath[1024] = {0};
		char TempFilePath[_MAX_PATH] = {0};
		sprintf_s(TempFilePath, _MAX_PATH,  "%s\\temp\\sta.tmp", CurrentPath);
#ifdef SWITCH_NETRESOURCE
		sprintf(WebPath, "http://%s/RetailWeb/getstorechanneltype?regname=%s&stat=1", JspServer, UserName);
		if(!DownLoad(WebPath, TempFilePath, quit, socketList))
		{
			return;
		}
		else{
			if(cf.Open(TempFilePath,CFile::modeRead))
			{
				long len = cf.GetLength();
				if(len>0){
					char* buf = (char*)malloc(len+1);
					ZeroMemory(buf, len+1);
					cf.Read(buf, len);
					CString itemTitle;
					CString temp = m_pTree->GetItemText(hNetResource);
					if(temp.ReverseFind('[')>0)
						itemTitle = temp.Left(temp.ReverseFind('['));
					else
						itemTitle = m_pTree->GetItemText(hNetResource);
					CString nstr;
					nstr.Format("%s[%s]", itemTitle, buf),
						m_pTree->SetItemText(hNetResource,nstr);
					free(buf);
				}
				cf.Close();
			}
			DeleteFile(TempFilePath);
		}
#endif
#ifdef SWITCH_ORDER

#endif
	}
	catch(...){
#ifdef _DEBUG
		//	AfxMessageBox("void CGuidTreeView::GetNetStatics()");
#endif
	}
}

void CGuidTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (!pMainWnd->isfreashing)
	{
		pMainWnd->PostMessage(WM_TREEITEM_SELECTED, (WPARAM)pNMTreeView,(LPARAM)thing_tree);
	}
	*pResult = 0;
}

LRESULT CGuidTreeView::OnThreadFinished(WPARAM w,LPARAM l)
{
	HANDLE handle = (HANDLE)w;
	POSITION pos = threadList.Find(handle);
	if(pos)
		threadList.RemoveAt(pos);
	return TRUE;
}

void CGuidTreeView::OnDestroy() 
{
	KillTimer(1);	
	quit = TRUE;
	//TernminateAllThread(socketList, threadList);
	CTreeView::OnDestroy();
}

void CGuidTreeView::InsertFriendList(HTREEITEM hItem)
{
	hFriend = m_pTree->InsertItem("在线好友", 3, 3, hItem);
	SetTreeItemData(hFriend, Is_NetFriendList, NULL);
}

void CGuidTreeView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	try{
		TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(pNMTreeView->itemOld.hItem);
		if(!info)
			free(info);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CGuidTreeView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)");
#endif
	}	
	*pResult = 0;
}

void CGuidTreeView::ShowTree()
{
	KillTimer(1);
	quit = TRUE;
	TernminateAllThread(socketList, threadList);
	quit = FALSE;
	m_pTree->DeleteAllItems();

	pTreeView = this;
	pTree = m_pTree;

	HANDLE handle = (HANDLE)_beginthreadex(NULL,0, _ShowTree,(LPVOID)this,CREATE_SUSPENDED,NULL);
	if(handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}


void CGuidTreeView::InsertSDFolderItem(HTREEITEM hItem)
{
	if(strlen(SDDisk)==0)
		return;

	DWORD data = 0;
	CFileFindEx find(InsertSDFolder, SetFolderStatics, CountFile);
	find.Find(SDDisk, &hItem, data);
	CString title;
	title.Format("%s(%d本)", m_pTree->GetItemText(hItem), data);
	m_pTree->SetItemText(hItem, title);
	hSDDisk = hItem;
}

void CGuidTreeView::FreeTreeItem(HTREEITEM item)
{
	HTREEITEM hItem = item;
	while(hItem){
		TreeItemInfo *info = (TreeItemInfo*)m_pTree->GetItemData(hItem);
		if(m_pTree->GetChildItem(hItem))
			FreeTreeItem(m_pTree->GetChildItem(hItem));
		hItem = m_pTree->GetNextItem(hItem, TVGN_NEXT);
	}
}

void CGuidTreeView::InsertRemoveableDisk()
{
	if(strlen(SDDisk)==0)
		return;
	// 	if (!uDiskAuthInfo.flag)
	// 	{
	// 		CUDiskAuth *uDisk = new CUDiskAuth;
	// 		uDisk->GetUSBDiskID(SDDisk[0]);
	// 	}
	// 	if (uDiskAuthInfo.flag && uDiskAuthInfo.diskFlag == SDDisk[0])
	{ // modified by zhanganzhan
		char Txt[100] = "我的阅读器";
		HTREEITEM hItem = m_pTree->InsertItem(Txt, 7, 7, NULL);
		SetTreeItemData(hItem, Is_SDCard, NULL);
		InsertSDFolderItem(hItem);
		pMainWnd->GetUserInfo();
	}

}

void CGuidTreeView::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1){
		char Path[_MAX_PATH] = {0};

		char collectName[100] = {0};
		char collect_style[100] = {0};
		sprintf_s(Path, _MAX_PATH, "%s/system/config.ini", CurrentPath);
		GetPrivateProfileString("favor","collect-style", "", collect_style, 100, Path);
		if (strlen(collect_style)>0 && atoi(collect_style)==1) {
			char precent[100] = {0};
			GetPrivateProfileString("favor","spec-precent", "", precent, 100, Path);
			int iprecent = 0;
			if (strlen(precent) > 0) {
				iprecent = atoi(precent);
				sprintf_s(collectName, 100, "桌面读物(已完成%d.0%c)", ((iprecent>99)?99:iprecent), '%');
			}
			else
				strcpy_s(collectName, 100, "桌面读物");

			iprecent += 5;
			memset(precent, 0, 100);
			sprintf_s(precent, 100, "%d", iprecent);
			WritePrivateProfileString("favor", "spec-precent", precent, Path);
		}
		else {
			sprintf_s(Path, _MAX_PATH,  "%s/system/sysinfo.ini", CurrentPath);
			char tFSize[100] = {0};
			GetPrivateProfileString("UpdateDB","FinishSize", "", tFSize, 100, Path);
			char tTSize[100] = {0};
			GetPrivateProfileString("UpdateDB","TotalSize", "", tTSize, 100, Path);
			char collectName[100] = {0};
			if(strlen(tTSize)>0){
				__int64 tsize = _atoi64(tTSize);
				__int64 fsize = 0;
				if(strlen(tFSize)>0)
					fsize = _atoi64(tFSize);
				if((float)fsize/(float)tsize*100.0>99)
					sprintf_s(collectName, 100, "桌面读物(已完成99.9%c)", '%');
				else{
					sprintf_s(collectName, 100, "桌面读物(已完成%0.1f%c)", (float)fsize/(float)tsize*100.0, '%');
				}
			}
			else
				strcpy_s(collectName, 100, "桌面读物");
		}
		m_pTree->SetItemText(m_MyBookcaseItem, collectName);
	}

	CTreeView::OnTimer(nIDEvent);
}

void CGuidTreeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeView::OnRButtonDown(nFlags, point);

	hit_Item = m_pTree->HitTest(point);

	if(!hit_Item)
		return;

	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	HTREEITEM next_hit_Item;

	if(!info)
		return;
	ClientToScreen(&point);
	CString url;
	CString destPath;

	CMenu* pMenu;
	switch(info->Type){
	case Is_MyFavourate:
		m_menu.LoadMenu(IDR_MENU_TREE);
		pMenu = m_menu.GetSubMenu(0);
		pMenu->DeleteMenu(3,MF_BYPOSITION);
		pMenu->DeleteMenu(1,MF_BYPOSITION);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;
	case Is_SDCard:
		m_menu.LoadMenu(IDR_MENU_TREE);
		pMenu = m_menu.GetSubMenu(0);
		pMenu->DeleteMenu(1,MF_BYPOSITION);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;
	case Is_MyFavourate_Folder:
	case Is_SDCard_Folder:
		m_menu.LoadMenu(IDR_MENU_TREE);
		pMenu = m_menu.GetSubMenu(0);
		pMenu->DeleteMenu(3,MF_BYPOSITION);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;
		//////////////////////////////////////////////////////////////////////////
	case Is_NetGroup:
		m_menu.LoadMenu(IDR_MENU_RE_GROUP);
		m_menu.EnableMenuItem(ID_MENU_DELETE,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
		m_menu.EnableMenuItem(ID_MENU_CREATE_REGROUP,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
		m_menu.EnableMenuItem(ID_MENU_CREATE_LEVELGROUP,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);			
		pMenu = m_menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;
	case Is_MyResource:
		m_menu.LoadMenu(IDR_MENU_MYRESOURCE);					
		pMenu = m_menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;			
	case Is_NetBook:
		//读取用户操作权限。这里可以对用户右键操作左树进行限制。

		/*		url.Format("%s/share/getUserPermission.asp?username=%s&id=%d", AspServer,UserName,info->BookNumber);
		destPath.Format("%s\\temp\\userPermission.txt", CurrentPath);
		//DeleteFile(destPath);

		if(!DownLoad(url,destPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
		{
		break;
		}
		else
		{
		FILE* fp;
		fp = fopen(destPath,"rb");
		if(fp)
		{
		int ch;
		ch = fgetc(fp);
		fclose(fp);
		if ((char)ch == '0')//没有操作权限。
		{
		m_menu.LoadMenu(IDR_MENU_NETBOOK);
		m_menu.EnableMenuItem(ID_MENU_DELNETBOOK,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);			
		m_menu.EnableMenuItem(ID_MENU_ADDNETBOOK,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);			
		m_menu.EnableMenuItem(ID_MENU_SETNETBOOK,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);			

		pMenu = m_menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

		break;
		}
		else
		{
		next_hit_Item = m_pTree->GetChildItem(hit_Item);
		//最底层节点，可以删除。
		if (next_hit_Item == NULL)
		{
		m_menu.LoadMenu(IDR_MENU_NETBOOK);
		pMenu = m_menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;
		}
		//非最底层节点，不能删除。
		m_menu.LoadMenu(IDR_MENU_NETBOOK);
		m_menu.EnableMenuItem(ID_MENU_DELNETBOOK,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);			
		pMenu = m_menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;
		}
		}

		}
		*/
		//非最底层节点，不能删除。
		m_menu.LoadMenu(IDR_MENU_NETBOOK);
		m_menu.EnableMenuItem(ID_MENU_DELNETBOOK,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
		//m_menu.EnableMenuItem(ID_MENU_MAKEBOOK,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
		pMenu = m_menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;
	case Is_NetBookLeaf:
		m_menu.LoadMenu(IDR_MENU_NETBOOK);
		m_menu.EnableMenuItem(ID_MENU_DELNETBOOK,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
		//m_menu.EnableMenuItem(ID_MENU_ADDNETBOOK,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
		pMenu = m_menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		break;
	case Is_NetGroup_Level:
		//判断是否为 层叶结点， 是层叶结点才能添加
		next_hit_Item = m_pTree->GetChildItem(hit_Item);
		if (next_hit_Item == NULL)
		{
			//弹出添加1、Add group 2、Add level的菜单 3、Delete item菜单项
			m_menu.LoadMenu(IDR_MENU_RE_GROUP);
			//			m_menu.EnableMenuItem(ID_MENU_TOMYFAVOR,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
			m_menu.EnableMenuItem(ID_MENU_DELETE,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
			pMenu = m_menu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			break;
		}
		TreeItemInfo* subinfo = (TreeItemInfo*)m_pTree->GetItemData(next_hit_Item);
		//判断子结点是否为group，若是则弹出只有 Add group的菜单
		if (subinfo->Type == Is_NetGroup){
			m_menu.LoadMenu(IDR_MENU_RE_GROUP);
			//			m_menu.EnableMenuItem(ID_MENU_TOMYFAVOR,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);

			m_menu.EnableMenuItem(ID_MENU_CREATE_LEVELGROUP,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
			m_menu.EnableMenuItem(ID_MENU_DELETE,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
			pMenu = m_menu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			break;
		}
		//判断子结点是否为level，若是则弹出只有 Add level的菜单
		if (subinfo->Type == Is_NetGroup_Level)
		{
			m_menu.LoadMenu(IDR_MENU_RE_GROUP);
			//			m_menu.EnableMenuItem(ID_MENU_TOMYFAVOR,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);

			m_menu.EnableMenuItem(ID_MENU_CREATE_REGROUP,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);
			m_menu.EnableMenuItem(ID_MENU_DELETE,MF_DISABLED | MF_BYCOMMAND | MF_GRAYED);			
			pMenu = m_menu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			break;
		}			
		break;
	}

	m_menu.DestroyMenu();
}

void CGuidTreeView::OnMenuitemOpenfolder() 
{
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	if(!info)
		return;
	if(info->Type!=Is_SDCard_Folder && info->Type!=Is_MyFavourate_Folder
		&& info->Type!=Is_SDCard && info->Type!=Is_MyFavourate)
		return;

	CString relativePath;
	HTREEITEM hCurItem = hit_Item;
	while(info->Type==Is_MyFavourate_Folder||info->Type==Is_SDCard_Folder){
		CString tempPath = relativePath;
		CString name = m_pTree->GetItemText(hCurItem);
		int index = -1;
		if((index = name.ReverseFind('('))<0)
			relativePath.Format("%s\\%s", name, tempPath);
		else
			relativePath.Format("%s\\%s", name.Left(index), tempPath);
		hCurItem = m_pTree->GetParentItem(hCurItem);
		info = (TreeItemInfo*)m_pTree->GetItemData(hCurItem);
	};
	CString curFolderPath;
	if(info->Type==Is_MyFavourate)
		curFolderPath.Format("%s\\%s", FavorPath, relativePath);
	else
		curFolderPath.Format("%s\\%s", SDDisk, relativePath);

	::ShellExecute(NULL, "open", curFolderPath, NULL, NULL, SW_SHOW);

}

void CGuidTreeView::OnMenuitemAddfolder() 
{
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	if(!info)
		return;
	if(info->Type!=Is_SDCard_Folder && info->Type!=Is_MyFavourate_Folder
		&& info->Type!=Is_SDCard && info->Type!=Is_MyFavourate)
		return;

	CInputDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	CString relativePath;
	HTREEITEM hCurItem = hit_Item;
	while(info->Type==Is_MyFavourate_Folder||info->Type==Is_SDCard_Folder){
		CString tempPath = relativePath;
		CString name = m_pTree->GetItemText(hCurItem);
		int index = -1;
		if((index = name.ReverseFind('('))<0)
			relativePath.Format("%s\\%s", name, tempPath);
		else
			relativePath.Format("%s\\%s", name.Left(index), tempPath);
		hCurItem = m_pTree->GetParentItem(hCurItem);
		info = (TreeItemInfo*)m_pTree->GetItemData(hCurItem);
	};
	CString tempPath = relativePath;
	relativePath.Format("%s%s", tempPath,dlg.m_str);
	CString curFolderPath;
	if(info->Type==Is_MyFavourate)
		curFolderPath.Format("%s\\%s", FavorPath, relativePath);
	else
		curFolderPath.Format("%s\\%s", SDDisk, relativePath);
	if(PathFileExists(curFolderPath)){
		MessageBox("该分类已经存在！", "错误", MB_OK);
		return;
	}
	if(-1==_mkdir(curFolderPath)){
		MessageBox("分类创建失败！", "错误", MB_OK);
		return;
	}
	HTREEITEM hNewItem = m_pTree->InsertItem(dlg.m_str, 4, 4, hit_Item);
	if(info->Type==Is_MyFavourate)
		SetTreeItemData(hNewItem, Is_MyFavourate_Folder, NULL);
	else
		SetTreeItemData(hNewItem, Is_SDCard_Folder, NULL);
	m_pTree->Invalidate(FALSE);
}

void CGuidTreeView::OnMenuitemDelfolder() 
{
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	if(!info)
		return;
	if(info->Type!=Is_MyFavourate_Folder
		&& info->Type!=Is_SDCard_Folder)
		return;

	if(IDYES!=MessageBox("删除此分类，会移除此分类下的所有文件，是否继续？",SOFTWARE_NAME,MB_YESNO))
		return;

	CString relativePath;
	HTREEITEM hCurItem = hit_Item;
	while(info->Type==Is_MyFavourate_Folder||info->Type==Is_SDCard_Folder){
		CString tempPath = relativePath;
		CString name = m_pTree->GetItemText(hCurItem);
		int index = -1;
		if((index = name.ReverseFind('('))<0)
			relativePath.Format("%s\\%s", name, tempPath);
		else
			relativePath.Format("%s\\%s", name.Left(index), tempPath);
		hCurItem = m_pTree->GetParentItem(hCurItem);
		info = (TreeItemInfo*)m_pTree->GetItemData(hCurItem);
	};

	CString curFolderPath;
	if(info->Type==Is_MyFavourate)
		curFolderPath.Format("%s\\%s", FavorPath, relativePath);
	else
		curFolderPath.Format("%s\\%s", SDDisk, relativePath);

	CPushAdv::DeleteAllFiles(curFolderPath);
	_rmdir(curFolderPath);
	m_pTree->DeleteItem(hit_Item);
	m_pTree->Invalidate(FALSE);
}


void CGuidTreeView::GetSubGroup(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetSubGroup,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}

void CGuidTreeView::GetSubNetBook(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetSubNetBook,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}

void CGuidTreeView::GetSubFanshuBook(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetSubFanshuBook,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}

void CGuidTreeView::GetSubPublishCom(HTREEITEM hItem)
{

	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetSubPublishCom,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}

void CGuidTreeView::GetSubBookType(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetSubBookType,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}
void CGuidTreeView::GetMySub(HTREEITEM hItem)//"我的读物群落"
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetMySub,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}
void CGuidTreeView::GetMyReGroup(HTREEITEM hItem)//"我的读物群落"
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetMyGroup,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if (handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}

/*
unsigned __stdcall CGuidTreeView::_GetSubGroup(LPVOID pParam)
{
HTREEITEM hItem = (HTREEITEM)pParam;
TreeItemInfo* info = (TreeItemInfo*)pTree->GetItemData(hItem);

if(!info)

return 1;

CString url;

url.Format("%s/share/getGroupNode.asp?fid=%d", AspServer, info->BookNumber);

CString dstPath;



//以天为单位进行缓存的更新

CTime ct = CTime::GetCurrentTime();

int day = ct.GetDay();

int month = ct.GetMonth();

dstPath.Format("%s\\temp\\node_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);

//不存在则下载

//       if((fopen(dstPath,"r"))==NULL)                    //要打开的文件名
//       {
//                 if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
//                          return 1;
//       }
if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
{//zhanganzhan 
int tempMonth=month,tempDay=day+1; //从今天开始搜索以前的缓存       
int flag =1;//控制搜索次数的标识         
while(flag)
{
while(true)
{
dstPath.Format("%s\\temp\\node_%d_%d_%d.xml", CurrentPath, 
info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
FILE *fp=fopen(dstPath,"r");//确认该缓存文件是否存在
flag++;
if(fp!=NULL)//找到则退出查找，继续文件的解析
{
fclose(fp);
flag =0;                    
break;                    
}  
if(flag>3)//最近三天内没有新的缓存，则舍弃该节点
{
// Tips("高校部落内部缓存",td_alert,5);
return 0;
}              
}            
--tempMonth ;//说明本月查找不成功，从上月开始查找
tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
}        

}

int pos = 0;

long searchSum = 0;
IXMLDOMDocumentPtr pDoc;

IXMLDOMNodeListPtr pNodeList;

try{

HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));

if(!SUCCEEDED(hr))

throw 1;    //xml 解析失败

if(!pDoc->load((LPCSTR)dstPath))

throw 2;    //xml 解析失败

pNodeList = pDoc->selectNodes("NodeList/Node");

if(pNodeList){

for(pos=0; pos<pNodeList->length && !pTreeView->quit; pos++){

CString glid = GetXmlValue(pNodeList, "glid", pos);

CString name = GetXmlValue(pNodeList, "name", pos);

CString leaf = GetXmlValue(pNodeList, "leaf", pos);

CString tag        = GetXmlValue(pNodeList, "tag", pos);

// add by dzh 0726

CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);

searchSum += atol(searchCount);   



HTREEITEM hCurItem = pTree->InsertItem(name, 2, 2, hItem);

pTreeView->SetTreeItemData(hCurItem, Is_NetGroup_Level, NULL, atol(searchCount), atol(glid),tag);

if(atoi((LPCSTR)leaf))

pTreeView->AddLeaf(hCurItem);

else

pTreeView->GetSubGroup(hCurItem);

}
pNodeList.Release();

}
pDoc.Release();

}

catch(_com_error& e){}

pTreeView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
CString ftitle;

//设置树名和统计信息

//changed by dzh 080726 统计信息为新搜索到的资源数

//ftitle.Format("%s(%d)",pTree->GetItemText(hItem), pos);

//       if (searchSum>info->TotlePage)

//       {

//                 ftitle.Format("%s(%d)",pTree->GetItemText(hItem), searchSum);

//       }

//       else
ftitle.Format("%s(%d)",pTree->GetItemText(hItem), info->TotlePage);
pTree->SetItemText(hItem, ftitle);

return 0;

}
*/

unsigned __stdcall CGuidTreeView::_GetSubGroup(LPVOID pParam)//“高教课程读物部落”
{
	int i = 0;
	while (i < 200)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		i ++;
		Sleep(500);
	}
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pRLTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/share/getGroupNode.asp?fid=%d", AspServer, info->BookNumber);
	CString dstPath;

	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();
	dstPath.Format("%s\\temp\\node_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	//不存在则下载
	//	if((fopen(dstPath,"r"))==NULL)			//要打开的文件名
	//	{
	//		if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
	//			return 1;
	//	}
	if(!DownLoad(url,dstPath,m_RLView->quit,m_RLView->socketList,m_RLView->m_mutex))
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		int tempMonth=month,tempDay=day;
		int tempM = 12,tempD=31;
		int flag =1;
		while(tempM--&&flag)
		{
			while(tempD--)
			{
				dstPath.Format("%s\\temp\\node_%d_%d_%d.xml", CurrentPath, 
					info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
				FILE *fp=fopen(dstPath,"r");
				if(fp!=NULL)
				{
					fclose(fp);
					flag =0;
					//  Tips("高校部落内部缓存",td_alert,5);
					break;                    
				}                
			}
			--tempMonth ;
			tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
		}
		if(tempM<=0)
		{
			return 0;
		}
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	int pos = 0;
	long searchSum = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		if(pNodeList)
		{
			for(pos=0; pos<pNodeList->length && !m_RLView->quit; pos++)
			{
				CString glid = GetXmlValue(pNodeList, "glid", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				CString leaf = GetXmlValue(pNodeList, "leaf", pos);
				CString tag	 = GetXmlValue(pNodeList, "tag", pos);
				// add by dzh 0726
				CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);
				searchSum += atol(searchCount);	

				HTREEITEM hCurItem = m_pRLTree->InsertItem(name, 2, 2, hItem);
				m_RLView->SetTreeItemData(hCurItem, Is_NetGroup_Level, NULL, (WORD)atol(searchCount), atol(glid),tag);
				if(atoi((LPCSTR)leaf))
					m_RLView->AddLeaf(hCurItem);
				else
					m_RLView->GetSubGroup(hCurItem);
				if (pMainWnd->m_quit)
				{
					return 0;
				}
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			break;
		}
		return 0;
	}
	m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	CString ftitle;
	ftitle.Format("%s(%d)",m_pRLTree->GetItemText(hItem), info->TotlePage);	
	m_pRLTree->SetItemText(hItem, ftitle);

	return 0;
}

unsigned __stdcall CGuidTreeView::_GetSubNetBook(LPVOID pParam)//"网络原创读物"
{
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pDCTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/share/getNetBookNode.asp?fid=%d", AspServer, info->BookNumber);
	CString dstPath;

	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();
	dstPath.Format("%s\\temp\\netBookNode_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	//不存在则下载
	if(!DownLoad(url,dstPath,m_DCView->quit,m_DCView->socketList,m_DCView->m_mutex))
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		int tempMonth=month,tempDay=day;
		int tempM = 12,tempD=31;
		int flag =1;
		while(tempM--&&flag)
		{
			while(tempD--)
			{
				dstPath.Format("%s\\temp\\netBookNode_%d_%d_%d.xml", CurrentPath, 
					info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
				FILE *fp=fopen(dstPath,"r");
				if(fp!=NULL)
				{
					fclose(fp);
					flag =0;
					break;                    
				}                
			}
			--tempMonth ;
			tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
		}
		if(tempM<=0)
		{
			return 0;
		}
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	int pos = 0;
	long searchSum = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		if(pNodeList)
		{
			for(pos=0; pos<pNodeList->length && !m_DCView->quit; pos++)
			{
				CString glid = GetXmlValue(pNodeList, "glid", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				CString url = GetXmlValue(pNodeList,"url",pos);
				CString leaf = GetXmlValue(pNodeList, "leaf", pos);
				CString tag	 = GetXmlValue(pNodeList, "tag", pos);
				// add by dzh 0726
				CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);
				searchSum += atol(searchCount);	

				HTREEITEM hCurItem = m_pDCTree->InsertItem(name, 2, 2, hItem);

				if(atoi((LPCSTR)leaf))
				{
					m_DCView->SetTreeItemData(hCurItem, Is_NetBookLeaf, NULL, (WORD)atol(searchCount), (DWORD)atol(glid),tag,url);
					CString ftitle;	
					BOOL flg = 0;
					//按拼音序，做a，b，c划分线
					for (char c = 'A';c<='Z'; c++)
					{
						CString str;
						str.Format("%c----->>",c);
						if (name.Find(str) != -1)
						{
							ftitle.Format("-----%c-----(%d)",c,atoi(searchCount));
							flg = 1;
							break;
						}		

					}
					if (!flg)
					{	
						ftitle.Format("%s(%d)",m_pDCTree->GetItemText(hCurItem), atoi(searchCount));
					}

					m_pDCTree->SetItemText(hCurItem, ftitle);
					m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
				}
				else
				{
					m_DCView->SetTreeItemData(hCurItem, Is_NetBook, NULL, (WORD)atol(searchCount), (WORD)atol(glid),tag,url);
					m_DCView->GetSubNetBook(hCurItem);
				}
				if (pMainWnd->m_quit)
				{
					return 0;
				}
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			break;
		}
		return 0;
	}
	m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	//	CString ftitle;
	//	ftitle.Format("%s(%d)",m_pDCTree->GetItemText(hItem), info->TotlePage);	
	//	m_pDCTree->SetItemText(hItem, ftitle);
	return 0;
}
unsigned __stdcall CGuidTreeView::_GetSubFanshuBook(LPVOID pParam)//"番薯读物"
{
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pDCTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/share/getFanshuBookNode.asp?fid=%d", AspServer, info->BookNumber);
	CString dstPath;

	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();
	dstPath.Format("%s\\temp\\FanshuBookNode_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	//不存在则下载
	if(!DownLoad(url,dstPath,m_DCView->quit,m_DCView->socketList,m_DCView->m_mutex))
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		int tempMonth=month,tempDay=day;
		int tempM = 12,tempD=31;
		int flag =1;
		while(tempM--&&flag)
		{
			while(tempD--)
			{
				dstPath.Format("%s\\temp\\FanshuBookNode_%d_%d_%d.xml", CurrentPath, 
					info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
				FILE *fp=fopen(dstPath,"r");
				if(fp!=NULL)
				{
					fclose(fp);
					flag =0;
					break;                    
				}                
			}
			--tempMonth ;
			tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));         
		}
		if(tempM<=0)
		{
			return 0;
		}
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	int pos = 0;
	long searchSum = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		if(pNodeList)
		{
			for(pos=0; pos<pNodeList->length && !m_DCView->quit; pos++)
			{
				CString glid = GetXmlValue(pNodeList, "glid", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				CString url = GetXmlValue(pNodeList,"url",pos);
				CString leaf = GetXmlValue(pNodeList, "leaf", pos);
				CString tag	 = GetXmlValue(pNodeList, "tag", pos);

				CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);
				searchSum += atol(searchCount);	

				HTREEITEM hCurItem = m_pDCTree->InsertItem(name, 2, 2, hItem);

				if(atoi((LPCSTR)leaf))
				{
					m_DCView->SetTreeItemData(hCurItem, Is_FanShuLeaf, NULL, (WORD)atol(searchCount), (DWORD)atol(glid),tag,url);
					CString ftitle;
					ftitle.Format("%s(%d)",m_pDCTree->GetItemText(hCurItem), atoi(searchCount));

					m_pDCTree->SetItemText(hCurItem, ftitle);
					m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
				}
				else
				{
					m_DCView->SetTreeItemData(hCurItem, Is_FanShu, NULL, (WORD)atol(searchCount), (DWORD)atol(glid),tag,url);
					m_DCView->GetSubFanshuBook(hCurItem);
				}
				if (pMainWnd->m_quit)
				{
					return 0;
				}
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			break;
		}
		return 0;
	}
	m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	//	CString ftitle;
	//	ftitle.Format("%s(%d)",m_pDCTree->GetItemText(hItem), info->TotlePage);	
	//	m_pDCTree->SetItemText(hItem, ftitle);
	return 0;
}

//获得istation的图书频道Channel节点090618
unsigned __stdcall CGuidTreeView::_GetSubPublishCom(LPVOID pParam)//"读书联盟"
{
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pDCTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/bookstore/getChannelList.asp?sid=%d", AspServer, info->BookNumber);

	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();

	CString dstPath;
	dstPath.Format("%s\\temp\\ChannelList_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	if(!PathFileExists(dstPath))
	{
		if(!DownLoad(url,dstPath,m_DCView->quit,m_DCView->socketList,m_DCView->m_mutex))
		{
			if (pMainWnd->m_quit)
			{
				return 0;
			}
			int tempMonth=month,tempDay=day;
			int tempM = 12,tempD=31;
			int flag =1;
			while(tempM--&&flag)
			{
				while(tempD--)
				{
					dstPath.Format("%s\\temp\\ChannelList_%d_%d_%d.xml", CurrentPath, 
						info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
					FILE *fp=fopen(dstPath,"r");
					if(fp!=NULL)
					{
						fclose(fp);
						flag =0;
						break;                    
					}                
				}
				--tempMonth ;
				tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
			}
			if(tempM<=0)
			{
				return 0;
			}
		}
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	int pos = 0;
	long searchSum = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("ChannelList/Channel");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		if(pNodeList)
		{
			for(pos=0; pos < pNodeList->length && !m_DCView->quit; pos++)
			{
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);	

				HTREEITEM hCurItem = m_pDCTree->InsertItem(name, 2, 2, hItem);
				m_DCView->SetTreeItemData(hCurItem, Is_iChannel, NULL,0, atol(id),"","");
				m_DCView->GetSubBookType(hCurItem);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			break;
		}
		return 0;
	}
	m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}

//获得istation的图书频道Channel节点090618
unsigned __stdcall CGuidTreeView::_GetSubBookType(LPVOID pParam)
{
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pDCTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/bookstore/getBookTypeList.asp?cid=%d", AspServer, info->BookNumber);
	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();

	CString dstPath;
	dstPath.Format("%s\\temp\\TypeList_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	if(!PathFileExists(dstPath))
	{
		if(!DownLoad(url,dstPath,m_DCView->quit,m_DCView->socketList,m_DCView->m_mutex))
		{
			if (pMainWnd->m_quit)
			{
				return 0;
			}
			int tempMonth=month,tempDay=day;
			int tempM = 12,tempD=31;
			int flag =1;
			while(tempM--&&flag)
			{
				while(tempD--)
				{
					dstPath.Format("%s\\temp\\TypeList_%d_%d_%d.xml", CurrentPath, 
						info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
					FILE *fp=fopen(dstPath,"r");
					if(fp!=NULL)
					{
						fclose(fp);
						flag =0;
						break;                    
					}                
				}
				--tempMonth ;
				tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
			}
			if(tempM<=0)
			{
				return 0;
			}
		}
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	int pos = 0;
	long searchSum = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("TypeList/Type");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		if(pNodeList)
		{
			for(pos=0; pos < pNodeList->length && !m_DCView->quit; pos++)
			{
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);	
				CString num = GetXmlValue(pNodeList,"num",pos);

				HTREEITEM hCurItem = m_pDCTree->InsertItem(name, 2, 2, hItem);
				m_DCView->SetTreeItemData(hCurItem, Is_iType, NULL,(WORD)atol(num), (DWORD)atol(id),"","");
				CString ftitle;
				ftitle.Format("%s(%d册)",name,atol(num));	
				m_pDCTree->SetItemText(hCurItem,ftitle);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			break;
		}
		return 0;
	}
	m_DCView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}


//zuoss 090105
unsigned __stdcall CGuidTreeView::_GetMySub(LPVOID pParam)//"我的读物群落"
{
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pRLTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/share/getGroupNode.asp?fid=%d", AspServer, info->BookNumber);
	CString dstPath;

	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();

	dstPath.Format("%s\\temp\\mysubnode_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	if(!DownLoad(url,dstPath,m_RLView->quit,m_RLView->socketList,m_RLView->m_mutex))
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		int tempMonth=month,tempDay=day;
		int tempM = 12,tempD=31;
		int flag =1;
		while(tempM--&&flag)
		{
			while(tempD--)
			{
				dstPath.Format("%s\\temp\\mysubnode_%d_%d_%d.xml", CurrentPath, 
					info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
				FILE *fp=fopen(dstPath,"r");
				if(fp!=NULL)
				{
					fclose(fp);
					flag =0;
					break;                    
				}                
			}
			--tempMonth ;
			tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
		}
		if(tempM<=0)
		{
			return 0;
		}
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	int pos = 0;
	long searchSum = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		if(pNodeList)
		{
			for(pos=0; pos<pNodeList->length && !m_RLView->quit; pos++)
			{
				CString glid = GetXmlValue(pNodeList, "glid", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				CString leaf = GetXmlValue(pNodeList, "leaf", pos);
				CString tag	 = GetXmlValue(pNodeList, "tag", pos);
				CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);
				searchSum += atol(searchCount);	

				HTREEITEM hCurItem = m_pRLTree->InsertItem(name, 2, 2, hItem);
				m_RLView->SetTreeItemData(hCurItem, Is_NetGroup_Level, NULL, (WORD)atol(searchCount), (DWORD)atol(glid),tag);
				if(atoi((LPCSTR)leaf))
					m_RLView->AddLeaf(hCurItem);
				else
					m_RLView->GetMySub(hCurItem);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			break;
		}
		return 0;
	}
	m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());	
	CString ftitle;
	ftitle.Format("%s(%d)",m_pRLTree->GetItemText(hItem), info->TotlePage);

	m_pRLTree->SetItemText(hItem, ftitle);
	return 0;
}
//end 

/*unsigned __stdcall CGuidTreeView::_GetMyGroup(LPVOID pParam)

{
HTREEITEM hItem = (HTREEITEM)pParam;
TreeItemInfo* info = (TreeItemInfo*)pTree->GetItemData(hItem);
//根据UserName返回xml解析并显示
if(!info)

return 1;
CString url;
//http://61.181.14.184/Readman/share/getmyGroupNode.asp?username=Celialin
url.Format("%s/share/getmyGroupNode.asp?username=%s", AspServer, UserName);
CString dstPath;
dstPath.Format("%s\\temp\\myGroupnode_%d.xml", CurrentPath, info->BookNumber);
if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
{//zhanganzhan
FILE *fp=fopen(dstPath,"r");//确认该缓存文件是否存?       
if(fp==NULL)//找到则退出查找，继续文件的解析
{
return 1;                           
}   
else
{
fclose(fp);
}
}

//       return 1;//zhanganzhan

int pos = 0;
IXMLDOMDocumentPtr pDoc;
IXMLDOMNodeListPtr pNodeList;
try{

HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));

if(!SUCCEEDED(hr))

throw 1;    //xml 解析失败

if(!pDoc->load((LPCSTR)dstPath))

throw 2;    //xml 解析失败

pNodeList = pDoc->selectNodes("LeafList/Leaf");

if(pNodeList){

for(pos=0; pos<pNodeList->length && !pTreeView->quit; pos++){

CString gid = GetXmlValue(pNodeList, "gid", pos);

CString gname = GetXmlValue(pNodeList, "gname", pos);

CString totalPeople = GetXmlValue(pNodeList, "totalpeople", pos);

CString totalSource = GetXmlValue(pNodeList,"sourcenum",pos);

CString tag = GetXmlValue(pNodeList,"tag",pos);



CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);

if(searchCount=="")

gname = gname+"(0)";

else

gname = gname+"("+searchCount+")";//加入统计信息



GROUPINFO* ginfo = (GROUPINFO*)calloc(1,sizeof(GROUPINFO));

ginfo->gid = atol((LPCSTR)gid);

strcpy(ginfo->groupname, gname);

ginfo->totalnum  = atol(totalPeople);

HTREEITEM hCurItem = pTree->InsertItem(gname, 28, 28, hItem);

pTreeView->SetTreeItemData(hCurItem, Is_NetGroup, NULL, 0, (DWORD)ginfo,tag);

}

}

}

catch(_com_error& e){}

pTreeView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

return 0;

}*/

unsigned __stdcall CGuidTreeView::_GetMyGroup(LPVOID pParam)//"我的读物群落"
{
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pRLTree->GetItemData(hItem);
	//根据UserName返回xml解析并显示
	if(!info)
		return 1;
	CString url;
	//	url.Format("%s/share/getmyGroupNode1.asp?username=%s", AspServer, UserName);
	url.Format("%s/share/getmyGroupNode.asp?username=%s", AspServer, UserName);
	CString dstPath;
	dstPath.Format("%s\\temp\\node_%d.xml", CurrentPath, info->BookNumber);

	if(!DownLoad(url,dstPath,m_RLView->quit,m_RLView->socketList,m_RLView->m_mutex))
		return 1;
	if (pMainWnd->m_quit)
	{
		//m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	}
	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("LeafList/Leaf");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !m_RLView->quit; pos++){
				CString gid = GetXmlValue(pNodeList, "gid", pos);
				CString gname = GetXmlValue(pNodeList, "gname", pos);
				CString totalPeople = GetXmlValue(pNodeList, "totalpeople", pos);
				CString totalSource = GetXmlValue(pNodeList,"sourcenum",pos);
				CString tag = GetXmlValue(pNodeList,"tag",pos);

				CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);
				if(searchCount=="")
					gname = gname+"(0)";
				else
					gname = gname+"("+searchCount+")";//加入统计信息

				GROUPINFO* ginfo = (GROUPINFO*)calloc(1,sizeof(GROUPINFO));
				ginfo->gid = atol((LPCSTR)gid);
				strcpy_s(ginfo->groupname, 60, gname);
				ginfo->totalnum  = atol(totalPeople);
				HTREEITEM hCurItem = m_pRLTree->InsertItem(gname, 28, 28, hItem);
				m_RLView->SetTreeItemData(hCurItem, Is_NetGroup, NULL, 0, (DWORD)ginfo,tag);
				if (pMainWnd->m_quit)
				{
					return 0;
				}
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}

	m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}

void CGuidTreeView::AddLeaf(HTREEITEM hItem)//“我的读物部落”
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetGroupLeaf,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	threadList.AddTail(handle);
	ResumeThread(handle);
}

unsigned __stdcall CGuidTreeView::_GetGroupLeaf(LPVOID pParam)//“我的读物部落”
{
	HTREEITEM hItem = (HTREEITEM)pParam;

	TreeItemInfo* info = (TreeItemInfo*)m_pRLTree->GetItemData(hItem);
	if(!info)
		return 1;

	CString url;
	url.Format("%s/share/getGroupLeaf.asp?glid=%d", AspServer, info->BookNumber);
	CString dstPath;

	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();

	dstPath.Format("%s\\temp\\leaf_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	//不存在则下载
	//	if((fopen(dstPath,"r"))==NULL)			/*要打开的文件名*/ 
	//	{
	//		if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
	//			return 1;
	//	}

	if(!DownLoad(url,dstPath,m_RLView->quit,m_RLView->socketList,m_RLView->m_mutex))
	{// zhanganzhan
		// 		int tempMonth=month,tempDay=day;
		//         int tempM = 12,tempD=31;
		//         int flag =1;
		//         while(tempM--&&flag)
		//         {
		//             while(tempD--)
		//             {
		//                 dstPath.Format("%s\\temp\\leaf_%d_%d_%d.xml", CurrentPath, 
		//                            info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
		//                 FILE *fp =fopen(dstPath,"r");
		// 				if(fp!=NULL)
		//                 {
		//                     fclose(fp);
		//                     flag =0;
		//                     Tips("高校部落内部缓存",td_alert,5);
		//                     break;                    
		//                 }                
		//             }
		//             --tempMonth ;
		//             tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
		//         }
		//         if(tempM<=0)
		//         {
		//             return 0;
		//         }
		return 0;
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	int pos = 0;
	long searchCount = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("LeafList/Leaf");
		if(pNodeList){
			for(pos=0; pos < pNodeList->length && !m_RLView->quit; pos++){
				CString gid = GetXmlValue(pNodeList, "gid", pos);
				CString gname = GetXmlValue(pNodeList, "gname", pos);
				CString totalPeople = GetXmlValue(pNodeList, "totalpeople", pos);

				CString totalSource = GetXmlValue(pNodeList,"sourcenum",pos);
				searchCount += atol(totalSource);

				CString tag = GetXmlValue(pNodeList,"tag",pos);
				gname = gname+"("+totalSource+")";//加入统计信息

				GROUPINFO* ginfo = (GROUPINFO*)calloc(1,sizeof(GROUPINFO));
				ginfo->gid = atol((LPCSTR)gid);
				strcpy_s(ginfo->groupname, 60, gname);
				ginfo->totalnum  = atol(totalPeople);
				HTREEITEM hCurItem = m_pRLTree->InsertItem(gname, 28, 28, hItem);
				m_RLView->SetTreeItemData(hCurItem, Is_NetGroup, NULL, (WORD)atol(totalSource), (DWORD)ginfo,tag);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}

	CString ftitle;
	//	ftitle.Format("%s(%d)",pTree->GetItemText(hItem), pos);
	//	if(searchCount>0){
	//		ftitle.Format("%s(%d)",pTree->GetItemText(hItem), searchCount);
	//	}
	//	else
	ftitle.Format("%s(%d)",m_pRLTree->GetItemText(hItem), info->TotlePage);

	m_pRLTree->SetItemText(hItem, ftitle);
	m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	return 0;
}

void CGuidTreeView::InsertResourceGroup()
{
	hAllGroup = m_pTree->InsertItem("高教课程读物部落", 8, 8, NULL);
	SetTreeItemData(hAllGroup, Is_AllNetGroupList, NULL, 0, 0);
	GetSubGroup(hAllGroup);
}

void CGuidTreeView::InsertNetBook()
{
	hAllGroup = m_pTree->InsertItem("网络原创读物", 8, 8, NULL);
	SetTreeItemData(hAllGroup, Is_AllNetGroupList, NULL, 0, 0);
	GetSubNetBook(hAllGroup);
}

void CGuidTreeView::InsertFanshuBook()
{
	hAllGroup = m_pTree->InsertItem("番薯读物", 8, 8, NULL);
	SetTreeItemData(hAllGroup, Is_FanShu, NULL, 0, 0);
	GetSubFanshuBook(hAllGroup);
}
void CGuidTreeView::InsertPublishCom()//"读书联盟"
{
	hAllGroup = m_pTree->InsertItem("读书联盟", 8, 8, NULL);
	SetTreeItemData(hAllGroup, Is_AllNetGroupList, NULL, 0, 0);

	//zuoss 20090618 istation书目添加到左树
	CString url;
	url.Format("%s/bookstore/getBookStoreList.asp", AspServer);

	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();

	CString dstPath;
	dstPath.Format("%s\\temp\\bookStoreList_%d_%d.xml", CurrentPath,month,day);

	//当天的缓存不存在则下载，下载不成功则调以前的缓存。
	if(!PathFileExists(dstPath))
	{
		if(!DownLoad(url,dstPath,m_DCView->quit,m_DCView->socketList,m_DCView->m_mutex))
		{
			if (pMainWnd->m_quit)
			{
				return;
			}
			int tempMonth=month,tempDay=day;
			int tempM = 12,tempD=31;
			int flag =1;
			while(tempM--&&flag)
			{
				while(tempD--)
				{
					dstPath.Format("%s\\temp\\bookStoreList_%d_%d.xml", CurrentPath,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
					FILE *fp=fopen(dstPath,"r");
					if(fp!=NULL)
					{
						fclose(fp);
						flag =0;
						break;                    
					}                
				}
				--tempMonth ;
				tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
			}
			if(tempM<=0)
			{
				return;
			}
		}
		if (pMainWnd->m_quit)
		{
			return;
		}
	}

	//解析bookStore的XML文件
	int pos = 0;
	long searchSum = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("BookStoreList/Store");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		if(pNodeList)
		{
			for(pos=0; pos < pNodeList->length && !m_DCView->quit; pos++)
			{
				CString name = GetXmlValue(pNodeList, "name", pos);
				CString id = GetXmlValue(pNodeList, "id", pos);

				HTREEITEM hCurItem = m_pDCTree->InsertItem(name, 2, 2, hAllGroup);
				m_DCView->SetTreeItemData(hCurItem, Is_iStore, NULL, 0, atol(id),"","");
				m_DCView->GetSubPublishCom(hCurItem);
				if (pMainWnd->m_quit)
				{
					return;
				}
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			break;
		}
		return;
	}
	return;
	//end

	/*//zuoss 090506 istation原接口方式加入
	CHttpDownLoadDlg dlg;
	char TmpFileName[_MAX_PATH] = {0};
	sprintf(TmpFileName, "%s\\temp\\tmp.web", CurrentPath);

	char ChannelFileName[_MAX_PATH ] = {0};
	sprintf(ChannelFileName, "%s\\temp\\channel.bin", CurrentPath);

	if(!ChannelIsNew(ChannelFileName))
	{
	dlg.m_URL.Format("%s/RetailWeb/getstorechanneltype?regname=jinke2009&version=readings",JspServer);
	dlg.m_Path.Format("%s\\temp\\tmp.web", CurrentPath);
	if(dlg.DoModal() == IDOK)
	{
	SaveChannelInfo(TmpFileName, ChannelFileName);
	}
	}
	InsertChannelItem(hAllGroup,ChannelFileName);
	//end*/
}

LRESULT	CGuidTreeView::OnCreateNewFavorFolder(WPARAM w,LPARAM l)
{
	LPSTR name = (LPSTR)w;
	m_pTree->Expand(hMyFavor,TVE_COLLAPSE);
	HTREEITEM hItem = m_pTree->InsertItem(name, 4, 4, hMyFavor);
	SetTreeItemData(hItem, Is_MyFavourate_Folder, NULL, 2, 1);
	m_pTree->Expand(hMyFavor,TVE_EXPAND);
	free(name);
	return TRUE;
}

void CGuidTreeView::OnMenuCreateRegroup() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CCreateReGroup createGroup(/*info->BookNumber,*/NULL);
	createGroup.DoModal();
	if(createGroup.m_name.GetLength()<=0)
	{
		Tips("未添加群落名称，创建群落失败",td_alert,5);
		return;
	}
	if(createGroup.m_des.GetLength()<=0)
	{
		createGroup.m_des = "";
	}
	if (createGroup.m_keywords.GetLength()<=0)
	{
		createGroup.m_keywords = "";
	}
	//添加访问数据库函数，插入表s_group中
	//info->BookNumber为glid
	//createGroup.m_name为gname
	//createGroup.m_des为描述信息
	//UserName为 用户名
	char HttpAdd[1024] = {0};
	char Tadd[1024] = {0};
	try{
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format("%s/share/editgroup.asp?glid=%d&gname=%s&username=%s&desc=%s", AspServer,info->BookNumber,createGroup.m_name,UserName,createGroup.m_des);
		//		dlg.m_URL.Format("%s/share/editNetBooklevel.asp?glid=%d&username=%s&name=%s&desc=%s", AspServer,info->BookNumber,UserName,createGroup.m_name,createGroup.m_des);
		dlg.m_Path.Format("%s\\temp\\editgroup.txt", CurrentPath);
		if(dlg.DoModal()!=IDOK)
		{
			Tips("创建群落失败",td_alert,3);
			return;
		}
		else
		{
			//保存用户行为记录    add by Lins 20081225
			char behavior[1024] = {0};
			sprintf_s(behavior, 1024, "创建了\"%s\"群落",createGroup.m_name);
			char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
			pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);
			//刷新左侧树，插入新节点，将gid赋值给新节点	
			CFile cf;
			int length;
			int num;
			char * numbuf;
			if(cf.Open(dlg.m_Path,CFile::modeRead)){
				length = (int)cf.GetLength();
				if (length<=0)
					return;
				numbuf = (char*)malloc(length+1);
				memset(numbuf,0,length+1);
				cf.Read(numbuf,length);
				int i=0;
				while (i<length)
				{
					if (numbuf[i]==',')
						break;
					i++;
				}
				if (i>=length)
					return;
				numbuf[i] ='\n';
				num = atoi(numbuf);//存放group的id
				cf.Close();
			}
			//插入新节点
			HTREEITEM hCurItem = m_pTree->InsertItem(createGroup.m_name+"(0)", 28, 28, hit_Item);
			TreeItemInfo * info = (TreeItemInfo*)malloc(sizeof(TreeItemInfo));
			memset(info,0,sizeof(TreeItemInfo));
			info->Type = Is_NetGroup;
			GROUPINFO* ginfo = (GROUPINFO*)malloc(sizeof(GROUPINFO));
			memset(ginfo,0,sizeof(GROUPINFO));
			ginfo->gid = num;
			info->BookNumber = (DWORD)ginfo;
			m_pTree->SetItemData(hCurItem,(DWORD)info);
			//插入关键字
			char* kw_b64 = (char*)encode2((PUCHAR)(LPCSTR)createGroup.m_keywords,strlen(createGroup.m_keywords));
			//ginfo->gid 为 gid
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format("%s/share/editgrouptag.asp?gid=%d&tag=%s&username=%s&base64=1",AspServer,ginfo->gid,kw_b64,UserName);
			dlg.m_Path.Format("%s\\temp\\editgrouptag.txt",CurrentPath);

			if(dlg.DoModal()!=IDOK){
				Tips("删除操作失败，请重新尝试！",td_alert,2);
				return;
			}
			free(kw_b64);
			Tips("创建类别成功",td_msg,2);
		}
	}
	catch(_com_error& e){
		AfxMessageBox("操作失败，请重新尝试！");
	}
}

void CGuidTreeView::OnMenuCreateLevelgroup() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CCreateReGroup createGroup(NULL);
	createGroup.DoModal();
	if(createGroup.m_name.GetLength()<=0)
	{
		Tips("未添加分类层次名称，创建层次失败",td_alert,5);
		return;
	}
	if(createGroup.m_des.GetLength()<=0)
	{
		createGroup.m_des = "";
	}
	if (createGroup.m_keywords.GetLength()<=0)
	{
		createGroup.m_keywords = "";
	}
	//添加访问数据库语句，向grouplevel表中插入数据；并将glid为info->BookNumber的isleaf项置为0
	//createGroup.m_name为 name
	//createGroup.m_des为 描述信息
	//info->BookNumber为upid
	//1 为 isleaf的值
	try{
		CHttpDownLoadDlg dlg;
		//		dlg.m_URL.Format( "%s/share/editlevel1.asp?glid=%d&username=%s&name=%s&desc=%s", AspServer,info->BookNumber,UserName,createGroup.m_name,createGroup.m_des);
		dlg.m_URL.Format( "%s/share/editlevel.asp?glid=%d&username=%s&name=%s&desc=%s", AspServer,info->BookNumber,UserName,createGroup.m_name,createGroup.m_des);
		//dlg.m_URL.Format( "%s/share/editNetBooklevel.asp?glid=%d&username=%s&name=%s&desc=%s", AspServer,info->BookNumber,UserName,createGroup.m_name,createGroup.m_des);

		dlg.m_Path.Format("%s\\temp\\editlevel.txt", CurrentPath);
		if(dlg.DoModal()!=IDOK){
			Tips("创建群落失败",td_alert,3);
			return;
		}
		else{
			//刷新左侧树，插入新节点，将glid赋值给新节点	
			CFile cf;
			int length;
			int num;
			char * numbuf;
			if(cf.Open(dlg.m_Path,CFile::modeRead)){
				length = cf.GetLength();
				if (length<=0)
					return;
				numbuf = (char*)malloc(length+1);
				memset(numbuf,0,length+1);
				cf.Read(numbuf,length);
				int i=0;
				while (i<length)
				{
					if (numbuf[i]==',')
						break;
					i++;
				}
				if (i>=length)
					return;
				numbuf[i] ='\n';
				num = atoi(numbuf);//存放grouplevel的id
				cf.Close();
			}
			//插入树结点.怎样同时在我的读物群落中插入？？
			HTREEITEM hCurItem = m_pTree->InsertItem(createGroup.m_name+"(0)", 2, 2, hit_Item);
			TreeItemInfo * info = (TreeItemInfo*)malloc(sizeof(TreeItemInfo));
			memset(info,0,sizeof(TreeItemInfo));
			info->BookNumber = num;
			info->Type = Is_NetGroup_Level;
			m_pTree->SetItemData(hCurItem,(DWORD)info);
			//插入关键字
			CHttpDownLoadDlg dlg;
			char* kw_b64 = (char*)encode2((PUCHAR)(LPCSTR)createGroup.m_keywords,strlen(createGroup.m_keywords));
			dlg.m_URL.Format( "%s/share/editleveltag.asp?glid=%d&tag=%s&username=%s&base64=1", AspServer,info->BookNumber,kw_b64,UserName);
			//dlg.m_URL.Format( "%s/share/editNetBookTag.asp?glid=%d&tag=%s&username=%s&base64=1", AspServer,info->BookNumber,kw_b64,UserName);
			dlg.m_Path.Format("%s\\temp\\editleveltag.txt", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("创建列表标签失败，请重新尝试！",td_alert,2);
				free(kw_b64);
				return;
			}
			free(kw_b64);

			//保存用户行为记录    add by Lins 20081229
			char behavior[1024] = {0};
			//	treename.Left(treename.Find('('))
			sprintf_s(behavior, 1024, "创建了\"%s\"类别",createGroup.m_name);
			char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
			pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);

			Tips("创建类别成功",td_msg,2);
		}
	}
	catch(_com_error& e){
		AfxMessageBox("操作失败，请重新尝试！");
	}

}
//删除节点的相关操作
void CGuidTreeView::OnMenuDelete() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CString	treename = m_pTree->GetItemText(hit_Item);
	if (info->Type==Is_NetGroup_Level)
	{
		//将 info->BookNumber传给服务器端，在grouplevel表中删除该信息
		//刷新树
		try
		{
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format( "%s/share/deletelevel.asp?glid=%d", AspServer,info->BookNumber);
			dlg.m_Path.Format("%s\\temp\\deletelevel.txt", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("删除操作失败，请重新尝试！",td_alert,2);
				return;
			}
			FILE* fp;
			fp = fopen(dlg.m_Path,"rb");
			if(fp){
				int ch;
				ch = fgetc(fp);
				//	fread(&ch,sizeof(char),1,fp);
				if ((char)ch=='1')
				{
					m_pTree->DeleteItem(hit_Item);
					Tips("删除操作成功",td_msg,2);
				}
				else{
					Tips("该节点不可删除",td_alert,2);
				}
				fclose(fp);
				//	DeleteFile(dlg.m_Path);
			}
		}
		catch(_com_error& e){
			AfxMessageBox("操作失败，请重新尝试！");
		}
		//treename.Left(treename.Find('('))
		//保存用户行为记录    add by Lins 20081229
		char behavior[1024] = {0};
		sprintf_s(behavior, 1024, "删除了\"%s\"群落",treename.Left(treename.Find('(')));
		char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
		pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);
	}
	if(info->Type == Is_NetGroup){
		GROUPINFO* ginfo = (GROUPINFO*)info->BookNumber;
		//将 ginfo->gid 传给服务前，在s_group表中删除该信息
		try
		{
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format( "%s/share/deletegroup.asp?gid=%d", AspServer,ginfo->gid);
			dlg.m_Path.Format("%s\\temp\\deletegroup.txt", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("删除操作失败，请重新尝试！",td_alert,5);
				return;
			}

			FILE * fp;
			fp=fopen(dlg.m_Path,"rb");
			if (fp)
			{
				int ch = fgetc(fp);
				if ((char)ch=='1')
				{
					m_pTree->DeleteItem(hit_Item);
					Tips("删除成功",td_msg,2);
				}
				else{
					Tips("该节点不可删除",td_alert,2);
					fclose(fp);
					return;
				}
				fclose(fp);
			}
		}
		catch(_com_error& e){
			AfxMessageBox("操作失败，请重新尝试！");
		}
		//保存用户行为记录    add by Lins 20081229
		char behavior[1024] = {0};
		sprintf_s(behavior, 1024, "删除了\"%s\"群落",ginfo->groupname);
		char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
		pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);
	}
}

void CGuidTreeView::OnMenuKeywords() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	if (info->Type==Is_NetGroup_Level)
	{
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format( "%s/share/showleveltag.asp?glid=%d&username=%s", AspServer,info->BookNumber,UserName);
		LPSTR URL = (LPSTR)malloc(1024);
		ZeroMemory(URL,1024);
		strcpy_s(URL, 1024, dlg.m_URL);   
		pMainWnd->PostMessage(WM_OPEN_URL,(WPARAM)URL);
	}
	if (info->Type == Is_NetGroup)
	{
		GROUPINFO* ginfo = (GROUPINFO*)info->BookNumber;
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format("%s/share/editgrouptag.asp?gid=%d&username=%s",AspServer,ginfo->gid,UserName);
		LPSTR URL = (LPSTR)malloc(1024);
		ZeroMemory(URL,1024);
		strcpy_s(URL, 1024, dlg.m_URL);   
		pMainWnd->PostMessage(WM_OPEN_URL,(WPARAM)URL);
	}

	int flag,id;
	CString treename;
	if (info->Type==Is_NetGroup){
		flag = 0;
		GROUPINFO* ginfo = (GROUPINFO*)info->BookNumber;
		id = ginfo->gid;
	}
	else{
		flag = 1;
		id = info->BookNumber;
	}
	treename = m_pTree->GetItemText(hit_Item);
	CSetRGroupKeyWords KeyDialog(info->KeyWords,id,flag,treename.Left(treename.Find('(')));
	//	CSetRGroupKeyWords KeyDialog(info->KeyWords);
	KeyDialog.DoModal();

	CString keywords = KeyDialog.m_keywords;
	CString newtreename = KeyDialog.m_treename;

	if (newtreename.Compare(treename.Left(treename.Find('(')))!=0)
	{
		//更新树的名字
		UpdateTreeName(newtreename,flag,id);
		m_pTree->SetItemText(hit_Item,newtreename+treename.Right(treename.GetLength()-treename.Find('(')));
	}

	if (keywords.GetLength()<=0)
	{
		Tips("设定未成功");
		return ;
	}
	//		strcpy(info->KeyWords,keywords);//added by zuoss 20080813 更新主题词。

	//keywords 为 tag字段
	//UserName[50] 为 username
	//info->BookNumber 为glid
	if (info->Type==Is_NetGroup_Level)
	{
		CHttpDownLoadDlg dlg;
		char* kw_b64 = (char*)encode2((PUCHAR)(LPCSTR)keywords,strlen(keywords));
		dlg.m_URL.Format( "%s/share/editleveltag.asp?glid=%d&tag=%s&username=%s&base64=1&mode=1&roller=%s", AspServer,info->BookNumber,kw_b64,UserName,UserName);
		free(kw_b64);
		//			dlg.m_Path.Format("%s\\temp\\editleveltag.asp", CurrentPath);
		// 			if(dlg.DoModal()!=IDOK){
		// 				Tips("删除操作失败，请重新尝试！",td_alert,5);
		// 				return;
		// 			}
		//保存用户行为记录    add by Lins 20081229
		char behavior[1024] = {0};
		sprintf_s(behavior, 1024, "将\"%s\"群落的主题词更改为\"%s\"",newtreename,keywords);
		char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
		pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);


		LPSTR URL = (LPSTR)malloc(1024);
		ZeroMemory(URL,1024);
		strcpy_s(URL, 1024, dlg.m_URL);   
		pMainWnd->PostMessage(WM_OPEN_URL,(WPARAM)URL);

	}
	if (info->Type == Is_NetGroup)
	{
		char* kw_b64 = (char*)encode2((PUCHAR)(LPCSTR)keywords,strlen(keywords));
		GROUPINFO* ginfo = (GROUPINFO*)info->BookNumber;
		//ginfo->gid 为 gid
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format("%s/share/editgrouptag.asp?gid=%d&tag=%s&username=%s&base64=1&mode=0",AspServer,ginfo->gid,kw_b64,UserName);
		//	dlg.m_Path.Format("%s\\temp\\editgrouptag.asp",CurrentPath);
		//	if(dlg.DoModal()!=IDOK){
		//		Tips("删除操作失败，请重新尝试！",td_alert,5);
		//		return;
		//	}
		LPSTR URL = (LPSTR)malloc(1024);
		ZeroMemory(URL,1024);
		strcpy_s(URL, 1024, dlg.m_URL);   
		pMainWnd->PostMessage(WM_OPEN_URL,(WPARAM)URL);

		free(kw_b64);
	}
	Tips("添加主题词成功！",td_msg,5);
}

void CGuidTreeView::InsertMyResourceGroup()
{
	hMyGroup = m_pTree->InsertItem("我的读物群落", 1, 1, NULL);
	SetTreeItemData(hMyGroup, Is_MyNetGroup, NULL, 0, 0);
	GetMyReGroup(hMyGroup);
	//add by dongzhenhua 080627
	GetMyGrouplevel(hMyGroup);
	GetMyNetBook(hMyGroup);
}
//将用户喜好的群落转置“我的读物部落”
void CGuidTreeView::OnMenuTomyfavor() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CString hit_name = m_pTree->GetItemText(hit_Item);

	if(info->Type == Is_NetGroup)
	{
		GROUPINFO* ginfo = (GROUPINFO*)info->BookNumber;
		//根据	ginfo->gid 和 UserName 更改 服务器对应表，更新或插入
		try
		{
			CHttpDownLoadDlg dlg;
			//	dlg.m_URL.Format( "%s/share/insertfavorite1.asp?username=%s&gid=%d", AspServer,UserName,ginfo->gid);
			dlg.m_URL.Format( "%s/share/insertfavorite.asp?username=%s&gid=%d", AspServer,UserName,ginfo->gid);

			dlg.m_Path.Format("%s\\temp\\InsertU_G.txt", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("操作失败，请重新尝试！",td_alert,5);
				return;
			}
			//保存用户行为记录    add by Lins 20081229
			char behavior[1024] = {0};
			sprintf_s(behavior, 1024, "收藏\"%s\"群落", hit_name);
			char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
			pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);
		}
		catch(_com_error& e){
			AfxMessageBox("操作失败，请重新尝试！");
		}
		Tips("类别设定成功！",td_msg,2);
		HTREEITEM hCurItem = m_pTree->InsertItem(ginfo->groupname, 28, 28, hMyGroup);
		this->SetTreeItemData(hCurItem, Is_NetGroup, NULL, 0, (DWORD)ginfo);
	}
	if (info->Type == Is_NetGroup_Level)
	{
		try
		{
			CHttpDownLoadDlg dlg;
			//	    	dlg.m_URL.Format( "%s/share/insertfavoritelevel1.asp?username=%s&glid=%d", AspServer,UserName,info->BookNumber);
			dlg.m_URL.Format( "%s/share/insertfavoritelevel.asp?username=%s&glid=%d", AspServer,UserName,info->BookNumber);

			dlg.m_Path.Format("%s\\temp\\InsertU_L.txt", CurrentPath);
			if(dlg.DoModal()!=IDOK)
			{
				Tips("操作失败，请重新尝试！",td_alert,2);
				return;
			}
		}
		catch(_com_error& e){
			AfxMessageBox("操作失败，请重新尝试！");
		}
		Tips("类别设定成功！",td_msg,2);
		HTREEITEM hCurItem = m_pRLTree->InsertItem(hit_name.Left(hit_name.Find('(')), 2, 2, hMyGroup);
		//	pTreeView->SetTreeItemData(hCurItem, Is_NetGroup, NULL, 0, (DWORD)ginfo);
		CString num1 = hit_name.Right(hit_name.GetLength()-hit_name.Find('(')-1);
		num1 = num1.Left(num1.GetLength()-1);

		this->SetTreeItemData(hCurItem, Is_NetGroup_Level, NULL, atoi((LPCSTR)num1), info->BookNumber,info->KeyWords);
		GetSubGroup(hCurItem);
	}
}

void CGuidTreeView::OnMenuitemBackup() 
{
	pMainWnd->BackupSDCard();
}

void CGuidTreeView::OnMenuShowtag() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	if (info->Type==Is_NetGroup_Level)
	{
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format( "%s/share/showleveltag.asp?glid=%d&username=%s", AspServer,info->BookNumber,UserName);
		LPSTR URL = (LPSTR)malloc(1024);
		ZeroMemory(URL,1024);
		strcpy_s(URL, 1024, dlg.m_URL);   
		pMainWnd->PostMessage(WM_OPEN_URL,(WPARAM)URL);
	}
	if (info->Type == Is_NetGroup)
	{
		GROUPINFO* ginfo = (GROUPINFO*)info->BookNumber;
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format("%s/share/editgrouptag.asp?gid=%d&username=%s",AspServer,ginfo->gid,UserName);
		LPSTR URL = (LPSTR)malloc(1024);
		ZeroMemory(URL,1024);
		strcpy_s(URL, 1024, dlg.m_URL);   
		pMainWnd->PostMessage(WM_OPEN_URL,(WPARAM)URL);
	}
}

void CGuidTreeView::GetMyGrouplevel(HTREEITEM hItem)//"我的读物群落"
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetMyGroupLevel,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if(handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}

void CGuidTreeView::GetMyNetBook(HTREEITEM hItem)//"我的读物群落"
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetMyNetBook,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if(handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}
void CGuidTreeView::GetMySubNetBook(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetMySubNetBook,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	if(handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}
/*
unsigned __stdcall CGuidTreeView::_GetMyGroupLevel(LPVOID pParam)
{

HTREEITEM hItem = (HTREEITEM)pParam;
TreeItemInfo* info = (TreeItemInfo*)pTree->GetItemData(hItem);
if(!info)
return 1;
CString url;
url.Format("%s/share/getfavoratelevel.asp?username=%s", AspServer, UserName);
CString dstPath;
//       dstPath.Format("%s\\temp\\node_%d.xml", CurrentPath, info->BookNumber);

//以天为单位进行缓存的更新
CTime ct = CTime::GetCurrentTime();
int day = ct.GetDay();
int month = ct.GetMonth();
dstPath.Format("%s\\temp\\mynode_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
//不存在则下载

//       if((fopen(dstPath,"r"))==NULL)                   // 要打开的文件名

//       {

//                 if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))

//                          return 1;

//       }
if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
{//zhanganzhan
Tips("进入我的群落内部缓存",td_alert,5);
int tempMonth=month,tempDay=day+1; //从今天开始搜索以前的缓存       
int flag =1;//控制搜索次数的标识         
while(flag)
{
while(true)
{
dstPath.Format("%s\\temp\\mynode_%d_%d_%d.xml", CurrentPath, 
info->BookNumber,tempMonth,(--tempDay>0?tempDay:(tempDay+31)));
FILE *fp=fopen(dstPath,"r");//确认该缓存文件是否存在
flag++;
if(fp!=NULL)//找到则退出查找，继续文件的解析
{
fclose(fp);
flag =0;                    
break;                    
}  

if(flag>60)//最近三天内没有新的缓存，则舍弃该节点
{
Tips("高校部落内部缓存超过60！",td_alert,5);
return 1;
}              
}            
--tempMonth ;//说明本月查找不成功，从上月开始查找
tempMonth=(tempMonth>0?tempMonth:(tempMonth+12));            
}        
}
int pos = 0;

IXMLDOMDocumentPtr pDoc;

IXMLDOMNodeListPtr pNodeList;

try{

HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));

if(!SUCCEEDED(hr))

throw 1;    //xml 解析失败

if(!pDoc->load((LPCSTR)dstPath))

throw 2;    //xml 解析失败

pNodeList = pDoc->selectNodes("NodeList/Node");

if(pNodeList){

for(pos=0; pos<pNodeList->length && !pTreeView->quit; pos++){

CString glid = GetXmlValue(pNodeList, "glid", pos);

CString name = GetXmlValue(pNodeList, "name", pos);

CString leaf = GetXmlValue(pNodeList, "leaf", pos);

CString tag        = GetXmlValue(pNodeList, "tag", pos);

CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);

HTREEITEM hCurItem = pTree->InsertItem(name, 2, 2, hItem);

pTreeView->SetTreeItemData(hCurItem, Is_NetGroup_Level, NULL,  atol(searchCount), atol(glid),tag);

if(atoi((LPCSTR)leaf))

pTreeView->AddLeaf(hCurItem);

else

pTreeView->GetSubGroup(hCurItem);

}

pNodeList.Release();
}
pDoc.Release();

}

catch(_com_error& e){}

pTreeView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());



CString ftitle;

ftitle.Format("%s(%d)",pTree->GetItemText(hItem), info->TotlePage);

pTree->SetItemText(hItem, ftitle);

return 0;

}*/

unsigned __stdcall CGuidTreeView::_GetMyGroupLevel(LPVOID pParam)//"我的读物群落"
{
	//	Sleep(1000);
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pRLTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	//	url.Format("%s/share/getfavoratelevel1.asp?username=%s", AspServer, UserName);
	url.Format("%s/share/getfavoratelevel.asp?username=%s", AspServer, UserName);

	CString dstPath;
	//	dstPath.Format("%s\\temp\\node_%d.xml", CurrentPath, info->BookNumber);
	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();
	dstPath.Format("%s\\temp\\mynode_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	//不存在则下载
	//	if((fopen(dstPath,"r"))==NULL)			//要打开的文件名
	//	{
	//		if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
	//			return 1;
	//	}
	if(!DownLoad(url,dstPath,m_RLView->quit,m_RLView->socketList,m_RLView->m_mutex))
		return 1;
	if (pMainWnd->m_quit)
	{
		//m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	}

	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !m_RLView->quit; pos++){
				CString glid = GetXmlValue(pNodeList, "glid", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				CString leaf = GetXmlValue(pNodeList, "leaf", pos);
				CString tag	 = GetXmlValue(pNodeList, "tag", pos);
				CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);
				HTREEITEM hCurItem = m_pRLTree->InsertItem(name, 2, 2, hItem);
				m_RLView->SetTreeItemData(hCurItem, Is_NetGroup_Level, NULL,  atol(searchCount), atol(glid),tag);
				if(atoi((LPCSTR)leaf))
					m_RLView->AddLeaf(hCurItem);
				else
					m_RLView->GetMySub(hCurItem);
				if (pMainWnd->m_quit)
				{
					//m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
					return 0;
				}
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}
	m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	CString ftitle;
	ftitle.Format("%s(%d)",m_pRLTree->GetItemText(hItem), info->TotlePage);
	m_pRLTree->SetItemText(hItem, ftitle);
	return 0;
}

unsigned __stdcall CGuidTreeView::_GetMyNetBook(LPVOID pParam)//"我的读物群落"
{
	//	Sleep(1000);
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pRLTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/share/getFavorateNetBook.asp?username=%s", AspServer, UserName);

	CString dstPath;
	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();
	dstPath.Format("%s\\temp\\myFavorateNetBook_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	//不存在则下载
	//	if((fopen(dstPath,"r"))==NULL)			//要打开的文件名
	//	{
	//		if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
	//			return 1;
	//	}
	if(!DownLoad(url,dstPath,m_RLView->quit,m_RLView->socketList,m_RLView->m_mutex))
		return 1;

	if (pMainWnd->m_quit)
	{
		return 0;
	}
	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !m_RLView->quit; pos++)
			{
				CString glid = GetXmlValue(pNodeList, "glid", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				CString url = GetXmlValue(pNodeList,"url",pos);
				CString leaf = GetXmlValue(pNodeList, "leaf", pos);
				CString tag	 = GetXmlValue(pNodeList, "tag", pos);
				CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);

				HTREEITEM hCurItem = m_pRLTree->InsertItem(name, 2, 2, hItem);
				m_RLView->SetTreeItemData(hCurItem, Is_MyResource, NULL, atol(searchCount), atol(glid),tag,url);
				if(atoi((LPCSTR)leaf))
				{
					CString ftitle;
					if(name.Compare("爱，从未离开") ==0)
					{
						ftitle.Format("%s(%d完结)",m_pRLTree->GetItemText(hCurItem), atoi(searchCount));	
					}
					else
					{
						ftitle.Format("%s(%d)",m_pRLTree->GetItemText(hCurItem), atoi(searchCount));
					}
					m_pRLTree->SetItemText(hCurItem, ftitle);
					m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
				}
				else
					m_RLView->GetMySubNetBook(hCurItem);
				if (pMainWnd->m_quit)
				{
					return 0;
				}
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}
	m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	//	CString ftitle;
	//	ftitle.Format("%s(%d)",pTree->GetItemText(hItem), info->TotlePage);
	//	pTree->SetItemText(hItem, ftitle);
	return 0;
}

unsigned __stdcall CGuidTreeView::_GetMySubNetBook(LPVOID pParam)////"我的读物群落"
{
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)m_pRLTree->GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/share/getNetBookNode.asp?fid=%d", AspServer, info->BookNumber);

	CString dstPath;
	//以天为单位进行缓存的更新
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();
	dstPath.Format("%s\\temp\\mySubFavorateNetBook_%d_%d_%d.xml", CurrentPath, info->BookNumber,month,day);
	//不存在则下载
	//	if((fopen(dstPath,"r"))==NULL)			//要打开的文件名
	//	{
	//		if(!DownLoad(url,dstPath,pTreeView->quit,pTreeView->socketList,pTreeView->m_mutex))
	//			return 1;
	//	}
	if(!DownLoad(url,dstPath,m_RLView->quit,m_RLView->socketList,m_RLView->m_mutex))
		return 1;
	if (pMainWnd->m_quit)
	{
		return 0;
	}

	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !m_RLView->quit; pos++)
			{
				CString glid = GetXmlValue(pNodeList, "glid", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				CString url = GetXmlValue(pNodeList,"url",pos);
				CString leaf = GetXmlValue(pNodeList, "leaf", pos);
				CString tag	 = GetXmlValue(pNodeList, "tag", pos);
				CString searchCount = GetXmlValue(pNodeList,"searchCount",pos);

				HTREEITEM hCurItem = m_pRLTree->InsertItem(name, 2, 2, hItem);
				m_RLView->SetTreeItemData(hCurItem, Is_MyResource, NULL, atol(searchCount), atol(glid),tag,url);
				if(atoi((LPCSTR)leaf))
				{
					CString ftitle;
					if(name.Compare("爱，从未离开") ==0)
					{
						ftitle.Format("%s(%d完结)",m_pRLTree->GetItemText(hCurItem), atoi(searchCount));	
					}
					else
					{
						ftitle.Format("%s(%d)",m_pRLTree->GetItemText(hCurItem), atoi(searchCount));
					}
					m_pRLTree->SetItemText(hCurItem, ftitle);
					m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
				}
				else
					m_RLView->GetMySubNetBook(hCurItem);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}
	m_RLView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	//	CString ftitle;
	//	ftitle.Format("%s(%d)",pTree->GetItemText(hItem), info->TotlePage);
	//	pTree->SetItemText(hItem, ftitle);
	return 0;
}

void CGuidTreeView::UpdateSearchCount(int flag, int id, int total)
{
	char WebPath[1024] = {0};
	char Tadd[_MAX_PATH] = {0};

	sprintf_s(Tadd, _MAX_PATH,  "%s\\temp\\updatesearchcount.tmp", CurrentPath);
	if(flag==Is_NetGroup_Level){
		sprintf_s(WebPath, 1024, "%s/share/updatesearchcount.asp?flag=1&id=%d&total=%d",AspServer,id, total);
		try{
			if(!DownLoad(WebPath,Tadd, this->quit, this->socketList))
			{
				return;
			}//, pTreeView->socketlistmutex);
		}
		catch(_com_error& e){
			return ;
		}
		if (pMainWnd->m_quit)
		{
			return;
		}
	}
	if (flag == Is_NetGroup){
		sprintf_s(WebPath, 1024, "%s/share/updatesearchcount.asp?flag=2&id=%d&total=%d",AspServer,id, total);
		try{
			if(!DownLoad(WebPath,Tadd, this->quit, this->socketList))
			{
				return;
			}//, pView->socketlistmutex);
		}
		catch(_com_error& e){
			return ;
		}
	}

	return;
}
/*
LRESULT CGuidTreeView::OnUpdateSearchCount(int flag, int id, int total)
{
char WebPath[1024] = {0};
char Tadd[_MAX_PATH] = {0};

sprintf(Tadd, "%s\\temp\\updatesearchcount.tmp", CurrentPath);
if(flag==Is_NetGroup_Level){
sprintf(WebPath, "%s/share/updatesearchcount.asp?flag=1&id=%d&total=%d",AspServer,id, total);
try{
DownLoad(WebPath,Tadd, pTreeView->quit, pTreeView->socketList);//, pTreeView->socketlistmutex);
}
catch(_com_error& e){
return 0 ;
}
}
if (flag == Is_NetGroup){
sprintf(WebPath, "%s/share/updatesearchcount.asp?flag=2&id=%d&total=%d",AspServer,id, total);
try{
DownLoad(WebPath,Tadd, pTreeView->quit, pTreeView->socketList);//, pView->socketlistmutex);
}
catch(_com_error& e){
return  0;
}
}
return 1;
}
*/

int CGuidTreeView::GetFileNum(CString Path)
{
	int filecount = 0;
	CFileFind find;
	Path += "\\*";
	BOOL ext = find.FindFile(Path);
	while(ext){
		ext = find.FindNextFile();
		if(find.IsDots())
			continue;
		if(find.IsDirectory())
			continue;
		if(unknownFileType == GetFileTypeFromExt(GetExtNameFromFull(find.GetFileName())))
			continue;
		filecount++;
	}
	return filecount;
}

void CGuidTreeView::UpdateTreeName(CString newname, int flag, int id)
{
	CHttpDownLoadDlg dlg;
	dlg.m_URL.Format("%s/share/updatecoursename.asp?id=%d&flag=%d&newname=%s", AspServer,id,flag,newname);
	dlg.m_Path.Format("%s\\temp\\updatecoursename.txt", CurrentPath);
	if(dlg.DoModal()!=IDOK)
	{
		Tips("名称更改未成功！",td_alert,5);
	}	
	return;
}

void CGuidTreeView::DeleXmlFiles()//未完成未应用20081028
{
	//首先应获得路径。这里默认设为C:\Program Files\Rao Team\Raorao

	CFileFind finder;
	CString path = "C:\\Program Files\\Rao Team\\Raorao\\temp";
	CString strWildcard = path + _T("\\*.xml");	

	ofstream output;
	output.close();
	output.clear();
	output.open("AllFilenames.txt");

	//将所有.xml文件的文件名写入记事本。

	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		CTime creation_time;

		if (finder.IsDots())
			continue;
		CString sFileName = finder.GetFileName();

		output << (LPCTSTR) finder.GetFileName() << endl;
		output << (LPCTSTR) finder.GetCreationTime(creation_time) << endl;
		output << (LPCTSTR) finder.GetLastAccessTime(creation_time) << endl;
		output << (LPCTSTR) finder.GetLastWriteTime(creation_time) << endl;

		//		CTime Now = CTime::GetCurrentTime();
		//		CTime ct;	
		//		CTimeSpan timespan;
		//		if(finder.GetLastWriteTime(ct))
		//		{
		//			timespan = ct - Now;
		//		}
		//        AfxMessageBox(CString(timespan.GetDays()));
		//        if (timespan.GetDays() > 10)
		//        {
		//			//DeleteFile(strWildcard +"\\" +sFileName)  
		//		output << (LPCTSTR)sFileName << endl;//输出查找文件夹下的所有文件名
		//		}
	}
	finder.Close();	
}

void CGuidTreeView::OnMenuAddnetbook() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CAddNetBookDlg addNetBook(NULL);
	addNetBook.DoModal();
	if(addNetBook.m_netBookName.GetLength()<=0)
	{
		Tips("网络读物名称不能为空，添加失败！",td_alert,2);
		return;
	}
	if(addNetBook.m_netBookURL.GetLength()<=0)
	{
		Tips("网络读物URL地址不能为空，添加失败！",td_alert,2);
		return;
	}
	try{
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format("%s/share/editNetBooklevel.asp?glid=%d&username=%s&name=%s&desc=%s&flag=0", AspServer,info->BookNumber,UserName,addNetBook.m_netBookName,addNetBook.m_netBookURL);
		dlg.m_Path.Format("%s\\temp\\addnetbook.txt", CurrentPath);
		if(dlg.DoModal()!=IDOK)
		{
			Tips("创建网络读物失败！",td_alert,2);
			return;
		}
		else
		{
			//刷新左侧树，插入新节点，将glid赋值给新节点	
			CFile cf;
			int length;
			int num;
			char * numbuf;
			if(cf.Open(dlg.m_Path,CFile::modeRead)){
				length = cf.GetLength();
				if (length<=0)
					return;
				numbuf = (char*)malloc(length+1);
				memset(numbuf,0,length+1);
				cf.Read(numbuf,length);
				int i=0;
				while (i<length)
				{
					if (numbuf[i]==',')
						break;
					i++;
				}
				if (i>=length)
					return;
				numbuf[i] ='\n';
				num = atoi(numbuf);//存放grouplevel的id
				cf.Close();
			}
			//插入树结点
			HTREEITEM hCurItem = m_pTree->InsertItem(addNetBook.m_netBookName+"(0)", 2, 2, hit_Item);
			m_DCView->SetTreeItemData(hCurItem, Is_NetBook, NULL, 0, num,addNetBook.m_netBookName,addNetBook.m_netBookURL);

			//保存用户行为记录    add by Lins 20081229
			char behavior[1024] = {0};
			sprintf_s(behavior, 1024, "创建了\"%s\"类别",addNetBook.m_netBookName);
			char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
			pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);		
			Tips("创建类别成功",td_msg,2);
		}
	}
	catch(_com_error& e){
		AfxMessageBox("操作失败，请重新尝试！");
	}
}

void CGuidTreeView::OnMenuSetnetbook() 
{
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CString name;
	name.Format("%s",m_pTree->GetItemText(hit_Item));
	name.Format("%s",name.Left(name.Find('(')));

	CString url;
	url.Format("%s",info->url);

	CAddNetBookDlg changeNetBook(name,url);
	changeNetBook.DoModal();

	if(changeNetBook.m_netBookName.GetLength()<=0)
	{
		Tips("网络读物名称不能为空，修改失败！",td_alert,2);
		return;
	}
	if(changeNetBook.m_netBookURL.GetLength()<=0)
	{
		Tips("网络读物URL地址不能为空，修改失败！",td_alert,2);
		return;
	}
	try
	{
		CHttpDownLoadDlg dlg;
		//flag=1:修改。flag=0:添加。
		dlg.m_URL.Format("%s/share/editNetBooklevel.asp?glid=%d&username=%s&name=%s&desc=%s&flag=1", AspServer,info->BookNumber,UserName,changeNetBook.m_netBookName,changeNetBook.m_netBookURL);
		dlg.m_Path.Format("%s\\temp\\addnetbook.txt", CurrentPath);
		if(dlg.DoModal()!=IDOK)
		{
			Tips("修改网络读物信息失败！",td_alert,2);
			return;
		}
		else
		{
			//刷新左侧树，插入新节点，将glid赋值给新节点	
			CFile cf;
			int length;
			int num;
			char * numbuf;
			if(cf.Open(dlg.m_Path,CFile::modeRead)){
				length = cf.GetLength();
				if (length<=0)
					return;
				numbuf = (char*)malloc(length+1);
				memset(numbuf,0,length+1);
				cf.Read(numbuf,length);
				int i=0;
				while (i<length)
				{
					if (numbuf[i]==',')
						break;
					i++;
				}
				if (i>=length)
					return;
				numbuf[i] ='\n';
				num = atoi(numbuf);//存放grouplevel的id
				cf.Close();
			}
			//修改树节点数据
			m_DCView->SetTreeItemData(hit_Item, Is_NetBook, NULL, 0, num,changeNetBook.m_netBookName,changeNetBook.m_netBookURL);

			//保存用户行为记录    add by Lins 20081229
			char behavior[1024] = {0};
			sprintf_s(behavior, 1024, "修改了\"%s\"类别",changeNetBook.m_netBookName);
			char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
			pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);

			Tips("修改类别类别成功",td_msg,5);


		}
	}
	catch(_com_error& e){
		AfxMessageBox("操作失败，请重新尝试！");
	}
}

void CGuidTreeView::OnMenuDelnetbook() 
{
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CString	treename = m_pTree->GetItemText(hit_Item);
	if (info->Type==Is_NetBook)
	{
		//将 info->BookNumber传给服务器端，在grouplevel表中删除该信息
		//刷新树
		try
		{
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format( "%s/share/deleteNetBook.asp?glid=%d", AspServer,info->BookNumber);
			dlg.m_Path.Format("%s\\temp\\deleteNetBook.txt", CurrentPath);
			if(dlg.DoModal()!=IDOK)
			{
				Tips("删除操作失败，请重新尝试！",td_alert,2);
				return;
			}
			FILE* fp;
			fp = fopen(dlg.m_Path,"rb");
			if(fp)
			{
				int ch;
				ch = fgetc(fp);
				if ((char)ch=='1')
				{
					m_pTree->DeleteItem(hit_Item);

					//保存用户行为记录    add by Lins 20081229
					char behavior[1024] = {0};
					sprintf_s(behavior, 1024, "删除了\"%s\"网络读物",treename.Left(treename.Find('(')));
					char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
					pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);

					Tips("删除操作成功",td_msg,2);
				}
				else
				{
					Tips("该节点不可删除",td_alert,2);
				}
				fclose(fp);
			}
		}
		catch(_com_error& e)
		{
			AfxMessageBox("操作失败，请重新尝试！");
		}
	}
}
//zuoss 20090320 设置树节点显示不同颜色
void CGuidTreeView::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.color = (COLORREF)-1;
	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

void CGuidTreeView::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	m_pTree->SetItemState(hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD);
}

void CGuidTreeView::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.logfont.lfFaceName[0] = '\0';
	cf.color = color;
	m_mapColorFont[hItem] = cf;
}

BOOL CGuidTreeView::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return FALSE;
	if( cf.logfont.lfFaceName[0] == '\0' ) 
		return FALSE;
	*plogfont = cf.logfont;
	return TRUE;

}

BOOL CGuidTreeView::GetItemBold(HTREEITEM hItem)
{
	return m_pTree->GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
}

COLORREF CGuidTreeView::GetItemColor(HTREEITEM hItem)
{
	// Returns (COLORREF)-1 if color was not set
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return (COLORREF)-1;
	return cf.color;

}
//end

void CGuidTreeView::OnMenuMoveup() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CString hit_name = m_pTree->GetItemText(hit_Item);

	if (info->Type == Is_NetBook)
	{
		try
		{
			CHttpDownLoadDlg dlg;
			//	    	dlg.m_URL.Format( "%s/share/insertfavoritelevel1.asp?username=%s&glid=%d", AspServer,UserName,info->BookNumber);
			dlg.m_URL.Format( "%s/share/insertFavoriteNetBook.asp?username=%s&glid=%d", AspServer,UserName,info->BookNumber);

			dlg.m_Path.Format("%s\\temp\\InsertNetBook.txt", CurrentPath);
			if(dlg.DoModal()!=IDOK)
			{
				Tips("操作失败，请重新尝试！",td_alert,2);
				return;
			}
		}
		catch(_com_error& e){
			AfxMessageBox("操作失败，请重新尝试！");
		}
		Tips("类别设定成功！",td_msg,2);
		HTREEITEM hCurItem = m_pDCTree->InsertItem(hit_name.Left(hit_name.Find('(')), 2, 2, hMyGroup);
		CString num1 = hit_name.Right(hit_name.GetLength()-hit_name.Find('(')-1);
		num1 = num1.Left(num1.GetLength()-1);

		//		pTreeView->SetTreeItemData(hCurItem, Is_NetBook, NULL, atoi((LPCSTR)num1), info->BookNumber,info->KeyWords);
		//		GetSubNetBook(hCurItem);
		m_DCView->SetTreeItemData(hCurItem, Is_MyResource, NULL, (WORD)info->TotlePage, info->BookNumber,info->KeyWords,info->url);
		GetSubNetBook(hCurItem);
	}
}

void CGuidTreeView::OnMenuitemRemove() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	CString	treename = m_pTree->GetItemText(hit_Item);
	if (info->Type==Is_MyResource)
	{
		//将 info->BookNumber传给服务器端，在myfavoritelevel表中删除该信息
		//刷新树
		try
		{
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format( "%s/share/deleteMyResource.asp?glid=%d", AspServer,info->BookNumber);
			dlg.m_Path.Format("%s\\temp\\deleteMyResource.txt", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("移除操作失败，请重新尝试！",td_alert,2);
				return;
			}
			FILE* fp;
			fp = fopen(dlg.m_Path,"rb");
			if(fp){
				int ch;
				ch = fgetc(fp);
				if ((char)ch=='1')
				{
					m_pTree->DeleteItem(hit_Item);
					Tips("移除操作成功",td_msg,2);
				}
				else
				{
					Tips("该节点不可移除,请移除添加时的根节点！",td_alert,2);
				}
				fclose(fp);
			}
		}
		catch(_com_error& e){
			AfxMessageBox("操作失败，请重新尝试！");
		}
	}

}

void CGuidTreeView::OnMenuMakebook() 
{
	// TODO: Add your command handler code here
	/*	CString userPermission;
	userPermission.Format("%s\\temp\\userPermission.txt", CurrentPath);

	FILE* fp;
	fp = fopen(userPermission,"rb");
	char type;
	if(fp)
	{
	type = fgetc(fp);
	type = fgetc(fp);
	fclose(fp);
	}
	if (type == 'x')
	{
	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	::ShellExecute(NULL, "open", "UrlToWol.exe", info->url, CurrentPath, SW_SHOW);
	}*/

	TreeItemInfo* info = (TreeItemInfo*)m_pTree->GetItemData(hit_Item);
	::ShellExecute(NULL, "open", "UrlToWol.exe", info->url, CurrentPath, SW_SHOW);
}

void CGuidTreeView::InsertHotKey()
{
	HTREEITEM hkey1 = m_pTree->InsertItem("最新", 5, 5, NULL);
	SetTreeItemData(hkey1, Is_HotKey, NULL);
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetHotKey1,(LPVOID)hkey1,CREATE_SUSPENDED,NULL);
	if(handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
	HTREEITEM hkey2 = m_pTree->InsertItem("最热", 5, 5, NULL);
	SetTreeItemData(hkey2, Is_HotKey, NULL);
	handle = (HANDLE)_beginthreadex(NULL,0,_GetHotKey2,(LPVOID)hkey2,CREATE_SUSPENDED,NULL);
	if(handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}

	HTREEITEM hkey3 = m_pTree->InsertItem("推荐", 5, 5, NULL);
	SetTreeItemData(hkey3, Is_HotKey, NULL);
	handle = (HANDLE)_beginthreadex(NULL,0,_GetHotKey3,(LPVOID)hkey3,CREATE_SUSPENDED,NULL);
	if(handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}

unsigned __stdcall CGuidTreeView::_GetHotKey1(LPVOID pParam)//"最新热词"
{
	//Sleep(1000);
	HTREEITEM hItem = (HTREEITEM)pParam;

	TreeItemInfo* info = (TreeItemInfo*)m_pHKTree->GetItemData(hItem);
	if(!info){
		m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 1;
	}

	try{
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		try{
			char WebPath[1024] = {0};
			char TmpPath[_MAX_PATH] = {0};

			sprintf_s(WebPath, 1024, "%s\\search\\hotsearch.asp", AspServer);
			sprintf_s(TmpPath, _MAX_PATH, "%s\\temp\\hotSearch.xml", CurrentPath);

			if(!DownLoad(WebPath,TmpPath,m_HKView->quit,m_HKView->socketList,m_HKView->m_mutex)|| (!offline))
			{

			}
			if (pMainWnd->m_quit)
			{
				//m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
				return 0;
			}
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//对象创建失败
			if(!pDoc->load(TmpPath))
				throw 2;	//xml 加载失败
			pNodeList = pDoc->selectNodes("HotKeyword");
			if(pNodeList == NULL)
				throw 3;	//xml 格式错误

			pNodeList = pDoc->selectNodes("HotKeyword/KeyInfo");
			if(pNodeList){
				int count = 0;
				for(int pos=0; pos<pNodeList->length && count < 20; pos++,count ++){
					CString Key = GetXmlValue(pNodeList, "Key", pos);
					CString Time = GetXmlValue(pNodeList, "Time", pos);
					CString Result = GetXmlValue(pNodeList, "Result", pos);
					if(!Key.IsEmpty() && !Time.IsEmpty() && !Result.IsEmpty()){
						CString str;
						str.Format("%s(%s)", Key, Result);
						HTREEITEM hCurItem = m_pHKTree->InsertItem(str, 28, 28, hItem);
						m_HKView->SetTreeItemData(hCurItem, Is_HotKeyleaf, NULL, 0, 0,Key);
					}
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(_com_error& e){
#ifdef _DEBUG
			//Tips(e.Description());
#endif
		}
		catch(int errnum){
			switch(errnum){
			case 3:		//已更新
				pNodeList.Release();
			case 2:		//格式错误
				pDoc.Release();
				break;
			}
		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CHotKeyListView::GetHotKeyWord()");
#endif
	}	
	m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	if (m_HKView->IsWindowVisible())
	{
		m_HKView->ShowWindow(SW_HIDE);
		m_HKView->ShowWindow(SW_SHOW);
	}
	return 0;
}

unsigned __stdcall CGuidTreeView::_GetHotKey2(LPVOID pParam)//"最热热词"
{
	//Sleep(1000);
	HTREEITEM hItem = (HTREEITEM)pParam;

	TreeItemInfo* info = (TreeItemInfo*)m_pHKTree->GetItemData(hItem);
	if(!info){
		m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 1;
	}

	try{
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		try{
			char WebPath[1024] = {0};
			char TmpPath[_MAX_PATH] = {0};

			sprintf_s(WebPath, 1024, "%s\\search\\mosthotkey.asp", AspServer); 
			sprintf_s(TmpPath, _MAX_PATH, "%s\\temp\\mosthotSearch.xml", CurrentPath);

			if(!DownLoad(WebPath,TmpPath,m_HKView->quit,m_HKView->socketList,m_HKView->m_mutex)|| (!offline))
			{

			}
			if (pMainWnd->m_quit)
			{
				//m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
				return 0;
			}
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//对象创建失败
			if(!pDoc->load(TmpPath))
				throw 2;	//xml 加载失败
			pNodeList = pDoc->selectNodes("HotKeyword");
			if(pNodeList == NULL)
				throw 3;	//xml 格式错误

			pNodeList = pDoc->selectNodes("HotKeyword/KeyInfo");
			if(pNodeList){
				int count = 0;
				for(int pos=0; pos<pNodeList->length && count < 20; pos++,count ++){
					CString Key = GetXmlValue(pNodeList, "Key", pos);
					CString Time = GetXmlValue(pNodeList, "Time", pos);
					CString Result = GetXmlValue(pNodeList, "Result", pos);
					if(!Key.IsEmpty() && !Time.IsEmpty() && !Result.IsEmpty()){
						CString str;
						str.Format("%s(共搜索%s)", Key, Time, Result);
						HTREEITEM hCurItem = m_pHKTree->InsertItem(str, 28, 28, hItem);
						m_HKView->SetTreeItemData(hCurItem, Is_HotKeyleaf, NULL, 0, 0,Key);
					}
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(_com_error& e){
#ifdef _DEBUG
			//Tips(e.Description());
#endif
		}
		catch(int errnum){
			switch(errnum){
			case 3:		//已更新
				pNodeList.Release();
			case 2:		//格式错误
				pDoc.Release();
				break;
			}
		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CHotKeyListView::GetHotKeyWord()");
#endif
	}
	if (m_HKView->IsWindowVisible())
	{
		m_HKView->ShowWindow(SW_HIDE);
		m_HKView->ShowWindow(SW_SHOW);
	}
	m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}

unsigned __stdcall CGuidTreeView::_GetHotKey3(LPVOID pParam)//"推荐热词"
{
	//Sleep(1000);
	HTREEITEM hItem = (HTREEITEM)pParam;

	TreeItemInfo* info = (TreeItemInfo*)m_pHKTree->GetItemData(hItem);
	if(!info){
		m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 1;
	}

	try{
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		try{
			char WebPath[1024] = {0};
			char TmpPath[_MAX_PATH] = {0};

			sprintf_s(WebPath, 1024, "%s\\search\\recommendhotkey.asp", AspServer);
			sprintf_s(TmpPath, _MAX_PATH, "%s\\temp\\recommendhotSearch.xml", CurrentPath); 

			if(!DownLoad(WebPath,TmpPath,m_HKView->quit,m_HKView->socketList,m_HKView->m_mutex)|| (!offline))
			{

			}
			if (pMainWnd->m_quit)
			{
				//m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
				return 0;
			}
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//对象创建失败
			if(!pDoc->load(TmpPath))
				throw 2;	//xml 加载失败
			pNodeList = pDoc->selectNodes("HotKeyword");
			if(pNodeList == NULL)
				throw 3;	//xml 格式错误

			pNodeList = pDoc->selectNodes("HotKeyword/KeyInfo");
			if(pNodeList){
				int count = 0;
				for(int pos=0; pos<pNodeList->length && count < 20; pos++,count ++){
					CString Key = GetXmlValue(pNodeList, "Key", pos);
					CString Time = GetXmlValue(pNodeList, "Time", pos);
					CString Result = GetXmlValue(pNodeList, "Result", pos);
					if(!Key.IsEmpty() && !Time.IsEmpty() && !Result.IsEmpty()){
						CString str;
						str.Format("%s(共搜索%s)", Key, Time, Result);
						HTREEITEM hCurItem = m_pHKTree->InsertItem(str, 28, 28, hItem);
						m_HKView->SetTreeItemData(hCurItem, Is_HotKeyleaf, NULL, 0, 0,Key);
					}
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(_com_error& e){
#ifdef _DEBUG
			//Tips(e.Description());
#endif
		}
		catch(int errnum){
			switch(errnum){
			case 3:		//已更新
				pNodeList.Release();
			case 2:		//格式错误
				pDoc.Release();
				break;
			}
		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CHotKeyListView::GetHotKeyWord()");
#endif
	}
	if (m_HKView->IsWindowVisible())
	{
		m_HKView->ShowWindow(SW_HIDE);
		m_HKView->ShowWindow(SW_SHOW);
	}
	m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}

void CGuidTreeView::InsertFanShuHotBook()
{

	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetFanShuHotBook,NULL,CREATE_SUSPENDED,NULL);
	if(handle)
	{
		threadList.AddTail(handle);
		ResumeThread(handle);
	}
}

unsigned __stdcall CGuidTreeView::_GetFanShuHotBook(LPVOID pParam)
{
	//Sleep(1000);
	HTREEITEM hkey1 = m_pHKTree->InsertItem("番薯畅销榜", 34, 34, NULL);
	m_HKView->SetTreeItemData(hkey1, Is_FanShuHotBook, NULL);
	HTREEITEM hItem1 = hkey1;
	HTREEITEM hkey2 = m_pHKTree->InsertItem("番薯新书榜", 34, 34, NULL);
	m_HKView->SetTreeItemData(hkey2, Is_FanShuHotBook, NULL);
	HTREEITEM hItem2 = hkey2;
	HTREEITEM hkey3 = m_pHKTree->InsertItem("番薯电子书店销售榜", 34, 34, NULL);
	m_HKView->SetTreeItemData(hkey3, Is_FanShuHotBook, NULL);
	HTREEITEM hItem3 = hkey3;
	HTREEITEM hkey4 = m_pHKTree->InsertItem("番薯电子书店最新上架", 34, 34, NULL);
	m_HKView->SetTreeItemData(hkey4, Is_FanShuHotBook, NULL);
	HTREEITEM hItem4 = hkey4;

	try{
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		try{
			char WebPath[1024] = {0};
			char TmpPath[_MAX_PATH] = {0};

			sprintf_s(WebPath, 1024, "%s\\search\\getfanshuhotbook.asp", AspServer);
			sprintf_s(TmpPath, _MAX_PATH, "%s\\temp\\getfanshuhotbook.xml", CurrentPath); 

			if(!DownLoad(WebPath,TmpPath,m_HKView->quit,m_HKView->socketList,m_HKView->m_mutex)|| (!offline))
			{

			}
			if (pMainWnd->m_quit)
			{
				//m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
				return 0;
			}
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//对象创建失败
			if(!pDoc->load(TmpPath))
				throw 2;	//xml 加载失败
			pNodeList = pDoc->selectNodes("root/畅销榜");
			if(pNodeList == NULL)
				throw 3;	//xml 格式错误

			pNodeList = pDoc->selectNodes("root/畅销榜/item");
			if(pNodeList){
				int count = 0;
				for(int pos=0; pos<pNodeList->length; pos++,count ++){
					CString title = GetXmlValue(pNodeList, "title", pos);
					if (title.GetAt(title.GetLength()-1)=='>'&&title.GetAt(title.GetLength()-2)=='a')
					{
						title=title.Left(title.GetLength()-6);
					}
					CTranscoding::UTF_8ToGB2312(title, title.GetBuffer(title.GetLength() + 1), title.GetLength());
					CString cover = GetXmlValue(pNodeList, "cover", pos);
					CString link = GetXmlValue(pNodeList, "link", pos);
					if(!title.IsEmpty()){
						HTREEITEM hCurItem;
						if (cover.GetLength()!=0)
						{
							try{
								char imgTmpPath[_MAX_PATH] = {0};
								sprintf_s(imgTmpPath, _MAX_PATH, "%s\\temp\\%s", CurrentPath,cover.Right(cover.GetLength()-cover.ReverseFind('/')-1)); 
								if(DownLoad(cover,imgTmpPath,m_HKView->quit,m_HKView->socketList,m_HKView->m_mutex))
								{
									HBITMAP hbitmap;
									CBitmap *bitmap = new CBitmap;
									CImageList *imgList = m_pHKTree->GetImageList(TVSIL_NORMAL);
									hbitmap=ImageLoad(imgTmpPath);
									bitmap->Attach(hbitmap);
									if(imgList->Add(bitmap,RGB(0,0,0))==-1)
									{
										hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem1);
										m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
										continue;
									}
									int num = imgList->GetImageCount();
									m_pHKTree->SetImageList(imgList, TVSIL_NORMAL);
									bitmap->DeleteObject();
									delete bitmap;
									imgList->Detach();
									hCurItem = m_pHKTree->InsertItem(title, num-1, num-1, hItem1);
								}else{hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem1);}
							}catch(...){
								hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem1);
								m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
							}
						}else{
							hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem1);
						}
						m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
					}
				}
				pNodeList.Release();
			}
			pNodeList = pDoc->selectNodes("root/新书榜");
			if(pNodeList == NULL)
				throw 3;	//xml 格式错误
			pNodeList = pDoc->selectNodes("root/新书榜/item");
			if(pNodeList){
				int count = 0;
				for(int pos=0; pos<pNodeList->length; pos++,count ++){
					CString title = GetXmlValue(pNodeList, "title", pos);
					if (title.GetAt(title.GetLength()-1)=='>'&&title.GetAt(title.GetLength()-2)=='a')
					{
						title=title.Left(title.GetLength()-6);
					}
					CTranscoding::UTF_8ToGB2312(title, title.GetBuffer(title.GetLength() + 1), title.GetLength());
					CString cover = GetXmlValue(pNodeList, "cover", pos);
					CString link = GetXmlValue(pNodeList, "link", pos);
					if(!title.IsEmpty()){
						HTREEITEM hCurItem;
						if (cover.GetLength()!=0)
						{
							try{
								char imgTmpPath[_MAX_PATH] = {0};
								sprintf_s(imgTmpPath, _MAX_PATH, "%s\\temp\\%s", CurrentPath,cover.Right(cover.GetLength()-cover.ReverseFind('/')-1)); 
								if(DownLoad(cover,imgTmpPath,m_HKView->quit,m_HKView->socketList,m_HKView->m_mutex))
								{
									HBITMAP hbitmap;
									CBitmap *bitmap = new CBitmap;
									CImageList *imgList = m_pHKTree->GetImageList(TVSIL_NORMAL);
									hbitmap=ImageLoad(imgTmpPath);
									bitmap->Attach(hbitmap);
									if(imgList->Add(bitmap,RGB(0,0,0))==-1)
									{
										hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem2);
										m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
										continue;
									}
									int num = imgList->GetImageCount();
									m_pHKTree->SetImageList(imgList, TVSIL_NORMAL);
									bitmap->DeleteObject();
									delete bitmap;
									imgList->Detach();
									hCurItem = m_pHKTree->InsertItem(title, num-1, num-1, hItem2);
								}else{hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem2);}
							}catch(...){
								hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem2);
								m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
							}

						}else{
							hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem2);
						}
						m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
					}
				}
				pNodeList.Release();
			}
			pNodeList = pDoc->selectNodes("root/电子书店销售榜");
			if(pNodeList == NULL)
				throw 3;	//xml 格式错误

			pNodeList = pDoc->selectNodes("root/电子书店销售榜/item");
			if(pNodeList){
				int count = 0;
				for(int pos=0; pos<pNodeList->length; pos++,count ++){
					CString title = GetXmlValue(pNodeList, "title", pos);
					if (title.GetAt(title.GetLength()-1)=='>'&&title.GetAt(title.GetLength()-2)=='a')
					{
						title=title.Left(title.GetLength()-6);
					}
					CTranscoding::UTF_8ToGB2312(title, title.GetBuffer(title.GetLength() + 1), title.GetLength());
					CString cover = GetXmlValue(pNodeList, "cover", pos);
					CString link = GetXmlValue(pNodeList, "link", pos);
					if(!title.IsEmpty()){
						HTREEITEM hCurItem;
						if (cover.GetLength()!=0)
						{
							try{
								char imgTmpPath[_MAX_PATH] = {0};
								sprintf_s(imgTmpPath, _MAX_PATH, "%s\\temp\\%s", CurrentPath,cover.Right(cover.GetLength()-cover.ReverseFind('/')-1)); 
								if(DownLoad(cover,imgTmpPath,m_HKView->quit,m_HKView->socketList,m_HKView->m_mutex))
								{
									HBITMAP hbitmap;
									CBitmap *bitmap = new CBitmap;
									CImageList *imgList = m_pHKTree->GetImageList(TVSIL_NORMAL);
									hbitmap=ImageLoad(imgTmpPath);
									bitmap->Attach(hbitmap);
									if(imgList->Add(bitmap,RGB(0,0,0))==-1)
									{
										hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem3);
										m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
										continue;
									}
									int num = imgList->GetImageCount();
									m_pHKTree->SetImageList(imgList, TVSIL_NORMAL);
									bitmap->DeleteObject();
									delete bitmap;
									imgList->Detach();
									hCurItem = m_pHKTree->InsertItem(title, num-1, num-1, hItem3);
								}else{hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem3);}
							}catch(...){
								hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem3);
								m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
							}

						}else{
							hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem3);
						}
						m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
					}
				}
				pNodeList.Release();
			}
			pNodeList = pDoc->selectNodes("root/电子书店最新上架");
			if(pNodeList == NULL)
				throw 3;	//xml 格式错误

			pNodeList = pDoc->selectNodes("root/电子书店最新上架/item");
			if(pNodeList){
				int count = 0;
				for(int pos=0; pos<pNodeList->length; pos++,count ++){
					CString title = GetXmlValue(pNodeList, "title", pos);
					if (title.GetAt(title.GetLength()-1)=='>'&&title.GetAt(title.GetLength()-2)=='a')
					{
						title=title.Left(title.GetLength()-6);
					}
					CTranscoding::UTF_8ToGB2312(title, title.GetBuffer(title.GetLength() + 1), title.GetLength());
					CString cover = GetXmlValue(pNodeList, "cover", pos);
					CString link = GetXmlValue(pNodeList, "link", pos);
					if(!title.IsEmpty()){
						HTREEITEM hCurItem;
						if (cover.GetLength()!=0)
						{
							try{
								char imgTmpPath[_MAX_PATH] = {0};
								sprintf_s(imgTmpPath, _MAX_PATH, "%s\\temp\\%s", CurrentPath,cover.Right(cover.GetLength()-cover.ReverseFind('/')-1)); 
								if(DownLoad(cover,imgTmpPath,m_HKView->quit,m_HKView->socketList,m_HKView->m_mutex))
								{
									HBITMAP hbitmap;
									CBitmap *bitmap = new CBitmap;
									CImageList *imgList = m_pHKTree->GetImageList(TVSIL_NORMAL);
									hbitmap=ImageLoad(imgTmpPath);
									bitmap->Attach(hbitmap);
									if(imgList->Add(bitmap,RGB(0,0,0))==-1)
									{
										hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem4);
										m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
										continue;
									}
									int num = imgList->GetImageCount();
									m_pHKTree->SetImageList(imgList, TVSIL_NORMAL);
									bitmap->DeleteObject();
									delete bitmap;
									imgList->Detach();
									hCurItem = m_pHKTree->InsertItem(title, num-1, num-1, hItem4);
								}else{hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem4);}
							}catch(...){
								hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem4);
								m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
							}

						}else{
							hCurItem = m_pHKTree->InsertItem(title, 34, 34, hItem4);
						}
						m_HKView->SetTreeItemData(hCurItem, Is_FanShuHotBookleaf, NULL, 0, 0,"",link);
					}
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(_com_error& e){
#ifdef _DEBUG
			//Tips(e.Description());
#endif
		}
		catch(int errnum){
			switch(errnum){
			case 3:		//已更新
				pNodeList.Release();
			case 2:		//格式错误
				pDoc.Release();
				break;
			}
		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("_GetFanShuHotBook()");
#endif
	}
	if (m_HKView->IsWindowVisible())
	{
		m_HKView->ShowWindow(SW_HIDE);
		m_HKView->ShowWindow(SW_SHOW);
	}
	m_HKView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}

HBITMAP CGuidTreeView::ImageLoad(CString szFile)
{
	LPPICTURE gpPicture; 
	HBITMAP hRet;
	HANDLE hFile = ::CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL); 
	if(INVALID_HANDLE_VALUE == hFile) 
		return NULL; 
	DWORD dwFileSize = ::GetFileSize(hFile, NULL); 
	ASSERT(-1 != dwFileSize); 
	LPVOID pvData = NULL; 
	HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, dwFileSize); 
	ASSERT(NULL != hGlobal); 
	pvData = ::GlobalLock(hGlobal); 
	ASSERT(NULL != pvData); 
	DWORD dwBytesRead = 0; 
	BOOL bRead = ::ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL); 
	ASSERT(FALSE != bRead); 
	::GlobalUnlock(hGlobal); 
	::CloseHandle(hFile); 
	LPSTREAM pstm = NULL; 
	HRESULT hr = ::CreateStreamOnHGlobal(hGlobal, TRUE, &pstm); 
	ASSERT(SUCCEEDED(hr) && pstm); 
	hr=::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture); 
	ASSERT(SUCCEEDED(hr) && gpPicture); 
	pstm->Release(); 
	OLE_HANDLE picHandle; 
	gpPicture->get_Handle(&picHandle); 
	HDC hdc = ::CreateCompatibleDC(NULL); 
	ASSERT(hdc); 
	::SelectObject(hdc, (HGDIOBJ) picHandle); 
	hRet = (HBITMAP)::GetCurrentObject(hdc, OBJ_BITMAP); 
	DeleteDC(hdc); 
	return hRet; 
}

void CGuidTreeView::InsertRStore()
{
	hAllGroup = m_pTree->InsertItem("Readings Store", 8, 8, NULL);
	SetTreeItemData(hAllGroup, Is_RStore, NULL, 0, 0);
}
