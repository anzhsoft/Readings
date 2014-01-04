// MailHistory.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "MailHistory.h"
#include "MainFrm.h"
#include "RECVMAILDIALOG.h"
#include "REQUESTMAILDIALOG.h"
#include "AGREEMAILDIALOG.h"
#include "CHARGETMAILDIALOG.h"
#include "REFUSETMAILDIALOG.h"
extern CMainFrame* pMainWnd;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailHistory dialog


CMailHistory::CMailHistory(CWnd* pParent /*=NULL*/)
	: CDialog(CMailHistory::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMailHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailHistory)
	DDX_Control(pDX, IDC_MailLIST3, m_HasReadedRefuseMailList);
	DDX_Control(pDX, IDC_MailLIST2, m_HasReadedChargeMailList);
	DDX_Control(pDX, IDC_MailLIST1, m_HasReadedAgreeMailList);
	DDX_Control(pDX, IDC_MAILLIST, m_HasReadedRequestMailList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailHistory, CDialog)
	//{{AFX_MSG_MAP(CMailHistory)
	ON_LBN_DBLCLK(IDC_MailLIST1, OnDblclkMailLIST1)
	ON_LBN_DBLCLK(IDC_MailLIST2, OnDblclkMailLIST2)
	ON_LBN_DBLCLK(IDC_MailLIST3, OnDblclkMailLIST3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailHistory message handlers

BOOL CMailHistory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ChargeMailList = new CList<CChargeMail,CChargeMail&>;
	AgreeMailList = new CList<CAgreeMail,CAgreeMail&>;
	RefuseMailList = new CList<CRefuseMail,CRefuseMail&>;
	RequestMailList = new CList<CRequestMail,CRequestMail&>;
	
	ChargeMailList = pMainWnd->Heart->ReadedChargeMailList;
	AgreeMailList = pMainWnd->Heart->ReadedAgreeMailList;
	RequestMailList = pMainWnd->Heart->ReadedRequestMailList;
	RefuseMailList = pMainWnd->Heart->ReadedRefuseMailList;
	
	POSITION pos = RequestMailList->GetHeadPosition();
	while(pos != NULL)
	{
		CRequestMail temmail=RequestMailList->GetNext(pos);
		m_HasReadedRequestMailList.AddString((temmail.RequesterUser + "请求\"" 
			+ temmail.RequestSourceFilename + "\"").c_str());	
	}
	
	pos = RefuseMailList->GetHeadPosition();
	while(pos != NULL)
	{
		CRefuseMail temmail=RefuseMailList->GetNext(pos);
		m_HasReadedRefuseMailList.AddString((temmail.Replyer + "拒绝了您的请求\"" + temmail.filename + "\"").c_str());	
	}
	
	pos = ChargeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		CChargeMail temmail=ChargeMailList->GetNext(pos);
		m_HasReadedChargeMailList.AddString(("与" + temmail.Replyer + "交换的资源已经到达").c_str());	
	}
	
	pos = AgreeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		CAgreeMail temmail=AgreeMailList->GetNext(pos);
		m_HasReadedAgreeMailList.AddString((temmail.AgreeReplyer + "同意了您的\"" + temmail.attname + "\"请求").c_str());	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMailHistory::OnDblclkMailLIST1() 
{
	// TODO: Add your control notification handler code here
	CString SelectedMailId;
	m_HasReadedAgreeMailList.GetText(m_HasReadedAgreeMailList.GetCurSel(),SelectedMailId);
	CAgreeMail SelectedMail;
	POSITION pos = pMainWnd->Heart->ReadedAgreeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		SelectedMail = pMainWnd->Heart->ReadedAgreeMailList->GetNext(pos);
		if ((SelectedMail.AgreeReplyer + "同意了您的\"" + SelectedMail.attname + "\"请求").c_str() == SelectedMailId)
		{
			break;
		}
	}
	if (SelectedMailId.GetLength() == 0)
	{
		AfxMessageBox("此封邮件错误");
		return;
	}
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAGREEMAILDIALOG* requestmaildialog = new CAGREEMAILDIALOG(SelectedMail,NULL);
	requestmaildialog->Create(IDD_AGREEMAILDIALOG);//,GetDesktopWindow());
	requestmaildialog->ShowWindow(SW_RESTORE);
}

void CMailHistory::OnDblclkMailLIST2() 
{
	// TODO: Add your control notification handler code here
	CString SelectedMailId;
	m_HasReadedChargeMailList.GetText(m_HasReadedChargeMailList.GetCurSel(),SelectedMailId);
	CChargeMail SelectedMail;
	POSITION pos = pMainWnd->Heart->ReadedChargeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		SelectedMail = pMainWnd->Heart->ReadedChargeMailList->GetNext(pos);
		if (("与" + SelectedMail.Replyer + "交换的资源已经到达").c_str() == SelectedMailId)
		{
			break;
		}
	}
	if (SelectedMailId.GetLength() == 0)
	{
		AfxMessageBox("此封邮件错误");
		return;
	}
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCHARGETMAILDIALOG* requestmaildialog = new CCHARGETMAILDIALOG(SelectedMail,NULL);
	//requestmaildialog.DoModal();
	requestmaildialog->Create(IDD_CHARGETMAILDIALOG);//,GetDesktopWindow());
	requestmaildialog->ShowWindow(SW_RESTORE);
}

void CMailHistory::OnDblclkMailLIST3() 
{
	// TODO: Add your control notification handler code here
	CString SelectedMailId;
	m_HasReadedRefuseMailList.GetText(m_HasReadedRefuseMailList.GetCurSel(),SelectedMailId);
	CRefuseMail SelectedMail;
	
	POSITION pos = pMainWnd->Heart->ReadedRefuseMailList->GetHeadPosition();
	while(pos != NULL)
	{
		SelectedMail = pMainWnd->Heart->ReadedRefuseMailList->GetNext(pos);
		if ((SelectedMail.Replyer + "拒绝了您的请求\"" + SelectedMail.filename + "\"").c_str() == SelectedMailId)
		{
			break;
		}
	}
	
	if (SelectedMailId.GetLength() == 0)
	{
		AfxMessageBox("此封邮件错误");
		return;
	}
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CREFUSETMAILDIALOG* requestmaildialog = new CREFUSETMAILDIALOG(SelectedMail,NULL);
	//AfxMessageBox(SelectedMailId);
	requestmaildialog->Create(IDD_REFUSETMAILDIALOG);//,GetDesktopWindow());
	requestmaildialog->ShowWindow(SW_RESTORE);
}
