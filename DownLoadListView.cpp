// DownLoadListView.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "DownLoadListView.h"
#include "HttpSocket.h"
#include "FormarWaterMark.h"
#include "MainFrm.h"
#include "BTOperation.h"

#include "HttpUtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char UserName[50];
extern char AspServer[1024];
extern char CurrentPath[_MAX_PATH];
extern char ShareFileFolder[_MAX_PATH];
extern char FavorPath[_MAX_PATH];
extern CMainFrame* pMainWnd;
extern char SDDisk[20];
extern _ConnectionPtr m_pConnection;
/////////////////////////////////////////////////////////////////////////////
// CDownLoadListView
CDownLoadListView* pView = NULL;

IMPLEMENT_DYNCREATE(CDownLoadListView, CListView)

CDownLoadListView::CDownLoadListView()
{
	m_quit = FALSE;
	listitem = 0;
}

CDownLoadListView::~CDownLoadListView()
{
}


BEGIN_MESSAGE_MAP(CDownLoadListView, CListView)
	//{{AFX_MSG_MAP(CDownLoadListView)
	ON_WM_CREATE()
	ON_MESSAGE(WM_FILEDOWNLOAD, OnFileDownload)
	ON_MESSAGE(WM_NEW_DOWNLOAD, OnNewDownload)
	ON_MESSAGE(WM_UPDATE_DOWNLOAD, OnUpdateDownload)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_MESSAGE(WM_P2P_FILEDOWNLOAD_FINISHED, OnDownloadFinished)
	ON_MESSAGE(WM_THREAD_FINISHED, OnThreadFinished)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_TIMER()
	ON_MESSAGE(WM_P2P_FILEDOWNLOAD_ERROR, OnP2PFileDownloadError)
	ON_MESSAGE(WM_SOCKET_FINISHED, OnSocketFinished)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_STOPDOWNLOAD, OnMenuStopdownload)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownLoadListView drawing

void CDownLoadListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDownLoadListView diagnostics

#ifdef _DEBUG
void CDownLoadListView::AssertValid() const
{
	CView::AssertValid();
}

void CDownLoadListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDownLoadListView message handlers

int CDownLoadListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pList = &GetListCtrl();
	pView = this;

	m_pList->ModifyStyle(0, LVS_REPORT|LVS_SINGLESEL|LVS_NOSORTHEADER/*|LVS_OWNERDRAWFIXED*/);
	m_pList->SetExtendedStyle(LVS_EX_FULLROWSELECT);

//	CBitmap bitmap;
//	bitmap.LoadBitmap(IDB_IMGLIST_DOWNLOAD);
	CImageList il;
	il.Create(IDB_IMGLIST_DOWNLOAD, 16, 1, WHITENESS);
	m_pList->SetImageList(&il, LVSIL_SMALL);
	il.Detach();

	m_pList->InsertColumn(0, "", LVCFMT_LEFT, 20);
	m_pList->InsertColumn(1, "文件名", LVCFMT_LEFT, 65);
	m_pList->InsertColumn(2, "大小", LVCFMT_LEFT, 40);
	m_pList->InsertColumn(3, "已完成", LVCFMT_LEFT, 50);
	m_pList->InsertColumn(4, "剩余时间", LVCFMT_LEFT, 60);
	m_pList->InsertColumn(5, "已用时", LVCFMT_LEFT, 58);
	m_pList->InsertColumn(6, "速度", LVCFMT_LEFT, 50);

	SetTimer(1,1000,NULL);

//add by liym 20100401
/*
	HANDLE thTorrentUpload = (HANDLE)::_beginthreadex(NULL,0,_UploadTorrents,(LPVOID)this,CREATE_SUSPENDED,NULL);
	if(thTorrentUpload)
	{
		m_threadList.AddTail(thTorrentUpload);
		::ResumeThread(thTorrentUpload);
	}
*/

	return 0;
}


LRESULT CDownLoadListView::OnFileDownload(WPARAM w,LPARAM l)

{
	DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)w;
	int count = m_pList->GetItemCount();
	m_pList->InsertItem(count, "", 1);
//zuoss 090304初始下载速度为1B/s,下载已用时为1s,剩余用时为文件大小除以1B/s
	missionInfo->speed = 1;
	missionInfo->passTime = 1;
	missionInfo->restTime = missionInfo->fileLen/missionInfo->speed;
