// MainListView.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "MainListView.h"
#include "base64.h"
#include "MainFrm.h"
#include "lib/image/xImage.h"
#include "HttpDownLoadDlg.h"
#include "SelPathDlg.h"
#include "sortclass.h"
#include "DialogComments.h"
#include "DialogSMS.h"
#include "DialogMail.h"
#include "sms.h"
#include "ForJmail.h"
#include "HttpUtl.h"
// #import  "jmail.dll"
// using namespace jmail;

//added by zuoss 2008.07.31
#include <vector>
#include <string>
#include <map>
using namespace std;
//end add

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainListView
extern BOOL offline;
extern char UserName[50];
extern char AspServer[1024];
extern char JspServer[1024];
extern char CurrentPath[_MAX_PATH];
extern _ConnectionPtr m_pConnection;
extern int page;       //20080711

extern bool gbOnSearching;
CStdioFile cashfile;//20080801
char cashpath[1024] = {0};//20080822  add by Lins 

extern CMainFrame* pMainWnd;
extern string RequestMail;
CMainListView* pView = NULL;

bool isOpen_makebook = 0; //��������������۵�״̬
IMPLEMENT_DYNCREATE(CMainListView, CListView)

CMainListView::CMainListView()
{
	style = style_list;
	hit_index = -1;
	orderstyle = order_by_isactive;
	basc=TRUE;
	finishedSearchCount = 0;
	forward = 0;
	isShareFlag = FALSE;//��ʼδ�������
	sharePage = 0;
}

CMainListView::~CMainListView()
{
}


BEGIN_MESSAGE_MAP(CMainListView, CListView)
//{{AFX_MSG_MAP(CMainListView)
ON_WM_SIZE()
ON_WM_CREATE()
ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
ON_MESSAGE(WM_TASKFINISHED, OnTaskFinished)
ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
ON_WM_DESTROY()
ON_MESSAGE(WM_THREAD_FINISHED, OnThreadFinished)
ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
ON_COMMAND(ID_MENUITEM_READ, OnMenuitemRead)
ON_COMMAND(ID_MENUITEM_MOVE, OnMenuitemMove)
ON_COMMAND(ID_MENUITEM_COPY, OnMenuitemCopy)
ON_COMMAND(ID_MENUITEM_DELETE, OnMenuitemDelete)
ON_COMMAND(ID_MENUITEM_OPENFOLDER, OnMenuitemOpenfolder)
ON_COMMAND(ID_MENUITEM_SYN, OnMenuitemSyn)
ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
ON_COMMAND(ID_MENUITEM_DOWNLOAD, OnMenuitemDownload)
ON_COMMAND(ID_MENUITEM_ORDER, OnMenuitemOrder)
ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
ON_COMMAND(ID_MENUITEM_ISACTIVE, OnOrderIsactive)
ON_COMMAND(ID_MENUITEM_TITLE, OnOrderTitle)
ON_COMMAND(ID_MENUITEM_FILETYPE, OnOrderFiletype)
ON_COMMAND(ID_MENUITEM_DOWNTIME, OnOrderDowntime)
ON_COMMAND(ID_MENUITEM_CREATEDATE, OnOrderCreatedate)
ON_COMMAND(ID_MENUITEM_SHAREUSER, OnOrderShareuser)
ON_COMMAND(ID_MENUITEM_SIZE, OnMenuitemSize)
ON_NOTIFY_REFLECT(HDN_ITEMCHANGED, OnItemchanged)
ON_WM_MOUSEMOVE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainListView drawing

void CMainListView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMainListView diagnostics

#ifdef _DEBUG
void CMainListView::AssertValid() const
{
	CListView::AssertValid();
}

void CMainListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainListView message handlers

void CMainListView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	if(m_plist->GetSafeHwnd() && style == style_list){
		m_plist->SetColumnWidth(0, cx-10);
		m_plist->RedrawItems(m_plist->GetTopIndex(),
			m_plist->GetTopIndex()+m_plist->GetCountPerPage());
	}
}

int CMainListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	pView = this;
	
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	//add by dzh 080801
	HINSTANCE hInstResource;
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDC_CURSOR), RT_GROUP_CURSOR);
	m_hand = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(IDC_CURSOR),
		IMAGE_CURSOR, 0, 0, 0);
	
	m_plist = &GetListCtrl();
	
	char iniPath[_MAX_PATH] = {0};
	char val[10] = {0};
	sprintf_s(iniPath, 10,  "%s\\system\\option.ini", CurrentPath);
	GetPrivateProfileString(UserName,"list-style",NULL,val,10,iniPath);
	if(style_list == atoi(val))
		ChangeToList();
	else
		ChangeToReport();
	quit = FALSE;
	return 0;
}

void CMainListView::SetType(int type)
{
	showtype = type;
}

void CMainListView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
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
		if(Is_FindList == showtype)
		{
			ShowFindListItem(pLVCD);
			*pResult = CDRF_SKIPDEFAULT;
		}
		else if(style==style_list)
		{
			switch(showtype)
			{
			case Is_BookList:
				ShowHanlinBookItem(pLVCD);
				*pResult = CDRF_SKIPDEFAULT;
				break;
			case Is_OrdList:
				ShowOrderListItem(pLVCD);
				*pResult = CDRF_SKIPDEFAULT;
				break;
			case Is_RecentlyList:
			case Is_FileList:
			case Is_CollectList:
			case Is_SDDiskList:
			case Is_PushList:
				ShowFileListItem(pLVCD);
				*pResult = CDRF_SKIPDEFAULT;
				break;
			case Is_ShareList:
				ShowShareListItem(pLVCD);
				*pResult = CDRF_SKIPDEFAULT;
				break;
			case Is_NetBookList:
				ShowNetBookListItem(pLVCD);
				*pResult = CDRF_SKIPDEFAULT;
				break;
			default:
				*pResult = CDRF_DODEFAULT;//CDRF_SKIPDEFAULT;
				break;
			}
		}
		else
			*pResult = CDRF_DODEFAULT;
	}
}

void CMainListView::ShowShareListItem(NMLVCUSTOMDRAW *pLVCD)
{
	try{
		int nSubItem = pLVCD->iSubItem;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		ShareInfo* info = (ShareInfo*)m_plist->GetItemData(nItem);
		if(!info)
			return;
		
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		m_plist->GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
		
		if(info->sid == syn_moveup || info->sid == syn_movedown)
			DrawMovePageClick(pDC, rect, info->sid);
		else
			DrawShareItem(pDC, rect, info, nItem);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::ShowShareListItem(NMLVCUSTOMDRAW *pLVCD)");
#endif
	}
}

void CMainListView::DrawShareItem(CDC *pDC, CRect rect, ShareInfo* info, int nItem)
{
	try{
		DrawFileItemIcon(pDC, rect, nItem);			//������
		DrawShareItemText(pDC, rect, info, nItem);	//
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::DrawShareItem(CDC *pDC, CRect rect, ShareInfo* info, int nItem)");
#endif
	}
}

void CMainListView::DrawShareItemText(CDC* pDC, CRect rect, ShareInfo* info, int nItem)
{
	UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_LEFT;;
	CFont ftitle;
	ftitle.CreateFont(18, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_SEMIBOLD, // nWeight 
		FALSE, // bItalic 
		TRUE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("����")); // lpszFac			Monotype Corsiva
	
	CFont fcont;
	fcont.CreateFont(14, // nHeight 
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
		_T("����")); // lpszFac			Monotype Corsiva
	
	rect.left += THUMBNAIL_WIDTH_LIST+10;
	
	CString outStr;
	CFont* oldFont = pDC->SelectObject(&ftitle);
	pDC->SetTextColor(RGB(1,52,220));
	pDC->TextOut(rect.left+20, rect.top+10, info->title);
	CSize sz = pDC->GetOutputTextExtent(info->title);
	
	CDC memDC;
	CBitmap bmp;
	if(info->isAlive)
	{
		bmp.LoadBitmap(IDB_BITMAP_BUTTON_BROWSE);
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmp);
		pDC->BitBlt(rect.left+20+sz.cx+20, rect.top+10, 31, 13, &memDC, 0, 0, SRCCOPY);
		bmp.DeleteObject();
		memDC.DeleteDC();
		CRect bwrRect(rect.left+20+sz.cx+20, rect.top+10, rect.left+20+sz.cx+20+31, rect.top+10+13);
		btn1Map.SetAt(nItem,bwrRect);
		
		bmp.LoadBitmap(IDB_BITMAP_BUTTON_DOWNLOAD);
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmp);
		pDC->BitBlt(rect.left+20+sz.cx+60, rect.top+10, 31, 13, &memDC, 0, 0, SRCCOPY);
		bmp.DeleteObject();
		memDC.DeleteDC();
		CRect dwnRect(rect.left+20+sz.cx+60, rect.top+10, rect.left+20+sz.cx+60+31, rect.top+10+13);
		btn2Map.SetAt(nItem,dwnRect);
	}
	else
	{
		bmp.LoadBitmap(IDB_BITMAP_BUTTON_BROWSEH);
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmp);
		pDC->BitBlt(rect.left+20+sz.cx+20, rect.top+10, 31, 13, &memDC, 0, 0, SRCCOPY);
		bmp.DeleteObject();
		memDC.DeleteDC();
		
		bmp.LoadBitmap(IDB_BITMAP_BUTTON_DOWNLOADH);
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmp);
		pDC->BitBlt(rect.left+20+sz.cx+60, rect.top+10, 31, 13, &memDC, 0, 0, SRCCOPY);
		bmp.DeleteObject();
		memDC.DeleteDC();
	}
	
	//add by dongzhenhua 20080508
	//���½��Ի��������� ����Ⱥ��
	bmp.LoadBitmap(IDB_BITMAP_BUTTON_GROUP);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp);
	pDC->BitBlt(rect.left+20+sz.cx+100, rect.top+10, 31, 13, &memDC, 0, 0, SRCCOPY);
    bmp.DeleteObject();
    memDC.DeleteDC();
    CRect gwnRect(rect.left+20+sz.cx+100, rect.top+10, rect.left+20+sz.cx+100+31, rect.top+10+13);
    btn3Map.SetAt(nItem,gwnRect);
	
	//add by dongzhenhua 20081118
	//����ʼ�����ת������
	bmp.LoadBitmap(IDB_BITMAP_BUTTON_EMAIL_R);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp);
	pDC->BitBlt(rect.left+20+sz.cx+140, rect.top+10, 55, 13, &memDC, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memDC.DeleteDC();
	CRect emailRect(rect.left+20+sz.cx+140, rect.top+10,rect.left+20+sz.cx+140+55, rect.top+10+13);
	btn4Map.SetAt(nItem,emailRect);
	
	pDC->SelectObject(&fcont);
	pDC->SetTextColor(0x9E9E9E);
	CString filelen;
	if (info->filesize==0)
	{
		filelen="δ֪";
	}
	else if(info->filesize>1024*1024){
		filelen.Format("%d.%02d MB", info->filesize/(1024*1024),  (info->filesize%(1024*1024))/1024/10);
	}
	else{
		filelen.Format("%d.%02d KB", info->filesize/1024,info->filesize%1024/10);
	}
	outStr.Format("��ǰ״̬:%s   �ṩ��:%s   ����Ⱥ��:%s   �ļ�����:%s  ����:%d  ����:%d  ��С:%s", 
		info->isAlive?"����":"����", info->UserName,
		info->group, GetFileExtentName(info->filetype), 
		info->DownTimes, info->CommentTimes,filelen);
	pDC->TextOut(rect.left+20, rect.top+32, outStr);
	pDC->SetTextColor(0);
	CRect arect(rect.left+20, rect.top+50, rect.left+700, rect.top+85);
	outStr.Format("���ݼ��:%s", info->intro);
	pDC->DrawText(outStr, outStr.GetLength(), arect, DT_WORDBREAK|DT_TOP|DT_LEFT);
	pDC->SelectObject(oldFont);
	
	ftitle.DeleteObject();
	fcont.DeleteObject();
}

void CMainListView::ShowFindListItem(NMLVCUSTOMDRAW *pLVCD)
{
	try{
		int nSubItem = pLVCD->iSubItem;
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec);
		FINDINFO* info = (FINDINFO*)m_plist->GetItemData(nItem);
		if(!info)
			return;
		if(nSubItem==0){
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CRect rect;
			m_plist->GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
			switch(info->src){
			case src_msg:
				DrawFindListHead(pDC, rect, info);
				break;
			case src_movepage_top:
			case src_movepage_bottom:
				DrawMovePageClick(pDC, rect, info->src);
				break;
			default:
				DrawFindResult(pDC, rect, info, nItem);
				break;
			}
		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::ShowFindListItem(NMLVCUSTOMDRAW *pLVCD)");
#endif
	}
}

void CMainListView::ShowFileListItem(NMLVCUSTOMDRAW *pLVCD)
{
	try{
		int nSubItem = pLVCD->iSubItem;
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec);
		
		DiskBookInfo* info = (DiskBookInfo*)m_plist->GetItemData(nItem);
		if(!info)
			return;
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		m_plist->GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
		if(info->syn == syn_moveup || info->syn == syn_movedown)
			DrawMovePageClick(pDC, rect, info->syn);
		else
		{
			DrawFileItem(pDC, rect, info, nItem);
		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::ShowFindListItem(NMLVCUSTOMDRAW *pLVCD)");
#endif
	}
}

void CMainListView::ShowNetBookListItem(NMLVCUSTOMDRAW *pLVCD)
{
	try
	{
		int nSubItem = pLVCD->iSubItem;
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec);
		
		FINDINFO* info = (FINDINFO*)m_plist->GetItemData(nItem);
		if(!info)
			return;
		if(nSubItem==0)
		{
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CRect rect;
			m_plist->GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
			/*switch(info->src){
			case src_msg:
			DrawFindListHead(pDC, rect, info);
			break;
			case src_movepage_top:
			case src_movepage_bottom:
			DrawMovePageClick(pDC, rect, info->src);
			break;
			default:
			DrawFindResult(pDC, rect, info, nItem);
			break;
		}*/
			DrawHanlinBookResultText(pDC, rect, info, nItem);
		}
	}
	catch(...)
	{
#ifdef _DEBUG
		Tips("void CMainListView::ShowNetBookListItem(NMLVCUSTOMDRAW *pLVCD)");
#endif
	}
}

void CMainListView::DrawMovePageClick(CDC* pDC, CRect rect, int src)
{
	try{
		CFont ftitle;
		ftitle.CreateFont(14, // nHeight 
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
			_T("����")); // lpszFac			Monotype Corsiva
					   /*	CFont ftitle1;
					   ftitle1.CreateFont(14, // nHeight 
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
					   _T("����")); // lpszFac			Monotype Corsiva
		*/
		//logo
		CBitmap bmp;
		CDC memDC;
		bmp.LoadBitmap(IDB_SEARCHLIST_ICON);
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmp);
		pDC->BitBlt(rect.left+50, rect.top+20, 131, 50, &memDC, 0, 0, SRCCOPY);
		bmp.DeleteObject();
		memDC.DeleteDC();
		
		CFont* oldFont = pDC->SelectObject(&ftitle);
		CString str;
		str.Format("������%d����¼ ��%dҳ,��ǰ��%dҳ", totalCount, totalPage, page);
		//	str.Format("������%d����¼ ��%dҳ,��ǰ��%dҳ", totalCount, totalPage1, page1+1);
		pDC->TextOut(rect.left+200, rect.top+50, str);
		
		//pDC->SelectObject(&ftitle1);
		if(page>1){
			pDC->SetTextColor(0xff0000);
			if(src==src_movepage_top || src==syn_moveup)
				up1.SetRect(rect.left+400, rect.top+40,rect.left+445, rect.top+85);
			else
				up2.SetRect(rect.left+400, rect.top+40,rect.left+445, rect.top+85);
		}
		else{
			pDC->SetTextColor(0);
			if(src == src_movepage_top || src==syn_moveup)
				up1.SetRectEmpty();
			else
				up2.SetRectEmpty();
		}
		
		//added by zuoss 20080805
		if ((page1 == 0))
		{
			pDC->SetTextColor(0);
		}
		else
		{
			pDC->SetTextColor(0xff0000);
		}
		//end add
		
		pDC->TextOut(rect.left+450, rect.top+50, "��һҳ");		
		up2.SetRect(rect.left+450, rect.top+50, rect.left+490, rect.top+75);
		if(page<totalPage){
			pDC->SetTextColor(0xff0000);
			if(src == src_movepage_top || src==syn_moveup)
				down1.SetRect(rect.left+475, rect.top+40,rect.left+520, rect.top+85);
			else
				down2.SetRect(rect.left+475, rect.top+40,rect.left+520, rect.top+85);
		}
		else{
			pDC->SetTextColor(0);
			if(src == src_movepage_top || src==syn_moveup)
				down1.SetRectEmpty();
			else
				down2.SetRectEmpty();
		}
		
		//added by zuoss 20080805
		if (((page1+1) == totalPage1))
		{
			pDC->SetTextColor(0);
		}
		else
		{
			pDC->SetTextColor(0xff0000);
		}
		//end add
		
		pDC->TextOut(rect.left+495, rect.top+50, "��һҳ");	
		down2.SetRect(rect.left+495, rect.top+50, rect.left+535, rect.top+75);
		pDC->SetTextColor(0);
		//	pDC->TextOut(rect.left+540, rect.top+40, "������      ҳ");
		
		if(src == src_movepage_top || src==syn_moveup)
		{
			jumpEditRect1.SetRect(rect.left+588, rect.top+42, rect.left+626, rect.top+61);
			pDC->Rectangle(&jumpEditRect1);
		}
		else{
			jumpEditRect2.SetRect(rect.left+588, rect.top+42, rect.left+626, rect.top+61);
			pDC->Rectangle(&jumpEditRect2);
		}
		
		if(src == src_movepage_bottom){
			pDC->SetTextColor(0xff0000);
			pDC->TextOut(rect.right-180, rect.top+78, "����Ȩ��������");
			zzqbh.SetRect(rect.right-180, rect.top+78, rect.right-75, rect.top+93);
			pDC->TextOut(rect.left+495, rect.top+40, "��һҳ");	
			down2.SetRect(rect.left+495, rect.top+40, rect.left+535, rect.top+75);
		}
		pDC->SelectObject(oldFont);
		ftitle.DeleteObject();
		//	ftitle1.DeleteObject();
}
catch(...){
#ifdef _DEBUG
	Tips("void CMainListView::DrawMovePageClick(CDC* pDC, CRect rect, int src)");
#endif
}
}
void CMainListView::DrawOrderListHead(CDC* pDC, CRect rect, OrderDetailInfo* info)
{
	CFont ftitle;
	ftitle.CreateFont(16, // nHeight 
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
		_T("����")); // lpszFac			Monotype Corsiva
	
	//logo
	CBitmap bmp;
	CDC memDC;
	bmp.LoadBitmap(IDB_SEARCHLIST_ICON);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp);
	pDC->BitBlt(rect.left+30, rect.top+20, 131, 50, &memDC, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memDC.DeleteDC();
	
	CFont* oldfont = pDC->SelectObject(&ftitle);
	pDC->TextOut(rect.left+200,rect.top+50, "ȫ������");
	pDC->TextOut(rect.left+300,rect.top+50, "ȫ���˶�");
	pDC->TextOut(rect.left+400,rect.top+50, "��ת");
	
	//�趨�����Ӧ����
	CRect order(rect.left+200,rect.top+50,rect.left+250,rect.top+70);
	CRect unorder(rect.left+300,rect.top+50,rect.left+350,rect.top+70);
	CRect reverse(rect.left+400,rect.top+50,rect.left+450,rect.top+70);
	orderAll = order;
	unOrderAll = unorder;
	reverseAll = reverse;
	
	//	int i = 0;
	//	orderAllMap.SetAt(i,orderall);
	//	unOrderAllMap.SetAt(i,unorderall);
	//	reverseMap.SetAt(i,reverse);
}

//zuoss 20090515
void CMainListView::DrawHanlinBookHead(CDC* pDC, CRect rect, FINDINFO* info)
{
	CFont ftitle;
	ftitle.CreateFont(17, // nHeight 
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
		_T("����")); // lpszFac			Monotype Corsiva
	
	//logo
	CBitmap bmp;
	CDC memDC;
	bmp.LoadBitmap(IDB_SEARCHLIST_ICON);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp);
	pDC->BitBlt(rect.left+30, rect.top+20, 131, 50, &memDC, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memDC.DeleteDC();
	
	//zuoss 090527 ��ҳ��ҳ
	CString str;
	str.Format("��%d����¼  ��%dҳ  ��ǰ��%dҳ", totalCount, totalPage, page);
	pDC->TextOut(rect.left+200, rect.top+50, str);
	if ((page == 1))
	{
		pDC->SetTextColor(0);
	}
	else
	{
		pDC->SetTextColor(0xff0000);
	}	
	pDC->TextOut(rect.left+400+40, rect.top+50, "��һҳ");	
    up1.SetRect(rect.left+400+40, rect.top+50, rect.left+440+40, rect.top+75);
	
	if(page < totalPage)
	{
		pDC->SetTextColor(0xff0000);
		down1.SetRect(rect.left+475, rect.top+50,rect.left+520, rect.top+64);
	}
	else
	{
		pDC->SetTextColor(0);
		down1.SetRectEmpty();
	}
	pDC->TextOut(rect.left+475+10, rect.top+50, "��һҳ");
	down1.SetRect(rect.left+475+10,rect.top+50,rect.left+485+40,rect.top+75);
	
	ftitle.DeleteObject();
	//end
}
//end
void CMainListView::DrawFindListHead(CDC *pDC, CRect rect, FINDINFO* info)
{
	try{
		//rect.left += 75;
		CFont ftitle;
		ftitle.CreateFont(14, // nHeight 
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
			_T("����")); // lpszFac			Monotype Corsiva
		//logo
		CBitmap bmp;
		CDC memDC;
		bmp.LoadBitmap(IDB_SEARCHLIST_ICON);
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmp);
		pDC->BitBlt(rect.left+50, rect.top+10, 131, 50, &memDC, 0, 0, SRCCOPY);
		bmp.DeleteObject();
		memDC.DeleteDC();
		
		CFont* oldfont = pDC->SelectObject(&ftitle);
		//�������
		pDC->TextOut(rect.left+200+50,rect.top+20, info->title);
		
		//added by zuoss 2008.07.29 
		//�����б�ͷ�ķ�����Ϣ˳��
		
		//add by dzh 080801
		SetRectCurEmpty();
		
		if(pMainWnd->curCondition == Is_NetGroup || pMainWnd->curCondition == Is_NetGroup_Level)
		{
			pDC->SetTextColor(0xff0000);
			pDC->TextOut(rect.left+200,rect.top + 5,"��ݷ���|");
			kjfl.SetRect(rect.left+200,rect.top + 5,rect.left + 260,rect.top + 20);
			
			pDC->TextOut(rect.left+270,rect.top + 5,"��������|");
			duzl.SetRect(rect.left+270,rect.top + 5,rect.left+330,rect.top + 20);
			
			pDC->TextOut(rect.left+340,rect.top + 5,"�ļ�����|");
			wjsx.SetRect(rect.left+340,rect.top + 5,rect.left+400,rect.top + 20);
			
			pDC->TextOut(rect.left+410,rect.top + 5,"�м���Ϣ|");
			yjxx.SetRect(rect.left+410,rect.top + 5,rect.left + 470,rect.top + 20);
			
			pDC->TextOut(rect.left+480,rect.top + 5,"�������");
			djhd.SetRect(rect.left+480,rect.top + 5,rect.left + 540,rect.top + 20);
			pDC->SetTextColor(0x0000ff);
			
			kaoshi.SetRectEmpty();
			xiti.SetRectEmpty();
			kejian.SetRectEmpty();
			jiaocai.SetRectEmpty();
			shijian.SetRectEmpty();
			daxiao.SetRectEmpty();
			gongxiangshu.SetRectEmpty();
			wendingdu.SetRectEmpty();
			zuixin.SetRectEmpty();
			zuikuai.SetRectEmpty();
			zuijurenqi.SetRectEmpty();
			zuidazhekou.SetRectEmpty();
			zuidijiage.SetRectEmpty();
			dijiajuan.SetRectEmpty();
			zengpin.SetRectEmpty();
			renqi.SetRectEmpty();
			tuijian.SetRectEmpty();
			jingdianhuigu.SetRectEmpty();
			
			if (curcondition == 1)
			{
				CBitmap  bmps;
				CDC  dc;
				bmps.LoadBitmap(IDB_BITMAP_KJFL);
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(&bmps);
				pDC->BitBlt(rect.left + 196, rect.top + 2, 170, 40, &dc, 0, 0, SRCCOPY);
				bmps.DeleteObject();
				dc.DeleteDC();
				
				pDC->SetTextColor(0xff0000);
				pDC->TextOut(rect.left+200,rect.top + 5,"��ݷ���|");
				kjfl.SetRect(rect.left+200,rect.top + 5,rect.left + 260,rect.top + 20);
				
				pDC->TextOut(rect.left+270,rect.top + 5,"��������|");
				duzl.SetRect(rect.left+270,rect.top + 5,rect.left+330,rect.top + 20);
				
				pDC->TextOut(rect.left+340,rect.top + 5,"�ļ�����|");
				wjsx.SetRect(rect.left+340,rect.top + 5,rect.left+400,rect.top + 20);
				
				pDC->TextOut(rect.left+410,rect.top + 5,"�м���Ϣ|");
				yjxx.SetRect(rect.left+410,rect.top + 5,rect.left + 470,rect.top + 20);
				
				pDC->TextOut(rect.left+480,rect.top + 5,"�������");
				djhd.SetRect(rect.left+480,rect.top + 5,rect.left + 540,rect.top + 20);
				pDC->SetTextColor(0x0000ff);
				
				
				pDC->TextOut(rect.left + 200,rect.top + 25,"����");
				zuixin.SetRect(rect.left + 200,rect.top + 25,rect.left + 230,rect.top + 40);
				
				pDC->TextOut(rect.left + 240,rect.top + 25,"���");
				zuikuai.SetRect(rect.left + 240,rect.top + 5,rect.left + 270,rect.top + 40);
				
				pDC->TextOut(rect.left + 280,rect.top + 25,"�������");
				zuijurenqi.SetRect(rect.left + 280,rect.top + 25,rect.left + 340,rect.top + 40);
			}
			
			if (curcondition == 2)
			{
				CBitmap  bmps;
				CDC  dc;
				bmps.LoadBitmap(IDB_BITMAP_DWZL);
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(&bmps);
				pDC->BitBlt(rect.left + 258, rect.top + 2, 170, 40, &dc, 0, 0, SRCCOPY);
				bmps.DeleteObject();
				dc.DeleteDC();
				
				pDC->SetTextColor(0xff0000);
				
				pDC->TextOut(rect.left+200,rect.top + 5,"��ݷ���|");
				kjfl.SetRect(rect.left+200,rect.top + 5,rect.left + 260,rect.top + 20);
				
				pDC->TextOut(rect.left+270,rect.top + 5,"��������|");
				duzl.SetRect(rect.left+270,rect.top + 5,rect.left + 330,rect.top + 20);
				
				pDC->TextOut(rect.left+340,rect.top + 5,"�ļ�����|");
				wjsx.SetRect(rect.left+340,rect.top + 5,rect.left+400,rect.top + 20);
				
				pDC->TextOut(rect.left+410,rect.top + 5,"�м���Ϣ|");
				yjxx.SetRect(rect.left+410,rect.top + 5,rect.left + 470,rect.top + 20);
				
				pDC->TextOut(rect.left+480,rect.top + 5,"�������");
				djhd.SetRect(rect.left+480,rect.top + 5,rect.left + 540,rect.top + 20);
				
				pDC->SetTextColor(0x0000ff);
				
				pDC->TextOut(rect.left + 260,rect.top + 25,"�̲�");
				jiaocai.SetRect(rect.left + 260,rect.top + 25,rect.left + 290,rect.top + 40);
				
				pDC->TextOut(rect.left + 300,rect.top + 25,"�μ�");
				kejian.SetRect(rect.left + 300,rect.top + 25,rect.left + 330,rect.top + 40);
				
				pDC->TextOut(rect.left + 340,rect.top + 25,"ϰ��");
				xiti.SetRect(rect.left + 340,rect.top + 25,rect.left + 370,rect.top + 40);
				
				pDC->TextOut(rect.left + 380,rect.top + 25,"����");
				kaoshi.SetRect(rect.left + 380,rect.top + 25,rect.left + 410,rect.top + 40);
				
			}
			
			if (curcondition==3)
			{
				
				CBitmap  bmps;
				CDC  dc;
				bmps.LoadBitmap(IDB_BITMAP_WJSX);
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(&bmps);
				pDC->BitBlt(rect.left + 328, rect.top + 2, 240, 40, &dc, 0, 0, SRCCOPY);
				bmps.DeleteObject();
				dc.DeleteDC();
				
				pDC->SetTextColor(0xff0000);
				
				pDC->TextOut(rect.left+200,rect.top + 5,"��ݷ���|");
				kjfl.SetRect(rect.left+200,rect.top + 5,rect.left + 260,rect.top + 20);
				
				pDC->TextOut(rect.left+270,rect.top + 5,"��������|");
				duzl.SetRect(rect.left+270,rect.top + 5,rect.left + 330,rect.top + 20);
				
				pDC->TextOut(rect.left+340,rect.top + 5,"�ļ�����|");
				wjsx.SetRect(rect.left+340,rect.top + 5,rect.left + 400,rect.top + 20);
				
				pDC->TextOut(rect.left+410,rect.top + 5,"�м���Ϣ|");
				yjxx.SetRect(rect.left+410,rect.top + 5,rect.left + 470,rect.top + 20);
				
				pDC->TextOut(rect.left+480,rect.top + 5,"�������");
				djhd.SetRect(rect.left+480,rect.top + 5,rect.left + 540,rect.top + 20);
				
				pDC->SetTextColor(0x0000ff);
				
				pDC->TextOut(rect.left + 330,rect.top + 25,"ʱ��");
				shijian.SetRect(rect.left + 330,rect.top + 25,rect.left + 360,rect.top + 40);
				
				pDC->TextOut(rect.left + 370,rect.top + 25,"��С");
				daxiao.SetRect(rect.left + 370,rect.top + 25,rect.left + 400,rect.top + 40);
				
				pDC->TextOut(rect.left + 410,rect.top + 25,"������");
				gongxiangshu.SetRect(rect.left + 410,rect.top + 25,rect.left + 455,rect.top + 40);
				
				pDC->TextOut(rect.left + 465,rect.top + 25,"�ȶ���");
				wendingdu.SetRect(rect.left + 465,rect.top + 25,rect.left + 510,rect.top + 40);
			}
			
			
			if (curcondition==4)
			{
				CBitmap  bmps;
				CDC  dc;
				bmps.LoadBitmap(IDB_BITMAP_YJXX);
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(&bmps);
				pDC->BitBlt(rect.left + 310, rect.top + 2, 250, 40, &dc, 0, 0, SRCCOPY);
				bmps.DeleteObject();
				dc.DeleteDC();
				
				pDC->SetTextColor(0xff0000);
				
				pDC->TextOut(rect.left+200,rect.top + 5,"��ݷ���|");
				kjfl.SetRect(rect.left+200,rect.top + 5,rect.left + 260,rect.top + 20);
				
				pDC->TextOut(rect.left+270,rect.top + 5,"��������|");
				duzl.SetRect(rect.left+270,rect.top + 5,rect.left + 330,rect.top + 20);
				
				pDC->TextOut(rect.left+340,rect.top + 5,"�ļ�����|");
				wjsx.SetRect(rect.left+340,rect.top + 5,rect.left+400,rect.top + 20);
				
				pDC->TextOut(rect.left+410,rect.top + 5,"�м���Ϣ|");
				yjxx.SetRect(rect.left+410,rect.top + 5,rect.left + 470,rect.top + 20);
				
				pDC->TextOut(rect.left+480,rect.top + 5,"�������");
				djhd.SetRect(rect.left+480,rect.top + 5,rect.left + 540,rect.top + 20);
				
				pDC->SetTextColor(0x0000ff);
				
				pDC->TextOut(rect.left + 315,rect.top + 25,"����ۿ�");
				zuidazhekou.SetRect(rect.left + 315,rect.top+25,rect.left + 375,rect.top + 40);
				
				pDC->TextOut(rect.left + 385,rect.top + 25,"��ͼ۸�");
				zuidijiage.SetRect(rect.left + 385,rect.top + 25,rect.left + 445,rect.top + 40);
				
				pDC->TextOut(rect.left + 455,rect.top + 25,"�ּ۾�");
				dijiajuan.SetRect(rect.left + 455,rect.top + 25,rect.left + 500,rect.top + 40);
				
				pDC->TextOut(rect.left + 510,rect.top + 25,"��Ʒ");
				zengpin.SetRect(rect.left + 510,rect.top + 25,rect.left + 540,rect.top + 40);
			}
			if (curcondition==5)
			{
				CBitmap  bmps;
				CDC  dc;
				bmps.LoadBitmap(IDB_BITMAP_DJHD);
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(&bmps);
				pDC->BitBlt(rect.left + 355, rect.top + 2, 250, 40, &dc, 0, 0, SRCCOPY);
				bmps.DeleteObject();
				dc.DeleteDC();
				
				pDC->SetTextColor(0xff0000);
				
				pDC->TextOut(rect.left+200,rect.top + 5,"��ݷ���|");
				kjfl.SetRect(rect.left+200,rect.top + 5,rect.left + 260,rect.top + 20);
				
				pDC->TextOut(rect.left+270,rect.top + 5,"��������|");
				duzl.SetRect(rect.left+270,rect.top + 5,rect.left + 330,rect.top + 20);
				
				pDC->TextOut(rect.left+340,rect.top + 5,"�ļ�����|");
				wjsx.SetRect(rect.left+340,rect.top + 5,rect.left+400,rect.top + 20);
				
				pDC->TextOut(rect.left+410,rect.top + 5,"�м���Ϣ|");
				yjxx.SetRect(rect.left+410,rect.top + 5,rect.left + 470,rect.top + 20);
				
				pDC->TextOut(rect.left+480,rect.top + 5,"�������");
				djhd.SetRect(rect.left+480,rect.top + 5,rect.left + 540,rect.top + 20);
				
				pDC->SetTextColor(0x0000ff);
				
				pDC->TextOut(rect.left + 360,rect.top + 25,"����");
				renqi.SetRect(rect.left + 360,rect.top + 25,rect.left + 390,rect.top + 40);
				
				pDC->TextOut(rect.left + 400,rect.top + 25,"����");
				zuixin.SetRect(rect.left + 400,rect.top + 25,rect.left + 430,rect.top + 40);
				
				pDC->TextOut(rect.left + 440,rect.top + 25,"�Ƽ�");
				tuijian.SetRect(rect.left + 440,rect.top + 25,rect.left + 470,rect.top + 40);
				
				pDC->TextOut(rect.left + 480,rect.top + 25,"����ع�");
				jingdianhuigu.SetRect(rect.left + 480,rect.top + 25,rect.left + 540,rect.top + 40);
			}
	}
	//end add
	
	pDC->SetTextColor(0xff0000);
	pDC->TextOut(rect.right-60, rect.top+5, "��������");
	mianze.SetRect(rect.right-60, rect.top+5, rect.right, rect.top+20);
	
	CString str;
	//	str.Format("��%dҳ,��ǰ��%dҳ", totalPage+1, page);
	//	str.Format("������%d����¼ ��%dҳ,��ǰ��%dҳ", totalCount, totalPage+1, page);
	totalPage1 = totalCount/20+1;
	str.Format("������%d����¼ ��%dҳ,��ǰ��%dҳ", totalCount, totalPage1, page1+1);
	pDC->TextOut(rect.left+200, rect.top+50, str);
	
	//added by zuoss 20080805
	if ((page1 == 0))
	{
		pDC->SetTextColor(0);
	}
	else
	{
		pDC->SetTextColor(0xff0000);
	}
	//end add
	
	pDC->TextOut(rect.left+400+40, rect.top+50, "��һҳ");	
    up1.SetRect(rect.left+400+40, rect.top+50, rect.left+440+40, rect.top+75);
	
	if(page<totalPage){
		pDC->SetTextColor(0xff0000);
		down1.SetRect(rect.left+475, rect.top+50,rect.left+520, rect.top+64);
	}
	else{
		pDC->SetTextColor(0);
		down1.SetRectEmpty();
	}
	
	//added by zuoss 20080805
	if (((page1+1) == totalPage1))
	{
		pDC->SetTextColor(0);
	}
	else
	{
		pDC->SetTextColor(0xff0000);
	}
	//end add
	
	pDC->TextOut(rect.left+475+10, rect.top+50, "��һҳ");
	down1.SetRect(rect.left+475+10,rect.top+50,rect.left+485+40,rect.top+75);
	
	pDC->SetTextColor(0);
	pDC->TextOut(rect.left+540, rect.top+50, "������      ҳ");
	jumpEditRect1.SetRect(rect.left+584, rect.top+48, rect.left+622, rect.top+67);
	pDC->Rectangle(&jumpEditRect1);
	pDC->SelectObject(oldfont);
	
	ftitle.DeleteObject();
	CBrush bgbrush;
	bgbrush.CreateSolidBrush(RGB(0xee,0xee,0xee));
	CBrush forebrush;
	forebrush.CreateSolidBrush(RGB(0x68,0xb5,0x74));
	
	//�±ߵĽ�����
	CRect rectangle(rect.left+50, rect.top+75, rect.right-30, rect.top+88);
	pDC->FillRect(&rectangle, &bgbrush);
	CRect finishRect(rectangle.left, rectangle.top, rectangle.left+rectangle.Width()*strlen(info->abst1)/SEARCHSOURCES, rectangle.bottom);
	pDC->FillRect(&finishRect, &forebrush);
	
	bgbrush.DeleteObject();
	forebrush.DeleteObject();
	
	CFont fsrc;
	fsrc.CreateFont(8, // nHeight 
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
		_T("����")); // lpszFac			Monotype Corsiva
	
	int dmode = pDC->SetROP2(R2_XORPEN);
	/********************************
	for(UINT index=0; index<strlen(info->abst1); index++){
	CRect strRect(rectangle.left+rectangle.Width()*index/SEARCHSOURCES, rectangle.top,rectangle.left+10+rectangle.Width()*(index+1)/SEARCHSOURCES, rectangle.bottom);
	//�趨�������������Ӧ����
	CString str = CheckSrc(info->abst1[index]);
	int num = info->abst1[index];
	headprocess[index].SetAt(num,strRect); 
	CString strTip;
	strTip.Format("%s(%d)", str, *((int*)&(info->abst2[index*sizeof(int)])));
	pDC->DrawText(strTip, &strRect, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	}
	*******************************/
	
	//added by zuoss 20081030
	int totlen = 0;
	for(UINT index = 0; index < strlen(info->abst1); index++)
	{
		CString  str = "";
		str = CheckSrc(info->abst1[index]);
		
		CString strTip;
		strTip.Format("%s(%d)", str, *((int*)&(info->abst2[index*sizeof(int)])));
		//zuoss 20081209 ������������Ϣ��ͳ������Ŀ��
		if(info->abst1[index] == src_share)
		{
			//��xml��ȡ���һ�ε�ֵ���޸�cnt
			
			CTime ct = CTime::GetCurrentTime();	
			int day = ct.GetDay();
			CString count_str = "";
			
			char Tadd[_MAX_PATH] = {0};
			if(pMainWnd->curCondition == Is_NetGroup || pMainWnd->curCondition == Is_NetGroup_Level)
			{
				sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\groupcache\\share\\findres%d_%d_%d.xml",CurrentPath,pMainWnd->curCondition,pMainWnd->curID,day);
			}
			else
			{
				sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\findres14.xml",CurrentPath);
			}
			if(PathFileExists(Tadd))
			{
				MSXML::IXMLDOMDocumentPtr pDoc;
				MSXML::IXMLDOMNodeListPtr pNodeList;
				try
				{
					HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
					if(!SUCCEEDED(hr))
						throw 1;	//xml ����ʧ��
					if(!pDoc->load(Tadd))
						throw 2;	//xml ����ʧ��
					pNodeList = pDoc->selectNodes("find");
					if (pNodeList == NULL)
					{
						throw 3;
					}
					if(pNodeList)
					{
						CSingleLock SingleLock(&mutex);
						SingleLock.Lock();
						if(SingleLock.IsLocked())
						{
							count_str = GetXmlValue(pNodeList, "totalcount", 0);
							strTip.Format("%s(%s)", str, count_str);
							
							char pcount[10] = {0};
							strcpy_s(pcount, 10, count_str);
							int i = atoi(pcount);
							*((int*)&(info->abst2[index*sizeof(int)])) = i;									
						}
						SingleLock.Unlock();
						pNodeList.Release();
					}						
					
				}
				catch(int errnum)
				{
					switch(errnum)
					{
					case 3:		//�Ѹ���
						pNodeList.Release();
					case 2:		//��ʽ����
						pDoc.Release();
						break;
					}
					return;
				}
			}
		}
		//end	
		int textWid = strlen(strTip) * 6 + 8;//ÿ����Դ��ʾ�Ŀ��
		CRect  strRect(rectangle.left + totlen,rectangle.top,rectangle.left + totlen + textWid,rectangle.bottom);
		totlen += textWid;
		//�趨�������������Ӧ����
		int num = info->abst1[index];
		headprocess[index].SetAt(num,strRect); 
		if (totlen > rectangle.Width())
		{
			CRect ect(rectangle.left + totlen - textWid,rectangle.top,rectangle.right,rectangle.bottom);
			pDC->DrawText("...", &ect, DT_VCENTER|DT_RIGHT|DT_SINGLELINE);
			break;
		}
		else
		{
			pDC->DrawText(strTip, &strRect, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
		}
		
	}
	//end add
	
	fsrc.DeleteObject();
	pDC->SetROP2(dmode);
}
catch(...){
#ifdef _DEBUG
	Tips("DrawFindListHead");
#endif
}
}

