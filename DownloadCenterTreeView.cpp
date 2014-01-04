// DownloadCenterTreeView.cpp: implementation of the CDownloadCenterTreeView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "DownloadCenterTreeView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern BOOL offline;
extern CDownloadCenterTreeView* m_DCView;
extern CTreeCtrl* m_pDCTree;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloadCenterTreeView::CDownloadCenterTreeView()
{
	type = downloadcertern;
	quit = FALSE;
	hit_Item = NULL;
	m_MyBookcaseItem = NULL;
	hNetLibrary = NULL;
	hpaperItem = NULL;
	hmagItem = NULL;
	hNetResource = NULL;
	hOrderItem = NULL;
	collectItemRead = NULL;
	collectItemPic = NULL;
	collectItemMeida = NULL;
	hFriend = NULL;
	hSDDisk = NULL;
	m_pTree = &GetTreeCtrl();
	m_pDCTree = m_pTree;
	m_DCView = this;
}

CDownloadCenterTreeView::~CDownloadCenterTreeView()
{
	
}

BOOL CDownloadCenterTreeView::RefreshTree()
{
	InsertPublishCom();//读书联盟
 	InsertNetBook();//网络原创读物
	InsertFanshuBook();
	if(!offline)
	{
		InsertResourceShare(NULL);
#ifdef SWITCH_NETRESOURCE
		InsertDownloadCenter(NULL);
		GetNetLibraryInfo();
#endif
		InsertRStore();
 	}
 	Invalidate(FALSE);
 	return TRUE;
}
