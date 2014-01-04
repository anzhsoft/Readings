// GetUserInfo.cpp: implementation of the CGetUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Readings.h"
#include "GetUserInfo.h"
#include "base64.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern BOOL offline;
extern char SDDisk[20];
extern char FavorPath[_MAX_PATH];
extern char UserName[50];
extern char JspServer[1024];
extern char CurrentPath[_MAX_PATH];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGetUserInfo::CGetUserInfo()
{
}

CGetUserInfo::~CGetUserInfo()
{

}

void CGetUserInfo::GetWriting(BOOL& quit)
{
	if(strlen(SDDisk)==0)
		return;
	//手写读物 rtf
	CString rtfloc;
	rtfloc.Format("%s\\Readman\\Comments\\@记事@notepad@pdf\\*.png", SDDisk);
	CFileFind findrtf;
	BOOL Ext = findrtf.FindFile(rtfloc);
	while(Ext && !quit){
		Ext = findrtf.FindNextFile();
		CString rtfpath;
		rtfpath.Format("%s\\手写文件", FavorPath);
		_mkdir(rtfpath);
		rtfpath.Format("%s\\手写文件\\记事文件", FavorPath);
		_mkdir(rtfpath);
		rtfpath.Format("%s\\手写文件\\记事文件\\%s", FavorPath, findrtf.GetFileName());
		CString pathtemp = findrtf.GetFilePath();
		CopyFile(pathtemp, rtfpath, FALSE);
	}
	findrtf.Close();
	

	CString locpath;
	locpath.Format("%s\\手写文件", FavorPath);
	_mkdir(locpath);
	locpath.Format("%s\\手写文件\\记事文件", FavorPath);
	
	rtfloc.Format("%s\\记事", quit);

	GetNotes(rtfloc,locpath,quit);
}

void CGetUserInfo::GetNotes(CString path, CString loc, BOOL& quit)
{
	_mkdir(loc);
	CFileFind findrtf;
	path += "\\*";
	BOOL Ext = findrtf.FindFile(path);
	while(Ext && !quit){
		Ext = findrtf.FindNextFile();
		if(findrtf.IsDots())
			continue;
		if(findrtf.IsDirectory()){
			CString locpath;
			locpath.Format("%s\\%s", loc, findrtf.GetFileTitle());
			GetNotes(findrtf.GetFilePath(),locpath,quit);
			continue;
		}
		if(findrtf.GetFileName().Compare("notepad.pdf")==0)
			continue;

		CString pathtemp = (loc+"\\"+findrtf.GetFileName());
		CopyFile(findrtf.GetFilePath(), pathtemp, FALSE);
	}
	findrtf.Close();
}


