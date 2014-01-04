// TreeCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "TreeCtrlEx.h"

#include "CreateRGDlg.h"
#include "UserInfoDlg.h"
#include "HttpDownLoadDlg.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char CurrentPath[_MAX_PATH];
extern char UserName[50];
extern char AspServer[1024];
extern char JspServer[1024];

extern CMainFrame* pMainWnd;
extern CUserCommDlg* pDlg;
/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx

CTreeCtrlEx::CTreeCtrlEx()
{
	rgid = -1;

	m_TimerTicks = 0;		//处理滚动的定时器所经过的时间
	m_nScrollTimerID = 0;	//处理滚动的定时器
	m_nHoverTimerID = 0;	//鼠标敏感定时器
	m_dwDragStart = 0;		//按下鼠标左键那一刻的时间
	m_bDragging = FALSE;	//标识是否正在拖动过程中
	m_pDragImage = NULL;	//拖动时显示的图象列表
	m_hItemDragS = NULL;	//被拖动的标签
	m_hItemDragD = NULL;	//接受拖动的标签
}

CTreeCtrlEx::~CTreeCtrlEx()
{
}

BEGIN_MESSAGE_MAP(CTreeCtrlEx, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlEx)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENUITEM_CREATERG, OnMenuitemCreaterg)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_MENUITEM_REFRESH, OnMenuitemRefresh)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_MENU_CREATE_REGROUP, OnMenuCreateRegroup)
	ON_COMMAND(ID_MENUITEM_RICKRESOURCE, OnMenuitemRickresource)
	ON_COMMAND(IDC_MENUITEM_ADDRICKER,OnAddRicker)
	ON_COMMAND(IDC_MENUITEM_DELETERICKER,OnDeleteRicker)
	ON_COMMAND(ID_MENUITEM_RICKERINFO, OnMenuitemRickerinfo)
	ON_COMMAND(ID_MENUITEM_TOMYSTUDY, OnMenuitemTomystudy)
	ON_COMMAND(ID_MENUITEM_ADDUNIVERSITY, OnMenuitemAdduniversity)
	ON_COMMAND(ID_MENUITEM_ADDHALL, OnMenuitemAddhall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx message handlers

void CTreeCtrlEx::SetTreeItemData(HTREEITEM hItem, BYTE type, char *ID, WORD Pages, DWORD Numbers)
{
try{
	TreeItemInfo* info = (TreeItemInfo*)malloc(sizeof(TreeItemInfo));
	memset(info, 0, sizeof(TreeItemInfo));
	info->Type = type;
	if(ID)
		strcpy_s(info->IndexID, 10, ID);
	if(Pages != 0){
		info->TotlePage = Pages;
		info->CurrentPage = 1;
	}
	info->BookNumber = Numbers;
	SetItemData(hItem, (DWORD)info);
}
catch(...){
#ifdef _DEBUG
	Tips("void CGuidTreeView::SetTreeItemData(HTREEITEM hItem, BYTE type, char *ID, WORD Pages, DWORD Numbers)");
#endif
}
}

void CTreeCtrlEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnRButtonDown(nFlags, point);
	HTREEITEM hItem = HitTest(point);
	if(!hItem)
		return;
	SelectItem(hItem);
	TreeItemInfo* info = (TreeItemInfo*)GetItemData(hItem);
	if(info->Type == Is_AllRickerGroupList
		|| info->Type == Is_RickerGroup_Node
		|| info->Type == Is_RickerGroup_Leaf)
	{
		rgid = (long)info->BookNumber;
		hCurItem = hItem;
		CMenu* pMenu;
		menu.LoadMenu(IDR_MENU_RIGHTTREE);
		pMenu = menu.GetSubMenu(0);
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		menu.DestroyMenu();
	}
	if (info->Type == Is_NetFirend)
	{
		rgid = (long)info->BookNumber;
		hCurItem = hItem;
		CMenu* pMenu;
		menu.LoadMenu(IDR_MENU_RIGHTTREE);
		pMenu = menu.GetSubMenu(1);
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		menu.DestroyMenu();
	}
	if (info->Type==Is_StudyHall)
	{
		CMenu* pMenu;
		menu.LoadMenu(IDR_MENU_RIGHTTREE);
		pMenu = menu.GetSubMenu(3);
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		menu.DestroyMenu();
	}
	if (info->Type == Is_School|| info->Type == Is_StudyBuilding || info->Type == Is_Hall)
	{
		rgid = (long)info->BookNumber;
		hCurItem = hItem;
		CMenu* pMenu;
		menu.LoadMenu(IDR_MENU_RIGHTTREE);
		pMenu = menu.GetSubMenu(2);
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
		menu.DestroyMenu();
	}
}

