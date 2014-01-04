// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char CurrentPath[_MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_LIST_SEL, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SEL, OnItemchangedListSel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitialList();

	CRect rect;
	GetClientRect(rect);

	m_normalPg.Create(IDD_CONFIGPAGE_NORMAL, this);
	m_normalPg.ShowWindow(SW_SHOW);
	m_normalPg.MoveWindow(170,0,rect.Width()-180, rect.Height()-40);

	m_sharePg.Create(IDD_CONFIGPAGE_SHARE,this);
	m_sharePg.ShowWindow(SW_HIDE);
	m_sharePg.MoveWindow(170,0,rect.Width()-180, rect.Height()-40);

	m_favorPg.Create(IDD_CONFIGPAGE_FAVOR, this);
	m_favorPg.ShowWindow(SW_HIDE);
	m_favorPg.MoveWindow(170,0,rect.Width()-180, rect.Height()-40);

	m_serverPg.Create(IDD_CONFIGPAGE_SERVER, this);
	m_serverPg.ShowWindow(SW_HIDE);
	m_serverPg.MoveWindow(170,0,rect.Width()-180, rect.Height()-40);

	m_pushPg.Create(IDD_CONFIGPAGE_PUSH, this);
	m_pushPg.ShowWindow(SW_HIDE);
	m_pushPg.MoveWindow(170,0,rect.Width()-180, rect.Height()-40);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::InitialList()
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BMPLIST_CONFIG);
	CImageList il;
	il.Create(32, 32, ILC_COLORDDB|ILC_MASK, 11, 1);
	il.Add(&bitmap, RGB(236,233,214));
	m_list.SetImageList(&il, LVSIL_SMALL);
	bitmap.DeleteObject();
	il.Detach();

	m_list.InsertColumn(0, "head",LVCFMT_LEFT, 140);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_list.InsertItem(0,"常规", 0);
	m_list.InsertItem(1,"共享", 1);
	m_list.InsertItem(2,"服务器", 2);
	m_list.InsertItem(3,"收藏夹管理",3);
	m_list.InsertItem(4,"推送服务",4);
}

void CConfigDlg::OnOK() 
{
	UpdateData(TRUE);

	m_normalPg.SaveInfo();
	m_sharePg.SaveInfo();
	m_favorPg.SaveInfo();
	m_serverPg.SaveInfo();
	m_pushPg.SaveInfo();
	MessageBox("您所进行的某些设置，需要在下一次运行本软件的时候才能起作用。","提示", MB_OK);
	CDialog::OnOK();
}

void CConfigDlg::OnDestroy() 
{
	CImageList* il;
	il = m_list.GetImageList(LVSIL_SMALL);
	if(il)
		il->DeleteImageList();

	CDialog::OnDestroy();
}

void CConfigDlg::OnItemchangedListSel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iItem<0)
		return;

	if(pNMListView->uNewState != (LVIS_SELECTED|LVIS_FOCUSED))
		return;

	m_normalPg.ShowWindow(SW_HIDE);
	m_sharePg.ShowWindow(SW_HIDE);
	m_favorPg.ShowWindow(SW_HIDE);
	m_serverPg.ShowWindow(SW_HIDE);
	m_pushPg.ShowWindow(SW_HIDE);

	switch(pNMListView->iItem){
	case 0:
		m_normalPg.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_sharePg.ShowWindow(SW_SHOW);
		break;
	case 2:
		m_serverPg.ShowWindow(SW_SHOW);
		break;
	case 3:
		m_favorPg.ShowWindow(SW_SHOW);
		break;
	case 4:
		m_pushPg.ShowWindow(SW_SHOW);
		break;
	default:
		m_normalPg.ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}