//zuoss 20090515
void CMainListView::DrawHanlinBookResult(CDC* pDC, CRect rect, FINDINFO*info, int nItem)
{
	try
	{	
		DrawHanlinBookResultText(pDC, rect, info, nItem);
	}
	catch(...)
	{
#ifdef _DEBUG
		Tips("void CMainListView::DrawFindResult(CDC *pDC, CRect rect, FINDINFO* info, int nItem)");
#endif
	}
}
//end

void CMainListView::DrawFindResult(CDC *pDC, CRect rect, FINDINFO* info, int nItem)
{
	try{
		
		DrawFindResultText(pDC, rect, info, nItem);
		DrawFindResultIcon(pDC, rect, info);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::DrawFindResult(CDC *pDC, CRect rect, FINDINFO* info, int nItem)");
#endif
	}
}

void CMainListView::DrawFileItem(CDC *pDC, CRect rect, DiskBookInfo* info, int nItem)
{
	try{
		DrawFileItemIcon(pDC, rect, nItem);
		DrawFileItemText(pDC, rect, info, nItem);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::DrawFindResult(CDC *pDC, CRect rect, FINDINFO* info, int nItem)");
#endif
	}
}

CString CMainListView::CheckSrc(int src)
{
	CString str;
	switch(src){
	case src_local:
		str = "����";
		break;
	case src_baiduwenku:
		str = "�ٶ��Ŀ�";
		break;
	case src_sinaishare:
		str = "���˹���";
		break;
	case src_share:
		str = "����";
		break;
		// 	case src_opencourse:
		// 		str = "OpenCourse";
		// 		break;
	case src_baidu:
		str = "Baidu";
		break;
	case src_baiduentity://zuoss 20081029
		str = "�ٶ�";
		break;
	case src_duxiu://zuoss 20081104
		str = "����";
		break;
	case src_verycd:
		str = "VeryCD";
		break;
		// 	case src_huntmine:
		// 		str = "Huntmine";
		// 		break;
	case src_order:
		str = "��������";
		break;
	case src_readman:
		str = "Readman";
		break;
	case src_istation:
		str = "����";
		break;
		// 	case src_zcom:
		// 		str = "Zcom";
		// 		break;
	case src_sinabook:
		str = "��Դ����";
		break;
	case src_netbook:
		str = "�������";
		break;
// 	case src_mailbook:
// 		str = "������Դ";
// 		break;
	case src_dangdang:
		str = "����";
		break;
	case src_fanshu:
		str = "����";
		break;
	default:
		str = "����";
		break;
	}
	return str;
}

void CMainListView::DrawFindResultIcon(CDC* pDC, CRect rect, FINDINFO* info)
{
	try{
		CPoint pt = rect.TopLeft();
		POINT sPt;
		sPt.x = 0;
		sPt.y = 0;
		SIZE sz;
		sz.cx = THUMBNAIL_WIDTH_LIST;
		if(info->src == src_readman){
			sz.cx = 135;
			sz.cy = THUMBNAIL_HEIGHT_LIST;
			imgList.DrawIndirect(pDC,info->icon,pt,sz,sPt);
			CBitmap bmp;
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_NAIL);
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(pt.x+135/2-10,pt.y+10,20,20,&memDC,0,0,SRCAND);
			bmp.DeleteObject();
			memDC.DeleteDC();
		}
		else{
			sz.cx = THUMBNAIL_WIDTH_LIST;
			sz.cy = THUMBNAIL_HEIGHT_LIST;
			imgList.DrawIndirect(pDC,info->icon,pt,sz,sPt);
		}
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::DrawFindResultIcon(CDC* pDC, CRect rect, FINDINFO* info)");
#endif
	}
}

void CMainListView::DrawFileItemIcon(CDC* pDC, CRect rect, int nIndex)
{
	try{
		CRect rcItem;
		CPoint pt = rect.TopLeft();
		POINT sPt;
		sPt.x = 0;
		sPt.y = 0;
		SIZE sz;
		sz.cx = THUMBNAIL_WIDTH_LIST;
		sz.cy = THUMBNAIL_HEIGHT_LIST;
		imgList.DrawIndirect(pDC,nIndex,pt,sz,sPt);
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::DrawFileItemIcon(CDC* pDC, CRect rect, FINDINFO* info)");
#endif
	}
}

void CMainListView::DrawFileItemText(CDC* pDC, CRect rect, DiskBookInfo* info, int nItem)
{
	UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_LEFT;;
	CFont ftitle;
	ftitle.CreateFont(18, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_SEMIBOLD, // nWeight 
		FALSE, // bItalic 
		TRUE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("����")); // lpszFac			Monotype Corsiva
	
	CFont fcont;
	fcont.CreateFont(14, // nHeight 
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
		_T("����")); // lpszFac			Monotype Corsiva
	
	rect.left += THUMBNAIL_WIDTH_LIST+10;
	
	CString outStr;
	CFont* oldFont = pDC->SelectObject(&ftitle);
	pDC->SetTextColor(RGB(1,52,220));
	pDC->TextOut(rect.left+20, rect.top+10, info->BookName);
	CSize sz = pDC->GetOutputTextExtent(info->BookName);
	
	CDC memDC;
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_BUTTON_READ);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp);
	pDC->BitBlt(rect.left+20+sz.cx+65, rect.top+10, 31, 13, &memDC, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memDC.DeleteDC();
	CRect readRect(rect.left+20+sz.cx+65, rect.top+10, rect.left+20+sz.cx+65+31, rect.top+10+13);
	btn1Map.SetAt(nItem,readRect);
	
	pDC->SelectObject(&fcont);
	pDC->SetTextColor(0x9E9E9E);
	outStr.Format("����:%s    ����ʱ��:%s   �ļ���С:%s    �ļ�����:%s", info->Author, info->CreateDate, info->Size, info->FileType);
	pDC->TextOut(rect.left+20, rect.top+32, outStr);
	pDC->SetTextColor(0);
	pDC->TextOut(rect.left+20, rect.top+50, info->Path);
	
	if(showtype != Is_RecentlyList){
		CString spath = info->Path;
		CString synpath = spath.Left(spath.ReverseFind('.')+1)+"syn";
		
		if(info->syn == syn_yes){
			pDC->TextOut(rect.left+20, rect.top+68, "ͬ��״̬:��ͬ��");
			bmp.LoadBitmap(IDB_BITMAP_BUTTON_SYN_CHK);
			memDC.CreateCompatibleDC(pDC);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.left+20+sz.cx+20, rect.top+10, 40, 13, &memDC, 0, 0, SRCCOPY);
			bmp.DeleteObject();
			memDC.DeleteDC();
		}
		else{
			pDC->TextOut(rect.left+20, rect.top+68, "ͬ��״̬:δͬ��");
			bmp.LoadBitmap(IDB_BITMAP_BUTTON_SYN_EMP);
			memDC.CreateCompatibleDC(pDC);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.left+20+sz.cx+20, rect.top+10, 40, 13, &memDC, 0, 0, SRCCOPY);
			bmp.DeleteObject();
			memDC.DeleteDC();
		}
		CRect synRect(rect.left+20+sz.cx+20, rect.top+10, rect.left+20+sz.cx+20+40, rect.top+10+13);
		btn2Map.SetAt(nItem,synRect);
	}
	
	pDC->SelectObject(oldFont);
	
	ftitle.DeleteObject();
	fcont.DeleteObject();
}
void CMainListView::DrawHanlinBookResultText(CDC * pDC, CRect rect, FINDINFO *info, int nItem)
{
	try
	{
		if (strcmp(info->title,"HanlinHead")==0 )
		{
			IS_istation = TRUE;
			CRect rect;
			m_plist->GetSubItemRect(0, 0, LVIR_BOUNDS, rect);
			DrawHanlinBookHead(pDC,rect,info);
			return;
		}
		else if (strcmp(info->title,"NetBookListHead")==0)
		{
			IS_istation = FALSE;
			CRect rect;
			m_plist->GetSubItemRect(0, 0, LVIR_BOUNDS, rect);
			DrawHanlinBookHead(pDC,rect,info);
			return;
		}
		else
		{
			CBitmap bmp;
			CDC memDC;
			bmp.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
			memDC.CreateCompatibleDC(pDC);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.left, rect.top, 131, 100, &memDC, 0, 0, SRCCOPY);
			bmp.DeleteObject();
			memDC.DeleteDC();
			
			COLORREF crText  = RGB(0, 0, 0);
			COLORREF crBkgnd = RGB(255, 255, 255);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(crBkgnd);
			CFont ftitle;
			ftitle.CreateFont(16, // nHeight 
				0, // nWidth 
				0, // nEscapement 
				0, // nOrientation 
				FW_SEMIBOLD, // nWeight 
				FALSE, // bItalic 
				TRUE, // bUnderline 
				0, // cStrikeOut 
				GB2312_CHARSET, // nCharSet 
				OUT_DEFAULT_PRECIS, // nOutPrecision 
				CLIP_DEFAULT_PRECIS, // nClipPrecision 
				DEFAULT_QUALITY, // nQuality 
				DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
				_T("����")); // lpszFac			Monotype Corsiva
			
			CFont fcont;
			fcont.CreateFont(14, // nHeight 
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
				_T("����")); // lpszFac			Monotype Corsiva
			
			rect.left += THUMBNAIL_WIDTH_LIST+10;
			int offset = 0;
			
			CString outStr;
			CRect titleRect(rect.left+20+offset,rect.top+15,rect.right-100-70-40,rect.top+15+18);
			CFont* oldFont = pDC->SelectObject(&ftitle);
			pDC->SetTextColor(RGB(1,52,220));
			pDC->DrawText(info->title,strlen(info->title),&titleRect,DT_LEFT|DT_SINGLELINE|DT_MODIFYSTRING|DT_END_ELLIPSIS);
			pDC->SelectObject(&fcont);
			pDC->SetTextColor(0x9E9E9E);
			pDC->TextOut(rect.left+20+offset, rect.top+37, info->abst1);
			pDC->SetTextColor(crText);
			CRect arect(rect.left+20+offset, rect.top+55, rect.left+700, rect.top+83);
			outStr.Format("%s", info->abst2);
			pDC->DrawText(outStr,&arect,DT_WORDBREAK|DT_TOP|DT_LEFT);
			pDC->SelectObject(oldFont);
			ftitle.DeleteObject();
			fcont.DeleteObject();
			
			CBitmap bmp1;
			CDC memDC1;
			//�����ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_BROWSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect browseRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
			btn1Map.SetAt(nItem,browseRect);
			
			if (info->src == Is_iType)
			{
				//������Դ��ť
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_EMAIL_R);
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-70, rect.top+8, 58, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
				
				CRect makebookRect(rect.right-100-70, rect.top+8,rect.right-100-70+58,rect.top+8+13);
				btn2Map.SetAt(nItem,makebookRect);
			}
			
			else if (info->src == Is_NetBook)
			{
				//�������鰴ť
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_TRYMAKEWOLF);
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-70, rect.top+8, 58, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
				
				CRect makebookRect(rect.right-100-70, rect.top+8,rect.right-100-70+58,rect.top+8+13);
				btn2Map.SetAt(nItem,makebookRect);
			}
		}
	}
	catch(...)
	{
#ifdef _DEBUG
		Tips("void CMainListView::DrawHanlinBookResultText(CDC *pDC, CRect rect, FINDINFO *info, int nItem)");
#endif
	}
}
//�����������
void CMainListView::DrawFindResultText(CDC *pDC, CRect rect, FINDINFO *info, int nItem)
{
	try{
		COLORREF crText  = RGB(0, 0, 0);
		COLORREF crBkgnd = RGB(255, 255, 255);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(crBkgnd);
		CFont ftitle;
		ftitle.CreateFont(16, // nHeight 
			0, // nWidth 
			0, // nEscapement 
			0, // nOrientation 
			FW_SEMIBOLD, // nWeight 
			FALSE, // bItalic 
			TRUE, // bUnderline 
			0, // cStrikeOut 
			GB2312_CHARSET, // nCharSet 
			OUT_DEFAULT_PRECIS, // nOutPrecision 
			CLIP_DEFAULT_PRECIS, // nClipPrecision 
			DEFAULT_QUALITY, // nQuality 
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
			_T("����")); // lpszFac			Monotype Corsiva
		
		CFont fcont;
		fcont.CreateFont(14, // nHeight 
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
			_T("����")); // lpszFac			Monotype Corsiva
		
		rect.left += THUMBNAIL_WIDTH_LIST+10;
		
		int offset = 0;
		if(info->src == src_readman)
			offset = 30;
		CString outStr;
		CRect titleRect(rect.left+20+offset,rect.top+15,rect.right-100-70-40,rect.top+15+18);
		CFont* oldFont = pDC->SelectObject(&ftitle);
		pDC->SetTextColor(RGB(1,52,220));
		pDC->DrawText(info->title,strlen(info->title),&titleRect,DT_LEFT|DT_SINGLELINE|DT_MODIFYSTRING|DT_END_ELLIPSIS);
		pDC->SelectObject(&fcont);
		pDC->SetTextColor(0x9E9E9E);
		pDC->TextOut(rect.left+20+offset, rect.top+37, info->abst1);
		pDC->SetTextColor(crText);
		CRect arect(rect.left+20+offset, rect.top+55, rect.left+700, rect.top+83);
		//	CRect arect(rect.left+20+offset, rect.top+55, rect.left+350, rect.top+112);//changed by lins 20080611
		outStr.Format("%s", info->abst2);
		pDC->DrawText(outStr,&arect,DT_WORDBREAK|DT_TOP|DT_LEFT);
		pDC->SelectObject(oldFont);
		ftitle.DeleteObject();
		fcont.DeleteObject();
		
		/*
		*	source logo			IDB_USE_TYPE
		*/
		CDC memDC;
		CBitmap bmp;
		memDC.CreateCompatibleDC(pDC);
		if(info->src == src_baiduwenku){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_BDWENKU);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
		}
		else if(info->src == src_sinaishare){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_SINAIASK);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
		}
		else if(info->src == src_share){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_SHARE);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
		}
		// 	else if(info->src == src_opencourse){
		// 		bmp.LoadBitmap(IDB_SEARCHLIST_ICON_OPENCOURSE);
		// 		memDC.SelectObject(&bmp);
		// 		pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
		// 	}
		else if(info->src == src_baidu){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_BAIDU);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
		}
		else if(info->src == src_dangdang){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_DANGDANG);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
		}
		else if(info->src == src_fanshu){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_FANSHU);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
		}
		//zuoss 20081029
		else if(info->src == src_baiduentity){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_BAIDUE);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
		}
		//end
		//zuoss 20081104
// 		else if(info->src == src_mailbook){//modified from src_duxiu.zhanganzhan
// 			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_ISTATION);
// 			memDC.SelectObject(&bmp);
// 			pDC->BitBlt(rect.right-100, rect.top+5, 69, 23, &memDC, 0, 0, SRCCOPY);
// 		}
		//end
		else if(info->src == src_verycd){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_VERYCD);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 53, 25, &memDC, 0, 0, SRCCOPY);
		}
		// 	else if(info->src == src_huntmine){
		// 		bmp.LoadBitmap(IDB_SEARCHLIST_ICON_HUNTMINE);
		// 		memDC.SelectObject(&bmp);
		// 		pDC->BitBlt(rect.right-100, rect.top+5, 66, 25, &memDC, 0, 0, SRCCOPY);
		// 	}
		else if(info->src == src_istation){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_ISTATION);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 53, 24, &memDC, 0, 0, SRCCOPY);
		}
		else if(info->src == src_readman){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_READMAN);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 68, 28, &memDC, 0, 0, SRCCOPY);
		}
		else if(info->src == src_order){
			if(info->abst3[0] == '1' || info->abst3[0] == '3'){
				bmp.LoadBitmap(IDB_SEARCHLIST_ICON_PDFPAPER);
				memDC.SelectObject(&bmp);
				pDC->BitBlt(rect.right-100, rect.top+8, 69, 20, &memDC, 0, 0, SRCCOPY);
			}
			else{
				bmp.LoadBitmap(IDB_SEARCHLIST_ICON_ZCOM);
				memDC.SelectObject(&bmp);
				pDC->BitBlt(rect.right-100, rect.top+10, 70, 17, &memDC, 0, 0, SRCCOPY);
			}
		}
		else if(info->src == src_netbook){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_ISTATION);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+10, 53, 24, &memDC, 0, 0, SRCCOPY);
		}
		else if(info->src == src_sinabook){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_MAKEBOOK);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+10, 37, 25, &memDC, 0, 0, SRCCOPY);
		}
		else if(info->src == src_share){
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_SHARE);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+4, 70, 36, &memDC, 0, 0, SRCCOPY);
		}
		else{
			bmp.LoadBitmap(IDB_SEARCHLIST_ICON_LOCAL);
			memDC.SelectObject(&bmp);
			pDC->BitBlt(rect.right-100, rect.top+5, 77, 16, &memDC, 0, 0, SRCCOPY);
		}
		bmp.DeleteObject();
		memDC.DeleteDC();
		
		
		if(info->src == src_order){
			CBitmap bmp1;
			CDC memDC1;
			
			//���İ�ť
			if(info->src != src_sinabook && info->abst3[0]!='3'){	//�������Ϳ���
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_ORDER);
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
				CRect btnRect(rect.right-100-70-38, rect.top+8, rect.right-100-70-7, rect.top+8+13);
				btn1Map.SetAt(nItem,btnRect);
				
				//added by zuoss 2008.08.04
				//���۰�ť
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-112-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
				//comRect �����Ӧ������ʱδ����Ӧ
				CRect comRect(rect.right-100-112-31, rect.top+8,rect.right-100-112, rect.top+8+13);
				//end add
				
			}
			else if(info->src != src_sinabook && info->abst3[0]=='3'){	//����
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_ADDTOORDER);//��ӵ��������İ�ť
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-70-97, rect.top+8, 90, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
				CRect addRect(rect.right-100-70-97, rect.top+8,rect.right-100-70-7,rect.top+8+13);
				btn1Map.SetAt(nItem,addRect);
				
				//added by zuoss 2008.08.04
				//���۰�ť
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-171-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
				//comRect �����Ӧ������ʱδ����Ӧ
				CRect comRect(rect.right-100-171-31, rect.top+8,rect.right-100-171, rect.top+8+13);
				//end add
			}
			
			
			
		}
		else if(info->src == src_readman){
			CBitmap bmp1;
			CDC memDC1;
			
			//���İ�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_ORDER);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//added by zuoss 2008.08.04
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-112-31, rect.top+8,rect.right-100-112, rect.top+8+13);
			//end add
			
		}
		else if(info->src == src_local){
			CBitmap bmp1;
			CDC memDC1;
			
			//�Ķ���ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_READ);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//ͬ����ť
			if(info->abst3[0] == 1)
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_SYN_CHK);
			else
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_SYN_EMP);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-40, rect.top+8, 40, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect synRect(rect.right-100-112-40, rect.top+8,rect.right-100-112, rect.top+8+13);
			btn2Map.SetAt(nItem,synRect);
			
			//added by zuoss 2008.08.04
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-44-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-112-44-31, rect.top+8,rect.right-100-156, rect.top+8+13);
			//end add
			
		}
		else if(info->src == src_istation){
			UINT resouce = IDB_BITMAP_BUTTON_BROWSE;
			CBitmap bmp1;
			CDC memDC1;
			
			char* temp = strchr(info->abst3, ';');
			if(temp){
				temp++;
				if(*temp=='1')
					resouce = IDB_BITMAP_BUTTON_DOWNLOAD;//���ذ�ť
				else if(*temp=='8'){
					resouce = IDB_BITMAP_BUTTON_MAKEWOLF;//���鰴ť
				}
			}
			
			bmp1.LoadBitmap(resouce);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//added by zuoss 2008.08.04
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-112-31, rect.top+8,rect.right-100-112, rect.top+8+13);
			//end add
			
		}
		else if(info->src == src_netbook)
		{
			CBitmap bmp1;
			CDC memDC1;
			
			//�������鰴ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_TRYMAKEWOLF);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-65, rect.top+8, 58, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-65, rect.top+8,rect.right-100-70-65+58,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//added by zuoss 2008.08.04
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-139-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-139-31, rect.top+8,rect.right-100-139, rect.top+8+13);
			//end add
		}
			
		else if(info->src == src_sinabook)
		{
			CBitmap bmp1;
			CDC memDC1;
			
			//�������鰴ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_TRYMAKEWOLF);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-65, rect.top+8, 58, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-65, rect.top+8,rect.right-100-70-65+58,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//added by zuoss 2008.08.04
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-139-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-139-31, rect.top+8,rect.right-100-139, rect.top+8+13);
			//end add

			//20100330 ���������۵�
			if (atoi(info->abst3) > 0)
			{
				if (!isOpen_makebook)
				{	
					bmp1.LoadBitmap(IDB_BITMAP_BUTTON_ZHANKAI);
				}
				else
				{
					bmp1.LoadBitmap(IDB_BITMAP_BUTTON_SHOUQI);
				}
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-170-50, rect.top+8, 50, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
				CRect openRect(rect.right-100-170-50, rect.top+8,rect.right-100-170-50+50,rect.top+8+13);
				btn2Map.SetAt(nItem,openRect);
			}

		}
		//add by zhanganzhan on 20090513
		else if (info->src == src_netbook && FALSE)
		{
			CBitmap bmp1;
			CDC memDC1;
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_BROWSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//�������鰴ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_TRYMAKEWOLF);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-65, rect.top+8, 58, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect1(rect.right-100-70-65, rect.top+8,rect.right-100-70-65+58,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-139-31, rect.top+8,rect.right-100-139, rect.top+8+13);
			
			
			
		}
		else if(info->src == src_share){
			CBitmap bmp1;
			CDC memDC1;
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_EMAIL_R);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-31-65, rect.top+8, 55, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect emailRect(rect.right-100-112-31-65, rect.top+8,rect.right-100-112-31-65+55,rect.top+8+13);
			btn4Map.SetAt(nItem,emailRect);
			
			//���ذ�ť
			//if(info->isAlive)
			if(true)
			{
				//			AfxMessageBox("info alive");
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_DOWNLOAD);
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
				CRect btnRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
				btn1Map.SetAt(nItem,btnRect);
			}
			else
			{
				//			AfxMessageBox("info not alive");
				bmp1.LoadBitmap(IDB_BITMAP_BUTTON_DOWNLOADH);
				memDC1.CreateCompatibleDC(pDC);
				memDC1.SelectObject(&bmp1);
				pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
				bmp1.DeleteObject();
				memDC1.DeleteDC();
			}
			
			
			//�����ť
			/*	bmp1.LoadBitmap(IDB_BITMAP_BUTTON_BROWSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect synRect(rect.right-100-112-31, rect.top+8,rect.right-100-112, rect.top+8+13);
			btn2Map.SetAt(nItem,synRect);*/
			
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			//	pDC->BitBlt(rect.right-100-145-31,rect.top+8,31,13,&memDC1,0,0,SRCCOPY);
			pDC->BitBlt(rect.right-100-112-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			//	CRect comRect(rect.right-100-145-31,rect.top+8,rect.right-100-145,rect.top+8+13);
			CRect comRect(rect.right-100-112-31, rect.top+8,rect.right-100-112, rect.top+8+13);
			btn3Map.SetAt(nItem,comRect);///20080729 BY LINSHAN
			///////////////////////////////////
			CString strstar;
			strstar.Format("��֣�");
			pDC->TextOut(rect.right-100-70-38, rect.top+37, strstar);
			strstar.Format("�÷֣�%s",info->score);
			pDC->TextOut(rect.right-100-70-38+35+85, rect.top+37, strstar);	
			
			bmp1.LoadBitmap(IDB_BITMAP_BLANKSTAR);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38+35, rect.top+35, 15, 15, &memDC1, 0, 0, SRCCOPY);
			CRect star1Rect(rect.right-100-70-38+35,rect.top+35,rect.right-100-70-38+35+15,rect.top+35+15);
			star1Map.SetAt(nItem,star1Rect);
			//	pDC->BitBlt(rect.right-100-145-31,rect.top+8,31,13,&memDC1,0,0,SRCCOPY);
			pDC->BitBlt(rect.right-100-70-38+35+15, rect.top+35, 15, 15, &memDC1, 0, 0, SRCCOPY);
			pDC->BitBlt(rect.right-100-70-38+35+30, rect.top+35, 15, 15, &memDC1, 0, 0, SRCCOPY);
			pDC->BitBlt(rect.right-100-70-38+35+45, rect.top+35, 15, 15, &memDC1, 0, 0, SRCCOPY);
			pDC->BitBlt(rect.right-100-70-38+35+60, rect.top+35, 15, 15, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			
			//CRect star1Rect(rect.right-100-145-31,rect.top+8,31,13);
			
			CRect star2Rect(rect.right-100-70-38+35+15,rect.top+35,rect.right-100-70-38+35+30,rect.top+35+15);
			star2Map.SetAt(nItem,star2Rect);
			CRect star3Rect(rect.right-100-70-38+35+30,rect.top+35,rect.right-100-70-38+35+45,rect.top+35+15);
			star3Map.SetAt(nItem,star3Rect);
			CRect star4Rect(rect.right-100-70-38+35+45,rect.top+35,rect.right-100-70-38+35+60,rect.top+35+15);
			star4Map.SetAt(nItem,star4Rect);
			CRect star5Rect(rect.right-100-70-38+35+60,rect.top+35,rect.right-100-70-38+35+75,rect.top+35+15);
			star5Map.SetAt(nItem,star5Rect);
			
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			////////////////////////////////////
		}
		else if(info->src == src_dangdang){
			CBitmap bmp1;
			CDC memDC1;
			//�����ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_BROWSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			//����ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_BUY);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-154-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-112-31, rect.top+8,rect.right-100-112, rect.top+8+13);
			//end add
		}else if(info->src == src_fanshu){
			CBitmap bmp1;
			CDC memDC1;
			//�����ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_BROWSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			//����ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_BUY);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-154-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-112-31, rect.top+8,rect.right-100-112, rect.top+8+13);
			//end add
		}else{
			CBitmap bmp1;
			CDC memDC1;
			//�����ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_BROWSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-70-38, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();
			CRect btnRect(rect.right-100-70-38, rect.top+8,rect.right-100-70-38+31,rect.top+8+13);
			btn1Map.SetAt(nItem,btnRect);
			
			//added by zuoss 2008.08.04
			//���۰�ť
			bmp1.LoadBitmap(IDB_BITMAP_BUTTON_COMMENT_FALSE);
			memDC1.CreateCompatibleDC(pDC);
			memDC1.SelectObject(&bmp1);
			pDC->BitBlt(rect.right-100-112-31, rect.top+8, 31, 13, &memDC1, 0, 0, SRCCOPY);
			bmp1.DeleteObject();
			memDC1.DeleteDC();

			
			
			//comRect �����Ӧ������ʱδ����Ӧ
			CRect comRect(rect.right-100-112-31, rect.top+8,rect.right-100-112, rect.top+8+13);
			//end add
		}
		
		bmp.LoadBitmap(IDB_SEARCHLIST_ICON_SOURCE);
		memDC.CreateCompatibleDC(pDC);
		memDC.SelectObject(&bmp);
		pDC->BitBlt(rect.right-100-70, rect.top+5, 63, 25, &memDC, 0, 0, SRCCOPY);
		bmp.DeleteObject();
		memDC.DeleteDC();
		///////////////////////////////////
		///////////////////////////////////
		CBitmap bmp1;
		CDC memDC1;
		if(info->src == src_fanshu){
			CString price;
			price.Format("�۸�19.9Ԫ");
			pDC->TextOut(rect.right-100-70-38-18-40, rect.top+37, price);
		}
		CString strstar;
		strstar.Format("�������֣�");
		pDC->TextOut(rect.right-100-70-38-18+40, rect.top+37, strstar);
		bmp1.LoadBitmap(IDB_BITMAP_STAR);
		memDC1.CreateCompatibleDC(pDC);
		memDC1.SelectObject(&bmp1);
		pDC->BitBlt(rect.right-100-70-38+35+40, rect.top+35, 15, 15, &memDC1, 0, 0, SRCCOPY);
		bmp1.DeleteObject();
		memDC1.DeleteDC();
		//CRect star1Rect(rect.right-100-70-38+35,rect.top+35,rect.right-100-70-38+35+15,rect.top+35+15);
		//star1Map.SetAt(nItem,star1Rect);
		////////////////////////////////////
		}
		catch(...){
#ifdef _DEBUG
			Tips("void CMainListView::DrawFindResultText(CDC *pDC, CRect rect, FINDINFO *info, int nItem)");
#endif
		}
}