void CTreeCtrlEx::OnMenuitemCreaterg() 
{
	CCreateRGDlg cdlg;
	if(cdlg.DoModal()!=IDOK)
		return;

	CHttpDownLoadDlg hdlg;
	hdlg.m_URL.Format("%s/share/rickergroup/creatRickerGroup.asp?title=%s&fid=%d&username=%s",
		AspServer,cdlg.m_title,rgid,UserName);
	hdlg.m_Path.Format("%s\\temp\\createRg.res",CurrentPath);
	if(hdlg.DoModal()==IDOK){
		if(Check(hdlg.m_Path)){
			DeleteChildItems(hCurItem);								//先删除
			CString title = GetItemText(hCurItem);
			CString name = title;
			int index = title.ReverseFind('(');
			if(index>=0)
				name=title.Left(index);
			SetItemText(hCurItem,name);
			pMainWnd->m_pCommDlg->GetSubGroup(hCurItem);			//再获取
			CString str;
			str.Format("群落：%s 创建成功！", cdlg.m_title);
			Tips(str,td_msg,5);
			return;
		}
	}
	Tips("群落创建失败",td_alert,5);
}

void CTreeCtrlEx::DeleteChildItems(HTREEITEM hItem)
{
	if(!hItem)
		return;

	CList<HTREEITEM,HTREEITEM&> treeItemList;
	HTREEITEM hSubItem = GetChildItem(hItem);
	while(hSubItem){
		treeItemList.AddTail(hSubItem);
		hSubItem = GetNextSiblingItem(hSubItem);
	}
	POSITION pos = treeItemList.GetHeadPosition();
	while(pos){
		HTREEITEM hSubItem = treeItemList.GetNext(pos);
		DeleteItem(hSubItem);
	}
}

/************************
*	    拖动效果		*
*************************/
#define DRAG_DELAY 60

void CTreeCtrlEx::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	//如果是无意拖动，则放弃操作
	if((GetTickCount()-m_dwDragStart) < DRAG_DELAY )
		return;

	m_hItemDragS = pNMTreeView->itemNew.hItem;
	TreeItemInfo* info = (TreeItemInfo*)GetItemData(m_hItemDragS);
	if(info->Type!=Is_RickerGroup_Node
		&& info->Type!=Is_RickerGroup_Leaf)
	{
		return;
	}

	m_hItemDragD = NULL;

	//得到用于拖动时显示的图象列表
	m_pDragImage = CreateDragImage(m_hItemDragS);
	if(!m_pDragImage)
		return;

	m_bDragging = true;
	m_pDragImage->BeginDrag(0,CPoint(8,8));
	CPoint  pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	m_pDragImage->DragEnter(this,pt);  //"this"将拖动操作限制在该窗口
	SetCapture();

	m_nScrollTimerID = SetTimer(2,40,NULL);
}

void CTreeCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM  hItem;
	UINT       flags;

	//检测鼠标敏感定时器是否存在,如果存在则删除,删除后再定时
	if(m_nHoverTimerID){
		KillTimer(m_nHoverTimerID);
		m_nHoverTimerID = 0;
	}
	m_nHoverTimerID = SetTimer(1,800,NULL);  //定时为 0.8 秒则自动展开
	m_HoverPoint = point;

	if(m_bDragging){
		CPoint pt = point;
		CImageList::DragMove(pt);

		//鼠标经过时高亮显示
		CImageList::DragShowNolock(false);  //避免鼠标经过时留下难看的痕迹
		if((hItem = HitTest(point,&flags)) != NULL){
			TreeItemInfo* info = (TreeItemInfo*)GetItemData(hItem);
			if(info->Type==Is_AllRickerGroupList
				|| info->Type==Is_RickerGroup_Node
				|| info->Type==Is_RickerGroup_Leaf)
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				SelectDropTarget(hItem);
				m_hItemDragD = hItem;
			}
			else{
				SetCursor(LoadCursor(NULL, IDC_NO));
			}
		}
		CImageList::DragShowNolock(true);

		//当条目被拖曳到左边缘时，将条目放在根下
		CRect rect;
		GetClientRect(&rect);
		if(point.x<rect.left+20)
 			m_hItemDragD = NULL;
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CTreeCtrlEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonUp(nFlags, point);

	if(m_bDragging){
		m_bDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pDragImage;
		SelectDropTarget(NULL);
		
		TreeItemInfo* infoS = (TreeItemInfo*)GetItemData(m_hItemDragS);
		TreeItemInfo* infoD = (TreeItemInfo*)GetItemData(m_hItemDragD);
		if(m_hItemDragS == m_hItemDragD){
			KillTimer(m_nScrollTimerID);
			return;
		}
		else if(infoD->Type!=Is_AllRickerGroupList
			&& infoD->Type!=Is_RickerGroup_Node
			&& infoD->Type!=Is_RickerGroup_Leaf)
		{
			KillTimer(m_nScrollTimerID);
			return;
		}

		Expand( m_hItemDragD,TVE_EXPAND );
		HTREEITEM  htiParent = m_hItemDragD;

		//如果是由父节点拖向子节点
		while((htiParent = GetParentItem(htiParent)) != NULL){
			if(htiParent == m_hItemDragS){
				//建立一个临时节点以完成操作
				HTREEITEM  htiNewTemp = CopyBranch( m_hItemDragS,NULL,TVI_LAST );
				HTREEITEM  htiNew = CopyBranch( htiNewTemp,m_hItemDragD,TVI_LAST );
				DeleteItem( htiNewTemp );
				SelectItem( htiNew );
				KillTimer( m_nScrollTimerID );
				return;
			}
		}

		HTREEITEM  htiNew = CopyBranch(m_hItemDragS,m_hItemDragD,TVI_LAST);
		DeleteItem(m_hItemDragS);
		SelectItem(htiNew);
		KillTimer(m_nScrollTimerID);

		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format("%s/share/rickergroup/updateRickerGroup.asp?rgID=%d&fid=%d",
			AspServer, infoS->BookNumber, infoD->BookNumber);
		dlg.m_Path.Format("%s\\temp\\move.res",CurrentPath);
		if(dlg.DoModal()!=IDOK)
			return;
		if(Check(dlg.m_Path)){
			Tips("移动成功！",td_msg,5);
		}
	}
}

//拷贝条目
HTREEITEM CTreeCtrlEx::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT tvstruct;
	HTREEITEM hNewItem;
	CString sText;

	//得到源条目的信息
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	GetItem( &tvstruct.item );
	sText = GetItemText( hItem );
	tvstruct.item.cchTextMax = sText.GetLength();
	tvstruct.item.pszText = sText.LockBuffer();

	//将条目插入到合适的位置
	tvstruct.hParent = htiNewParent;
	tvstruct.hInsertAfter = htiAfter;
	tvstruct.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	hNewItem = InsertItem(&tvstruct);
	sText.ReleaseBuffer();

	//限制拷贝条目数据和条目状态
	SetItemData(hNewItem,GetItemData(hItem));
	SetItemState(hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return hNewItem;
}

//拷贝分支
HTREEITEM CTreeCtrlEx::CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	HTREEITEM hChild;
	HTREEITEM hNewItem = CopyItem(htiBranch,htiNewParent,htiAfter);
	hChild = GetChildItem(htiBranch);

	while(hChild != NULL){
		CopyBranch(hChild,hNewItem,htiAfter);
		hChild = GetNextSiblingItem(hChild);
	}
	return  hNewItem;
}

void CTreeCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//处理无意拖曳
	m_dwDragStart = GetTickCount();
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CTreeCtrlEx::OnTimer(UINT nIDEvent) 
{
	//鼠标敏感节点
	if(nIDEvent==m_nHoverTimerID){
		KillTimer(m_nHoverTimerID);
		m_nHoverTimerID = 0;
		HTREEITEM trItem = 0;
		UINT  uFlag = 0;
		trItem = HitTest(m_HoverPoint,&uFlag);
		if(trItem && m_bDragging){
			SelectItem(trItem);
			Expand(trItem,TVE_EXPAND);
		}
	}
	//处理拖曳过程中的滚动问题
	else if(nIDEvent == m_nScrollTimerID){
		m_TimerTicks++;
		CPoint pt;
		GetCursorPos(&pt);
		CRect rect;
		GetClientRect(&rect);
		ClientToScreen(&rect);

		HTREEITEM  hItem = GetFirstVisibleItem();
		
		if(pt.y<rect.top+10){
			//向上滚动
			int slowscroll = 6-(rect.top+10-pt.y)/20;
			if(0==(m_TimerTicks%((slowscroll>0)?slowscroll:1))){
				CImageList::DragShowNolock(false);
				SendMessage(WM_VSCROLL,SB_LINEUP);
				SelectDropTarget(hItem);
				m_hItemDragD = hItem;
				CImageList::DragShowNolock(true);
			}
		}
		else if(pt.y>rect.bottom-10){
			//向下滚动
			int slowscroll = 6-(pt.y-rect.bottom+10)/20;
			if(0==(m_TimerTicks%((slowscroll>0)?slowscroll:1))){
				CImageList::DragShowNolock(false);
				SendMessage(WM_VSCROLL,SB_LINEDOWN);
				int nCount = GetVisibleCount();
				for(int i=0 ; i<nCount-1 ; i++)
					hItem = GetNextVisibleItem(hItem);
				if(hItem)
					SelectDropTarget(hItem);
				m_hItemDragD = hItem;
				CImageList::DragShowNolock(true);
			}
		}
	}
	else
		CTreeCtrl::OnTimer(nIDEvent);
}

BOOL CTreeCtrlEx::Check(CString FilePath)
{
	BOOL res = FALSE;
	CFile cf;
	if(cf.Open(FilePath,CFile::modeRead)){
		if(cf.GetLength()>0){
			char c;
			cf.Read(&c, 1);
			if(c=='1')
				res = TRUE;
		}
		cf.Close();
	}
	DeleteFile(FilePath);
	return res;
}

void CTreeCtrlEx::OnMenuitemRefresh() 
{
	DeleteChildItems(hCurItem);
	CString title = GetItemText(hCurItem);
	CString name = title;
	int index = title.ReverseFind('(');
	if(index>=0)
		name=title.Left(index);
	SetItemText(hCurItem,name);
	pMainWnd->m_pCommDlg->GetSubGroup(hCurItem);
}

void CTreeCtrlEx::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	//modified by zhanganzhan on 20090410
	//NM_TREEVIEW* pNMTreeView;// = (NM_TREEVIEW*)pNMHDR;
	// anzhsoftpro 20100418 变量未初始化的错误？
	pMainWnd->PostMessage(WM_USER_TREE_CLICKED);

// 	HTREEITEM hItem = GetSelectedItem();
// 	if(!hItem)
// 		return;
// 
// 	TreeItemInfo* info = (TreeItemInfo*)GetItemData(hItem);
// 	if(info->Type!=Is_NetFirend)
// 		return;
// 
// 	LPSTR UN = (LPSTR)calloc(100,1);
// 	strcpy(UN,GetItemText(hItem));
// 	GetParent()->PostMessage(WM_CREATE_TALK_DLG,(WPARAM)UN);
	//anzhsoft0409

	/*
	HWND handle = GetProcWnd();
	if(handle){
		AfxMessageBox("OK");
		::PostMessage(handle, WM_CLOSE, NULL, NULL);
	}
	*/
}


void CTreeCtrlEx::OnMenuCreateRegroup() 
{
	// TODO: Add your command handler code here
	
}

void CTreeCtrlEx::OnMenuitemRickresource() 
{
	// Modified by zhanganzhan on 20090410 for right click.
	// TODO: Add your command handler code here
	NM_TREEVIEW* pNMTreeView;// = (NM_TREEVIEW*)pNMHDR;
	pMainWnd->PostMessage(WM_USER_TREE_CLICKED, (WPARAM)pNMTreeView);

}
//added by zhanganzhan on 20090421 for my friends.090421
void CTreeCtrlEx::OnAddRicker()
{
	TreeItemInfo* info = (TreeItemInfo*)GetItemData(hCurItem);
	STRUCTUSERINFO *uinfo = (STRUCTUSERINFO*)info->BookNumber;
	CString url;
	if(strcmp(UserName,uinfo->UserName)==0)
	{
		Tips("加你自己为好友？",td_msg,5);
		return;
	}
	url.Format("%s/msg/addfriend.asp?username=%s&friendname=%s",AspServer,UserName,uinfo->UserName);
	UpLoad(url);

}