//end
	m_pList->SetItemData(count, (DWORD)missionInfo);
	
	//调度
	if(missionInfo->sourceType==dl_http)
	{
		HANDLE thRecv = (HANDLE)::_beginthreadex(NULL,0,_HttpDown,(LPVOID)missionInfo,CREATE_SUSPENDED,NULL);
		if(thRecv)
		{
			missioninfolist.AddTail(missionInfo);
			m_threadList.AddTail(thRecv);
			::ResumeThread(thRecv);
		}
	}
	else if(missionInfo->sourceType==dl_mail)
	{
		HANDLE thRecv = (HANDLE)::_beginthreadex(NULL,0,_DownLoadNetBookByEmail,(LPVOID)missionInfo,CREATE_SUSPENDED,NULL);
		if(thRecv)
		{
			missioninfolist.AddTail(missionInfo);
			m_threadList.AddTail(thRecv);
			::ResumeThread(thRecv);
		}
	}
	else if(missionInfo->sourceType==dl_bt)
	{
		HANDLE thRecv = (HANDLE)::_beginthreadex(NULL,0,_DownLoadNetBookByBT,(LPVOID)missionInfo,CREATE_SUSPENDED,NULL);
		if(thRecv)
		{
			missioninfolist.AddTail(missionInfo);
			m_threadList.AddTail(thRecv);
			::ResumeThread(thRecv);
		}
	}
	return TRUE;
}

unsigned __stdcall CDownLoadListView::_DownLoadNetBookByEmail(LPVOID pParam)
{
	DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)pParam;
	CString path = missionInfo->DstPath;
	if (path.GetLength() == 0)
	{
		CFileDialog fileDialog(FALSE,".wol", missionInfo->title,NULL,NULL);
		if(fileDialog.DoModal()!=IDOK){
			pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
			pMainWnd->DeleteFromDownLoadListView(missionInfo);
			return 0;
		}
		path = fileDialog.GetPathName().Left(fileDialog.GetPathName().ReverseFind('\\') + 1);
		strcpy_s(missionInfo->DstPath, _MAX_PATH, path);
	}
	CString missionsavepath;
	missionsavepath = CurrentPath + CString("\\system\\") + UserName;
	_mkdir(missionsavepath);
	missionsavepath += "\\missionsave.txt";
	CFile missionsave;
	while (!missionsave.Open(missionsavepath,CFile::modeReadWrite))
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		Sleep(1000);
	}
	CString info;//每个任务14行信息
	info.Format("%s\n%s\n%s\n%s\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%s\n",
	missionInfo->title,missionInfo->URL,missionInfo->DstPath,missionInfo->IP,
	missionInfo->MD5,missionInfo->fileType,missionInfo->sourceType,
	missionInfo->port,missionInfo->fileLen,missionInfo->finishedSize,
	missionInfo->speed,missionInfo->passTime,missionInfo->restTime,missionInfo->UserName);
	missionsave.SeekToEnd();
#if 0
	//anzhsoft delete for platform
	missionsave.WriteHuge(info,info.GetLength());
#endif
	missionsave.Close();
	CString url;	
	url.Format("%s/bookstore/getBookEmail.asp?bookid=%s",AspServer,missionInfo->URL);
	CString tmppath;
	tmppath.Format("%s\\temp\\netbookemailinfo.xml",CurrentPath);
	if(!DownLoad(url, tmppath, pMainWnd->m_quit, pMainWnd->m_socketList)) 
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		Tips("连接服务器失败，请检查当前网络连接");
		pView->DeleteFromMissionsave(missionInfo);
		pMainWnd->DeleteFromDownLoadListView(missionInfo);
		pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	} 
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	CString bookid;
	CString mailaddr;
	CString password;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)tmppath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("info");
		if(pNodeList){
			bookid = GetXmlValue(pNodeList, "bookid", 0);
			mailaddr = GetXmlValue(pNodeList, "email", 0);
			password = GetXmlValue(pNodeList, "pwd", 0);
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){
		DeleteFile(tmppath);
		Tips("连接服务器失败，请检查当前网络连接");
		pView->DeleteFromMissionsave(missionInfo);
		pMainWnd->DeleteFromDownLoadListView(missionInfo);
		pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	}
	DeleteFile(tmppath);
	
	url.Format("%s/bookstore/bookRequest.asp?bookid=%s&username=%s",AspServer,missionInfo->URL,UserName);
	if (!UpLoad(url))
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		Tips("连接服务器失败，请检查当前网络连接");
		pView->DeleteFromMissionsave(missionInfo);
		pMainWnd->DeleteFromDownLoadListView(missionInfo);
		pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	}
	Tips("请求已发送，若有好友处理您的请求，资源将保存到指定目录下");
	CForJmail* cj = new CForJmail(mailaddr,password);
	int index = cj->GetIndex(bookid);
	if (index == 0)
	{
		/*Tips("请求失败，对方可能无此文件");*/
		pMainWnd->DeleteFromDownLoadListView(missionInfo);
		pView->DeleteFromMissionsave(missionInfo);
		pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	}
	delete cj;
	Sleep(20*1000);
	cj = new CForJmail(mailaddr,password);
	path = cj->DownAtt(index,path,missionInfo->title);
	if(path.GetLength() != 0){
		Tips("下载成功，文件保存到" + path, td_msg,5);
	}else{/*Tips("请求失败，对方可能无此文件");*/}
	delete cj;
	url.Format("%s/bookstore/updateState.asp?bookid=%s",AspServer,missionInfo->URL);
	while (!UpLoad(url))
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		Sleep(1000);
	}
	strcpy_s(missionInfo->DstPath, _MAX_PATH, path);
	pView->DeleteFromMissionsave(missionInfo);
	pView->PostMessage(WM_P2P_FILEDOWNLOAD_FINISHED,(WPARAM)missionInfo);
	pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}