LRESULT CMainListView::Search(LPSTR keyword)
{
 	if (gbOnSearching == TRUE)
 	{
 		MessageBox("�������������Ժ�...");
 		return 0;
 	}
 	gbOnSearching = TRUE;
	//�趨״̬��Ϣ add by dzh 080717
	if (this->headflag != 1)
	{
		this->curcondition = 0;
		pMainWnd->curCondition = 0;
	}
	searchkey = keyword;//added by zuoss 20081203 ���������ؼ��֡�
	//�����û��ġ���������Ϊ��¼    add by Lins 20081223
	char behavior[1024] = {0};
	sprintf_s(behavior, 1024, "������\"%s\"",searchkey);
	char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
	pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);
	try{
	/*	
	int flag = 0;//��ʾ���������ؼ��ֻ���ֱ�ӹؼ���
	if (keyword[0]=='$'&&keyword[1]=='$'){
	flag = 1;
	}*/
		////////////////////20080801///////////////////////////
		//����ResultList������������ļ���������Ϊ��cash.xml��
		char cashpath[1024] = {0}; 
		sprintf_s(cashpath, 1024, "%s\\temp\\cash.xml",CurrentPath);
		DeleteFile(cashpath);
		if(!cashfile.Open(cashpath,CFile::modeReadWrite|CFile::modeCreate ))
		{
			return FALSE;
		}
		cashfile.SeekToBegin();
		cashfile.WriteString("<?xml version=\"1.0\" encoding=\"gb2312\"?>");
		cashfile.SeekToEnd();//ÿ�ζ������д��
		cashfile.WriteString("<find>");
		///////////////////20080801//////////////////////////////
		char* kw_b64;
		kw_b64 = (char*)encode2((PUCHAR)(LPCSTR)keyword,strlen(keyword));
		
		StopThread();
		m_plist->DeleteAllItems();
		SetType(Is_FindList);
		
		startTime = GetTickCount();
		status = 0;
		
		imgList.DeleteImageList();
		ChangeToList();
		//��ʱ����ô��
		imgList.Create(135, THUMBNAIL_HEIGHT_LIST, ILC_COLOR24, 0, 1);
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		imgList.SetImageCount(0);
		
		totalPage = 0;
		totalPage1 = 0;
		
		totalCount = 0;
		finishedSearchCount = 0;
		
		
		FINDINFO* findinfo = (FINDINFO*)malloc(sizeof(FINDINFO));
		ZeroMemory(findinfo, sizeof(FINDINFO));
		findinfo->src = src_msg;
		//	strcpy(findinfo->title, "������,���Ժ�...");
		//	m_plist->InsertItem(0,"������,���Ժ�...",0);
		strcpy_s(findinfo->title, 200, " ");
		m_plist->InsertItem(0," ",0);
		m_plist->SetItemData(0,(DWORD)findinfo);
		
		//	m_plist->SetRedraw(FALSE);
		CList<ThreadInfo,ThreadInfo&>* infolist = new CList<ThreadInfo,ThreadInfo&>;
		ThreadInfo* info_google = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_du8 = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		//	ThreadInfo* info_opencourse = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_verycd = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		//	ThreadInfo* info_huntmine = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_baidu = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		//zuoss 20081029
		ThreadInfo* info_baiduentity = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		//ThreadInfo* info_duxiu = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));//zuoss 081104
		
		ThreadInfo* info_readman = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_order = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		// 	ThreadInfo* info_zcom = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_local = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_sina = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_dangdang = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_fanshu = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		ThreadInfo* info_share = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
		
		//baiduwenku
		sprintf_s(info_google->WebPath, 1024,"%s/search/getbaiduwenku.asp?key=%s&base64=1", AspServer, kw_b64);
		info_google->id = src_baiduwenku;
		//AfxMessageBox(info_google->WebPath);
		info_google->pWnd = this;
		if (page == 1)
			info_google->doflg = 1;
		else
			info_google->doflg = 0;
		infolist->AddTail(*info_google);
		//du8
		sprintf_s(info_du8->WebPath, 1024, "%s/search/getsinaishare.asp?key=%s&base64=1", AspServer, kw_b64);
		info_du8->id = src_sinaishare;
		info_du8->pWnd = this;
		if (page == 1)
			info_du8->doflg = 1;
		else
			info_du8->doflg = 0;
		infolist->AddTail(*info_du8);
		//dangdang
		sprintf_s(info_dangdang->WebPath, 1024, "%s/search/getdangdang.asp?key=%s&base64=1", AspServer, kw_b64);
		info_dangdang->id = src_dangdang;
		info_dangdang->pWnd = this;
		if (page == 1)
			info_dangdang->doflg = 1;
		else
			info_dangdang->doflg = 0;
		infolist->AddTail(*info_dangdang);
		//fanshu
		sprintf_s(info_fanshu->WebPath, 1024, "%s/search/getfanshu.asp?key=%s&base64=1", AspServer, kw_b64);
		info_fanshu->id = src_fanshu;
		info_fanshu->pWnd = this;
		if (page == 1)
			info_fanshu->doflg = 1;
		else
			info_fanshu->doflg = 0;
		infolist->AddTail(*info_fanshu);
		//opencourse
		// 	sprintf(info_opencourse->WebPath, "%s/share/search_getMITOpenCourse.asp?key=%s", AspServer,keyword);
		// 	info_opencourse->id = src_opencourse;
		// 	info_opencourse->pWnd = this;
		// 	if (page == 1)
		// 	    info_opencourse->doflg = 1;
		// 	else
		//         info_opencourse->doflg = 0;
		//verycd
		sprintf_s(info_verycd->WebPath, 1024, "%s/search/getverycd.asp?key=%s&base64=1", AspServer, kw_b64);
		info_verycd->id = src_verycd;
		info_verycd->pWnd = this;
		if (page == 1)
			info_verycd->doflg = 1;
		else
			info_verycd->doflg = 0;
		infolist->AddTail(*info_verycd);
		//huntmine
		// 	sprintf(info_huntmine->WebPath, "%s/search/gethuntmine.asp?key=%s&base64=1", AspServer, kw_b64);
		// 	info_huntmine->id = src_huntmine;
		// 	info_huntmine->pWnd = this;
		// 	if (page == 1)
		// 	     info_huntmine->doflg = 1;
		// 	else
		//          info_huntmine->doflg = 0;
		
		//zuoss 20081030 �ٶ�ʵ������
		sprintf_s(info_baiduentity->WebPath, 1024, "%s/search/getbaidufile.asp?key=%s", AspServer, keyword);	    
		info_baiduentity->id = src_baiduentity;
		info_baiduentity->pWnd = this;
		if (page == 1)
			info_baiduentity->doflg = 1;
		else
			info_baiduentity->doflg = 0;
		infolist->AddTail(*info_baiduentity);
		//end 
		
		//zuoss 20081104 duxiu����
		//	sprintf(info_duxiu->WebPath, "%s/share/search_duxiu.asp?key=%s", AspServer, keyword);		    
		//	info_duxiu->id = src_duxiu;
		//zhanganzhan on 090611
// 		sprintf(info_duxiu->WebPath,"%s/search/searchmailbook.asp?key=%s&username=%s",AspServer,keyword,UserName);
// 		info_duxiu->id = src_mailbook;
// 		info_duxiu->pWnd = this;
// 		if (page == 1)
// 			info_duxiu->doflg = 1;
// 		else
// 			info_duxiu->doflg = 0;
		//end 
		
		//baidu
		sprintf_s(info_baidu->WebPath, 1024, "%s/search/getbaidu.asp?key=%s&base64=1", AspServer, kw_b64);
		//sprintf(info_baidu->WebPath, "%s/share/search_getbaidu.asp?key=%s", AspServer, keyword);
		info_baidu->id = src_baidu;
		info_baidu->pWnd = this;
		if (page == 1)
			info_baidu->doflg = 1;
		else
			info_baidu->doflg = 0;
		infolist->AddTail(*info_baidu);
		//readman
		sprintf_s(info_readman->WebPath, 1024, "%s/search/getreadman.asp?key=%s&base64=1", AspServer, kw_b64);
		info_readman->id = src_readman;
		info_readman->pWnd = this;
		if (page == 1)
			info_readman->doflg = 1;
		else
			info_readman->doflg = 0;
		infolist->AddTail(*info_readman);
		//order
		sprintf_s(info_order->WebPath, 1024, "%s/search/getorder.asp?key=%s&base64=1", AspServer, kw_b64);
		info_order->id = src_order;
		info_order->pWnd = this;
		if (page == 1)
			info_order->doflg = 1;
		else
			info_order->doflg = 0;
		infolist->AddTail(*info_order);
		//zcom
		// 	sprintf(info_zcom->WebPath, "%s/search/searchbook.asp?key=%s", AspServer, keyword);
		// 	info_zcom->id = src_netbook;
		// 	info_zcom->pWnd = this;
		// 	if (page == 1)
		// 		info_zcom->doflg = 1;
		// 	else
		// 		info_zcom->doflg = 0;
		//sinabook
		sprintf_s(info_sina->WebPath, 1024, "%s/search/getmakebook.asp?key=%s&base64=1", AspServer, kw_b64);
		info_sina->id = src_sinabook;
		info_sina->pWnd = this;
		if (page == 1)
			info_sina->doflg = 1;
		else
			info_sina->doflg = 0;
		infolist->AddTail(*info_sina);
		//local
		info_local->id = src_local;
		strcpy_s(info_local->WebPath, 1024, keyword);
		info_local->pWnd = this;
		//share changed by zuoss 20081209
		
		info_share->id = src_share;
		//	sprintf(info_share->WebPath, "%s/share/search_getshare.asp?key=%s&base64=1", AspServer, kw_b64);
		//sprintf(info_share->WebPath, "%s/share/search_getshare_1.asp?key=%s&base64=1", AspServer, kw_b64);
		string ss = tran_16bases((unsigned char*)keyword,strlen(keyword));
		sprintf_s(info_share->WebPath, 1024, "http://61.181.14.184:6969/SearchName.html?name=%s", ss.c_str());
		//sprintf(info_share->WebPath, "http://61.181.14.184:6969/SearchName.html?name=%s", keyword);
		info_share->pWnd =this;
		if (page == 1)
			info_share->doflg = 1;
		else
			info_share->doflg = 0;
		infolist->AddTail(*info_share);
		HANDLE allth = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(infolist),CREATE_SUSPENDED, NULL);
		if(allth){
			threadList.AddTail(allth);
			//allth->handle = allth;
			ResumeThread(allth);
		}
		/*HANDLE dlth_google = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_google),CREATE_SUSPENDED, NULL);
		HANDLE dlth_du8 = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_du8),CREATE_SUSPENDED, NULL);
		//	HANDLE dlth_opencourse = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_opencourse),CREATE_SUSPENDED, NULL);
		HANDLE dlth_verycd = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_verycd),CREATE_SUSPENDED, NULL); 
		//	HANDLE dlth_huntmine = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_huntmine),CREATE_SUSPENDED, NULL); 
		HANDLE dlth_baidu = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_baidu),CREATE_SUSPENDED, NULL); 
		//zuoss 20081029
		HANDLE dlth_baiduentity = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_baiduentity),CREATE_SUSPENDED, NULL); 
		//HANDLE dlth_duxiu = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_duxiu),CREATE_SUSPENDED, NULL); 
		
		HANDLE dlth_readman = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_readman),CREATE_SUSPENDED, NULL); 
		HANDLE dlth_order = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_order),CREATE_SUSPENDED, NULL); 
		//	HANDLE dlth_zcom = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_zcom),CREATE_SUSPENDED, NULL); 
		HANDLE dlth_sina = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_sina),CREATE_SUSPENDED, NULL); 	
		
		HANDLE dlth_share = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_share),CREATE_SUSPENDED, NULL); 
		HANDLE dlth_dangdang = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_dangdang),CREATE_SUSPENDED, NULL); 
		HANDLE dlth_fanshu = (HANDLE)_beginthreadex(NULL,0, _SearchNet,(LPVOID)(info_fanshu),CREATE_SUSPENDED, NULL); */
		
		HANDLE dlth_local = (HANDLE)_beginthreadex(NULL,0, _SearchLocal,(LPVOID)(info_local),CREATE_SUSPENDED, NULL); 
		/*if(dlth_google){
			threadList.AddTail(dlth_google);
			info_google->handle = dlth_google;
			ResumeThread(dlth_google);
		}
		if(dlth_du8){
			threadList.AddTail(dlth_du8);
			info_du8->handle = dlth_du8;
			ResumeThread(dlth_du8);
		}
		if(dlth_dangdang){
			threadList.AddTail(dlth_dangdang);
			info_dangdang->handle = dlth_dangdang;
			ResumeThread(dlth_dangdang);
		}
		if(dlth_fanshu){
			threadList.AddTail(dlth_fanshu);
			info_fanshu->handle = dlth_fanshu;
			ResumeThread(dlth_fanshu);
		}
		// 	if(dlth_opencourse){
		// 		threadList.AddTail(dlth_opencourse);
		// 		info_opencourse->handle = dlth_opencourse;
		// 		ResumeThread(dlth_opencourse);
		// 	}
		if(dlth_verycd){
			threadList.AddTail(dlth_verycd);
			info_verycd->handle = dlth_verycd;
			ResumeThread(dlth_verycd);
		}
		// 	if(dlth_huntmine){
		//   		threadList.AddTail(dlth_huntmine);
		//  		info_huntmine->handle = dlth_huntmine;
		//  		ResumeThread(dlth_huntmine);
		//  	}
		if(dlth_baidu){
			threadList.AddTail(dlth_baidu);
			info_baidu->handle = dlth_baidu;
			ResumeThread(dlth_baidu);
		}
		
		if(dlth_baiduentity){
			threadList.AddTail(dlth_baiduentity);
			info_baiduentity->handle = dlth_baiduentity;
			ResumeThread(dlth_baiduentity);
		}
// 		if(dlth_duxiu){
// 			threadList.AddTail(dlth_duxiu);
// 			info_duxiu->handle = dlth_duxiu;
// 			ResumeThread(dlth_duxiu);
// 		}
		
		if(dlth_readman){
			threadList.AddTail(dlth_readman);
			info_readman->handle = dlth_readman;
			ResumeThread(dlth_readman);
		}
		if(dlth_order){
			threadList.AddTail(dlth_order);
			info_order->handle = dlth_order;
			ResumeThread(dlth_order);
		}
		// 	if(dlth_zcom){
		//  		threadList.AddTail(dlth_zcom);
		// 		info_zcom->handle = dlth_zcom;
		// 		ResumeThread(dlth_zcom);
		// 	}
		if(dlth_sina){
			threadList.AddTail(dlth_sina);
			info_sina->handle = dlth_sina;
			ResumeThread(dlth_sina);
		}
		if(dlth_share){
			threadList.AddTail(dlth_share);
			info_share->handle = dlth_share;
			ResumeThread(dlth_share);
		}*/
		if(dlth_local){
			threadList.AddTail(dlth_local);
			info_local->handle = dlth_local;
			ResumeThread(dlth_local);
		}
		free(kw_b64);
		free(keyword);
}
catch(...){
#ifdef _DEBUG
	Tips("Search");
#endif
}
return TRUE;
}

unsigned __stdcall CMainListView::_SearchNet(LPVOID pParam)
{
	isOpen_makebook = 0;//�����۵�Ч������������ʼΪ�۵�״̬
	try{
		CList<ThreadInfo,ThreadInfo&>* info = (CList<ThreadInfo,ThreadInfo&>*)pParam;
		POSITION pos = info->GetHeadPosition();
		while (pos)
		{
			ThreadInfo tinfo = info->GetNext(pos);
			CMainListView* pWnd = (CMainListView*)tinfo.pWnd;
			DWORD ncount = 0;
			if(!offline)
				if (tinfo.doflg == 1)
					ncount = pWnd->SearchNet(&tinfo);
				else
					ncount = 0;
				if (pMainWnd->m_quit)
				{
					return 0;
				}
				//AfxMessageBox(pView->CheckSrc(info->id));
			pWnd->PostMessage(WM_TASKFINISHED,(WPARAM)tinfo.id,(LPARAM)ncount);
		}
		/*ThreadInfo* info = (ThreadInfo*)pParam;
		CMainListView* pWnd = (CMainListView*)info->pWnd;
		if(!info)
			return 0;
		DWORD ncount = 0;
		if(!offline)
			if (info->doflg == 1)
				ncount = pWnd->SearchNet(info);
			else
				ncount = 0;
			if (pMainWnd->m_quit)
			{
				return 0;
			}
			//AfxMessageBox(pView->CheckSrc(info->id));
			pWnd->PostMessage(WM_TASKFINISHED,(WPARAM)info->id,(LPARAM)ncount);
			pWnd->PostMessage(WM_THREAD_FINISHED,(WPARAM)info->handle);	
			free(info);
			_endthreadex(0);*/
		HANDLE handle = (HANDLE)::GetCurrentThread();
		pView->PostMessage(WM_THREAD_FINISHED,(WPARAM)handle);	
	}
	catch(...){
#ifdef _DEBUG
		Tips("_SearchNet");
#endif
	}
	return 0;
}

unsigned __stdcall CMainListView::_SearchLocal(LPVOID pParam)
{
	try{
		ThreadInfo* info = (ThreadInfo*)pParam;
		CMainListView* pWnd = (CMainListView*)info->pWnd;
		DWORD ncount = pWnd->SearchLocal(info->WebPath);
		WPARAM wParam = src_local;
		pWnd->PostMessage(WM_TASKFINISHED,wParam,(LPARAM)ncount);
		pWnd->PostMessage(WM_THREAD_FINISHED,(WPARAM)info->handle);
		free(info);
		_endthreadex(0);
	}
	catch(...){
#ifdef _DEBUG
		Tips("_SearchLocal");
#endif
	}
	return 0;
}

DWORD CMainListView::SearchNet(ThreadInfo *info)
{
	try
	{
		DWORD ncount = 0;
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		char Tadd[_MAX_PATH] = {0};
		sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\findres%d.xml",CurrentPath, info->id);
		DeleteFile(Tadd);
		//if((fopen(Tadd,"r"))==NULL)			/*Ҫ�򿪵��ļ���*/ 
		//if(!PathFileExists(Tadd))
		//	{
 		//if(info->id != src_share)
 		//{
 			//if(!DownLoad(info->WebPath,Tadd, quit, socketList, socketlistmutex))
			//if(!DownLoad(info->WebPath,Tadd, quit, socketList))
 			//{
 				//AfxMessageBox("dowdload "+ CString(info->WebPath) +" faise");
 			//	return 0;	//xml ����ʧ��
			//}
		//}else{
			char* buffer;
			HttpUtl utl;
			if(utl.request(info->WebPath,buffer)==0)
			{
				Tips(Tadd+CString(" download failed"));
				return 0;
			}
			if (pMainWnd->m_quit)
			{
				return FALSE;
			}
			if(!WriteToFile(Tadd, buffer, strlen(buffer)))
			{
				Tips(Tadd+CString(" download failed"));
				return 0;
			}
			
		//}
		//	}
		if (pMainWnd->m_quit)
		{
			return FALSE;
		}
		// begin added by yangxiao 2008.8.15
		char oper[256];
		sprintf_s(oper, 256, "�ؼ�������");
		//oper.Format("��%s�����ؼ���",path);
		
		CFile * pFile = new CFile();
		pFile->Open(Tadd,CFile::modeRead);
		long fileSize = pFile->GetLength();
		pFile->Close();	
		
		m_searchLog.setLogItem(oper,fileSize, down,comm_oper_search);
		m_searchLog.writeLogItem();
		// end added by yangxiao 2008.8.15
		
		try
		{
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//xml ����ʧ��
			if(!pDoc->load(Tadd))
				throw 2;	//xml ����ʧ��
			pNodeList = pDoc->selectNodes("find/item");
			if (pNodeList == NULL)
			{
				throw 3;
			}
			if(pNodeList){
				for(int pos1=0; pos1<pNodeList->length && !quit; pos1++)
				{
					CSingleLock SingleLock(&mutex);
					SingleLock.Lock();
					if(SingleLock.IsLocked())
					{
						
						int count;
						CString title, cover, link, abst1, abst2, abst3, score, page, pagecount;
						CString isalive = "";
						count = m_plist->GetItemCount();
						title = GetXmlValue(pNodeList, "title", pos1);
						if (info->id == src_share)
						{
							string stitle = rtran_16bases((unsigned char*)title.GetBuffer(0),title.GetLength());
							title = stitle.c_str();
						}
						//AfxMessageBox(title);
						cover = GetXmlValue(pNodeList, "cover", pos1);
						link = GetXmlValue(pNodeList, "link", pos1);
						abst1 = GetXmlValue(pNodeList, "abst1", pos1);
						abst2 = GetXmlValue(pNodeList, "abst2", pos1);
						abst3 = GetXmlValue(pNodeList, "abst3", pos1);
						if (info->id == src_sinabook && isOpen_makebook == 0)//�۵�Ч��
						{
							int iabst3 = atoi(abst3);
							if (iabst3 < 0)
							{
								ncount++;
								//SetRedraw(TRUE);
								SingleLock.Unlock();
								continue;
							}
						}

						finishedSearchCount++;

						if (info->id == src_share)
						{
							isalive = GetXmlValue(pNodeList,"isalive",pos1);
						}
						//20080801
						FormResultList(title,cover,link,abst1,abst2,abst3,info->id);
						
						score = GetXmlValue(pNodeList,"score",pos1);
						page = GetXmlValue(pNodeList,"page",pos1);
						pagecount = GetXmlValue(pNodeList,"pagecount",pos1);
						totalPage = atoi(pagecount);
						
						FINDINFO* finfo = (FINDINFO*)calloc(1,sizeof(FINDINFO));
						
						strcpy_s(finfo->title, 200, title);
						strcpy_s(finfo->cover, 1024, cover);
						strcpy_s(finfo->link, 1024, link);
						strcpy_s(finfo->abst1, 1000, abst1);
						strcpy_s(finfo->abst2, 1000, abst2);
						strcpy_s(finfo->abst3, 1000, abst3);
						strcpy_s(finfo->score, 100, score);
						
						finfo->src = info->id;
						finfo->icon = count;
						if (finfo->src == src_share)
							finfo->isAlive = atoi((LPCSTR)isalive);
						
						FindIconListInfo listInfo = {0};
						listInfo.src = finfo->src;
						try{
							strcpy_s(listInfo.cover, 1024, finfo->cover);
						}
						catch(...){Tips("Here");}
						coverMap.SetAt(count,listInfo);
						SetRedraw(FALSE);
						imgList.SetImageCount(count+1);
						CBitmap bitmap;
						bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
						imgList.Replace(count,&bitmap,NULL);
						bitmap.DeleteObject();
						if (finishedSearchCount <= 20)
						{
							if(info->id == src_verycd){
								m_plist->InsertItem(count,title,count);
								m_plist->SetItemData(count,(DWORD)finfo);
							}
							else if(info->id == src_share){
								if (page == "1")
								{
									m_plist->InsertItem(finishedSearchCount,title,count);
									m_plist->SetItemData(finishedSearchCount,(DWORD)finfo);
								}
								
							}
							else{
								m_plist->InsertItem(finishedSearchCount,title,count);
								m_plist->SetItemData(finishedSearchCount,(DWORD)finfo);
								
							}
						}
						//	m_plist->InsertItem(count,title,count);
						//	m_plist->SetItemData(count,(DWORD)finfo);
						ncount++;
						int countpage = m_plist->GetCountPerPage();
						SetRedraw(TRUE);
				}
				SingleLock.Unlock();
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//�Ѹ���
			pNodeList.Release();
		case 2:		//��ʽ����
			pDoc.Release();
			break;
		}
		return 0;
	}
	
	if(info->id == src_share)//���������ֶ�
	{
		try
		{
			sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\findres14.xml",CurrentPath);
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//xml ����ʧ��
			if(!pDoc->load(Tadd))
				throw 2;	//xml ����ʧ��
			pNodeList = pDoc->selectNodes("find");
			if (pNodeList == NULL)
			{
				throw 3;
			}
			if(pNodeList)
			{
				CSingleLock SingleLock(&mutex);
				SingleLock.Lock();
				if(SingleLock.IsLocked())
				{
					CString scount = GetXmlValue(pNodeList, "totalcount", 0);
					char pcount[10] = {0};
					strcpy_s(pcount, 10, scount);
					ncount = atoi(pcount);
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(int errnum)
		{
			switch(errnum)
			{
			case 3:		//�Ѹ���
				pNodeList.Release();
			case 2:		//��ʽ����
				pDoc.Release();
				break;
			}
			return 0;
		}
	}	
	return ncount;
}
catch(...){
#ifdef _DEBUG
	Tips("SearchNet");
#endif
}
return 0;
}

DWORD CMainListView::SearchLocal(char *keyword)
{
	try{
		DWORD ncount = 0;
		CString KW = keyword;
		CurSearchKeywords = keyword;
		
		KW.Replace("'", "''");
		_RecordsetPtr m_pRecordset;
		try{
			char SQL[1024] = {0};//SELECT title,addr,cdate,size FROM loc WHERE InStr(1,LCase(title),LCase('%s'),0)<>0
			sprintf_s(SQL, 1024, "SELECT [title],[addr],[cdate],[size],[SynFlag] FROM loc WHERE InStr(1,LCase(title),LCase('%s'),0)<>0 ORDER BY nlist ASC, cdate DESC", KW);
			HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
			if(!SUCCEEDED(hr))
				throw 0;
			hr = m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // ��ȡ��ӿ��IDispatchָ��
				adOpenStatic, adLockOptimistic, adCmdText);
			if(!SUCCEEDED(hr)){
				//	m_pRecordset.Release();
				throw 0;
			}
			
			int recordCount = 0;
			if(!m_pRecordset->EndOfFile)
				recordCount = m_pRecordset->GetRecordCount();
			
			ncount = recordCount;
			totalPage = (int)(recordCount/MAXFINDLISTLOCAL);
			for(int lp=0; lp<min(MAXFINDLISTLOCAL,recordCount) && !quit; lp++){
				if(lp!=0)
					m_pRecordset->MoveNext();
				try{
					CSingleLock SingleLock(&mutex);
					SingleLock.Lock();
					if(SingleLock.IsLocked()){
						int count = m_plist->GetItemCount();
						FINDINFO* info = (FINDINFO*)malloc(sizeof(FINDINFO));
						memset(info, 0, sizeof(FINDINFO));
						strcpy_s(info->title,  200, GetCollectString(m_pRecordset, "title"));
						sprintf_s(info->cover,  1024, "%s\\system\\zwfm.gif", CurrentPath);
						CString path = GetCollectString(m_pRecordset,"addr");
						strcpy_s(info->link, 1024, path);
						CString strInfo;
						strInfo.Format("�ļ���С:%.3fKB       ����ʱ��:%s       �ļ�����:%s",
							atof(GetCollectString(m_pRecordset,"size"))/1024.0,GetCollectDate(m_pRecordset,"cdate"),
							path.Right(path.GetLength()-path.ReverseFind('.')));
						strcpy_s(info->abst1, 1000, strInfo);
						strcpy_s(info->abst2, 1000, path);
						info->src = src_local;
						info->icon = count;
						//ͬ����Ϣ
						if((char)GetCollectLong(m_pRecordset, "SynFlag")!=0)
							info->abst3[0] = 1;
						else
							info->abst3[0] = 0;
						
						FindIconListInfo listInfo = {0};
						listInfo.src = info->src;
						strcpy_s(listInfo.cover, 1024, info->cover);
						coverMap.SetAt(count,listInfo);
						
						//imgList.SetImageCount(count+1);
						CImageList* il = m_plist->GetImageList(LVSIL_SMALL);
						il->SetImageCount(count+1);
						CBitmap bitmap;
						bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
						il->Replace(count,&bitmap,NULL);
						bitmap.DeleteObject();
						m_plist->InsertItem(count,info->title,count);
						m_plist->SetItemData(count,(DWORD)info);
					}
				}
				catch(_com_error& e){Tips(e.ErrorMessage());}
			}
			m_pRecordset->Close();
			//		m_pRecordset.Release();
		}
		catch (_com_error& e){
#ifdef _DEBUG
			Tips(e.ErrorMessage());
#endif
			return 0;
		}
		catch(int err){
#ifdef _DEBUG
			CString msg;
			msg.Format("%d", err);
			Tips(msg);
#endif
		}
		return ncount;
	}
	catch(...){
#ifdef _DEBUG
		Tips("SearchLocal");
#endif
	}
	return 0;
}

LRESULT CMainListView::OnTaskFinished(WPARAM wParam, LPARAM lParam)
{
	try{
		FINDINFO* info = (FINDINFO*)(m_plist->GetItemData(0));
		status++;
		if(status < SEARCHSOURCES)
		{
			if(info)
			{
				if((status-1) >= 0)//����һ��SEARCHSOURCES���߳�
				{
					info->abst1[status-1] = (char)wParam;
					DWORD* dabst2 = (DWORD*)(info->abst2);
					dabst2[status-1] = (DWORD)lParam;
					m_plist->RedrawItems(0,0);//�ػ�������
					totalCount+=(DWORD)lParam;
				}
			}
			return FALSE;
		}
		//�����Դ����������������������ִ��
		
		//====================20080801===============================
		//д��cash�ļ���β���رմ��ļ�
		cashfile.WriteString("</find>");
		cashfile.Close();
		if (pMainWnd->curCondition!=Is_NetGroup&&pMainWnd->curCondition!=Is_NetGroup_Level)
			sprintf_s(cashpath, 1024, "%s\\temp\\cash.xml",CurrentPath);
		
		page1 = 0;
		totalPage1 = totalCount/20+1;
		//	ShowAllResult(cashpath,page1);//edit by Lins 2008/08/22
		//====================20080801===============================
		
		//������һ����Դ��Ϣ
		info->abst1[status-1] = (char)wParam;
		*((int*)&(info->abst2[(status-1)*sizeof(int)])) = (int)lParam;
		
		endTime = GetTickCount();
		
		/*	���ط���ͼƬ*/
		if(!quit){
			HANDLE handle = (HANDLE)::_beginthreadex(NULL,0,_LoadSearchIcon,(LPVOID)(this),CREATE_SUSPENDED, NULL);
			threadList.AddTail(handle);
			::ResumeThread(handle);
		}
		
		ZeroMemory(info->title,200);
		totalCount+=(int)lParam;
		
		//	sprintf(info->title, "�������  ������%d���鼮   ��ʱ%d.%d��", totalCount,(endTime-startTime)/1000,(endTime-startTime)%1000);
		m_plist->SetItemText(0,0,info->title);
		m_plist->RedrawItems(0,0);
		
		//���÷�ҳ����
		int temp = m_plist->GetItemCount();
		m_plist->InsertItem( temp, "��ҳ");
		FINDINFO* movepageinfo = (FINDINFO*)malloc(sizeof(FINDINFO));
		ZeroMemory(movepageinfo, sizeof(FINDINFO));
		movepageinfo->src = src_movepage_bottom;
		m_plist->SetItemData(temp, (DWORD)movepageinfo);
		
		//������������
		CRect rect;	//ͷ����
		m_plist->GetSubItemRect(0,0,LVIR_BOUNDS,rect);
		CRect rectangle(rect.left+50, rect.top+75, rect.right-30, rect.top+88);
		
		int textWid = 68;
		int n = SEARCHSOURCES;
		//	for(int index = 0; index < SEARCHSOURCES; index++)
		for(int index = 0; index < strlen(info->abst1); index++)
		{	
			CRect  strRect(rectangle.left + index * textWid,rectangle.top,rectangle.left + (index + 1) * textWid,rectangle.bottom);	
			//CRect strRect(rectangle.left+rectangle.Width()*index/SEARCHSOURCES, rectangle.top,rectangle.left+10+rectangle.Width()*(index+1)/SEARCHSOURCES, rectangle.bottom);
			clkRect[index].CopyRect(&strRect);
		}
		
		// ���� server�б�group �� grouplevel �е�searchCount��
		if(pMainWnd->curCondition==Is_NetGroup_Level||pMainWnd->curCondition==Is_NetGroup){
			pMainWnd->PostMessage(WM_UPDATE_GUIDETREECOUNT,(WPARAM)totalCount);
			UpdateSearchCount(pMainWnd->curCondition, pMainWnd->curID, totalCount);
		}
		
		//added by zuoss 20081203�����ȴʵ����¼
		else
		{	
			char* base64key = (char*)encode2((unsigned char*)(LPCSTR)searchkey,strlen(searchkey));
			char WebPath[1024] = {0};		
			sprintf_s(WebPath, 1024, "%s/search/recordSearch.asp?username=%s&keyword=%s&searchtime=%d&totalcount=%d",
				AspServer,UserName, base64key,(endTime-startTime)/1000,totalCount);
			
			char Tadd[_MAX_PATH] = {0};
			sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\updatehotkey.txt",CurrentPath);
			//DownLoad(WebPath,Tadd, quit, socketList, socketlistmutex);
			try
			{
				if(!DownLoad(WebPath,Tadd, pView->quit, pView->socketList, pView->socketlistmutex))
				{
					if (pMainWnd->m_quit)
					{
						return 0;
					}
					Tips("�����ȴ�ʧ��",td_tips,2);
				}	
			}
			catch(_com_error& e)
			{
				return FALSE;
			}
			DeleteFile(Tadd);
			free(base64key);
		}
		if (pMainWnd->m_quit)
		{
			return FALSE;
		}
		//end
		status = 0;
		gbOnSearching = FALSE;//zhanganzhan on 20081023
		Tips("�����Ѿ�����ˣ�",td_tips,3);
		//��¼������ʷ 
		/*
		char WebFileName[1024] = {0};
		char* base64key = (char*)encode2((unsigned char*)(LPCSTR)m_keyword,m_keyword.GetLength());
		BOOL flag = FALSE;
		char WebPath[1024] = {0};
		sprintf(WebPath, "%s/search/recordsearch.asp?username=%s&keyword=%s&searchtime=%d&totalcount=%d",
		ReadmanPath,UserName, base64key,(endTime-startTime)/1000,pMainWnd->m_listdlg->m_findlist.GetItemCount()-1);
		DownloadEx(WebPath,"tmp.tmp",&flag);
		free(base64key);
		*/
		//	Tips("�������!");
}
catch(...){
#ifdef _DEBUG
	Tips("OnTaskFinished");
#endif
}
quit = FALSE;
pMainWnd->PostMessage(WM_SEARCH_FINISHED);

return TRUE;
}

void CMainListView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	
	if(pNMListView->iItem<0)
		return;
	
	if(style != style_list)
		return;
	
	DWORD data = m_plist->GetItemData(pNMListView->iItem);
	if(!data)
		return;
	free((LPVOID)data);
}

void CMainListView::OnDestroy() 
{
	StopThread();
	CListView::OnDestroy();
}

void CMainListView::ShowFolder(CString Path, int type)
{
	StopThread();
	m_plist->DeleteAllItems();
	SetType(type);
	imgList.DeleteImageList();
	
	if(style_list == style){
		ChangeToList();
		imgList.Create(THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST, ILC_COLOR24, 0, 1);
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		imgList.SetImageCount(0);
	}
	else{
		ChangeToReport();
		imgList.Create(16, 16, ILC_COLOR24, 0, 1);
		CBitmap	bitmap;
		bitmap.LoadBitmap(IDB_IMGLIST_FILE);
		imgList.Add(&bitmap, RGB(236,233,214));
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		bitmap.DeleteObject();
	}
	
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
		
		int count = m_plist->GetItemCount();
		
		DiskBookInfo* info = (DiskBookInfo*)malloc(sizeof(DiskBookInfo));
		ZeroMemory(info, sizeof(DiskBookInfo));
		strcpy_s(info->BookName, 200, find.GetFileTitle());
		CTime cdate;
		find.GetLastWriteTime(cdate);
		strcpy_s(info->CreateDate, 50, cdate.Format("%Y-%m-%d %H:%M:%S"));
		
		CString name = find.GetFileName();
		int index = name.ReverseFind('.');
		if(index>=0)
			strcpy_s(info->FileType, 50, name.Right(name.GetLength()-index-1));
		strcpy_s(info->Path, _MAX_PATH, find.GetFilePath());
		sprintf_s(info->Size, 50, "%d.%d KB", find.GetLength()/1000, find.GetLength()%1000);
		info->syn = ExistSYN(find.GetFilePath());
		
		if(style_list == style){
			m_plist->InsertItem(count, "", count);
			m_plist->SetItemData(count, (DWORD)info);
			CImageList* il = m_plist->GetImageList(LVSIL_SMALL);
			il->SetImageCount(count+1);
			CxImage img;
			CString picPath = GetFileIcon(GetFileTypeFromExt(info->FileType));
			if(!picPath.IsEmpty()){
				if(img.Load(picPath, CXIMAGE_FORMAT_BMP)){
					CBitmap bmp;
					CBitmap* oldBmp;
					CDC* pDC = GetDC();
					CDC memDC;
					memDC.CreateCompatibleDC(pDC);
					bmp.CreateCompatibleBitmap(pDC, THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST);
					oldBmp = memDC.SelectObject(&bmp);
					memDC.FillSolidRect(0,0,THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST,RGB(0xff,0xff,0xff));
					img.Draw(memDC.m_hDC);
					memDC.SelectObject(oldBmp);
					il->Replace(count, &bmp, NULL);
					bmp.DeleteObject();
					memDC.DeleteDC();
					ReleaseDC(pDC);
				}
				img.Destroy();
			}
		}
		else{
			//ͬ��", "����","����ʱ��","����","��С(KB)","·��
			UINT icon = GetSmallIcon(GetFileTypeFromExt(info->FileType));
			m_plist->InsertItem(count, "", icon);
			m_plist->SetItemText(count, 1, info->BookName);
			m_plist->SetItemText(count, 2, info->CreateDate);
			m_plist->SetItemText(count, 3, info->FileType);
			m_plist->SetItemText(count, 4, info->Size);
			m_plist->SetItemText(count, 5, info->Path);
			if(info->syn == syn_yes)
				m_plist->SetCheck(count);
			free(info);
		}
	}
	find.Close();
}

void CMainListView::StopThread()
{
	if(!threadList.IsEmpty()){
 		quit = TRUE;
	}
	try{
		if(!socketList.IsEmpty()){
			POSITION pos = socketList.GetHeadPosition();
			while(pos){
				SOCKET s = socketList.GetNext(pos);
				::closesocket(s);
			}
		}
		
		if(!threadList.IsEmpty())
		{
			POSITION pos = threadList.GetHeadPosition();
			while(pos)
			{
				HANDLE hdl_thread = threadList.GetNext(pos);
 				while(quit)
 				{
 					if(::WaitForSingleObject(hdl_thread, 0) == WAIT_OBJECT_0)
 						break;
					MSG msg;
					while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)&&(!pMainWnd->m_quit)&&quit)
					{
						if (!AfxGetApp()->PumpMessage())
							break; 
					}
				}
			}
			threadList.RemoveAll();//added by zuoss 20090108
		}	
		
	}
	catch(...){
#ifdef _DEBUG
		AfxMessageBox("StopThread()");
#endif
	}
	quit = FALSE;
 	
}

