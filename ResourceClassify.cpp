// ResourceClassify.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ResourceClassify.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL offline;
extern char CurrentPath[_MAX_PATH];
extern char JspServer[1024];
extern char AspServer[1024];
extern char UserName[50];
extern char SDDisk[20];
extern char FavorPath[_MAX_PATH];
extern CMainFrame* pMainWnd;
extern _ConnectionPtr m_pConnection;
extern char AspServer[1024];
extern char CurrentPath[_MAX_PATH];

//extern pDownload1 DownLoad1;
/////////////////////////////////////////////////////////////////////////////
// ResourceClassify dialog


ResourceClassify::ResourceClassify(CWnd* pParent /*=NULL*/)
	: CDialog(ResourceClassify::IDD, pParent)
{
	//{{AFX_DATA_INIT(ResourceClassify)
	//}}AFX_DATA_INIT
}

ResourceClassify::ResourceClassify(const char * group, int id, HTREEITEM hAllGroup, CWnd* pParent)
	: CDialog(ResourceClassify::IDD, pParent)
{
	for (int i=0;i<3;i++)
	{
		gID[i] = -1;
	}
	sid = id;
	class1 = (char*)malloc(strlen(group)+1);
	memset(class1,0,strlen(group)+1);
	memcpy(class1,group,strlen(group));
	GroupRoot = hAllGroup;
	
}

void ResourceClassify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(ResourceClassify)
	DDX_Control(pDX, IDC_TYPELIST, m_typelist);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_del);
	DDX_Control(pDX, IDC_TREE_GROUP, m_pTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ResourceClassify, CDialog)
	//{{AFX_MSG_MAP(ResourceClassify)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonDel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_GROUP, OnSelchangedTreeGroup)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ResourceClassify message handlers

void ResourceClassify::OnOK() 
{
	// TODO: Add extra validation here
	int count = m_typelist.GetCount();
	if (count==0)
	{
		CDialog::OnOK();
	}
	//为gID赋值
	for (int i=0;i<count;i++){
//		 DWORD m = m_typelist.GetItemData(i);
	 	TreeItemInfo* info2 = (TreeItemInfo*)m_typelist.GetItemData(i);
		if (info2!=NULL)
		{
			GROUPINFO* ginfo = (GROUPINFO*)info2->BookNumber;
			gID[i]= ginfo->gid;
		}
	}
	//add by dongzhenhua 0514  ???改了就可以了？
	if(gID[0]>0 || gID[1]>0 || gID[2]>0)
	    UpdateGroupFile();
	CDialog::OnOK();
}

BOOL ResourceClassify::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitialImageList();
	TreeItemInfo* info = (TreeItemInfo*)m_pTree.GetItemData(GroupRoot);
	if(!info)
		return 1;

	//为list赋初值,以后改进
	if(strcmp(class1,"无") !=0)
    	m_typelist.InsertString(m_typelist.GetCount(),class1);


	//为树赋值
	CString m_text = m_pTree.GetItemText(GroupRoot); 
    HTREEITEM root = m_pTree.InsertItem(m_text, 8, 8, NULL);
	
	//部落号 赋值给树节点
//	int gid = info->BookNumber;//
	if(LB_ERR == m_pTree.SetItemData(root,(DWORD)info))
		return FALSE;
	CopyTree(GroupRoot,root);
//	HTREEITEM child = m_pTree.GetChildItem(GroupRoot);
//	CString fchildtest = m_pTree.GetItemText(child);
	
//	HTREEITEM fchild = m_pTree.InsertItem(fchildtest,2,2,root);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL ResourceClassify::InitialImageList()
{
try{
	m_pTree.ModifyStyle(0,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS);
	CBitmap bitmap;
	if(!bitmap.LoadBitmap(IDB_IMGLIST_TREE))
		return FALSE;
	CImageList imgList;
	if(!imgList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 11, 1))
		return FALSE;
	if(imgList.Add(&bitmap, RGB(236,233,214))==-1)
		return FALSE;
	m_pTree.SetImageList(&imgList, TVSIL_NORMAL);
	bitmap.DeleteObject();
	imgList.Detach();
}
catch(...){
#ifdef _DEBUG
	Tips("BOOL CGuidTreeView::InitialImageList()");
#endif
}
	return TRUE;
}

