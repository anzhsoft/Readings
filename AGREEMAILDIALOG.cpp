// AGREEMAILDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "readings.h"
#include "AGREEMAILDIALOG.h"
#include "MainFrm.h"
#include "RECVMAILDIALOG.h"
#include "EMailADO.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern char CurrentPath[_MAX_PATH];
extern char UserName[50];
extern char PassWord[50];
extern CMainFrame* pMainWnd;
extern string RequestMail;
extern string AgreeMail;
extern string RefuseMail;
extern string ChargingMail;
extern char AspServer[1024];
/////////////////////////////////////////////////////////////////////////////
// CAGREEMAILDIALOG dialog


CAGREEMAILDIALOG::CAGREEMAILDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(CAGREEMAILDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAGREEMAILDIALOG)
	//}}AFX_DATA_INIT
}

CAGREEMAILDIALOG::CAGREEMAILDIALOG(CAgreeMail mail, CRECVMAILDIALOG* dlg,CWnd* pParent /*=NULL*/)
: CDialog(CAGREEMAILDIALOG::IDD, pParent)
{
	agreemail = mail;
	recvmaildlg = dlg;
}


void CAGREEMAILDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAGREEMAILDIALOG)
	DDX_Control(pDX, IDC_ChargeFileList, m_chargefilelist);
	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Control(pDX, IDC_filename, m_filename);
	DDX_Control(pDX, IDC_Agree, m_Agreeer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAGREEMAILDIALOG, CDialog)
	//{{AFX_MSG_MAP(CAGREEMAILDIALOG)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAGREEMAILDIALOG message handlers

void CAGREEMAILDIALOG::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CButton*  pWndButton = (CButton*)GetDlgItem(IDC_BUTTON1);   
    pWndButton->EnableWindow(false);   
	pWndButton = (CButton*)GetDlgItem(IDCANCEL);
	pWndButton->EnableWindow(false); 

	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_deal,this,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);

}

