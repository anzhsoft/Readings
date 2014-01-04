// ShowView.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ShowView.h"

#include "FormarWaterMark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowView
extern char UserName[50];
extern char Password[50];
extern _ConnectionPtr m_pConnection;
extern char AspServer[1024];

IMPLEMENT_DYNCREATE(CShowView, CView)

CShowView::CShowView()
{
}

CShowView::~CShowView()
{
}


BEGIN_MESSAGE_MAP(CShowView, CView)
	//{{AFX_MSG_MAP(CShowView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_UP_PAGE, OnUpPage)
	ON_MESSAGE(WM_DOWN_PAGE, OnDownPage)
	ON_MESSAGE(WM_ZOOM, OnZoom)
	ON_MESSAGE(WM_BUTTON_MENU, OnMenu)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_WOLMENU, OnSelchangedTreeCatalog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowView drawing

void CShowView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CShowView diagnostics

#ifdef _DEBUG
void CShowView::AssertValid() const
{
	CView::AssertValid();
}

void CShowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowView message handlers

int CShowView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_showDialog.Create(IDD_DIALOG_SHOW, this))
		return -1;
//	m_showDialog.ShowWindow(SW_SHOW);

	
	if(!m_readDialog.Create(IDD_DIALOG_READ, this))
		return -1;
//	m_readDialog.ShowWindow(SW_HIDE);
	
	CString URL;
	if (strcmp(UserName,"Guest_Default") == 0)
	{
		URL.Format("%s/index.asp",AspServer);
	}
	else
	{
		URL.Format("%s/index.asp?userid=%s&pwd=%s",AspServer,UserName,Password);
	}
 	OpenURL(URL);

	return 0;
}

void CShowView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if(m_showDialog.GetSafeHwnd()){
		if(m_showDialog.IsWindowVisible())
			m_showDialog.MoveWindow(0,0,cx,cy);
	}

	int offset = 0;
	if(m_readDialog.GetSafeHwnd()){
		if(m_readDialog.m_BookMenu.GetSafeHwnd()){
			if(m_readDialog.m_BookMenu.IsWindowVisible()){
				offset = 200;
				m_readDialog.m_BookMenu.MoveWindow(cx-offset,0,offset,cy-25);
			}
		}
		m_readDialog.MoveWindow(0,0,cx-offset,cy-25);
	}

	if(m_readDialog.m_controlBar.GetSafeHwnd()){
		m_readDialog.m_controlBar.MoveWindow(0,cy-25,cx,25);
	}
}

void CShowView::OpenFile(CString FilePath)
{
	int FileType = GetFileTypeFromFull(FilePath);

	CString wm;
	int page;
	switch(FileType){
	case wolFileType:
	case txtFileType:
		m_readDialog.ShowWindow(SW_SHOW);
		m_readDialog.m_controlBar.ShowWindow(SW_SHOW);
		m_showDialog.ShowWindow(SW_HIDE);
		if(!m_readDialog.Open(FilePath)){
			//begin add by dongzhenhua 20090524 
			if (m_readDialog.tmpWol==TRUE)
			{
				OpenFile(m_readDialog.tmpFilePath);
				break;
			}
			
			//end add by dongzhenhua 20090524
			Tips("文件已损坏或已加密",td_msg,4);
			return;
		}
//		AfxMessageBox(FilePath);
		page = m_readDialog.getStartPage(FilePath,FileType);
		m_readDialog.ShowPage(page);
		break;
	case unknownFileType:
		return;
	case pdfFileType:
		wm = ExtractPDF(FilePath);
		if(!wm.IsEmpty())
			AfxMessageBox("传播踪迹:\r\n"+wm);
		::ShellExecute(NULL, "open", FilePath, NULL, NULL, SW_SHOW);
		break;
	case chmFileType:
		wm = ExtractCHM(FilePath);
		if(!wm.IsEmpty())
			AfxMessageBox("传播踪迹:\r\n"+wm);
		::ShellExecute(NULL, "open", FilePath, NULL, NULL, SW_SHOW);
		break;
	case docFileType:
		wm = ExtractDOC(FilePath);
		if(!wm.IsEmpty())
			AfxMessageBox("传播踪迹:\r\n"+wm);
		::ShellExecute(NULL, "open", FilePath, NULL, NULL, SW_SHOW);
		break;
	default:
		::ShellExecute(NULL, "open", FilePath, NULL, NULL, SW_SHOW);
	}
	AddToRecentlyRecord(FilePath, FileType);
}

