// RECVMAILDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "RECVMAILDIALOG.h"
#include "REQUESTMAILDIALOG.h"
#include "AGREEMAILDIALOG.h"
#include "CHARGETMAILDIALOG.h"
#include "REFUSETMAILDIALOG.h"
#include "MailHistory.h"
#include "MainFrm.h"
extern CMainFrame* pMainWnd;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRECVMAILDIALOG dialog

CRECVMAILDIALOG::CRECVMAILDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CRECVMAILDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRECVMAILDIALOG)
	//}}AFX_DATA_INIT
}



void CRECVMAILDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRECVMAILDIALOG)
	DDX_Control(pDX, IDC_stat, m_stat);
	DDX_Control(pDX, IDC_DownLoadFiles, m_DownLoadFiles);
	DDX_Control(pDX, IDC_MAILLIST, m_MailTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRECVMAILDIALOG, CDialog)
	//{{AFX_MSG_MAP(CRECVMAILDIALOG)
	ON_LBN_SELCHANGE(IDC_MAILLIST, OnSelchangeMaillist)
	ON_LBN_DBLCLK(IDC_MAILLIST, OnDblclkMaillist)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_MailLIST1, OnDblclkMailLIST1)
	ON_LBN_DBLCLK(IDC_MailLIST2, OnDblclkMailLIST2)
	ON_LBN_DBLCLK(IDC_MailLIST3, OnDblclkMailLIST3)
	ON_BN_CLICKED(IDC_HistoryBUTTON, OnHistoryBUTTON)
	ON_BN_CLICKED(IDC_Reflush, OnReflush)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRECVMAILDIALOG message handlers

void CRECVMAILDIALOG::OnSelchangeMaillist() 
{
	// TODO: Add your control notification handler code here
	
}
void CRECVMAILDIALOG::OnDblclkMaillist() 
{
	// TODO: Add your control notification handler code here
	CString SelectedMailId;
	m_MailTitle.GetText(m_MailTitle.GetCurSel(),SelectedMailId);
	CRequestMail SelectedMail;
	POSITION pos = pMainWnd->Heart->UnReadedRequestMailList->GetHeadPosition();
	while(pos != NULL)
	{
		SelectedMail = pMainWnd->Heart->UnReadedRequestMailList->GetNext(pos);
		if ((SelectedMail.RequesterUser + "����" + SelectedMail.RequestSourceFilename).c_str() == SelectedMailId)
		{
			break;
		}
	}
	if (SelectedMailId.GetLength() == 0)
	{
		AfxMessageBox("�˷��ʼ�����");
		return;
	}
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CREQUESTMAILDIALOG* requestmaildialog = new CREQUESTMAILDIALOG(SelectedMail,this);
	//requestmaildialog.DoModal();
	requestmaildialog->Create(IDD_REQUESTMAILDIALOG);//,GetDesktopWindow());
	requestmaildialog->ShowWindow(SW_RESTORE);
}

BOOL CRECVMAILDIALOG::OnInitDialog() 
{
	CDialog::OnInitDialog();
//	m_MailTitle.SetHorizontalExtent();
	
	// TODO: Add extra initialization here
 
	POSITION pos = pMainWnd->Heart->UnReadedRequestMailList->GetHeadPosition();
	while(pos != NULL)
	{
		CRequestMail temmail=pMainWnd->Heart->UnReadedRequestMailList->GetNext(pos);
		m_MailTitle.AddString((temmail.RequesterUser + "����" 
			+ temmail.RequestSourceFilename).c_str());	
	}

/*	pos = pMainWnd->Heart->UnReadedRefuseMailList->GetHeadPosition();
	while(pos != NULL)
	{
		CRefuseMail temmail=pMainWnd->Heart->UnReadedRefuseMailList->GetNext(pos);
		m_RefuseMailList.AddString((temmail.Replyer + "�ܾ�����������").c_str());	
	}

	pos = pMainWnd->Heart->UnReadedChargeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		CChargeMail temmail=pMainWnd->Heart->UnReadedChargeMailList->GetNext(pos);
		m_ChargeMailList.AddString(("��" + temmail.Replyer + "��������Դ�Ѿ�����").c_str());	
	}

	pos = pMainWnd->Heart->UnReadedAgreeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		CAgreeMail temmail=pMainWnd->Heart->UnReadedAgreeMailList->GetNext(pos);
		m_AgreeMailList.AddString((temmail.AgreeReplyer + "ͬ��������\"" + temmail.attname + "\"����").c_str());	
	}
*/
	pos = pMainWnd->Heart->HasDownLoadMailAtts->GetHeadPosition();
	while(pos != NULL)
	{
		CString DownLoadFile=pMainWnd->Heart->HasDownLoadMailAtts->GetNext(pos);
		m_DownLoadFiles.AddString(DownLoadFile);	
	}
 
 	pMainWnd->RecvDlg = this;
 	m_stat.SetWindowText(pMainWnd->Heart->MailStat);
	
	resetscol();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRECVMAILDIALOG::OnDestroy() 
{
	CDialog::OnDestroy();
	pMainWnd->RecvDlg = NULL;
	delete this;
	// TODO: Add your message handler code here
	
}

void CRECVMAILDIALOG::OnDblclkMailLIST1() 
{
	// TODO: Add your control notification handler code here
	CString SelectedMailId;
//	m_AgreeMailList.GetText(m_AgreeMailList.GetCurSel(),SelectedMailId);
	CAgreeMail SelectedMail;
	POSITION pos = pMainWnd->Heart->UnReadedAgreeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		SelectedMail = pMainWnd->Heart->UnReadedAgreeMailList->GetNext(pos);
		if ((SelectedMail.AgreeReplyer + "ͬ��������" + SelectedMail.attname + "����").c_str() == SelectedMailId)
		{
			break;
		}
	}
	if (SelectedMailId.GetLength() == 0)
	{
		AfxMessageBox("�˷��ʼ�����");
		return;
	}
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAGREEMAILDIALOG* requestmaildialog = new CAGREEMAILDIALOG(SelectedMail,this);
	//requestmaildialog.DoModal();
	requestmaildialog->Create(IDD_AGREEMAILDIALOG);//,GetDesktopWindow());
	requestmaildialog->ShowWindow(SW_RESTORE);
}

