// BTOperation.cpp: implementation of the CBTOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "BTOperation.h"
#include "UploadShareFileList.h"
#include <Tlhelp32.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern char UserName[50];
extern char CurrentPath[_MAX_PATH];
extern CMainFrame* pMainWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBTOperation::CBTOperation()
{

}

CBTOperation::~CBTOperation()
{

}

/*
   ��P2P���ڷ�����Ϣ
   ����:
   pWnd �����̴���ָ��
   msg ���͵���Ϣ
*/
void CBTOperation::sendMessage(CWnd *pWnd,string msg){
   COPYDATASTRUCT     cds;
   cds.dwData=8888;
   cds.lpData=(char*)msg.c_str();   
   cds.cbData=msg.length()+1;   
   HWND p2pHandle=::FindWindow(NULL,"P2PClient");
   //CString temp;
   //temp.Format("%d",p2pHandle);
   //AfxMessageBox(temp);
   //::SendMessage(p2pHandle,WM_COPYDATA,(WPARAM)pWnd->GetSafeHwnd(),(LPARAM)(PCOPYDATASTRUCT)&cds);
   ::SendMessage(p2pHandle,WM_COPYDATA,0,(LPARAM)(PCOPYDATASTRUCT)&cds);
}

/*
   ��P2P���ڷ��͹����ļ�����Ϣ
   ����:
   pWnd �����̴���ָ��
   msg ���͵���Ϣ
*/
void CBTOperation::sendShareMsg(CWnd *pWnd,string msg){
	string path = GetTorrentFloder().GetBuffer(0);
	string tempmsg=string("share:")+string(UserName)+";"+path+";"+msg;
	//AfxMessageBox(tempmsg.c_str());
	sendMessage(pWnd,tempmsg);
}

/*
   ��P2P���ڷ���������Ϣ
   ����:
   pWnd �����̴���ָ��
   msg ���͵���Ϣ
*/
void CBTOperation::sendDownloadMsg(CWnd *pWnd,string msg){
	string tempmsg=string("download:")+msg;
	//AfxMessageBox(tempmsg.c_str());
	sendMessage(pWnd,tempmsg);
}

BOOL CBTOperation::OpenServer()
{
	//���Ȼ�ȡ�û������ļ����б�
	CString namelist;
	CList<CString,CString&>* filelist = new CList<CString,CString&>;
	filelist = CUploadShareFileList::GetShareFileList();
	POSITION pos = filelist->GetHeadPosition();
	while (pos && !pMainWnd->m_quit)
	{
		namelist += filelist->GetNext(pos)+";";
	}
	if(namelist.GetLength() > 0){
		namelist.Delete(namelist.GetLength()-1);
	}

	//�������ط������(����ǰ�ȹر���ǰ�����Ľ���) add by dongzhenhua 20100403
	HANDLE  handle3 = NULL;
	HANDLE  handle4 = NULL;
	PROCESSENTRY32   ProInfo;
	HANDLE	hProSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL,0); 
	if(::Process32First(hProSnap,&ProInfo))   
	{   
		for(;::Process32Next(hProSnap,&ProInfo);)   
		{   
			if(CString(ProInfo.szExeFile).Find("download.exe") != -1 || 
			CString(ProInfo.szExeFile).Find("DOWNLOAD.EXE") != -1)   
			{   
				handle3 = ::OpenProcess(PROCESS_ALL_ACCESS,NULL, ProInfo.th32ProcessID);   
				TerminateProcess(handle3, 0);
				//AfxMessageBox("download.exe killed");
				//Sleep(2000);
			}
		}
	} 
	CString exePath;
	exePath.Format("%s\\p2p\\download.exe",CurrentPath);
	_mkdir(exePath);
	CString param = "download.exe";
	::ShellExecute(NULL, "open", exePath, param, NULL, SW_HIDE);
	
	//�������ļ����б����û����������ط������
	int i=0;
	while (i < 20)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		i ++;
		Sleep(500);
	}
	extern CReadingsApp theApp;
	sendShareMsg(theApp.GetMainWnd(),namelist.GetBuffer(0));

	return TRUE;
}

CString CBTOperation::DownloadFile(CString torrentURL, CString DstPath)
{
	// add by liym 20100401
	CString torrentFolder = GetTorrentFloder();
	if (PathFileExists(torrentFolder+"\\"+torrentURL))
	{
		return torrentURL;
	}
	CString torrentName;
	char *c = (char *)strrchr(torrentURL, '/');
	if (!c) {
		Tips(" c == NULL!");
		return "";
	}
	torrentName.Format(c+1);
	
	CString torrentPath;
	torrentPath.Format("%s\\%s", torrentFolder, torrentName);
	
	DeleteFile(torrentPath);
	char Tadd[1000] = {0};
	sprintf_s(Tadd, "%s\\system\\sysdl.dll", CurrentPath);
	HINSTANCE 	hDll = ::LoadLibrary(Tadd);	
	if(!hDll){
		AfxMessageBox("���ض�̬��ʧ��,�����°�װ����!");
		memset(Tadd, 0, 1000);
		return "";
	}
	typedef int  (CALLBACK* pDownload)(const char *Url, const char *SaveFileAdd);
	pDownload Dlldownload = (pDownload)::GetProcAddress(hDll, _T("download"));
	
	if(Dlldownload(torrentURL, torrentPath) != 1)
	{
		Tips("���ӷ���������");
		::FreeLibrary(hDll);
		return "";
	}
	
	::FreeLibrary(hDll);

	extern CReadingsApp theApp;
	//��Ϣ��ʽ������·��;Ŀ��·��
	sendDownloadMsg(theApp.GetMainWnd(),(torrentPath+";"+DstPath).GetBuffer(0));

	return torrentName;
}

CString CBTOperation::GetTorrentFloder()
{
	CString TorrentFloder;
	TorrentFloder.Format("%s\\system\\�ҵ�ReaTo",CurrentPath);
	_mkdir(TorrentFloder);
	return TorrentFloder;
}