//added by zhanganzhan on 20090621 for my friends.090621
void CTreeCtrlEx::OnDeleteRicker()
{
	TreeItemInfo* info = (TreeItemInfo*)GetItemData(hCurItem);
	STRUCTUSERINFO *uinfo = (STRUCTUSERINFO*)info->BookNumber;
	CString url;
	if(strcmp(UserName,uinfo->UserName)==0)
	{
		Tips("删除你自己？",td_msg,5);
		return;
	}
	url.Format("%s/msg/delfriend.asp?username=%s&friendname=%s",AspServer,UserName,uinfo->UserName);
	UpLoad(url);
	
}


void CTreeCtrlEx::OnMenuitemRickerinfo() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)GetItemData(hCurItem);
	STRUCTUSERINFO *uinfo = (STRUCTUSERINFO*)info->BookNumber;
//	AfxMessageBox(uinfo->UserName);
	CUserInfoDlg userinfoDlg(*uinfo);
	userinfoDlg.DoModal();
}

void CTreeCtrlEx::OnMenuitemTomystudy() 
{
	// TODO: Add your command handler code here
	TreeItemInfo* info = (TreeItemInfo*)GetItemData(hCurItem);
	CString hit_name = GetItemText(hCurItem);

	int hflag = 0 ;//标示哪种类型的房间转到我的自习室
	if(info->Type == Is_School)
		hflag = 1;
	else if(info->Type == Is_StudyBuilding)
		hflag = 2;
	else if(info->Type == Is_Hall)
		hflag = 3;
	else{
		Tips("转到我的自习室没有成功,td_alert,5");
		return;
	}
		//根据	ginfo->gid 和 UserName 更改 服务器对应表，更新或插入
		try
		{
		    CHttpDownLoadDlg dlg;
	    	dlg.m_URL.Format( "%s/share/studyhall/insertmystudy.asp?username=%s&id=%d&hflag=%d", AspServer,UserName,info->BookNumber,hflag);
		    dlg.m_Path.Format("%s\\temp\\InsertMyStudy.txt", CurrentPath);
		    if(dlg.DoModal()!=IDOK){
			    Tips("转到我的自习室没有成功",td_alert,5);
			    return;
			}
			//查看是否插入成功
				CFile cf;
				if(cf.Open(dlg.m_Path,CFile::modeRead)){
					char cc[2];
					cf.Read(cc,2);
					if('0'==cc[0]){
						Tips("该房间已经列入您的自习室",td_alert,5);
							return;
					}
					cf.Close();
				}
	
		}
		catch(_com_error& e){
			Tips("转到我的自习室没有成功",td_alert,5);
			return;
		}
		
		Tips("转到我的自习室",td_msg,5);
		pDlg->PostMessage(WM_ADD_TO_MYHALL,(WPARAM)info,(LPARAM)hCurItem);
	

// 	if (info->Type == Is_NetGroup_Level)
// 	{
// 		try
// 		{
// 		    CHttpDownLoadDlg dlg;
// 	    	dlg.m_URL.Format( "%s/share/insertfavoritelevel.asp?username=%s&glid=%d", AspServer,UserName,info->BookNumber);
// 		    dlg.m_Path.Format("%s\\temp\\InsertU_L.asp", CurrentPath);
// 		    if(dlg.DoModal()!=IDOK){
// 			    Tips("操作失败，请重新尝试！",td_alert,5);
// 			    return;
// 			}
// 		}
// 		catch(_com_error& e){
// 			AfxMessageBox("操作失败，请重新尝试！");
// 		}
// 		Tips("类别设定成功！",td_msg,5);
// 		HTREEITEM hCurItem = pTree->InsertItem(hit_name.Left(hit_name.Find('(')), 2, 2, hMyGroup);
// 	//	pTreeView->SetTreeItemData(hCurItem, Is_NetGroup, NULL, 0, (DWORD)ginfo);
// 		CString num1 = hit_name.Right(hit_name.GetLength()-hit_name.Find('(')-1);
// 		num1 = num1.Left(num1.GetLength()-1);
// 
// 		pTreeView->SetTreeItemData(hCurItem, Is_NetGroup_Level, NULL, atoi((LPCSTR)num1), info->BookNumber,info->KeyWords);
// 		GetSubGroup(hCurItem);
//	}
}

