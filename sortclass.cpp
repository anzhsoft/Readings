// SortClass.cpp : implementation file
//

#include "stdafx.h"
#include "SortClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortClass

CSortClass::CSortClass(CListCtrl * _pWnd, const int _iCol)
{
	iCol = _iCol;
	pWnd = _pWnd;

	if (!pWnd) return;
	int max = pWnd->GetItemCount();
	DWORD dw;
	CString txt;
	
	for (int t = 0; t<max;t++)
	{
		dw=pWnd->GetItemData(t);
		txt = pWnd->GetItemText(t, iCol);
		pWnd->SetItemData(t, (DWORD) new CSortItem(dw, txt, pWnd->GetCheck(t)));
	}
}

CSortClass::~CSortClass()
{
	int max = pWnd->GetItemCount();
	CSortItem * pItem;

	for (int t = 0; t<max;t++)
	{
		pItem =(CSortItem*)pWnd->GetItemData(t);
		pWnd->SetItemData(t, pItem->dw);
		pWnd->SetCheck(t, pItem->check);
		//****************************************
		//为上一页和下一页
		if(pItem->dw == 1){
			char txt[100] = {0};
			pWnd->GetItemText(t, 0, txt, 100);
			pWnd->DeleteItem(t);			
			pWnd->InsertItem(0, txt, 7);
			pWnd->SetItemData(0, 1);
		}
		else if(pItem->dw == 2){
			char txt[100] = {0};
			pWnd->GetItemText(t, 0, txt, 100);

			pWnd->InsertItem(max, txt, 6);
			pWnd->SetItemData(max, 2);

			pWnd->DeleteItem(t);
			t--;
			max--;
		}
		//****************************************
		delete pItem;
	}	
}

//*********************************************************
//_iType表示当前排序列的数据类型
//   1：字符串
//   2：INT
//   3：LONG
//	 4：FLOAT
//*********************************************************
void CSortClass::Sort(const bool bAsc,int _iType)// 1 char;2 int;3 long;4 float
{
	long lParamSort = _iType;
	if (!bAsc)
		lParamSort *= -1;
	pWnd->SortItems(Compare, lParamSort);
}

int CALLBACK CSortClass::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	short sOrder = lParamSort >0 ? 1 : -1;
	int iType = (int)(lParamSort * sOrder);
	
	CSortItem * i1 = (CSortItem *) lParam1;
	CSortItem * i2 = (CSortItem *) lParam2;
	ASSERT(i1 && i2);
	switch (iType)
	{
	case 1:
		return (i1->txt.CompareNoCase(i2->txt))*sOrder;
	case 2:
		return (atoi(i1->txt)-atoi(i2->txt))*sOrder;
	case 3:
		return (atol(i1->txt)-atol(i2->txt))*sOrder;
	case 4:
		return ((atof(i1->txt)-atof(i2->txt))>0?1:-1)*sOrder;
	default:
		return 0;
		break;
	}
}

CSortClass::CSortItem::CSortItem(const DWORD _dw, const CString & _txt, const BOOL _check)
{
	dw = _dw;
	txt = _txt;
	check = _check;
}

CSortClass::CSortItem::~CSortItem()
{

}
