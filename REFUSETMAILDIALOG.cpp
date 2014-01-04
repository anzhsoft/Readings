// REFUSETMAILDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "REFUSETMAILDIALOG.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern char CurrentPath[_MAX_PATH];
extern char UserName[50];
extern CMainFrame* pMainWnd;
extern string RequestMail;
extern string AgreeMail;
extern string RefuseMail;
extern string ChargingMail;
extern char AspServer[1024];
/////////////////////////////////////////////////////////////////////////////
// CREFUSETMAILDIALOG dialog


CREFUSETMAILDIALOG::CREFUSETMAILDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CREFUSETMAILDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CREFUSETMAILDIALOG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CREFUSETMAILDIALOG::CREFUSETMAILDIALOG(CRefuseMail mail, CRECVMAILDIALOG* dlg, CWnd* pParent /*=NULL*/)
: CDialog(CREFUSETMAILDIALOG::IDD, pParent)
{
	refusemail = mail;
	recvmaildlg = dlg;
}


void CREFUSETMAILDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CREFUSETMAILDIALOG)
	DDX_Control(pDX, IDC_fileSTATIC, m_filename);
	DDX_Control(pDX, IDC_Reason, m_Reason);
	DDX_Control(pDX, IDC_Refuser, m_Refuser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CREFUSETMAILDIALOG, CDialog)
	//{{AFX_MSG_MAP(CREFUSETMAILDIALOG)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CREFUSETMAILDIALOG message handlers

void CREFUSETMAILDIALOG::OnDestroy() 
{
	CDialog::OnDestroy();
	delete this;
	// TODO: Add your message handler code here
	
}

BOOL CREFUSETMAILDIALOG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Refuser.SetWindowText(refusemail.Replyer.c_str());
	m_filename.SetWindowText(refusemail.filename.c_str());
	m_Reason.SetWindowText(refusemail.Reason.c_str());
// 	CEMailADO::InsertMail(AspServer,refusemail.MailID.c_str(),UserName,"RefuseMail",refusemail.Replyer.c_str());
// 	POSITION pos = pMainWnd->UnReadedRefuseMailList->GetHeadPosition();
// 	while(pos != NULL)
// 	{
// 		POSITION pos1 = pos;
// 		CRefuseMail tempmail = pMainWnd->UnReadedRefuseMailList->GetNext(pos);
// 		if (tempmail.MailID.compare(refusemail.MailID) == 0)
// 		{
// 			pMainWnd->UnReadedRefuseMailList->RemoveAt(pos1);
// 			break;
// 		}
// 	}
// 	if (recvmaildlg != NULL)
// 	{
// 		
// 		recvmaildlg->m_RefuseMailList.ResetContent();
// 		pos = pMainWnd->UnReadedRefuseMailList->GetHeadPosition();
// 		while(pos != NULL)
// 		{
// 			CRefuseMail temmail = pMainWnd->UnReadedRefuseMailList->GetNext(pos);
// 			recvmaildlg->m_RefuseMailList.AddString((temmail.Replyer + "¾Ü¾øÁËÄúµÄÇëÇó").c_str());	
// 		}
// 	}
// 	pMainWnd->ReadedRefuseMailList->AddTail(refusemail);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

unsigned __stdcall CREFUSETMAILDIALOG::_deal(LPVOID pParam)
{
	return TRUE;
}
