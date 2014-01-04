// UserCommDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "UserCommDlg.h"
#include "MainFrm.h"
#include "PubInfo.h"

//MultiMedia
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char AspServer[1024];
extern char ShareFileFolder[_MAX_PATH];
extern char CurrentPath[_MAX_PATH];
extern char UserName[50];
extern CMainFrame* pMainWnd;
extern _ConnectionPtr m_pConnection;
extern char hostInerIp[20];
extern int chatRoomID;
extern int chatRoomID1;
extern int chatRoomID2;
BOOL isChatroom1;
BOOL isChatroom2;//added by zhanganzhan to flag the chat room on 20090326
char strChatId[100];
CStdioFile shareFile;//zhanganzhan 1111
CStdioFile bookListFile;//zhanganzhan 090105
int fileNumCounter=0;//zhanganzhan 1118
int fileItemNumber =0;//zhanganzhan 1118
BOOL liveFlag=TRUE;// zhanganzhan 1119
CList<msgNode,msgNode&> msgList1;//added by zhanganzhan on 20090326 for chatrooms
CList<msgNode,msgNode&> msgList2;
CList<msgNode,msgNode&> msgList;

CUserCommDlg* pDlg;

/////////////////////////////////////////////////////////////////////////////
// CUserCommDlg dialog

CUserCommDlg::CUserCommDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserCommDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserCommDlg)
	//}}AFX_DATA_INIT
//	m_bShowList = TRUE;
	quit = FALSE;
	liveFlag = TRUE;//added by zhanganzhan on 20081119
	closeflag = 0;
	hFriend = NULL;
	hChatroom = NULL;//added by zhanganzhan on 20090325
	hGroup = NULL;
	MessageFlag = 0;
	MessageCount = 0;
	reconnectTimes = 0;
	reConnectionFlag = FALSE;
	isChatroom1 = TRUE;//added by zhanganzhan on 20090326
	isChatroom2 = FALSE;// 默认进第一聊天室
	memset(lastMessage,0,BUFFER_SIZE);
	try
	{
		char *p;
	//	strcpy(p,"asgw");
	}
	catch (CMemoryException* e)
	{
		
	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
	
}


void CUserCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserCommDlg)
	DDX_Control(pDX, IDC_BUTTON_MSGRECORD, m_msgRecord);
	DDX_Control(pDX, IDC_RICHEDIT_RECV, m_ctrRichR);
	DDX_Control(pDX, IDC_USER_TREE, m_tree);
	DDX_Control(pDX, IDC_EDIT_SEND, m_ctreSend);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_ctrSend);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ctrClose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserCommDlg, CDialog)
	//{{AFX_MSG_MAP(CUserCommDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_MESSAGE(WM_REQUEST_RESUOURCE_FROM_SERVER, OnRequestResource)
	ON_MESSAGE(WM_BEGIN_DOWNLOAD, OnDownLoadFile)
	ON_MESSAGE(WM_SERVER_ERROR, OnError)
	ON_MESSAGE(WM_ADD_TO_MYHALL,OnAddtoMyhall)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_NOTIFY(TVN_SELCHANGED, IDC_USER_TREE, OnSelchangedUserTree)
	ON_MESSAGE(WM_THREAD_FINISHED , OnThreadFinished)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CREATE_TALK_DLG,OnCreateTalkDlg)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_MSGRECORD, OnButtonMsgrecord)
	ON_NOTIFY(NM_CLICK, IDC_USER_TREE, OnClickUserTree)
	ON_NOTIFY(NM_RCLICK, IDC_USER_TREE, OnRclickUserTree)
	ON_MESSAGE(WM_SEND_CHAT_INFO1,OnChangeChatroom1)
	ON_MESSAGE(WM_SEND_CHAT_INFO2,OnChangeChatroom2)
	ON_MESSAGE(WM_SEND_CHAT_INFO,OnChangeChatroom)
	ON_MESSAGE(WM_USER_REFRESH,OnChangeUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserCommDlg message handlers

void CUserCommDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(m_tree.GetSafeHwnd())
		m_tree.MoveWindow(3, 0, cx-3*2, 300);

	if(m_ctrRichR.GetSafeHwnd())
		m_ctrRichR.MoveWindow(3, 303, cx-3*2, (int)((cy-303-25)*0.7));
	if(m_ctreSend.GetSafeHwnd())
		m_ctreSend.MoveWindow(3, 303+(int)((cy-303-25)*0.7), cx-3*2, (int)((cy-303-25)*0.3));
	if(m_ctrSend.GetSafeHwnd())
		m_ctrSend.MoveWindow(cx-3-60, cy-25, 60, 23);
	if(m_ctrClose.GetSafeHwnd())
		m_ctrClose.MoveWindow(cx-3-130, cy-25, 60, 23);
	if(m_msgRecord.GetSafeHwnd())
		m_msgRecord.MoveWindow(cx-3-130, cy-25, 60, 23);

	/*
	if(m_bShowList){
		if(m_userlist.GetSafeHwnd()){
			m_userlist.MoveWindow(3, 0, cx-3*2, 400-21);
			m_userlist.SetColumnWidth(0, cx-3*2);
		}
		if(m_grouplist.GetSafeHwnd()){
			m_grouplist.MoveWindow(3, 400-20, cx-3*2, 20);
			m_grouplist.SetColumnWidth(0, cx-3*2);
		}
	}
	else{
		if(m_userlist.GetSafeHwnd()){
			m_userlist.MoveWindow(3, 0, cx-3*2, 20);
			m_userlist.SetColumnWidth(0, cx-3*2);
		}
		if(m_grouplist.GetSafeHwnd()){
			m_grouplist.MoveWindow(3, 21, cx-3*2, 400-20);
			m_grouplist.SetColumnWidth(0, cx-3*2);
		}
	}
	*/
}

BOOL CUserCommDlg::OnInitDialog() 
{
	pDlg = this;
	CDialog::OnInitDialog();

try{
	InitialTree();
//	InitialList();

	m_ctrSend.EnableWindow(TRUE);
	m_ctrClose.EnableWindow(FALSE);
	m_msgRecord.EnableWindow(TRUE);
	/*RichEdit*/
	m_ctrRichR.SetBackgroundColor(FALSE,0xffffff);

	cf_name.cbSize = sizeof(CHARFORMAT); 
	cf_name.dwMask = CFM_BOLD|CFM_COLOR|CFM_FACE|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE; 
	cf_name.dwEffects = 0; 
	cf_name.yHeight = 180;//文字高度 
	cf_name.crTextColor = RGB(0, 128, 0); //文字颜色 
	strcpy(cf_name.szFaceName ,_T("宋体"));//设置字体 
	cf_msg.cbSize = sizeof(CHARFORMAT); 
	cf_msg.dwMask = CFM_BOLD|CFM_COLOR|CFM_FACE|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE; 
	cf_msg.dwEffects = 0; 
	cf_msg.yHeight = 210;//文字高度 
	cf_msg.crTextColor = RGB(0, 0, 0xff); //文字颜色 
	strcpy(cf_msg.szFaceName ,_T("宋体"));//设置字体 

	//zhanganzhan 090814 for change charroom flag.
	cf_flag.cbSize = sizeof(CHARFORMAT);
	cf_flag.dwMask = CFM_BOLD|CFM_COLOR|CFM_FACE|CFM_ITALIC|CFM_SIZE|CFM_UNDERLINE;
	cf_flag.dwEffects = 0;
	cf_flag.yHeight = 230;
	cf_flag.crTextColor = RGB(128,0,0);
	strcpy(cf_flag.szFaceName, _T("楷体"));

	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_OFFSETINDENT;//|PFM_STARTINDENT;
	pf.dxStartIndent = 100;

	font.CreateFont(15, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体")); // lpszFac			Monotype Corsiva

	m_ctreSend.SetFont(&font);

 	if (strcmp(UserName,"Guest_Default")==0 ||strcmp(UserName,"Guest")==0)
 	{
 		return TRUE;
 	}
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_Initial,(LPVOID)this,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
	m_thlist.AddTail(handle);
}
catch(...){
#ifdef _DEBUG
	Tips("BOOL CUserCommDlg::OnInitDialog() ");
#endif
}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

unsigned __stdcall CUserCommDlg::_Initial(LPVOID pParam)
{
	CUserCommDlg* p = (CUserCommDlg*)pParam;//Modified by zhanganzhan on 20081120
	//return 0;
try{

	
	if(!p->InitSevConn())
//added by zuoss 20081009
	{
		//读本地数据库access获得聊天记录，显示
//		p->GetOutlineRecord();//deleted by zhanganzhan on 20090305
	
		return 0;
	}
//end add
	// added by zhanganzhan on 20090316 for web tech.


 	p->hGetMsg = (HANDLE)_beginthreadex(NULL,0,_GetMessage,(LPVOID)p,CREATE_SUSPENDED,NULL);
 	ResumeThread(p->hGetMsg);
 	p->m_thlist.AddTail(p->hGetMsg);

    HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_Listen,(LPVOID)p,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
	p->m_thlist.AddTail(handle);
	p->SetTimer(1, 30*1000, NULL);
	/*
	CClientListenerThread* cth = (CClientListenerThread*)AfxBeginThread(RUNTIME_CLASS(CClientListenerThread),
		THREAD_PRIORITY_NORMAL,	0, CREATE_SUSPENDED, NULL);		//默认挂起状态
	cth->m_pWnd = p;
	cth->m_bAutoDelete = TRUE;				//Client中的run结束后自动结束线程
	cth->ResumeThread();					//执行线程
	*/
	
//	CPubInfo pubInfo(ShareFileFolder);
//	pubInfo.StartPubInfo(pMainWnd->m_quit);

// 注释 by 董振华1016 测试用
// 	p->PubInfo();
}
catch(...){
#ifdef _DEBUG
	Tips("unsigned __stdcall CUserCommDlg::_Initial(LPVOID pParam)");
#endif
}
	closesocket(p->fileSock);//added by zhanganzhan on 20081120
	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	_endthreadex(0);
	return 0;  //deleted by zhanganzhan
}
unsigned __stdcall CUserCommDlg::_ReConnectionInitial(LPVOID pParam)
{
	CUserCommDlg* p = (CUserCommDlg*)pParam;//Modified by zhanganzhan on 20081120
try{
	
	if(!p->InitSevConn())
//added by zuoss 20081009
	{
		//读本地数据库access获得聊天记录，显示
//		p->GetOutlineRecord();//deleted by zhanganzhan on 20090305
		return 0;
	}
//end add
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_Listen,(LPVOID)p,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
	p->m_thlist.AddTail(handle);
	p->SetTimer(1, 30*1000, NULL);

}
catch(...){
#ifdef _DEBUG
	Tips("unsigned __stdcall CUserCommDlg::_Initial(LPVOID pParam)");
#endif
}
	closesocket(p->fileSock);//added by zhanganzhan on 20081120
	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	_endthreadex(0);
	return 0;
}
BOOL CUserCommDlg::InitSevConn()
{
try{
	//set global variable
	char iniPath[_MAX_PATH] = {0};
	char serverIP[50] = {0};
	char szport[20] = {0};
	char szctrport[20] = {0};
	char sztranport[20] = {0};
	sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("server", "ip", NULL, serverIP, 50, iniPath);
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}
/*//zuoss 20081217
	CString sIPAddress;
	struct hostent FAR *lpHostEnt=gethostbyname("readings.vicp.net");
	if(lpHostEnt == NULL)
	{
		sIPAddress = _T("");
	}
	LPSTR lpaddr = lpHostEnt->h_addr_list[0];
	if(lpaddr)
	{
		struct in_addr inAddr;
		memmove (&inAddr,lpaddr, 4);
		sIPAddress=inet_ntoa (inAddr);
		if(sIPAddress.IsEmpty())
			sIPAddress=_T("Not available");
	} 
	strcpy(serverIP,sIPAddress);
//	AfxMessageBox(sIPAddress);
//end*/
	GetPrivateProfileString("server", "port", NULL, szport, 20, iniPath);
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}
	GetPrivateProfileString("client", "ctrport", NULL, szctrport, 20, iniPath);
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}
	GetPrivateProfileString("client", "tranport", NULL, sztranport, 20, iniPath);
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}

	int sevport = atoi(szport);
	int ctrport = atoi(szctrport);
	int tranport = atoi(sztranport);