unsigned __stdcall CDownLoadListView::_HttpDown(LPVOID pParam)
{
try{
	DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)pParam;
	CHttpSocket HttpSocket;
	CFile DownloadFile;
	CString strServer,strObject;
	unsigned short nPort;
	DWORD dwServiceType;
	long nLength;
	const char *pRequestHeader = NULL;
	SOCKET tmpS = 0;
	if(!AfxParseURL(missionInfo->URL,dwServiceType,strServer,strObject,nPort))
	{
		if(missionInfo->from == from_SMS)
		{
			pMainWnd->Heart->SMSDownloadList->RemoveAt(pMainWnd->Heart->SMSDownloadList->Find(CString(missionInfo->bookID)));
		}
		return FALSE;
	}
	try{
		pRequestHeader = HttpSocket.FormatRequestHeader((LPTSTR)(LPCTSTR)strServer,(LPTSTR)(LPCTSTR)strObject,nLength, nPort);	
		if(!HttpSocket.Socket())
			throw 0;
		tmpS = HttpSocket.GetSocket();
		pView->m_sockList.AddTail(tmpS);
//		HttpSocket.SetTimeout(10000,0);//zhanganzhan on 090415
		if(!HttpSocket.Connect((LPTSTR)(LPCTSTR)strServer,nPort))
			throw 1;
		if(!HttpSocket.SendRequest())
			throw 1;
		
		int nLineSize = 0;
		char szLine[256];
		while(nLineSize != -1)
			nLineSize = HttpSocket.GetResponseLine(szLine,256);

		char szValue[30];
		BOOL bContLen = TRUE;
		int nFileSize = -1;
		missionInfo->fileLen = -1;
		if(HttpSocket.GetField("Content-Length",szValue,30)==-1)
			bContLen = FALSE;
		else
			missionInfo->fileLen = atol(szValue);

		if(!DownloadFile.Open(missionInfo->DstPath, CFile::modeCreate|CFile::modeWrite))
		{
			//AfxMessageBox(CString("can't create ") + missionInfo->DstPath);
			throw 2;
		}
		char pData[1024];
		int nReceSize = 0;
		BOOL first = TRUE;
		
		DWORD startTime = GetTickCount();
		DWORD oldTime = startTime;
		long oldFinishedSize = missionInfo->finishedSize;

		while(TRUE){
			nReceSize = HttpSocket.Receive(pData,1024);
			if(nReceSize == 0)
			{
				//AfxMessageBox("receive 0 byte");
				break;
			}
			if(nReceSize == -1)
				throw 3;
			if(first&&!bContLen){
				char* temp = strstr(pData,"\r\n");
				if(!temp)
					throw 3;
				nReceSize = nReceSize-(temp+2-pData);
				DownloadFile.Write(temp+2,nReceSize);
			}
			else
				DownloadFile.Write(pData,nReceSize);
			first = FALSE;
			missionInfo->finishedSize += nReceSize;
			DWORD curTime = GetTickCount();
			long curFinishedSize = missionInfo->finishedSize;
			missionInfo->passTime = (curTime-startTime)/1000;
			if(missionInfo->speed)
				missionInfo->restTime = ((missionInfo->fileLen-missionInfo->finishedSize)/missionInfo->speed);
			if(curTime-oldTime>1000){
				missionInfo->speed = (long)(curFinishedSize - oldFinishedSize)/((long)(curTime-oldTime)/1000);
				oldTime = curTime;
				oldFinishedSize = curFinishedSize;
			}
			if(bContLen && curFinishedSize>=missionInfo->fileLen)
				break;
		}
		if(!bContLen){
			long len = (long)DownloadFile.GetLength();
			DownloadFile.SetLength(len-7);
		}
		DownloadFile.Close();
		HttpSocket.CloseSocket();
		pView->PostMessage(WM_P2P_FILEDOWNLOAD_FINISHED,(WPARAM)missionInfo);
		pView->PostMessage(WM_SOCKET_FINISHED, (WPARAM)tmpS,(LPARAM)type_socket);
	}
	catch(int err){
		LVFINDINFO itemInfo = {0};
		itemInfo.flags = LVFI_PARAM;
		itemInfo.lParam = (LPARAM)missionInfo;
		int n = pView->m_pList->FindItem(&itemInfo);
		
		CString msg;
		msg.Format("%s 下载失败", missionInfo->DstPath);
		
		if(n>=0)
			pView->m_pList->DeleteItem(n);
		if(missionInfo->from == from_SMS)
		{
			pMainWnd->Heart->SMSDownloadList->RemoveAt(pMainWnd->Heart->SMSDownloadList->Find(CString(missionInfo->bookID)));
		}
		Tips(msg);
		//AfxMessageBox(msg);
		switch(err){
		case 3:
			DownloadFile.Close();
		case 2:
		case 1:
			HttpSocket.CloseSocket();
			break;
		}
		return 1;
	}
	
	pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}
