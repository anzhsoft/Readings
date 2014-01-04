// RowSplitWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "RowSplitWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRowSplitWnd
IMPLEMENT_DYNCREATE(CRowSplitWnd, CSplitterWnd)

CRowSplitWnd::CRowSplitWnd()
{

}

CRowSplitWnd::~CRowSplitWnd()
{
	
}


BEGIN_MESSAGE_MAP(CRowSplitWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CRowSplitWnd)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRowSplitWnd message handlers

void CRowSplitWnd::OnDrawSplitter(CDC *pDC, ESplitType nType, const CRect &rectArg)
{
	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);
	CRect rect = rectArg;

	switch(nType) 
	{
		case splitBorder:
			pDC->Draw3dRect(rect,RGB(214,211,206),RGB(214,211,206));
			if(rect.bottom - rect.top < 10)
				pDC->FillSolidRect(rect,RGB(214,211,206));
			break;
        case splitBar: 			
			CRect brect;
			brect.top = rect.top;
			brect.bottom = rect.bottom;

			brect.left = (rect.left + rect.right)/2 - 25;
			brect.right = (rect.left + rect.right)/2 + 25;

			pDC->Draw3dRect(rect,RGB(214,211,206),RGB(214,211,206));
			pDC->FillSolidRect(rect,RGB(214,211,206));
			m_btnRect = brect;
			DrawLine(pDC);
			break;
	}
	//pDC->FillSolidRect(m_btnRect,RGB(255,0,0));
}

int CRowSplitWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSplitterWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstResource;
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDC_CURSOR), RT_GROUP_CURSOR);
	m_hand = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(IDC_CURSOR),
		IMAGE_CURSOR, 0, 0, 0);
	return 0;
}

void CRowSplitWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CSplitterWnd::OnMouseMove(nFlags, point);
	
	if(m_btnRect.PtInRect(point)){
		if(m_hand)
			SetCursor(m_hand);
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
}

void CRowSplitWnd::DrawLine(CDC *pDC)
{
	CBitmap	cBitmap;

	if(IsDown())
		cBitmap.LoadBitmap(IDB_UP);
	else
		cBitmap.LoadBitmap(IDB_DOWN);

	CBitmap* pOldMemBmp = NULL;
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	pDC->BitBlt(m_btnRect.left, m_btnRect.top, m_btnRect.Width(), m_btnRect.Height(), &memDC, 0, 0, SRCCOPY);

	if(pOldMemBmp) 
		memDC.SelectObject(pOldMemBmp);

	cBitmap.DeleteObject();
	memDC.DeleteDC();
}

BOOL CRowSplitWnd::IsUp()
{
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	if(cyCur0<20)
		return TRUE;
	return FALSE;
}

BOOL CRowSplitWnd::IsDown()
{
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	if(cyCur0>(cyCur0+cyCur1)*0.6)
		return TRUE;
	return FALSE;
}

void CRowSplitWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_btnRect.PtInRect(point))
	{
		if(IsDown())
			ToUp();	
		else if(IsUp())
			ToDown();
		else
			ToBottom();
	}
	CSplitterWnd::OnLButtonDown(nFlags, point);
}


void CRowSplitWnd::ToDown()
{
	/*********
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	SetRowInfo(0, cyCur0+cyCur1, 0);
	SetRowInfo(1, 0, 0);
	RecalcLayout();
	**********/

//added by zuoss 20080807 //分割条置于中下方。
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	SetRowInfo(0, (cyCur0+cyCur1)*0.6-1, 0);
	RecalcLayout();
//end add
}

void CRowSplitWnd::ToUp()
{
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	SetRowInfo(0, (cyCur0+cyCur1)*0.6-1, 0);
	RecalcLayout();
}

void CRowSplitWnd::ToTop()
{
	SetRowInfo(0, 0, 0);
	RecalcLayout();
}

void CRowSplitWnd::ToBottom()
{
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	SetRowInfo(0, cyCur0+cyCur1, 0);
	RecalcLayout();
}