//
	serverSock = socket(AF_INET,SOCK_STREAM, 0);
	if(serverSock == INVALID_SOCKET){
		CString msg;
		msg.Format("socket() failed:%d", WSAGetLastError());
		Tips(msg);
		return FALSE;
	}
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverIP);
	server.sin_port = htons(sevport);
	memset(&(server.sin_zero), '\0', 8);

	//连接服务器 delete by dzh 20081014
// 	if(connect(serverSock, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR){
// 		CString msg;
// 		msg.Format("connect() failed:%d", WSAGetLastError());
// 		Tips(msg);
// 		return 0;
//	}

	//modified by zhanganzhan on 20090303 for re-connection every 5 mins
	//add by dzh 20081014
	static int connectCount = 0;		//标识连接次数，以区分是否是程序初始时网络故障
	while (this->quit == FALSE&&(connect(serverSock, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR))
	{
		if (pMainWnd->m_quit)
		{
			return FALSE;
		}
		reconnectTimes++;
		reConnectionFlag = TRUE;
		//为了调用本地缓存，对首次登陆的网络故障进行特殊处理
	   	if (connectCount==0)
		{
			connectCount++;
			//Tips("首次登陆失败");
		//	AfxMessageBox("首次登陆失败");
// 			quit = TRUE;
// 			closeflag =1;
		//	return FALSE;
		}
		
		
	//	AfxMessageBox("尝试再次登陆");
		closesocket(serverSock);
		serverSock = socket(AF_INET,SOCK_STREAM, 0);	
		if(serverSock == INVALID_SOCKET){
			CString msg;
			msg.Format("socket() failed:%d", WSAGetLastError());
			Tips(msg);
//			quit = TRUE;
//			closeflag = 1;
			return FALSE;
		}
		int tmp = 0;
		while (tmp < 60)
		{
			if (pMainWnd->m_quit)
			{
				return FALSE;
			}
			Sleep(1000);
		}
		//Sleep(60*1000);//added by zhanganzhan on 20090303 for re-connection every 5mins
		Tips("您与服务器的连接已经断开，系统正尝试重新连接",td_msg,2);	
	}
	connectCount++;
// 	if (connectCount>=5)
// 	{
// 		CString msg;
//  		msg.Format("connect() failed:%d", WSAGetLastError());
//  		Tips(msg);
//  		return FALSE;
// 	}

	//登录
	char buf[BUFFER_SIZE] = {0};
	buf[0] = state_login_r;      
	STRUCTLOGIN logInfo;
	sprintf(logInfo.UserName, UserName);
	logInfo.ctrPort = ctrport;
	logInfo.tranPort = tranport;

	//获得本机IP
	char computename[155]={0};
	char username[28]={0};
	PHOSTENT hostinfo; 
	if(gethostname(computename, sizeof(computename)) == 0) { 
		if((hostinfo = gethostbyname(computename)) != NULL) { //这些就是获得IP的函数
			strcpy(logInfo.IP ,inet_ntoa (*(in_addr *)*hostinfo->h_addr_list)); 
			strcpy(hostInerIp,logInfo.IP);
		} 
	} 

	memcpy(buf+1, &logInfo,sizeof(STRUCTLOGIN));   
	if(SOCKET_ERROR == send(serverSock, buf, BUFFER_SIZE, 0)){
#ifdef _DEBUG
		CString msg;
		msg.Format("send() failed:%d", WSAGetLastError());
		Tips(msg);
#endif
		return TRUE;
	}
    if (pMainWnd->m_quit)
	{
		return FALSE;
	} 
	ZeroMemory(buf,BUFFER_SIZE);
	if(SOCKET_ERROR == recv(serverSock, buf, BUFFER_SIZE, 0)){
#ifdef _DEBUG
		CString msg;
		msg.Format("recv() failed:%d", WSAGetLastError());
		Tips(msg);
#endif
		return TRUE;
	}
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}
	//AfxMessageBox(buf);
}
catch(...){
#ifdef _DEBUG
	Tips("BOOL CUserCommDlg::InitSevConn()");
#endif
}
	m_slist.AddTail(serverSock);
	return TRUE;
}

unsigned __stdcall CUserCommDlg::_Listen(LPVOID pParam)
{
	CUserCommDlg* p = (CUserCommDlg*)pParam;
	p->Listen();
	p->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	_endthreadex(0);
	return 0;  //deleted by zhanganzhan
}
// added by zhanganzhan on 20081204
void CUserCommDlg::holeTest(char *packet)
{

//	sockaddr_in tempAddr = (sockaddr_in)(packet+1);
	Tips("step into here.");
	char tempIP[BUFFER_SIZE];
	int port=0;
	ZeroMemory(tempIP,BUFFER_SIZE);
	strncpy(tempIP,packet+1,20);
	Tips(tempIP,td_alert,5);
	Tips("haah",td_alert,5);
	char buf[10];
	ZeroMemory(buf,10);
	strncpy(buf,packet+22,10);
	port =atoi(buf);

	SOCKET udpSock;
	sockaddr_in UDPAddr;
	UDPAddr.sin_family=AF_INET;
	UDPAddr.sin_port=htons(port);
	UDPAddr.sin_addr.S_un.S_addr=inet_addr(tempIP);
	Tips(tempIP,td_msg,5);
	udpSock = socket(AF_INET,SOCK_DGRAM,0);
	int i=5;
	while(i--)
	{	
		sendto(udpSock,tempIP,BUFFER_SIZE,0,(sockaddr *)&UDPAddr,sizeof(UDPAddr));
	    Sleep(10);
	}
}