catch(...){
#ifdef _DEBUG
	Tips("下载失败");
#endif
}
	return 0;
}


void CDownLoadListView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	DWORD data = m_pList->GetItemData(pNMListView->iItem);
	if(!data)
		free((LPVOID)data);

	*pResult = 0;
}

LRESULT CDownLoadListView::OnDownloadFinished(WPARAM w,LPARAM l)
{
	DownLoadMissionInfo* info = (DownLoadMissionInfo*)w;
	LVFINDINFO itemInfo = {0};
	itemInfo.flags = LVFI_PARAM;
	itemInfo.lParam = (LPARAM)info;

	int n = m_pList->FindItem(&itemInfo);

	CString msg;
	msg.Format("%s 下载完毕", info->title);

	if(n>=0)
		m_pList->DeleteItem(n);

	Tips(msg);
//added by zuoss 090522
	//下载后拷贝到share目录下。
	CString fname;
	CString temp;
	int pos;
	temp.Format("%s",info->DstPath);
	pos = temp.GetLength() - temp.ReverseFind('\\') - 1;
	fname = temp.Right(pos);
	if(info->from == from_rstore)
	{
		fname = info->title;
	}

	CString sharepath;
	sharepath.Format("%s\\share\\%s",CurrentPath,fname);
	CopyFile(info->DstPath,sharepath,TRUE);//同名文件存在不覆盖。

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
	if (cf.GetStatus(info->DstPath,fstatus))
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
		sprintf_s(SQL, 1000, "SELECT [addr],[cdate],[type],[title],[size],[year],[nlist] FROM loc WHERE type=%d and addr=\"%s\"", filetype, info->DstPath);
			m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
				adOpenKeyset, adLockOptimistic, adCmdText);
			if(m_pRecordset->EndOfFile)
			{
				m_pRecordset->AddNew();
				m_pRecordset->PutCollect("addr", _variant_t(info->DstPath));
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
		sprintf_s(reSQL, 1000, "SELECT [addr],[cdate],[type],[title] FROM recently WHERE type=%d and addr=\"%s\"", filetype, info->DstPath);
		m_pRecordset1->Open(reSQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
			adOpenKeyset, adLockOptimistic, adCmdText);
			if(m_pRecordset1->EndOfFile)
			{
				m_pRecordset1->AddNew();
				m_pRecordset1->PutCollect("addr", _variant_t(info->DstPath));
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
//	CString updatePath;
//	updatePath.Format("%s//share//updateShareAfterDownload.asp?username=%s&lastwritetime=%s&nfilesize=%d&filetype=%s",AspServer,UserName,strTime,filesize,filetype);
	//访问该页面更新数据库

	//询问是否同步到阅读器	
//	if (AfxMessageBox("是否将文件同步到阅读器？",MB_YESNO) == IDYES)
	{
		if (strcmp(SDDisk,"") == 0)
		{
			AfxMessageBox("当前不存在阅读器！请您确定阅读器已连接后，点击菜单“同步按钮”即可。");
			//存储到本地数据库
			char updateSQL[2048] = {0};
			sprintf_s(updateSQL, 2048, "update loc set SynFlag = 1 where addr = \"%s\"",info->DstPath);
			m_pConnection->Execute(updateSQL, NULL, adCmdText);
		}
		else
		{
			try{
			// ReadingsSync
			char path[_MAX_PATH];
			if(info->from == from_rstore){
				sprintf_s(path, _MAX_PATH, "%s\\RStore\\",SDDisk);
				fname = info->title;
			}else{
				sprintf_s(path, _MAX_PATH, "%s\\我的下载\\",SDDisk);
			}
		//	AfxMessageBox(SDDisk);
			if(!PathFileExists(path))
				_mkdir(path);
		
			strcat_s(path, _MAX_PATH, fname);

			if(CopyFile(info->DstPath,path,TRUE))
			{
				Tips(fname+" 下载完成，并且已经同步到阅读器",td_msg,5);
			}else{
				Tips(fname+" 同步失败",td_msg,5);
			}
			}catch(...){}
		}
	}
//end
		

	//假如是通过短信请求获得，则通知服务器下载完成
	if(info->from == from_SMS)
	{
		CString CompleteInfo;
		CompleteInfo.Format("http://61.181.14.184:8088/Readings/notifyDLStatus.do?id=%s&status=download_succ",info->requestID);
		if(UpLoad(CompleteInfo))
		{
			pMainWnd->Heart->SMSDownloadList->RemoveAt(pMainWnd->Heart->SMSDownloadList->Find(CString(info->bookID)));
		}
		//AfxMessageBox(info->requestID);
		pMainWnd->Heart->AddToPushList(info);
	}
	return TRUE;
}

LRESULT CDownLoadListView::OnThreadFinished(WPARAM w,LPARAM l)
{
	HANDLE handle = (HANDLE)w;
// 	POSITION pos = m_threadList.Find(handle);
// 	if(pos){
// 		m_threadList.RemoveAt(pos);
// 		missioninfolist.RemoveAt(pos);
// 	}
	HANDLE hd;
	DownLoadMissionInfo * dm = NULL;
	POSITION pos = m_threadList.GetHeadPosition();
	POSITION pos1 = missioninfolist.GetHeadPosition();
	POSITION pos2,pos3;
	while (pos)
	{
		pos2 = pos;
		pos3 = pos1;
		hd = m_threadList.GetNext(pos);
		dm = missioninfolist.GetNext(pos1);
		if (hd == handle)
		{
			m_threadList.RemoveAt(pos2);
			missioninfolist.RemoveAt(pos3);
			break;
		}
	}

	return TRUE;
}

void CDownLoadListView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = 0;

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT|CDDS_SUBITEM))
	{
		CRect rect;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		m_pList->GetSubItemRect(nItem, pLVCD->iSubItem, LVIR_BOUNDS, rect);
		rect.left+=2;
		rect.right-=2;
		rect.top+=1;
		DownLoadMissionInfo* info = (DownLoadMissionInfo*)m_pList->GetItemData(nItem);
		CString output;
		switch(pLVCD->iSubItem){
		case 0://图标
			*pResult = CDRF_DODEFAULT;//CDRF_SKIPDEFAULT;
			break;
		case 1://"文件名"
			pDC->DrawText(info->title,strlen(info->title),rect,DT_SINGLELINE|DT_MODIFYSTRING|DT_PATH_ELLIPSIS);
			*pResult = CDRF_SKIPDEFAULT;
			break;
		case 2://"大小"
			if(info->fileLen>1024*1024){
				output.Format("%d.%02d MB", info->fileLen/(1024*1024),  (info->fileLen%(1024*1024))/1024/10);
			}
			else{
				output.Format("%d.%02d KB", info->fileLen/1024,info->fileLen%1024/10);
			}
			pDC->DrawText((LPCSTR)output,output.GetLength(),rect,DT_SINGLELINE|DT_MODIFYSTRING|DT_PATH_ELLIPSIS);
			*pResult = CDRF_SKIPDEFAULT;
			break;
		case 3://"已完成"
			//pDC->FillSolidRect(rect,0xffffff);
			if(info->finishedSize>1024*1024){
				output.Format("%d.%02d MB", info->finishedSize/(1024*1024),  (info->finishedSize%(1024*1024))/1024/10);
			}
			else{
				output.Format("%d.%02d KB", info->finishedSize/1024,info->finishedSize%1024/10);
			}
			pDC->DrawText((LPCSTR)output,output.GetLength(),rect,DT_SINGLELINE|DT_MODIFYSTRING|DT_PATH_ELLIPSIS);
			*pResult = CDRF_SKIPDEFAULT;
			break;
		case 4://"剩余时间"
			if(info->restTime>3600)
				output.Format( "%d:%02d:%02d", info->restTime/3600,(info->restTime%3600)/60,info->restTime%60);
			else if(info->restTime>60)
				output.Format( "%d:%02d", info->restTime/60,info->restTime%60);
			else
				output.Format( "%d", info->restTime);
			pDC->DrawText((LPCSTR)output,output.GetLength(),rect,DT_SINGLELINE|DT_MODIFYSTRING|DT_PATH_ELLIPSIS);
			*pResult = CDRF_SKIPDEFAULT;
			break;
		case 5://"已用时"
			if(info->passTime>3600)
				output.Format( "%d:%02d:%02d", info->passTime/3600,(info->passTime%3600)/60,info->passTime%60);
			else if(info->restTime>60)
				output.Format( "%d:%02d", info->passTime/60,info->passTime%60);
			else
				output.Format( "%d", info->passTime);
			pDC->DrawText((LPCSTR)output,output.GetLength(),rect,DT_SINGLELINE|DT_MODIFYSTRING|DT_PATH_ELLIPSIS);
			*pResult = CDRF_SKIPDEFAULT;
			break;
		case 6://"速度"
			if(info->speed>1024*1024)
				output.Format("%d.%02d MB/s", info->speed/(1024*1024),  (info->speed%(1024*1024))/1024/10);
			else if(info->speed>1024)
				output.Format("%d.%02d KB/s", info->speed/1024,info->speed%1024/10);
			else
				output.Format("%d B/s", info->speed);
			pDC->DrawText((LPCSTR)output,output.GetLength(),rect,DT_SINGLELINE|DT_MODIFYSTRING|DT_PATH_ELLIPSIS);
			*pResult = CDRF_SKIPDEFAULT;
			break;
		default:
			*pResult = CDRF_DODEFAULT;//CDRF_SKIPDEFAULT;
		}
	}
}

