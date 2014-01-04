// ForJmail.cpp: implementation of the CForJmail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "ForJmail.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern CMainFrame* pMainWnd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CForJmail::CForJmail(CString addr, CString pass)
{
	CoInitialize(NULL); 
	pPOP3 = jmail::IPOP3Ptr("JMail.POP3");
	add = addr;
	password = pass;
}

CForJmail::~CForJmail()
{
	
}

BOOL CForJmail::GetMailHead()
{
	try{
		pPOP3->DownloadHeaders();
	}catch(_com_error e){
		return FALSE;
	}
	return TRUE;
}

BOOL CForJmail::ConnectToPOP3()
{
	POP3Server = GetPOP3Server(add);
	username = add.Left(add.ReverseFind('@'));
	pPOP3->Timeout = 300;
	try
	{
		pPOP3->Connect( (LPCTSTR)username,
			(LPCTSTR)password, 
			(LPCTSTR)POP3Server, 
			110);
	}
	catch (_com_error e)
	{
		return FALSE;
	}
	return TRUE;
}

CString CForJmail::GetPOP3Server(CString addr)
{
	CString server = addr.Right(addr.GetLength() - addr.ReverseFind('@') - 1);
	if (server == "sina.com")
	{
		return "pop.sina.com.cn";
	}else if (server == "sohu.com")
	{
		return "pop3.sohu.com";
	}else if (server == "qq.com")
	{
		return "pop.qq.com";
	}else{
		return "";
	}
}

BOOL CForJmail::DisConnectToPOP3()
{
	try{
		pPOP3->Disconnect();
	}catch (_com_error e)
	{
		return FALSE;
	}
	return TRUE;
}

CString CForJmail::DownAtt(int index, CString Path, CString title)
{
	int i = 1;
	while(!ConnectToPOP3())
	{
		//AfxMessageBox("失败");
		if (pMainWnd->m_quit)
		{
			return "";
		}
		Sleep(i*60*1000);
		i ++;
	}
	//AfxMessageBox("pop连接成功");
	jmail::IMessagePtr pMessage = pPOP3->Messages->Item[index];
	if (pMessage->GetAttachments()->GetCount() == 0)
	{
		return "";
	}
	string attname = pMessage->GetAttachments()->GetItem(0)->GetName();
	string ExName = attname.substr(attname.find_last_of('.') + 1 ,attname.length() - attname.find_last_of('.') - 1);
	attname = (LPCSTR)title;
	string savepath;
	savepath = (LPCSTR)Path+attname;
	savepath = CEMailADO::GetSavePath(savepath);
	pMessage->GetAttachments()->GetItem(0)->SaveToFile(savepath.c_str());
	DisConnectToPOP3();
	return savepath.c_str();
}

int CForJmail::GetIndex(CString subject)
{
	int index = 0;
	int i = 1;
	while(!ConnectToPOP3())
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		Sleep(i*60*1000);
		//AfxMessageBox("失败");
		i ++;
	}
	//AfxMessageBox("pop连接成功");
	GetMailHead();
	jmail::IMessagesPtr pMessages = pPOP3->Messages;
	long lCount = pMessages->Count - 1;
	jmail::IMessagePtr pMessage;
	if(lCount != 0)
	{
		for(long i = 1; i <= lCount; i++)
		{
			pMessage = pMessages->Item[i];
			_bstr_t bstrSubject = pMessage->Subject;
			if (bstrSubject.length() == 0)
			{
				continue;
			}
			CString sub = _com_util::ConvertBSTRToString(bstrSubject);
			if (sub == subject)
			{
				index = i;
			}
		}
	}
	DisConnectToPOP3();
	return index;
}
