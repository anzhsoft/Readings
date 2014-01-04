// IEView.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "IEView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIEView

IMPLEMENT_DYNCREATE(CIEView, CHtmlView)

CIEView::CIEView()
{
	//{{AFX_DATA_INIT(CIEView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CIEView::~CIEView()
{
}

void CIEView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIEView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIEView, CHtmlView)
	//{{AFX_MSG_MAP(CIEView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIEView diagnostics

#ifdef _DEBUG
void CIEView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CIEView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIEView message handlers