void CDownLoadListView::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1){
		int nItem = m_pList->GetItemCount();
		//zuoss 090305
		for(int i = 0;i<nItem;i++)
		{
			DownLoadMissionInfo* missoinInfo = (DownLoadMissionInfo*)m_pList->GetItemData(i);
			missoinInfo->passTime += 1; 
		}
		//end
		SetRedraw(TRUE);
		if(nItem)
			m_pList->RedrawItems(0, nItem-1);
	}
}

int CDownLoadListView::SendRequest(DownLoadMissionInfo* missoinInfo)
{
	SOCKET dipHole = ::socket(AF_INET, SOCK_DGRAM, 0);
	if(dipHole == INVALID_SOCKET)
		return -1;
	
	char iniPath[_MAX_PATH] = {0};
	char serverIP[50] = {0};
	char szheartport[20] = {0};
	char sztranport[20] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("server", "ip", NULL, serverIP, 50, iniPath);
	GetPrivateProfileString("server", "heartport", NULL, szheartport, 20, iniPath);
	GetPrivateProfileString("client", "tranport", NULL, sztranport, 20, iniPath);

	int tranport = atoi(sztranport);
	int retryTimes = 0;

	while(retryTimes<10){
		sockaddr_in local = {0};
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = ADDR_ANY;
		local.sin_port = htons(tranport);
		if(SOCKET_ERROR != ::bind(dipHole, (SOCKADDR*)&local, sizeof(local))){
			break;
		}
		retryTimes++;
		tranport++;
	}

	if(retryTimes>=10){
#ifdef _DEBUG
		Tips("bind() error");
#endif
		::closesocket(dipHole);
		return -1;
	}
	
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverIP);
	server.sin_port = htons(atoi(szheartport));
	memset(&(server.sin_zero), '\0', 8);

	char SendBuf[BUFFER_SIZE] ={0};
	SendBuf[0] = state_peer_connect_request;
	FILEREQ* res = (FILEREQ*)(SendBuf+1);
	strcpy_s(res->FileName, _MAX_PATH+1, missoinInfo->URL);
	memcpy(res->MD5, missoinInfo->MD5, MD5LEN);
	strcpy_s(res->UserName, 30, missoinInfo->UserName);
	//私有ip和私有端口
	char computename[155]={0};
	char username[28]={0};
	PHOSTENT hostinfo; 
	if(gethostname(computename, sizeof(computename)) == 0)
		if((hostinfo = gethostbyname(computename)) != NULL)
			strcpy_s(res->IP, 20, inet_ntoa(*(in_addr *)*hostinfo->h_addr_list));

	res->port = tranport;

	if(SOCKET_ERROR == ::sendto(dipHole, SendBuf, BUFFER_SIZE, 0,(SOCKADDR*)&server, sizeof(server))){
		::closesocket(dipHole);
		return -1;
	}


	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(missoinInfo->IP);
	server.sin_port = htons(missoinInfo->port);
	memset(&(server.sin_zero), '\0', 8);

	if(SOCKET_ERROR == ::sendto(dipHole, SendBuf, BUFFER_SIZE, 0,(SOCKADDR*)&server, sizeof(server))){
		::closesocket(dipHole);
		return -1;
	}
	char tipInfo[BUFFER_SIZE];
	sprintf_s(tipInfo, BUFFER_SIZE, "如果下载速度过慢，请使用邮箱方式获得资源。\n%s::%d",missoinInfo->IP,missoinInfo->port);
	Tips(tipInfo);

	//::shutdown(dipHole,SD_RECEIVE);
	::closesocket(dipHole);
	return tranport;
}

