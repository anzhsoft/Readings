// REQUESTMAILDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "REQUESTMAILDIALOG.h"
#include "MainFrm.h"
#include <string.h>

// #import  "jmail.dll"
// using namespace jmail;
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
// CREQUESTMAILDIALOG dialog

struct CRDAGREEINFO 
{
	CList<CString,CString&>* selecteditems;
	CRequestMail mailinfo;
};

CREQUESTMAILDIALOG::CREQUESTMAILDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CREQUESTMAILDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CREQUESTMAILDIALOG)
	//}}AFX_DATA_INIT
}

CREQUESTMAILDIALOG::CREQUESTMAILDIALOG(CRequestMail info, CRECVMAILDIALOG* dlg, CWnd* pParent /*=NULL*/)
: CDialog(CREQUESTMAILDIALOG::IDD, pParent)
{
	recvmaildlg = dlg;
	mailinfo = info;
	selecteditems = new CList<CString,CString&>();
}

void CREQUESTMAILDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CREQUESTMAILDIALOG)
	DDX_Control(pDX, IDC_AGREEBUTTON, m_agreeButton);
	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Control(pDX, IDC_REFUSEBUTTON, m_REFUSEBUTTON);
	DDX_Control(pDX, IDC_REQUESTSOURCE, m_sourcename);
	DDX_Control(pDX, IDC_USERNAME, m_username);
	DDX_Control(pDX, IDC_LIST1, m_SOURCTMEMBER);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CREQUESTMAILDIALOG, CDialog)
	//{{AFX_MSG_MAP(CREQUESTMAILDIALOG)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REFUSEBUTTON, OnRefusebutton)
	ON_BN_CLICKED(IDC_AGREEBUTTON, OnAgreebutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CREQUESTMAILDIALOG message handlers



void CREQUESTMAILDIALOG::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CREQUESTMAILDIALOG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	m_username.SetWindowText(mailinfo.RequesterUser.c_str());
	m_sourcename.SetWindowText(mailinfo.RequestSourceFilename.c_str());
	POSITION pos = mailinfo.files->GetHeadPosition();
	while(pos != NULL)
	{
		CString temp = mailinfo.files->GetNext(pos);
		int i;
		for(i = 0; i < m_SOURCTMEMBER.GetCount(); i ++)
		{
			CString str;
			m_SOURCTMEMBER.GetText(i,str);
			if (temp == str)
			{
				break;
			}
		}
		if (i == m_SOURCTMEMBER.GetCount())
		{
			m_SOURCTMEMBER.AddString(temp);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

unsigned __stdcall CREQUESTMAILDIALOG::_SendEMail(LPVOID pParam)
{
	
	CREQUESTMAILDIALOG * p = (CREQUESTMAILDIALOG *)pParam;
	p->m_info.SetWindowText("正在发送邮件，请不要关闭窗口...");
	CRequestMail mailinfo = p->mailinfo;
	CList<CString,CString&>* selecteditems = p->selecteditems;
	string MyUserName = UserName;
	//把文件名和扩展名分别提取出来，文件名在进行数据库查找时用，扩展名在查找得到的结果中查找被请求文件而且是发送时的参数
	string ExName = mailinfo.RequestSourceFilename.substr(mailinfo.RequestSourceFilename.find_last_of('.') + 1 ,mailinfo.RequestSourceFilename.length() - mailinfo.RequestSourceFilename.find_last_of('.') - 1);
	string FileName = mailinfo.RequestSourceFilename.substr(0 ,mailinfo.RequestSourceFilename.length() - ExName.length() - 1);

	string TheWholePath = CEMailADO::FindFilePath(mailinfo.RequestSourceFilename);
	
	//没有找到文件，发送一个拒绝邮件
	if(TheWholePath.length() == 0)
	{
		AfxMessageBox("被请求文件未找到!将发送一个拒绝邮件");
		jmail::IMessagePtr pMessage1("JMail.Message");
		try
		{
			pMessage1->MailServerUserName = pMainWnd->Heart->MyEMailUserName.c_str();   
			pMessage1->MailServerPassWord = pMainWnd->Heart->MyEMailPassWord.c_str();
			// 发件人邮箱
			pMessage1->From = (pMainWnd->Heart->MyEMailUserName + "@" + pMainWnd->Heart->MyEMailServer).c_str();
			// 发件人姓名
			pMessage1->FromName = MyUserName.c_str();
			// 添加收件人
			pMessage1->AddRecipient(mailinfo.RequesterEmail.c_str(), "", "");
			// 优先级设置,1-5逐次降低, 3为中级
			pMessage1->Priority = 3;
			// 编码方式设置, 默认是iso-8859-1
			pMessage1->Charset = "GB2312";
			// 主题
			pMessage1->Subject = (mailinfo.RequestSourceFilename + RefuseMail).c_str();
			// 正文
			pMessage1->AppendText(("username:" + MyUserName + "\r\n").c_str());
			pMessage1->AppendText(("filename:" + mailinfo.RequestSourceFilename + "\r\n").c_str());
			pMessage1->AppendText("reason:请求文件在目标机器上未找到\r\n");
			// 开始发送
			pMessage1->Send(pMainWnd->Heart->SmtpServer.c_str(), VARIANT_FALSE);
			Tips("回复拒绝邮件!");
			CEMailADO::InsertMail(AspServer,mailinfo.MailId.c_str(),UserName,"RequestMail",mailinfo.RequesterUser.c_str());
			p->m_info.SetWindowText("邮件发送完毕");
			POSITION pos = pMainWnd->Heart->UnReadedRequestMailList->GetHeadPosition();
			while(pos != NULL)
			{
				POSITION pos1 = pos;
				CRequestMail tempmail = pMainWnd->Heart->UnReadedRequestMailList->GetNext(pos);
				if (tempmail.MailId.compare(p->mailinfo.MailId) == 0)
				{
					pMainWnd->Heart->UnReadedRequestMailList->RemoveAt(pos1);
					break;
				}
			}
			if (p->recvmaildlg != NULL)
			{
				p->recvmaildlg->m_MailTitle.ResetContent();
				pos = pMainWnd->Heart->UnReadedRequestMailList->GetHeadPosition();
				while(pos != NULL)
				{
					CRequestMail temmail = pMainWnd->Heart->UnReadedRequestMailList->GetNext(pos);
					p->recvmaildlg->m_MailTitle.AddString((temmail.RequesterUser + "请求" + temmail.RequestSourceFilename).c_str());	
				}
				p->recvmaildlg->resetscol();
			}
			pMainWnd->Heart->ReadedRequestMailList->AddTail(p->mailinfo);
		}
		catch (_com_error e)
		{
			p->m_info.SetWindowText("邮件发送失败");
			CString strErr;
			strErr.Format("错误信息：%s\r\n错误描述：%s", (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
			Tips(strErr);
		}
		if (pMessage1 != NULL)
		{
			pMessage1.Release();
		}
		
		
	}else //找到被请求的文件，发送文件
	{
		CFile att;
		att.Open(TheWholePath.c_str(),CFile::modeRead);
		if (att.GetLength() > 10*1024*1024)
		{
			Tips("文件过大!");
			CButton* pWndButton1 = (CButton*)p->GetDlgItem(IDCANCEL);
			pWndButton1->EnableWindow(true);
			p->m_info.SetWindowText("邮件发送失败");
			att.Close();
			return 0;
		}
		att.Close();
		Tips("找到文件,正在发送...");
		jmail::IMessagePtr pMessage1("JMail.Message");
		try
		{
			pMessage1->MailServerUserName = pMainWnd->Heart->MyEMailUserName.c_str();   
			pMessage1->MailServerPassWord = pMainWnd->Heart->MyEMailPassWord.c_str();
			// 发件人邮箱
			pMessage1->From = (pMainWnd->Heart->MyEMailUserName + "@" + pMainWnd->Heart->MyEMailServer).c_str();
			// 发件人姓名
			pMessage1->FromName = MyUserName.c_str();
			// 添加收件人
			pMessage1->AddRecipient(mailinfo.RequesterEmail.c_str(), "", "");
			// 优先级设置,1-5逐次降低, 3为中级
			pMessage1->Priority = 3;
			// 编码方式设置, 默认是iso-8859-1
			pMessage1->Charset = "GB2312";
			// 主题
			pMessage1->Subject = (mailinfo.RequestSourceFilename + AgreeMail).c_str();
			// 正文
			pMessage1->AppendText(("username:" + MyUserName + "\r\n").c_str());
			pMessage1->AppendText(("email:" + pMainWnd->Heart->MyEMailUserName + "@" + pMainWnd->Heart->MyEMailServer + "\r\n").c_str());
			pMessage1->AppendText("resource-list:\r\n");
			POSITION pos = selecteditems->GetHeadPosition();
			if (pos != NULL)
			{
				while(pos != NULL)
				{
					CString temp = selecteditems->GetNext(pos) + "\r\n";
					pMessage1->AppendText(temp.GetBuffer(256));
				}
			}else{
				pMessage1->AppendText("无\r\n");
			}
			
			//附件
			pMessage1->AddAttachment(TheWholePath.c_str(),true,ExName.c_str());
			// 开始发送
			pMessage1->Send(pMainWnd->Heart->SmtpServer.c_str(), VARIANT_FALSE);
			Tips("邮件请求已成功回复!");
			p->m_info.SetWindowText("邮件发送完毕");
			CEMailADO::InsertMail(AspServer,mailinfo.MailId.c_str(),UserName,"RequestMail",mailinfo.RequesterUser.c_str());
			pos = pMainWnd->Heart->UnReadedRequestMailList->GetHeadPosition();
			while(pos != NULL)
			{
				POSITION pos1 = pos;
				CRequestMail tempmail = pMainWnd->Heart->UnReadedRequestMailList->GetNext(pos);
				if (tempmail.MailId.compare(p->mailinfo.MailId) == 0)
				{
					pMainWnd->Heart->UnReadedRequestMailList->RemoveAt(pos1);
					break;
				}
			}
			if (p->recvmaildlg != NULL)
			{
				
				p->recvmaildlg->m_MailTitle.ResetContent();
				pos = pMainWnd->Heart->UnReadedRequestMailList->GetHeadPosition();
				while(pos != NULL)
				{
					CRequestMail temmail = pMainWnd->Heart->UnReadedRequestMailList->GetNext(pos);
					p->recvmaildlg->m_MailTitle.AddString((temmail.RequesterUser + "请求" + temmail.RequestSourceFilename).c_str());	
				}
			}
			pMainWnd->Heart->ReadedRequestMailList->AddTail(p->mailinfo);
		}
		catch (_com_error e)
		{
			p->m_info.SetWindowText("邮件发送失败");
			CString strErr;
			strErr.Format("错误信息：%s\r\n错误描述：%s", (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
			Tips(strErr);
		}
		if (pMessage1 != NULL)
		{
			pMessage1.Release();
		}
		
	}
	CButton* pWndButton = (CButton*)p->GetDlgItem(IDCANCEL);
	pWndButton->EnableWindow(true);
	return 0;
}

void CREQUESTMAILDIALOG::OnDestroy() 
{
	CDialog::OnDestroy();
	delete this;
	// TODO: Add your message handler code here
	
}

void CREQUESTMAILDIALOG::OnRefusebutton() 
{
	// TODO: Add your control notification handler code here
	CButton*  pWndButton = (CButton*)GetDlgItem(IDC_AGREEBUTTON);   
    pWndButton->EnableWindow(false);   
	pWndButton = (CButton*)GetDlgItem(IDC_REFUSEBUTTON);
	pWndButton->EnableWindow(false);
	pWndButton = (CButton*)GetDlgItem(IDCANCEL);
	pWndButton->EnableWindow(false);
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_SendRefuseEMail,this,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
}

unsigned __stdcall CREQUESTMAILDIALOG::_SendRefuseEMail(LPVOID pParam)
{
	CREQUESTMAILDIALOG * p = (CREQUESTMAILDIALOG *) pParam;
	p->m_info.SetWindowText("正在发送邮件，请不要关闭窗口...");
	CRequestMail mailinfo = p->mailinfo;
	string MyUserName = UserName;
	jmail::IMessagePtr pMessage1("JMail.Message");
	try
	{
		pMessage1->MailServerUserName = pMainWnd->Heart->MyEMailUserName.c_str();   
		pMessage1->MailServerPassWord = pMainWnd->Heart->MyEMailPassWord.c_str();
		// 发件人邮箱
		pMessage1->From = (pMainWnd->Heart->MyEMailUserName + "@" +pMainWnd->Heart->MyEMailServer).c_str();
		// 发件人姓名
		pMessage1->FromName = MyUserName.c_str();
		// 添加收件人
		pMessage1->AddRecipient(mailinfo.RequesterEmail.c_str(), "", "");
		// 优先级设置,1-5逐次降低, 3为中级
		pMessage1->Priority = 3;
		// 编码方式设置, 默认是iso-8859-1
		pMessage1->Charset = "GB2312";
		// 主题
		pMessage1->Subject = (mailinfo.RequestSourceFilename + RefuseMail).c_str();
		// 正文
		pMessage1->AppendText(("username:" + MyUserName + "\r\n").c_str());
		pMessage1->AppendText(("filename:" + mailinfo.RequestSourceFilename + "\r\n").c_str());
		pMessage1->AppendText("reason:无\r\n");
		// 开始发送
		pMessage1->Send(pMainWnd->Heart->SmtpServer.c_str(), VARIANT_FALSE);
		pMessage1.Release();
		CEMailADO::InsertMail(AspServer,mailinfo.MailId.c_str(),UserName,"RequestMail",mailinfo.RequesterUser.c_str());
		POSITION pos = pMainWnd->Heart->UnReadedRequestMailList->GetHeadPosition();
		while(pos != NULL)
		{
			POSITION pos1 = pos;
			CRequestMail tempmail = pMainWnd->Heart->UnReadedRequestMailList->GetNext(pos);
			if (tempmail.MailId.compare(p->mailinfo.MailId) == 0)
			{
				pMainWnd->Heart->UnReadedRequestMailList->RemoveAt(pos1);
				break;
			}
		}
		if (p->recvmaildlg != NULL)
		{
			p->recvmaildlg->m_MailTitle.ResetContent();
			pos = pMainWnd->Heart->UnReadedRequestMailList->GetHeadPosition();
			while(pos != NULL)
			{
				CRequestMail temmail = pMainWnd->Heart->UnReadedRequestMailList->GetNext(pos);
				p->recvmaildlg->m_MailTitle.AddString((temmail.RequesterUser + "请求" + temmail.RequestSourceFilename).c_str());	
			}
			p->recvmaildlg->resetscol();
		}
		pMainWnd->Heart->ReadedRequestMailList->AddTail(p->mailinfo);
		Tips("回复拒绝邮件!");
		p->m_info.SetWindowText("邮件发送完毕");
	}
	catch (_com_error e)
	{
		p->m_info.SetWindowText("邮件发送失败");
		if (pMessage1 != NULL)
		{
			pMessage1.Release();
		}
		CString strErr;
		strErr.Format("错误信息：%s\r\n错误描述：%s", (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
		Tips(strErr);
	}
	CButton* pWndButton = (CButton*)p->GetDlgItem(IDCANCEL);
	pWndButton->EnableWindow(true);
	return 0;
}

void CREQUESTMAILDIALOG::OnAgreebutton() 
{
	// TODO: Add your control notification handler code here
	CButton*  pWndButton = (CButton*)GetDlgItem(IDC_AGREEBUTTON);   
    pWndButton->EnableWindow(false);   
	pWndButton = (CButton*)GetDlgItem(IDC_REFUSEBUTTON);
	pWndButton->EnableWindow(false); 
	pWndButton = (CButton*)GetDlgItem(IDCANCEL);
	pWndButton->EnableWindow(false);
	int* ItemIndex = new int[100]; 
	int nItem = m_SOURCTMEMBER.GetSelCount();
	m_SOURCTMEMBER.GetSelItems(100, ItemIndex);
	if (nItem != 0)
	{
		for (int i = 0; i < nItem; i ++)
		{
			CString thebookname;
			m_SOURCTMEMBER.GetText(ItemIndex[i],thebookname);
			selecteditems->AddHead(thebookname);
		}
	}
	
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_SendEMail,this,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
}
