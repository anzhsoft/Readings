// ReadOnLine.cpp: implementation of the CReadOnLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "ReadOnLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern CReadOnLine* m_RLView;
extern CTreeCtrl* m_pRLTree;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReadOnLine::CReadOnLine()
{
	type = readonline;
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
	m_pRLTree = m_pTree;
	m_RLView = this;
}

CReadOnLine::~CReadOnLine()
{

}

BOOL CReadOnLine::RefreshTree()
{
// 	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetNetInfo,(LPVOID)this,CREATE_SUSPENDED,NULL);
// 	if(handle)
// 	{
// 		threadList.AddTail(handle);
// 		ResumeThread(handle);
// 	}
 	InsertMyResourceGroup();//我的读物群落
 	InsertResourceGroup();//高教课程读物部落
// 	InsertNetBook();//网络原创读物
// 	InsertPublishCom();//读书联盟
	Invalidate(FALSE);
	return TRUE;
}