LRESULT CDownLoadListView::OnP2PFileDownloadError(WPARAM w,LPARAM l)
{
	DownLoadMissionInfo* info = (DownLoadMissionInfo*)w;
    LONG nError = (LONG)l;
	LVFINDINFO itemInfo = {0};
	itemInfo.flags = LVFI_PARAM;
	itemInfo.lParam = (LPARAM)info;

	int n = m_pList->FindItem(&itemInfo);
	if(n>=0){
		CString msg;
		msg.Format("%s 下载失败！", info->title);
		Tips(msg, td_alert);
		m_pList->DeleteItem(n);
	}

	return TRUE;
}

LRESULT CDownLoadListView::OnSocketFinished(UINT socket, LONG type)
{
	POSITION pos;
	if(type==type_socket){
		if(pos=m_sockList.Find(socket))
			m_sockList.RemoveAt(pos);
	}
	
	return TRUE;
}

void CDownLoadListView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	listitem = m_pList->HitTest(point,NULL); 

	CListView::OnRButtonDown(nFlags, point);
	ClientToScreen(&point);
	CMenu* pMenu;
	m_menu.LoadMenu(IDR_MENU_DOWNLOAD);
	pMenu = m_menu.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	m_menu.DestroyMenu();
}

void CDownLoadListView::OnMenuStopdownload() 
{
	// TODO: Add your command handler code here
	HANDLE hd;
	DownLoadMissionInfo* dm;
	int i = listitem;
	POSITION pos = m_threadList.GetHeadPosition();
	POSITION pos1 = missioninfolist.GetHeadPosition();
	if (pos == NULL || i < 0)
	{
		return;
	}
	
	while (i --)
	{
		hd = m_threadList.GetNext(pos);
		dm = missioninfolist.GetNext(pos1); 
	}
	POSITION pos2 = pos;
	POSITION pos3 = pos1;
	hd = m_threadList.GetNext(pos);
	dm = missioninfolist.GetNext(pos1); 
	
	m_threadList.RemoveAt(pos2);
	DeleteFromMissionsave(dm);
	missioninfolist.RemoveAt(pos3);

	if (listitem >= 0)
	{
		m_pList->DeleteItem(listitem);
	}
	TerminateThread(hd,0);
	
}