BOOL CAGREEMAILDIALOG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_filename.SetWindowText(agreemail.attname.c_str());
	m_Agreeer.SetWindowText(agreemail.AgreeReplyer.c_str());

	POSITION pos = agreemail.FCResource->GetHeadPosition();
	while (pos != NULL)
	{
		CString tem = agreemail.FCResource->GetNext(pos);
		int i;
		for(i = 0; i < m_chargefilelist.GetCount(); i ++)
		{
			CString str;
			m_chargefilelist.GetText(i,str);
			if (tem == str)
			{
				break;
			}
		}
		if (i == m_chargefilelist.GetCount())
		{
			m_chargefilelist.AddString(tem);
		}
		
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAGREEMAILDIALOG::OnDestroy() 
{
	CDialog::OnDestroy();
	delete this;
	// TODO: Add your message handler code here
	
}

unsigned __stdcall CAGREEMAILDIALOG::_deal(LPVOID pParam)
{
	CAGREEMAILDIALOG *p = (CAGREEMAILDIALOG *)pParam;
	p->m_info.SetWindowText("���ڴ����벻Ҫ�رմ���.....");
	string savepath = CEMailADO::GetSavePath(CurrentPath + string("\\share\\") + p->agreemail.attname);
	if (savepath.length() == 0)
	{
		CButton*  pWndButton = (CButton*)p->GetDlgItem(IDC_BUTTON1);
		pWndButton->EnableWindow(false);   
		pWndButton = (CButton*)p->GetDlgItem(IDCANCEL);
		pWndButton->EnableWindow(true); 
		AfxMessageBox("�Ҳ�������·����");
		return 0;
	}
	p->agreemail.pMessage->GetAttachments()->GetItem(0)->SaveToFile(savepath.c_str());
	POSITION pos = p->agreemail.FCResource->GetHeadPosition();
	if (pos != NULL)
	{
		CString firstfile = p->agreemail.FCResource->GetNext(pos);
		if (firstfile != "��")
		{
			jmail::IMessagePtr pMessage1("JMail.Message");
			try
			{
				pMessage1->MailServerUserName = pMainWnd->Heart->MyEMailUserName.c_str();   
				pMessage1->MailServerPassWord = pMainWnd->Heart->MyEMailPassWord.c_str();
				// ����������
				pMessage1->From = (pMainWnd->Heart->MyEMailUserName + "@" + pMainWnd->Heart->MyEMailServer).c_str();
				// ����������
				pMessage1->FromName = UserName;
				// ����ռ���
				pMessage1->AddRecipient(p->agreemail.desEMail.c_str(), "", "");
				// ���ȼ�����,1-5��ν���, 3Ϊ�м�
				pMessage1->Priority = 3;
				// ���뷽ʽ����, Ĭ����iso-8859-1
				pMessage1->Charset = "GB2312";
				// ����
				pMessage1->Subject = (p->agreemail.attname + ChargingMail).c_str();
				// ����
				pMessage1->AppendText(("username:" + string(UserName) + "\r\n").c_str());
				pMessage1->AppendText("filenotfind:\r\n");
				POSITION pos = p->agreemail.FCResource->GetHeadPosition();
				while(pos != NULL)
				{
					CString temp1 = p->agreemail.FCResource->GetNext(pos);
					string temp = temp1.GetBuffer(256);
					string TheWholePath = CEMailADO::FindFilePath(temp);
					//���ļ�������չ���ֱ���ȡ�������ļ����ڽ������ݿ����ʱ�ã���չ���ڲ��ҵõ��Ľ���в��ұ������ļ������Ƿ���ʱ�Ĳ���
					string ExName = temp.substr(temp.find_last_of('.') + 1 ,temp.length() - temp.find_last_of('.') - 1);
					string FileName = temp.substr(0 ,temp.length() - ExName.length() - 1);
					if(TheWholePath.length() == 0)
					{
						pMessage1->AppendText((temp + "\r\n").c_str());
					}else
					{
						CFile att;
						att.Open(TheWholePath.c_str(),CFile::modeRead);
						if (att.GetLength() < 10*1024*1024)
						{
							att.Close();
							pMessage1->AddAttachment(TheWholePath.c_str(),true,ExName.c_str());
						}else{
							att.Close();
						}
					}
				}
				// ��ʼ����
				pMessage1->Send(pMainWnd->Heart->SmtpServer.c_str(), VARIANT_FALSE);
						
			}
			catch (_com_error e)
			{
				p->m_info.SetWindowText("�ʼ�����ʧ��");
				CButton*  pWndButton = (CButton*)p->GetDlgItem(IDC_BUTTON1);   
				pWndButton->EnableWindow(false);   
				pWndButton = (CButton*)p->GetDlgItem(IDCANCEL);
				pWndButton->EnableWindow(true); 
				CString strErr;
				strErr.Format("������Ϣ��%s\r\n����������%s", (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
				Tips(strErr);
				if (pMessage1 != NULL)
				{
					pMessage1.Release();
				}
				return 0;
			}
			if (pMessage1 != NULL)
			{
				pMessage1.Release();
			}
			
		}
	}
	pos = pMainWnd->Heart->UnReadedAgreeMailList->GetHeadPosition();
	while(pos != NULL)
	{
		POSITION pos1 = pos;
		CAgreeMail tempmail = pMainWnd->Heart->UnReadedAgreeMailList->GetNext(pos);
		if (tempmail.MailID.compare(p->agreemail.MailID) == 0)
		{
			pMainWnd->Heart->UnReadedAgreeMailList->RemoveAt(pos1);
			break;
		}
	}
	pMainWnd->Heart->ReadedAgreeMailList->AddTail(p->agreemail);
/*	if(p->recvmaildlg != NULL)
	{
		p->recvmaildlg->m_AgreeMailList.ResetContent();
		pos = pMainWnd->Heart->UnReadedAgreeMailList->GetHeadPosition();
		while(pos != NULL)
		{
			CAgreeMail temmail = pMainWnd->Heart->UnReadedAgreeMailList->GetNext(pos);
			p->recvmaildlg->m_AgreeMailList.AddString((temmail.AgreeReplyer + "ͬ��������" + temmail.attname + "����").c_str());	
		}
	}*/
	CEMailADO::InsertMail(AspServer,p->agreemail.MailID.c_str(),UserName,"AgreeMail",p->agreemail.AgreeReplyer.c_str());
	p->m_info.SetWindowText("�ʼ��������");
	Tips("�ʼ��������");
	CButton*  pWndButton = (CButton*)p->GetDlgItem(IDC_BUTTON1);   
    pWndButton->EnableWindow(false);   
	pWndButton = (CButton*)p->GetDlgItem(IDCANCEL);
	pWndButton->EnableWindow(true); 
	
	return 0;
}