void CGetUserInfo::GetComment(BOOL& quit, CSocketList& socketList, CMutex& mutex)
{
	if(strlen(SDDisk)==0)
		return;
	//已批读物 png
	CString rtfloc;
	rtfloc.Format("%s\\Readman\\Comments\\*.", SDDisk);
	CFileFind findrtf;
	BOOL Ext = findrtf.FindFile(rtfloc);
	while(Ext && !quit){
		Ext = findrtf.FindNextFile();
		if(findrtf.IsDots())
			continue;
		if(!findrtf.IsDirectory())
			continue;
		if(findrtf.GetFileTitle().CompareNoCase("@记事@notepad@pdf")==0)
			continue;

		CFileFind findpng;
		CString tttpa;
		tttpa.Format("%s\\*.png", findrtf.GetFilePath());
		BOOL ext2 = findpng.FindFile(tttpa);
		while(ext2){
			ext2 = findpng.FindNextFile();
			CString pngPath;
			pngPath.Format("%s\\手写文件", FavorPath, UserName);
			_mkdir(pngPath);
			pngPath.Format("%s\\手写文件\\已批读物", FavorPath, UserName);
			_mkdir(pngPath);
			char temp_name[_MAX_PATH] = {0};
			strcpy_s(temp_name, _MAX_PATH, findpng.GetFileName());
			char* temp1 = strchr(temp_name, '_');
			if(!temp1)
				continue;
			temp1 += 1;
			char* temp2 = strrchr(temp_name, '_');
			if(!temp2)
				continue;
			if(temp2<=temp1)
				continue;
			*temp2 = 0;
			
			CString filename;

			int start = -1;
			int end = findrtf.GetFileTitle().ReverseFind('@');
			CString strFileTitle;
			CString extension;
			
			if(end>0){
				strFileTitle = findrtf.GetFileTitle().Left(end);
				start = strFileTitle.ReverseFind('@');
				if(start>=0 && start+1<strFileTitle.GetLength()-1){
					strFileTitle = strFileTitle.Mid(start+1, strFileTitle.GetLength()-start-1);
					extension = findrtf.GetFileTitle().Right(findrtf.GetFileTitle().GetLength()-end-1);
					strFileTitle += ".";
					strFileTitle += extension;
				}
				else
					strFileTitle = findrtf.GetFileTitle();
			}
			else
				strFileTitle = findrtf.GetFileTitle();
			filename.Format("%s_%s.png", strFileTitle, temp1);
			pngPath.Format("%s\\手写文件\\已批读物\\%s", FavorPath, filename);
			CopyFile(findpng.GetFilePath(), pngPath, FALSE);
			if(!offline){
				CString lastedit;
				CTime tm;
				findpng.GetLastWriteTime(tm);
				lastedit.Format("%d-%02d-%02d %02d:%02d:%02d", 
					tm.GetYear(), tm.GetMonth(), tm.GetDay(),
					tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				CTime curtime = CTime::GetCurrentTime();
				char WebPath[1024] = {0};
				unsigned char* b64Name = encode((unsigned char*)(LPCSTR)strFileTitle, strFileTitle.GetLength());
				sprintf_s(WebPath, 1024, "%s/RetailWeb/recorduserhandwriting?username=%s&lastedit=%s&flag=2&typeflag=5&filename=%s&lastvisit=%s",
					JspServer, UserName, lastedit, (char*)b64Name, curtime.Format("%Y-%m-%d %H:%M:%S"));
				free(b64Name);
				char Tadd[_MAX_PATH] = {0};
				sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\rec.tmp", CurrentPath);
				BOOL flag = FALSE;
				if(!DownLoad(WebPath, Tadd, quit, socketList, mutex))
				{
				//	AfxMessageBox(WebPath);
					Tips("CGetUserInfo::GetComment--Download");
				}
				DeleteFile(Tadd);
			}
		}
		findpng.Close();
	}
	findrtf.Close();
}

void CGetUserInfo::GetRestore(BOOL& quit)
{
	if(strlen(SDDisk)==0)
		return;
	//收藏文件 rtf
	CString rtfloc;
	rtfloc.Format("%s\\Readman\\Restore\\*.rtf", SDDisk);
	CFileFind findrtf;
	BOOL Ext = findrtf.FindFile(rtfloc);
	while(Ext && !quit){
		Ext = findrtf.FindNextFile();
		CString rtfpath;
		rtfpath.Format("%s\\手写文件", FavorPath);
		_mkdir(rtfpath);
		rtfpath.Format("%s\\手写文件\\收藏读物", FavorPath);
		_mkdir(rtfpath);
		rtfpath.Format("%s\\手写文件\\收藏读物\\%s", FavorPath, findrtf.GetFileName());
		CopyFile(findrtf.GetFilePath(), rtfpath, FALSE);
	}
	findrtf.Close();
}

void CGetUserInfo::UpBugLib(BOOL& quit, CSocketList& socketList, CMutex& mutex)
{
	if(strlen(SDDisk)==0)
		return;

	char UpFilePath[_MAX_PATH] = {0};
	sprintf_s(UpFilePath, _MAX_PATH, "%s\\Readman\\Recording\\v2_use_record.txt", SDDisk);

	if(!PathFileExists(UpFilePath))
		return;

	CFileFind find;
	BOOL ext = find.FindFile(UpFilePath);
	if(!ext){
		find.Close();
		return;
	}
	find.FindNextFile();
	long size = find.GetLength();
	CTime reftm;
	find.GetCreationTime(reftm);
	CString createdate = reftm.Format("%Y-%m-%d %H:%M:%S");
	find.GetLastWriteTime(reftm);
	CString modifydate = reftm.Format("%Y-%m-%d %H:%M:%S");
	find.Close();

	//建立FTP会话
	CInternetSession* m_pInetSession=new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	CFtpConnection *m_pFtpConnection;

	if(!DownLoad(FTPSET, _T("ftp.tmp"), quit, socketList, mutex))
		return;

	FILE *fp;
	fp = fopen("ftp.tmp","rb");
	if(!fp)
		return;
	fseek(fp, 0, 2);
	long len = ftell(fp);
	fseek(fp, 0, 0);
	char* buf;
	buf = (char*)malloc(len+1);
	memset(buf, 0, len+1);
	fread(buf, 1, len+1, fp);
	fclose(fp);
	char* head = buf;
	char* tmp;
	
	if(*head=='0')
		return;

	char* addr;
	char* usr;
	char* pwd;
	char* port;

	head = strchr(head, '#');
	head++;
	tmp = strchr(head,'#');
	
	addr = (char*)malloc(tmp-head+1);
	memset(addr, 0, tmp-head+1);
	memcpy(addr, head, tmp-head);

	head = tmp+1;
	tmp = strchr(head,'#');
	usr = (char*)malloc(tmp-head+1);
	memset(usr, 0, tmp-head+1);
	memcpy(usr, head, tmp-head);

	head = tmp+1;
	tmp = strchr(head,'#');
	pwd = (char*)malloc(tmp-head+1);
	memset(pwd, 0, tmp-head+1);
	memcpy(pwd, head, tmp-head);

	head = tmp+1;
	tmp = strchr(head,0);
	port = (char*)malloc(tmp-head+1);
	memset(port, 0, tmp-head+1);
	memcpy(port, head, tmp-head);

	CString strAddr;
	CString strPath;
	
	head = NULL;
	tmp = NULL;
	head = strstr(addr,"ftp://");
	head += 6;
	tmp = strchr(head,'/');
	if(tmp == NULL){
		strAddr = head;
		strPath = ".";
	}
	else{
		*tmp=0;
		strAddr = head;
		strPath = ++tmp;
	}

	try{
		 m_pFtpConnection = m_pInetSession->GetFtpConnection(strAddr, usr, pwd, atoi(port));
	}
	catch(CInternetException *pEx)
	{
		TCHAR szError[1024] = {0};
		if(pEx->GetErrorMessage(szError,1024))
			AfxMessageBox(szError);
		else 
			AfxMessageBox(_T("There was an exception"));
		pEx->Delete();
		m_pFtpConnection=NULL;

		free(buf);
		free(addr);
		free(usr);
		free(pwd);
		free(port);

		return;
	}
	free(buf);
	free(addr);
	free(usr);
	free(pwd);
	free(port);

try{
	if(m_pFtpConnection->SetCurrentDirectory(strPath)){//设置上传目录
		if(!m_pFtpConnection->SetCurrentDirectory(UserName)){
			m_pFtpConnection->CreateDirectory(UserName);
			if(!m_pFtpConnection->SetCurrentDirectory(UserName))
				goto disconnect;
		}
		char RemoteName[_MAX_PATH] = {0};
		sprintf_s(RemoteName, _MAX_PATH, "ad_%s.txt", UserName);
		m_pFtpConnection->PutFile(UpFilePath, RemoteName);
		char WebPath[1024] = {0};
		sprintf_s(WebPath, 1024, "%s/RetailWeb/recorduserhandwriting?username=%s&filesize=%d&createdate=%s&lastvisit=%s&flag=2&typeflag=2&filename=%s&filerealname=%s",
			JspServer, UserName, size, createdate, modifydate, RemoteName, RemoteName);
		if(!DownLoad(WebPath, _T("nothing.tmp"), quit, socketList, mutex))
		{
			Tips("CGetUserInfo::UpBugLib");
		}
			DeleteFile("nothing.tmp");
		//上传bug库
		char BugFilePath[_MAX_PATH] = {0};
		sprintf_s(BugFilePath, _MAX_PATH, "%s\\Readman\\Bug\\Bugrecord.txt", SDDisk);
		if(PathFileExists(BugFilePath)){
			//bug库ftp上传
			ZeroMemory(RemoteName,_MAX_PATH);
			sprintf_s(RemoteName, _MAX_PATH, "bug_%s.txt", UserName);
			CFileFind findbug;
			BOOL extbug = findbug.FindFile(UpFilePath);
			if(extbug){
				findbug.FindNextFile();
				long sizebug = findbug.GetLength();
				CTime reftmbug;
				findbug.GetCreationTime(reftmbug);
				CString createdatebug = reftmbug.Format("%Y-%m-%d %H:%M:%S");
				findbug.GetLastWriteTime(reftmbug);
				CString modifydatebug = reftmbug.Format("%Y-%m-%d %H:%M:%S");
				m_pFtpConnection->PutFile(BugFilePath, RemoteName);
				ZeroMemory(WebPath,1024);
				sprintf_s(WebPath, 1024, "http://%s/RetailWeb/recorduserhandwriting?username=%s&filesize=%d&createdate=%s&lastvisit=%s&flag=2&typeflag=3&filename=%s&filerealname=%s",
					JspServer, UserName, sizebug, createdatebug, modifydatebug, RemoteName, RemoteName);
				if(!DownLoad(WebPath, _T("nothing.tmp"), quit, socketList, mutex))
					DeleteFile("nothing.tmp");
			}
			findbug.Close();
		}
	}
disconnect:
	if(m_pFtpConnection!=NULL){//断开ftp会话
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
	}
	delete m_pInetSession;
}
catch(CInternetException e){
#ifdef _DEBUG
	char msg[1000] = {0};
	e.GetErrorMessage(msg,1000);
	Tips(msg);
#endif
}
catch(...){}
}