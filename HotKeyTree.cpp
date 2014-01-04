// HotKeyTree.cpp: implementation of the CHotKeyTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "HotKeyTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern CHotKeyTree* m_HKView;
extern CTreeCtrl* m_pHKTree;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHotKeyTree::CHotKeyTree()
{
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
	m_pHKTree = m_pTree;
	m_HKView = this;
}

CHotKeyTree::~CHotKeyTree()
{

}

BOOL CHotKeyTree::RefreshTree()
{
	InsertHotKey();
	//InsertFanShuHotBook();
	Invalidate(FALSE);
	return TRUE;
}