void CShowView::OpenURL(CString FilePath)
{
	m_readDialog.ShowWindow(SW_HIDE);
	m_readDialog.m_controlBar.ShowWindow(SW_HIDE);
	m_showDialog.ShowWindow(SW_SHOW);
	m_showDialog.m_browser.Navigate(FilePath, NULL, NULL, NULL, NULL);
}

LRESULT CShowView::OnUpPage(WPARAM w,LPARAM l)
{
	m_readDialog.PostMessage(WM_UP_PAGE);
	return TRUE;
}

LRESULT CShowView::OnDownPage(WPARAM w,LPARAM l)
{
	m_readDialog.PostMessage(WM_DOWN_PAGE);
	return TRUE;
}

LRESULT CShowView::OnZoom(WPARAM w,LPARAM l)
{
	m_readDialog.PostMessage(WM_ZOOM);
	return TRUE;
}

LRESULT CShowView::OnMenu(WPARAM w,LPARAM l)
{
	m_readDialog.SendMessage(WM_BUTTON_MENU);

	CRect rect;
	GetClientRect(rect);

	int cx = rect.Width();
	int cy = rect.Height();
	
	int offset = 0;
	if(m_readDialog.GetSafeHwnd()){
		if(m_readDialog.m_BookMenu.GetSafeHwnd()){
			if(m_readDialog.m_BookMenu.IsWindowVisible()){
				offset = 200;
				m_readDialog.m_BookMenu.MoveWindow(cx-offset,0,offset,cy-25);
			}
		}
		m_readDialog.MoveWindow(0,0,cx-offset,cy-25);
		m_readDialog.Invalidate(FALSE);
	}

	if(m_readDialog.m_controlBar.GetSafeHwnd()){
		m_readDialog.m_controlBar.MoveWindow(0,cy-25,cx,25);
	}


	return TRUE;
}

void CShowView::AddToRecentlyRecord(CString FilePath, int filetype)
{
	_RecordsetPtr m_pRecordset;
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	try{    
		char SQL[1000]={0};
		sprintf_s(SQL, 1000, "DELETE FROM push WHERE path=\"%s\"", FilePath);
		m_pConnection->Execute(SQL, NULL, adCmdText);
		memset(SQL, 0, 1000);
		FilePath.Replace('/','\\');
		sprintf_s(SQL, 1000, "SELECT [addr],[record],[cdate],[title],[type] FROM recently WHERE addr=\"%s\"", FilePath);
		m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
			adOpenDynamic, adLockOptimistic, adCmdText);
		if(m_pRecordset->EndOfFile){
			CFileFind find;
			BOOL ext = find.FindFile(FilePath);
			if(ext){
				find.FindNextFile();				
				m_pRecordset->AddNew();
				m_pRecordset->PutCollect("addr", _variant_t(FilePath));
				CTime tm = CTime::GetCurrentTime();
				CString strTime = tm.Format("%Y-%m-%d %H:%M");
				m_pRecordset->PutCollect("record", _variant_t(strTime));
				find.GetLastWriteTime(tm);	//.GetCreationTime(tm);
				strTime = tm.Format("%Y-%m-%d %H:%M");
				m_pRecordset->PutCollect("cdate", _variant_t(strTime));
				m_pRecordset->PutCollect("title", _variant_t(find.GetFileTitle()));
				m_pRecordset->PutCollect("type", (_variant_t)(long)filetype);
				m_pRecordset->Update();
			}
			find.Close();
		}
		else{
			m_pRecordset->PutCollect("record", _variant_t(GetDate()));
			m_pRecordset->Update();
		}
		m_pRecordset->Close();
	//	m_pRecordset.Release();
	}
	catch(_com_error& e){}
}

void CShowView::OnSelchangedTreeCatalog(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = m_readDialog.m_BookMenu.GetSelectedItem();
	DWORD PageNum = m_readDialog.m_BookMenu.GetItemData(hItem);
	m_readDialog.ShowPage(PageNum);
}