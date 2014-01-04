// HotKeyListView.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "HotKeyListView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL offline;
extern char AspServer[1024];
extern char JspServer[1024];
extern char CurrentPath[_MAX_PATH];
extern CMainFrame* pMainWnd;
extern enum COMMOPER ;
extern bool gbOnSearching;// Added by zhanganzhan on 20081024

/////////////////////////////////////////////////////////////////////////////
// CHotKeyListView

IMPLEMENT_DYNCREATE(CHotKeyListView, CListView)

CHotKeyListView::CHotKeyListView()
{
	thGetKey = NULL;
	quit = FALSE;

    ctlfont.CreateFont(12, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_MEDIUM, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体")); 

	selfont.CreateFont(13, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_SEMIBOLD, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体")); 

    strfont.CreateFont(12, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体")); 

	m_mouseOn = m_mouseOn_old = -1; 

}

CHotKeyListView::~CHotKeyListView()
{
	ctlfont.DeleteObject();
	strfont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CHotKeyListView, CListView)
	//{{AFX_MSG_MAP(CHotKeyListView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHotKeyListView drawing

void CHotKeyListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHotKeyListView diagnostics

#ifdef _DEBUG
void CHotKeyListView::AssertValid() const
{
	CListView::AssertValid();
}

void CHotKeyListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHotKeyListView message handlers
int CHotKeyListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFont(&ctlfont);
	showFlag = 0;

	CListCtrl& list = GetListCtrl();
	list.ModifyStyle(0, LVS_REPORT|LVS_SINGLESEL|LVS_OWNERDRAWFIXED);

	thGetKey = AfxBeginThread(_GetHotKeyWord, this);
	thGetKey->m_bAutoDelete = TRUE;
	thGetKey->ResumeThread();


	return 0;
}

UINT CHotKeyListView::_GetHotKeyWord(LPVOID pParam)
{
	CHotKeyListView* pView = (CHotKeyListView*)pParam;
	if(!pParam)
		return 1;

	pView->GetHotKeyWord();

	return 0;
}

void CHotKeyListView::GetHotKeyWord()
{
	//热词改为触发式，取消定时刷新。
try{
	CoInitialize(NULL);
	KillTimer(1);
	//int elapse = 1000*60*3;	//三分钟
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		char WebPath[1024] = {0};
		char TmpPath[_MAX_PATH] = {0};
		switch(showFlag)
		{
		case 0: sprintf_s(WebPath, 1024, "%s\\search\\hotsearch.asp", AspServer);sprintf_s(TmpPath, _MAX_PATH, "%s\\temp\\hotSearch.xml", CurrentPath); break;
		case 1: sprintf_s(WebPath, 1024, "%s\\search\\mosthotkey.asp", AspServer); sprintf_s(TmpPath,  _MAX_PATH,  "%s\\temp\\mosthotSearch.xml", CurrentPath);break;
		case 2: sprintf_s(WebPath, 1024, "%s\\search\\recommendhotkey.asp", AspServer);sprintf_s(TmpPath,  _MAX_PATH, "%s\\temp\\recommendhotSearch.xml", CurrentPath); break;
		}
		
		
		//DeleteFile(TmpPath);		delete by dzh 081009
		if(!DownLoad(WebPath, TmpPath, quit, socklist) || (!offline))//modified by zuoss 20081010
		{
	//		Tips("本地缓存热词");
			//throw 0;			delete by dzh 081009
		}
		if (pMainWnd->m_quit)
		{
			return;
		}
		 //modified by yangixao 2008.8.15 begin
// 		CFile * pFile = new CFile();
// 		pFile->Open(TmpPath,CFile::modeRead);
// 		long fileSize = pFile->GetLength();
// 		pFile->Close();	
// 		m_hotKeyLog.setLogItem("传送热词",fileSize, down, comm_oper_tranHotKey);
// 		m_hotKeyLog.writeLogItem();
// 	
		//modified by yangixao 2008.8.15 end

		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//对象创建失败
		if(!pDoc->load(TmpPath))
			throw 2;	//xml 加载失败
		pNodeList = pDoc->selectNodes("HotKeyword");
		if(pNodeList == NULL)
			throw 3;	//xml 格式错误
		//设置下次的刷新时间间隔
		CString refreshInterval = GetXmlValue(pNodeList, "RefreshInterval", 0);
		//以秒为单位
		//if(atoi((LPCSTR)refreshInterval)>0)
			//elapse = 1000*atoi((LPCSTR)refreshInterval);//90秒

		CListCtrl& list = GetListCtrl();
		CString onlineUserNum = GetXmlValue(pNodeList, "OnlineUserNum", 0);
		if(!onlineUserNum.IsEmpty())
		{
			list.DeleteColumn(0);
			CString itemTitle;
			itemTitle.Format("在线人数:%s  刷新显示", onlineUserNum);
			list.InsertColumn(0, itemTitle, LVCFMT_LEFT, 200);
		}
		
		pNodeList.Release();
		pNodeList = pDoc->selectNodes("HotKeyword/KeyInfo");
		if(pNodeList){
			list.DeleteAllItems();
			for(int pos=0; pos<pNodeList->length; pos++){
				CString Key = GetXmlValue(pNodeList, "Key", pos);
				CString Time = GetXmlValue(pNodeList, "Time", pos);
				CString Result = GetXmlValue(pNodeList, "Result", pos);
				if(!Key.IsEmpty() && !Time.IsEmpty() && !Result.IsEmpty()){
					CString str;
					str.Format("%s  %s  %s", Key, Time, Result);
					list.InsertItem(pos, str);
					HotKeyInfo* info = (HotKeyInfo*)malloc(sizeof(HotKeyInfo));
					ZeroMemory(info, sizeof(HotKeyInfo));
					strcpy_s(info->key, 1024, Key);
					strcpy_s(info->date, 100, Time);
					strcpy_s(info->res, 100, Result);
					list.SetItemData(pos, (DWORD)info);
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
//	SetTimer(1, elapse, NULL);
	CoUninitialize();
}
catch(...){
#ifdef _DEBUG
	Tips("void CHotKeyListView::GetHotKeyWord()");
#endif
}
}

void CHotKeyListView::OnTimer(UINT nIDEvent) 
{
//	if(nIDEvent==1){
//		thGetKey = AfxBeginThread(_GetHotKeyWord, this);
//		thGetKey->m_bAutoDelete = TRUE;
//		thGetKey->ResumeThread();
//	}
 	
	CListView::OnTimer(nIDEvent);
}

void CHotKeyListView::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 24;
}

void CHotKeyListView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
/*
	if(lpDrawItemStruct->itemState & ODS_SELECTED){
		DrawRow(pDC, lpDrawItemStruct->itemID, lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHTTEXT), GetSysColor(COLOR_HIGHLIGHT), lpDrawItemStruct->itemData);
	}
	else{
		DrawRow(pDC, lpDrawItemStruct->itemID, lpDrawItemStruct->rcItem, RGB(0, 0, 0), RGB(255, 255, 255), lpDrawItemStruct->itemData);
	}
*/
	DrawRow(pDC, lpDrawItemStruct->itemID, lpDrawItemStruct->rcItem, RGB(0, 0, 0), RGB(255, 255, 255), lpDrawItemStruct->itemData);
}

void CHotKeyListView::DrawRow(CDC* pDC, int nItem, CRect rect, COLORREF textColor, COLORREF bgColor, DWORD data)
{
try{
	if(!data)
		return;

	CFont* oldFont;

	HotKeyInfo* info = (HotKeyInfo*)data;
	pDC->FillSolidRect(rect, bgColor);
	CRect krect(10+rect.left, (rect.Height()-14)/2+rect.top,rect.right, rect.bottom);
	pDC->SetTextColor(textColor);
	if(nItem==m_mouseOn){
		oldFont = pDC->SelectObject(&selfont);
		pDC->DrawText(info->key, strlen(info->key), &krect, DT_LEFT|DT_SINGLELINE|DT_MODIFYSTRING|DT_END_ELLIPSIS);
		pDC->SelectObject(oldFont);
	}
	else
		pDC->DrawText(info->key, strlen(info->key), &krect, DT_LEFT|DT_SINGLELINE|DT_MODIFYSTRING|DT_END_ELLIPSIS);
	CSize sz = pDC->GetOutputTextExtent(info->key);
	
/*
	// add by dongzhenhua 20080116
	CPoint OriginPoint,EndPoint;
	OriginPoint.x = rect.left + 10;
	OriginPoint.y = (rect.Height()-14)/2+rect.top + 15;
	EndPoint.x = rect.right;
	EndPoint.y = (rect.Height()-14)/2+rect.top + 15;
	// 使线与字为相同的颜色
	CPen m_pen; 
	pDC->SetROP2(R2_NOTXORPEN); 
	m_pen.CreatePen(PS_SOLID,1,textColor); 
	CPen* pen=(CPen*)pDC->SelectObject(&m_pen); 
	COLORREF OldColor=pDC->SetBkColor(pDC->GetBkColor()); 
	int OldBkMode=pDC->SetBkMode(TRANSPARENT); 
	pDC->MoveTo(OriginPoint);
	pDC->LineTo(EndPoint);
	pDC->SelectObject(pen); 
	pDC->SetBkMode(OldBkMode); 
	pDC->SetBkColor(OldColor);
	//end add by dongzhenhua 20080116
*/

	oldFont = pDC->SelectObject(&strfont);
	CString str;
	str.Format("%s %s", info->date, info->res);
	CRect orect(20+rect.left+sz.cx, (rect.Height()-14)/2+rect.top+2,rect.right,rect.bottom);
	pDC->DrawText((LPCSTR)str, str.GetLength(), &orect, DT_LEFT|DT_SINGLELINE|DT_MODIFYSTRING|DT_END_ELLIPSIS);
	pDC->SelectObject(oldFont);
}
catch(...){
#ifdef _DEBUG
	Tips("void CHotKeyListView::DrawRow(CDC* pDC, int nItem, CRect rect, COLORREF textColor, COLORREF bgColor, DWORD data)");
#endif
}
}

/*
void CHotKeyListView::OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CListCtrl& list = GetListCtrl();
	for(int index=0; index<list.GetItemCount(); index++){
		DWORD data = list.GetItemData(index);
		if(!data)
			continue;
		HotKeyInfo* info = (HotKeyInfo*)data;
		free(info);		
	}
	*pResult = 0;
}
*/

void CHotKeyListView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;

	CListCtrl& list = GetListCtrl();
try{
	DWORD data = list.GetItemData(pNMListView->iItem);
	if(!data)
		return;
	HotKeyInfo* info = (HotKeyInfo*)data;
	free(info);		
}
catch(...){
#ifdef _DEBUG
	Tips("void CHotKeyListView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) ");
#endif
}
}

void CHotKeyListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
/*
try{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0)
		return;

	CListCtrl& list = GetListCtrl();
	DWORD data = list.GetItemData(pNMListView->iItem);
	if(!data)
		return;

	HotKeyInfo* info = (HotKeyInfo*)data;
	pMainWnd->m_MainBar.Search(info->key);
}
catch(...){
#if _DEBUG
	Tips("void CHotKeyListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)");
#endif
}
*/
}

void CHotKeyListView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CListView::OnMouseMove(nFlags, point);
	m_mouseOn = GetListCtrl().HitTest(point);
	if(m_mouseOn<0){
		m_mouseOn_old = -1;
		return;
	}

	if(m_mouseOn != m_mouseOn_old){
		GetListCtrl().RedrawItems(GetListCtrl().GetTopIndex(),
			GetListCtrl().GetTopIndex()+GetListCtrl().GetCountPerPage());
		m_mouseOn_old = m_mouseOn;
	}
}