//added by zhanganzhan for web-chatroom on 20090310
unsigned __stdcall CUserCommDlg::_GetMessage(LPVOID pParam)
{
	while(chatRoomID1 == MAX_CHATROOM_ID)
	{
		if (pMainWnd->m_quit)
		{
			//pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
			return 0;
		}
		Sleep(500);
	}

//	AfxMessageBox("here");
	int msgID=0;
	int chatID=0;
	int flag =0;
	int iniFlag =0;
	int interval =1;
	int pos =0;
	CString id;
	CString temp;
	CString name;
	CString time;
	CString content;
	CString dstPath;
	CString url;
	char msg[BUFFER_SIZE];
	char tmpBuf[50];

	CUserCommDlg *p = (CUserCommDlg*)pParam;

	flag =0;//标记是否是第一次接收信息
	msgID = 0;
	while (pDlg->quit == FALSE)
	{	
		interval=1;
		
		url.Format("%s/Msg/getMsg.asp?id=%d", AspServer, msgID);//,chatRoomID);

	    dstPath.Format("%s\\msg.xml", CurrentPath);

	    if(!DownLoad(url,dstPath,pDlg->quit,pDlg->m_slist,pDlg->m_mutex))
		    return 1;
		if (pMainWnd->m_quit)
		{
			//pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
			return 0;
		}
        CTime tm = CTime::GetCurrentTime();
//  	  sprintf(buf+1, "%s %02d-%02d-%02d %02d:%02d:%02d", UserName, tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(), tm.GetMinute(), tm.GetSecond());
//  	  strcpy(buf+2+strlen(buf+1), (LPCSTR)msg);
		
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		try
		{
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
			{
				throw 1;
			}
			if(!pDoc->load((LPCSTR)dstPath))
			{
				throw 2;	//xml 解析失败
			}
			pNodeList = pDoc->selectNodes("msgList/msg");
			if(pNodeList)
			{		
				
				for(pos=0; pos<pNodeList->length ; pos++)
				{
					if (pMainWnd->m_quit)
					{
						//pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
						return 0;
					}
					ZeroMemory(msg,BUFFER_SIZE);
					id =      GetXmlValue(pNodeList, "id",      pos);
					interval+=interval;
					name =    GetXmlValue(pNodeList, "name",    pos);		
					time =    GetXmlValue(pNodeList, "time",    pos);		
					content = GetXmlValue(pNodeList,  "content",pos);
					msgID = atoi(id);
					temp = GetXmlValue(pNodeList,"chatRoomID",pos);
					if(temp.Find('_') == -1)
					{
						chatID = atoi(temp);
						if (strcmp(name,UserName)==0 && flag!=0)
						{
							continue;//该用户的信息在提交发送时已经送显
						}
						
						sprintf(tmpBuf,"%s",time);					
						if (flag ==0)
						{
							sprintf(msg,"%s %s",name,tmpBuf+5);
						}
						else		
							sprintf(msg,"%s %s",name,tmpBuf+5);
						strcpy(msg+2+strlen(msg+1), (LPCSTR)content);			
						
						
						msgNode node ={0};
						strcpy(node.content,content);
						strcpy(node.name,name);
						strcpy(node.time,time);
						strcpy(node.chatroomID,temp);
						//AfxMessageBox(CString(node.chatroomID)+CString(node.content));
						// 暂时聊天室内容完全一直，因为新注册的用户没有分配聊天室的号码
						if (chatID==chatRoomID1)
						{
							
							if (msgList1.GetCount()>15)
							{
								//	   AfxMessageBox("Remove 1");
								msgList1.RemoveHead();
									
							}
							msgList1.AddTail(node);
					//		AfxMessageBox(CString(node.chatroomID)+CString(node.content));
							
						}
						else if (chatID==chatRoomID2)
						{
							if (msgList2.GetCount()>15)
							{
								msgList2.RemoveHead();
								
							}
							msgList2.AddTail(node);
							//AfxMessageBox(CString(node.chatroomID)+CString(node.content));
						}	
						
						if(p->RecordMsg(msg)<=0 &&flag !=0)//&&chatID == chatRoomID)//新消息到来
						{
							p->ReadMsg(msg);
							
						}				
						
						if (flag!=0)
						{
							//	Sleep(interval*100);
							if (pDlg->quit == TRUE)
							{
								break;
							}
						}
					}else
					{
						if (strcmp(name,UserName)==0 && flag!=0)
						{
							continue;//该用户的信息在提交发送时已经送显
						}
						
						sprintf(tmpBuf,"%s",time);					
						if (flag ==0)
						{
							sprintf(msg,"%s %s",name,tmpBuf+5);
						}
						else		
							sprintf(msg,"%s %s",name,tmpBuf+5);
						strcpy(msg+2+strlen(msg+1), (LPCSTR)content);			
						
						
						msgNode node ={0};
						strcpy(node.content,content);
						strcpy(node.name,name);
						strcpy(node.time,time);
						strcpy(node.chatroomID,temp);
						//AfxMessageBox(CString(node.chatroomID)+CString(node.content));
						//if (msgList.GetCount()>15)
						//{
						//	msgList.RemoveHead();
						//}
						msgList.AddTail(node);
						if(p->RecordMsg(msg)<=0 &&flag !=0)//&&chatID == chatRoomID)//新消息到来
						{
							p->ReadMsg(msg);
							
						}				
						
						if (flag!=0)
						{
							//	Sleep(interval*100);
							if (pDlg->quit == TRUE)
							{
								break;
							}
						}
					}
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(_com_error& e)
		{
			
		}
		//	break;
		

		if (flag==0)
		{
			
			msgNode node;
		
			p->m_ctrRichR.SetSel(0,-1);
			p->m_ctrRichR.ReplaceSel("");//清屏
			p->ChangeChatRoom("欢迎来到锐客好友聊天室");
		//	p->m_ctrRichR.SetModify(FALSE);

			POSITION tpos = msgList1.GetHeadPosition();
			while (tpos)
			{
				node = msgList1.GetNext(tpos);
				sprintf(tmpBuf,"%s",node.time);					
				sprintf(msg,"%s %s",node.name,tmpBuf+5);
				strcpy(msg+2+strlen(msg+1), (LPCSTR)node.content);	
				//AfxMessageBox(CString(node.chatroomID)+CString(node.content));
				p->ReadMsg(msg);	
				
			}
		//	p->m_ctrRichR.SetModify(TRUE);
		}
		
		flag++;
#if 0
		if (interval<=8)
		{
			int tmp = 0;
			while (!pMainWnd->m_quit && tmp < 30)
			{
				tmp ++;
				Sleep(1000);
			}
			//Sleep(TIME_30_SECOND);
		}
#endif 
		Sleep(5000);

	}

	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	
	return 0;
}
void CUserCommDlg::Listen()
{
try{
	char buffer[BUFFER_SIZE] = {0};
	char package[BUFFER_SIZE] = {0};
	int packOffset = 0;
	int msgCont=0;//added by zhanganzhan on 20090309 for reconnection.
	int recvnum = 0;

	while(!quit){

		ZeroMemory(buffer, BUFFER_SIZE);
 		recvnum = recv(serverSock, buffer, BUFFER_SIZE, 0);
		if(SOCKET_ERROR == recvnum){
			
//////////////////////////////////////////////////////////////////////////
// 			int flag=reConnectServer();
// 			if (flag>=10 || flag<0)
// 			{
// 				CString msg;
//  				msg.Format("connect() failed:%d", WSAGetLastError());
//  				Tips(msg);
//  				if(!quit){
//  					Tips("您与服务器的连接已经断开，请重新登录！");
// 					quit = TRUE;
// 				}
// 			//	continue;
// 			}
// 
//			continue;
//delete by dzh 20081014
			if(!quit){
 		 				Tips("您与服务器的连接已经断开，请重新登录！");
 		 		 				quit = TRUE;
								closeflag = 1;
 		 	}
			continue;
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

		char* tmpBuf;
		switch(package[0]){
		case state_userinfo_append:
		//	MessageToMainWnd(WM_P2P_APPENDUSERINFO,package);

			// begin added by yangxiao 2008.8.11
			pMainWnd->m_commLog.setLogItem("用户信息追加",sizeof(package), down,comm_oper_appendUser);
			pMainWnd->m_commLog.writeLogItem();
			// end added by yangxiao 2008.8.11

			break;
		case state_userinfo_delete:
		//	MessageToMainWnd(WM_P2P_DELETEUSERINFO,package);
			// begin added by yangxiao 2008.8.11
			pMainWnd->m_commLog.setLogItem("用户信息删除",sizeof(package), down,comm_oper_delUser);
			pMainWnd->m_commLog.writeLogItem();
			// end added by yangxiao 2008.8.11

			break;
		case state_userinfo_detail_answer:
			//SaveUserInfoDetail(package);
			break;
		case state_server_search_info_result:
			/*
			tmpBuf = (char*)malloc(BUFFER_SIZE);
			memcpy(tmpBuf, package, BUFFER_SIZE);
			PostMessage(WM_P2P_APPEND_SHARE_LIST, (WPARAM)tmpBuf);
			*/
			break;
		case start_peer_one_resource_response:
			tmpBuf = (char*)malloc(sizeof(SEARCHRES));
			memcpy(tmpBuf, package+1, sizeof(SEARCHRES));
			PostMessage(WM_BEGIN_DOWNLOAD,(WPARAM)tmpBuf);
			
			// begin added by yangxiao 2008.8.11
			pMainWnd->m_commLog.setLogItem("服务器端返回所请求的资源结构体标示",sizeof(package), down,comm_oper_server_response_stru);
			pMainWnd->m_commLog.writeLogItem();
			// end added by yangxiao 2008.8.11

			break;
		case start_peer_one_resource_response_no:
			tmpBuf = (char*)malloc(BUFFER_SIZE-1);
			memcpy(tmpBuf, package+1, BUFFER_SIZE-1);
			PostMessage(WM_SERVER_ERROR, (WPARAM)tmpBuf);

			// begin added by yangxiao 2008.8.11
			pMainWnd->m_commLog.setLogItem("服务器端返回的资源结构体标示失败",sizeof(package), down,comm_oper_server_response_stru_no);
			pMainWnd->m_commLog.writeLogItem();
			// end added by yangxiao 2008.8.11
			
			break;
		case state_peer_send_request_info:
			MessageToMainWnd(WM_SENDFILE,package);
			break;
		case state_peer_comm_request_info:
			tmpBuf = (char*)malloc(BUFFER_SIZE-1);
			memcpy(tmpBuf, package+1, BUFFER_SIZE-1);
			PostMessage(WM_COMM_CONNECT, (WPARAM)tmpBuf);
			break;
		case state_groupinfo_append:
			MessageToMainWnd(WM_APPENDGROUPINFO, package);
			break;
// 		case state_p2p_request_response://zhanganzhan for p2p holing 20081204
//             holeTest(package);	
// 			break;
		case state_comm:
		//只有当本地数据中没有该条message时，才显示
		//	if(RecordMsg(package+1)<=0)
			CString name;
			name.Format("%s\r\n", package+1);
			int lpos = name.Find(' ');
			name = name.Left(lpos);
		//MessageFlag 标示本条信息是否来自本地，若来自本地，表示已经送显，则不显示
// 			if(MessageFlag==0){
// 			
// 					ReadMsg(package+1);
// 			    	memset(lastMessage,0,BUFFER_SIZE);
// 				    strcpy(lastMessage,package+1);
//			}
// 			else
// 				MessageFlag = 0;
			//change by dzh 0730
			MessageCount++;
// 			if(reConnectionFlag == TRUE)
// 			{
// 				msgCont++;
// 				if(msgCont == 15)
// 				{
// 					reConnectionFlag = FALSE;
// 				}
// 				break;
// 
// 			}
			//deleted by zhanganzhan for chat by web access.20090318
// 
// 			if (MessageCount>15&&FALSE)//重叠完毕，则开始判断数据库中记录
// 			{
// 				if(RecordMsg(package+1)<=0)
// 				{
// 					ReadMsg(package+1);
// 				}
// 			}
// 			else{//前十五条聊天记录入库并送显
// 				if(reconnectTimes ==0&&FALSE)//added by zhanganzhan to delete the duplied msgs.20090305
// 				{
// 					RecordMsg(package+1);
// 				    ReadMsg(package+1);
// 				}			
// 			}
				break;
		}
		ZeroMemory(package,BUFFER_SIZE);
		memcpy(package, buffer+BUFFER_SIZE-packOffset,recvnum-(BUFFER_SIZE-packOffset));
		packOffset=recvnum-(BUFFER_SIZE-packOffset);

		// begin added by yangxiao 2008.8.14
		pMainWnd->m_commLog.setLogItem("接收聊天消息",sizeof(package), down,comm_oper_recvMsg);
		pMainWnd->m_commLog.writeLogItem();
		// end added by yangxiao 2008.8.14


	}
}
catch(_com_error& e){
	AfxMessageBox(e.ErrorMessage());
#ifdef _DEBUG
	Tips("void CUserCommDlg::Listen()");
#endif
}
}

void CUserCommDlg::MessageToMainWnd(UINT msg, LPCSTR buffer)
{
try{
	char* buf = (char*)malloc(BUFFER_SIZE);
	ZeroMemory(buf, BUFFER_SIZE);
	memcpy(buf, buffer, BUFFER_SIZE);
	try{
	pMainWnd->PostMessage(msg, (WPARAM)buf);
	}
	catch(...){
		if(pMainWnd==NULL)
			Tips("NULL");
		else
			Tips("error");
	}
}
catch(...){
#ifdef _DEBUG
	Tips("void CUserCommDlg::MessageToMainWnd(UINT msg, LPCSTR buffer)");
	CString smsg;
	smsg.Format("msg : %d", msg);
	Tips(smsg);
#endif
}
}

void CUserCommDlg::OnDestroy() 
{
	KillTimer(1);
	quit = TRUE;
	liveFlag = FALSE;//anzhsoft pro1.
//	closesocket(serverSock);
	::closesocket(heartSock);
	TernminateAllThread(m_slist,m_thlist);

	font.DeleteObject();

	CDialog::OnDestroy();
}

void CUserCommDlg::OnButtonSend() 
{
	CString msg;
	CString url;
	msgNode node ={0};
	char time[50];
	m_ctreSend.GetWindowText(msg);

	if(msg.IsEmpty()){
		Tips("不能发送空消息",td_alert,1);
		return;
	}
	if (msg.GetLength()>950)
	{
		Tips("聊天内容超出了最大字数限制",td_alert,1);
		return;
	}
	char buf[BUFFER_SIZE] = {0};
	buf[0] = state_comm;
	strcpy(node.content,msg);

	CTime tm = CTime::GetCurrentTime();
 	sprintf(buf+1, "%s %02d-%02d %02d:%02d:%02d", UserName, tm.GetMonth(),tm.GetDay(),tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	strcpy(buf+2+strlen(buf+1), (LPCSTR)msg);

	//先送显在本地
	if(RecordMsg(buf+1)<=0)
		ReadMsg(buf+1);
	//added by zhanganzhan on 20090311 to chat by web access.
	if (isChatroom1 || isChatroom2)
	{
		url.Format("%s/Msg/updateMsg.asp?name=%s&content=%s&chatRoomID=%d", AspServer, UserName,msg,chatRoomID);
	}
	else
	{
		url.Format("%s/Msg/updateMsg.asp?name=%s&content=%s&chatRoomID=%s", AspServer, UserName,msg,strChatId);
		//strChatId.
	}

	
	UpLoad(url);
//	AfxMessageBox(url);	
	 
	 strcpy(node.name,UserName);
	 sprintf(time,"%02d-%02d %02d:%02d:%02d",tm.GetMonth(),tm.GetDay(),tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	 strcpy(node.time,time);
	 if (isChatroom1)
	 {
		 msgList1.AddTail(node);
	 }
     else if (isChatroom2)
	 {
		 msgList2.AddTail(node);
	 }	
	 else
	 {
		 strcpy(node.chatroomID,strChatId);
		 msgList.AddTail(node);
	 }

	MessageFlag = 1;

	m_ctreSend.SetWindowText("");
	m_ctreSend.SetFocus();

	// begin added by yangxiao 2008.8.14
	pMainWnd->m_commLog.setLogItem("发送聊天消息",sizeof(buf), up,comm_oper_sentMSg);
	pMainWnd->m_commLog.writeLogItem();
	// end added by yangxiao 2008.8.14
	
}

LRESULT CUserCommDlg::OnRequestResource(WPARAM reserve, long sid)
{
	char sendBuf[BUFFER_SIZE] = {0};
	sendBuf[0]=state_peer_one_resource_request;
	REQUESTONERESOURCE* info = (REQUESTONERESOURCE*)(sendBuf+1);
	info->sid = sid;
	if(SOCKET_ERROR == send(serverSock, sendBuf, BUFFER_SIZE, 0))
	{
#ifdef _DEBUG
		Tips("LRESULT CUserCommDlg::OnRequestResource(WPARAM reserve, long sid)::send()");
#endif
	}
	return TRUE;
}

CString CUserCommDlg::GetSavePath(CString filename)
{
	CString filePath = filename;
	filePath.Replace('\\', '/');
	int index = filePath.ReverseFind('/');
	if(index==filePath.GetLength()-1){
		Tips("文件名错误！");
		return "";
	}
	else if(index>=0)
		filename = filePath.Right(filePath.GetLength()-index-1);
	else
		filename = filePath;
	
	CFileDialog fileDialog(FALSE,NULL, filename);
	if(fileDialog.DoModal()!=IDOK)
		return "";

	return fileDialog.GetPathName();
}

LRESULT CUserCommDlg::OnDownLoadFile(WPARAM w,LPARAM l)
{
	SEARCHRES *info = (SEARCHRES *)w; 
	CString savePath = GetSavePath(info->FileName);
	if(savePath.IsEmpty()){
		free(info);
		return TRUE;
	}
	DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
	ZeroMemory(missionInfo, sizeof(DownLoadMissionInfo));
	strcpy(missionInfo->UserName, info->UserName);
	strcpy(missionInfo->title, info->FileName);//need modified
	strcpy(missionInfo->IP, info->IP);
	strcpy(missionInfo->URL, info->FileName);
	strcpy(missionInfo->MD5, info->MD5);
	strcpy(missionInfo->DstPath, (LPCSTR)savePath);
	missionInfo->fileType = info->FileType;
	missionInfo->port = info->tranport;
	missionInfo->fileLen = info->nFileSize;
	//missionInfo->sourceType = dl_p2p;
	missionInfo->sourceType = dl_bt;
	pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
	//下载纪录
	char sendBuf[BUFFER_SIZE] = {0};
	sendBuf[0] = state_resource_download_times;
	DOWNLOADREPORT* report =  (DOWNLOADREPORT*)(sendBuf+1);
	report->sid = info->sid;
	if(SOCKET_ERROR == send(serverSock,sendBuf, BUFFER_SIZE, 0)){
#ifdef _DEBUG
		Tips("Error In state_resource_download_times");
#endif
	}
	free(info);
	return TRUE;
}
LRESULT CUserCommDlg::OnAddtoMyhall(WPARAM w,LPARAM l)
{
	TreeItemInfo* info = (TreeItemInfo*)w;
	HTREEITEM h_item =(HTREEITEM)l; 
	CString name = m_tree.GetItemText(h_item);
	if (info->Type==Is_School)
	{
		HTREEITEM hCurItem1 = m_tree.InsertItem(name, 29,29 , hMyStudyhall);
		m_tree.SetTreeItemData(hCurItem1, Is_School, NULL, 0, info->BookNumber);
		GetBuilding(hCurItem1);
	}
	if (info->Type == Is_StudyBuilding)
	{
		HTREEITEM hCurItem1 = m_tree.InsertItem(name, 30,30 , hMyStudyhall);
		m_tree.SetTreeItemData(hCurItem1, Is_StudyBuilding, NULL, 0, info->BookNumber);
		GetStudyHall(hCurItem1);
	}
	if (info->Type == Is_Hall)
	{
		HTREEITEM hCurItem1 = m_tree.InsertItem(name, 31,31 , hMyStudyhall);
		m_tree.SetTreeItemData(hCurItem1, Is_Hall, NULL, 0, info->BookNumber);
	//	GetBuilding(hCurItem1);
	}
	return TRUE;
}
LRESULT CUserCommDlg::OnError(WPARAM w,LPARAM l)
{
	LPSTR errorMsg = (LPSTR)w;
	Tips(errorMsg);
	free(errorMsg);
	return TRUE;
}

int CUserCommDlg::DipHoleForComm(LPCSTR UN)
{
	SOCKET dipHole = ::socket(AF_INET, SOCK_DGRAM, 0);
	if(dipHole == INVALID_SOCKET)
		return -1;
	
	char iniPath[_MAX_PATH] = {0};
	char serverIP[50] = {0};
	char szheartport[20] = {0};
	char szctrport[20] = {0};
	sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
	GetPrivateProfileString("server", "ip", NULL, serverIP, 50, iniPath);
/*//zuoss 20081217
	CString sIPAddress;
	struct hostent FAR *lpHostEnt=gethostbyname("readings.vicp.net");
	if(lpHostEnt == NULL)
	{
		sIPAddress = _T("");
	}
	LPSTR lpaddr = lpHostEnt->h_addr_list[0];
	if(lpaddr)
	{
		struct in_addr inAddr;
		memmove (&inAddr,lpaddr, 4);
		sIPAddress=inet_ntoa (inAddr);
		if(sIPAddress.IsEmpty())
			sIPAddress=_T("Not available");
	} 
	strcpy(serverIP,sIPAddress);
	//AfxMessageBox(sIPAddress);
//end*/
	GetPrivateProfileString("server", "heartport", NULL, szheartport, 20, iniPath);
	GetPrivateProfileString("client", "ctrport", NULL, szctrport, 20, iniPath);

	int ctrport = atoi(szctrport);
	int retryTimes = 0;

	while(retryTimes<10){
		sockaddr_in local = {0};
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = ADDR_ANY;
		local.sin_port = htons(ctrport);
		if(SOCKET_ERROR != ::bind(dipHole, (SOCKADDR*)&local, sizeof(local))){
			break;
		}
		retryTimes++;
		ctrport++;
	}

	if(retryTimes>=10){
#ifdef _DEBUG
		Tips("bind() error");
#endif
		::closesocket(dipHole);
		return -1;
	}
	
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverIP);
	server.sin_port = htons(atoi(szheartport));
	memset(&(server.sin_zero), '\0', 8);

	char SendBuf[BUFFER_SIZE] ={0};
	SendBuf[0] = state_peer_comm_request;
	SEARCHRES* res = (SEARCHRES*)(SendBuf+1);
	strcpy(res->UserName, UN);
	strcpy(res->FileName, UserName);

	if(SOCKET_ERROR == ::sendto(dipHole, SendBuf, BUFFER_SIZE, 0,(SOCKADDR*)&server, sizeof(server))){
		::closesocket(dipHole);
		return -1;
	}

	::closesocket(dipHole);
	return ctrport;
}

void CUserCommDlg::OnButtonClose() 
{
}

BOOL CUserCommDlg::InitialTree()
{
	m_tree.ModifyStyle(0,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS);
	CBitmap bitmap;
	if(!bitmap.LoadBitmap(IDB_IMGLIST_TREE))
		return FALSE;
	CImageList imgList;
	if(!imgList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 11, 1))
		return FALSE;
	if(imgList.Add(&bitmap, RGB(236,233,214))==-1)
		return FALSE;
	m_tree.SetImageList(&imgList, TVSIL_NORMAL);
	bitmap.DeleteObject();
	imgList.Detach();

	hFriend = m_tree.InsertItem("锐客好友", 3, 3, NULL);
	m_tree.SetTreeItemData(hFriend, Is_NetFriendList, NULL);
//	m_tree.SetItemImage(hFriend,4,4);
	
	//added by zhanganzhan on 20090325
	hChatroom = m_tree.InsertItem("同城锐客",33,33,NULL);
	m_tree.SetTreeItemData(hChatroom,Is_Chatroom,NULL);

	hMyFriend = m_tree.InsertItem("我的好友",10,10,NULL);
	m_tree.SetTreeItemData(hMyFriend,Is_MyFriend,NULL);


	hGroup = m_tree.InsertItem("我的群落", 1, 1, NULL);
	m_tree.SetTreeItemData(hGroup, Is_MyNetGroupList, NULL);

	hAllGroup = m_tree.InsertItem("锐客群落", 8, 8, NULL);
	m_tree.SetTreeItemData(hAllGroup, Is_AllRickerGroupList, NULL, 0, 0);
	GetSubGroup(hAllGroup);

	hMyStudyhall = m_tree.InsertItem("我的自习室",5,5,NULL);
	m_tree.SetTreeItemData(hMyStudyhall,Is_MyStudyHall,NULL,0,0);
	GetMyStudy(hMyStudyhall);

	hStudyhall = m_tree.InsertItem("自习室",19,19,NULL);
	m_tree.SetTreeItemData(hStudyhall,Is_StudyHall,NULL);
	GetSchool(hStudyhall);

	return TRUE;
}

void CUserCommDlg::OnSelchangedUserTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	pMainWnd->PostMessage(WM_TREEITEM_SELECTED, (WPARAM)pNMTreeView, (LPARAM)body_tree);
/*	AfxMessageBox("lalala");
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	pMainWnd->PostMessage(WM_TREEITEM_SELECTED, (WPARAM)pNMTreeView, (LPARAM)body_tree);
	*pResult = 0;*/
}
// author: zhanganzhan
// time:  20081112
// description:将本地已经生成的包含本地共享信息的
// XML文件发送给Server端的8888端口 
unsigned __stdcall CUserCommDlg::_SendXML(LPVOID pParam)
{
	CUserCommDlg *pDlg=(CUserCommDlg*)pParam;
	char iniPath[_MAX_PATH] = {0};
	char serverIP[50] = {0};
	char szport[20] = {0};
	char filePath[1024]={0};
	char dbgPath[1024];
	
	sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
	sprintf(dbgPath, "%s\\system\\dbg.txt", CurrentPath);
	
	GetPrivateProfileString("server", "ip", NULL, serverIP, 50, iniPath);
	GetPrivateProfileString("server", "filePort", NULL, szport, 20, iniPath);


	char sendBuf[BUFFER_SIZE];
	sendBuf[0] = state_server_share_info;
	
	//int totalFileLen=shareFile.GetLength();//zhanganzhan on 20081118
	char temp[10];
	int totalFileNum=fileNumCounter+1;
	itoa(totalFileNum,temp,10);
	sprintf(sendBuf+1,"%s",temp);
//	Tips("Begin to send xml,",td_msg,5);
// 
// 	if(SOCKET_ERROR == send(pDlg->serverSock, sendBuf, BUFFER_SIZE, 0))
// 	{
// 		Tips("Failed to send the fileshare information",td_msg,5);		
// 		return 0 ;
//  	}
	int sevport = atoi(szport);
    pDlg->fileSock = socket(AF_INET,SOCK_STREAM, 0);
	if(pDlg->fileSock == INVALID_SOCKET){
		CString msg;
		msg.Format("socket() failed:%d", WSAGetLastError());
		Tips(msg);
		return 0;
	}
	pDlg->m_slist.AddTail(pDlg->fileSock);
	
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverIP);
	server.sin_port = htons(sevport);
	memset(&(server.sin_zero), '\0', 8);


	int connectCount = 0;		//重复建立连接次数
	while (connectCount<5 && (connect(pDlg->fileSock, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR))
	{
		connectCount++;
		closesocket(pDlg->fileSock);
		Tips("error in connect.",td_alert,5);
		Sleep(1000*100);
		pDlg->fileSock = socket(AF_INET,SOCK_STREAM, 0);	
		if(pDlg->fileSock == INVALID_SOCKET){
			CString msg;
			msg.Format("socket() failed:%d", WSAGetLastError());
			Tips(msg);
			return 0;
		}
	}
	if(connectCount>=5)
	{
		closesocket(pDlg->fileSock);
		return 0;
	}
	int i=0;
	CStdioFile file;
	
	sprintf(sendBuf,"%s",UserName);

	if (SOCKET_ERROR == send(pDlg->fileSock,sendBuf,BUFFER_SIZE,0))
	{
		closesocket(pDlg->fileSock);
//		AfxMessageBox("Error in username");
		return 0;
	}
	sprintf(sendBuf,"%d",++fileNumCounter);
//	AfxMessageBox(sendBuf);
	if (SOCKET_ERROR == send(pDlg->fileSock,sendBuf,BUFFER_SIZE,0))
	{
		closesocket(pDlg->fileSock);
//		AfxMessageBox("Error in file len");
		return 0;
	}
	int tempNum=0;
	while(i < fileNumCounter + 1 &&tempNum++<2  )
	{
		sprintf(filePath,"%s\\system\\sharefile\\sharefile_%d.xml",CurrentPath,i);
		if(!file.Open(filePath,CFile::modeRead))
		{	
	//		AfxMessageBox("Error in open file");
		    break;//change1
		}
		sendBuf[0] = state_server_share_info;		
		itoa(file.GetLength(),temp,10);
	    sprintf(sendBuf+1, "%s", temp);
	    if(SOCKET_ERROR == send(pDlg->fileSock, sendBuf, BUFFER_SIZE, 0))
		{
//			AfxMessageBox("Error in send file len");
			break;
		}
	
        int num;
	    int sendLength=0;
		ZeroMemory(sendBuf,1024);
		while(file.Read(sendBuf,BUFFER_SIZE))
		{
			if( !(num= send(pDlg->fileSock, sendBuf, BUFFER_SIZE, 0)))
			{
	//			AfxMessageBox("Send error");
				break ;
			}
		    sendLength+=num;
			if(sendLength>=file.GetLength())
			{
//				AfxMessageBox("send end.");
				break;
			}
		}
		i++;
	    file.Close();
		DeleteFile(filePath);
	    if(SOCKET_ERROR == recv(pDlg->fileSock,sendBuf,BUFFER_SIZE,0))
		{
	//		AfxMessageBox("recv echo error");
	
			break;

		}		
		//Sleep(5000);
	}
	
	closesocket(pDlg->fileSock);

//	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
//	_endthreadex(0);
	
 	return 0;

}


void CUserCommDlg::PubInfo()
{
}

void CUserCommDlg::PubDB(int filetype, LPCSTR iniPath, BOOL all)
{
	if(quit)
		return;
try{
	CString title;
	title.Format("format-%d", filetype);

	char val[10] = {0};
	GetPrivateProfileString("share",title,NULL,val,10,iniPath);
	if(atoi(val) || all)
		CPubInfo::PubDataBase(filetype, serverSock, quit);
}
catch(...){
#ifdef _DEBUG
	AfxMessageBox("PubDB");
#endif
}
}

void CUserCommDlg::GetSubGroup(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetSubGroup,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	m_thlist.AddTail(handle);
	ResumeThread(handle);
}

unsigned __stdcall CUserCommDlg::_GetSubGroup(LPVOID pParam)
{
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)pDlg->m_tree.GetItemData(hItem);
	if(!info)
		return 1;

	CString url;
	url.Format("%s/share/rickergroup/getNode.asp?fid=%d", AspServer, info->BookNumber);
	CString dstPath;
	dstPath.Format("%s\\temp\\rickernode_%d.xml", CurrentPath, info->BookNumber);

	if(!DownLoad(url,dstPath,pDlg->quit,pDlg->m_slist,pDlg->m_mutex))
		return 1;

	int pos = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !pDlg->quit; pos++){
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString title = GetXmlValue(pNodeList, "title", pos);
				CString isleaf = GetXmlValue(pNodeList, "isleaf", pos);
				CString roomid = GetXmlValue(pNodeList,"chatroomid",pos);
				HTREEITEM hCurItem = pDlg->m_tree.InsertItem(title, 2, 2, hItem);
				if(!atoi((LPCSTR)isleaf)){
					pDlg->m_tree.SetTreeItemData(hCurItem, Is_RickerGroup_Node, roomid.GetBuffer(0), 0, atol(id));
					pDlg->GetSubGroup(hCurItem);
				}
				else
					pDlg->m_tree.SetTreeItemData(hCurItem, Is_RickerGroup_Leaf, roomid.GetBuffer(0), 0, atol(id));
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}
	catch(...){}
	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	if(pDlg->m_tree.ItemHasChildren(hItem)){
		CString ftitle;
		ftitle.Format("%s(%d)",pDlg->m_tree.GetItemText(hItem), pos);
		pDlg->m_tree.SetItemText(hItem, ftitle);
	}

	DeleteFile((LPCSTR)dstPath);
	return 0;
}

LRESULT CUserCommDlg::OnThreadFinished(WPARAM w,LPARAM l)
{
	HANDLE thread = (HANDLE)w; 
	POSITION pos = m_thlist.Find(thread);
	if(pos)
		m_thlist.RemoveAt(pos);
		//重连接服务
	if(closeflag==1 && (!pMainWnd->m_quit))//&&FALSE)//&&reconnectTimes<5){
	{
	//	AfxMessageBox("lalalala");
		reconnectTimes++;
		reConnectionFlag = TRUE;
		Tips("重新连接服务器");
		quit = FALSE;
		closeflag = 0;
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_ReConnectionInitial,(LPVOID)this,CREATE_SUSPENDED,NULL);
		ResumeThread(handle);
		m_thlist.AddTail(handle);
	}

	return TRUE;
}
void CUserCommDlg::ChangeChatRoom(char *buf)
{
	m_ctrRichR.SetModify(FALSE);
	CString str;
	CString info;
	info.Format("%s\r\n",buf);

	
	m_ctrRichR.SetSel(-1,-1);
	m_ctrRichR.ReplaceSel(info);
	m_ctrRichR.GetWindowText(str);
	int t1 = str.GetLength()-info.GetLength();
	int t2 = str.GetLength()-1;
	m_ctrRichR.SetSel(t1,t2); //设置处理区域 
	m_ctrRichR.SetSelectionCharFormat(cf_flag);

	m_ctrRichR.SetParaFormat(pf);
	m_ctrRichR.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
//	m_ctrRichR.PostMessage(WM_VSCROLL,SB_TOP,0);
	m_ctrRichR.SetModify(TRUE);

	pMainWnd->MessageAlert();
	

}
void CUserCommDlg::ReadMsg(char *buf)
{
	m_ctrRichR.SetModify(FALSE);

	CTime tm = CTime::GetCurrentTime();
	
	CString name;
	name.Format("%s\r\n", buf);

	//deleted by zhanganzhan for web chat on 20090318
// 	int lpos = name.Find(' ');
// //	int rpos = name.ReverseFind(' ');
// 	//变换成保留 月日时分 的格式
// 	CString Sdate = name.Right(16);//name.Right(name.GetLength()-rpos);
// 	Sdate = Sdate.Left(11);
// 	Sdate = Sdate +"\r\n";
// 	name = name.Left(lpos+1);
// 	name = name + Sdate;

	CString msg;
	msg.Format("%s\r\n", buf+strlen(buf)+1);
	

	CString str;

	/*
	m_ctrRichR.GetWindowText(str);
	m_ctrRichR.SetSel(0,-1);
	m_ctrRichR.ReplaceSel(msg);

	m_ctrRichR.SetSel(-1,-1);
	m_ctrRichR.ReplaceSel(str);
	
	int t1 = 0;
	int t2 = msg.GetLength();
	m_ctrRichR.SetSel(t1,t2);
//	m_ctrRichR.SetSelectionCharFormat(cf_name);

	m_ctrRichR.GetWindowText(str);
	m_ctrRichR.SetSel(0,-1);
	m_ctrRichR.ReplaceSel(name);

	m_ctrRichR.SetSel(-1,-1);
	m_ctrRichR.ReplaceSel(str+"\r\n");

	t1 = 0;
	t2 = name.GetLength();
	m_ctrRichR.SetSel(t1,t2);
//	m_ctrRichR.SetSelectionCharFormat(cf_name);
	*/
	
	m_ctrRichR.SetSel(-1,-1);
	m_ctrRichR.ReplaceSel(name);
	m_ctrRichR.GetWindowText(str);
	int t1 = str.GetLength()-name.GetLength();
	int t2 = str.GetLength()-1;
	m_ctrRichR.SetSel(t1,t2); //设置处理区域 
	m_ctrRichR.SetSelectionCharFormat(cf_name);

	m_ctrRichR.SetSel(-1,-1);
	m_ctrRichR.ReplaceSel(msg);
	m_ctrRichR.GetWindowText(str);
	t1 = str.GetLength()-msg.GetLength();
	t2 = str.GetLength()-1;
	m_ctrRichR.SetSel(t1,t2);
	m_ctrRichR.SetSelectionCharFormat(cf_msg);
	m_ctrRichR.SetParaFormat(pf);
	m_ctrRichR.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
//	m_ctrRichR.PostMessage(WM_VSCROLL,SB_TOP,0);
	m_ctrRichR.SetModify(TRUE);

	pMainWnd->MessageAlert();

	Dang();
}

