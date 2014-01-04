// ClientListenerThread.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ClientListenerThread.h"
#include "CLientThread.h"
#include "UserCommDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char CurrentPath[_MAX_PATH];
/////////////////////////////////////////////////////////////////////////////
// CClientListenerThread

IMPLEMENT_DYNCREATE(CClientListenerThread, CWinThread)

CClientListenerThread::CClientListenerThread()
{
}

CClientListenerThread::~CClientListenerThread()
{
}

BOOL CClientListenerThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CClientListenerThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CClientListenerThread, CWinThread)
	//{{AFX_MSG_MAP(CClientListenerThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientListenerThread message handlers

int CClientListenerThread::Run() 
{
	m_quit = FALSE;
	char iniPath[_MAX_PATH] = {0};
	char szctrport[20] = {0};
	sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("client", "ctrport", NULL, szctrport, 20, iniPath);
	int ctrport = atoi(szctrport);
	int err;
	UDTSOCKET sListen = UDT::socket(AF_INET,SOCK_STREAM, 0);
	if(UDT::ERROR == sListen){
#ifdef _DEBUG
		CString msg;
		msg.Format("CClientListenerThread()::Run()::socket() failed: %d", WSAGetLastError());
		Tips(msg);
#endif
		return -1;
	}
	sockaddr_in tcpaddr;
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = ::htons(ctrport);
	tcpaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(tcpaddr.sin_zero), '\0', 8);

	if(UDT::ERROR == UDT::bind(sListen, (struct sockaddr*)&tcpaddr, sizeof(tcpaddr))){
#ifdef _DEBUG
		CString msg;
		msg.Format("CClientListenerThread()::Run()::bind() failed: %d", UDT::getlasterror().getErrorCode());
		Tips(msg);
#endif
		UDT::close(sListen);
		return -1;
	}

	err = UDT::listen(sListen, SOMAXCONN);//Winsock2默认SOMAXCONN为5

	if(err==UDT::ERROR){
#ifdef _DEBUG
		CString msg;
		msg.Format("CClientListenerThread()::Run()::listen() failed: %d", WSAGetLastError());
		Tips(msg);
#endif
		UDT::close(sListen);
		return -1;
	}

	sockaddr_in clientaddr;
	while(!m_quit){
		int iAddrSize = sizeof(clientaddr);
		UDTSOCKET csock = UDT::accept(sListen, (SOCKADDR*)&clientaddr, &iAddrSize);	//客户端 socket
		if(csock == UDT::ERROR){
			break;
		}

		CCLientThread* cth = (CCLientThread*)AfxBeginThread(RUNTIME_CLASS(CCLientThread),
			THREAD_PRIORITY_NORMAL,	0, CREATE_SUSPENDED, NULL);		//默认挂起状态
		cth->m_pWnd = m_pWnd;
		cth->m_s = csock;
		cth->m_ip.Format("%s",inet_ntoa(clientaddr.sin_addr));
		cth->m_bAutoDelete = TRUE;				//Client中的run结束后自动结束线程

		CUserCommDlg* p = (CUserCommDlg*)m_pWnd;
		cth->m_pQuit = &(p->quit);
		p->socketList.AddTail(csock);
		p->threadList.AddTail(cth);
		cth->ResumeThread();				//执行线程
	}
	AfxEndThread(0);
	
	return 0;
}