void CMainListView::ShowCollectYearFile(DWORD filetype, int year, DWORD currentPage)
{
	StopThread();
	m_plist->DeleteAllItems();
	SetType(Is_CollectList);
	imgList.DeleteImageList();
	if(style_list == style){
		ChangeToList();
		imgList.Create(THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST, ILC_COLOR24, 0, 1);
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		imgList.SetImageCount(0);
	}
	else{
		ChangeToReport();
		imgList.Create(16, 16, ILC_COLOR24, 0, 1);
		CBitmap	bitmap;
		bitmap.LoadBitmap(IDB_IMGLIST_FILE);
		imgList.Add(&bitmap, RGB(236,233,214));
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		bitmap.DeleteObject();
	}
	try{
		_RecordsetPtr m_pRecordset;
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			return;
		char SQL[1024] = {0};
		sprintf_s(SQL, 1024, "SELECT [title],[addr],[cdate],[synFlag],[size] FROM loc WHERE type=%d and year=%d ", filetype, year);
		
		if(orderstyle == order_by_title)
			strcat_s(SQL, 1024, "ORDER BY [title]");
		else if(orderstyle == order_by_filetype)
			strcat_s(SQL, 1024, "ORDER BY [type]");
		else if(orderstyle == order_by_filesize)
			strcat_s(SQL, 1024, "ORDER BY [size]");
		else
			strcat_s(SQL, 1024, "ORDER BY [cdate]");
		if(basc)
			strcat_s(SQL, 1024, " DESC");
		
		m_pRecordset->PageSize = LOCALSEARCHMAXIUM;
		hr = m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(),
			adOpenStatic, adLockReadOnly, adCmdText);
		totalCount = m_pRecordset->GetRecordCount();
		totalPage = m_pRecordset->PageCount;
		m_pRecordset->PutAbsolutePage((enum PositionEnum)(currentPage));
		
		page = currentPage;
		//m_filetype = filetype;
		//m_year = year;
		
		if(style_list == style){
			DiskBookInfo* info = (DiskBookInfo*)calloc(1, sizeof(DiskBookInfo));
			strcpy_s(info->BookName, 200, "��˫���鿴��һҳ");
			info->syn = syn_moveup;
			CImageList* il = m_plist->GetImageList(LVSIL_SMALL);
			il->SetImageCount(1);
			m_plist->InsertItem(0, info->BookName, 0);
			m_plist->SetItemData(0,(DWORD)info);
		}
		else{
			if(page>1){
				UINT icon = GetSmallIcon(syn_moveup);
				m_plist->InsertItem(0, "", icon);
				m_plist->SetItemText(0, 1, "��˫���鿴��һҳ");
			}
		}
		
		for(int nIndex=0; nIndex<LOCALSEARCHMAXIUM && !m_pRecordset->EndOfFile; nIndex++){
			try{
				DiskBookInfo* info = (DiskBookInfo*)calloc(1, sizeof(DiskBookInfo));
				strcpy_s(info->BookName, 200, GetCollectString(m_pRecordset, "title"));
				sprintf_s(info->Size, 50, "%d.%dKB", GetCollectLong(m_pRecordset, "size")/1000,
					GetCollectLong(m_pRecordset, "size")%1000);
				sprintf_s(info->CreateDate, 50, GetCollectDate(m_pRecordset, "cdate"));
				strcpy_s(info->Path, _MAX_PATH, GetCollectString(m_pRecordset, "addr"));
				strcpy_s(info->FileType, 50, GetFileExtentName(filetype));
				info->syn = GetCollectLong(m_pRecordset, "synFlag")?syn_yes:syn_no;
				int count = m_plist->GetItemCount();
				if(style_list == style){
					m_plist->InsertItem(count, info->BookName, count);
					m_plist->SetItemData(count,(DWORD)info);
					CImageList* il = m_plist->GetImageList(LVSIL_SMALL);
					il->SetImageCount(count+1);
					CxImage img;
					CString picPath = GetFileIcon(filetype);
					if(!picPath.IsEmpty()){
						if(img.Load(picPath, CXIMAGE_FORMAT_BMP)){
							CBitmap bmp;
							CBitmap* oldBmp;
							CDC* pDC = GetDC();
							CDC memDC;
							memDC.CreateCompatibleDC(pDC);
							bmp.CreateCompatibleBitmap(pDC, THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST);
							oldBmp = memDC.SelectObject(&bmp);
							memDC.FillSolidRect(0,0,THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST,RGB(0xff,0xff,0xff));
							img.Draw(memDC.m_hDC);
							memDC.SelectObject(oldBmp);
							il->Replace(count, &bmp, NULL);
							bmp.DeleteObject();
							memDC.DeleteDC();
							ReleaseDC(pDC);
						}
						img.Destroy();
					}
				}
				else{
					UINT icon = GetSmallIcon(filetype);
					m_plist->InsertItem(count, "", icon);
					m_plist->SetItemText(count, 1, info->BookName);
					m_plist->SetItemText(count, 2, info->CreateDate);
					m_plist->SetItemText(count, 3, info->FileType);
					m_plist->SetItemText(count, 4, info->Size);
					m_plist->SetItemText(count, 5, info->Path);
					if(info->syn==syn_yes)
						m_plist->SetCheck(count);
					free(info);
				}
			}
			catch(_com_error& e){
#ifdef _DEBUG
				Tips("void CMainListView::ShowCollectYearFile(DWORD filetype, int year, DWORD currentPage)");
				Tips(e.ErrorMessage());
#endif
			}
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
		//	m_pRecordset.Release();
		
		if(style_list == style){
			int count = m_plist->GetItemCount();
			DiskBookInfo* info = (DiskBookInfo*)calloc(1, sizeof(DiskBookInfo));
			strcpy_s(info->BookName, 200, "��˫���鿴��һҳ");
			info->syn = syn_movedown;
			CImageList* il = m_plist->GetImageList(LVSIL_SMALL);
			il->SetImageCount(count+1);
			m_plist->InsertItem(count, info->BookName, 0);
			m_plist->SetItemData(count,(DWORD)info);
		}
		else{
			if(page<totalPage){
				int count = m_plist->GetItemCount();
				UINT icon = GetSmallIcon(syn_movedown);
				m_plist->InsertItem(count, "", icon);
				m_plist->SetItemText(count, 1, "��˫���鿴��һҳ");
				m_plist->SetItemData(count, syn_movedown);
			}
		}
}
catch(_com_error& e){
#ifdef _DEBUG
	Tips("void CMainListView::ShowCollectYearFile(DWORD filetype, int year, DWORD currentPage)");
	Tips(e.ErrorMessage());
#endif
}
catch(...){
#ifdef _DEBUG
	Tips("void CMainListView::ShowCollectYearFile(DWORD filetype, int year, DWORD currentPage)");
#endif
}
}

LRESULT CMainListView::OnThreadFinished(WPARAM w,LPARAM l)
{
	HANDLE thread = (HANDLE)w;
	POSITION pos = threadList.Find(thread);
	if(pos)
		threadList.RemoveAt(pos);
	return TRUE;
}

unsigned __stdcall CMainListView::_LoadSearchIcon(LPVOID pParam)
{
	CMainListView* pView = (CMainListView*)pParam;
	CListCtrl* plist = pView->m_plist;
	CImageList* imgList = plist->GetImageList(LVSIL_SMALL);
	
	HBRUSH hBrushBk=::CreateSolidBrush(RGB(255, 255, 255));
	RECT rcBorder;
	rcBorder.left=rcBorder.top=0;
	rcBorder.right=THUMBNAIL_WIDTH;
	rcBorder.bottom=THUMBNAIL_HEIGHT;
	
	for(int index=0; index<plist->GetItemCount() && !pView->quit; index++){
		FindIconListInfo listInfo;
		if(!pView->coverMap.Lookup(index,listInfo))
			continue;
		
		CxImage image;
		char Path[_MAX_PATH] = {0};
		if(strlen(listInfo.cover)==0){
			sprintf_s(Path, _MAX_PATH, "%s/system/zwfm.gif", CurrentPath);
			image.Load(Path, CXIMAGE_FORMAT_GIF);
			if(!image.IsValid()){
				image.Destroy();
				continue;
			}
		}
		else{
			sprintf_s(Path, _MAX_PATH, "%s/cover", CurrentPath);
			_mkdir(Path);
			strcat_s(Path, _MAX_PATH, "/search");
			_mkdir(Path);
			memset(Path, 0, _MAX_PATH);
			sprintf_s(Path,  _MAX_PATH, "%s/cover/search/%d.jpg", CurrentPath, index);
			char WebPath[_MAX_PATH] = {0};
			//sprintf(WebPath, "%s", info->cover);
			sprintf_s(WebPath,  _MAX_PATH, "%s", listInfo.cover);
			
			if(!DownLoad(WebPath,Path,pView->quit, pView->socketList)){
				if (pMainWnd->m_quit)
				{
					return 0;
				}
				memset(Path, 0, _MAX_PATH);
				sprintf_s(Path,  _MAX_PATH, "%s/system/zwfm.gif", CurrentPath);
				image.Load(Path, CXIMAGE_FORMAT_GIF);
				if(!image.IsValid()){
					image.Destroy();
					continue;
				}
			}
			else{
				if (pMainWnd->m_quit)
				{
					return 0;
				}
				image.Load(Path, CXIMAGE_FORMAT_JPG);
				if(!image.IsValid()){
					image.Load(Path, CXIMAGE_FORMAT_GIF);
					if(!image.IsValid()){
						image.Destroy();
						memset(Path, 0, _MAX_PATH);
						sprintf_s(Path, _MAX_PATH, "%s/system/zwfm.gif", CurrentPath);
						image.Load(Path, CXIMAGE_FORMAT_GIF);
						if(!image.IsValid()){
							image.Destroy();
							continue;
						}
					}
				}
			}
		}
		if (pView->quit)
		{
			return 0;
		}
		int XDest, YDest, nDestWidth, nDestHeight;
		float fImgRatio=(float)image.GetHeight()/image.GetWidth();
		
		CClientDC cdc(plist);
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
		HBITMAP bm;
		if(listInfo.src != src_readman){
			XDest = 16;
			YDest = 18;
			nDestWidth = THUMBNAIL_WIDTH_LIST-18;
			nDestHeight = THUMBNAIL_HEIGHT_LIST-20;
			
			bm = CreateCompatibleBitmap(cdc.m_hDC, THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST);
			HBITMAP pOldBitmapImage = (HBITMAP)SelectObject(hDC,bm);
			::FillRect(hDC, &rcBorder, hBrushBk);
			
			CxImage tImg;
			char bmpPath[_MAX_PATH] = {0};
			sprintf_s(bmpPath, _MAX_PATH, "%s/system/small.bmp", CurrentPath);
			tImg.Load(bmpPath, CXIMAGE_FORMAT_BMP);
			if(tImg.IsValid()){
				tImg.Draw(hDC,0,9,THUMBNAIL_WIDTH_LIST,THUMBNAIL_HEIGHT_LIST-9);
				image.Stretch(hDC, XDest, YDest, nDestWidth, nDestHeight);
				tImg.Destroy();
			}
			SelectObject(hDC, pOldBitmapImage);
		}
		else{	//rss
			RECT readmanBorder;
			readmanBorder.left=rcBorder.top=0;
			readmanBorder.right=135;
			readmanBorder.bottom=THUMBNAIL_HEIGHT;
			bm = CreateCompatibleBitmap(cdc.m_hDC, 135, THUMBNAIL_HEIGHT_LIST);
			HBITMAP pOldBitmapImage = (HBITMAP)SelectObject(hDC,bm);
			::FillRect(hDC, &rcBorder, hBrushBk);
			
			CxImage tImg;
			char bmpPath[_MAX_PATH] = {0};
			sprintf_s(bmpPath, _MAX_PATH, "%s/system/rssbg.bmp", CurrentPath);
			tImg.Load(bmpPath, CXIMAGE_FORMAT_BMP);
			if(tImg.IsValid()){
				tImg.Draw(hDC,0,0,135,THUMBNAIL_HEIGHT_LIST);
				if(image.GetWidth()==120 &&  image.GetHeight()==60)
					image.Draw(hDC, 2,20,120,60);
				else{
					float width = image.GetWidth();
					float height = image.GetHeight();
					if(width/height>2){
						XDest = 3;
						nDestWidth = 119;
						nDestHeight = height*(119.0/width);
						YDest = 20+(60-nDestHeight)/2;
					}
					else{
						YDest = 20;
						nDestHeight = 60;
						nDestWidth = width*(60.0/height);
						XDest = 3+(119-nDestWidth)/2;
					}
					image.Draw(hDC, XDest, YDest, nDestWidth, nDestHeight);
				}
				tImg.Destroy();
			}
			SelectObject(hDC, pOldBitmapImage);
		}
		CBitmap bitmap;
		bitmap.Attach(bm);
		imgList->Replace(index, &bitmap, NULL);
		plist->RedrawItems(index,index);
		DeleteDC(hDC);
		DeleteObject(bm);
		bitmap.DeleteObject();
		image.Destroy();
	}
	DeleteObject(hBrushBk);
	if(!pView->quit){
		plist->RedrawItems(0,plist->GetItemCount());
	}
	pView->coverMap.RemoveAll();
	//	AfxMessageBox("finished!");
	
	pView->PostMessage(WM_THREAD_FINISHED,(WPARAM)::GetCurrentThread());
	
	_endthreadex(0);
	return 0;
}

void CMainListView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem == -1 || pNMListView->uNewState != (LVIS_SELECTED|LVIS_FOCUSED))
		return;
	
	DWORD data = m_plist->GetItemData(pNMListView->iItem);
	if(showtype == Is_FindList){
		if(!data)
			return;
		FINDINFO* info = (FINDINFO*)data;
		if(info->src != src_movepage_bottom
			&& info->src != src_movepage_top
			&& info->src != src_msg && info->src != src_order){
			char* link = (char*)malloc(1024);
			ZeroMemory(link,1024);
			strcpy_s(link, 1024, info->link);
			pMainWnd->PostMessage(WM_OPEN_URL_NOT_SHOW, (WPARAM)link);
			
			//	pMainWnd->PostMessage(WM_OPEN_URL, (WPARAM)link);
		}
	}
	else if(style_report == style){
		if(pNMListView->uOldState == 0 && pNMListView->uNewState == 0)   
			return; //   No   change   
		// Old check box state   
		BOOL bPrevState = (BOOL)(((pNMListView->uOldState&LVIS_STATEIMAGEMASK)>>12)-1);
		if(bPrevState<0)//On startup there's no previous state     
			bPrevState=0;//so assign as false (unchecked)   
		//New check box state   
		BOOL bChecked = (BOOL)(((pNMListView->uNewState&LVIS_STATEIMAGEMASK)>>12)-1);
		if(bChecked<0)//On non-checkbox notifications assume false   
			bChecked=0;     
		if(bPrevState==bChecked)//No change in check box   
			return;
		else{
			CString path = m_plist->GetItemText(pNMListView->iItem,5);
			if(Is_CollectList == showtype){//��ʽ�ɼ�
				char SQL[2048] = {0};
				sprintf_s(SQL, 2048, "UPDATE loc SET synFlag=%d WHERE addr=\"%s\"", bChecked, path);
				m_pConnection->Execute(SQL, NULL, adCmdText);
			}
			else if(Is_RecentlyList == showtype || Is_FileList == showtype
				||Is_PushList == showtype || Is_SDDiskList==showtype){
				if(bChecked)
					CreateSYN(path);					
				else
					DeleteSYN(path);
			}
			else if(Is_OrdList == showtype){
				CString strId = m_plist->GetItemText(hit_index,5);
				int* id = new int;
				*id = atoi((LPCSTR)strId);
				if(bChecked){
					HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _Select,id,CREATE_SUSPENDED,NULL); 
					threadList.AddTail(hd);
					::ResumeThread(hd);
                    //�����û��ġ���������Ϊ��¼    add by Lins 20081230
					char behavior[1024] = {0};
					sprintf_s(behavior, 1024, "������\"%s\"",m_plist->GetItemText(hit_index,1));
					char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
					pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);
				}
				else{
					HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _DisSelect,id,CREATE_SUSPENDED,NULL); 
					threadList.AddTail(hd);
					::ResumeThread(hd);
					//�����û��ġ���������Ϊ��¼    add by Lins 20081230
					char behavior[1024] = {0};
					sprintf_s(behavior, 1024, "�˶���\"%s\"",m_plist->GetItemText(hit_index,1));
					char* behaviorbase64key = (char*)encode2((PUCHAR)(LPCSTR)behavior,strlen(behavior));
					pMainWnd->PostMessage(WM_RECORD_BEHAVIOR,(WPARAM)behaviorbase64key);
				}
				m_plist->SetCheck(hit_index,!bChecked);
			}
		}
	}
	*pResult = 0;
}

void CMainListView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CListView::OnLButtonDown(nFlags, point);
	
	int index = m_plist->HitTest(point);
	if(index == -1)
		return;
	
	DWORD data = m_plist->GetItemData(index);
	if(!data)
		return;
	
	if(data==syn_moveup || data==syn_movedown)
		return;
	
	if(showtype == Is_FindList)
		ClickFindList(index, (FINDINFO*)data, point);
	else if(showtype == Is_OrdList)
		ClickOrderHead(index, (OrderDetailInfo*)data,point);
	//ClickOrderList(index, (OrderDetailInfo*)data, point);
	else if(showtype == Is_FileList || showtype == Is_CollectList
		||showtype == Is_RecentlyList || showtype == Is_SDDiskList
		||showtype == Is_PushList)
		ClickFileList(index, (DiskBookInfo*)data, point);
	else if(showtype == Is_ShareList)
		ClickShareList(index, (ShareInfo*)data, point);
	else if (showtype == Is_NetBookList)
	{
		ClickNetBookList(index, (FINDINFO*)data, point);
	}
}

void CMainListView::ClickShareList(int nItem, ShareInfo* info, CPoint point)
{
	CRect rect;
	if(info->sid == syn_moveup){
		if(up2.PtInRect(point)){
			if(page>1){
				page--;
				//pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
				pMainWnd->ShowResourceShareFiletype(pMainWnd->m_info);
			}
			return;
		}
		if(down2.PtInRect(point)){
			if(page<totalPage){
				page++;
				//pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
				pMainWnd->ShowResourceShareFiletype(pMainWnd->m_info);
			}
			return;
		}
	}
	if(info->sid == syn_movedown){
		if(up2.PtInRect(point)){
			if(page>1){
				page--;
				//pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
				pMainWnd->ShowResourceShareFiletype(pMainWnd->m_info);
			}
			return;
		}
		if(down2.PtInRect(point)){
			if(page<totalPage){
				page++;
				//pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
				pMainWnd->ShowResourceShareFiletype(pMainWnd->m_info);
			}
			return;
		}
	}
	else{
		if(btn1Map.Lookup(nItem,rect)){
			if(rect.PtInRect(point)){//�����ť
				LPSTR url = (LPSTR)malloc(1024);
				ZeroMemory(url,1024);
				if (info->filefrom == 0)//Ⱥ������Դ
				{
					sprintf_s(url, 1024, "%s/share/sharePageToShow.asp?modelflag=5&sid=%d",AspServer, info->sid);
				}
				else
				{
					sprintf_s(url, 1024, "http://61.181.14.184:6969/FileHistoryTest.html?hash=%s", info->hash);
					//AfxMessageBox(url);
					//sprintf(url,"%s/share/main.asp?modelflag=5&sid=%d", AspServer, info->sid);
				}
				pMainWnd->PostMessage(WM_OPEN_URL, (WPARAM)url);
				return;
			}
		}
		if(btn2Map.Lookup(nItem,rect)){//zhanganzhan 090415
			if(rect.PtInRect(point))
			{
				if (info->downloadType == dl_http)
				{				
					CString filename;
					filename.Format("%s",info->title);
					
					CFileDialog fileDialog(FALSE,NULL, filename);
					if(fileDialog.DoModal()!=IDOK)
						return;
					
					DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
					ZeroMemory(missionInfo, sizeof(DownLoadMissionInfo));
					strcpy_s(missionInfo->DstPath, _MAX_PATH, fileDialog.GetPathName());
					strcpy_s(missionInfo->URL, 1024, info->url);
					strcpy_s(missionInfo->title, _MAX_PATH, fileDialog.GetFileName());
					missionInfo->fileType = info->filetype;
					missionInfo->sourceType = dl_http;
					
					pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
				}
				else if (info->downloadType==dl_p2p)
				{				
					pMainWnd->PostMessage(WM_REQUEST_RESUOURCE_FROM_SERVER, 0, (LPARAM)info->sid);
				}
				else if (info->downloadType == dl_bt)
				{
					CString filename;
					filename.Format("%s",info->title);
					
					CFileDialog fileDialog(FALSE,NULL, filename);
					if(fileDialog.DoModal()!=IDOK)
						return;

					DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
					ZeroMemory(missionInfo, sizeof(DownLoadMissionInfo));

					//��ȡĿ��·����������뵽Readings�����ļ�����
					CString dstpath;
					dstpath = fileDialog.GetPathName();
					dstpath = dstpath.Left(dstpath.ReverseFind('\\'));
					/*if (dstpath.Find("\\�ҵ�Readings�ļ���")==-1)
					{
						dstpath += "\\�ҵ�Readings�ļ���";
					}*/
					strcpy_s(missionInfo->DstPath, _MAX_PATH, dstpath);
					_mkdir(dstpath);
					CString infoPath;
					infoPath.Format("%s\\system\\sharepath.inf", CurrentPath);
					CStdioFile cf;
					if (!cf.Open(infoPath, CFile::modeReadWrite))
					{
						cf.Open(infoPath, CFile::modeCreate|CFile::modeWrite);
					}
					CString listfi;
					cf.ReadString(listfi);
					if(listfi.Find(missionInfo->DstPath)==-1)
					{
						cf.SeekToEnd();
						cf.WriteString(missionInfo->DstPath);
						cf.WriteString("\n");
					}
					cf.Close();
					

					strcpy_s(missionInfo->URL, 1024, info->url);
					strcpy_s(missionInfo->title, _MAX_PATH, filename);
					missionInfo->fileType = info->filetype;
					missionInfo->sourceType = dl_bt;
					missionInfo->fileLen = info->filesize;
					pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
				}
				
			}
			
		}
		//add by dongzhenhua 0508
		//��� ���� ��ť���� ���ﲿ�����öԻ���
		if (btn3Map.Lookup(nItem,rect)){
			if (rect.PtInRect(point))
			{
				ResourceClassify RCDialog(info->group,info->sid,pMainWnd->m_ReadOnLineView->hAllGroup,NULL);
				RCDialog.DoModal();
			}
		}
		//add by dzh 20081117 
		//��� ���ʼ����� ��ť�������ʼ�
		if (btn4Map.Lookup(nItem,rect))
		{
			if (rect.PtInRect(point))
			{
				CDialogMail mail;
				mail.resource=info->title;
				if(IDOK==mail.DoModal()){
					if(mail.issms)
						ReadingsSMS(UserName,info->UserName,info->title,AspServer,this);
					
					//�����ʼ�����
					HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _SendRequestMail,info,CREATE_SUSPENDED,NULL); 
					pMainWnd->m_threadList.AddTail(hd);
					::ResumeThread(hd);
				}
			}
		}
		//add by linshan 0610
		//��� ���� ��ť���� ���ﲿ�����öԻ���
		if (star1Map.Lookup(nItem,rect)){
			if (rect.PtInRect(point))
			{
				AfxMessageBox("chenggong");
			}
		}
	}
}

void CMainListView::ClickFileList(int nItem, DiskBookInfo *info, CPoint point)
{
	CRect rect;
	if(info->syn == syn_moveup){
		if(up1.PtInRect(point)){
			if(page>1){
				page--;
				pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
			}
			return;
		}
		if(down1.PtInRect(point)){
			if(page<totalPage){
				page++;
				pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
			}
			return;
		}
	}
	else if(info->syn == syn_movedown){
		if(up2.PtInRect(point)){
			if(page>1){
				page--;
				pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
			}
			return;
		}
		if(down2.PtInRect(point)){
			if(page<totalPage){
				page++;
				pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
			}
			return;
		}
	}
	else{
		if(btn1Map.Lookup(nItem,rect)){
			if(rect.PtInRect(point)){
				//	AfxMessageBox("����");
				LPSTR FilePath = (LPSTR)malloc(1024);
				ZeroMemory(FilePath,1024);
				strcpy_s(FilePath, 1024, info->Path);
				pMainWnd->PostMessage(WM_OPEN_FILE, (WPARAM)FilePath);
				
				return;
			}
		}
		if(btn2Map.Lookup(nItem,rect)){
			if(rect.PtInRect(point)){
				if(showtype == Is_CollectList){
					char SQL[2048] = {0};
					sprintf_s(SQL, 2048,"UPDATE loc SET synFlag=%d WHERE addr=\"%s\"", !info->syn, info->Path);
					m_pConnection->Execute(SQL, NULL, adCmdText);
					if (info->syn == syn_yes)
					{
						info->syn = syn_no;
					}
					else if (info->syn == syn_no)
						info->syn = syn_yes;
				}
				else{
					if(info->syn == syn_yes){
						DeleteSYN(info->Path);
						info->syn = syn_no;
					}
					else{
						CreateSYN(info->Path);
						info->syn = syn_yes;
					}
				}
				//	info->syn = !info->syn;
				
				
				m_plist->RedrawItems(nItem,nItem);
				return;
			}
		}
	}
}
//added by zuoss 090216
//��������ͷ��ѡ��
void CMainListView::ClickOrderHead(int nItem, OrderDetailInfo *info, CPoint point)
{
	if ((!orderAll.PtInRect(point))&& (!unOrderAll.PtInRect(point)) &&(!reverseAll.PtInRect(point)))
	{
		ClickOrderList(nItem,info,point);
	}
	else
	{
		HANDLE phandle[1000] = {0}; 
		if(!threadList.IsEmpty())
		{
			int n = 0;
			POSITION pos = threadList.GetHeadPosition();
			while(pos)
			{
				HANDLE hdl_thread = threadList.GetNext(pos);
				phandle[n] = hdl_thread;
				n++;	
			}
		}
		//�������ȫ�����ġ�
		if (orderAll.PtInRect(point))
		{
			WaitForMultipleObjects(threadList.GetCount(),phandle,TRUE,1000);
			for (int n = 1;n < m_plist->GetItemCount(); n++)
			{
				OrderDetailInfo *info = (OrderDetailInfo *)m_plist->GetItemData(n);
				info->checked = FALSE;//��δѡ�и�Ϊѡ��
				ClickOneOrder(n,info);	
			}
			//AfxMessageBox("ȫ�����ĳɹ�");
			//Tips("ȫ�����ĳɹ�",td_msg,2);
		}
		else if (unOrderAll.PtInRect(point))
		{
			WaitForMultipleObjects(threadList.GetCount(),phandle,TRUE,1000);
			for (int n = 1;n < m_plist->GetItemCount(); n++)
			{
				OrderDetailInfo *info = (OrderDetailInfo *)m_plist->GetItemData(n);
				info->checked = TRUE;//��ѡ�и�Ϊδѡ��
				ClickOneOrder(n,info);	
			}
			//AfxMessageBox("ȫ���˶��ɹ�");
			//Tips("ȫ���˶��ɹ�",td_msg,2);
		}
		else if (reverseAll.PtInRect(point))
		{
			WaitForMultipleObjects(threadList.GetCount(),phandle,TRUE,1000);
			for (int n = 1;n < m_plist->GetItemCount(); n++)
			{
				OrderDetailInfo *info = (OrderDetailInfo *)m_plist->GetItemData(n);
				ClickOneOrder(n,info);
			}
			//AfxMessageBox("��ת���ĳɹ�");
			//Tips("��ת���ĳɹ�",td_msg,2);
		}
		
	}
}

