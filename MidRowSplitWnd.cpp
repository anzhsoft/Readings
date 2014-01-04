// MidRowSplitWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "MidRowSplitWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMidRowSplitWnd

IMPLEMENT_DYNCREATE(CMidRowSplitWnd, CSplitterWnd)

CMidRowSplitWnd::CMidRowSplitWnd()
{
}

CMidRowSplitWnd::~CMidRowSplitWnd()
{
}

BEGIN_MESSAGE_MAP(CMidRowSplitWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CMidRowSplitWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMidRowSplitWnd::OnDrawSplitter(CDC *pDC, ESplitType nType, const CRect &rectArg)
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


				brect.left = (rect.left + rect.right)/4 - 25;
				brect.right = (rect.left + rect.right)/4 + 25;
				
				CRect brect2;
				brect2.top = rect.top;
				brect2.bottom = rect.bottom;
				brect2.left = 3*(rect.left + rect.right)/4 - 25;
				brect2.right = 3*(rect.left + rect.right)/4 + 75;

				m_btnRect2 = brect2;
				DrawLine(pDC);

			pDC->Draw3dRect(rect,RGB(214,211,206),RGB(214,211,206));
			pDC->FillSolidRect(rect,RGB(214,211,206));
			m_btnRect = brect;
			DrawLine(pDC);
			break;
	}
	
}
/*
int CMidRowSplitWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSplitterWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstResource;
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDC_CURSOR), RT_GROUP_CURSOR);
	m_hand = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(IDC_CURSOR),
		IMAGE_CURSOR, 0, 0, 0);
	return 0;
}*/
/*
void CMidRowSplitWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CSplitterWnd::OnMouseMove(nFlags, point);
	
	if(m_btnRect.PtInRect(point)){
		if(m_hand)
			SetCursor(m_hand);
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
//added by zuoss 20080807 //添加手行标记。
	if(m_btnRect2.PtInRect(point)){
		if(m_hand)
			SetCursor(m_hand);
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
//end add
}
*/
void CMidRowSplitWnd::DrawLine(CDC *pDC)
{
	CBitmap	cBitmap;
	
//added by zuoss 20080807	
	CBitmap cBitmap2;
	CBitmap* pOldMemBmp2 = NULL;
	CDC memDC2;

//	if (id == 1)
// 	{
		if(IsDown())
		{
			cBitmap.LoadBitmap(IDB_UP);
			cBitmap2.LoadBitmap(IDB_UP);
		}
		else if(IsUp())
		{
			cBitmap.LoadBitmap(IDB_DOWN);
			cBitmap2.LoadBitmap(IDB_DOWN);
		}
		else
		{
			cBitmap.LoadBitmap(IDB_DOWN);
			cBitmap2.LoadBitmap(IDB_UP);
		}
// 	}
// 	else
// 	{
//		if(IsDown())
//			cBitmap.LoadBitmap(IDB_UP);
//		else
//			cBitmap.LoadBitmap(IDB_DOWN);
// 	}
//end add

/**************
	if(IsDown())
		cBitmap.LoadBitmap(IDB_UP);
	else
		cBitmap.LoadBitmap(IDB_DOWN);
*************/
	CBitmap* pOldMemBmp = NULL;
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	pDC->BitBlt(m_btnRect.left, m_btnRect.top, m_btnRect.Width(), m_btnRect.Height(), &memDC, 0, 0, SRCCOPY);

	if(pOldMemBmp) 
		memDC.SelectObject(pOldMemBmp);

	cBitmap.DeleteObject();
	memDC.DeleteDC();
//	if (id == 1)
// 	{
		memDC.CreateCompatibleDC(pDC);
		pOldMemBmp = memDC.SelectObject(&cBitmap2);
		pDC->BitBlt(m_btnRect2.left,m_btnRect2.top,m_btnRect2.Width(),m_btnRect2.Height(),&memDC,0,0,SRCCOPY);
		if (pOldMemBmp)
		{
			memDC.SelectObject(pOldMemBmp);
		}		
// 	}
	cBitmap2.DeleteObject();
	memDC.DeleteDC();
}

BOOL CMidRowSplitWnd::IsUp()
{
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	if(cyCur0<20)
		return TRUE;
	return FALSE;
}

BOOL CMidRowSplitWnd::IsDown()
{
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	if(cyCur0>(cyCur0+cyCur1)*0.6)
		return TRUE;
	return FALSE;
}
/*
void CMidRowSplitWnd::OnLButtonDown(UINT nFlags, CPoint point) 
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
//added by zuoss 20080807
	if (m_btnRect2.PtInRect(point))
	{
		if (IsDown())
			ToTop();
		else if (IsUp())
			ToDown();
		else
			ToUp();
	}
//end add		

	CSplitterWnd::OnLButtonDown(nFlags, point);
}
*/
void CMidRowSplitWnd::ToDown()
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

void CMidRowSplitWnd::ToUp()
{
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	SetRowInfo(0, (cyCur0+cyCur1)*0.6-1, 0);
	RecalcLayout();
}

void CMidRowSplitWnd::ToTop()
{
	SetRowInfo(0, 0, 0);
	RecalcLayout();
}

void CMidRowSplitWnd::ToBottom()
{
	int cyCur0,cyCur1,cyMin;
	GetRowInfo(0, cyCur0, cyMin);
	GetRowInfo(1, cyCur1, cyMin);
	SetRowInfo(0, cyCur0+cyCur1, 0);
	RecalcLayout();
}

/////////////////////////////////////////////////////////////////////////////
// CMidRowSplitWnd message handlers

void CMidRowSplitWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_btnRect.PtInRect(point))
	{
		if(IsDown())
			ToUp();	
		else if(IsUp())
			ToDown();
		else
			ToBottom();
	}
//added by zuoss 20080807
	if (m_btnRect2.PtInRect(point))
	{
		if (IsDown())
			ToUp();
		else if (IsUp())
			ToDown();
		else
			ToTop();
	}
//end add	
	
	CSplitterWnd::OnLButtonDown(nFlags, point);
}

void CMidRowSplitWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CSplitterWnd::OnMouseMove(nFlags, point);
	if(m_btnRect.PtInRect(point)){
		if(m_hand)
			SetCursor(m_hand);
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	if(m_btnRect2.PtInRect(point)){
		if(m_hand)
			SetCursor(m_hand);
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
	}	

}


int CMidRowSplitWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSplitterWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
		HINSTANCE hInstResource;
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDC_CURSOR), RT_GROUP_CURSOR);
	m_hand = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(IDC_CURSOR),
		IMAGE_CURSOR, 0, 0, 0);
	
	return 0;
}
