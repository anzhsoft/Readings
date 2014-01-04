// HotKeyView.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "HotKeyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHotKeyView

IMPLEMENT_DYNCREATE(CHotKeyView, CView)

CHotKeyView::CHotKeyView()
{
}

CHotKeyView::~CHotKeyView()
{
}


BEGIN_MESSAGE_MAP(CHotKeyView, CView)
	//{{AFX_MSG_MAP(CHotKeyView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHotKeyView drawing

void CHotKeyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHotKeyView diagnostics

#ifdef _DEBUG
void CHotKeyView::AssertValid() const
{
	CView::AssertValid();
}

void CHotKeyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHotKeyView message handlers

int CHotKeyView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
 		if(!m_hotDlg.Create(IDD_DIALOG_HOTKEY, this))
 			return -1;
 		m_hotDlg.ShowWindow(SW_SHOW);

	return 0;
}

void CHotKeyView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	 	if(m_hotDlg.GetSafeHwnd()){
 			m_hotDlg.MoveWindow(0,0,cx,cy);
		}
}
BOOL CHotKeyView::Create(CWnd *pWnd,CRect rect)
{
	return CView::Create(NULL, NULL,  WS_VISIBLE|WS_BORDER,rect, pWnd, 1234);
	return TRUE;
}
