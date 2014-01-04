// SplitWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "SplitWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL offline;

/////////////////////////////////////////////////////////////////////////////
// CSplitWnd

IMPLEMENT_DYNCREATE(CSplitWnd, CSplitterWnd)

CSplitWnd::CSplitWnd()
{
	/*
	m_cxSplitter = 4;		//must >=4 ,拖动时拖动条的宽度
	m_cySplitter = 4;
	m_cxBorderShare = 1;	//按下鼠标时拖动条的偏移量
	m_cyBorderShare = 1;
	*/
	m_cxSplitterGap = 8;		//splitter拖动条的宽度
	m_cySplitterGap = 8;
	
	m_btnRect[0] = (0,0,0,0);
	m_btnRect[1] = (0,0,0,0);
}

CSplitWnd::~CSplitWnd()
{
}


BEGIN_MESSAGE_MAP(CSplitWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CSplitWnd)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitWnd message handlers


void CSplitWnd::OnDrawSplitter(CDC *pDC, ESplitType nType, const CRect &rectArg)
{
	if (pDC == NULL)
	{
		RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);
	CRect rect = rectArg;
	UINT nBar;

	switch(nType) 
	{
		case splitBorder:
			pDC->Draw3dRect(rect,RGB(214,211,206),RGB(214,211,206));
			if(rect.bottom - rect.top < 10)
				pDC->FillSolidRect(rect,RGB(214,211,206));
			break;
        case splitBar: 			
			CRect brect;
			brect.left = rect.left;
			brect.right = rect.right;

			brect.top = (rect.top + rect.bottom)/2 - 25;
			brect.bottom = (rect.top + rect.bottom)/2 + 25;
			
			pDC->Draw3dRect(rect,RGB(214,211,206),RGB(214,211,206));
			pDC->FillSolidRect(rect,RGB(214,211,206));
			nBar = GetSplitBar(brect);
			if(0 == nBar)
				m_btnRect[0] = brect;
			else
				m_btnRect[1] = brect;
			DrawLine(pDC, nBar);
			break;
	}
//	pDC->FillSolidRect(m_btnRect,RGB(255,0,0));
}

int CSplitWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSplitterWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstResource;
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDC_CURSOR), RT_GROUP_CURSOR);
	m_hand = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(IDC_CURSOR),
		IMAGE_CURSOR, 0, 0, 0);
	return 0;
}

void CSplitWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CSplitterWnd::OnMouseMove(nFlags, point);
	
	if(m_btnRect[0].PtInRect(point)||m_btnRect[1].PtInRect(point)){
		if(m_hand)
			SetCursor(m_hand);
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
}

void CSplitWnd::DrawLine(CDC *pDC, UINT nBar)
{
	CRect rect;
	CBitmap	cBitmap;
	if(0 == nBar){
		rect = m_btnRect[0];
		if(IsLeft())
			cBitmap.LoadBitmap(IDB_RIGHT);
		else
			cBitmap.LoadBitmap(IDB_LEFT);
	}
	else{
		rect = m_btnRect[1];
		if(IsRight())
			cBitmap.LoadBitmap(IDB_LEFT);
		else
			cBitmap.LoadBitmap(IDB_RIGHT);
	}
	
	CBitmap* pOldMemBmp = NULL;
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	if (pOldMemBmp) 
		memDC.SelectObject(pOldMemBmp);

	cBitmap.DeleteObject();
	memDC.DeleteDC();
}

BOOL CSplitWnd::IsLeft()
{
	int cur, min;
	GetColumnInfo(0, cur, min);
	if(cur<170)
		return TRUE;
	
	return FALSE;
}

BOOL CSplitWnd::IsRight()
{	
	if(offline)
		return FALSE;
	int cur, min;
	GetColumnInfo(2, cur, min);
	if(cur<180)
		return TRUE;
	
	return FALSE;
}

void CSplitWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_btnRect[0].PtInRect(point)){
		if(IsLeft())
			Left2Right();
		else
			Left2Left();
	}
	else if(m_btnRect[1].PtInRect(point)){
		if(IsRight())
			Right2Left();
		else
			Right2Right();
	}
	CSplitterWnd::OnLButtonDown(nFlags, point);
}

UINT CSplitWnd::GetSplitBar(CRect rect)
{
	int cxMin, cxCur;
	GetColumnInfo(0, cxCur, cxMin);
	if(cxCur-5<rect.left && cxCur+5>rect.left)
		return 0;
	else
		return 1;
}

void CSplitWnd::Left2Left()
{
	int cxCur0, cxCur1, cxMin;
	GetColumnInfo(0, cxCur0, cxMin);
	GetColumnInfo(1, cxCur1, cxMin);
	SetColumnInfo(0, 0, 0);
	SetColumnInfo(1, cxCur0+cxCur1, 0);
	RecalcLayout();
}

void CSplitWnd::Left2Right()
{
	int cxCur1, cxMin;
	GetColumnInfo(1, cxCur1, cxMin);
	SetColumnInfo(0, 170, 0);
	SetColumnInfo(1, cxCur1-170, 0);
	RecalcLayout();
}

void CSplitWnd::Right2Left()
{
	if(offline)
		return;
	int cxCur0, cxCur1, cxMin;
	GetColumnInfo(0, cxCur0, cxMin);
	GetColumnInfo(1, cxCur1, cxMin);
	SetColumnInfo(0, cxCur0, 0);
	SetColumnInfo(1, cxCur1-182, 0);
	SetColumnInfo(2, 182, 0);
	RecalcLayout();
}

void CSplitWnd::Right2Right()
{
	if(offline)
		return;
	int cxCur0, cxCur1, cxCur2, cxMin;
	GetColumnInfo(0, cxCur0, cxMin);
	GetColumnInfo(1, cxCur1, cxMin);
	GetColumnInfo(2, cxCur2, cxMin);
	SetColumnInfo(0, cxCur0, 0);
	SetColumnInfo(1, cxCur1+cxCur2, 0);
	SetColumnInfo(2, 0, 0);
	RecalcLayout();
}