void CTreeCtrlEx::OnMenuitemAdduniversity() 
{
	// TODO: Add your command handler code here
	AddUniversity addUdlg;
	if(addUdlg.DoModal()!=IDOK){
		Tips("添加大学未成功");
		return;
	}
	if (addUdlg.m_name==""){
		Tips("没有添加大学名称");
		return;	
	}
//studyhall/insertuniversity.asp?name=清华大学&desc=test&username=start_dzh
	CString uname = addUdlg.m_name;
	CString udesc = addUdlg.m_desc;
	CHttpDownLoadDlg dlg;
	dlg.m_URL.Format( "%s/share/studyhall/insertuniversity.asp?name=%s&desc=%s&username=%s", AspServer,uname,udesc,UserName);
	dlg.m_Path.Format("%s\\temp\\insertuniversity.txt", CurrentPath);
	if(dlg.DoModal()!=IDOK){
		Tips("添加大学失败",td_alert,5);
		return ;
	}

		//刷新右侧侧树，插入新节点，将id赋值给新大学节点	
			CFile cf;
			int length;
			int num;
			char * numbuf;
			if(cf.Open(dlg.m_Path,CFile::modeRead)){
				length = cf.GetLength();
				if (length<=0){
					cf.Close();
					return;
				}
				numbuf = (char*)malloc(length+1);
				memset(numbuf,0,length+1);
				cf.Read(numbuf,length);
				num = atoi(numbuf);//存放 大学的id
				cf.Close();
			}
		if (0==num)
		{
			Tips("添加未成功");
			return;
		}
	
		//插入新节点
		HTREEITEM hCurItem1 = pDlg->m_tree.InsertItem(uname, 29,29 , pDlg->hStudyhall);
		pDlg->m_tree.SetTreeItemData(hCurItem1, Is_School, NULL, 0, num);
		Tips("添加大学成功");
	//	AfxMessageBox(uname);

}

void CTreeCtrlEx::OnMenuitemAddhall() 
{
	// TODO: Add your command handler code here
	CDlgAddStudyBuilding addDlg;
	if (addDlg.DoModal()!=IDOK)
	{
		Tips("添加未成功!");
		return;
	}
    TreeItemInfo* info = (TreeItemInfo*)GetItemData(hCurItem);
	CString hit_name = GetItemText(hCurItem);
	
	CString uname = addDlg.m_name;
	CString udesc = addDlg.m_desc;
	CHttpDownLoadDlg dlg;
	if(info->Type==Is_School)
		dlg.m_URL.Format( "%s/share/studyhall/insertbuilding.asp?name=%s&desc=%s&username=%s&schoolid=%d", AspServer,uname,udesc,UserName,info->BookNumber);
	if(info->Type==Is_StudyBuilding)
		dlg.m_URL.Format( "%s/share/studyhall/insertHall.asp?name=%s&desc=%s&username=%s&buildingid=%d", AspServer,uname,udesc,UserName,info->BookNumber);

	dlg.m_Path.Format("%s\\temp\\inserthall.txt", CurrentPath);
	if(dlg.DoModal()!=IDOK){
		Tips("添加失败",td_alert,5);
		return ;
	}

		//刷新右侧侧树，插入新节点，将id赋值给新大学节点	
			CFile cf;
			int length;
			int num;
			char * numbuf;
			if(cf.Open(dlg.m_Path,CFile::modeRead)){
				length = cf.GetLength();
				if (length<=0){
					cf.Close();
					return;
				}
				numbuf = (char*)malloc(length+1);
				memset(numbuf,0,length+1);
				cf.Read(numbuf,length);
				num = atoi(numbuf);//存放 大学的id
				cf.Close();
			}
		//插入新节点
			if (0==num)
			{
				Tips("添加未成功",td_alert,5);
				return;
			}
			if(info->Type==Is_School){
				HTREEITEM hCurItem1 = pDlg->m_tree.InsertItem(uname,30,30,hCurItem);
				pDlg->m_tree.SetTreeItemData(hCurItem1, Is_StudyBuilding, NULL, 0, num);
			}
			if (info->Type==Is_StudyBuilding)
			{				
				HTREEITEM hCurItem1 = pDlg->m_tree.InsertItem(uname,31,31,hCurItem);
				pDlg->m_tree.SetTreeItemData(hCurItem1, Is_Hall, NULL, 0, num);
			}
			Tips("添加成功");
		
}
