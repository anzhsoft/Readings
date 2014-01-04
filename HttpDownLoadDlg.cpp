// HttpDownLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "HttpDownLoadDlg.h"
#include "HttpSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHttpDownLoadDlg dialog


CHttpDownLoadDlg::CHttpDownLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHttpDownLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHttpDownLoadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	sock = NULL;
	quit = FALSE;
}


void CHttpDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHttpDownLoadDlg)
	DDX_Control(pDX, IDC_STATIC_TIP, m_ctrTip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHttpDownLoadDlg, CDialog)
	//{{AFX_MSG_MAP(CHttpDownLoadDlg)
	ON_MESSAGE(WM_CLOSE_DIALOG, OnCloseDialog)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpDownLoadDlg message handlers

BOOL CHttpDownLoadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWinThread* th = AfxBeginThread(_DownLoadThread, this);
	th->ResumeThread();

	nDot = 0;
	SetTimer(1, 1000, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

UINT CHttpDownLoadDlg::_DownLoadThread(LPVOID pParam)
{
	CHttpDownLoadDlg* p = (CHttpDownLoadDlg*)pParam;
	if(!p)
		return 1;

	if(p->DownLoadThread())
	{
		p->PostMessage(WM_CLOSE_DIALOG, IDOK);
	}
	else
	{
		p->PostMessage(WM_CLOSE_DIALOG, IDCANCEL);
	}

	AfxEndThread(0, TRUE);
	return 0;
}

BOOL CHttpDownLoadDlg::DownLoadThread()
{
	CHttpSocket HttpSocket;
	CFile DownloadFile;
	CString strServer,strObject;
	unsigned short nPort;
	DWORD dwServiceType;
	long nLength;
	const char *pRequestHeader = NULL;
	if(!AfxParseURL(m_URL,dwServiceType,strServer,strObject,nPort))
	{
		return FALSE;
	}
	try
	{
		pRequestHeader = HttpSocket.FormatRequestHeader((LPTSTR)(LPCTSTR)strServer,(LPTSTR)(LPCTSTR)strObject,nLength, nPort);	
		if(!HttpSocket.Socket())
			throw 0;

		if(quit){
			return FALSE;
		}
		sock = HttpSocket.GetSocket();
		if(!HttpSocket.Connect((LPTSTR)(LPCTSTR)strServer,nPort))
			throw 1;
		if(!HttpSocket.SendRequest())
			throw 1;
		HttpSocket.SetTimeout(10000,0);

		int nLineSize = 0;
		char szLine[256];
		while(nLineSize != -1)
			nLineSize = HttpSocket.GetResponseLine(szLine,256);

		int nReceSize = 0;
		char pData[1024];

		if(!DownloadFile.Open(m_Path, CFile::modeCreate|CFile::modeWrite))
			throw 2;

		char szValue[30];
		int ishttp = HttpSocket.GetField("Content-Length",szValue,30);
		int nCompletedSize = 0;

		if (ishttp == -1)//非Http，javaclass返回结果
		{
			nReceSize = HttpSocket.Receive(pData,1024);

			CString str,str2;
			str.Format("%s",pData);
			str2 = str.Left(str.Find('\n')+1);
			str.Replace(str2,"");
			memset(pData, 0, 1024);
			strcpy_s(pData, 1024, str);

			while(nReceSize != 0)
			{
				if(nReceSize == -1)
				{
					//throw 3;
					break;
				}
				DownloadFile.Write(pData,nReceSize);
				nReceSize = HttpSocket.Receive(pData,1024);	
			}
		}

/*		if (ishttp == -1)//非Http，javaclass返回结果
		{
			nReceSize = HttpSocket.Receive(pData,11);
			CString str,str2;
			str.Format("%s",pData);
			str2 = str.Left(str.Find('\n')+1);
			str.Replace(str2,"");
			sprintf(pData,str);
			int filelength = 0;
			filelength = atoi(pData);

			while(nCompletedSize < filelength)
			{
				nReceSize = HttpSocket.Receive(pData,1024);
				if(nReceSize == 0)
					break;
				if(nReceSize == -1)
					throw 3;
				DownloadFile.Write(pData,nReceSize);
				nCompletedSize += nReceSize;
			}
		}*/
		else
		{
			int nSvrState = HttpSocket.GetServerState();
			int nFileSize = atoi(szValue);
		
			while(nCompletedSize<nFileSize)
			{
				nReceSize = HttpSocket.Receive(pData,1024);
				if(nReceSize == 0)
					break;
				if(nReceSize == -1)
					throw 3;
				DownloadFile.Write(pData,nReceSize);
				nCompletedSize += nReceSize;
			}
		}
		DownloadFile.Close();
		HttpSocket.CloseSocket();
	}
	catch(int err){
		switch(err){
		case 3:
			DownloadFile.Close();
		case 2:

		case 1:
			HttpSocket.CloseSocket();
			break;
		}
		return FALSE;
	}
	return TRUE;
}

LRESULT CHttpDownLoadDlg::OnCloseDialog(WPARAM wParam, LPARAM lParam)
{
	KillTimer(1);
	EndDialog(wParam);
	return TRUE;
}

void CHttpDownLoadDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1){
		CString msg = "正在获取服务器信息，请稍候";
		for(int n=0; n<nDot; n++)
			msg += '.';
		m_ctrTip.SetWindowText(msg);
		nDot++;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CHttpDownLoadDlg::OnCancel() 
{
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	quit = TRUE;
	if(sock)
		::closesocket(sock);
}
