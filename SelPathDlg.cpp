// SelPathDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "SelPathDlg.h"
#include "InputDlg.h"
#include "FileFindEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelPathDlg dialog
CTreeCtrl* pTreeCtrl;

extern char FavorPath[_MAX_PATH];
extern char CurrentPath[_MAX_PATH];
extern char UserName[50];
extern char SDDisk[20];
extern USB_DETAIL_INFO uDiskAuthInfo;

CSelPathDlg::CSelPathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelPathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelPathDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSelPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelPathDlg)
	DDX_Control(pDX, IDC_TREE_DOWNLOAD, m_ctrTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelPathDlg, CDialog)
	//{{AFX_MSG_MAP(CSelPathDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DOWNLOAD, OnSelchangedTreeDownload)
	ON_BN_CLICKED(IDC_BUTTON_NEWTYPE, OnButtonNewtype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelPathDlg message handlers


BOOL CSelPathDlg::OnInitDialog()
{
//本地根文件夹 0； 阅读器根文件夹1；本地子文件夹 2； 阅读器子文件夹3
//本地二级子文件夹 4； 阅读器二级子文件夹5
	CDialog::OnInitDialog();

	pTreeCtrl = &m_ctrTree;

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_IMGLIST_TREE);
	CImageList m_image;
	m_image.Create(16, 16, ILC_COLORDDB|ILC_MASK, 11, 1);
	m_image.Add(&bitmap, RGB(236,233,214));
	m_ctrTree.SetImageList(&m_image, TVSIL_NORMAL);	
	m_image.Detach();
	bitmap.Detach();
	bitmap.DeleteObject();

	char Path[_MAX_PATH] = {0};
	sprintf_s(Path, _MAX_PATH, "%s/temp/%s/*.", CurrentPath, UserName);
	if(!PathFileExists(Path))
		_mkdir(Path);

	localItem = m_ctrTree.InsertItem("我的收藏夹", 6, 6);
	m_ctrTree.SetItemData(localItem, Is_MyFavourate);
	DWORD data = 0;
	CFileFindEx find(InsertFolder, SetFolderStatics, CountFile);
	find.Find(FavorPath, &localItem, data);

	if(strlen(SDDisk)>0)// && uDiskAuthInfo.flag && uDiskAuthInfo.diskFlag == SDDisk[0]){//modified by zhanganzhan on 090914
	{	removeableItem = m_ctrTree.InsertItem("我的阅读器", 7, 7);
		m_ctrTree.SetItemData(removeableItem, 1);
		CFileFindEx findSD(InsertSDFolder, SetFolderStatics, CountFile);
		findSD.Find(SDDisk, &removeableItem, data);
	}

	m_ctrTree.SetFocus();
	if(defSel==PC2SD){
		m_ctrTree.Select(removeableItem,TVGN_CARET);
	}
	else{
		m_ctrTree.Select(localItem,TVGN_CARET);
	}

	flag = FALSE;
	
	return TRUE;
}

void CSelPathDlg::OnSelchangedTreeDownload(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
}

void CSelPathDlg::OnButtonNewtype() 
{
	HTREEITEM hItem = m_ctrTree.GetSelectedItem();
	if(!hItem)
		return;

	CInputDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	CString relativePath;
	DWORD info = m_ctrTree.GetItemData(hItem);
	HTREEITEM hCurItem = hItem;
	while(info==Is_MyFavourate_Folder||info==Is_SDCard_Folder){
		CString tempPath = relativePath;
		CString name = m_ctrTree.GetItemText(hCurItem);
		int index = -1;
		if((index = name.ReverseFind('('))<0)
			relativePath.Format("%s\\%s", name, tempPath);
		else
			relativePath.Format("%s\\%s", name.Left(index), tempPath);
		hCurItem = m_ctrTree.GetParentItem(hCurItem);
		info = m_ctrTree.GetItemData(hCurItem);
	};
	CString tempPath = relativePath;
	relativePath.Format("%s%s", tempPath,dlg.m_str);

	CString curFolderPath;
	if(info==Is_MyFavourate)
		curFolderPath.Format("%s\\%s", FavorPath, relativePath);
	else
		curFolderPath.Format("%s\\%s", SDDisk, relativePath);
	
	if(PathFileExists(curFolderPath)){
		MessageBox("该分类已经存在！", "错误", MB_OK);
		return;
	}
	if(-1==_mkdir(curFolderPath)){
		MessageBox("分类创建失败！", "错误", MB_OK);
		return;
	}
	HTREEITEM hNewItem = m_ctrTree.InsertItem(dlg.m_str, 4, 4, hItem);
	if(info==Is_MyFavourate)
		m_ctrTree.SetItemData(hNewItem, Is_MyFavourate_Folder);
	else
		m_ctrTree.SetItemData(hNewItem, Is_SDCard_Folder);
	m_ctrTree.Expand(hItem,TVE_EXPAND);
	m_ctrTree.SelectItem(hNewItem);

	flag = TRUE;
}