void CDownLoadListView::OnDestroy() 
{
	CListView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CDownLoadListView::DeleteFromMissionsave(DownLoadMissionInfo* minfo)
{
	CList<DownLoadMissionInfo*,DownLoadMissionInfo*&> infolist;
	CString missionsavepath;
	missionsavepath = CurrentPath + CString("\\system\\") + UserName;
	missionsavepath += "\\missionsave.txt";
 	if(!PathFileExists(missionsavepath))
 		return FALSE;
	CFile missionsave;
	while (!missionsave.Open(missionsavepath,CFile::modeReadWrite))
	{
		if (pMainWnd->m_quit)
		{
			return FALSE;
		}
		Sleep(1000);
	}
	missionsave.Close();
	ifstream info;
	info.open(missionsavepath);
	int i = 0;
	char lineinfo[MAX_PATH];
	
	while (info.getline(lineinfo,MAX_PATH))
	{
		DownLoadMissionInfo* miinfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
		ZeroMemory(miinfo, sizeof(DownLoadMissionInfo));
		strcpy_s(miinfo->title, _MAX_PATH, lineinfo);
		if(info.getline(lineinfo,MAX_PATH))
			strcpy_s(miinfo->URL, 1024, lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			strcpy_s(miinfo->DstPath, _MAX_PATH, lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
			strcpy_s(miinfo->IP, 20, lineinfo);
		else
			break;
		if(info.getline(lineinfo,MAX_PATH))
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
	POSITION pos = infolist.GetHeadPosition();
	while (pos)
	{
		POSITION pos1 = pos;
		DownLoadMissionInfo* mis = infolist.GetNext(pos);
		if ((mis->fileType == minfo->fileType) && strcmp(mis->IP,minfo->IP) == 0
			&& strcmp(mis->MD5,minfo->MD5) == 0 && mis->port == minfo->port
			&& mis->sourceType == minfo->sourceType && strcmp(mis->title,minfo->title) == 0
			&& strcmp(mis->URL,minfo->URL) == 0 && strcmp(mis->UserName,minfo->UserName) == 0)
		{
			infolist.RemoveAt(pos1);
			break;
		}
	}
	while(!missionsave.Open(missionsavepath,CFile::modeCreate|CFile::modeReadWrite))
	{
		if (pMainWnd->m_quit)
		{
			return FALSE;
		}
		Sleep(100);
	}
	pos = infolist.GetHeadPosition();
	while (pos)
	{
		DownLoadMissionInfo* mis = infolist.GetNext(pos);
		CString missinfo;//每个任务14行信息
		missinfo.Format("%s\n%s\n%s\n%s\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%s\n",
			mis->title,mis->URL,mis->DstPath,mis->IP,
			mis->MD5,mis->fileType,mis->sourceType,
			mis->port,mis->fileLen,mis->finishedSize,
			mis->speed,mis->passTime,mis->restTime,mis->UserName);
		missionsave.SeekToEnd();
#if 0
		// deleted by for platform
		missionsave.WriteHuge(missinfo,missinfo.GetLength());
#endif
	}
	missionsave.Close();
	return TRUE;
}

LRESULT CDownLoadListView::OnNewDownload(WPARAM w,LPARAM l){
/*	int count = m_pList->GetItemCount();
	m_pList->InsertItem(count, "", 1);
	missionInfo->speed = 1;
	missionInfo->passTime = 1;
	missionInfo->restTime = missionInfo->fileLen/missionInfo->speed;
	m_pList->SetItemData(count, (DWORD)missionInfo);*/
	return true;
}

//更新BT下载进度
LRESULT CDownLoadListView::OnUpdateDownload(WPARAM w,LPARAM l){
	DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo *)w;
    int count = m_pList->GetItemCount();
	if(m_pList->GetItemCount() != 0){
		int i;
		for(i=0;i<count;i++){
			DownLoadMissionInfo *tempInfo=(DownLoadMissionInfo *)m_pList->GetItemData(i);
			if(strcmp(tempInfo->URL,missionInfo->URL)==0){
				//AfxMessageBox(CString("update ")+tempInfo->title);
				tempInfo->finishedSize=missionInfo->finishedSize;
				tempInfo->process=missionInfo->process;
				if (tempInfo->finishedSize == 0)
				{
					tempInfo->finishedSize = (double)tempInfo->fileLen*tempInfo->process;
				}
				if(tempInfo->passTime>0)
					tempInfo->speed = tempInfo->finishedSize/tempInfo->passTime;
				if(tempInfo->speed>0)
					tempInfo->restTime = (tempInfo->fileLen - tempInfo->finishedSize)/tempInfo->speed;
				//if (tempInfo->finishedSize >= tempInfo->fileLen)
				if(tempInfo->process == 1.0)
				{
					tempInfo->finishedSize = tempInfo->fileLen;
					strcpy_s(tempInfo->DstPath, _MAX_PATH, tempInfo->DstPath+CString("\\")+CString(tempInfo->title));
					Tips("文件\"" + CString(tempInfo->title) + "\"下载完成!");
					pView->m_pList->DeleteItem(i);
					pMainWnd->AddToReadings(tempInfo->DstPath);//加入最近阅读和同步列表
					pMainWnd->Heart->AddToPushList(tempInfo);//加入到推送箱
				}
				break;
			}
		}
		if (i == count && missionInfo->process < 1.0)
		{
			pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
		}
	}else if(missionInfo->process < 1.0){
		pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
	}
	return true;
}

unsigned __stdcall CDownLoadListView::_DownLoadNetBookByBT(LPVOID pParam)
{
	DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)pParam;

	//用下载后的种子文件名作为任务的唯一标识
	CString torrentname = CBTOperation::DownloadFile(missionInfo->URL, missionInfo->DstPath);
	if (torrentname == "")
	{
		Tips("文件\"" + CString(missionInfo->title) + "\"下载失败，请检查当前网络连接");
		LVFINDINFO itemInfo = {0};
		itemInfo.flags = LVFI_PARAM;
		itemInfo.lParam = (LPARAM)missionInfo;
		int n = pView->m_pList->FindItem(&itemInfo);
		if(n>=0)
			pView->m_pList->DeleteItem(n);
		return false;
	}
	strcpy_s(missionInfo->URL, 1024, torrentname);
	
	Tips("文件\"" + CString(missionInfo->title) + "\"开始下载，下载完成后将保存到指定文件夹下，您可以在右下角查看下载进度。");

	return true;
}