void CMainListView::ClickOneOrder(int nItem, OrderDetailInfo *info)
{
	int* id = new int;
	*id = atoi(info->id);
	if(info->checked){
		HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _DisSelect,id,CREATE_SUSPENDED,NULL); 
		threadList.AddTail(hd);
		::ResumeThread(hd);
	}
	else{
		HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _Select,id,CREATE_SUSPENDED,NULL); 
		threadList.AddTail(hd);
		::ResumeThread(hd);
	}
	info->checked = !info->checked;
	m_plist->RedrawItems(nItem,nItem);
	return;
}
//end
void CMainListView::ClickOrderList(int nItem, OrderDetailInfo *info, CPoint point)
{
	CRect rect;
	
	if(btn1Map.Lookup(nItem,rect)){
		if(rect.PtInRect(point)){
			int* id = new int;
			*id = atoi(info->id);
			if(info->checked){
				HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _DisSelect,id,CREATE_SUSPENDED,NULL); 
				threadList.AddTail(hd);
				::ResumeThread(hd);
			}
			else{
				HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _Select,id,CREATE_SUSPENDED,NULL); 
				threadList.AddTail(hd);
				::ResumeThread(hd);
			}
			info->checked = !info->checked;
			m_plist->RedrawItems(nItem,nItem);
			return;
		}
	}
	if(btn2Map.Lookup(nItem,rect))
		if(rect.PtInRect(point))
			DownLoadOrder(info);
}
void CMainListView::ShowNextSharePage()
{
	StopThread();
	char webpath[1024] = {0};
	char fpath[_MAX_PATH] = {0};
	
	char* kw_b64;
	kw_b64 = (char*)encode2((PUCHAR)(LPCSTR)searchkey,strlen(searchkey));
	
	//	sprintf(webpath,"%s/share/search_getshare.asp?key=%s&base64=1&page=%d",AspServer,kw_b64,sharePage);
	//sprintf(webpath,"%s/share/search_getshare_1.asp?key=%s&base64=1&page=%d",AspServer,kw_b64,sharePage);
	string ss = tran_16bases((unsigned char*)searchkey.GetBuffer(0),searchkey.GetLength());
	sprintf_s(webpath, 1024, "http://61.181.14.184:6969/SearchName.html?name=%s&page=%d",ss.c_str(),sharePage);
	
	if(pMainWnd->curCondition == Is_NetGroup || pMainWnd->curCondition == Is_NetGroup_Level)
	{
		CTime ct = CTime::GetCurrentTime();
		int day = ct.GetDay();
		//������������Ļ���ҳ
		sprintf_s(fpath, _MAX_PATH, "%s\\temp\\groupcache\\share\\findres%d_%d_%d_%d.xml",CurrentPath,pMainWnd->curCondition,pMainWnd->curID,day,sharePage);	
		if(!PathFileExists(fpath))
		{
			if(!DownLoad(webpath,fpath, quit, socketList, socketlistmutex))
			{
				return;	//xml ����ʧ��
			}
		}
	}
	else
	{
		//�ȴ������������������Ļ���ҳ��
		sprintf_s(fpath, _MAX_PATH, "%s\\temp\\findres14_%d.xml",CurrentPath,sharePage);
		if(!DownLoad(webpath,fpath, quit, socketList, socketlistmutex))
		{
			return;	//xml ����ʧ��
		}
	}
	//����fpath ��ʾ
	//ɾ�������һ�����������ֻ������һ��洢��������������
	
	int listsum = m_plist->GetItemCount();//
	int listcount = listsum - 1;
	for (listcount; listcount > 0; --listcount)
	{
		m_plist->DeleteItem(listcount);
	}
	
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	int count = 0;
	try
	{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml ����ʧ��
		if(!pDoc->load(fpath))
			throw 2;	//xml ����ʧ��
		pNodeList = pDoc->selectNodes("find/item");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		if(pNodeList)
		{
			for(int pos1 = 0; pos1 < pNodeList->length && !quit; pos1++)
			{
				CSingleLock SingleLock(&mutex);
				SingleLock.Lock();
				if(SingleLock.IsLocked())
				{
					CString title, cover, link, abst1, abst2, abst3,score,isalive;
					count = m_plist->GetItemCount();
					if (count > 20)
					{
						break;
					}		
					title = GetXmlValue(pNodeList, "title", pos1);
					cover = GetXmlValue(pNodeList, "cover", pos1);
					link = GetXmlValue(pNodeList, "link", pos1);
					abst1 = GetXmlValue(pNodeList, "abst1", pos1);
					abst2 = GetXmlValue(pNodeList, "abst2", pos1);
					abst3 = GetXmlValue(pNodeList, "abst3", pos1);
					score = GetXmlValue(pNodeList,"score",pos1);
					isalive = GetXmlValue(pNodeList,"isalive",pos1);
					
					FINDINFO* finfo;
					finfo = (FINDINFO*)malloc(sizeof(FINDINFO));
					memset(finfo, 0, sizeof(FINDINFO));
					
					strcpy_s(finfo->title, 200, title);
					strcpy_s(finfo->cover, 1024, cover);
					strcpy_s(finfo->link, 1024, link);
					strcpy_s(finfo->abst1, 1000, abst1);
					strcpy_s(finfo->abst2, 1000, abst2);
					strcpy_s(finfo->abst3, 1000,abst3);
					strcpy_s(finfo->score, 100, score);
					
					finfo->src = src_share;
					finfo->icon = count;
					finfo->isAlive = atoi((LPCSTR)isalive);
					
					FindIconListInfo listInfo = {0};
					
					listInfo.src = finfo->src;
					strcpy_s(listInfo.cover, 1024, finfo->cover);
					coverMap.SetAt(count,listInfo);
					SetRedraw(FALSE);
					imgList.SetImageCount(count+1);
					
					CBitmap bitmap;
					bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
					imgList.Replace(count,&bitmap,NULL);
					bitmap.DeleteObject();
					
					m_plist->InsertItem(count,title,count);
					m_plist->SetItemData(count,(DWORD)finfo);
					
					SetRedraw(TRUE);
				}
				SingleLock.Unlock();
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//�Ѹ���
			pNodeList.Release();
		case 2:		//��ʽ����
			pDoc.Release();
			break;
		}
		return;
	}
}

//added by zuoss 090527 �����������Ӧ
void CMainListView::ClickNetBookList(int nItem, FINDINFO *info, CPoint point)
{
	
	CRect rect;
	if (info->src == Is_NetBook || !IS_istation)
	{
		if(up1.PtInRect(point))
		{
			if(page > 1)
			{
				page--;
				pMainWnd->ShowNetBookResource(pMainWnd->m_info);
			}
			return;
		}
		if(down1.PtInRect(point))
		{
			if(page < totalPage)
			{
				page++;
				pMainWnd->ShowNetBookResource(pMainWnd->m_info);
			}
			return;
		}
		CRect rect1; //�����ť
		if(btn1Map.Lookup(nItem, rect1))	
		{
			LPSTR URL = (LPSTR)malloc(1024);
			ZeroMemory(URL,1024);
			strcpy_s(URL, 1024, info->link);
			pMainWnd->PostMessage(WM_OPEN_URL_NOT_SHOW,(WPARAM)URL);
		}
		
		CRect rect2; //���鰴ť
		if(btn2Map.Lookup(nItem, rect2))	
		{
			if(rect2.PtInRect(point))
			{
				::ShellExecute(NULL, "open", "UrlToWol.exe", info->link, CurrentPath, SW_SHOW);
			}
		}
	}
	else if (info->src == Is_iType || IS_istation)
	{
		if(up1.PtInRect(point))
		{
			if(page > 1)
			{
				page--;
				pMainWnd->ShowiBook(itypeid,ichannelid);
			}
			return;
		}
		if(down1.PtInRect(point))
		{
			if(page < totalPage)
			{
				page++;
				pMainWnd->ShowiBook(itypeid,ichannelid);
			}
			return;
		}
		CRect rect1; //�����ť
		if(btn1Map.Lookup(nItem, rect1))	
		{
			LPSTR URL = (LPSTR)malloc(1024);
			ZeroMemory(URL,1024);
			strcpy_s(URL, 1024, info->link);
			pMainWnd->PostMessage(WM_OPEN_URL_NOT_SHOW,(WPARAM)URL);
		}
		
		CRect rect2; //������Դ��ť
		if(btn2Map.Lookup(nItem, rect2))	
		{
			if(rect2.PtInRect(point))
			{
				DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
				ZeroMemory(missionInfo, sizeof(DownLoadMissionInfo));
				strcpy_s(missionInfo->UserName, 30, UserName);
				strcpy_s(missionInfo->title, _MAX_PATH, info->title + (CString)".wol");//need modified
				strcpy_s(missionInfo->IP, 20, "");
				strcpy_s(missionInfo->URL, 1024, info->abst3);
				strcpy_s(missionInfo->MD5, MD5LEN, "");
				strcpy_s(missionInfo->DstPath, _MAX_PATH, "");
				missionInfo->fileType = 0;
				missionInfo->port = 0;
				missionInfo->fileLen = 0;
				missionInfo->sourceType = dl_mail;
				pMainWnd->PostMessage(WM_FILEDOWNLOAD, (WPARAM)missionInfo);
			}
		}
	}
}

void CMainListView::ClickFindList(int nItem, FINDINFO *info, CPoint point)
{	
	HCURSOR   hc;
	if(info->src == src_movepage_top || info->src == src_msg)
	{
		//added by zuoss 20081029
		CRect procRect;
		int num = 0;
		for (int i = 0; i < SEARCHSOURCES; i++)
		{
			num = info->abst1[i];
			if (headprocess[i].Lookup(num,procRect))
			{
				if (procRect.PtInRect(point))
				{
					hc=LoadCursor(NULL,IDC_WAIT);
					SetCursor(hc);
					ShowOneResource(num);
				}
			}
		}
		//end add
		/*		CRect procRect;
		int num = info->abst1[0];
		if(headprocess[0].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		{
		//��MainListView��ʾ �����������
		//added by zuoss 20081029
		hc=LoadCursor(NULL,IDC_WAIT);
		SetCursor(hc);
		ShowOneResource(num);
		//	AfxMessageBox("00");
		}
		}
		num = info->abst1[1];
		if(headprocess[1].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		{
		hc=LoadCursor(NULL,IDC_WAIT);
		SetCursor(hc);
		ShowOneResource(num);
		//			AfxMessageBox("11");
		}
		}
		num = info->abst1[2];
		if(headprocess[2].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		ShowOneResource(num);
		//			AfxMessageBox("22");
		}
		num = info->abst1[3];
		if(headprocess[3].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		ShowOneResource(num);
		//			AfxMessageBox("33");
		}
		num = info->abst1[4];
		if(headprocess[4].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		ShowOneResource(num);
		//			AfxMessageBox("44");
		}
		num = info->abst1[5];
		if(headprocess[5].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		ShowOneResource(num);
		//			AfxMessageBox("55");
		}
		num = info->abst1[6];
		if(headprocess[6].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		ShowOneResource(num);
		//			AfxMessageBox("66");
		}
		num = info->abst1[7];
		if(headprocess[7].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		ShowOneResource(num);
		//			AfxMessageBox("77");
		}
		num = info->abst1[8];
		if(headprocess[8].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		ShowOneResource(num);
		//			AfxMessageBox("88");
		}
		num = info->abst1[9];
		if(headprocess[9].Lookup(num, procRect))
		{
		if(procRect.PtInRect(point))
		ShowOneResource(num);
		//		AfxMessageBox("99");
		}
		
		  num = info->abst1[10];
		  if(headprocess[10].Lookup(num, procRect))
		  {
		  if(procRect.PtInRect(point))
		  ShowOneResource(num);
		  //		AfxMessageBox("99");
		  }
		  
			num = info->abst1[11];
			if(headprocess[11].Lookup(num, procRect))
			{
			if(procRect.PtInRect(point))
			ShowOneResource(num);
			//		AfxMessageBox("99");
			}
			*/
			
			/*********************
			//add by dzh 080716
			if (dwfl.PtInRect(point))
			{
			curcondition = 1;
			m_plist->RedrawItems(0,0);
			}
			if (xxfw.PtInRect(point))
			{
			curcondition = 2;
			m_plist->RedrawItems(0,0);
			}
			if (zsfw.PtInRect(point))
			{
			curcondition = 3;
			m_plist->RedrawItems(0,0);
			}
			if (zkfw.PtInRect(point))
			{
			curcondition = 4;
			m_plist->RedrawItems(0,0);
			}
			if (djhd.PtInRect(point))
			{
			curcondition = 5;
			m_plist->RedrawItems(0,0);
			}
			//
***********************/

//added by zuoss 2008.07.29

if (kjfl.PtInRect(point))
{
	curcondition = 1;
	m_plist->RedrawItems(0,0);
}
if (duzl.PtInRect(point))
{
	curcondition = 2;
	m_plist->RedrawItems(0,0);
}
if (wjsx.PtInRect(point))
{
	curcondition = 3;
	m_plist->RedrawItems(0,0);
}
if (yjxx.PtInRect(point))
{
	curcondition = 4;
	m_plist->RedrawItems(0,0);
}
if (djhd.PtInRect(point))
{
	curcondition = 5;
	m_plist->RedrawItems(0,0);
}
//end add
if (shijian.PtInRect(point))
{
	
	char path[_MAX_PATH] ={0};
	sprintf_s(path, _MAX_PATH, "%s1.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (daxiao.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s2.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (gongxiangshu.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s3.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (wendingdu.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s4.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}

if (zuixin.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s5.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (zuikuai.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s6.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (zuijurenqi.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s7.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}	
if (zuidazhekou.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s8.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (zuidijiage.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s9.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (dijiajuan.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s10.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (zengpin.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s11.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (renqi.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s12.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}	
if (zuixin.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s13.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (tuijian.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s14.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}
if (jingdianhuigu.PtInRect(point))
{
	char path[_MAX_PATH]={0};
	sprintf_s(path, _MAX_PATH, "%s15.xml",pMainWnd->currentkeyword);
	ShowOneClass(path);
}

//add by dzh 080714
if (kaoshi.PtInRect(point))
{
// 	if (gbOnSearching == TRUE)
// 	{
// 		MessageBox("�������������Ժ�...");
// 		return;
// 	}
// 	gbOnSearching = TRUE;
	CString keywords;
	keywords = pMainWnd->currentkeyword +" ����";
	char* s_keyword = (char*)malloc(keywords.GetLength()+1);
	ZeroMemory(s_keyword, keywords.GetLength()+1);
	strcpy_s(s_keyword, keywords.GetLength()+1, keywords);
	this->page = 1;
	this->keyWord = s_keyword;
	this->headflag = 1;			//�����Ƿ���ͷ��
	this->Search(s_keyword);
	this->headflag = 0;
}
if(xiti.PtInRect(point))
{
// 	if (gbOnSearching == TRUE)
// 	{
// 		MessageBox("�������������Ժ�...");
// 		return;
// 	}
// 	gbOnSearching = TRUE;
	CString keywords;
	keywords = pMainWnd->currentkeyword +" ϰ��";
	char* s_keyword = (char*)malloc(keywords.GetLength()+1);
	ZeroMemory(s_keyword, keywords.GetLength()+1);
	strcpy_s(s_keyword, keywords.GetLength()+1, keywords);
	//		AfxMessageBox(s_keyword);
	this->page = 1;
	this->keyWord = s_keyword;
	this->headflag = 1;
	this->Search(s_keyword);
	this->headflag = 0;
}
if(kejian.PtInRect(point))
{
// 	if (gbOnSearching == TRUE)
// 	{
// 		MessageBox("�������������Ժ�...");
// 		return;
// 	}
// 	gbOnSearching = TRUE;
	CString keywords;
	keywords = pMainWnd->currentkeyword +" ppt";
	char* s_keyword = (char*)malloc(keywords.GetLength()+1);
	ZeroMemory(s_keyword, keywords.GetLength()+1);
	strcpy_s(s_keyword, keywords.GetLength()+1, keywords);
	//		AfxMessageBox(s_keyword); 
	this->page = 1;
	this->keyWord = s_keyword;
	this->headflag = 1;
	this->Search(s_keyword);
	this->headflag = 0;
}
if(jiaocai.PtInRect(point))
{
// 	if (gbOnSearching == TRUE)
// 	{
// 		MessageBox("�������������Ժ�...");
// 		return;
// 	}
// 	gbOnSearching = TRUE;
	CString keywords;
	keywords = pMainWnd->currentkeyword +" �̲�";
	char* s_keyword = (char*)malloc(keywords.GetLength()+1);
	ZeroMemory(s_keyword, keywords.GetLength()+1);
	strcpy_s(s_keyword, keywords.GetLength()+1, keywords);
	//		AfxMessageBox(s_keyword);
	this->page = 1;
	this->keyWord = s_keyword;
	this->headflag = 1;
	this->Search(s_keyword);
	this->headflag = 0;
}

if(mianze.PtInRect(point)){
	char WebPath[1024] = {0};
	sprintf_s(WebPath, 1024, "%s/RetailWeb/hanlinclient/ReadingsUserGuide/usersguide/declare.htm", JspServer);
	ShellExecute(NULL, "open", WebPath, NULL, NULL, SW_SHOW);
}
else if(up1.PtInRect(point))
{
	//�Ϸ�ҳ
	if (isShareFlag && sharePage > 0)
	{
		//������һҳ����������ʾ ShowNextSharePage()
		sharePage--;
		ShowNextSharePage();	
 			}
	else
 			{
		if(page1>0)
		{
			page1--;
			char CashFile[BUFFER_SIZE]={0};
			if (pMainWnd->curCondition==Is_NetGroup || pMainWnd->curCondition==Is_NetGroup_Level)
			{
				sprintf_s(CashFile, BUFFER_SIZE, "%s\\temp\\course_%d_%d.xml",CurrentPath,1,pMainWnd->curID);
			}
			else
			{
				sprintf_s(CashFile, BUFFER_SIZE, "%s\\temp\\cash.xml",CurrentPath);
			}
			
			ShowAllResult(CashFile,page1);
		}
		else
			return;	
	}
	// 			}
	/*	if(page>0)
				page--;
				else
				return;
				if(page==1){
				//pMainWnd->Search(pMainWnd->m_ShowView->m_htmlDlg.m_keyword);
				}
				else{
				CWinThread* th = AfxBeginThread(_MovePage, this);
				th->m_bAutoDelete = TRUE;
				th->ResumeThread();
}*/
	
}
else if(down1.PtInRect(point))
{
	//�·�ҳ
	if (isShareFlag)//�����Ƕ�share�·�ҳ
	{
		//������һҳ����������ʾ ShowNextSharePage()
		sharePage++;
		ShowNextSharePage();
		
	}
	else
	{
		if(page1<totalPage1-1)
		{
			page1++;
			char CashFile[BUFFER_SIZE]={0};
			if (pMainWnd->curCondition==Is_NetGroup || pMainWnd->curCondition==Is_NetGroup_Level)
			{
				sprintf_s(CashFile, BUFFER_SIZE, "%s\\temp\\course_%d_%d.xml",CurrentPath,1,pMainWnd->curID);
			}
			else
			{
				sprintf_s(CashFile, BUFFER_SIZE, "%s\\temp\\cash.xml",CurrentPath);
			}
			ShowAllResult(CashFile,page1);
		}
		else
			return;
	}
	/*
	if(page<totalPage)
				page++;
				else
				return;
				CWinThread* th = AfxBeginThread(_MovePage, this);
				th->m_bAutoDelete = TRUE;
				th->ResumeThread();
	*/
}
else if(jumpEditRect1.PtInRect(point)){
	//��ת��ָ��ҳ
	/*
	if(pEdit)
				pEdit->DestroyWindow();
				else
				pEdit = new CPageEdit();
				pEdit->Create(ES_LEFT|ES_WANTRETURN, jumpEditRect1, this, ID_TEXT_JUMPEDIT);
				pEdit->ModifyStyleEx(0,WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
				pEdit->ShowWindow(SW_SHOW);
				pEdit->SetFocus();
	*/
}
return;
	}
	else if(info->src == src_movepage_bottom){
		if(zzqbh.PtInRect(point)){
			//����Ȩ��������
			char WebPath[1024] = {0};
			sprintf_s(WebPath, 1024, "%s/RetailWeb/hanlinclient/ReadingsUserGuide/usersguide/copyrightprotect.htm", JspServer);
			ShellExecute(NULL, "open", WebPath, NULL, NULL, SW_SHOW);
			return;
		}
		else if(up2.PtInRect(point)){
			
			if(page>1){
				page--;
				this->Search(keyWord);
			}
			else
				return;
			//	if(page==1){
			//pMainWnd->Search(pMainWnd->m_ShowView->m_htmlDlg.m_keyword);
			//m_v1WndSplitter.ToUp();
			//     this->Search(keyWord);
			//		}
			//	else{
			//	CWinThread* th = AfxBeginThread(_MovePage, this);
			//	th->m_bAutoDelete = TRUE;
			//	th->ResumeThread();
			//	}
			
			//	return;
		}
		else if(down2.PtInRect(point)){
			
			if(page<totalPage)
			{	
				page++;
				//	m_v1WndSplitter.ToUp();
				this->Search(keyWord);
			}
			
			else
				return;
			//	CWinThread* th = AfxBeginThread(_MovePage, this);
			//	th->m_bAutoDelete = TRUE;
			//	th->ResumeThread();
			return;
			
		}
		else if(jumpEditRect2.PtInRect(point)){
		/*
		if(pEdit)
		pEdit->DestroyWindow();
		else
		pEdit = new CPageEdit();
		pEdit->Create(ES_LEFT|ES_WANTRETURN, jumpEditRect2, this, ID_TEXT_JUMPEDIT);
		pEdit->ModifyStyleEx(0,WS_EX_CLIENTEDGE, SWP_DRAWFRAME);
		pEdit->ShowWindow(SW_SHOW);
		pEdit->SetFocus();
			*/
			return;
		}
		else
			return;
	}
	
	//ͬ��
	CRect synRect;
	if(btn2Map.Lookup(nItem, synRect)){
		if(synRect.PtInRect(point)){
			if(info->src == src_local){
				//ͬ������	��Ҫ����
				if(!PathFileExists(info->link)){
					Tips("���ļ������ڣ��޷�ͬ����");
					return;
				}
			}
			else if (info->src == src_sinabook )//�����۵�Ч��
			{	
				isOpen_makebook = !isOpen_makebook;
				ShowOneResource(src_sinabook);
			}
			return;
		}
	}
	//����
	CRect comRect;
	if (btn3Map.Lookup(nItem,comRect))
	{
		if (comRect.PtInRect(point))
		{
			//	ResourceClassify RCDialog(info->group,info->sid,pMainWnd->m_pGuidTreeView->hAllGroup,NULL);
			//	RCDialog.DoModal();
			//�����Դid��info->abst3
			CString username = UserName;
			CString title = info->title;
			CDialogComments commentDlg(title,atol(info->abst3),username,title,NULL);
			//	CDialogComments commentDlg(atol(info->abst3));
			commentDlg.DoModal();
		}
	}
	//����	
	CRect star1Rect;
	if (star1Map.Lookup(nItem,star1Rect))
	{
		if (star1Rect.PtInRect(point)){
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format("%s/share/comments/updatescore.asp?star=1&sid=%d", AspServer,atol(info->abst3));
			dlg.m_Path.Format("%s\\temp\\updatescore.asp", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("����Ⱥ��ʧ��",td_alert,5);
				return;
			}
			Tips("���ǳɹ���",td_msg,5);
		}
	}
	CRect star2Rect;
	if (star2Map.Lookup(nItem,star2Rect))
	{
		if (star2Rect.PtInRect(point))
		{
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format("%s/share/comments/updatescore.asp?star=2&sid=%d", AspServer,atol(info->abst3));
			dlg.m_Path.Format("%s\\temp\\updatescore.asp", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("����Ⱥ��ʧ��",td_alert,5);
				return;
			}
			Tips("���ǳɹ���",td_msg,5);
		}
	}
	CRect star3Rect;
	if (star3Map.Lookup(nItem,star3Rect))
	{
		if (star3Rect.PtInRect(point))
		{
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format("%s/share/comments/updatescore.asp?star=3&sid=%d", AspServer,atol(info->abst3));
			dlg.m_Path.Format("%s\\temp\\updatescore.asp", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("����Ⱥ��ʧ��",td_alert,5);
				return;
			}
			Tips("���ǳɹ���",td_msg,5);
		}
	}
	CRect star4Rect;
	if (star4Map.Lookup(nItem,star4Rect))
	{
		if (star4Rect.PtInRect(point))
		{
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format("%s/share/comments/updatescore.asp?star=4&sid=%d", AspServer,atol(info->abst3));
			dlg.m_Path.Format("%s\\temp\\updatescore.asp", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("����Ⱥ��ʧ��",td_alert,5);
				return;
			}
			Tips("���ǳɹ���",td_msg,5);
		}
	}
	CRect star5Rect;
	if (star5Map.Lookup(nItem,star5Rect))
	{
		if (star5Rect.PtInRect(point))
		{
			CHttpDownLoadDlg dlg;
			dlg.m_URL.Format("%s/share/comments/updatescore.asp?star=5&sid=%d", AspServer,atol(info->abst3));
			dlg.m_Path.Format("%s\\temp\\updatescore.asp", CurrentPath);
			if(dlg.DoModal()!=IDOK){
				Tips("����Ⱥ��ʧ��",td_alert,5);
				return;
			}
			Tips("���ǳɹ���",td_msg,5);
		}
	}
	CRect mailRect;//�ʼ�����ť
	if (btn4Map.Lookup(nItem,mailRect))
	{
		if (mailRect.PtInRect(point))
		{
			//AfxMessageBox(info->title);
			char* name = new char[100];
			char* resource = new char[100];
			string temp(info->abst1);
			if (temp.length()==0)
			{
				AfxMessageBox("δ�ҵ������ߣ�");
				return;
			}
			ShareInfo* minfo = new ShareInfo;
			CString key="�ϴ���:";
			int begin=temp.find("�ϴ���:",0)+key.GetLength();//strstr(info->abst1,"ӵ����:");
			int end=temp.find(" ",0);
			temp = temp.substr(begin, end-begin);
			strcpy_s(minfo->UserName, 50, temp.c_str());
			strcpy_s(minfo->title, 200, info->title);
			strcpy_s(resource, 100, info->title);
			int len=strlen(resource);
			//AfxMessageBox(name);
			//AfxMessageBox(resource);
			CDialogMail mail;
			mail.resource=resource;
			if(IDOK==mail.DoModal()){		
				if(mail.issms)
					ReadingsSMS(name,UserName,resource,AspServer,this);
				//AfxMessageBox("�ʼ�����");
				//�������ݿ⣬��ñ��˵�email��password���Լ���Դӵ���ߵ�email
				//char WebPath[1024] = {0};
				//char TmpPath[_MAX_PATH] = {0};
				//sprintf(WebPath,"%s/share/emailrequest.asp?request=%s&response=%s", AspServer,UserName,name);
				//	AfxMessageBox(WebPath);
				//	AfxMessageBox(info->title);
				//sprintf(TmpPath, "%s\\temp\\emailrequest.txt", CurrentPath);
				//DeleteFile(TmpPath);		
				//info->title
				/*if(!DownLoad(WebPath, TmpPath, quit, socketList))//modified by zuoss 
				{
					Tips("�����ʼ�������Դʧ��");
					return;
				}*/
				//�� TmpPath �ж�ȡ���˵��������룬�Լ���Դӵ���ߵ����䣬����������
				HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _SendRequestMail,minfo,CREATE_SUSPENDED,NULL); 
				pMainWnd->m_threadList.AddTail(hd);
				::ResumeThread(hd);
				
				//HANDLE hd1 = (HANDLE)::_beginthreadex(NULL,0, _SendRequestSMS,smsInfo,CREATE_SUSPENDED,NULL); 
				//::ResumeThread(hd1);
			}
			
			
		}
	}
	
	//zuoss 20081225 ����
	//	CRect dlRect;
	//	if (info->src == src_share)
	//	{
	//		if(btn1Map.Lookup(nItem,dlRect))
	//		{
	//			if(dlRect.PtInRect(point))
	//				pMainWnd->PostMessage(WM_REQUEST_RESUOURCE_FROM_SERVER, 0, (LPARAM)info->abst3);
	//		}
	// 	}
	//end 
	//����&�Ķ�
	CRect rect;
	if(btn1Map.Lookup(nItem, rect))	
	{
		if(rect.PtInRect(point))
		{
			if (info->src == src_sinabook || info->src == src_netbook)
			{	
				::ShellExecute(NULL, "open", "UrlToWol.exe", info->link, CurrentPath, SW_SHOW);
			}
			if (info->src == src_share)
			{
				CString filename;
				filename.Format("%s",info->title);
				
				CFileDialog fileDialog(FALSE,NULL, filename);
				if(fileDialog.DoModal()!=IDOK)
					return;
				
				DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
				ZeroMemory(missionInfo, sizeof(DownLoadMissionInfo));
				
				//��ȡĿ��·����������뵽Readings�����ļ�����
				CString dstpath;
				dstpath = fileDialog.GetPathName();
				dstpath = dstpath.Left(dstpath.ReverseFind('\\'));
				/*if (dstpath.Find("\\�ҵ�Readings�ļ���")==-1)
				{
				dstpath += "\\�ҵ�Readings�ļ���";
				}*/
				strcpy_s(missionInfo->DstPath, _MAX_PATH, dstpath);
				//AfxMessageBox(missionInfo->DstPath);
				_mkdir(dstpath);
				CString infoPath;
				infoPath.Format("%s\\system\\sharepath.inf", CurrentPath);
				CStdioFile cf;
				if (!cf.Open(infoPath, CFile::modeReadWrite))
				{
					cf.Open(infoPath, CFile::modeCreate|CFile::modeWrite);
				}
				CString listfi;
				cf.ReadString(listfi);
				if(listfi.Find(missionInfo->DstPath)==-1)
				{
					cf.SeekToEnd();
					cf.WriteString(missionInfo->DstPath);
					cf.WriteString("\n");
				}
				cf.Close();
				strcpy_s(missionInfo->URL, 1024, "http://61.181.14.184:6969");
				strcat_s(missionInfo->URL, 1024, info->link);
				//AfxMessageBox(missionInfo->URL);
				strcpy_s(missionInfo->title, _MAX_PATH, filename);
				//AfxMessageBox(missionInfo->title);
				//missionInfo->fileType = info->filetype;
				missionInfo->sourceType = dl_bt;
				CString filen = info->abst2;
				filen.Replace("�ļ���С:","");
				missionInfo->fileLen = atol(filen.GetBuffer(0));
				//missionInfo->fileLen = 10000;
				pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
			}
		}
	}
	
	
	if (info->src == src_msg)
	{
		AfxMessageBox("head");
	}
	if(info->src == src_readman){
		int* pParam = (int*)malloc(sizeof(int));
		*pParam = atoi(info->abst3);
		HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _OrderRss,pParam,CREATE_SUSPENDED,NULL); 
		threadList.AddTail(hd);
		::ResumeThread(hd);
	}
	else if(info->src == src_order){
		if(info->abst3[0]<'3'){
			int* id = new int;
			*id = atoi(info->abst3+1);
			HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _Select,id,CREATE_SUSPENDED,NULL); 
			threadList.AddTail(hd);
			::ResumeThread(hd);
		}
		else{
			HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _AddToOrderCenter,info,CREATE_SUSPENDED,NULL); 
			threadList.AddTail(hd);
			::ResumeThread(hd);
		}
	}
	/*	else if(info->src == src_share)
	{
	//����б�����ʱ�����أ���������ء���ťʱ������
	//	pMainWnd->PostMessage(WM_REQUEST_RESUOURCE_FROM_SERVER, 0, (LPARAM)atol(info->abst3));
	}*/
	else if(info->src != src_local)
	{
		LPSTR URL = (LPSTR)malloc(1024);
		ZeroMemory(URL,1024);
		strcpy_s(URL, 1024, info->link);
		pMainWnd->PostMessage(WM_OPEN_URL_NOT_SHOW,(WPARAM)URL);
	}
	else if(info->src == src_local)
	{
		LPSTR FilePath = (LPSTR)malloc(1024);
		ZeroMemory(FilePath,1024);
		strcpy_s(FilePath, 1024, info->link);
		pMainWnd->PostMessage(WM_OPEN_FILE, (WPARAM)FilePath);
		//	AfxMessageBox("2");
	}
}

void CMainListView::ShowOrderItem(CString Path)
{
	StopThread();
	m_plist->DeleteAllItems();
	SetType(Is_OrdList);
	imgList.DeleteImageList();
	if(style_list == style){
		ChangeToList();
		imgList.Create(THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST, ILC_COLOR24, 0, 1);
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		imgList.SetImageCount(0);
	}
	else{
		ChangeToReport();
		imgList.Create(16, 16, ILC_COLOR24, 0, 1);
		CBitmap	bitmap;
		bitmap.LoadBitmap(IDB_IMGLIST_FILE);
		imgList.Add(&bitmap, RGB(236,233,214));
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		bitmap.DeleteObject();
	}
	
	CFile cf;
	if(!cf.Open(Path, CFile::modeRead))
		return;
	
	//zuoss 090217 ����ҳ�����ͷ��
	OrderDetailInfo* head = (OrderDetailInfo*)malloc(sizeof(OrderDetailInfo));
	strcpy_s(head->name, 200, "orderhead");
	m_plist->InsertItem(0,"",0);
	m_plist->SetItemData(0, (DWORD)head);
	//for(int nIndex=1; nIndex<cf.GetLength()/sizeof(OrderDetailInfo); nIndex++){
	//end
	for(int nIndex=1; nIndex<cf.GetLength()/sizeof(OrderDetailInfo); nIndex++){
		OrderDetailInfo* info = (OrderDetailInfo*)malloc(sizeof(OrderDetailInfo));
		cf.Read(info, sizeof(OrderDetailInfo));
		if(style_list == style){
			CImageList* il = m_plist->GetImageList(LVSIL_SMALL);
			il->SetImageCount(nIndex+1);
			CxImage img;
			CString picPath = GetFileIcon(info->type);
			if(!picPath.IsEmpty()){
				if(img.Load(picPath, CXIMAGE_FORMAT_BMP)){
					CBitmap bmp;
					CBitmap* oldBmp;
					CDC* pDC = GetDC();
					CDC memDC;
					memDC.CreateCompatibleDC(pDC);
					bmp.CreateCompatibleBitmap(pDC, THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST);
					oldBmp = memDC.SelectObject(&bmp);
					memDC.FillSolidRect(0,0,THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST,RGB(0xff,0xff,0xff));
					img.Draw(memDC.m_hDC);
					memDC.SelectObject(oldBmp);
					il->Replace(nIndex, &bmp, NULL);
					bmp.DeleteObject();
					memDC.DeleteDC();
					ReleaseDC(pDC);
				}
				img.Destroy();
			}
			m_plist->InsertItem(nIndex, info->name, nIndex);
			m_plist->SetItemData(nIndex, (DWORD)info);
		}
		else{
			//"����","���Ŀ�����","��������","�����������","������"
			UINT icon = GetSmallIcon(info->type);
			m_plist->InsertItem(nIndex, "", icon);
			m_plist->SetItemText(nIndex, 1, info->name);
			m_plist->SetItemText(nIndex, 2, info->Cycle);
			m_plist->SetItemText(nIndex, 3, info->Last);
			m_plist->SetItemText(nIndex, 4, GetFileExtentName(info->type));
			m_plist->SetItemText(nIndex, 5, info->id);
			free(info);
		}
	}
	cf.Close();
}

void CMainListView::ShowHanlinBookItem(NMLVCUSTOMDRAW* pLVCD)
{
	try
	{
		int nSubItem = pLVCD->iSubItem;
		int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec);
		FINDINFO* info = (FINDINFO*)m_plist->GetItemData(nItem);
		if(!info)
			return;
		if(nSubItem==0)
		{
			CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			CRect rect;
			m_plist->GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
			DrawHanlinBookResult(pDC, rect, info, nItem);
		}
	}
	catch(...)
	{
#ifdef _DEBUG
		Tips("void CMainListView::ShowHanlinBookItem(NMLVCUSTOMDRAW *pLVCD)");
#endif
	}
	
}

void CMainListView::ShowOrderListItem(NMLVCUSTOMDRAW* pLVCD)
{
	try
	{
		int nSubItem = pLVCD->iSubItem;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		OrderDetailInfo* info = (OrderDetailInfo*)m_plist->GetItemData(nItem);
		if(!info)
			return;
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		m_plist->GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
		DrawOrderItem(pDC, rect, info, nItem);
	}
	catch(...)
	{
#ifdef _DEBUG
		Tips("void CMainListView::ShowOrderListItem(NMLVCUSTOMDRAW *pLVCD)");
#endif
	}
}

void CMainListView::DrawOrderItem(CDC *pDC, CRect rect, OrderDetailInfo* info, int nItem)
{
	try{
		if (nItem != 0)//orderlist��һ����������ͷ��������ʾ��Դͼ��
		{
			DrawFileItemIcon(pDC, rect, nItem);			//������
		}
		DrawOrderItemText(pDC, rect, info, nItem);	//
	}
	catch(...){
#ifdef _DEBUG
		Tips("void CMainListView::DrawOrderItem(CDC *pDC, CRect rect, OrderDetailInfo* info, int nItem)");
#endif
	}
}

void CMainListView::DrawOrderItemText(CDC* pDC, CRect rect, OrderDetailInfo* info, int nItem)
{
	if (strcmp(info->name,"orderhead")==0)
	{
		CRect rect;
		m_plist->GetSubItemRect(0, 0, LVIR_BOUNDS, rect);
		DrawOrderListHead(pDC,rect,info);
		return;
	}
	COLORREF crText  = RGB(0, 0, 0);
	COLORREF crBkgnd = RGB(255, 255, 255);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(crBkgnd);
	
	UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_LEFT;;
	CFont ftitle;
	ftitle.CreateFont(18, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_SEMIBOLD, // nWeight 
		FALSE, // bItalic 
		TRUE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("����")); // lpszFac			Monotype Corsiva
	
	CFont fcont;
	fcont.CreateFont(14, // nHeight 
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
		_T("����")); // lpszFac			Monotype Corsiva
	
	rect.left += THUMBNAIL_WIDTH_LIST+10;
	
	CString outStr;
	CFont* oldFont = pDC->SelectObject(&ftitle);
	pDC->SetTextColor(RGB(1,52,220));
	outStr.Format("%s", info->name);
	pDC->TextOut(rect.left+20, rect.top+10, outStr);
	CSize sz = pDC->GetOutputTextExtent(outStr);
	pDC->SetTextColor(0);
	
	CDC memDC;
	CBitmap bmp;
	if(!info->checked)
		bmp.LoadBitmap(IDB_BITMAP_BUTTON_ORDER_EMP);//IDB_BITMAP_TOOLTIPS_DY
	else
		bmp.LoadBitmap(IDB_BITMAP_BUTTON_ORDER_CHK);//IDB_BITMAP_TOOLTIPS_TD
	
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp);
	pDC->BitBlt(rect.left+20+sz.cx+20, rect.top+10, 40, 13, &memDC, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memDC.DeleteDC();
	CRect mrect(rect.left+20+sz.cx+20, rect.top+10, rect.left+20+sz.cx+20+40, rect.top+23);
	btn1Map.SetAt(nItem, mrect);
	
	bmp.LoadBitmap(IDB_BITMAP_BUTTON_DOWNLOAD);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp);
	pDC->BitBlt(rect.left+20+sz.cx+20+45, rect.top+10, 31, 13, &memDC, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memDC.DeleteDC();
	mrect.SetRect(rect.left+20+sz.cx+20+45, rect.top+10, rect.left+20+sz.cx+96, rect.top+23);
	btn2Map.SetAt(nItem, mrect);
	
	pDC->SelectObject(&fcont);
	pDC->SetTextColor(0x9E9E9E);
	
	outStr.Format("������ʱ��:%s   �ļ�����:%s", info->Last, GetFileExtentName(info->type));
	pDC->TextOut(rect.left+20, rect.top+32, outStr);
	pDC->SetTextColor(crText);
	if(info->checked)
		outStr.Format("��������:%s    ����״̬:�Ѷ���", info->Cycle);
	else
		outStr.Format("��������:%s    ����״̬:δ����", info->Cycle);
	pDC->TextOut(rect.left+20, rect.top+50,outStr);
	pDC->SelectObject(oldFont);
	
	ftitle.DeleteObject();
	fcont.DeleteObject();
}

unsigned __stdcall  CMainListView::_Select(LPVOID pParam)
{
	int*p = (int*)pParam;
	char WebPath[1024] = {0};
	sprintf_s(WebPath, 1024, "%s/order/orderitem.asp?id=%d&userid=%s",AspServer,*p,UserName);
	char TmpFileName[_MAX_PATH] = {0};
	sprintf_s(TmpFileName, _MAX_PATH, "%s\\temp\\nothing.tmp", CurrentPath);
	if(!DownLoad(WebPath, TmpFileName, pView->quit, pView->socketList))
	{
		delete p;
		return 1;
	}
	DeleteFile(TmpFileName);
	delete p;
	HANDLE chd = ::GetCurrentThread();
	pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)chd);
	//Tips("���ĳɹ���");
	return 0;
}

unsigned __stdcall CMainListView::_DisSelect(LPVOID pParam)
{
	int*p = (int*)pParam;
	char WebPath[1024] = {0};
	sprintf_s(WebPath, 1024, "%s/order/clearitem.asp?id=%d&userid=%s",AspServer,*p,UserName);
	char TmpFileName[_MAX_PATH] = {0};
	sprintf_s(TmpFileName, _MAX_PATH, "%s\\temp\\nothing.tmp", CurrentPath);
	if(!DownLoad(WebPath, TmpFileName, pView->quit, pView->socketList))
	{
		delete p;
		return 1;
	}
	delete p;
	HANDLE chd = ::GetCurrentThread();
	pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)chd);
	//Tips("�˶��ɹ���");
	return 0;
}

unsigned __stdcall  CMainListView::_AddToOrderCenter(LPVOID pParam)
{
	FINDINFO* info = (FINDINFO*)pParam;
	if(!info)
		return 0;
	BOOL flag = FALSE;
	char WebPath[1024] = {0};
	sprintf_s(WebPath, 1024, "%s/order/AddToOrder.asp?userid=%s&id=%d", AspServer, UserName, atoi(info->abst3+1));
	char Tadd[_MAX_PATH];
	sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\addtoordercenter.tmp", CurrentPath);
	if(!DownLoad(WebPath, Tadd, pView->quit, pView->socketList, pView->socketlistmutex))
	{
		return 1;
	}
	else
	{
		FINDINFO* cInfo = (FINDINFO*)malloc(sizeof(FINDINFO));
		memcpy(cInfo,info,sizeof(FINDINFO));
		pMainWnd->PostMessage(WM_ADD_ORDER_ITEM, (WPARAM)cInfo);
		Tips("��ӳɹ�");
		if(info->abst3[0]=='4' && info->src != src_sinabook){
			char* temp = strchr(info->abst3, ';');
			if(temp){
				int* id = new int;
				*id = atoi(temp+1);
				HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _Select,id,0, NULL); 
				pView->threadList.AddTail(hd);
				::ResumeThread(hd);
			}
		}
	}
	HANDLE chd = ::GetCurrentThread();
	pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)chd);
	return 0;
}