void CSelPathDlg::OnOK() 
{
	HTREEITEM hItem =  m_ctrTree.GetSelectedItem();
	if(!hItem)
		return;

	strPath.Empty();

	CString relativePath;
	DWORD info = 0;
	HTREEITEM hCurItem = hItem;
	do{
		CString tempPath = relativePath;
	
	//	AfxMessageBox(relativePath);
		CString name = m_ctrTree.GetItemText(hCurItem);
		int index = -1;
		if((index = name.ReverseFind('('))<0){		
			relativePath.Format("%s\\%s", name, tempPath);
		}
		else{
			relativePath.Format("%s\\%s", name.Left(index), tempPath);
		}
		hCurItem = m_ctrTree.GetParentItem(hCurItem);
		if (hCurItem!=NULL)
			info = m_ctrTree.GetItemData(hCurItem);
	}while(info==Is_MyFavourate_Folder||info==Is_SDCard_Folder);

	if(info==Is_MyFavourate)
		strPath.Format("%s\\%s", FavorPath, relativePath);
	else
		strPath.Format("%s\\%s", SDDisk, relativePath);

	CDialog::OnOK();
}

LPVOID CSelPathDlg::InsertFolder(LPCSTR Path, WIN32_FIND_DATA* pData,
							 LPVOID pfParam, DWORD& subdata)
{
try{
	if(_stricmp(pData->cFileName,"Readman") == 0)
		return NULL;
	else if(_stricmp(pData->cFileName,"_Adv") == 0)
		return NULL;

	HTREEITEM* pfItem = (HTREEITEM*)pfParam;
	HTREEITEM* pcurItem = new HTREEITEM;
	HTREEITEM curItem = pTreeCtrl->InsertItem(pData->cFileName, 4, 4, *pfItem);
	memcpy(pcurItem, &curItem, sizeof(HTREEITEM));
	pTreeCtrl->SetItemData(*pcurItem, Is_MyFavourate_Folder);
	subdata = 0;
	return (LPVOID)pcurItem;
}
catch(...){
#ifdef _DEBUG
	Tips("LPVOID CSelPathDlg::InsertFolder(LPCSTR Path, WIN32_FIND_DATA* pData, LPVOID pfParam, DWORD& subdata)");
#endif
}
	return NULL;
}

LPVOID CSelPathDlg::InsertSDFolder(LPCSTR Path, WIN32_FIND_DATA* pData,
							 LPVOID pfParam, DWORD& subdata)
{
try{
	HTREEITEM* pfItem = (HTREEITEM*)pfParam;
	HTREEITEM* pcurItem = new HTREEITEM;
	HTREEITEM curItem = pTreeCtrl->InsertItem(pData->cFileName, 4, 4, *pfItem);
	memcpy(pcurItem, &curItem, sizeof(HTREEITEM));
	pTreeCtrl->SetItemData(*pcurItem, Is_SDCard_Folder);
	subdata = 0;
	return (LPVOID)pcurItem;
}
catch(...){
#ifdef _DEBUG
	Tips("LPVOID CSelPathDlg::InsertFolder(LPCSTR Path, WIN32_FIND_DATA* pData, LPVOID pfParam, DWORD& subdata)");
#endif
}
	return NULL;
}

void CSelPathDlg::SetFolderStatics(LPCSTR Path, WIN32_FIND_DATA* pData,
						   LPVOID pParam, DWORD subdata, DWORD& data)
{
try{
	HTREEITEM* pcurItem = (HTREEITEM*)pParam;
	delete pcurItem;
}
catch(...){
#ifdef _DEBUG
	Tips("void CSelPathDlg::SetFolderStatics(LPCSTR Path, WIN32_FIND_DATA* pData, LPVOID pParam, DWORD subdata, DWORD& data)");
#endif
}
}

void CSelPathDlg::CountFile(LPCSTR Path, WIN32_FIND_DATA* pData, DWORD& data)
{
}

