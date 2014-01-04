// Drawers.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "Drawers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CMainFrame* pMainWnd;
/////////////////////////////////////////////////////////////////////////////
// CDrawers dialog


CDrawers::CDrawers(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawers::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawers)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDrawers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawers)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawers, CDialog)
	//{{AFX_MSG_MAP(CDrawers)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawers message handlers

BOOL CDrawers::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	DWORD dwf = CGfxOutBarCtrl::fDragItems|CGfxOutBarCtrl::fEditGroups|CGfxOutBarCtrl::fEditItems|CGfxOutBarCtrl::fRemoveGroups|
		CGfxOutBarCtrl::fRemoveItems|CGfxOutBarCtrl::fAddGroups|CGfxOutBarCtrl::fAnimation|CGfxOutBarCtrl::fSelHighlight;
	CRect rc;
	GetClientRect(&rc);
	wndBar.Create(WS_CHILD|WS_VISIBLE, rc, this,
		IDC_LST_MENU, dwf);
	wndBar.SetOwner(this);
	wndBar.SetAnimationTickCount(10);
	wndBar.SetAnimSelHighlight(200);
	wndBar.AddFolder("读物下载",0);
	wndBar.AddFolder("读物管理",1);
	wndBar.AddFolder("高教课程",2);
	wndBar.AddFolder("订阅中心",3);
	wndBar.AddFolder("热点排行",4);
	//wndBar.AddFolder("个人信息",5);
	wndBar.SetSelFolder(0);
	wndBar.SetSelFolder(1);
	wndBar.SetSelFolder(2);
	wndBar.SetSelFolder(3);
//	wndBar.SetSelFolder(4);
	wndBar.SetSelFolder(0);
// 	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_CreateDrawers,(LPVOID)this,CREATE_SUSPENDED,NULL);
// 	ResumeThread(handle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDrawers::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if (wndBar.GetSafeHwnd())
	{
		if (wndBar.IsWindowVisible())
			wndBar.MoveWindow(0,0,cx,cy);
	}
}
unsigned __stdcall CDrawers::_CreateDrawers(LPVOID pParam)
{
	CDrawers* p = (CDrawers*)pParam;
	
	
	
	return 0;
}