void CUserCommDlg::Dang()
{
	BOOL brtn;
	LPSTR lpres;
	HANDLE hres;
	HRSRC hresinfo;
	HINSTANCE nl=AfxGetInstanceHandle();
	/* find the wave resource. */
	hresinfo= FindResource(nl,MAKEINTRESOURCE(IDR_WAVE_DANG),"WAVE");
	if(hresinfo==NULL)
		return;
	/* load the wave resource. */
	hres = LoadResource(nl,hresinfo);
	if(hres==NULL)
		return;
	/* lock the wave resource and play it. */
	lpres=(LPSTR)LockResource(hres);
	if(lpres==NULL)
		return;
	brtn = sndPlaySound(lpres,SND_MEMORY|SND_SYNC);
	if(brtn==NULL)
		return;
	/* free the wave resource and return success or failure. */
	FreeResource(hres);
}

void CUserCommDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1){
		char sendBuf[BUFFER_SIZE] = {0};
		sendBuf[0] = state_login_a;
		send(serverSock, sendBuf, BUFFER_SIZE, 0);
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CALLBACK CUserCommDlg::EnumWinProc(HWND hwnd, LPARAM lParam)   
{   
	DWORD dwID;   
    
	EnumParam* pep = (EnumParam*)lParam;   
	GetWindowThreadProcessId(hwnd, &dwID);   
	if(dwID == pep->dwProcessID){   
		pep->hMainWnd = hwnd;
		return 0;   
	}
	return TRUE;   
}

HWND CUserCommDlg::CreateTalkWnd()
{
	char Path[_MAX_PATH]={0};
	sprintf(Path, "%s\\system\\talk.exe", CurrentPath);

	EnumParam   ep;   
	STARTUPINFO   si;   
	PROCESS_INFORMATION   pi;   
   
	ep.hMainWnd = NULL;   
	memset(&si, 0, sizeof(si));   
	si.cb = sizeof(STARTUPINFO);   
	if(CreateProcess(NULL,Path,   
		NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))     
	{   
		CloseHandle(pi.hThread);   
		CloseHandle(pi.hProcess);   
		ep.dwProcessID = pi.dwProcessId;
		do{
			Sleep(50);
			EnumWindows((WNDENUMPROC)EnumWinProc, (long)&ep);   
		}while(!ep.hMainWnd);
		return ep.hMainWnd;
	}   
	return NULL;
}

LRESULT CUserCommDlg::OnCreateTalkDlg(WPARAM w,LPARAM l)
{
	LPSTR UN = (LPSTR)w;
	HWND hWnd = CreateTalkWnd();
	if(hWnd){
		::PostMessage(hWnd,PROCESS_WM_NOTIFY_NAME, (WPARAM)UN, (LPARAM)m_hWnd);
		m_wndList.AddTail(hWnd);
	}
	else
		free(UN);

	return TRUE;
}

void CUserCommDlg::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnRButtonDblClk(nFlags, point);
}

void CUserCommDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnRButtonDown(nFlags, point);
// 	hit_item = m_tree.HitTest(point);
// 	if(!hit_item)
// 		return;
// 	TreeItemInfo* info = (TreeItemInfo*)m_tree.GetItemData(hit_item);
// 	if(!info)
// 		return;
// 	ClientToScreen(&point);
// 
// 	CMenu* pMenu;
}
//聊天记录存放于文本文件 和 数据库 中
int CUserCommDlg::RecordMsg(char *package)
{
	int count = 0;										//判断是否为重复message录入
	CString name;
	name.Format("%s\r\n", package);
	int pos = name.Find(' ');
	CString username = name.Left(pos);					//存用户名
	CString utime = name.Right(name.GetLength()-pos);	//存时间
	CString msg;										//存信息
	msg.Format("%s\r\n", package+strlen(package)+1);
	char msgRecordPath[_MAX_PATH];
	sprintf(msgRecordPath,"%s\\system\\MsgRecord.txt",CurrentPath);

//added by zuoss 20081009 聊天记录存于文本 未完成，如何在文件头插入数据？
//	CFile cf;
//	if(!cf.Open(msgRecordPath,CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate))
//		Tips("写入聊天记录失败");
//	cf.SeekToEnd();
//	cf.Write((LPCSTR)username,username.GetLength());
//	cf.SeekToEnd();
//	cf.Write((LPCSTR)utime,utime.GetLength());
//	cf.SeekToEnd();
//	cf.Write((LPCSTR)msg,msg.GetLength());
//	
//	cf.Close();
//end add

	//聊天记录存于数据库
	//查询数据库中是否有相同的message
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			throw 0;
			char SQLSEL[1024] = {0};
			sprintf(SQLSEL, "SELECT count(0) AS cnt FROM messageRecord WHERE username=\"%s\" AND time =\"%s\"",username,utime);
			hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
			count = GetCollectLong(m_pRecordset, "cnt");
	}
	catch(_com_error &e){
			if(SUCCEEDED(hr)){
				if(m_pRecordset->State)
					m_pRecordset->Close();
				//m_pRecordset.Release();
			}
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();
	//无重复信息则，插入数据库
	if(count<=0){
		char SQL[1000]={0};
		sprintf(SQL, "INSERT INTO messageRecord ([username],[time],[message]) values(\"%s\",\"%s\",\"%s\")", username,utime,msg);
		m_pConnection->Execute(SQL, NULL, adCmdText);
	}
	return count;
}