void CHotKeyListView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
try{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0)
		return;

	CListCtrl& list = GetListCtrl();
	DWORD data = list.GetItemData(pNMListView->iItem);
	if(!data)
		return;
    //zhanganzhan on 20081024
//     if(gbOnSearching==TRUE)
//     {
//         MessageBox("正在进行搜索，请稍候...");
//         return;
//     }
//     gbOnSearching = TRUE;
	HotKeyInfo* info = (HotKeyInfo*)data;
	pMainWnd->m_MainBar.Search(info->key);
}
catch(...){
#if _DEBUG
	Tips("void CHotKeyListView::OnClick(NMHDR* pNMHDR, LRESULT* pResult)");
#endif
}

}

void CHotKeyListView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	RefreshHotkey();
	
	*pResult = 0;
}

void CHotKeyListView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

// 	if(m_hotDlg.GetSafeHwnd()){
// 		if(m_hotDlg.IsWindowVisible())
// 			m_hotDlg.MoveWindow(0,303,cx,cy);
//	}
}

BOOL CHotKeyListView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
BOOL CHotKeyListView::Create(CWnd *pWnd,CRect rect)
{
   return CView::Create(NULL, NULL,  WS_VISIBLE|WS_BORDER,rect, pWnd, 1234);
	return TRUE;
}


void CHotKeyListView::RefreshHotkey()
{
	thGetKey = AfxBeginThread(_GetHotKeyWord, this);
	thGetKey->m_bAutoDelete = TRUE;
	thGetKey->ResumeThread();
}
