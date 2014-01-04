// DataListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "DataListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataListCtrl

CDataListCtrl::CDataListCtrl()
{
}

CDataListCtrl::~CDataListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDataListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CDataListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataListCtrl message handlers

BOOL CDataListCtrl::DeleteAllItems()
{
	for(int nIndex=0; nIndex<GetItemCount(); nIndex++){
		LPVOID data = (LPVOID)GetItemData(nIndex);
		if(!data)
			free(data);
	}

	return CListCtrl::DeleteAllItems();
}