void CUserCommDlg::OnButtonMsgrecord() 
{
	// TODO: Add your control notification handler code here
	CString MsgRecord;
	MsgRecord.Format("%s\\system\\MsgRecord.txt",CurrentPath);
	CFile cf;
	if(!cf.Open(MsgRecord,CFile::modeCreate|CFile::modeWrite))
		Tips("打开聊天记录失败");
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			throw 0;
			char SQLSEL[1024] = {0};
			sprintf(SQLSEL, "SELECT [username],[time],[message] FROM messageRecord order by ID DESC");
			hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
			while(!m_pRecordset->EndOfFile){
				CString uname = GetCollectString(m_pRecordset, "username");
				CString utime = GetCollectString(m_pRecordset, "time");
				CString message = GetCollectString(m_pRecordset, "message");

				cf.SeekToEnd();
				cf.Write((LPCSTR)uname,uname.GetLength());
				cf.SeekToEnd();
				cf.Write((LPCSTR)utime,utime.GetLength());
				cf.SeekToEnd();
				cf.Write((LPCSTR)message,message.GetLength());
				m_pRecordset->MoveNext();
			}
			
	}
	catch(_com_error &e){
			if(SUCCEEDED(hr)){
				if(m_pRecordset->State)
					m_pRecordset->Close();
			//	m_pRecordset.Release();
			}
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();
	cf.Close();
	::ShellExecute(NULL, "open", MsgRecord, NULL, NULL, SW_SHOW);
}

