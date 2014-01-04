// AdvancedBitmapButton.cpp : implementation file
//

#include "stdafx.h"
#include "AdvancedBitmapButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvancedBitmapButton
CAdvancedBitmapButton::CAdvancedBitmapButton()
{
	ButtonType = 0;
	IsNotCapture = TRUE;
}

CAdvancedBitmapButton::~CAdvancedBitmapButton()
{
	m_bmpCommon.DeleteObject();
	m_bmpSel.DeleteObject();
	m_bmpFocus.DeleteObject();
	m_bmpDisable.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAdvancedBitmapButton, CButton)
	//{{AFX_MSG_MAP(CAdvancedBitmapButton)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedBitmapButton message handlers
BOOL CAdvancedBitmapButton::LoadBitmaps(UINT nIDBitmapResource, UINT nIDBitmapResourceSel, UINT nIDBitmapResourceFocus, UINT nIDBitmapResourceDisabled)
{
	m_bmpCommon.DeleteObject();
	m_bmpSel.DeleteObject();
	m_bmpFocus.DeleteObject();
	m_bmpDisable.DeleteObject();
	if (!m_bmpCommon.LoadBitmap(nIDBitmapResource))
		return FALSE;

	if (nIDBitmapResourceSel==0)
		return TRUE;
	else if (!m_bmpSel.LoadBitmap(nIDBitmapResourceSel))
		return FALSE;

	if (nIDBitmapResourceFocus==0)
		return TRUE;
	else if (!m_bmpFocus.LoadBitmap(nIDBitmapResourceFocus))
		return FALSE;

	if (nIDBitmapResourceDisabled==0)
		return TRUE;
	else if (!m_bmpDisable.LoadBitmap(nIDBitmapResourceDisabled))
		return FALSE;

	return TRUE;
}

BOOL CAdvancedBitmapButton::LoadBitmaps(LPCTSTR lpszBitmapResource, LPCTSTR lpszBitmapResourceSel, LPCTSTR lpszBitmapResourceFocus, LPCTSTR lpszBitmapResourceDisabled)
{
	if (!m_bmpCommon.LoadBitmap(lpszBitmapResource))
		return FALSE;

	if (lpszBitmapResourceSel==NULL)
		return TRUE;
	else if (!m_bmpSel.LoadBitmap(lpszBitmapResourceSel))
		return FALSE;

	if (lpszBitmapResourceFocus==NULL)
		return TRUE;
	else if (!m_bmpFocus.LoadBitmap(lpszBitmapResourceFocus))
		return FALSE;

	if (lpszBitmapResourceDisabled==NULL)
		return TRUE;
	else if (!m_bmpDisable.LoadBitmap(lpszBitmapResourceDisabled))
		return FALSE;

	return TRUE;
}

void CAdvancedBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CBitmap* pOldBmp;
	CDC MemDC;
	CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
 	CRect rcItem;
 	rcItem=lpDrawItemStruct->rcItem;
	BITMAP bmp;

	if((lpDrawItemStruct->itemAction & ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED)){
		MemDC.CreateCompatibleDC(pDC);
		pOldBmp=MemDC.SelectObject(&m_bmpFocus);
		m_bmpSel.GetBitmap(&bmp);
		//pDC->BitBlt(0,0,rcItem.Width(),rcItem.Height(),&MemDC,0,0,SRCCOPY);
		pDC->StretchBlt(0,0,rcItem.Width(),rcItem.Height(),&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();
	}
	else if(lpDrawItemStruct->itemState & ODS_DISABLED){
		MemDC.CreateCompatibleDC(pDC);
		pOldBmp=MemDC.SelectObject(&m_bmpDisable);
		m_bmpDisable.GetBitmap(&bmp);
		//pDC->BitBlt(0,0,rcItem.Width(),rcItem.Height(),&MemDC,0,0,SRCCOPY);
		pDC->StretchBlt(0,0,rcItem.Width(),rcItem.Height(),&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();
	}
	else{
		MemDC.CreateCompatibleDC(pDC);
		pOldBmp=MemDC.SelectObject(&m_bmpCommon);
		m_bmpCommon.GetBitmap(&bmp);
		//pDC->BitBlt(0,0,rcItem.Width(),rcItem.Height(),&MemDC,0,0,SRCCOPY);
		pDC->StretchBlt(0,0,rcItem.Width(),rcItem.Height(),&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();
	}
	ReleaseDC(pDC);
}

void CAdvancedBitmapButton::OnMouseMove(UINT nFlags, CPoint point) 
{

	CRect clientRect;
	GetClientRect(&clientRect);
	CBitmap* pOldBmp;
	CDC MemDC;
	CDC* pDC=GetDC();
	BITMAP bmp = {0};
	CRect rect( clientRect.left+5, clientRect.top+5, clientRect.right-5, clientRect.bottom-5); 
	
	if (rect.PtInRect(point)){
		if(IsNotCapture){
			SetCapture();
			IsNotCapture = FALSE;
			MemDC.CreateCompatibleDC(pDC);
			pOldBmp = MemDC.SelectObject(&m_bmpSel);
			if(m_bmpSel.GetSafeHandle())
				m_bmpSel.GetBitmap(&bmp);
			pDC->StretchBlt(0,0,clientRect.Width(),clientRect.Height(),&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			MemDC.SelectObject(pOldBmp);
			MemDC.DeleteDC();
		}
	}
	else{
		MemDC.CreateCompatibleDC(pDC);
		pOldBmp=MemDC.SelectObject(&m_bmpCommon);
		if(m_bmpCommon.GetSafeHandle())
			m_bmpCommon.GetBitmap(&bmp);
		pDC->StretchBlt(0,0,clientRect.Width(),clientRect.Height(),&MemDC,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
		MemDC.SelectObject(pOldBmp);
		MemDC.DeleteDC();
	//	::SetCursor(::LoadCursor(::AfxGetInstanceHandle(), IDC_ARROW));
		ReleaseCapture();
		IsNotCapture = TRUE;
	}
	ReleaseDC(pDC);

	CButton::OnMouseMove(nFlags, point);
}

void CAdvancedBitmapButton::SetFocusCursor(UINT nIDCursor)
{
	if (nIDCursor==0){
		hFocusCur=NULL;
		return;
	}
	else
		hFocusCur=::AfxGetApp()->LoadCursor(nIDCursor);
}

void CAdvancedBitmapButton::AutoSize()
{
	CRect rect;
	BITMAP bp;

	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);
	//GetParent()->GetClientRect(&rect);

	//GetWindowRect(&rect);
	//rect.top-=GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYSIZEFRAME);
	if (m_bmpCommon.m_hObject!=NULL)
	{
		m_bmpCommon.GetBitmap(&bp);
		rect.bottom=rect.top+bp.bmHeight;
		rect.right=rect.left+bp.bmWidth;
	}
	else if (m_nWidth>0)
	{
		rect.bottom=rect.top+m_nHeight;
		rect.right=rect.left+m_nWidth;
	}
	else
		return;
	
	MoveWindow(&rect);
}
