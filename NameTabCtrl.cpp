// NameTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "NameTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNameTabCtrl

CNameTabCtrl::CNameTabCtrl()
{
}

CNameTabCtrl::~CNameTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CNameTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CNameTabCtrl)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNameTabCtrl message handlers

BOOL CNameTabCtrl::InsertItem(LPCTSTR lpszItem)
{
	int count = GetItemCount();
	TCITEM* tItem = (TCITEM*)malloc(sizeof(TCITEM));
	ZeroMemory(tItem, sizeof(TCITEM));
	tItem->pszText = (LPSTR)malloc(strlen(lpszItem)+1);
	ZeroMemory(tItem->pszText, strlen(lpszItem)+1);
	strcpy_s(tItem->pszText, strlen(lpszItem)+1, lpszItem);
	tItem->mask = TCIF_TEXT;
	return CTabCtrl::InsertItem(count, tItem);
}


int CNameTabCtrl::Find(LPCSTR name)
{
	for(int index=0; index<GetItemCount(); index++){
		TCITEM tItem = {0};
		char nm[50] = {0};
		tItem.mask = TCIF_TEXT;
		tItem.pszText = nm;
		tItem.cchTextMax = 50;
		GetItem(index, &tItem);
		if(strcmp(name, tItem.pszText)==0)
			return index;
	}
	return -1;
}

CString CNameTabCtrl::GetItemText(int nItem)
{
	TCITEM tItem = {0};
	char nm[50] = {0};
	tItem.mask = TCIF_TEXT;
	tItem.pszText = nm;
	tItem.cchTextMax = 50;
	GetItem(nItem, &tItem);
	if(tItem.pszText)
		return tItem.pszText;
	else
		return "";
}

void CNameTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTabCtrl::OnMouseMove(nFlags, point);
}
