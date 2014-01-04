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

	m_TimerTicks = 0;		//��������Ķ�ʱ����������ʱ��
	m_nScrollTimerID = 0;	//��������Ķ�ʱ��
	m_nHoverTimerID = 0;	//������ж�ʱ��
	m_dwDragStart = 0;		//������������һ�̵�ʱ��
	m_bDragging = FALSE;	//��ʶ�Ƿ������϶�������
	m_pDragImage = NULL;	//�϶�ʱ��ʾ��ͼ���б�
	m_hItemDragS = NULL;	//���϶��ı�ǩ
	m_hItemDragD = NULL;	//�����϶��ı�ǩ
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
			DeleteChildItems(hCurItem);								//��ɾ��
			CString title = GetItemText(hCurItem);
			CString name = title;
			int index = title.ReverseFind('(');
			if(index>=0)
				name=title.Left(index);
			SetItemText(hCurItem,name);
			pMainWnd->m_pCommDlg->GetSubGroup(hCurItem);			//�ٻ�ȡ
			CString str;
			str.Format("Ⱥ�䣺%s �����ɹ���", cdlg.m_title);
			Tips(str,td_msg,5);
			return;
		}
	}
	Tips("Ⱥ�䴴��ʧ��",td_alert,5);
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
*	    �϶�Ч��		*
*************************/
#define DRAG_DELAY 60

void CTreeCtrlEx::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	//����������϶������������
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

	//�õ������϶�ʱ��ʾ��ͼ���б�
	m_pDragImage = CreateDragImage(m_hItemDragS);
	if(!m_pDragImage)
		return;

	m_bDragging = true;
	m_pDragImage->BeginDrag(0,CPoint(8,8));
	CPoint  pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	m_pDragImage->DragEnter(this,pt);  //"this"���϶����������ڸô���
	SetCapture();

	m_nScrollTimerID = SetTimer(2,40,NULL);
}

void CTreeCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM  hItem;
	UINT       flags;

	//���������ж�ʱ���Ƿ����,���������ɾ��,ɾ�����ٶ�ʱ
	if(m_nHoverTimerID){
		KillTimer(m_nHoverTimerID);
		m_nHoverTimerID = 0;
	}
	m_nHoverTimerID = SetTimer(1,800,NULL);  //��ʱΪ 0.8 �����Զ�չ��
	m_HoverPoint = point;

	if(m_bDragging){
		CPoint pt = point;
		CImageList::DragMove(pt);

		//��꾭��ʱ������ʾ
		CImageList::DragShowNolock(false);  //������꾭��ʱ�����ѿ��ĺۼ�
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

		//����Ŀ����ҷ�����Եʱ������Ŀ���ڸ���
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

		//������ɸ��ڵ������ӽڵ�
		while((htiParent = GetParentItem(htiParent)) != NULL){
			if(htiParent == m_hItemDragS){
				//����һ����ʱ�ڵ�����ɲ���
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
			Tips("�ƶ��ɹ���",td_msg,5);
		}
	}
}

//������Ŀ
HTREEITEM CTreeCtrlEx::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT tvstruct;
	HTREEITEM hNewItem;
	CString sText;

	//�õ�Դ��Ŀ����Ϣ
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	GetItem( &tvstruct.item );
	sText = GetItemText( hItem );
	tvstruct.item.cchTextMax = sText.GetLength();
	tvstruct.item.pszText = sText.LockBuffer();

	//����Ŀ���뵽���ʵ�λ��
	tvstruct.hParent = htiNewParent;
	tvstruct.hInsertAfter = htiAfter;
	tvstruct.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	hNewItem = InsertItem(&tvstruct);
	sText.ReleaseBuffer();

	//���ƿ�����Ŀ���ݺ���Ŀ״̬
	SetItemData(hNewItem,GetItemData(hItem));
	SetItemState(hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return hNewItem;
}

