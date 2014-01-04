// CHARGETMAILDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "CHARGETMAILDIALOG.h"
#include "MainFrm.h"
#include "EMailADO.h"
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
// CCHARGETMAILDIALOG dialog


CCHARGETMAILDIALOG::CCHARGETMAILDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CCHARGETMAILDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCHARGETMAILDIALOG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCHARGETMAILDIALOG::CCHARGETMAILDIALOG(CChargeMail mail,  CRECVMAILDIALOG* dlg, CWnd* pParent /*=NULL*/)
: CDialog(CCHARGETMAILDIALOG::IDD, pParent)
{
	chargemail = mail;
	recvmaildlg = dlg;
}


void CCHARGETMAILDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCHARGETMAILDIALOG)
	DDX_Control(pDX, IDC_LIST1, m_attlist);
	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Control(pDX, IDC_FileList, m_filelist);
	DDX_Control(pDX, IDC_Sender, m_sender);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCHARGETMAILDIALOG, CDialog)
	//{{AFX_MSG_MAP(CCHARGETMAILDIALOG)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCHARGETMAILDIALOG message handlers

void CCHARGETMAILDIALOG::OnDestroy() 
{
	CDialog::OnDestroy();
	delete this;
	// TODO: Add your message handler code here
	
}

void CCHARGETMAILDIALOG::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CButton*  pWndButton = (CButton*)GetDlgItem(IDC_BUTTON1);   
    pWndButton->EnableWindow(false);   
	pWndButton = (CButton*)GetDlgItem(IDCANCEL);
	pWndButton->EnableWindow(false); 
	
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_deal,this,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
}

unsigned __stdcall CCHARGETMAILDIALOG::_deal(LPVOID pParam)
{
	CCHARGETMAILDIALOG *p = (CCHARGETMAILDIALOG *)pParam;
	p->m_info.SetWindowText("正在处理，请不要关闭窗口.....");
	jmail::IAttachmentsPtr atts= p->chargemail.pMessage->GetAttachments();
	char * path = new char[_MAX_PATH];
	sprintf_s(path, _MAX_PATH, "%s\\share\\", CurrentPath);
	string savepath = path;
	int i = 0;
	if (atts != NULL)
	{
		for(;i<atts->Count;i++){
			CFile tempfile;
			string attachmentname = p->chargemail.pMessage->GetAttachments()->GetItem(i)->GetName();
			string savepath = CEMailADO::GetSavePath(CurrentPath + string("\\share\\") + attachmentname);
			p->chargemail.pMessage->GetAttachments()->GetItem(i)->SaveToFile(savepath.c_str());
			Tips(("交换的资源" + attachmentname + "已送到，保存到" + savepath).c_str());
		}
	}
	CButton*  pWndButton = (CButton*)p->GetDlgItem(IDC_BUTTON1);   
	pWndButton->EnableWindow(false);   
	pWndButton = (CButton*)p->GetDlgItem(IDCANCEL);
	pWndButton->EnableWindow(true); 
	CEMailADO::InsertMail(AspServer,p->chargemail.MailID.c_str(),UserName,"ChargeMail",p->chargemail.Replyer.c_str());
	p->m_info.SetWindowText("邮件处理完毕");
	POSITION pos = pMainWnd->Heart->UnReadedChargeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		POSITION pos1 = pos;
		CChargeMail tempmail = pMainWnd->Heart->UnReadedChargeMailList->GetNext(pos);
		if (tempmail.MailID.compare(p->chargemail.MailID) == 0)
		{
			pMainWnd->Heart->UnReadedChargeMailList->RemoveAt(pos1);
			break;
		}
	}
/*	if (p->recvmaildlg != NULL)
	{
		p->recvmaildlg->m_ChargeMailList.ResetContent();
		pos = pMainWnd->Heart->UnReadedChargeMailList->GetHeadPosition();
		while(pos != NULL)
		{
			CChargeMail temmail = pMainWnd->Heart->UnReadedChargeMailList->GetNext(pos);
			p->recvmaildlg->m_ChargeMailList.AddString(("与" + temmail.Replyer + "交换的资源已经到达").c_str());	
		}
	}*/
	pMainWnd->Heart->ReadedChargeMailList->AddTail(p->chargemail);
	return 0;
}

BOOL CCHARGETMAILDIALOG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_sender.SetWindowText(chargemail.Replyer.c_str());
	
	POSITION pos = chargemail.filenotfind->GetHeadPosition();
	while(pos != NULL)
	{
		CString temp = chargemail.filenotfind->GetNext(pos);
		int i;
		for(i = 0; i < m_filelist.GetCount(); i ++)
		{
			CString str;
			m_filelist.GetText(i,str);
			if (temp == str)
			{
				break;
			}
		}
		if (i == m_filelist.GetCount())
		{
			m_filelist.AddString(temp);
		}
	}
	pos = chargemail.attlist->GetHeadPosition();
	while(pos != NULL)
	{
		CString temp = chargemail.attlist->GetNext(pos);
		int i;
		for(i = 0; i < m_attlist.GetCount(); i ++)
		{
			CString str;
			m_attlist.GetText(i,str);
			if (temp == str)
			{
				break;
			}
		}
		if (i == m_attlist.GetCount())
		{
			m_attlist.AddString(temp);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
