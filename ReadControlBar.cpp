// ReadControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ReadControlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReadControlBar dialog


CReadControlBar::CReadControlBar(CWnd* pParent /*=NULL*/)
	: CDialog(CReadControlBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReadControlBar)
	m_page = _T("");
	//}}AFX_DATA_INIT
}


void CReadControlBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReadControlBar)
	DDX_Control(pDX, IDC_BUTTON_ZOOM, m_zoom);
	DDX_Control(pDX, IDC_BUTTON_MENU, m_menu);
	DDX_Control(pDX, IDC_BUTTON_RIGHT, m_right);
	DDX_Control(pDX, IDC_BUTTON_LEFT, m_left);
	DDX_Text(pDX, IDC_EDIT1, m_page);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReadControlBar, CDialog)
	//{{AFX_MSG_MAP(CReadControlBar)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_LEFT, OnUpPage)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnDownPage)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM, OnZoom)
	ON_BN_CLICKED(IDC_BUTTON_MENU, OnMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadControlBar message handlers

BOOL CReadControlBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_left.LoadBitmaps(IDB_CTLBAR_LEFT, 4, 0, 0, 0, 0);
	m_right.LoadBitmaps(IDB_CTLBAR_RIGHT, 4, 0, 0, 0, 0);
	m_menu.LoadBitmaps(IDB_CTLBAR_MENU, 4, 0, 0, 0, 0);
	m_zoom.LoadBitmaps(IDB_CTLBAR_ZOOM, 4, 0, 0, 0, 0);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CReadControlBar::OnEraseBkgnd(CDC* pDC) 
{
	//考虑到系统默认背景不同,才更改这个值
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect,RGB(236, 233, 216));
	return TRUE;
}

void CReadControlBar::OnUpPage() 
{
	GetParent()->PostMessage(WM_UP_PAGE);
}

void CReadControlBar::OnDownPage() 
{
	GetParent()->PostMessage(WM_DOWN_PAGE);
}

void CReadControlBar::OnZoom() 
{
	GetParent()->PostMessage(WM_ZOOM);
}

void CReadControlBar::OnMenu() 
{
	GetParent()->PostMessage(WM_BUTTON_MENU);	
}
