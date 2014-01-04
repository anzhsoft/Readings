// HotkeyNew.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "HotkeyNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHotkeyNew dialog


CHotkeyNew::CHotkeyNew(CWnd* pParent /*=NULL*/)
	: CDialog(CHotkeyNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHotkeyNew)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHotkeyNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHotkeyNew)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHotkeyNew, CDialog)
	//{{AFX_MSG_MAP(CHotkeyNew)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHotkeyNew message handlers
