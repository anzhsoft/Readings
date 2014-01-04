// UserListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "UserListCtrl.h"
#include "lib/image/xImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char CurrentPath[_MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CUserListCtrl

CUserListCtrl::CUserListCtrl()
{
}

CUserListCtrl::~CUserListCtrl()
{
}


BEGIN_MESSAGE_MAP(CUserListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CUserListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserListCtrl message handlers

void CUserListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 42;
}

void CUserListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if(lpDrawItemStruct->itemState & ODS_SELECTED){
		Draw(pDC, lpDrawItemStruct->itemID,	lpDrawItemStruct->rcItem,
			GetSysColor(COLOR_HIGHLIGHTTEXT), GetSysColor(COLOR_HIGHLIGHT));
	}
	else{
		Draw(pDC, lpDrawItemStruct->itemID,	lpDrawItemStruct->rcItem,
			GetSysColor(COLOR_WINDOWTEXT),GetSysColor(COLOR_WINDOW));
	}
}

void CUserListCtrl::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	DWORD data = GetItemData(pNMListView->iItem);
	if(!data)
		return;

	free(LPVOID(data));
}

void CUserListCtrl::Draw(CDC *pDC, int nItem, CRect rect, COLORREF txtColor, COLORREF bgColor)
{
	pDC->FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(), bgColor);
	
	CImageList* il = GetImageList(LVSIL_SMALL);
	if(il){
		CPoint pt = rect.TopLeft();
		pt.x += 2;
		pt.y += 1;
		if(bgColor == GetSysColor(COLOR_HIGHLIGHT))
			il->Draw(pDC, nItem, pt, ILD_FOCUS);
		else 
			il->Draw(pDC, nItem, pt, ILD_NORMAL);
	}

	COLORREF oldColor = pDC->SetTextColor(txtColor);
	CRect txtRect(rect.left+44, rect.top+2, rect.right, rect.bottom);
	CString name = GetItemText(nItem, 0);
	pDC->DrawText((LPCSTR)name, name.GetLength(), txtRect, 
		DT_MODIFYSTRING|DT_END_ELLIPSIS);
	pDC->SetTextColor(oldColor);
}

BOOL CUserListCtrl::SetImage(UINT icon, int nItem)
{
	BOOL res = FALSE;
	CImageList* il = GetImageList(LVSIL_SMALL);
	if(!il)
		return res;


	CString picPath;
	icon = icon%26;
	picPath.Format("%s\\system\\icon\\user\\%d.bmp", CurrentPath, icon);

	CxImage img;
	if(!img.Load(picPath, CXIMAGE_FORMAT_BMP))
		return res;
	if(img.IsValid()){
		CClientDC dc(this);
		CDC memDC;
		CBitmap bmp;
		CBitmap* oldBmp;
		memDC.CreateCompatibleDC(&dc);
		bmp.CreateCompatibleBitmap(&dc,40,40);
		oldBmp = memDC.SelectObject(&bmp);
		img.Draw(memDC.m_hDC, 0,0,40,40);
		memDC.SelectObject(oldBmp);
		il->SetImageCount(nItem+1);
		il->Replace(nItem, &bmp, NULL);
		memDC.DeleteDC();
		bmp.DeleteObject();
		res = TRUE;
	}
	img.Destroy();
	return res;
}

BOOL CUserListCtrl::CreateImageList()
{
	CImageList il;
	if(!il.Create(40, 40, ILC_COLORDDB|ILC_MASK, 0, 1))
		return FALSE;
	il.SetImageCount(0);
	SetImageList(&il,LVSIL_SMALL);
	il.Detach();
	return TRUE;
}

void CUserListCtrl::DeleteImageList()
{
	CImageList* il = GetImageList(LVSIL_SMALL);
	if(il)
		il->DeleteImageList();
}

void CUserListCtrl::RemoveImage(int nItem)
{
	CImageList* il = GetImageList(LVSIL_SMALL);
	if(il)
		il->Remove(nItem);
}
