// CommView.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "CommView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommView

IMPLEMENT_DYNCREATE(CCommView, CView)

CCommView::CCommView()
{
}

CCommView::~CCommView()
{
}


BEGIN_MESSAGE_MAP(CCommView, CView)
	//{{AFX_MSG_MAP(CCommView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommView drawing

void CCommView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCommView diagnostics

#ifdef _DEBUG
void CCommView::AssertValid() const
{
	CView::AssertValid();
}

void CCommView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCommView message handlers

int CCommView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_CommDlg.Create(IDD_DIALOG_COMMDLG, this))
		return -1;

	m_CommDlg.ShowWindow(SW_SHOW);
	
	return 0;
}

void CCommView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
 	if(m_CommDlg.GetSafeHwnd())
 		m_CommDlg.MoveWindow(0,0,cx,cy);
	
}