void CMainListView::ShowPushFolder()
{
	StopThread();
	m_plist->DeleteAllItems();
	SetType(Is_PushList);
	imgList.DeleteImageList();
	if(style_list == style){
		ChangeToList();
		imgList.Create(THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST, ILC_COLOR24, 0, 1);
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		imgList.SetImageCount(0);
	}
	else{
		ChangeToReport();
		imgList.Create(16, 16, ILC_COLOR24, 0, 1);
		CBitmap	bitmap;
		bitmap.LoadBitmap(IDB_IMGLIST_FILE);
		imgList.Add(&bitmap, RGB(236,233,214));
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		bitmap.DeleteObject();
	}
	
	_RecordsetPtr m_pRecordset;
	char SQL[1000]={0};
	sprintf_s(SQL, 1000, "SELECT * FROM push WHERE user='%s' ", UserName);
	if(orderstyle == order_by_title)
		strcat_s(SQL, 1000, "ORDER BY [title] DESC");
	else if(orderstyle == order_by_filetype)
		strcat_s(SQL, 1000, "ORDER BY [filetype] DESC");
	else if(orderstyle == order_by_filesize)
		strcat_s(SQL, 1000, "ORDER BY CLng([size]) DESC");
	else
		strcat_s(SQL, 1000, "ORDER BY [cdate] DESC");
	
	try{
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // ��ȡ��ӿ��IDispatchָ��
			adOpenStatic, adLockOptimistic, adCmdText);
		UINT index = 0;
		while(!m_pRecordset->EndOfFile){
			CString path = GetCollectString(m_pRecordset, "path");
			if(PathFileExists(path)){
				int count = m_plist->GetItemCount();
				DiskBookInfo* info = (DiskBookInfo*)malloc(sizeof(DiskBookInfo));
				ZeroMemory(info, sizeof(DiskBookInfo));
				strcpy_s(info->BookName, 200, GetCollectString(m_pRecordset, "title"));
				strcpy_s(info->CreateDate, 50, GetCollectDate(m_pRecordset, "cdate"));
				int filetype = atoi((LPCSTR)GetCollectString(m_pRecordset, "filetype"));
				strcpy_s(info->FileType, 50, GetFileExtentName(filetype));
				strcpy_s(info->Path, _MAX_PATH, path);
				CString sz = GetCollectString(m_pRecordset, "size");
				long size = atol(sz);
				sprintf_s(info->Size, 50, "%d.%d KB", size/1000, size%1000);
				info->syn = ExistSYN(path);
				if(style_list == style){
					m_plist->InsertItem(count, GetCollectString(m_pRecordset, "title"), count);
					m_plist->SetItemData(count, (DWORD)info);
					CImageList* il = m_plist->GetImageList(LVSIL_SMALL);
					il->SetImageCount(count+1);
					CxImage img;
					CString picPath = GetFileIcon(filetype);
					if(!picPath.IsEmpty()){
						if(img.Load(picPath, CXIMAGE_FORMAT_BMP)){
							CBitmap bmp;
							CBitmap* oldBmp;
							CDC* pDC = GetDC();
							CDC memDC;
							memDC.CreateCompatibleDC(pDC);
							bmp.CreateCompatibleBitmap(pDC, THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST);
							oldBmp = memDC.SelectObject(&bmp);
							memDC.FillSolidRect(0,0,THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST,RGB(0xff,0xff,0xff));
							img.Draw(memDC.m_hDC);
							memDC.SelectObject(oldBmp);
							il->Replace(count, &bmp, NULL);
							bmp.DeleteObject();
							memDC.DeleteDC();
							ReleaseDC(pDC);
						}
						img.Destroy();
					}
				}
				else{
					//ͬ��", "����","����ʱ��","����","��С(KB)","·��
					UINT icon = GetSmallIcon(GetFileTypeFromExt(info->FileType));
					m_plist->InsertItem(count, "", icon);
					m_plist->SetItemText(count, 1, info->BookName);
					m_plist->SetItemText(count, 2, info->CreateDate);
					m_plist->SetItemText(count, 3, info->FileType);
					m_plist->SetItemText(count, 4, info->Size);
					m_plist->SetItemText(count, 5, info->Path);
					if(info->syn==syn_yes)
						m_plist->SetCheck(count);
					free(info);
				}
			}
			else
				m_pRecordset->Delete(adAffectCurrent);
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
	}
	catch(_com_error& e){}
	//	m_pRecordset.Release();
}

void CMainListView::ShowRecentlyRead()
{
	StopThread();
	m_plist->DeleteAllItems();
	SetType(Is_RecentlyList);
	imgList.DeleteImageList();
	if(style_list == style){
		ChangeToList();
		imgList.Create(THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST, ILC_COLOR24, 0, 1);
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		imgList.SetImageCount(0);
	}
	else{
		ChangeToReport();
		imgList.Create(16, 16, ILC_COLOR24, 0, 1);
		CBitmap	bitmap;
		bitmap.LoadBitmap(IDB_IMGLIST_FILE);
		imgList.Add(&bitmap, RGB(236,233,214));
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		bitmap.DeleteObject();
	}
	_RecordsetPtr m_pRecordset;
	char SQL[1000]={0};
	sprintf_s(SQL, 1000, "SELECT TOP %d [addr],[cdate],[type],[title] FROM recently ", LOCALSEARCHMAXIUM);
	
	if(orderstyle == order_by_title)
		strcat_s(SQL, 1000, "ORDER BY [title] DESC");
	else if(orderstyle == order_by_filetype)
		strcat_s(SQL, 1000, "ORDER BY [type] DESC");
	else
		strcat_s(SQL, 1000, "ORDER BY [cdate] DESC");
	
	try{
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // ��ȡ��ӿ��IDispatchָ��
			adOpenStatic, adLockOptimistic, adCmdText);
		UINT index = 0;
		int count = m_pRecordset->GetRecordCount();
		while(!m_pRecordset->EndOfFile){
			try{
				CString filepath = GetCollectString(m_pRecordset, "addr");
				CFile cf;
				if(!cf.Open(filepath, CFile::modeRead)){
					m_pRecordset->Delete(adAffectCurrent);
					m_pRecordset->MoveNext();
					continue;
				}
				int size = cf.GetLength();
				cf.Close();
				
				DiskBookInfo* info = (DiskBookInfo*)malloc(sizeof(DiskBookInfo));
				ZeroMemory(info, sizeof(DiskBookInfo));
				strcpy_s(info->BookName, 200, GetCollectString(m_pRecordset, "title"));
				sprintf_s(info->Size, 50, "%d.%dKB", size/1000, size%1000);
				sprintf_s(info->CreateDate, 50, GetCollectDate(m_pRecordset, "cdate"));
				strcpy_s(info->Path, _MAX_PATH, filepath);
				strcpy_s(info->FileType, 50, GetExtNameFromFull(filepath));
				int count = m_plist->GetItemCount();
				
				if(style_list == style){
					m_plist->InsertItem(count, info->BookName, count);
					m_plist->SetItemData(count,(DWORD)info);
					CImageList* il = m_plist->GetImageList(LVSIL_SMALL);
					il->SetImageCount(count+1);
					CxImage img;
					CString picPath = GetFileIcon(GetFileTypeFromExt(info->FileType));
					if(!picPath.IsEmpty()){
						if(img.Load(picPath, CXIMAGE_FORMAT_BMP)){
							CBitmap bmp;
							CBitmap* oldBmp;
							CDC* pDC = GetDC();
							CDC memDC;
							memDC.CreateCompatibleDC(pDC);
							bmp.CreateCompatibleBitmap(pDC, THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST);
							oldBmp = memDC.SelectObject(&bmp);
							memDC.FillSolidRect(0,0,THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST,RGB(0xff,0xff,0xff));
							img.Draw(memDC.m_hDC);
							memDC.SelectObject(oldBmp);
							il->Replace(count, &bmp, NULL);
							bmp.DeleteObject();
							memDC.DeleteDC();
							ReleaseDC(pDC);
						}
						img.Destroy();
					}
				}
				else{
					//ͬ��", "����","����ʱ��","����","��С(KB)","·��
					UINT icon = GetSmallIcon(GetFileTypeFromExt(info->FileType));
					m_plist->InsertItem(count, "", icon);
					m_plist->SetItemText(count, 1, info->BookName);
					m_plist->SetItemText(count, 2, info->CreateDate);
					m_plist->SetItemText(count, 3, info->FileType);
					m_plist->SetItemText(count, 4, info->Size);
					m_plist->SetItemText(count, 5, info->Path);
					if(info->syn==syn_yes)
						m_plist->SetCheck(count);
					free(info);
				}
			}
			catch(_com_error& e){
#ifdef _DEBUG
				Tips("void CMainListView::ShowRecentlyRead()");
				Tips(e.ErrorMessage());
#endif
			}
			m_pRecordset->MoveNext();
		}
		m_pRecordset->Close();
		//		m_pRecordset.Release();
	}
	catch(_com_error& e){
#ifdef _DEBUG
		Tips("void CMainListView::ShowRecentlyRead()");
		Tips(e.ErrorMessage());
#endif
	}
}

void CMainListView::ShowResourceShareFiletype(CString FilePath)
{
	//AfxMessageBox(FilePath);
	StopThread();
	m_plist->DeleteAllItems();
	SetType(Is_ShareList);
	imgList.DeleteImageList();
	if(style_list == style){
		ChangeToList();
		imgList.Create(THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST, ILC_COLOR24, 0, 1);
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		imgList.SetImageCount(0);
	}
	else{
		ChangeToReport();
		imgList.Create(16, 16, ILC_COLOR24, 0, 1);
		CBitmap	bitmap;
		bitmap.LoadBitmap(IDB_IMGLIST_FILE);
		imgList.Add(&bitmap, RGB(236,233,214));
		m_plist->SetImageList(&imgList, LVSIL_SMALL);
		bitmap.DeleteObject();
	}
	
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
		{
			//AfxMessageBox("throw 1");
			throw 1;	//xml ����ʧ��
		}
		if(!pDoc->load((LPCSTR)FilePath))
		{
			//AfxMessageBox("throw 2");
			throw 2;	//xml ����ʧ��
		}
		pNodeList = pDoc->selectNodes("share");
		if (pNodeList == NULL)
		{
			//AfxMessageBox("throw 3");
			throw 3;
		}
		if(pNodeList){
			totalPage = atol((LPCSTR)GetXmlValue(pNodeList, "totalpage", 0));
			totalCount = atol((LPCSTR)GetXmlValue(pNodeList, "totalcount", 0));
			page = atol((LPCSTR)GetXmlValue(pNodeList, "curpage", 0));
			pNodeList.Release();
		}
		else
			throw 3;
		
		if(style_list == style){
			ShareInfo* info = (ShareInfo*)calloc(1,sizeof(ShareInfo));
			info->sid = syn_moveup;
			m_plist->InsertItem(0,"˫���鿴��һҳ",0);
			m_plist->SetItemData(0, (DWORD)info);
		}
		else{
			if(page>1){
				UINT icon = GetSmallIcon(syn_moveup);
				m_plist->InsertItem(0,"˫���鿴��һҳ",icon);
				m_plist->SetItemData(0, (DWORD)syn_moveup);
			}
		}
		pNodeList = pDoc->selectNodes("share/entity");
		if(pNodeList){
			for(int pos1=0; pos1<pNodeList->length && !quit; pos1++){
				CString title, sid, usrid,url,
					lwt, isalive,type,filetype,intro,downtimes,
					commenttimes,belonggroup,size,thehash;
				title = GetXmlValue(pNodeList, "title", pos1);
				// added by zhanganzhan for httpdownload. 090415
				if (FilePath.Find("share_netfriend") != -1)
				{
					url = "http://61.181.14.184:6969"+GetXmlValue(pNodeList,"dladdr",pos1);
					size = GetXmlValue(pNodeList,"size",pos1);
				}else{
					url   = GetXmlValue(pNodeList,"url",pos1);		
				}
				sid = GetXmlValue(pNodeList, "sid", pos1);
				usrid = GetXmlValue(pNodeList, "usrid", pos1);
				lwt = GetXmlValue(pNodeList, "lastwritetime", pos1);
				isalive = GetXmlValue(pNodeList, "isalive", pos1);
				type = GetXmlValue(pNodeList, "type", pos1);
				filetype = GetXmlValue(pNodeList, "filetype", pos1);
				intro = GetXmlValue(pNodeList, "introduction", pos1);
				downtimes = GetXmlValue(pNodeList, "downtimes", pos1);
				commenttimes = GetXmlValue(pNodeList, "commenttimes", pos1);
				belonggroup = GetXmlValue(pNodeList, "belonggroup", pos1);
				ShareInfo* info = (ShareInfo*)malloc(sizeof(ShareInfo));
				ZeroMemory(info, sizeof(ShareInfo));
				if (FilePath.Find("share_chatroom") != -1)
				{
					info->filefrom = 0;
				}
				else if (FilePath.Find("share_netfriend") != -1)
				{
					info->filefrom = 1;
				}
				strcpy_s(info->title, 200, title);
				// added by zhanganzhan for httpdownload. 090415
				if (FilePath.Find("share_netfriend") != -1)
				{
					memcpy(info->url,url,url.GetLength());
					info->downloadType = dl_bt;
					string stitle = rtran_16bases((unsigned char*)info->title,strlen(info->title));
					strcpy_s(info->title, 200, stitle.c_str());
					string ExName = stitle.substr(stitle.find_last_of('.') + 1 ,stitle.length() - stitle.find_last_of('.') - 1);
					info->filetype = GetFileTypeFromExt(ExName.c_str());
					thehash = GetXmlValue(pNodeList,"dladdr",pos1);
					thehash.Replace("/torrents/","");
					thehash.Replace(".torrent","");
					sprintf_s(info->hash, 1024, thehash.GetBuffer(0));
				}
				else if (url.GetLength()>5)
				{
					memcpy(info->url,url,url.GetLength());
					info->downloadType = dl_http;
					//AfxMessageBox("http");
				}
				else 
				{
					//AfxMessageBox("p2p");
					info->downloadType = dl_p2p;
				}
				strcpy_s(info->type, 100, type);
				strcpy_s(info->LWT, 50, lwt);
				strcpy_s(info->intro, 1024, intro);
				strcpy_s(info->UserName, 50, usrid);
				strcpy_s(info->group, 100, belonggroup);
				info->isAlive = atoi((LPCSTR)isalive);
				info->sid = atol((LPCSTR)sid);
				if (FilePath.Find("share_netfriend") == -1)
				{
					info->filetype = atol((LPCSTR)filetype);
				}
				
				info->DownTimes = atol((LPCSTR)downtimes);
				info->CommentTimes = atol((LPCSTR)commenttimes);
				info->filesize = atol((LPCSTR)size);
				SetRedraw(FALSE);
				
				int nIndex = m_plist->GetItemCount();
				
				if(style_list == style){
					imgList.SetImageCount(nIndex+1);
					CxImage img;
					CString picPath = GetFileIcon(info->filetype);
					if(!picPath.IsEmpty()){
						if(img.Load(picPath, CXIMAGE_FORMAT_BMP)){
							CBitmap bmp;
							CBitmap* oldBmp;
							CDC* pDC = GetDC();
							CDC memDC;
							memDC.CreateCompatibleDC(pDC);
							bmp.CreateCompatibleBitmap(pDC, THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST);
							oldBmp = memDC.SelectObject(&bmp);
							memDC.FillSolidRect(0,0,THUMBNAIL_WIDTH_LIST, THUMBNAIL_HEIGHT_LIST,RGB(0xff,0xff,0xff));
							img.Draw(memDC.m_hDC);
							memDC.SelectObject(oldBmp);
							imgList.Replace(nIndex, &bmp, NULL);
							bmp.DeleteObject();
							memDC.DeleteDC();
							ReleaseDC(pDC);
						}
						img.Destroy();
					}
					m_plist->InsertItem(nIndex,title,nIndex);
					m_plist->SetItemData(nIndex,(DWORD)info);
				}
				else{
					//"����","����","���ش���","��ǰ״̬","����ʱ��","��������","�ṩ��","���ݼ��","��Դ��ʶ"
					UINT icon = GetSmallIcon(info->filetype);
					m_plist->InsertItem(nIndex, info->title, icon);
					m_plist->SetItemText(nIndex, 1, GetFileExtentName(info->filetype));
					m_plist->SetItemText(nIndex, 2, downtimes);
					m_plist->SetItemText(nIndex, 3, info->isAlive?"����":"����");
					m_plist->SetItemText(nIndex, 4, info->LWT);
					m_plist->SetItemText(nIndex, 5, info->group);
					m_plist->SetItemText(nIndex, 6, info->UserName);
					m_plist->SetItemText(nIndex, 7, info->intro);
					m_plist->SetItemText(nIndex, 8, sid);
					free(info);
				}
				SetRedraw(TRUE);
			}
			pNodeList.Release();
		}
		pDoc.Release();
		if(style_list == style){
			int nIndex = m_plist->GetItemCount();
			ShareInfo* info = (ShareInfo*)calloc(1,sizeof(ShareInfo));
			info->sid = syn_movedown;
			m_plist->InsertItem(nIndex,"˫���鿴��һҳ",nIndex);
			m_plist->SetItemData(nIndex, (DWORD)info);
		}
		else{
			if(page<totalPage){
				int nIndex = m_plist->GetItemCount();
				UINT icon = GetSmallIcon(syn_movedown);
				m_plist->InsertItem(nIndex,"˫���鿴��һҳ",icon);
				m_plist->SetItemData(nIndex, (DWORD)syn_movedown);
			}
		}
	}
	catch(int errnum){
		switch(errnum){
		case 3:		//�Ѹ���
			pNodeList.Release();
		case 2:		//��ʽ����
			pDoc.Release();
			break;
		}
		//AfxMessageBox("error1");
		return;
	}
	catch(_com_error &e){
#ifdef _DEBUG
		//		Tips(e.Description());
#endif
		//AfxMessageBox(e.Description());
		return;
	}
}
void CMainListView::ShowiBook(CString FilePath)
{
	StopThread();
	m_plist->DeleteAllItems();
	SetType(Is_NetBookList);
	imgList.DeleteImageList();
	
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try
	{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml ����ʧ��
		if(!pDoc->load((LPCSTR)FilePath))
			throw 2;	//xml ����ʧ��
		pNodeList = pDoc->selectNodes("BookList/info");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		else
		{
			totalPage = atol((LPCSTR)GetXmlValue(pNodeList, "totalpage", 0));
			totalCount = atol((LPCSTR)GetXmlValue(pNodeList, "totalcount", 0));
			page = atol((LPCSTR)GetXmlValue(pNodeList, "curpage", 0));
			pNodeList.Release();
		}
		pNodeList = pDoc->selectNodes("BookList/Book");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		else	
		{
			FINDINFO* infohead = (FINDINFO*)malloc(sizeof(FINDINFO));
			ZeroMemory(infohead, sizeof(FINDINFO));
			memset(infohead, 0, sizeof(FINDINFO));
			
			CString str("HanlinHead");
			strcpy_s(infohead->title, 200, str);
			
			SetRedraw(FALSE);
			int nIndex =m_plist->GetItemCount();
			
			m_plist->InsertItem(nIndex,infohead->title,nIndex);
			m_plist->SetItemData(nIndex,(DWORD)infohead);
			SetRedraw(TRUE);
			
			for(int pos=0; pos < pNodeList->length && !quit; pos++)
			{
				CString title = GetXmlValue(pNodeList, "name", pos);
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString classname = GetXmlValue(pNodeList, "typename",pos);
				
				CString foundtime = GetXmlValue(pNodeList, "updatetime",pos);
				CString intro = GetXmlValue(pNodeList, "intro",pos);
				CString filesize = GetXmlValue(pNodeList, "filesize",pos);
				CString url = "";
				url.Format("%s/bookstore/showBookForExchange.asp?bookid=%s&username=%s",AspServer,id,UserName);
				
				//ÿһ����¼��Ҫ�¶���һ���ṹ��ָ�롣�����޸ĸ�ָ����ָ��ֵ��
				FINDINFO* info = (FINDINFO*)malloc(sizeof(FINDINFO));
				ZeroMemory(info, sizeof(FINDINFO));
				memset(info, 0, sizeof(FINDINFO));
				info->src = Is_iType;
				
				strcpy_s(info->title, 200, title);

				CString temp;
				temp.Format("�ļ���С��%sK  �������%s  �ռ�ʱ�䣺%s",filesize,classname,foundtime);
				strcpy_s(info->abst1, 1000, temp);
				
				temp.Format("��飺%s",intro);
				strcpy_s(info->abst2, 1000, temp);
				
				strcpy_s(info->abst3, 1000, id);//��abst3�洢���id
				
				strcpy_s(info->link, 1024, url);
				
				SetRedraw(FALSE);
				nIndex = m_plist->GetItemCount();
				
				m_plist->InsertItem(nIndex,title,nIndex);
				m_plist->SetItemData(nIndex,(DWORD)info);
				SetRedraw(TRUE);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//�Ѹ���
			pNodeList.Release();
		case 2:		//��ʽ����
			pDoc.Release();
			break;
		}
		return;
	}
	
}
void CMainListView::ShowNetBookResource(CString FilePath)
{
	StopThread();
	m_plist->DeleteAllItems();
	SetType(Is_NetBookList);
	imgList.DeleteImageList();
	
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try
	{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml ����ʧ��
		if(!pDoc->load((LPCSTR)FilePath))
			throw 2;	//xml ����ʧ��
		pNodeList = pDoc->selectNodes("NetBookList/info");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		else
		{
			totalPage = atol((LPCSTR)GetXmlValue(pNodeList, "totalpage", 0));
			totalCount = atol((LPCSTR)GetXmlValue(pNodeList, "totalcount", 0));
			page = atol((LPCSTR)GetXmlValue(pNodeList, "currentpage", 0));
			pNodeList.Release();
		}
		pNodeList = pDoc->selectNodes("NetBookList/item");
		if (pNodeList == NULL)
		{
			throw 3;
		}
		else	
		{
			FINDINFO* infohead = (FINDINFO*)malloc(sizeof(FINDINFO));
			ZeroMemory(infohead, sizeof(FINDINFO));
			memset(infohead, 0, sizeof(FINDINFO));
			
			CString str("NetBookListHead");
			strcpy_s(infohead->title, 200, str);
			
			SetRedraw(FALSE);
			int nIndex =m_plist->GetItemCount();
			
			m_plist->InsertItem(nIndex,infohead->title,nIndex);
			m_plist->SetItemData(nIndex,(DWORD)infohead);
			SetRedraw(TRUE);
			
			for(int pos=0; pos < pNodeList->length && !quit; pos++)
			{
				CString title = GetXmlValue(pNodeList, "title", pos);
				CString source = GetXmlValue(pNodeList, "source", pos);
				CString classname = GetXmlValue(pNodeList, "classname",pos);
				CString url = GetXmlValue(pNodeList,"url",pos);
				CString foundtime = GetXmlValue(pNodeList, "foundtime",pos);
				
				//ÿһ����¼��Ҫ�¶���һ���ṹ��ָ�롣�����޸ĸ�ָ����ָ��ֵ��
				FINDINFO* info = (FINDINFO*)malloc(sizeof(FINDINFO));
				ZeroMemory(info, sizeof(FINDINFO));
				memset(info, 0, sizeof(FINDINFO));
				
				info->src = Is_NetBook;
				
				strcpy_s(info->title, 200, title);
				CString temp;
				temp.Format("���������Դ��%s",source);
				strcpy_s(info->abst1,1000, temp);
				
				temp.Format("�������%s  �ռ�ʱ�䣺%s",classname,foundtime);
				strcpy_s(info->abst2, 1000, temp);
				
				strcpy_s(info->link, 1024, url);
				
				SetRedraw(FALSE);
				nIndex = m_plist->GetItemCount();
				
				m_plist->InsertItem(nIndex,title,nIndex);
				m_plist->SetItemData(nIndex,(DWORD)info);
				SetRedraw(TRUE);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//�Ѹ���
			pNodeList.Release();
		case 2:		//��ʽ����
			pDoc.Release();
			break;
		}
		return;
	}
}

unsigned __stdcall CMainListView::_OrderRss(LPVOID pParam)
{
	int* p = (int*)pParam;
	char WebPath[1024] = {0};
	char Tadd[_MAX_PATH] = {0};
	sprintf_s(WebPath, 1024, "%s/opeRss.asp?username=%s&opeflag=1&id=%d",AspServer,UserName, *p);
	//	Tips(WebPath);
	
	sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\rssorder.tmp", CurrentPath);
	BOOL flag = FALSE;
	if(!DownLoad(WebPath, Tadd, pView->quit, pView->socketList,pView->socketlistmutex))
	{
		return 1;
	}
	else 
		Tips("Rss���ĳɹ�",td_msg);
	free(p);
	DeleteFile(Tadd);
	HANDLE chd = ::GetCurrentThread();
	pView->PostMessage(WM_THREAD_FINISHED, (WPARAM)chd);
	return 0;
}


void CMainListView::DownLoadOrder(OrderDetailInfo *info)
{
	CHttpDownLoadDlg dlg;
	dlg.m_URL.Format("%s/order/GetURL.asp?id=%s", AspServer,info->id);
	dlg.m_Path.Format("%s\\temp\\paperurl.tmp",CurrentPath);
	if(dlg.DoModal()!=IDOK){
		//MessageBox("�������ͨѶʧ�ܣ������������ӣ�","����",MB_OK);
		Tips("�������ͨѶʧ�ܣ������������ӣ�",td_alert);
		return;
	}
	CFile cf;
	if(!cf.Open(dlg.m_Path, CFile::modeRead)){
		Tips("�������ͨѶʧ�ܣ������������ӣ�",td_alert);
		return;
	}
	long len = cf.GetLength();
	if(len<=10){
		cf.Close();
		Tips("�������ͨѶʧ�ܣ������������ӣ�",td_alert);
		return;
	}
	char slen[20] = {0};
	cf.Read(slen, 10);
	if(atoi(slen)!=len-10){
		cf.Close();
		Tips("�������ͨѶʧ�ܣ������������ӣ�",td_alert);
		return;
	}
	char* url = (char*)malloc(len-10+1);
	ZeroMemory(url, len-10+1);
	cf.Read(url,len-10);
	cf.Close();
	
	CString filename;
	filename.Format("%s.%s",info->name,GetFileExtentName(info->type));
	
	CFileDialog fileDialog(FALSE,NULL, filename);
	if(fileDialog.DoModal()!=IDOK)
		return;
	
	DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
	ZeroMemory(missionInfo, sizeof(DownLoadMissionInfo));
	strcpy_s(missionInfo->DstPath, _MAX_PATH, fileDialog.GetPathName());
	strcpy_s(missionInfo->URL, 1024, url);
	strcpy_s(missionInfo->title, _MAX_PATH, fileDialog.GetFileName());
	missionInfo->fileType = info->type;
	missionInfo->sourceType = dl_http;
	
	pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
	
	free(url);
}

void CMainListView::ChangeToList()
{
	StopThread();
	m_plist->DeleteAllItems();
	imgList.DeleteImageList();
	
	CHeaderCtrl* pctr = m_plist->GetHeaderCtrl();
	if(pctr){
		int count = pctr->GetItemCount();
		for(int index=0; index<count; index++)
			m_plist->DeleteColumn(0);
	}
	m_plist->ModifyStyle(0, LVS_REPORT|LVS_SINGLESEL|LVS_NOCOLUMNHEADER);
	m_plist->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_plist->InsertColumn(0,"info",LVCFMT_LEFT,600);
}

void CMainListView::ChangeToReport()
{
	StopThread();
	m_plist->DeleteAllItems();
	imgList.DeleteImageList();
	
	CHeaderCtrl* pctr = m_plist->GetHeaderCtrl();
	if(pctr){
		int count = pctr->GetItemCount();
		for(int index=0; index<count; index++)
			m_plist->DeleteColumn(0);
	}
	
	m_plist->ModifyStyle(LVS_SINGLESEL|LVS_NOCOLUMNHEADER, LVS_REPORT);
	
	char fileColName[6][20] = {"ͬ��", "����","����ʱ��","����","��С","·��"};
	long fileColWidth[6] = {40, 200, 150, 80, 80, 200};
	
	char orderColName[6][20] = {"����","���Ŀ�����","��������","�����������","�ļ�����","������"};
	long orderColWidth[6] = {40, 350, 90, 150, 80, 120};
	
	char shareColName[9][20] = {"����","����","���ش���","��ǰ״̬","����ʱ��","����Ⱥ��","�ṩ��","���ݼ��","��Դ��ʶ"};
	long shareColWidth[9] = {200,40,60,40,100,60,100,300,50};
	
	int i=0;
	
	switch(showtype){
	case Is_RecentlyList:
	case Is_FileList:
	case Is_CollectList:
	case Is_SDDiskList:
	case Is_PushList:
		m_plist->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
		for(; i<6; i++)
			m_plist->InsertColumn(i, fileColName[i], LVCFMT_LEFT, fileColWidth[i]);
		break;
	case Is_OrdList:
		m_plist->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
		for(; i<6; i++)
			m_plist->InsertColumn(i, orderColName[i], LVCFMT_LEFT, orderColWidth[i]);
		break;
	case Is_ShareList:
		m_plist->SetExtendedStyle(LVS_EX_FULLROWSELECT);
		for(; i<9; i++)
			m_plist->InsertColumn(i, shareColName[i], LVCFMT_LEFT, shareColWidth[i]);
		break;
	}
}

void CMainListView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CListView::OnRButtonDown(nFlags, point);
	
	hit_index = m_plist->HitTest(point);
	if(hit_index==-1){
		return;
	}
	FINDINFO* info = (FINDINFO*)m_plist->GetItemData(hit_index);
	ClientToScreen(&point);
	
	if(style_list == style){
		
		
		if(showtype==Is_ShareList){
			m_menu.LoadMenu(IDR_MENU_LIST);
			CMenu* pMenu;
			pMenu = m_menu.GetSubMenu(2);
			CMenu* pSubMenu = pMenu->GetSubMenu(0);
			if(orderstyle==order_by_title)
				pSubMenu->CheckMenuRadioItem(0,5,0,MF_BYPOSITION);
			else if(orderstyle==order_by_shareuser)
				pSubMenu->CheckMenuRadioItem(0,5,1,MF_BYPOSITION);
			else if(orderstyle==order_by_createdate)
				pSubMenu->CheckMenuRadioItem(0,5,2,MF_BYPOSITION);
			else if(orderstyle==order_by_filetype)
				pSubMenu->CheckMenuRadioItem(0,5,3,MF_BYPOSITION);
			else if(orderstyle==order_by_downtimes)
				pSubMenu->CheckMenuRadioItem(0,5,4,MF_BYPOSITION);
			else if(orderstyle==order_by_isactive)
				pSubMenu->CheckMenuRadioItem(0,5,5,MF_BYPOSITION);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			m_menu.DestroyMenu();
		}
		else if(showtype==Is_CollectList || showtype==Is_PushList
			|| showtype==Is_RecentlyList || showtype==Is_FileList){
			m_menu.LoadMenu(IDR_MENU_LIST);
			CMenu* pMenu;
			pMenu = m_menu.GetSubMenu(3);
			CMenu* pSubMenu = pMenu->GetSubMenu(0);
			if(orderstyle==order_by_title)
				pSubMenu->CheckMenuRadioItem(0,3,0,MF_BYPOSITION);
			else if(orderstyle==order_by_filesize && showtype!=Is_RecentlyList)
				pSubMenu->CheckMenuRadioItem(0,3,1,MF_BYPOSITION);
			else if(orderstyle==order_by_filetype)
				pSubMenu->CheckMenuRadioItem(0,3,3,MF_BYPOSITION);
			else
				pSubMenu->CheckMenuRadioItem(0,3,2,MF_BYPOSITION);
			if(showtype==Is_RecentlyList)
				pSubMenu->DeleteMenu(1,MF_BYPOSITION);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			m_menu.DestroyMenu();
		}
		else if((showtype==Is_FindList)&&(info->src == src_local))//Lins20081017
        {
			m_menu.LoadMenu(IDR_MENU_LIST);
			CMenu* pMenu;
			pMenu = m_menu.GetSubMenu(4);
			if (pMenu)
			{
				pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			}
			
			m_menu.DestroyMenu();
			
		}
		
	}
	else if(hit_index>=0){	//style_report
		m_menu.LoadMenu(IDR_MENU_LIST);
		switch(showtype){
		case Is_FileList:
		case Is_CollectList:
		case Is_RecentlyList:
		case Is_SDDiskList:
		case Is_PushList:
			CMenu* pMenu;
			pMenu = m_menu.GetSubMenu(0);
			if(m_plist->GetCheck(hit_index)){
				pMenu->ModifyMenu(1,MF_BYPOSITION,ID_MENUITEM_SYN,"ȡ��ͬ��");
			}
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		case Is_OrdList:
			pMenu = m_menu.GetSubMenu(1);
			if(m_plist->GetCheck(hit_index)){
				pMenu->ModifyMenu(0,MF_BYPOSITION,ID_MENUITEM_ORDER,"ȡ������");
			}
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
			break;
		}
		m_menu.DestroyMenu();
	}
	
}
void CMainListView::OnMenuitemRead() 
{
	if(style_report != style)
		return;
	
	LPSTR url = (LPSTR)malloc(1024);
	ZeroMemory(url,1024);
	m_plist->GetItemText(hit_index,5,url,1024);
	pMainWnd->PostMessage(WM_OPEN_FILE, (WPARAM)url);
	//	AfxMessageBox("3");
}

void CMainListView::OnMenuitemMove() 
{
	if(style_report != style)
		return;
	
	CSelPathDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;
	
	CString srcPath = m_plist->GetItemText(hit_index,5);
	CString title = m_plist->GetItemText(hit_index,1);
	CString ext = m_plist->GetItemText(hit_index,3);
	
	CString dstPath;
	dstPath.Format("%s%s.%s", dlg.strPath, title, ext);
	if(MoveFile(srcPath,dstPath)){
		Tips("�ƶ��ɹ�");
		m_plist->DeleteItem(hit_index);
	}
	else
		Tips("�ƶ�ʧ��");
}

void CMainListView::OnMenuitemCopy() 
{
	if(style_report != style)
		return;
	
	CSelPathDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;
	
	CString srcPath = m_plist->GetItemText(hit_index,5);
	CString title = m_plist->GetItemText(hit_index,1);
	CString ext = m_plist->GetItemText(hit_index,3);
	
	CString dstPath;
	dstPath.Format("%s%s.%s", dlg.strPath, title, ext);
	if(CopyFile(srcPath,dstPath,FALSE))
		Tips("���Ƴɹ�");
	else
		Tips("����ʧ��");
}

void CMainListView::OnMenuitemDelete() 
{
	if(style_report != style)
		return;
	CString title = m_plist->GetItemText(hit_index,1);
	CString ext = m_plist->GetItemText(hit_index,3);
	CString msg;
	msg.Format("�Ƿ�Ҫɾ�� %s.%s",title, ext);
	if(IDYES!=MessageBox(msg,SOFTWARE_NAME,MB_YESNO))
		return;
	CString srcPath = m_plist->GetItemText(hit_index,5);
	DeleteFile(srcPath);
	DeleteSYN(srcPath);
	m_plist->DeleteItem(hit_index);
	
	if(Is_PushList == showtype){
		CString SQL;
		SQL.Format("DELETE FROM push WHERE path=\"%s\"", srcPath);
		m_pConnection->Execute((LPCSTR)SQL,NULL,adCmdText);   
	}
	else if(Is_RecentlyList == showtype){
		CString SQL;
		SQL.Format("DELETE FROM recently WHERE addr=\"%s\"", srcPath);
		m_pConnection->Execute((LPCSTR)SQL,NULL,adCmdText);   
	}
}

void CMainListView::OnMenuitemOpenfolder() 
{
	FINDINFO* info = (FINDINFO*)m_plist->GetItemData(hit_index);
	if ((showtype==Is_FindList)&&(info->src == src_local))//�������Ϊ������Դ
	{
        int len = strlen(info->link);
		char* link = (char*)malloc(len+1);
		ZeroMemory(link, len+1);
		strcpy_s(link, 1024, info->link);
		char* cur1 = link;
		cur1 = strrchr(cur1,'\\');    
		_strnset_s( cur1, len+1, '\0', 1 );
		//CString folder = strstr(info->link,"//");
		::ShellExecute(NULL,"open", link, NULL, NULL, SW_SHOW);
		free(link);
	}
	if(style_report != style)
		return;
	CString srcPath = m_plist->GetItemText(hit_index,5);
	int index = srcPath.ReverseFind('\\');
	if(index<0)
		return;
	CString folder = srcPath.Left(index);
	::ShellExecute(NULL,"open", folder, NULL, NULL, SW_SHOW);
}

void CMainListView::OnMenuitemSyn() 
{
	if(style_report != style)
		return;
	
	BOOL bChecked = m_plist->GetCheck(hit_index);
	CString path = m_plist->GetItemText(hit_index,5);
	if(Is_CollectList == showtype){//��ʽ�ɼ�
		char SQL[2048] = {0};
		sprintf_s(SQL, 2048, "UPDATE loc SET synFlag=%d WHERE addr=\"%s\"", bChecked, path);
		m_pConnection->Execute(SQL, NULL, adCmdText);
	}
	else if(Is_RecentlyList == showtype || Is_FileList == showtype
		||Is_PushList == showtype || Is_SDDiskList==showtype){
		if(bChecked)
			CreateSYN(path);					
		else
			DeleteSYN(path);
	}
	m_plist->SetCheck(hit_index, !bChecked);
}

void CMainListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(style_report != style)
		return;
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem<0)
		return;
	
	if(Is_RecentlyList == showtype || Is_FileList == showtype
		||Is_PushList == showtype || Is_SDDiskList == showtype)
	{
		LPSTR path = (LPSTR)malloc(1024);
		ZeroMemory(path, 1024);
		m_plist->GetItemText(pNMListView->iItem,5,path,1024);
		pMainWnd->PostMessage(WM_OPEN_FILE,(WPARAM)path);
		//	AfxMessageBox("4");
	}
	else if(Is_CollectList == showtype){
		DWORD data =  m_plist->GetItemData(pNMListView->iItem);
		if(data == syn_moveup){
			page--;
			pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
		}
		else if(data == syn_movedown){
			page++;
			pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
		}
		else if(data==0){
			LPSTR path = (LPSTR)malloc(1024);
			ZeroMemory(path, 1024);
			m_plist->GetItemText(pNMListView->iItem,5,path,1024);
			pMainWnd->PostMessage(WM_OPEN_FILE,(WPARAM)path);
			//	AfxMessageBox("5");
		}
	}
	else if(Is_ShareList == showtype){
		DWORD data =  m_plist->GetItemData(pNMListView->iItem);
		if(data == syn_moveup){
			page--;
			pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
		}
		else if(data == syn_movedown){
			page++;
			pMainWnd->Operation(pMainWnd->m_hItem, pMainWnd->m_info);
		}
	}
	
	*pResult = 0;
}

