// TalkDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "TalkDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTalkDialog dialog


CTalkDialog::CTalkDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTalkDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTalkDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTalkDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTalkDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTalkDialog, CDialog)
	//{{AFX_MSG_MAP(CTalkDialog)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTalkDialog message handlers

BOOL CTalkDialog::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetWindowRect(&rect);

	CDC memDC;
	CBitmap	&cBitmap = bgBmp;
	CBitmap* pOldMemBmp = NULL;
	
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	if(pOldMemBmp)
		memDC.SelectObject( pOldMemBmp );
	if(memDC.GetSafeHdc())
		memDC.DeleteDC();

	return TRUE;
}

BOOL CTalkDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	bgBmp.LoadBitmap(IDB_BITMAP_DIALOG_BG);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
