// PersonInfo.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "PersonInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPersonInfo

IMPLEMENT_DYNCREATE(CPersonInfo, CView)

CPersonInfo::CPersonInfo()
{
}

CPersonInfo::~CPersonInfo()
{
}


BEGIN_MESSAGE_MAP(CPersonInfo, CView)
	//{{AFX_MSG_MAP(CPersonInfo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPersonInfo drawing

void CPersonInfo::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPersonInfo diagnostics

#ifdef _DEBUG
void CPersonInfo::AssertValid() const
{
	CView::AssertValid();
}

void CPersonInfo::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPersonInfo message handlers

void CPersonInfo::OnRefresh()
{

}
