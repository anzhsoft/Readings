// DigHotkey.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "DigHotkey.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDigHotkey dialog

extern CMainFrame* pMainWnd;

CDigHotkey::CDigHotkey(CWnd* pParent /*=NULL*/)
	: CDialog(CDigHotkey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDigHotkey)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDigHotkey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDigHotkey)
	DDX_Control(pDX, IDC_TAB_MOSTNEW, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDigHotkey, CDialog)
	//{{AFX_MSG_MAP(CDigHotkey)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MOSTNEW, OnSelchangeTabMostnew)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigHotkey message handlers

void CDigHotkey::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
 
 	if(m_hotlistview!=NULL){
		if(m_hotlistview->GetSafeHwnd())
//	  		if(m_hotlistview->IsWindowVisible())
  				m_hotlistview->MoveWindow(1,30,cx,cy);
	}
	if (m_tab.GetSafeHwnd())
	{
//		if (m_tab.IsWindowVisible())
			m_tab.MoveWindow(0,0,cx,cy);
	}
	// TODO: Add your message handler code here
}

BOOL CDigHotkey::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_tab.InsertItem(0,"最新");  //添加参数一选项卡
	m_tab.InsertItem(1,"最热");  //添加参数二选项卡
	m_tab.InsertItem(2,"推荐");  

	CRect rs;
    m_tab.GetClientRect(rs);

    rs.top +=30;
	rs.bottom -=60;
	rs.left +=1;
	rs.right -=2;
	m_hotlistview = new CHotKeyListView;
	if(!m_hotlistview->Create(this,rs))
    {
        return FALSE;
    }

// 	CRuntimeClass* pViewClass=RUNTIME_CLASS(CHotKeyListView); 
// 	m_hotlistview =(CHotKeyListView*)pViewClass->CreateObject(); 
// 	m_hotlistview->MoveWindow(rs);
//	m_hotlistview->ShowWindow(SW_NORMAL);
	
  // m_hotlistview.Create(NULL,CView,WS_VISIBLE|WS_BORDER,rs,this,0);
   
	// TODO: Add extra initialization here

/*	
	m_hotkeynew.Create(IDD_DIALOG_TABNEW,GetDlgItem(IDC_TAB_MOSTNEW));
	CRect rs;
	m_tab.GetClientRect(&rs);
	rs.top +=20;
	rs.bottom -=60;
	rs.left +=1;
	rs.right -=2;
	
	m_hotkeynew.MoveWindow(&rs);
	m_hotkeynew.ShowWindow(true);
	m_tab.SetCurSel(0);
*/	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDigHotkey::OnSelchangeTabMostnew(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
	case 0:
		m_hotlistview->showFlag=0;
		break;
	case 1:
		m_hotlistview->showFlag=1;
		break;
	case 2:
		m_hotlistview->showFlag=2;
	break;
	}
	m_hotlistview->RefreshHotkey();
	*pResult = 0;
}


int CDigHotkey::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_hotlistview=NULL;
//	CListCtrl& list = GetListCtrl();
//	list.ModifyStyle(0, LVS_REPORT|LVS_SINGLESEL|LVS_OWNERDRAWFIXED);

	// TODO: Add your specialized creation code here
	
	return 0;
}

