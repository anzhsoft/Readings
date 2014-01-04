// PubInfo.cpp: implementation of the CPubInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Readings.h"
#include "PubInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern _ConnectionPtr m_pConnection;
extern char CurrentPath[_MAX_PATH];
extern char UserName[50];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPubInfo::CPubInfo()
{
	ZeroMemory(rootDir, _MAX_PATH+1);
	strcpy_s(rootDir, _MAX_PATH+1, ".");
	offset = 0;
	ZeroMemory(buffer, BUFFER_SIZE);
}
CPubInfo::CPubInfo(LPCSTR root)
{
	ZeroMemory(rootDir, _MAX_PATH+1);
	strcpy_s(rootDir, _MAX_PATH+1, root);
	offset = 0;
	ZeroMemory(buffer, BUFFER_SIZE);
}

CPubInfo::~CPubInfo()
{

}

void CPubInfo::FindFileInDir(char* Path, BOOL& quit)
{
	if(quit)
		return;

try{
	char fname[_MAX_PATH] = {0};					//当前文件名
    char filePathName[_MAX_PATH] = {0};				//当前文件名的路径名
	char filePathNameCopy[_MAX_PATH] = {0};			//copy当前文件名的路径名，不带*
    BOOL bSearchFinished = FALSE;
    HANDLE hSearch;
	WIN32_FIND_DATA fd = {0};						//定义Find_DATA

	sprintf_s(filePathName, _MAX_PATH, "%s\\*", Path);
	hSearch = ::FindFirstFile(filePathName, &fd);
	if (hSearch==INVALID_HANDLE_VALUE)
		return;

	if(quit)
		return;
	//Is directory：是目录则调用递归操作
    if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
       && strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )       
	{
		char subDirPath[_MAX_PATH+1] = {0};
		sprintf_s(subDirPath, _MAX_PATH, "%s\\%s", Path, fd.cFileName);
		FindFileInDir(subDirPath, quit);				//递归调用
    }
	else if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")){
		SHAREINFO oneInfo ={0}; 
		FormShareInfo(Path, &fd, &oneInfo);	//生成SHAREINFO结构体
	}
    while(!bSearchFinished && !quit){
		if(::FindNextFile(hSearch, &fd)){			//对同级目录的其他文件操作
	   	   //is directory：是目录则向下递归
		   if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			  && strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )       
		   {
				char subDirPath[_MAX_PATH+1] = {0};
				sprintf_s(subDirPath, _MAX_PATH, "%s\\%s", Path, fd.cFileName);
				FindFileInDir(subDirPath, quit);				//递归调用
		   }
		   else if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")){
				ZeroMemory(fname,_MAX_PATH);
	    		SHAREINFO oneInfo = {0};
				FormShareInfo(Path,&fd,&oneInfo);
		   }
	   }
	   else
		  bSearchFinished = TRUE;     //Terminate Search
    }

	::FindClose(hSearch);
}
catch(...){
#ifdef _DEBUG
	AfxMessageBox("FindFileInDir");
#endif
}
}

#include "HttpUtl.h"

BOOL UpLoadOneTorrent(CString tokenPath)
{
	CFile torrent;
	if (!torrent.Open(tokenPath,CFile::modeRead))
	{
		return NULL;
	}
	CString filename = torrent.GetFileName();
	char * buff = new char[torrent.GetLength()+10];
	memset(buff,0,torrent.GetLength()+10);
#if 0 
	// delete by anzhsoft for platform
	torrent.ReadHuge(buff, torrent.GetLength());
#endif
	
	CString head;
    HttpUtl utl;
	CString pathName=torrent.GetFilePath();//torrent.GetPathName();
	CString fileName=torrent.GetFileName();

//	utl.post("61.181.14.184","/ClientUpload.html",pathName.GetBuffer(0),buff,fileName.GetBuffer(0),6969);

	delete [] buff;
	return FALSE;
}

