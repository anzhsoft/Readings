// ReadingsView.cpp : implementation of the CReadingsView class
//

#include "stdafx.h"
#include "Readings.h"
#include "ReadingsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReadingsView

IMPLEMENT_DYNCREATE(CReadingsView, CView)

BEGIN_MESSAGE_MAP(CReadingsView, CView)
	//{{AFX_MSG_MAP(CReadingsView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadingsView construction/destruction

CReadingsView::CReadingsView()
{
	// TODO: add construction code here

}

CReadingsView::~CReadingsView()
{
}

BOOL CReadingsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CReadingsView drawing

void CReadingsView::OnDraw(CDC* pDC)
{
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CReadingsView printing

BOOL CReadingsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CReadingsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CReadingsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CReadingsView diagnostics

#ifdef _DEBUG
void CReadingsView::AssertValid() const
{
	CView::AssertValid();
}

void CReadingsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CReadingsView message handlers
