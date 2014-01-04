// SynListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "SynListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSynListDlg dialog


CSynListDlg::CSynListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSynListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSynListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSynListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSynListDlg)
	DDX_Control(pDX, IDC_SYNLIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSynListDlg, CDialog)
	//{{AFX_MSG_MAP(CSynListDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELALL, OnButtonSelall)
	ON_BN_CLICKED(IDC_BUTTON_CANCELALL, OnButtonCancelall)
	ON_BN_CLICKED(IDC_BUTTON_SELHIGHLIGHT, OnButtonSelhighlight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSynListDlg message handlers

BOOL CSynListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);//|LVS_EX_GRIDLINES);
	char ColName[8][20] = {"书名","分类","文件类型","路径"};
	long ColWidth[8] = {160, 80, 80, 300};
	for(int i = 0; i < 4; i++)
		m_list.InsertColumn(i, ColName[i], LVCFMT_LEFT, ColWidth[i]);
	CImageList il;
	CBitmap	bitmap;
	bitmap.LoadBitmap(IDB_IMGLIST_FILE);
	il.Create(16, 16, ILC_COLORDDB|ILC_MASK, 3, 1);
	il.Add(&bitmap, RGB(236,233,214));
	m_list.SetImageList(&il,LVSIL_SMALL);
	il.Detach();
	bitmap.Detach();
	bitmap.DeleteObject();
	il.DeleteImageList();

	InsertItem();

	return TRUE;
}

void CSynListDlg::InsertItem()
{
	POSITION pos = m_synlist->GetHeadPosition();
	int cnt = 0;
	while(pos){
		LOCALLIST locdata = m_synlist->GetNext(pos);
		int icontype;
		CString filetype;
		icontype = GetSmallIcon(locdata.FileType);
		filetype = GetFileExtentName(locdata.FileType);
		CString title = locdata.Title;
		if(title.ReverseFind('.')==-1)
			m_list.InsertItem(cnt, title, icontype);
		else
			m_list.InsertItem(cnt, title.Left(title.ReverseFind('.')), icontype);
		m_list.SetItemText(cnt, 1, locdata.Type);
		m_list.SetItemText(cnt, 2, filetype);
		m_list.SetItemText(cnt, 3, locdata.FilePath);
		m_list.SetCheck(cnt, TRUE);
		cnt++;
	}
}

void CSynListDlg::OnOK() 
{
	m_synlist->RemoveAll();
	for(int index=0; index<m_list.GetItemCount(); index++){
		if(!m_list.GetCheck(index))
			continue;
		LOCALLIST locdata = {0};
		sprintf_s(locdata.Title, 256, "%s.%s", m_list.GetItemText(index,0), m_list.GetItemText(index,2));
		strcpy_s(locdata.Type, 20, m_list.GetItemText(index,1));
		strcpy_s(locdata.FilePath, 1024, m_list.GetItemText(index,3));
		locdata.FileType = GetFileTypeFromExt(m_list.GetItemText(index,2));
		m_synlist->AddTail(locdata);
	}
	CDialog::OnOK();
}

void CSynListDlg::OnButtonSelall() 
{
	for(int index=0; index<m_list.GetItemCount(); index++)
		m_list.SetCheck(index, TRUE);
}

void CSynListDlg::OnButtonCancelall() 
{
	for(int index=0; index<m_list.GetItemCount(); index++)
		m_list.SetCheck(index, FALSE);
}

void CSynListDlg::OnButtonSelhighlight() 
{
	for(int index=0; index<m_list.GetItemCount(); index++)
		if(m_list.GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED)
			m_list.SetCheck(index, TRUE);
}