void CMainListView::OnMenuitemDownload() 
{
	if(style_report != style)
		return;
	OrderDetailInfo* info = (OrderDetailInfo*)malloc(sizeof(OrderDetailInfo));
	ZeroMemory(info, sizeof(OrderDetailInfo));
	strcpy_s(info->name, 200, m_plist->GetItemText(hit_index,1));
	strcpy_s(info->id, 50, m_plist->GetItemText(hit_index,5));
	info->type = GetFileTypeFromExt(m_plist->GetItemText(hit_index,4));
	DownLoadOrder(info);
	free(info);
}

void CMainListView::OnMenuitemOrder() 
{
	if(style_report != style)
		return;
	
	BOOL bCheck = m_plist->GetCheck(hit_index);
	CString strId = m_plist->GetItemText(hit_index,5);
	int* id = new int;
	*id = atoi((LPCSTR)strId);
	
	if(bCheck){
		HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _DisSelect,id,CREATE_SUSPENDED,NULL); 
		threadList.AddTail(hd);
		::ResumeThread(hd);
	}
	else{
		HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _Select,id,CREATE_SUSPENDED,NULL); 
		threadList.AddTail(hd);
		::ResumeThread(hd);
	}
	m_plist->SetCheck(hit_index,!bCheck);
}

void CMainListView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	
	switch(showtype)
	{
	case Is_RecentlyList:
	case Is_FileList:
	case Is_PushList:
	case Is_SDDiskList:
		{
			CSortClass csc(m_plist, pNMListView->iSubItem);
			if(1 == pNMListView->iSubItem || 2 == pNMListView->iSubItem
				|| 3 == pNMListView->iSubItem || 5 == pNMListView->iSubItem)
				csc.Sort(basc,1);
			else if(4 == pNMListView->iSubItem)
				csc.Sort(basc,4);
			else
				return;
		}
		break;
	case Is_OrdList:
		{
			CSortClass csc(m_plist, pNMListView->iSubItem);
			if(1 == pNMListView->iSubItem || 2 == pNMListView->iSubItem
				|| 3 == pNMListView->iSubItem || 4 == pNMListView->iSubItem)
				csc.Sort(basc,1);
			else if(5 == pNMListView->iSubItem)
				csc.Sort(basc,2);
			else
				return;	
		}
		break;
	case Is_ShareList:
		switch(pNMListView->iSubItem){
		case 0:
			orderstyle = order_by_title;
			break;
		case 1:
			orderstyle = order_by_filetype;
			break;
		case 2:
			orderstyle = order_by_downtimes;
			break;
		case 3:
			orderstyle = order_by_isactive;
			break;
		case 4:
			orderstyle = order_by_createdate;
			break;
		case 6:
			orderstyle = order_by_shareuser;
			break;
		default:
			return;
		}
		page = 1;
		page1 = 0;
		pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
		break;		
		case Is_CollectList:
			switch(pNMListView->iSubItem){
			case 1:
				orderstyle = order_by_title;
				break;
			case 2:
				orderstyle = order_by_createdate;
				break;
			case 3:
				orderstyle = order_by_filetype;
				break;
			case 4:
				orderstyle = order_by_filesize;
				break;
			default:
				return;
			}
			page = 1;
			page1 = 0;
			pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
			break;
	}
	basc = !basc;
}

void CMainListView::OnOrderIsactive() 
{
	orderstyle = order_by_isactive;	
	page = 1;
	pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
	basc=!basc;
}

void CMainListView::OnOrderTitle() 
{
	orderstyle = order_by_title;
	page = 1;
	if(showtype==Is_FileList)
		m_plist->SortItems(CompareFunc,0);
	else{
		pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
		basc=!basc;
	}
}

void CMainListView::OnOrderFiletype() 
{
	orderstyle = order_by_filetype;
	page = 1;
	if(showtype==Is_FileList)
		m_plist->SortItems(CompareFunc,0);
	else{
		pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
		basc=!basc;
	}
}

void CMainListView::OnOrderDowntime() 
{
	orderstyle = order_by_downtimes;
	page = 1;
	pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
	basc=!basc;
}

void CMainListView::OnOrderCreatedate() 
{
	orderstyle = order_by_createdate;
	page = 1;
	if(showtype==Is_FileList)
		m_plist->SortItems(CompareFunc,0);
	else{
		pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
		basc=!basc;
	}
	
}

void CMainListView::OnOrderShareuser() 
{
	orderstyle = order_by_shareuser;
	page = 1;
	pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
	basc=!basc;
}

void CMainListView::OnMenuitemSize() 
{
	orderstyle = order_by_filesize;
	page = 1;
	if(showtype==Is_FileList)
		m_plist->SortItems(CompareFunc,0);
	else{
		pMainWnd->Operation(pMainWnd->m_hItem,pMainWnd->m_info);
		basc=!basc;
	}
}

int CALLBACK CMainListView::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int orderstyle = pView->orderstyle;
	if(pView->showtype!=Is_FileList)
		return 0;
	if(pView->style!=style_list)
		return 0;
	
	DiskBookInfo* info1 = (DiskBookInfo*)lParam1;
	DiskBookInfo* info2 = (DiskBookInfo*)lParam2;
	
	if(orderstyle == order_by_title)
		return strcmp(info1->BookName, info2->BookName);
	else if(orderstyle == order_by_filesize)
		return atol(info1->Size)>atol(info2->Size)?-1:1;
	else if(orderstyle == order_by_createdate)
		return (-1)*strcmp(info1->CreateDate, info2->CreateDate);
	else if(orderstyle == order_by_filetype)
		return GetFileTypeFromExt(info1->FileType)>GetFileTypeFromExt(info2->FileType)?-1:1;
	else
		return 0;
}
//��������
void CMainListView::SearchCache(int flag, int id,CString keyword, int page)
{
	if (gbOnSearching)
	{
		MessageBox("�������������Ժ�...");
		return;
	}
	gbOnSearching = true;
	//	AfxMessageBox("search cache");
	searchkey = keyword;//added by zuoss 20081203 ���������ؼ��֡�
	
	char* kw_b64;
	kw_b64 = (char*)encode2((PUCHAR)(LPCSTR)keyword,strlen(keyword));
	//StopThread();
	m_plist->DeleteAllItems();
	SetType(Is_FindList);
	
	startTime = GetTickCount();
	status = 0;
	////////////////////20080801///////////////////////////
	//����ResultList������������ļ�����������Ϊ����ڵ㻺���ļ�course_1_levelid.xml��Ҷ�ڵ㻺���ļ�course_0_leafid.xml
	//	char cashpath[1024] = {0}; 
	//	sprintf(cashpath,"%s\\temp\\cash.xml",CurrentPath);
	sprintf_s(cashpath, 1024, "%s\\temp\\course_1_%d.xml",CurrentPath,id);//add by Lins 2008/08/22
	//	cashfile(cashpath,CFile::modeReadWrite);
	
	DeleteFile(cashpath);
	if(!cashfile.Open(cashpath,CFile::modeReadWrite|CFile::modeCreate))
	{
		return ;
	}
	cashfile.SeekToBegin();   
	cashfile.WriteString("<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	cashfile.SeekToEnd();//ÿ�ζ������д��
	cashfile.WriteString("<find>");
	///////////////////20080801//////////////////////////////
	
	imgList.DeleteImageList();
	ChangeToList();
	//��ʱ����ô��
	imgList.Create(135, THUMBNAIL_HEIGHT_LIST, ILC_COLOR24, 0, 1);
	m_plist->SetImageList(&imgList, LVSIL_SMALL);
	imgList.SetImageCount(0);
	
	totalPage = 0;
	totalCount = 0;
	finishedSearchCount=0;
	
	FINDINFO* findinfo = (FINDINFO*)malloc(sizeof(FINDINFO));
	ZeroMemory(findinfo, sizeof(FINDINFO));
	findinfo->src = src_msg;
	//	strcpy(findinfo->title, "������,���Ժ�...");
	//	m_plist->InsertItem(0,"������,���Ժ�...",0);
	strcpy_s(findinfo->title, 200, " ");
	m_plist->InsertItem(0," ",0);
	m_plist->SetItemData(0,(DWORD)findinfo);
	
	ThreadInfo* info_google = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_du8 = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	//	ThreadInfo* info_opencourse = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_verycd = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	//	ThreadInfo* info_huntmine = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_baidu = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	//zuoss 20081029
	ThreadInfo* info_baiduentity = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_duxiu = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));//zuoss 081104
	
	ThreadInfo* info_readman = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_order = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	// 	ThreadInfo* info_zcom = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_local = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_sina = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_share = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_dangdang = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	ThreadInfo* info_fanshu = (ThreadInfo*)calloc(1,sizeof(ThreadInfo));
	int f = 0;//����ǲ�νڵ�f=1,����Ҷ�ڵ�f=0
	//google
	//added by zuoss 20080928
	if (flag == Is_NetGroup_Level)
	{
		f = 1;
	}
	else
	{
		f = 0;
	}
	sprintf_s(info_google->WebPath, 1024, "%s/share/search_getgoogle.asp?flg=%d&id=%d", AspServer, f,id);
	//end add
	info_google->id = src_baiduwenku;
	info_google->pWnd = this;
	info_google->flag = flag;
	info_google->gid = id;
	if (page == 1)
		info_google->doflg = 1;
	else
		info_google->doflg = 0;
	
	//du8
	sprintf_s(info_du8->WebPath, 1024, "%s/share/search_getdu8.asp?flg=%d&id=%d", AspServer, f,id);//added by zuoss
	
	info_du8->id = src_sinaishare;
	info_du8->pWnd = this;
	info_du8->flag = flag;
	info_du8->gid = id;
	if (page == 1)
		info_du8->doflg = 1;
	else
        info_du8->doflg = 0;
	// 	//opencourse
	//     //added by zuoss 20081008
	// 	sprintf(info_opencourse->WebPath, "%s/share/search_getMITOpenCourse.asp?flg=%d&id=%d", AspServer, f,id);
	//     //end add
	// 	info_opencourse->id = src_opencourse;
	// 	info_opencourse->pWnd = this;
	// 	info_opencourse->flag = flag;
	// 	info_opencourse->gid = id;
	// 	if (page == 1)
	// 	    info_opencourse->doflg = 1;
	// 	else
	//         info_opencourse->doflg = 0;
	//verycd
	sprintf_s(info_verycd->WebPath, 1024, "%s/search/getverycd.asp?key=%s&base64=1", AspServer, kw_b64);
	info_verycd->id = src_verycd;
	info_verycd->pWnd = this;
	info_verycd->flag = flag;
	info_verycd->gid = id;
	if (page == 1)
		info_verycd->doflg = 1;
	else
        info_verycd->doflg = 0;
	//huntmine
	// 	sprintf(info_huntmine->WebPath, "%s/search/gethuntmine.asp?key=%s&base64=1", AspServer, kw_b64);
	// 	info_huntmine->id = src_huntmine;
	// 	info_huntmine->pWnd = this;
	// 	info_huntmine->flag = flag;
	// 	info_huntmine->gid = id;
	// 	if (page == 1)
	// 	    info_huntmine->doflg = 1;
	// 	else
	//         info_huntmine->doflg = 0;
	//added by zuoss 20080927
	if (flag==Is_NetGroup_Level)
	{
		f = 1;
	}
	else
	{
		f = 0;
	}
	sprintf_s(info_baidu->WebPath, 1024, "%s/share/search_getbaidu.asp?flg=%d&id=%d", AspServer, f,id);
	//end add	
	info_baidu->id = src_baidu;
	info_baidu->pWnd = this;
	info_baidu->flag = flag;
	info_baidu->gid = id;
	if (page == 1)
		info_baidu->doflg = 1;
	else
        info_baidu->doflg = 0;
	
	//zuoss 20081029  //baiduentity
	if (flag==Is_NetGroup_Level)
	{
		f = 1;
	}
	else
	{
		f = 0;
	}
	sprintf_s(info_baiduentity->WebPath, 1024, "%s/share/search_baidu.asp?flg=%d&id=%d", AspServer, f,id);	
	info_baiduentity->id = src_baiduentity;
	info_baiduentity->pWnd = this;
	info_baiduentity->flag = flag;
	info_baiduentity->gid = id;
	if (page == 1)
		info_baiduentity->doflg = 1;
	else
        info_baiduentity->doflg = 0;
	//end
	//dangdang
	if (flag==Is_NetGroup_Level)
	{
		f = 1;
	}
	else
	{
		f = 0;
	}
	sprintf_s(info_dangdang->WebPath, 1024, "%s/search/getdangdang.asp?flg=%d&id=%d", AspServer, f,id);	
	info_dangdang->id = src_dangdang;
	info_dangdang->pWnd = this;
	info_dangdang->flag = flag;
	info_dangdang->gid = id;
	if (page == 1)
		info_baiduentity->doflg = 1;
	else
        info_baiduentity->doflg = 0;
	//fanshu
	if (flag==Is_NetGroup_Level)
	{
		f = 1;
	}
	else
	{
		f = 0;
	}
	sprintf_s(info_fanshu->WebPath, 1024, "%s/search/getdangdang.asp?flg=%d&id=%d", AspServer, f,id);	
	info_fanshu->id = src_fanshu;
	info_fanshu->pWnd = this;
	info_fanshu->flag = flag;
	info_fanshu->gid = id;
	if (page == 1)
		info_baiduentity->doflg = 1;
	else
        info_baiduentity->doflg = 0;
	//duxiu zuoss 20081104
	if (flag==Is_NetGroup_Level)
	{
		f = 1;
	}
	else
	{
		f = 0;
	}
	sprintf_s(info_duxiu->WebPath, 1024, "%s/share/search_duxiu.asp?flg=%d&id=%d", AspServer, f,id);	
	//    sprintf(info_duxiu->WebPath, "%s/search/gethongxiu.asp?key=%s", AspServer,keyword);	
	//	AfxMessageBox(info_duxiu->WebPath);
	info_duxiu->id = src_duxiu;
	info_duxiu->pWnd = this;
	info_duxiu->flag = flag;
	info_duxiu->gid = id;
	if (page == 1)
		info_duxiu->doflg = 1;
	else
        info_duxiu->doflg = 0;
	//end
	//readman
	sprintf_s(info_readman->WebPath, 1024, "%s/search/getreadman.asp?key=%s&base64=1", AspServer, kw_b64);
	info_readman->id = src_readman;
	info_readman->pWnd = this;
	info_readman->flag = flag;
	info_readman->gid = id;
	if (page == 1)
		info_readman->doflg = 1;
	else
        info_readman->doflg = 0;
	//order
	sprintf_s(info_order->WebPath, 1024, "%s/search/getorder.asp?key=%s&base64=1", AspServer, kw_b64);
	info_order->id = src_order;
	info_order->pWnd = this;
	info_order->flag = flag;
	info_order->gid = id;
	if (page == 1)
		info_order->doflg = 1;
	else
        info_order->doflg = 0;
	//zcom
	// 	sprintf(info_zcom->WebPath, "%s/search/searchnetbook.asp?key=%s&base64=1", AspServer, keyword);
	// 	info_zcom->id = src_netbook;
	// 	info_zcom->pWnd = this;
	// 	info_zcom->flag = flag;
	// 	info_zcom->gid = id;
	// 	if (page == 1)
	// 	    info_zcom->doflg = 1;
	// 	else
	//         info_zcom->doflg = 0;
	//sinabook
	//ע��by dzh 20080926
	sprintf_s(info_sina->WebPath, 1024, "%s/search/getsina.asp?key=%s&base64=1", AspServer, kw_b64);
	//add by dzh 20080926		
	//		if(flag==Is_NetGroup_Level)
	//			sprintf(info_sina->WebPath, "%s/share/sgetsina.asp?flg=%d&glid=%d", AspServer, 1,id);
	//		else
	//			sprintf(info_sina->WebPath, "%s/share/sgetsina.asp?flg=%d&gid=%d", AspServer, 0,id);
	//	AfxMessageBox(info_sina->WebPath);
	
	info_sina->id = src_sinabook;
	info_sina->pWnd = this;
	info_sina->flag = flag;
	info_sina->gid = id;
	if (page == 1)
		info_sina->doflg = 1;
	else
        info_sina->doflg = 0;
	//local
	info_local->id = src_local;
	strcpy_s(info_local->WebPath, 1024, keyword);
	info_local->pWnd = this;
	info_local->flag = flag;
	info_local->gid = id;
	if (page == 1)
		info_local->doflg = 1;
	else
        info_local->doflg = 0;
	//share changed by zuoss 20081209
	info_share->id = src_share;
	//sprintf(info_share->WebPath, "%s/share/search_getshare_1.asp?key=%s&base64=1", AspServer, kw_b64);
	string ss = tran_16bases((unsigned char*)keyword.GetBuffer(0),keyword.GetLength());
	sprintf_s(info_share->WebPath, 1024, "http://61.181.14.184:6969/SearchName.html?name=%s", ss.c_str());
	info_share->pWnd =this;
	info_share->flag = flag;
	info_share->gid = id;
	if (page == 1)
		info_share->doflg = 1;
	else
        info_share->doflg = 0;
	HANDLE dlth_google = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_google),CREATE_SUSPENDED, NULL);
	HANDLE dlth_du8 = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_du8),CREATE_SUSPENDED, NULL);
	//	HANDLE dlth_opencourse = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_opencourse),CREATE_SUSPENDED, NULL);
	HANDLE dlth_verycd = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_verycd),CREATE_SUSPENDED, NULL); 
	//	HANDLE dlth_huntmine = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_huntmine),CREATE_SUSPENDED, NULL); 
	HANDLE dlth_baidu = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_baidu),CREATE_SUSPENDED, NULL); 
	//zuoss 200081029
	HANDLE dlth_baiduentity = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_baiduentity),CREATE_SUSPENDED, NULL); 
	HANDLE dlth_duxiu = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_duxiu),CREATE_SUSPENDED, NULL); 
	HANDLE dlth_dangdang = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_dangdang),CREATE_SUSPENDED, NULL); 
	HANDLE dlth_fanshu = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_fanshu),CREATE_SUSPENDED, NULL); 
	
	HANDLE dlth_readman = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_readman),CREATE_SUSPENDED, NULL); 
	HANDLE dlth_order = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_order),CREATE_SUSPENDED, NULL); 
	//	HANDLE dlth_zcom = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_zcom),CREATE_SUSPENDED, NULL); 
	HANDLE dlth_sina = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_sina),CREATE_SUSPENDED, NULL); 	
	HANDLE dlth_local = (HANDLE)_beginthreadex(NULL,0, _SearchLocal,(LPVOID)(info_local),CREATE_SUSPENDED, NULL); 
	HANDLE dlth_share = (HANDLE)_beginthreadex(NULL,0, _SearchCacheNet,(LPVOID)(info_share),CREATE_SUSPENDED, NULL); 
	
	if(dlth_google){
		threadList.AddTail(dlth_google);
		info_google->handle = dlth_google;
		ResumeThread(dlth_google);
	}
	if(dlth_du8){
		threadList.AddTail(dlth_du8);
		info_du8->handle = dlth_du8;
		ResumeThread(dlth_du8);
	}
	// 	if(dlth_opencourse){
	// 		threadList.AddTail(dlth_opencourse);
	// 		info_opencourse->handle = dlth_opencourse;
	// 		ResumeThread(dlth_opencourse);
	// 	}
	if(dlth_verycd){
		threadList.AddTail(dlth_verycd);
		info_verycd->handle = dlth_verycd;
		ResumeThread(dlth_verycd);
	}
	// 	if(dlth_huntmine){
	//   		threadList.AddTail(dlth_huntmine);
	//  		info_huntmine->handle = dlth_huntmine;
	//  		ResumeThread(dlth_huntmine);
	//  	}
	if(dlth_baidu){
		threadList.AddTail(dlth_baidu);
		info_baidu->handle = dlth_baidu;
		ResumeThread(dlth_baidu);
	}
	
	if(dlth_baiduentity){
		threadList.AddTail(dlth_baiduentity);
		info_baiduentity->handle = dlth_baiduentity;
		ResumeThread(dlth_baiduentity);
	}
	if(dlth_dangdang){
		threadList.AddTail(dlth_dangdang);
		info_dangdang->handle = dlth_dangdang;
		ResumeThread(dlth_dangdang);
	}
	if(dlth_fanshu){
		threadList.AddTail(dlth_fanshu);
		info_fanshu->handle = dlth_fanshu;
		ResumeThread(dlth_fanshu);
	}
	if(dlth_duxiu){
		threadList.AddTail(dlth_duxiu);
		info_duxiu->handle = dlth_duxiu;
		ResumeThread(dlth_duxiu);
	}
	if(dlth_readman){
		threadList.AddTail(dlth_readman);
		info_readman->handle = dlth_readman;
		ResumeThread(dlth_readman);
	}
	if(dlth_order){
		threadList.AddTail(dlth_order);
		info_order->handle = dlth_order;
		ResumeThread(dlth_order);
	}
	// 	if(dlth_zcom){
	//  		threadList.AddTail(dlth_zcom);
	// 		info_zcom->handle = dlth_zcom;
	// 		ResumeThread(dlth_zcom);
	// 	}
	if(dlth_sina){
		threadList.AddTail(dlth_sina);
		info_sina->handle = dlth_sina;
		ResumeThread(dlth_sina);
	}
	if(dlth_share){
		threadList.AddTail(dlth_share);
		info_share->handle = dlth_share;
		ResumeThread(dlth_share);
	}
	if(dlth_local){
		threadList.AddTail(dlth_local);
		info_local->handle = dlth_local;
		ResumeThread(dlth_local);
	}
}

unsigned __stdcall CMainListView::_SearchCacheNet(LPVOID pParam)
{
	try{
		ThreadInfo* info = (ThreadInfo*)pParam;
		CMainListView* pWnd = (CMainListView*)info->pWnd;
		if(!info)
			return 0;
		DWORD ncount = 0;
		if(!offline)
			if (info->doflg == 1)
				ncount = pWnd->SeachCacheNet(info);
			else
				ncount = 0;
			
			pWnd->PostMessage(WM_TASKFINISHED,(WPARAM)info->id,(LPARAM)ncount);
			pWnd->PostMessage(WM_THREAD_FINISHED,(WPARAM)info->handle);	
			free(info);
			_endthreadex(0);
	}
	catch(...){
#ifdef _DEBUG
		Tips("_SearchCacheNet");
#endif
	}
	return 0;
}


