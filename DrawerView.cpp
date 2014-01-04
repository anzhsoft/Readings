// DrawerView.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "DrawerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawerView

IMPLEMENT_DYNCREATE(CDrawerView, CView)

CDrawerView::CDrawerView()
{
}

CDrawerView::~CDrawerView()
{
}


BEGIN_MESSAGE_MAP(CDrawerView, CView)
	//{{AFX_MSG_MAP(CDrawerView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawerView drawing

void CDrawerView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CDrawerView diagnostics

#ifdef _DEBUG
void CDrawerView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawerView message handlers

void CDrawerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(cd.GetSafeHwnd()){
		if(cd.IsWindowVisible())
			cd.MoveWindow(0,0,cx,cy);
	}
}

int CDrawerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if(!cd.Create(IDD_drawers, this))
		return -1;
	cd.ShowWindow(SW_SHOW);
	return 0;
}
UINT CDrawerView::_CreateDrawers(LPVOID pParam)
{
	CDrawerView* p = (CDrawerView*)pParam;
	
	
	return 0;
}