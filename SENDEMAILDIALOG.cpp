// SENDEMAILDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "SENDEMAILDIALOG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSENDEMAILDIALOG dialog


CSENDEMAILDIALOG::CSENDEMAILDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CSENDEMAILDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSENDEMAILDIALOG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSENDEMAILDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSENDEMAILDIALOG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSENDEMAILDIALOG, CDialog)
	//{{AFX_MSG_MAP(CSENDEMAILDIALOG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSENDEMAILDIALOG message handlers

void CSENDEMAILDIALOG::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
