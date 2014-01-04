// ManBooks.cpp: implementation of the CManBooks class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "ManBooks.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern CManBooks* m_MBView;
extern CTreeCtrl* m_pMBTree;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CManBooks::CManBooks()
{
	type = manbooks;
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
	m_pMBTree = m_pTree;
	m_MBView = this;
}

CManBooks::~CManBooks()
{

}

BOOL CManBooks::RefreshTree()
{
	InsertLocalFolder(NULL);
	InsertRemoveableDisk();
	Invalidate(FALSE);
	return TRUE;
}