void CRECVMAILDIALOG::OnDblclkMailLIST2() 
{
	// TODO: Add your control notification handler code here
	CString SelectedMailId;
//	m_ChargeMailList.GetText(m_ChargeMailList.GetCurSel(),SelectedMailId);
	CChargeMail SelectedMail;
	POSITION pos = pMainWnd->Heart->UnReadedChargeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		SelectedMail = pMainWnd->Heart->UnReadedChargeMailList->GetNext(pos);
		if (("��" + SelectedMail.Replyer + "��������Դ�Ѿ�����").c_str() == SelectedMailId)
		{
			break;
		}
	}
	if (SelectedMailId.GetLength() == 0)
	{
		AfxMessageBox("�˷��ʼ�����");
		return;
	}
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCHARGETMAILDIALOG* requestmaildialog = new CCHARGETMAILDIALOG(SelectedMail,this);
	//requestmaildialog.DoModal();
	requestmaildialog->Create(IDD_CHARGETMAILDIALOG);//,GetDesktopWindow());
	requestmaildialog->ShowWindow(SW_RESTORE);
}

void CRECVMAILDIALOG::OnDblclkMailLIST3() 
{
	// TODO: Add your control notification handler code here
	CString SelectedMailId;
/*	m_RefuseMailList.GetText(m_RefuseMailList.GetCurSel(),SelectedMailId);*/
	CRefuseMail SelectedMail;
	
	POSITION pos = pMainWnd->Heart->UnReadedRefuseMailList->GetHeadPosition();
	while(pos != NULL)
	{
		SelectedMail = pMainWnd->Heart->UnReadedRefuseMailList->GetNext(pos);
		if ((SelectedMail.Replyer + "�ܾ�����������").c_str() == SelectedMailId)
		{
			break;
		}
	}
	
	if (SelectedMailId.GetLength() == 0)
	{
		AfxMessageBox("�˷��ʼ�����");
		return;
	}
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CREFUSETMAILDIALOG* requestmaildialog = new CREFUSETMAILDIALOG(SelectedMail,this);
	//AfxMessageBox(SelectedMailId);
	requestmaildialog->Create(IDD_REFUSETMAILDIALOG);//,GetDesktopWindow());
	requestmaildialog->ShowWindow(SW_RESTORE);
}

void CRECVMAILDIALOG::OnHistoryBUTTON() 
{
	// TODO: Add your control notification handler code here
	CMailHistory* requestmaildialog = new CMailHistory();
	//AfxMessageBox(SelectedMailId);
	requestmaildialog->Create(IDD_MailHistory);//,GetDesktopWindow());
	requestmaildialog->ShowWindow(SW_RESTORE);
}

void CRECVMAILDIALOG::OnReflush()
{
	// TODO: Add your control notification handler code here
	if (pMainWnd->Heart->IsMailSearchDone == FALSE)
	{
		AfxMessageBox("�ϴ��ʼ���ѯδ����,���Ե�!");
		return;
	}
	m_MailTitle.ResetContent();
//	m_DownLoadFiles.ResetContent();

	pMainWnd->ReflushMail();
}

void CRECVMAILDIALOG::resetscol()
{
	CString   str;   
	CSize sz;   
	int dx=0;   
	CDC* pDC = m_MailTitle.GetDC();   
	int i;
	for (i=0;i < m_MailTitle.GetCount();i++)   
	{   
        m_MailTitle.GetText(i, str);   
        sz = pDC->GetTextExtent(str);   
		
        if (sz.cx > dx)   
			dx = sz.cx;   
	}   
	m_MailTitle.ReleaseDC(pDC); 
	m_MailTitle.SetHorizontalExtent(dx); 
	
	dx=0;   
	pDC = m_DownLoadFiles.GetDC();   
	for (i=0;i < m_DownLoadFiles.GetCount();i++)   
	{   
        m_DownLoadFiles.GetText(i, str);   
        sz = pDC->GetTextExtent(str);   
		
        if (sz.cx > dx)   
			dx = sz.cx;   
	}   
	m_DownLoadFiles.ReleaseDC(pDC); 
	m_DownLoadFiles.SetHorizontalExtent(dx);  

}
