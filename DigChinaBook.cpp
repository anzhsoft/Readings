// DigChinaBook.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "DigChinaBook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DigChinaBook dialog


DigChinaBook::DigChinaBook(CWnd* pParent /*=NULL*/)
	: CDialog(DigChinaBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(DigChinaBook)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DigChinaBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DigChinaBook)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DigChinaBook, CDialog)
	//{{AFX_MSG_MAP(DigChinaBook)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DigChinaBook message handlers

void DigChinaBook::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent ==1)
	{
		bgBmp.DeleteObject();
		EndDialog(IDOK);
		KillTimer(1);
	}
	
	CDialog::OnTimer(nIDEvent);
}

int DigChinaBook::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(1,10*1000,NULL);
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL DigChinaBook::OnInitDialog() 
{
	CDialog::OnInitDialog();
	bgBmp.LoadBitmap(IDB_BITMAP_ZST);
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL DigChinaBook::OnEraseBkgnd(CDC* pDC) 
{
	CRect	rect;
	GetWindowRect(&rect);
//	AfxMessageBox("d");

	CDC memDC;
	CBitmap			&cBitmap = bgBmp;
	CBitmap*		pOldMemBmp = NULL;
	
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	if(pOldMemBmp)
		memDC.SelectObject( pOldMemBmp );
	if(memDC.GetSafeHdc())
		memDC.DeleteDC();

	return TRUE;
}
