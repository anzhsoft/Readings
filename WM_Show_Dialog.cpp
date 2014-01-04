// WM_Show_Dialog.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "WM_Show_Dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WM_Show_Dialog dialog


WM_Show_Dialog::WM_Show_Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(WM_Show_Dialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(WM_Show_Dialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void WM_Show_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(WM_Show_Dialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(WM_Show_Dialog, CDialog)
	//{{AFX_MSG_MAP(WM_Show_Dialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WM_Show_Dialog message handlers