void ResourceClassify::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void ResourceClassify::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	int cur = m_typelist.GetCurSel();
	gID[cur] = -1;
	m_typelist.DeleteString(cur);
}

BOOL ResourceClassify::CopyTree(HTREEITEM Or, HTREEITEM New)
{
	HTREEITEM Or_FChild = m_pTree.GetChildItem(Or);
	if (Or_FChild == NULL)
	{
		return FALSE;
	}
	//设置第一个 子节点
	CString ftext = m_pTree.GetItemText(Or_FChild);
	HTREEITEM New_FChild = m_pTree.InsertItem(ftext,2,2,New);
	//为节点赋值：gid
	TreeItemInfo* info1 = (TreeItemInfo*)m_pTree.GetItemData(Or_FChild);
//	int gid = info1->BookNumber;//
	if(LB_ERR == m_pTree.SetItemData(New_FChild,(DWORD)info1))
		return FALSE;
	CopyTree(Or_FChild,New_FChild);
	//设置兄弟结点
	HTREEITEM Or_BChild = m_pTree.GetNextSiblingItem(Or_FChild);
	while (Or_BChild!=NULL)
	{
		CString btext = m_pTree.GetItemText(Or_BChild);
		HTREEITEM New_BChild = m_pTree.InsertItem(btext,2,2,New);
	//为节点赋值：gid
	TreeItemInfo* info2 = (TreeItemInfo*)m_pTree.GetItemData(Or_BChild);
//	DWORD gid2 = info2->BookNumber;
	if(LB_ERR == m_pTree.SetItemData(New_BChild,(DWORD)info2))
		return FALSE;

		CopyTree(Or_BChild,New_BChild);
		Or_BChild = m_pTree.GetNextSiblingItem(Or_BChild);
	}
	return TRUE;
}
/*
void ResourceClassify::OnClickTreeGroup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_typelist.AddString("fds");
	*pResult = 0;
}*/

void ResourceClassify::OnSelchangedTreeGroup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM	hItem = m_pTree.GetSelectedItem();
	//若非叶子节点则返回
	if (m_pTree.GetChildItem(hItem)!=NULL)
		return;
	if(m_typelist.GetCount()>=3)
		return;
	
//	int groupid = (int)m_pTree.GetItemData(hItem);
	TreeItemInfo* info = (TreeItemInfo*)m_pTree.GetItemData(hItem);

	if(info->Type == Is_NetGroup_Level)
			return;	

	CString contenttext = m_pTree.GetItemText(hItem);
	//为新的list项赋值：
	m_typelist.InsertString(m_typelist.GetCount(),contenttext);//AddString(contenttext);
	int index = m_typelist.GetCount()-1;
	m_typelist.SetItemData(index,(DWORD)info);
	*pResult = 0;
}

BOOL ResourceClassify::UpdateGroupFile()
{

	//CoInitialize(NULL);

	
	char HttpAdd[1024] = {0};
	char Tadd[1024] = {0};
	sprintf_s(HttpAdd, 1024, "%s/share/updategroupfile.asp?sid=%d&gid0=%d&gid1=%d&gid2=%d", AspServer, sid,gID[0],gID[1],gID[2]);
	sprintf_s(Tadd, 1024, "%s\\temp\\update.asp", CurrentPath);
	try{
	  if(!DownLoad(HttpAdd,Tadd, pMainWnd->m_quit, pMainWnd->m_socketList, pMainWnd->m_mutex))
		  return FALSE;
			//file下载失败
	}
	catch(_com_error& e){
			return FALSE;
	}
		return TRUE;

}