int CUserCommDlg::reConnectServer()
{
	
		closesocket(serverSock);
			//set global variable
			char iniPath[_MAX_PATH] = {0};
			char serverIP[50] = {0};
			char szport[20] = {0};
			char szctrport[20] = {0};
			char sztranport[20] = {0};
			sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
			GetPrivateProfileString("server", "ip", NULL, serverIP, 50, iniPath);
/*	//zuoss 20081217
			CString sIPAddress;
			struct hostent FAR *lpHostEnt=gethostbyname("readings.vicp.net");
			if(lpHostEnt == NULL)
			{
				sIPAddress = _T("");
			}
			LPSTR lpaddr = lpHostEnt->h_addr_list[0];
			if(lpaddr)
			{
				struct in_addr inAddr;
				memmove (&inAddr,lpaddr, 4);
				sIPAddress=inet_ntoa (inAddr);
				if(sIPAddress.IsEmpty())
					sIPAddress=_T("Not available");
			} 
			strcpy(serverIP,sIPAddress);
			//AfxMessageBox(sIPAddress);
//end*/
			GetPrivateProfileString("server", "port", NULL, szport, 20, iniPath);
			GetPrivateProfileString("client", "ctrport", NULL, szctrport, 20, iniPath);
			GetPrivateProfileString("client", "tranport", NULL, sztranport, 20, iniPath);
			int sevport = atoi(szport);
			int ctrport = atoi(szctrport);
			int tranport = atoi(sztranport);
			serverSock = socket(AF_INET,SOCK_STREAM, 0);
			if(serverSock == INVALID_SOCKET){
				CString msg;
				msg.Format("socket() failed:%d", WSAGetLastError());
				return -1;
			}
			struct sockaddr_in server;
			server.sin_family = AF_INET;
			server.sin_addr.s_addr = inet_addr(serverIP);
			server.sin_port = htons(sevport);
			memset(&(server.sin_zero), '\0', 8);
			int connectCount = 0;		//重复建立连接次数
			while (connectCount<10 && (connect(serverSock, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR))
			{
				connectCount++;
				closesocket(serverSock);
				serverSock = socket(AF_INET,SOCK_STREAM, 0);	
				if(serverSock == INVALID_SOCKET){
					CString msg;
					msg.Format("socket() failed:%d", WSAGetLastError());
			//		Tips(msg);
					AfxMessageBox(msg);
					return -1;
				}
			}
	
	//登录
	char buf[BUFFER_SIZE] = {0};
	buf[0] = state_login_r;    

	STRUCTLOGIN logInfo;
	sprintf(logInfo.UserName, UserName);
	logInfo.ctrPort = ctrport;
	logInfo.tranPort = tranport;
    
	//获得本机IP
	char computename[155]={0};
	char username[28]={0};
	PHOSTENT hostinfo; 
	if(gethostname(computename, sizeof(computename)) == 0) { 
		if((hostinfo = gethostbyname(computename)) != NULL) { //这些就是获得IP的函数
			strcpy(logInfo.IP ,inet_ntoa (*(in_addr *)*hostinfo->h_addr_list)); 
			strcpy(hostInerIp,logInfo.IP);
		} 
	} 
	memcpy(buf+1, &logInfo,sizeof(STRUCTLOGIN));  
	
//	AfxMessageBox(buf+1);
	
	if(SOCKET_ERROR == send(serverSock, buf, BUFFER_SIZE, 0)){
			#ifdef _DEBUG
					CString msg;
					msg.Format("send() failed:%d", WSAGetLastError());
					AfxMessageBox(msg);
				//	Tips(msg);
			#endif
					return -1;
	}
     
	ZeroMemory(buf,BUFFER_SIZE);
	if(SOCKET_ERROR == recv(serverSock, buf, BUFFER_SIZE, 0)){
			#ifdef _DEBUG
					CString msg;
					msg.Format("recv() failed:%d", WSAGetLastError());
					AfxMessageBox(msg);
				//	Tips(msg);
			#endif
			return -1;
	}
			return connectCount;
}

void CUserCommDlg::OnClickUserTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	//HTREEITEM hItem = NULL;
	//TreeItemInfo* info = NULL;
	//hItem = this->m_tree.GetSelectedItem();
	//if (hItem == NULL)
	//{
	//	return;
	//}
	//info = (TreeItemInfo*)this->m_tree.GetItemData(hItem);
	//if(info->Type == Is_NetFriendList || info->Type == Is_Chatroom)
	//{
		pMainWnd->PostMessage(WM_TREEITEM_SELECTED, (WPARAM)pNMTreeView, (LPARAM)body_tree);
	//}else{
	//	if(info->IndexID != NULL)
	//	{
	//		CString croomid = info->IndexID;
	//		AfxMessageBox(croomid);
	//	}
	//}
	*pResult = 0;
}
//added by zhanganzhan on 20090326
int nn=0;
LRESULT CUserCommDlg::OnChangeChatroom1(WPARAM wParam,LPARAM lParam)
{
	char tmpBuf[50];
	char msg[BUFFER_SIZE];

	POSITION pos;
	msgNode node;
	int cont=0;
	if (isChatroom1 == TRUE)
	{
		return false; //zhanganzhan on 100322
	}
	nn++;
	isChatroom1 = TRUE;
	isChatroom2 = FALSE;	
	chatRoomID = chatRoomID1;
	m_tree.SetItemImage(hChatroom,33,33);
	m_tree.SetItemImage(hFriend,3,3);

	m_ctrRichR.SetSel(0,-1);
	m_ctrRichR.ReplaceSel("");//清屏
	ChangeChatRoom("欢迎来到锐客好友聊天室");

	SuspendThread(hGetMsg);

	cont=0;
	cont=msgList1.GetCount();
	pos=msgList1.GetHeadPosition();

	while (pos)
	{
		node = msgList1.GetNext(pos);
		sprintf(tmpBuf,"%s",node.time);					
		sprintf(msg,"%s %s",node.name,tmpBuf+5);
		strcpy(msg+2+strlen(msg+1), (LPCSTR)node.content);	
	//	if(RecordMsg(msg)<=0 )	
 			ReadMsg(msg);
	//		msgList1.RemoveAt(pos);
		
	}
	//msgList1.RemoveAll();
	ResumeThread(hGetMsg);
	return true;
}