void CPubInfo::FormShareInfo(char* filepath, WIN32_FIND_DATA* pfd, SHAREINFO* oneInfo)
{
//add by liym 20100401

	CString exePath;
	CString param;
	CString torrentPath;
	CString torrentName;

	torrentPath.Format("%s\\torrent", CurrentPath);

	_mkdir(torrentPath);

	torrentName.Format("%s.torrent", pfd->cFileName);

	exePath.Format("%s//download", CurrentPath);

//	AfxMessageBox(exePath);

	param.Format("make %s %s %s %s 61.181.14.184 6969 %s", filepath, pfd->cFileName, torrentPath, torrentName, UserName);

//	AfxMessageBox(param);

	CString torrentFullPath;
	torrentFullPath.Format("%s\\%s", torrentPath, torrentName);

	::DeleteFile(torrentFullPath);
	
//	::ShellExecute(NULL, "open", exePath, param, NULL, SW_HIDE);



//	AfxMessageBox(torrentFullPath);

//	UpLoadOneTorrent(torrentFullPath);

	return;
}

int CPubInfo::GetFileType(char* filename)
{
try{
	char *filetype = strrchr(filename, '.');
	if (!filetype)
		return unknownFileType;
	if(filetype-filename>=strlen(filename))
		return unknownFileType;

	filetype++;								//前移一位去掉"."
	return GetFileTypeFromExt(filetype);
}
catch(...){
#ifdef _DEBUG
	AfxMessageBox("GetFileType");
#endif
}
return 0;
}

void CPubInfo::StartPubInfo(BOOL& quit)
{
try{
	FindFileInDir(rootDir, quit);
}
catch(...){
#ifdef _DEBUG
	AfxMessageBox("StartPubInfo");
#endif
}
}

UINT CPubInfo::PubDataBase(int filetype, SOCKET s, BOOL& quit)
{
	if(quit)
		return 0;
try{
	try{
		_RecordsetPtr m_pRecordset;
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			return 1;
		char SQL[1024] = {0};
		sprintf_s(SQL, 1024, "SELECT [title],[addr],[cdate],[synFlag],[size] FROM loc WHERE type=%d ORDER BY cdate DESC", filetype);
		hr = m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(),
			adOpenStatic, adLockOptimistic, adCmdText);
		char SendBuf[BUFFER_SIZE] = {0};
		int offset = sizeof(SHAREINFOHEAD)+1;
		int count = 0;
		while(!m_pRecordset->EndOfFile && !quit){
			SHAREINFO info = {0};
			strcpy_s(info.FileName, _MAX_PATH+1, GetCollectString(m_pRecordset,"addr"));
			info.FileType = filetype;
			info.nFileSize = GetCollectLong(m_pRecordset, "size");
			try{
				VARIANT var = m_pRecordset->GetCollect("cdate");
				if(var.vt == VT_NULL){
					m_pRecordset->MoveNext();
					continue;
				}
				SYSTEMTIME st;
				VariantTimeToSystemTime(var.date, &st);
				SystemTimeToFileTime(&st,&info.ftLastWriteTime);
			}
			catch(_com_error& e){
				m_pRecordset->MoveNext();
				continue;
			}
			m_pRecordset->MoveNext();
			memcpy(SendBuf+offset,&info,sizeof(SHAREINFO));
			offset+=sizeof(SHAREINFO);
			count++;
			if(offset+sizeof(SHAREINFO)>BUFFER_SIZE || m_pRecordset->EndOfFile){
				offset = sizeof(SHAREINFOHEAD)+1;
				SHAREINFOHEAD head = {0};
				head.num = count;
				count = 0;
				memcpy(SendBuf+1,&head,sizeof(SHAREINFOHEAD));
				SendBuf[0] = state_server_share_info;
				::send(s,SendBuf,BUFFER_SIZE,0);
				ZeroMemory(SendBuf,BUFFER_SIZE);
			}
		}
		m_pRecordset->Close();
		m_pRecordset.Release();
	}
	catch(_com_error& e){
#ifdef _DEBUG
		Tips(e.ErrorMessage());
#endif
	}
}
catch(...){
#ifdef _DEBUG
	AfxMessageBox("StartPubInfo");
#endif
}
	return 0;
}