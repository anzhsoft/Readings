// CLientThread.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "CLientThread.h"
#include "PubInfo.h"
#include "UserCommDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char UserName[50];
extern char CurrentPath[_MAX_PATH];
extern char ShareFileFolder[_MAX_PATH];
/////////////////////////////////////////////////////////////////////////////
// CCLientThread


IMPLEMENT_DYNCREATE(CCLientThread, CWinThread)

CCLientThread::CCLientThread()
{
}

CCLientThread::~CCLientThread()
{
}

BOOL CCLientThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CCLientThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCLientThread, CWinThread)
	//{{AFX_MSG_MAP(CCLientThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLientThread message handlers

int CCLientThread::Run() 
{
	char buffer[BUFFER_SIZE] = {0};
	char package[BUFFER_SIZE] = {0};
	int packOffset = 0;
	int recvnum = 0;
	SendUserShareInfoQuery();
	while(!*m_pQuit){
		ZeroMemory(buffer, BUFFER_SIZE);
 		recvnum = UDT::recv(m_s, buffer, BUFFER_SIZE, 0);
 		if(UDT::ERROR==recvnum){
			break;
 		}
 		else if(0==recvnum)
 			continue;
 
 		if(packOffset+recvnum>=BUFFER_SIZE){
 			memcpy(package+packOffset, buffer, BUFFER_SIZE-packOffset);
 		}
 		else{
 			memcpy(package+packOffset, buffer, recvnum);
 			packOffset += recvnum;
 			continue;
 		}

		switch(package[0]){
		case state_comm:		//用户通信 文本
			m_Msg += m_UserName;
			m_Msg += ":\r\n";
			m_Msg += package+1;
			m_Msg += "\r\n";
			MessagePostToDialog(WM_P2P_COMM_DLG_COMM, package);
			break;
		case state_server_userinfo_query:
			SendShareInfo(package);
			break;
		}
		ZeroMemory(package,BUFFER_SIZE);
		memcpy(package, buffer+BUFFER_SIZE-packOffset,recvnum-(BUFFER_SIZE-packOffset));
		packOffset=recvnum-(BUFFER_SIZE-packOffset);
	}
	UDT::close(m_s);
	UDT::close(m_sl);
	m_pWnd->PostMessage(WM_CLIENTTHREAD_FINISHED,(WPARAM)m_s,(LPARAM)this);
	AfxEndThread(0);
	return 0;
}

void CCLientThread::MessagePostToDialog(UINT msg, LPCSTR buffer)
{
	LPSTR buf = (LPSTR)malloc(BUFFER_SIZE);
	ZeroMemory(buf, BUFFER_SIZE);
	memcpy(buf, buffer, BUFFER_SIZE);

	LPSTR UN = (LPSTR)malloc(m_UserName.GetLength()+1);
	ZeroMemory(UN, m_UserName.GetLength()+1);
	strcpy(UN, m_UserName);

	m_pWnd->PostMessage(msg, (WPARAM)buf, (LPARAM)UN);
}

BOOL CCLientThread::SendUserShareInfoQuery()
{
	char sendBuf[BUFFER_SIZE] = {0};
	sendBuf[0] = state_server_userinfo_query;

	char iniPath[_MAX_PATH] = {0};
	char szctrport[20] = {0};
	char sztranport[20] = {0};
	sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("client", "ctrport", NULL, szctrport, 20, iniPath);
	GetPrivateProfileString("client", "tranport", NULL, sztranport, 20, iniPath);
	STRUCTUSERINFO info = {0};
	strcpy(info.UserName, UserName);
	info.ctrPort = atoi(szctrport);
	info.tranPort = atoi(sztranport);

	memcpy(sendBuf+1, &info, sizeof(STRUCTUSERINFO));

	if(UDT::ERROR == UDT::send(m_s, sendBuf, BUFFER_SIZE, 0)){
#ifdef _DEBUG
		CString msg;
		msg.Format("UINT CCLientThread::_SendUserShareInfoQuery(LPVOID pParam)::send(): %d", WSAGetLastError());
		Tips(msg);
#endif
		return FALSE;
	}
	return TRUE;
}


void CCLientThread::SendShareInfo(LPSTR buffer) 
{
	
	STRUCTUSERINFO* info = (STRUCTUSERINFO*)(buffer+1);
	m_UserName = info->UserName;
	m_tranport = info->tranPort;

	//向服务器申请
	SEARCHINFO searchInfo = {0};
	strcpy(searchInfo.UserName, m_UserName);
	char sendBuf[BUFFER_SIZE] = {0};
	sendBuf[0] = state_server_search_info_request;
	memcpy(sendBuf+1, &searchInfo, sizeof(SEARCHINFO));
	CUserCommDlg* p = (CUserCommDlg*)m_pWnd;
	if(SOCKET_ERROR == send(p->serverSock, sendBuf, BUFFER_SIZE, 0)){
		CString msg;
		msg.Format("Failed In OnButtonSearch()::Send(): %d", WSAGetLastError());
//		Tips(msg);
		return;
	}

//	CPubInfo pubInfo(m_s,ShareFileFolder);
//	pubInfo.StartPubInfo();
}