//added by zhanganzhan on 20090326
LRESULT CUserCommDlg::OnChangeChatroom2(WPARAM wParam,LPARAM lParam)
{
	if (chatRoomID2 == MAX_CHATROOM_ID)
	{
		Tips("您不属于该聊天室，请申请加入",td_msg,5);
		return false;

	}
	char tmpBuf[50];
	char msg[BUFFER_SIZE];
	int cont=0;
	POSITION pos;
	msgNode node;
	if (isChatroom2 == TRUE)//防止多次点击
	{
		return false; //deleted by zhanganzhan on 100322
	}
	isChatroom1 = FALSE;
	isChatroom2 = TRUE;
	m_tree.SetItemImage(hChatroom,10,10);
	m_tree.SetItemImage(hFriend,32,32);

	chatRoomID = chatRoomID2;
	m_ctrRichR.SetSel(0,-1);
	m_ctrRichR.ReplaceSel("");

	ChangeChatRoom("欢迎来到同城锐客聊天室");
		SuspendThread(hGetMsg);

	cont=msgList2.GetCount();
	

	pos = msgList2.GetHeadPosition();
	while (pos)
	{
		node = msgList2.GetNext(pos);
		sprintf(tmpBuf,"%s",node.time);					
		sprintf(msg,"%s %s",node.name,tmpBuf+5);
		strcpy(msg+2+strlen(msg+1), (LPCSTR)node.content);	
	//	if(RecordMsg(msg)<=0 )	
 			ReadMsg(msg);
	//		msgList1.RemoveAt(pos);
		
	}
	ResumeThread(hGetMsg);
	return true;

}
LRESULT CUserCommDlg::OnChangeChatroom(WPARAM index,LPARAM l)
{
	char tmpBuf[50];
	char msg[BUFFER_SIZE];
	int cont=0;

	isChatroom1 = FALSE;
	isChatroom2 = FALSE;
	CString cid(strChatId);
	
	if(cid == CString((char*)index)){
		return false;
	}
	ZeroMemory(strChatId,100);
	strncpy(strChatId,(char*)index,100);
//	AfxMessageBox(strChatId);

	POSITION pos;
	msgNode node;
	
	m_ctrRichR.SetSel(0,-1);
	m_ctrRichR.ReplaceSel("");

//	ChangeChatRoom("欢迎来到同城锐客聊天室");
	
	SuspendThread(hGetMsg);

	cont=msgList.GetCount();
	pos=msgList.GetHeadPosition();

	while (pos)
	{
		node = msgList.GetNext(pos);
		if (strcmp(node.chatroomID,strChatId)==0)
		{
			sprintf(tmpBuf,"%s",node.time);					
			sprintf(msg,"%s %s",node.name,tmpBuf+5);
			strcpy(msg+2+strlen(msg+1), (LPCSTR)node.content);	
			
			ReadMsg(msg);
		}
		
	}
	ResumeThread(hGetMsg);
	return true;

}
LRESULT CUserCommDlg::OnChangeUser(WPARAM wParam,LPARAM lParam)
{

	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_Initial,(LPVOID)this,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
	m_thlist.AddTail(handle);
	pMainWnd->PostMessage(WM_MAINFRAME_REFRESH_USER,NULL,NULL);
	return true;
}
void CUserCommDlg::OnRclickUserTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    	*pResult = 0;
}

void CUserCommDlg::GetSchool(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetSchool,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	m_thlist.AddTail(handle);
	ResumeThread(handle);
}

unsigned __stdcall CUserCommDlg::_GetSchool(LPVOID pParam)
{
	HTREEITEM hItem = (HTREEITEM)pParam;
	CString url;
	url.Format("%s/share/studyhall/getschool.asp", AspServer);
	CString dstPath;
	dstPath.Format("%s\\temp\\School.xml", CurrentPath);
	if(!DownLoad(url,dstPath,pDlg->quit,pDlg->m_slist,pDlg->m_mutex))
		return 1;

	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !pDlg->quit; pos++){
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				HTREEITEM hCurItem = pDlg->m_tree.InsertItem(name, 29, 29, hItem);
				pDlg->m_tree.SetTreeItemData(hCurItem, Is_School, NULL, 0, atol(id));
				pDlg->GetBuilding(hCurItem);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}
	catch(...){}
	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	return 0;
}

void CUserCommDlg::GetBuilding(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetBuild,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	m_thlist.AddTail(handle);
	ResumeThread(handle);
}

unsigned __stdcall CUserCommDlg::_GetBuild(LPVOID pParam)
{
//	share/studyhall/getbuilding.asp?schoolid=1
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)pDlg->m_tree.GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/share/studyhall/getbuilding.asp?schoolid=%d", AspServer,info->BookNumber);
	CString dstPath;
	dstPath.Format("%s\\temp\\SchoolBuild_%d.xml", CurrentPath,info->BookNumber);
	if(!DownLoad(url,dstPath,pDlg->quit,pDlg->m_slist,pDlg->m_mutex))
		return 1;
		int pos = 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !pDlg->quit; pos++){
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				HTREEITEM hCurItem = pDlg->m_tree.InsertItem(name,30,30, hItem);
				pDlg->m_tree.SetTreeItemData(hCurItem,Is_StudyBuilding,NULL, 0, atol(id));
				pDlg->GetStudyHall(hCurItem);			//添加自习室
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}
	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());

	return 0;
}

void CUserCommDlg::GetStudyHall(HTREEITEM hItem)
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetStudyHall,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	m_thlist.AddTail(handle);
	ResumeThread(handle);
}

unsigned __stdcall CUserCommDlg::_GetStudyHall(LPVOID pParam)
{
	HTREEITEM hItem = (HTREEITEM)pParam;
	TreeItemInfo* info = (TreeItemInfo*)pDlg->m_tree.GetItemData(hItem);
	if(!info)
		return 1;
	CString url;
	url.Format("%s/share/studyhall/gethall.asp?buildid=%d", AspServer,info->BookNumber);
	CString dstPath;
	dstPath.Format("%s\\temp\\StudyHall_%d.xml", CurrentPath,info->BookNumber);
	if(!DownLoad(url,dstPath,pDlg->quit,pDlg->m_slist,pDlg->m_mutex))
		return 1;

	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !pDlg->quit; pos++){
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);
				HTREEITEM hCurItem = pDlg->m_tree.InsertItem(name,31,31, hItem);
				pDlg->m_tree.SetTreeItemData(hCurItem,Is_Hall,NULL, 0, atol(id));
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}
	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}

void CUserCommDlg::GetMyStudy(HTREEITEM hItem)
{
	
	HANDLE handle = (HANDLE)_beginthreadex(NULL,0,_GetMyUniversity,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	m_thlist.AddTail(handle);
	ResumeThread(handle);
	
	HANDLE handle1 = (HANDLE)_beginthreadex(NULL,0,_GetMyStudyBuilding,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	m_thlist.AddTail(handle1);
	ResumeThread(handle1);

	HANDLE handle2 = (HANDLE)_beginthreadex(NULL,0,_GetMyStudyHall,(LPVOID)hItem,CREATE_SUSPENDED,NULL);
	m_thlist.AddTail(handle2);
	ResumeThread(handle2);
}

unsigned __stdcall CUserCommDlg::_GetMyUniversity(LPVOID pParam)
{
	
	HTREEITEM hItem = (HTREEITEM)pParam;
	CString url;
	url.Format("%s/share/studyhall/getfavoriteuniversity.asp?username=%s", AspServer, UserName);
	CString dstPath;
	dstPath.Format("%s\\temp\\favoriteuniversity.xml", CurrentPath);

	if(!DownLoad(url,dstPath,pDlg->quit,pDlg->m_slist,pDlg->m_mutex))
 		return 1;
	

	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !pDlg->quit; pos++){
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);

				HTREEITEM hCurItem = pDlg->m_tree.InsertItem(name, 29, 29, hItem);
				pDlg->m_tree.SetTreeItemData(hCurItem, Is_School, NULL, 0, atol(id));
				pDlg->GetBuilding(hCurItem);
				
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}

	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}

unsigned __stdcall CUserCommDlg::_GetMyStudyBuilding(LPVOID pParam)
{
	//share/studyhall/getfavoritebuilding.asp?username=start_dzh
	HTREEITEM hItem = (HTREEITEM)pParam;
	CString url;
	url.Format("%s/share/studyhall/getfavoritebuilding.asp?username=%s", AspServer, UserName);
	CString dstPath;
	dstPath.Format("%s\\temp\\favoritebuilding.xml", CurrentPath);
	if(!DownLoad(url,dstPath,pDlg->quit,pDlg->m_slist,pDlg->m_mutex))
 		return 1;
	

	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !pDlg->quit; pos++){
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);

				HTREEITEM hCurItem = pDlg->m_tree.InsertItem(name, 30, 30, hItem);
				pDlg->m_tree.SetTreeItemData(hCurItem, Is_StudyBuilding, NULL, 0, atol(id));
				pDlg->GetStudyHall(hCurItem);
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}

	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}

unsigned __stdcall CUserCommDlg::_GetMyStudyHall(LPVOID pParam)
{
	//share/studyhall/getfavoritehall.asp?username=start_dzh
	HTREEITEM hItem = (HTREEITEM)pParam;
	CString url;
	url.Format("%s/share/studyhall/getfavoritehall.asp?username=%s", AspServer, UserName);
	CString dstPath;
	dstPath.Format("%s\\temp\\favoritehall.xml", CurrentPath);
	if(!DownLoad(url,dstPath,pDlg->quit,pDlg->m_slist,pDlg->m_mutex))
 		return 1;
	

	int pos = 0;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dstPath))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList){
			for(pos=0; pos<pNodeList->length && !pDlg->quit; pos++){
				CString id = GetXmlValue(pNodeList, "id", pos);
				CString name = GetXmlValue(pNodeList, "name", pos);

				HTREEITEM hCurItem = pDlg->m_tree.InsertItem(name, 31, 31, hItem);
				pDlg->m_tree.SetTreeItemData(hCurItem, Is_Hall, NULL, 0, atol(id));
			}
			pNodeList.Release();
		}
		pDoc.Release();
	}
	catch(_com_error& e){}
	pDlg->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return 0;
}