DWORD CMainListView::SeachCacheNet(ThreadInfo *info)
{
	// 	if (gbOnSearching == TRUE)
	// 	{
	// 		MessageBox("�������������Ժ�...");
	// 		return 0;
	// 	}//added by zhanganzhan on 20081023
	// 	gbOnSearching = TRUE;
	try
	{
		DWORD ncount = 0;
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		char Tadd[_MAX_PATH] = {0};
		char path[_MAX_PATH] = {0};
		
		switch(info->id)
		{
		case src_baidu: sprintf_s(path, _MAX_PATH, "baidu");break;
		case src_baiduentity: sprintf_s(path,  _MAX_PATH, "baiduentity");break;
		case src_duxiu: sprintf_s(path, _MAX_PATH, "duxiu");break;
		case src_sinabook: sprintf_s(path,  _MAX_PATH, "sina");break;
		case src_baiduwenku: sprintf_s(path,  _MAX_PATH, "baiduwenku");break;
		case src_sinaishare: sprintf_s(path,  _MAX_PATH,  "sinaishare");break;
			//		case src_opencourse: sprintf(path,"OpenCourse");break;
		case src_verycd: sprintf_s(path,  _MAX_PATH,  "VeryCD");break;
			//		case src_huntmine: sprintf(path,"Huntmine");break;
		case src_netbook:	 sprintf_s(path,  _MAX_PATH, "�������");break;
		case src_order:  sprintf_s(path, _MAX_PATH, "order"); break;
		case src_share:  sprintf_s(path, _MAX_PATH, "share"); break;
		case src_readman: sprintf_s(path, _MAX_PATH, "readman");break;
//		case src_mailbook: sprintf(path,"������Դ");break;
		case src_dangdang: sprintf_s(path, _MAX_PATH, "����");break;
		case src_fanshu: sprintf_s(path, _MAX_PATH, "����");break;
		}
		//����Ϊ��λ���л���ĸ���
		CTime ct = CTime::GetCurrentTime();
		int day = ct.GetDay();
		sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\groupcache\\%s\\findres%d_%d_%d.xml",CurrentPath,path,info->flag,info->gid,day);
		//	DeleteFile(Tadd);//�Ƿ�ɾ������
		//������������
		if(!PathFileExists(Tadd))
		{
			if(!DownLoad(info->WebPath,Tadd, quit, socketList, socketlistmutex))
				return FALSE;	//xml ����ʧ��
		}
		
		// begin added by yangxiao 2008.8.15
		char oper[256];
		sprintf_s(oper, 256, "��������%s",path);
		//oper.Format("��%s�����ؼ���",path);
		
		CFile * pFile = new CFile();
		pFile->Open(Tadd,CFile::modeRead);
		long fileSize = pFile->GetLength();
		pFile->Close();	
		
		m_searchLog.setLogItem(oper,fileSize, down,comm_oper_leftTree_search);
		m_searchLog.writeLogItem();
		// end added by yangxiao 2008.8.15
		
		try
		{
			
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//xml ����ʧ��
			if(!pDoc->load(Tadd))
				throw 2;	//xml ����ʧ��
			pNodeList = pDoc->selectNodes("find/item");
			if (pNodeList == NULL)
			{
				throw 3;
			}
			if(pNodeList)
			{
				for(int pos1=0; pos1<pNodeList->length && !quit; pos1++)
				{	
					CSingleLock SingleLock(&mutex);
					SingleLock.Lock();
					if(SingleLock.IsLocked())
					{
						finishedSearchCount++;	//zuoss 20081031
						int count;
						CString title, cover, link, abst1, abst2, abst3, score, szpage,page, pagecount;
						CString isalive = "";
						count = m_plist->GetItemCount();
						title = GetXmlValue(pNodeList, "title", pos1);
						cover = GetXmlValue(pNodeList, "cover", pos1);
						link = GetXmlValue(pNodeList, "link", pos1);
						abst1 = GetXmlValue(pNodeList, "abst1", pos1);
						abst2 = GetXmlValue(pNodeList, "abst2", pos1);
						abst3 = GetXmlValue(pNodeList, "abst3", pos1);
						if (info->id == src_share)
						{
							isalive = GetXmlValue(pNodeList,"isalive",pos1);
						}
						//20080801
						FormResultList(title,cover,link,abst1,abst2,abst3,info->id);
						
						score = GetXmlValue(pNodeList,"score",pos1);
						page = GetXmlValue(pNodeList,"page",pos1);
						pagecount = GetXmlValue(pNodeList,"pagecount",pos1);
						totalPage = atoi(pagecount);
						FINDINFO* finfo;
						finfo = (FINDINFO*)malloc(sizeof(FINDINFO));
						memset(finfo, 0, sizeof(FINDINFO));
						
						strcpy_s(finfo->title, 200, title);
						strcpy_s(finfo->cover, 1024, cover);
						strcpy_s(finfo->link, 1024, link);
						strcpy_s(finfo->abst1, 1000, abst1);
						strcpy_s(finfo->abst2, 1000, abst2);
						strcpy_s(finfo->abst3, 1000, abst3);
						strcpy_s(finfo->score, 100, score);
						
						finfo->src = info->id;
						finfo->icon = count;
						if (finfo->src == src_share)
						{ 
							finfo->isAlive = atoi((LPCSTR)isalive);
						}
						
						FindIconListInfo listInfo = {0};
						listInfo.src = finfo->src;
						strcpy_s(listInfo.cover, 1024, finfo->cover);
						coverMap.SetAt(count,listInfo);
						
						SetRedraw(FALSE);
						
						imgList.SetImageCount(count+1);
						CBitmap bitmap;
						bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
						imgList.Replace(count,&bitmap,NULL);
						bitmap.DeleteObject();
						
						if (finishedSearchCount <= 20)
						{
							if(info->id == src_verycd)
							{
								//	m_plist->InsertItem(count,title,count);
								//	m_plist->SetItemData(count,(DWORD)finfo);
								
								//ÿ�ѳ�һ���������m_plist���������롣ˢ����ʾʱ������20081031
								//zuoss 20081031
								m_plist->InsertItem(finishedSearchCount,title,count);
								m_plist->SetItemData(finishedSearchCount,(DWORD)finfo);
								//end
							}
							else if(info->id == src_share)
							{
								
								if (szpage == page)
								{
									//m_plist->InsertItem(1,title,count);
									//m_plist->SetItemData(1,(DWORD)finfo);
									//zuoss 20081031
									m_plist->InsertItem(finishedSearchCount,title,count);
									m_plist->SetItemData(finishedSearchCount,(DWORD)finfo);
									//end
								}
								
							}
							else
							{
								//m_plist->InsertItem(1,title,count);
								//m_plist->SetItemData(1,(DWORD)finfo);
								//zuoss 20081031
								m_plist->InsertItem(finishedSearchCount,title,count);
								m_plist->SetItemData(finishedSearchCount,(DWORD)finfo);
								//end
							}
						}
						ncount++;
						SetRedraw(TRUE);
				}
				SingleLock.Unlock();
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(int errnum)
	{
		switch(errnum)
		{
		case 3:		//�Ѹ���
			pNodeList.Release();
		case 2:		//��ʽ����
			pDoc.Release();
			break;
		}
		return 0;
	}
	
	if(info->id == src_share)
	{
		try
		{
			sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\groupcache\\share\\findres%d_%d_%d.xml",CurrentPath,info->flag,info->gid,day);
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//xml ����ʧ��
			if(!pDoc->load(Tadd))
				throw 2;	//xml ����ʧ��
			pNodeList = pDoc->selectNodes("find");
			if (pNodeList == NULL)
			{
				throw 3;
			}
			if(pNodeList)
			{
				CSingleLock SingleLock(&mutex);
				SingleLock.Lock();
				if(SingleLock.IsLocked())
				{
					CString scount = GetXmlValue(pNodeList, "totalcount", 0);
					char pcount[10] = {0};
					strcpy_s(pcount, 10, scount);
					ncount = atoi(pcount);
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(int errnum)
		{
			switch(errnum)
			{
			case 3:		//�Ѹ���
				pNodeList.Release();
			case 2:		//��ʽ����
				pDoc.Release();
				break;
			}
			return 0;
		}
	}	
	return ncount;
}
catch(...)
{
#ifdef _DEBUG
	Tips("SearchCacheNet");
#endif
}
return 0;
}

DWORD CMainListView::ShowOneResource(int num)
{
	StopThread();
	
	if(!quit){
		HANDLE handle = (HANDLE)::_beginthreadex(NULL,0,_LoadSearchIcon,(LPVOID)(this),CREATE_SUSPENDED, NULL);
		threadList.AddTail(handle);
		::ResumeThread(handle);
	}
	
	//added by zuoss 20080806
	//ɾ�������һ�����������ֻ������һ��洢��������������
	int listsum = m_plist->GetItemCount();//
	int listcount = listsum - 1;
	for (listcount; listcount > 0; --listcount)
	{
		m_plist->DeleteItem(listcount);
	}
	//end add
	
	if (num==src_local)
	{
		char* s_keyword = (char*)malloc(CurSearchKeywords.GetLength()+1);
		ZeroMemory(s_keyword, CurSearchKeywords.GetLength()+1);
		strcpy_s(s_keyword, CurSearchKeywords.GetLength()+1, CurSearchKeywords);
		SearchLocal(s_keyword);
		return 1;
	}
	//zuoss 20081209
	if (num == src_share)
	{
		isShareFlag = TRUE;
		sharePage = 1;
		//��������
	}
	else
	{
		isShareFlag = FALSE;
		sharePage = 0;
	}
	//end
	
	try{
		DWORD ncount = 0;
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		char Tadd[_MAX_PATH] = {0};
		char path[_MAX_PATH] = {0};
		
		//�ҵ�����ҳ��
		if(pMainWnd->curCondition == Is_NetGroup || pMainWnd->curCondition == Is_NetGroup_Level)
		{
			switch(num)
			{
		case src_baidu: sprintf_s(path, _MAX_PATH, "baidu");break;
		case src_baiduentity: sprintf_s(path,  _MAX_PATH, "baiduentity");break;
		case src_duxiu: sprintf_s(path, _MAX_PATH, "duxiu");break;
		case src_sinabook: sprintf_s(path,  _MAX_PATH, "sina");break;
		case src_baiduwenku: sprintf_s(path,  _MAX_PATH, "baiduwenku");break;
		case src_sinaishare: sprintf_s(path,  _MAX_PATH,  "sinaishare");break;
			//		case src_opencourse: sprintf(path,"OpenCourse");break;
		case src_verycd: sprintf_s(path,  _MAX_PATH,  "VeryCD");break;
			//		case src_huntmine: sprintf(path,"Huntmine");break;
		case src_netbook:	 sprintf_s(path,  _MAX_PATH, "�������");break;
		case src_order:  sprintf_s(path, _MAX_PATH, "order"); break;
		case src_share:  sprintf_s(path, _MAX_PATH, "share"); break;
		case src_readman: sprintf_s(path, _MAX_PATH, "readman");break;
//		case src_mailbook: sprintf(path,"������Դ");break;
		case src_dangdang: sprintf_s(path, _MAX_PATH, "����");break;
		case src_fanshu: sprintf_s(path, _MAX_PATH, "����");break;
			}
			//����Ϊ��λ���л���ĸ���
			CTime ct = CTime::GetCurrentTime();
			int day = ct.GetDay();
			
			//������������Ļ���ҳ
			sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\groupcache\\%s\\findres%d_%d_%d.xml",CurrentPath,path,pMainWnd->curCondition,pMainWnd->curID,day);
			//zuoss 20081204
			//sprintf(Tadd,"%s\\temp\\course_%d_%d.xml",CurrentPath,1,pMainWnd->curID);
			
		}
		else
		{
			//�ȴ������������������Ļ���ҳ��
			sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\findres%d.xml",CurrentPath, num);
			//sprintf(Tadd,"%s\\temp\\cash.xml",CurrentPath);
		}
		if(!PathFileExists(Tadd))
			return 0;
		int count = 1;
		try{
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//xml ����ʧ��
			if(!pDoc->load(Tadd))
				throw 2;	//xml ����ʧ��
			pNodeList = pDoc->selectNodes("find/item");
			
			if(pNodeList)
			{
				for(int pos1 = 0; pos1 < pNodeList->length && !quit; pos1++)
				{
					CSingleLock SingleLock(&mutex);
					SingleLock.Lock();
					if(SingleLock.IsLocked())
					{
						CString title, cover, link, abst1, abst2, abst3,score;
						CString sid = "";
						CString isalive = "";
						count = m_plist->GetItemCount();
						if (count > 20)
						{
							break;
						}
						
						title = GetXmlValue(pNodeList, "title", pos1);
						//AfxMessageBox(Tadd);
						if (CString(Tadd).Find("findres14.xml")!=-1)
						{
							string stitle = rtran_16bases((unsigned char*)title.GetBuffer(0),title.GetLength());
							title = stitle.c_str();
						}
						cover = GetXmlValue(pNodeList, "cover", pos1);
						link = GetXmlValue(pNodeList, "link", pos1);
						abst1 = GetXmlValue(pNodeList, "abst1", pos1);
						abst2 = GetXmlValue(pNodeList, "abst2", pos1);
						abst3 = GetXmlValue(pNodeList, "abst3", pos1);
						score = GetXmlValue(pNodeList,"score",pos1);
						if (num == src_sinabook && isOpen_makebook == 0)//�۵�Ч��
						{
							int iabst3 = atoi(abst3);
							if (iabst3 < 0)
							{
								ncount++;
								//SetRedraw(TRUE);
								SingleLock.Unlock();
								continue;
							}
						}

						if (num == src_share)
						{
							isalive = GetXmlValue(pNodeList,"isalive",pos1);
						}
						
						FINDINFO* finfo;
						finfo = (FINDINFO*)malloc(sizeof(FINDINFO));
						memset(finfo, 0, sizeof(FINDINFO));
						
						strcpy_s(finfo->title,  200, title);
						strcpy_s(finfo->cover, 1024, cover);
						strcpy_s(finfo->link, 1024, link);
						strcpy_s(finfo->abst1, 1000, abst1);
						strcpy_s(finfo->abst2, 1000, abst2);
						strcpy_s(finfo->abst3, 1000, abst3);
						strcpy_s(finfo->score, 100, score);
						
						finfo->src = num;
						finfo->icon = count;
						if (finfo->src == src_share)
						{ 
							finfo->isAlive = atoi((LPCSTR)isalive);
						}
						
						FindIconListInfo listInfo = {0};
						
						listInfo.src = finfo->src;
						strcpy_s(listInfo.cover, 1024, finfo->cover);
						coverMap.SetAt(count,listInfo);
						SetRedraw(FALSE);
						imgList.SetImageCount(count+1);
						
						CBitmap bitmap;
						bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
						imgList.Replace(count,&bitmap,NULL);
						bitmap.DeleteObject();
						
						m_plist->InsertItem(count,title,count);
						m_plist->SetItemData(count,(DWORD)finfo);
						
						ncount++;
						SetRedraw(TRUE);
					}
					SingleLock.Unlock();
				}
				pNodeList.Release();
			}
			//added by zuoss 20081205
			if (ncount == 0)
			{
				CSingleLock SingleLock(&mutex);
				SingleLock.Lock();
				if(SingleLock.IsLocked())
				{
					FINDINFO* tipinfo;
					tipinfo = (FINDINFO*)malloc(sizeof(FINDINFO));
					memset(tipinfo, 0, sizeof(FINDINFO));
					//strcpy(tipinfo->title,"***����Դ�Ľ��Ϊ��***");
					strcpy_s(tipinfo->abst2, 1000, "Tips:  �����������������");
					strcpy_s(tipinfo->abst1, 1000, "����Դ���������Ϊ��");
					
					tipinfo->src = num;
					tipinfo->icon = count;
					
					FindIconListInfo listInfo = {0};
					
					listInfo.src = tipinfo->src;
					strcpy_s(listInfo.cover, 1024, tipinfo->cover);
					coverMap.SetAt(count,listInfo);
					SetRedraw(FALSE);
					imgList.SetImageCount(1);
					
					m_plist->InsertItem(count,tipinfo->title,count);
					m_plist->SetItemData(count,(DWORD)tipinfo);	
					
					ncount++;
					SetRedraw(TRUE);
				}
				SingleLock.Unlock();
			}
			pDoc.Release();
		}
		catch(_com_error &e)
		{
#ifdef _DEBUG
			CString msg;
			msg.Format("%s", CheckSrc(num));
			Tips(msg);
#endif
			return 0;
		}		
		
		if (m_plist->GetItemCount() < 20)
		{
			if((pMainWnd->curCondition == Is_NetGroup || pMainWnd->curCondition == Is_NetGroup_Level))
				sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\course_%d_%d.xml",CurrentPath,1,pMainWnd->curID);
			else
				sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\cash.xml",CurrentPath);
			if(!PathFileExists(Tadd))
				return 0;
			try
			{
				HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
				if(!SUCCEEDED(hr))
					throw 1;	//xml ����ʧ��
				if(!pDoc->load(Tadd))
					throw 2;	//xml ����ʧ��
				pNodeList = pDoc->selectNodes("find/item");
				if (pNodeList == NULL)
				{
					throw 3;
				}
				if(pNodeList)
				{
					int pos1 = 0;
					for(pos1 = 0; pos1 < pNodeList->length && !quit; pos1++)
					{
						CSingleLock SingleLock(&mutex);
						SingleLock.Lock();
						if(SingleLock.IsLocked())
						{
							CString title, cover, link, abst1, abst2, abst3,src;
							count = m_plist->GetItemCount() ;
							if (count > 20)
							{
								break;
							}
							src = GetXmlValue(pNodeList,"src",pos1);
							if (atoi(src) == num)
							{
								continue;
							}
							title = GetXmlValue(pNodeList, "title", pos1);
							cover = GetXmlValue(pNodeList, "cover", pos1);
							link = GetXmlValue(pNodeList, "link", pos1);
							abst1 = GetXmlValue(pNodeList, "abst1", pos1);
							abst2 = GetXmlValue(pNodeList, "abst2", pos1);
							abst3 = GetXmlValue(pNodeList, "abst3", pos1);
							
							FINDINFO* finfo;
							finfo = (FINDINFO*)malloc(sizeof(FINDINFO));
							memset(finfo, 0, sizeof(FINDINFO));
							
							strcpy_s(finfo->title, 200, title);
							strcpy_s(finfo->cover, 1024, cover);
							strcpy_s(finfo->link, 1024, link);
							strcpy_s(finfo->abst1, 1000, abst1);
							strcpy_s(finfo->abst2, 1000, abst2);
							strcpy_s(finfo->abst3, 1000, abst3);
							
							finfo->src = atoi(src);
							finfo->icon = count;
							
							FindIconListInfo listInfo = {0};
							
							listInfo.src = finfo->src;
							strcpy_s(listInfo.cover, 1024, finfo->cover);
							coverMap.SetAt(count,listInfo);
							SetRedraw(FALSE);
							imgList.SetImageCount(count+1);
							
							CBitmap bitmap;
							bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
							imgList.Replace(count,&bitmap,NULL);
							bitmap.DeleteObject();
							
							m_plist->InsertItem(count,title,count);
							m_plist->SetItemData(count,(DWORD)finfo);
							
							ncount++;
							SetRedraw(TRUE);
						}
						SingleLock.Unlock();
					}
					
					pNodeList.Release();
				}
				pDoc.Release();		
			}
			catch(int errnum)
			{
				switch(errnum)
				{
				case 3:		//�Ѹ���
					pNodeList.Release();
				case 2:		//��ʽ����
					pDoc.Release();
					break;
				}
				return 0;
			}
			return ncount;
		}
		//end
	}
	catch(...)
	{
#ifdef _DEBUG
		Tips("ShowOneResource");
#endif
	}
	return 0;
}

DWORD CMainListView::ShowOneClass(char* path1)
{
	StopThread();
	int listcount = 1;
	int listsum = m_plist->GetItemCount();	//list������
	while (listcount<listsum)
	{
		m_plist->DeleteItem(1);
		listcount++;
	}
	
	try{
		DWORD ncount = 0;
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		char Tadd[_MAX_PATH] = {0};
		char path[_MAX_PATH] = {0};
		//�ҵ�����ҳ��
		sprintf_s(Tadd,"%s\\temp\\%s", _MAX_PATH, CurrentPath, path1);
		try{
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr)){
				throw 1;	//xml ����ʧ��
			}
			if(!pDoc->load(Tadd)){
				throw 2;	//xml ����ʧ��
			}
			pNodeList = pDoc->selectNodes("find/item");
			if (pNodeList == NULL)
			{
				throw 3;
			}
			if(pNodeList){
				for(int pos1=0; pos1<pNodeList->length && !quit; pos1++){
					CSingleLock SingleLock(&mutex);
					SingleLock.Lock();
					if(SingleLock.IsLocked()){
						int count;
						CString title, cover, link, abst1, abst2, abst3,score,source;
						count = m_plist->GetItemCount();
						title = GetXmlValue(pNodeList, "title", pos1);
						if (CString(path).Find("findres14.xml")!=-1)
						{
							string stitle = rtran_16bases((unsigned char*)title.GetBuffer(0),title.GetLength());
							title = stitle.c_str();
						}
						cover = GetXmlValue(pNodeList, "cover", pos1);
						link = GetXmlValue(pNodeList, "link", pos1);
						abst1 = GetXmlValue(pNodeList, "abst1", pos1);
						abst2 = GetXmlValue(pNodeList, "abst2", pos1);
						abst3 = GetXmlValue(pNodeList, "abst3", pos1);
						score = GetXmlValue(pNodeList,"score",pos1);
						source = GetXmlValue(pNodeList,"source",pos1);
						FINDINFO* finfo;
						finfo = (FINDINFO*)malloc(sizeof(FINDINFO));
						memset(finfo, 0, sizeof(FINDINFO));
						
						strcpy_s(finfo->title, 200, title);
						strcpy_s(finfo->cover, 1024, cover);
						strcpy_s(finfo->link, 1024, link);
						strcpy_s(finfo->abst1, 1000, abst1);
						strcpy_s(finfo->abst2, 1000, abst2);
						strcpy_s(finfo->abst3, 1000, abst3);
						strcpy_s(finfo->score, 100, score);
						//		AfxMessageBox(source);
						finfo->src = 2;
						if (source.Compare("local")==0)
							finfo->src =1;
						if (source.Compare("share")==0)
							finfo->src =13;
						//								if(source.Compare("huntmine")==0)
						//									finfo->src = 14;
						if(source.Compare("sina")==0)
							finfo->src = 12;
						if(source.Compare("google")==0)
							finfo->src = 3;
						if(source.Compare("du8")==0)
							finfo->src = 4;
						//								if(source.Compare("opencourse")==0)
						//									finfo->src = 5;
						if (source.Compare("baidu")==0)
							finfo->src = 6;
						if(source.Compare("verycd")==0)
							finfo->src = 9;
						if(source.Compare("readman")==0)
							finfo->src = 10;
						if(source.Compare("order")==0)
							finfo->src = 11;
						if(source.Compare("baiduentity")==0)
							finfo->src = 7;
						if(source.Compare("duxiu")==0)
							finfo->src = 8;
						if(source.Compare("dangdang")==0)
							finfo->src = 9;
						if(source.Compare("fanshu")==0)
							finfo->src = 10;
						finfo->icon = count;
						
						FindIconListInfo listInfo = {0};
						listInfo.src = finfo->src;
						strcpy_s(listInfo.cover, 1024, finfo->cover);
						coverMap.SetAt(count,listInfo);
						SetRedraw(FALSE);
						imgList.SetImageCount(count+1);
						CBitmap bitmap;
						bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
						imgList.Replace(count,&bitmap,NULL);
						bitmap.DeleteObject();
						m_plist->InsertItem(count,title,count);
						m_plist->SetItemData(count,(DWORD)finfo);
						ncount++;
						SetRedraw(TRUE);
					}
					SingleLock.Unlock();
				}
				pNodeList.Release();
			}
			pDoc.Release();
				}
				catch(int errnum){
					switch(errnum){
					case 3:		//�Ѹ���
						pNodeList.Release();
					case 2:		//��ʽ����
						pDoc.Release();
						break;
					}
					return 0;
				}
				catch(_com_error &e){
#ifdef _DEBUG
					CString msg;
					//				msg.Format("%s", CheckSrc(num));
					Tips(msg);
					//		Tips(e.Description());
#endif
					return 0;
				}
				return ncount;
	}
	catch(...){
#ifdef _DEBUG
		Tips("ShowOneClass");
#endif
	}
	return 0;
}

void CMainListView::UpdateSearchCount(int flag, int id, int total)
{
	char WebPath[1024] = {0};
	char Tadd[_MAX_PATH] = {0};
	
	sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\updatesearchcount.tmp", CurrentPath);
	if(flag==Is_NetGroup_Level){
		sprintf_s(WebPath, 1024, "%s/share/updatesearchcount.asp?flag=1&id=%d&total=%d",AspServer,id, total);
		try{
			if(!DownLoad(WebPath,Tadd, pView->quit, pView->socketList, pView->socketlistmutex))
			{
				Tips("CMainListView::UpdateSearchCount");
				return;
			}
		}
		catch(_com_error& e){
			return ;
		}
	}
	if (flag == Is_NetGroup){
		sprintf_s(WebPath, 1024, "%s/share/updatesearchcount.asp?flag=2&id=%d&total=%d",AspServer,id, total);
		try{
			if(!DownLoad(WebPath,Tadd, pView->quit, pView->socketList, pView->socketlistmutex))
			{
				return;
			}
		}
		catch(_com_error& e){
			return ;
		}
	}
	return;
}

void CMainListView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListView::OnMouseMove(nFlags, point);
	CurToHand(point);
	
	// 		if(m_btnRect[0].PtInRect(point)||m_btnRect[1].PtInRect(point)){
	// 		if(m_hand)
	// 			SetCursor(m_hand);
	// 		else
	// 			SetCursor(LoadCursor(NULL, IDC_ARROW));
	//	}
}

void CMainListView::SetRectCurEmpty()
{
	kjfl.SetRectEmpty();		//��ݷ���
	duzl.SetRectEmpty();		//��������
	wjsx.SetRectEmpty();		//�ļ�����
	yjxx.SetRectEmpty();		//�м���Ϣ
	djhd.SetRectEmpty();
	
	kaoshi.SetRectEmpty();
	xiti.SetRectEmpty();
	kejian.SetRectEmpty();
	jiaocai.SetRectEmpty();
	shijian.SetRectEmpty();
	daxiao.SetRectEmpty();
	gongxiangshu.SetRectEmpty();
	wendingdu.SetRectEmpty();
	zuixin.SetRectEmpty();
	zuikuai.SetRectEmpty();
	zuijurenqi.SetRectEmpty();
	zuidazhekou.SetRectEmpty();
	zuidijiage.SetRectEmpty();
	dijiajuan.SetRectEmpty();
	zengpin.SetRectEmpty();
	renqi.SetRectEmpty();
	tuijian.SetRectEmpty();
	jingdianhuigu.SetRectEmpty();
}
//===================FormResultList=====================================================//
//Function:Form a search result file from all the XML files of different search resources
//parameter1:xml file path
//parameter2:search source id
//add by Linshan 20080801
//======================================================================================//
BOOL CMainListView::FormResultList(CString title,CString cover,CString link,CString abst1,CString abst2,CString abst3,int info_id)
{ 
	CString writeline;
	cashfile.SeekToEnd();
	writeline.Format("<item>");
	cashfile.WriteString(writeline);
	
	cashfile.SeekToEnd();
	writeline.Format("<src><![CDATA[%d]]></src>",info_id);
	cashfile.WriteString(writeline);
	
	cashfile.SeekToEnd();
	writeline.Format("<title><![CDATA[%s]]></title>",title);
	cashfile.WriteString(writeline);
	
	cashfile.SeekToEnd();
	writeline.Format("<cover><![CDATA[%s]]></cover>",cover);
	cashfile.WriteString(writeline);
	
	cashfile.SeekToEnd();
	writeline.Format("<link><![CDATA[%s]]></link>",link);
	cashfile.WriteString(writeline);
	
	cashfile.SeekToEnd();
	writeline.Format("<abst1><![CDATA[%s]]></abst1>",abst1);
	cashfile.WriteString(writeline);
	
	cashfile.SeekToEnd();
	writeline.Format("<abst2><![CDATA[%s]]></abst2>",abst2);
	cashfile.WriteString(writeline);
	
	cashfile.SeekToEnd();
	writeline.Format("<abst3><![CDATA[%s]]></abst3>",abst3);
	cashfile.WriteString(writeline);
	
	cashfile.SeekToEnd();
	writeline.Format("</item>");
	cashfile.WriteString(writeline);
	
	/*CStdioFile readfile(sourcefile,CFile::modeRead);
	CString strReadLine;        //�����sourcefile��һ���ж����������
	fopen(sourcefile, "rb");
	readfile.SeekToBegin();            //ָ���ļ�ͷ 
	if(!readfile)
	return FALSE;
	CString charflg;//����ÿ�еĿ�ͷ��5���ַ������ж�*?
	
	  //����ResultList������������ļ���������Ϊ��cash.xml��
	  //	CString cashpath; 
	  //	sprintf(cashpath,"%s\\temp\\cash.xml",CurrentPath);
	  //	CStdioFile cashfile(cashpath,CFile::modeReadWrite);
	  //  if(cashfile.Open(cashpath,CFile::modeReadWrite))
	  //	{
	  //	cashfile.WriteString("<?xml version=\"1.0\" encoding=\"gb2312\"?>");
	  //	cashfile.WriteString("<find>");
	  while(readfile.ReadString(strReadLine))
	  {
	    	if(strReadLine == "")
			break;
			charflg = strReadLine.Left(5);
			if( (charflg == "<?xml") || (charflg == "<find") || (charflg == "</fin"))
			continue;
			else
			cashfile.SeekToEnd();
			cashfile.WriteString(strReadLine);		
			}
			//  cashfile.WriteString("</find>");
			//	}
			readfile.Close();
			//	cashfile.Close();	
	*/
	return TRUE;
}

void CMainListView::RefreshHanlinBook(TreeItemInfo* info,TreeItemInfo* Pinfo, CString FileName)
{
	StopThread();
	m_plist->DeleteAllItems();
	
	FILE *fp = fopen(FileName, "rb");
	if(!fp)
		return;
	fseek(fp, 0, 2);
	long flen = ftell(fp);
	
	/* style=sytle_reportʱ�����⡣
	if(info->CurrentPage > 1 && style==style_report)
	{
	char txt[100] = {0};
	sprintf(txt, "˫���˴��鿴��һҳ����(��ǰ%d/%dҳ)", info->CurrentPage, info->TotlePage);
	m_plist->InsertItem(0, "", 7);
	m_plist->SetItemText(0, 1, txt);
	m_plist->SetItemData(0, 1);
	}
	else if(style == style_list)
	{
	m_plist->InsertItem(0,"",0);
	BookInfo* binfo = (BookInfo*)malloc(sizeof(BookInfo));
	ZeroMemory(binfo, sizeof(BookInfo));
	binfo->flag = 1;//��ҳ��ʶ
	sprintf(binfo->Author,"%d", info->TotlePage);
	sprintf(binfo->BookId,"%d", info->CurrentPage);
	m_plist->SetItemData(0, (DWORD)binfo);
}*/
	if(flen > sizeof(BookInfoFileHead))
	{
		fseek(fp, sizeof(BookInfoFileHead), 0);
		int booknum = (flen - sizeof(BookInfoFileHead))/sizeof(BookInfo);
		
		totalCount = booknum;
		totalPage = booknum / 10 + 1;
		page = 1;
		
		if(style == style_list)
		{
			CBitmap bitmap;
			bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
			imgList.SetImageCount(booknum+2);
			for(int iconindex=0; iconindex < booknum+2; iconindex++)
				imgList.Replace(iconindex, &bitmap, NULL);//�Ƴ�ԭ�е�Ĵָͼ
			bitmap.DeleteObject();
		}
		
		SetRedraw(FALSE);
		int start = m_plist->GetItemCount();
		for(int i = 0; i < booknum; i++)
		{
			BookInfo bookinfo = {0};
			fread(&bookinfo, 1, sizeof(BookInfo), fp);
			long count = m_plist->GetItemCount();
			
			/*if(style == style_report)
			{
			if(atoi(Pinfo->IndexID) == 1)
			m_plist->InsertItem(count, "", 14);		//14
			else if(atoi(Pinfo->IndexID) == 8)
			m_plist->InsertItem(count, "", 15);		//15
			else if(atoi(Pinfo->IndexID) == 19)
			m_plist->InsertItem(count, "", 16);		//16
			else
			m_plist->InsertItem(count, "", 5);		//5
			m_plist->SetItemText(count, 1, bookinfo.BookName);
			m_plist->SetItemText(count, 2, bookinfo.Author);
			m_plist->SetItemText(count, 3, bookinfo.Size);
			//list.SetItemText(count, 4, bookinfo.Quality);
			//list.SetItemText(count, 5, bookinfo.VisitNumber);
			m_plist->SetItemText(count, 6, bookinfo.Publisher);
			m_plist->SetItemText(count, 7, bookinfo.Path);
			m_plist->SetItemText(count, 8, bookinfo.BookId);
			
			  BookInfo* binfo = (BookInfo*)malloc(sizeof(BookInfo));
			  memset(binfo, 0, sizeof(BookInfo));
			  memcpy(binfo, &bookinfo, sizeof(BookInfo));
			  m_plist->SetItemData(count,(DWORD)binfo);
			  }
			else */
			if(style == style_list)
			{
				CString str;
				str.Format("����:%s  �ϼ�ʱ��:%s  �ļ���С:%s",bookinfo.Author, bookinfo.RefreshData, bookinfo.Size);
				
				CString str2;
				str2.Format("������:%s �����ȼ�:%s",bookinfo.HanlinCode,bookinfo.Quality);
				
				SetType(Is_BookList);
				FINDINFO* finfo;
				finfo = (FINDINFO*)malloc(sizeof(FINDINFO));
				memset(finfo, 0, sizeof(FINDINFO));
				if (i==0)
				{
					CString hanlinhead;
					hanlinhead.Format("HanlinHead");
					memcpy(finfo->title,(LPCSTR)hanlinhead,199);
				}
				else
				{
					memcpy(finfo->title, (LPCSTR)bookinfo.BookName, 199); 
				}
				memcpy(finfo->abst1, (LPCSTR)str, 999);
				memcpy(finfo->abst2, (LPCSTR)str2, 999);
				memcpy(finfo->abst3, (LPCSTR)"hello", 999);
				
				m_plist->InsertItem(count,finfo->title,count);
				m_plist->SetItemData(count,(DWORD)finfo);
			}
		}
		SetRedraw(TRUE);
		Invalidate();
	}
	
	/*	m_plist->InsertItem(count,title,count);
	m_plist->SetItemData(count,(DWORD)finfo);
	ncount++;
	SetRedraw(TRUE);
	
	  imgList.SetImageCount(count+1);
	  CBitmap bitmap;
	  bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
	  imgList.Replace(count,&bitmap,NULL);
	  bitmap.DeleteObject();
	*/
}

DWORD CMainListView::ShowAllResult(char *path,int pagenum)
{
	
	//	StopThread();
	int count_item = 0;
	int listcount = 1;
	int listsum = m_plist->GetItemCount();	//list������
	while (listcount<listsum)
	{
		m_plist->DeleteItem(1);
		listcount++;
	}
	
	try{
		DWORD ncount = 0;
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		
		//�ҵ�����ҳ��
		try{
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr)){
				throw 1;	//xml ����ʧ��
			}
			if(!pDoc->load(path)){
				//	AfxMessageBox("2");
				throw 2;	//xml ����ʧ��
			}
			pNodeList = pDoc->selectNodes("find/item");
			if (pNodeList == NULL)
			{
				throw 3;
			}
			if(pNodeList){
				int beginnum =0;
				int endnum = 0;
				if (pNodeList->length%20!=0)
				{
					totalPage1 = pNodeList->length/20+1;
				}
				else
					totalPage1 = pNodeList->length/20;
				
				//totalPage = totalPage1;
				if (20*pagenum+20 >pNodeList->length)
				{
					endnum = pNodeList->length-1;
				}
				else 
					endnum = 20*pagenum+20 ;
				for(int pos1=20*pagenum; pos1<endnum && !quit; pos1++){
					CSingleLock SingleLock(&mutex);
					SingleLock.Lock();
					if(SingleLock.IsLocked()){
						int count;
						CString src,title, cover, link, abst1, abst2, abst3,score,source;
						count = m_plist->GetItemCount();
						
						source = GetXmlValue(pNodeList, "src", pos1);
						title = GetXmlValue(pNodeList, "title", pos1);
						//AfxMessageBox(path);
						if (CString(path).Find("findres14.xml")!=-1)
						{
							string stitle = rtran_16bases((unsigned char*)title.GetBuffer(0),title.GetLength());
							title = stitle.c_str();
						}
						
						cover = GetXmlValue(pNodeList, "cover", pos1);
						link = GetXmlValue(pNodeList, "link", pos1);
						abst1 = GetXmlValue(pNodeList, "abst1", pos1);
						abst2 = GetXmlValue(pNodeList, "abst2", pos1);
						abst3 = GetXmlValue(pNodeList, "abst3", pos1);
						//score = GetXmlValue(pNodeList,"score",pos1);
						//source = GetXmlValue(pNodeList,"source",pos1);
						FINDINFO* finfo;
						finfo = (FINDINFO*)malloc(sizeof(FINDINFO));
						memset(finfo, 0, sizeof(FINDINFO));
						
						strcpy_s(finfo->title, 200, title);
						strcpy_s(finfo->cover, 1024, cover);
						strcpy_s(finfo->link, 1024, link);
						strcpy_s(finfo->abst1, 1000, abst1);
						strcpy_s(finfo->abst2, 1000, abst2);
						strcpy_s(finfo->abst3, 1000, abst3);
						strcpy_s(finfo->score, 100, score);

						finfo->src = 2;

						if (atoi(source))
							finfo->src =atoi(source);
						else
							finfo->src = 2;
						
						finfo->icon = count;
						
						FindIconListInfo listInfo = {0};
						listInfo.src = finfo->src;
						strcpy_s(listInfo.cover, 1024, finfo->cover);
						coverMap.SetAt(count,listInfo);
						SetRedraw(FALSE);
						imgList.SetImageCount(count+1);
						CBitmap bitmap;
						bitmap.LoadBitmap(IDB_FILEICON_3DSMALLBOOK);
						imgList.Replace(count,&bitmap,NULL);
						bitmap.DeleteObject();
						m_plist->InsertItem(count,title,count);
						m_plist->SetItemData(count,(DWORD)finfo);
						ncount++;
						SetRedraw(TRUE);
					}
					SingleLock.Unlock();
				}
				pNodeList.Release();
					}
					pDoc.Release();
				}
				
				catch(int errnum){
					switch(errnum){
					case 3:		//�Ѹ���
						pNodeList.Release();
					case 2:		//��ʽ����
						pDoc.Release();
						break;
					}
					return 0;
				}
				catch(_com_error &e){
#ifdef _DEBUG
					CString msg;
					//				msg.Format("%s", CheckSrc(num));
					Tips(msg);
					//		Tips(e.Description());
#endif
					return 0;
				}
				return ncount;
	}
	catch(...){
#ifdef _DEBUG
		Tips("ShowAllResult");
#endif
	}
	return 0;
}

void CMainListView::CurToHand(CPoint point)
{
	if (orderAll.PtInRect(point)||unOrderAll.PtInRect(point)||reverseAll.PtInRect(point)||kjfl.PtInRect(point)
		||duzl.PtInRect(point)||wjsx.PtInRect(point)||yjxx.PtInRect(point)||djhd.PtInRect(point)
		||kaoshi.PtInRect(point)||xiti.PtInRect(point)||kejian.PtInRect(point)||jiaocai.PtInRect(point)
		||shijian.PtInRect(point)||daxiao.PtInRect(point)||gongxiangshu.PtInRect(point)
		||wendingdu.PtInRect(point)||zuixin.PtInRect(point)||zuikuai.PtInRect(point)
		||zuijurenqi.PtInRect(point)||zuidazhekou.PtInRect(point)||zuidijiage.PtInRect(point)
		||dijiajuan.PtInRect(point)||zengpin.PtInRect(point)||renqi.PtInRect(point)||tuijian.PtInRect(point)||jingdianhuigu.PtInRect(point)
		||zzqbh.PtInRect(point)||mianze.PtInRect(point)			//���� ��������˵��
		||up2.PtInRect(point)||down2.PtInRect(point)||up1.PtInRect(point)||down1.PtInRect(point))	//�ײ��� ��һҳ ��һҳ
	{
		if (m_hand)
			SetCursor(m_hand);
		else
			SetCursor(LoadCursor(NULL,IDC_ARROW));
	}
	
	int index;//m_plist->HitTest(point);
	CRect rect1,rect2,rect3,rect4;
	for (index=0;index<m_plist->GetItemCount();index++)
	{
		if(btn1Map.Lookup(index,rect1)){
			if (rect1.PtInRect(point))
			{
				if (m_hand)
					SetCursor(m_hand);
				else
					SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
		}
		if(btn2Map.Lookup(index,rect2)){
			if (rect2.PtInRect(point))
			{
				if (m_hand)
					SetCursor(m_hand);
				else
					SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
		}
		if(btn3Map.Lookup(index,rect3)){
			if (rect3.PtInRect(point))
			{
				if (m_hand)
					SetCursor(m_hand);
				else
					SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
		}
		if (btn4Map.Lookup(index,rect4))
		{
			if (rect4.PtInRect(point))
			{
				if (m_hand)
					SetCursor(m_hand);
				else
					SetCursor(LoadCursor(NULL,IDC_ARROW));
			}
		}
	}
}

unsigned __stdcall CMainListView::_SendRequestMail(LPVOID pParam)
{
	ShareInfo* info = (ShareInfo*)pParam;
	char WebPath[2048] = {0};
	char TmpPath[_MAX_PATH] = {0};
	sprintf_s(WebPath, 2048, "%s/emailrequest/getemailaddr.asp?username=%s", AspServer,UserName);
	char* buffer;
	HttpUtl utl;
	if(utl.request(WebPath,buffer)==0)
	{
		Tips("�ʼ�����ʧ�ܣ����鵱ǰ�������ӣ�");
		return 0;
	}
	if (sizeof(buffer) == 0)
	{
		Tips("��δ��д������Ϣ��������������Ϣ���ٽ����ʼ�����");
		return 0;
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	sprintf_s(WebPath, 2048, "%s/emailrequest/emailrequest.asp?requester=%s&title=%s&offer=%s", AspServer,UserName,info->title,info->UserName);
	//AfxMessageBox(WebPath);
	if(!UpLoad(WebPath))
	{
		Tips("�ʼ�����ʧ�ܣ����鵱ǰ�������ӣ�");
		return 0;
	}
	Tips("���ͳɹ�!");
	HANDLE handle = (HANDLE)::GetCurrentThread();
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	_endthreadex(0);
	return 0;
}

unsigned __stdcall CMainListView::_SendRequestSMS(LPVOID pParam)
{
	SMSInfo * info=(SMSInfo *)pParam;
	if(!info){
		return 0;
	}
	
	char *pstxt=info->Content.GetBuffer(1024);
	unsigned char  *pustxt=encode2((unsigned char *)pstxt,strlen(pstxt));
	CString url;
    char TmpPath[1024]={0};
	CString testAspServer="http://202.113.25.81";
    sprintf_s(TmpPath, 1024, "%s\\temp\\smsrequestno.txt", CurrentPath);
	
	url.Format("%s/search/sms/getno.asp?username=%s",AspServer,info->UserName);
	
	if(!DownLoad(url,TmpPath,info->mainListView->quit,info->mainListView->socketList)){
		Tips("����sms����ʧ��");
		return 0;
	}
    ifstream smsrequestno;
	smsrequestno.open(TmpPath,ios::in);			
	char temp[1024] = {0};
	smsrequestno.getline(temp,sizeof(temp));
   	
	if(temp[0]==0){
		Tips("����sms����ʧ��");
		return 0;
	}
	BOOL quit=false;
    smsrequestno.close();
	char testpstxt[100]="hello";
	url.Format("%s/sms.asp?txtNo=%s&txtContent=%s", testAspServer,temp,pustxt);
    sprintf_s(TmpPath, 1024, "%s\\temp\\smsrequest.txt", CurrentPath);
	if(!DownLoad(url,TmpPath,quit,info->mainListView->socketList)){
		Tips("����sms����ʧ��");
		return 0;
	}
	
	ifstream smsrequest;
	smsrequest.open(TmpPath,ios::in);			
	smsrequest.getline(temp,sizeof(temp));
	if(temp[0]==0) AfxMessageBox("kong");
	if(temp[0]==0||temp[0]!='1'||temp[1]!='1'){
		
		Tips("����sms����ʧ��");
		return 0;
	}else{
		Tips("����sms����ɹ�");
	}	
	
	smsrequest.close();
	HANDLE handle = (HANDLE)::GetCurrentThread();
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	_endthreadex(0);
	return 0;
}

string CMainListView::rtran_16bases(unsigned char *input, int inputLen)
{
	char* out=new char[inputLen];
	for(int i=0;i<inputLen;i+=2){
		unsigned char tmp=0;
		if(input[i]>='A'&&input[i]<='F'){
			tmp|=(input[i]-'A'+10)<<4;
		}else{
			tmp|=((input[i])-'0')<<4;
		}
		
		if(input[i+1]>='A'&&input[i+1]<='F'){
			tmp|=(input[i+1]-'A'+10);
		}else{
			tmp|=((input[i+1])-'0');
		}
		out[i/2]=tmp;
	}
	out[inputLen/2]='\0';
	std::string outString(out);
	delete[] out;
	return outString;
}

string CMainListView::tran_16bases(unsigned char *deviceid, int deviceidlen)
{
	char* out=new char[deviceidlen*2+1];
	for(int i=0;i<deviceidlen;i++){
		int high=deviceid[i]>>4;
		if(high<10){
			out[i*2]='0'+high;
		}else{
			out[i*2]='A'+high-10;
		}
		int low=deviceid[i]&0x0F;
		if(low<10){
			out[i*2+1]='0'+low;
		}else{
			out[i*2+1]='A'+low-10;
		}
	}
	out[deviceidlen*2]='\0';
	std::string outString(out);
	delete[] out;
	return outString;
}
