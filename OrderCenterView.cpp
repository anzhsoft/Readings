// OrderCenterView.cpp: implementation of the COrderCenterView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "OrderCenterView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern BOOL offline;
extern COrderCenterView* m_OCView;
extern CTreeCtrl* m_pOCTree;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrderCenterView::COrderCenterView()
{
	type = ordercertern;
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
	m_pOCTree = m_pTree;
	m_OCView = this;
}

COrderCenterView::~COrderCenterView()
{

}

BOOL COrderCenterView::RefreshTree()
{
	if(!offline)
	{
#ifdef SWITCH_ORDER
	InsertOrderCenter(NULL);//¶©ÔÄÖÐÐÄ
	GetOrderInfo();
#endif
	}
	Invalidate(FALSE);
	return TRUE;
}