//������֧
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
	//����������ҷ
	m_dwDragStart = GetTickCount();
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CTreeCtrlEx::OnTimer(UINT nIDEvent) 
{
	//������нڵ�
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
	//������ҷ�����еĹ�������
	else if(nIDEvent == m_nScrollTimerID){
		m_TimerTicks++;
		CPoint pt;
		GetCursorPos(&pt);
		CRect rect;
		GetClientRect(&rect);
		ClientToScreen(&rect);

		HTREEITEM  hItem = GetFirstVisibleItem();
		
		if(pt.y<rect.top+10){
			//���Ϲ���
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
			//���¹���
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
	// anzhsoftpro 20100418 ����δ��ʼ���Ĵ���
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
		Tips("�����Լ�Ϊ���ѣ�",td_msg,5);
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
		Tips("ɾ�����Լ���",td_msg,5);
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

	int hflag = 0 ;//��ʾ�������͵ķ���ת���ҵ���ϰ��
	if(info->Type == Is_School)
		hflag = 1;
	else if(info->Type == Is_StudyBuilding)
		hflag = 2;
	else if(info->Type == Is_Hall)
		hflag = 3;
	else{
		Tips("ת���ҵ���ϰ��û�гɹ�,td_alert,5");
		return;
	}
		//����	ginfo->gid �� UserName ���� ��������Ӧ�����»����
		try
		{
		    CHttpDownLoadDlg dlg;
	    	dlg.m_URL.Format( "%s/share/studyhall/insertmystudy.asp?username=%s&id=%d&hflag=%d", AspServer,UserName,info->BookNumber,hflag);
		    dlg.m_Path.Format("%s\\temp\\InsertMyStudy.txt", CurrentPath);
		    if(dlg.DoModal()!=IDOK){
			    Tips("ת���ҵ���ϰ��û�гɹ�",td_alert,5);
			    return;
			}
			//�鿴�Ƿ����ɹ�
				CFile cf;
				if(cf.Open(dlg.m_Path,CFile::modeRead)){
					char cc[2];
					cf.Read(cc,2);
					if('0'==cc[0]){
						Tips("�÷����Ѿ�����������ϰ��",td_alert,5);
							return;
					}
					cf.Close();
				}
	
		}
		catch(_com_error& e){
			Tips("ת���ҵ���ϰ��û�гɹ�",td_alert,5);
			return;
		}
		
		Tips("ת���ҵ���ϰ��",td_msg,5);
		pDlg->PostMessage(WM_ADD_TO_MYHALL,(WPARAM)info,(LPARAM)hCurItem);
	

// 	if (info->Type == Is_NetGroup_Level)
// 	{
// 		try
// 		{
// 		    CHttpDownLoadDlg dlg;
// 	    	dlg.m_URL.Format( "%s/share/insertfavoritelevel.asp?username=%s&glid=%d", AspServer,UserName,info->BookNumber);
// 		    dlg.m_Path.Format("%s\\temp\\InsertU_L.asp", CurrentPath);
// 		    if(dlg.DoModal()!=IDOK){
// 			    Tips("����ʧ�ܣ������³��ԣ�",td_alert,5);
// 			    return;
// 			}
// 		}
// 		catch(_com_error& e){
// 			AfxMessageBox("����ʧ�ܣ������³��ԣ�");
// 		}
// 		Tips("����趨�ɹ���",td_msg,5);
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
		Tips("��Ӵ�ѧδ�ɹ�");
		return;
	}
	if (addUdlg.m_name==""){
		Tips("û����Ӵ�ѧ����");
		return;	
	}
//studyhall/insertuniversity.asp?name=�廪��ѧ&desc=test&username=start_dzh
	CString uname = addUdlg.m_name;
	CString udesc = addUdlg.m_desc;
	CHttpDownLoadDlg dlg;
	dlg.m_URL.Format( "%s/share/studyhall/insertuniversity.asp?name=%s&desc=%s&username=%s", AspServer,uname,udesc,UserName);
	dlg.m_Path.Format("%s\\temp\\insertuniversity.txt", CurrentPath);
	if(dlg.DoModal()!=IDOK){
		Tips("��Ӵ�ѧʧ��",td_alert,5);
		return ;
	}

		//ˢ���Ҳ�����������½ڵ㣬��id��ֵ���´�ѧ�ڵ�	
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
				num = atoi(numbuf);//��� ��ѧ��id
				cf.Close();
			}
		if (0==num)
		{
			Tips("���δ�ɹ�");
			return;
		}
	
		//�����½ڵ�
		HTREEITEM hCurItem1 = pDlg->m_tree.InsertItem(uname, 29,29 , pDlg->hStudyhall);
		pDlg->m_tree.SetTreeItemData(hCurItem1, Is_School, NULL, 0, num);
		Tips("��Ӵ�ѧ�ɹ�");
	//	AfxMessageBox(uname);

}

void CTreeCtrlEx::OnMenuitemAddhall() 
{
	// TODO: Add your command handler code here
	CDlgAddStudyBuilding addDlg;
	if (addDlg.DoModal()!=IDOK)
	{
		Tips("���δ�ɹ�!");
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
		Tips("���ʧ��",td_alert,5);
		return ;
	}

		//ˢ���Ҳ�����������½ڵ㣬��id��ֵ���´�ѧ�ڵ�	
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
				num = atoi(numbuf);//��� ��ѧ��id
				cf.Close();
			}
		//�����½ڵ�
			if (0==num)
			{
				Tips("���δ�ɹ�",td_alert,5);
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
			Tips("��ӳɹ�");
		
}
