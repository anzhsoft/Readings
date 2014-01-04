// ErrorExitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ErrorExitDlg.h"


// CErrorExitDlg dialog

IMPLEMENT_DYNAMIC(CErrorExitDlg, CDialog)

CErrorExitDlg::CErrorExitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorExitDlg::IDD, pParent)
{

}

CErrorExitDlg::~CErrorExitDlg()
{
}

void CErrorExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CErrorExitDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CErrorExitDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CErrorExitDlg message handlers

void CErrorExitDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	OnOK();
}
