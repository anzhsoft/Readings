// UploadShareFileList.cpp: implementation of the CUploadShareFileList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "UploadShareFileList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMainFrame* pMainWnd;
extern char CurrentPath[_MAX_PATH];
extern char AspServer[1024];
extern char UserName[50];
extern _ConnectionPtr m_pConnection;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUploadShareFileList::CUploadShareFileList()
{

}

CUploadShareFileList::~CUploadShareFileList()
{

}

//将本地所有的共享文件列表查找出来
//2010.04.07改动：只获取所有共享的文件夹路径
CList<CString,CString&>* CUploadShareFileList::GetShareFileList()
{
	CList<CString,CString&>* ShareFileList = new CList<CString,CString&>;
	try
	{
		//首先从共享目录中查找所有共享文件
		char iniPath[_MAX_PATH] = {0};
		sprintf(iniPath, "%s\\system\\config.ini", CurrentPath);
		
		char val[50] = {0};
		GetPrivateProfileString("share", "sharefolder", NULL, val, 50, iniPath);

		if(atoi(val)){
			CString infoPath;
			infoPath.Format("%s\\system\\sharepath.inf", CurrentPath);
			CStdioFile cf;
			if(cf.Open(infoPath,CFile::modeRead)){
				CString path;
				while(cf.ReadString(path) && !pMainWnd->m_quit){
					int i =path.GetLength();//删除根目录的\问题
					char ch =path.GetAt(i-1);
					if(ch=='\\')
					{
						path.Delete(i-1,1);
					}
					ShareFileList->AddTail(path);
					
				}
				cf.Close();
			}
		}

		//若选择了按格式共享则将这些格式文件加入列表之中
/*		GetPrivateProfileString("share", "shareformat", NULL, val, 50, iniPath);
		if(atoi(val)){
			ZeroMemory(val, 50);
			GetPrivateProfileString("share", "all-format", NULL, val, 50, iniPath);
			BOOL all = atoi(val);
			PubDB(wolFileType, iniPath, all);
			PubDB(txtFileType, iniPath, all);
			PubDB(htmFileType, iniPath, all);
			PubDB(docFileType, iniPath, all);
			PubDB(pptFileType, iniPath, all);
			PubDB(pdfFileType, iniPath, all);
			
			PubDB(chmFileType, iniPath, all);
			PubDB(rarFileType, iniPath, all);
			PubDB(jpgFileType, iniPath, all);
			PubDB(bmpFileType, iniPath, all);
			PubDB(gifFileType, iniPath, all);
			PubDB(wavFileType, iniPath, all);
			PubDB(mp3FileType, iniPath, all);
			
			PubDB(wmvFileType, iniPath, all);
			PubDB(rmFileType, iniPath, all);
			PubDB(aviFileType, iniPath, all);
			PubDB(asfFileType, iniPath, all);
			PubDB(_3gpFileType, iniPath, all);
		}*/
	}
	catch (...){}

	//更新用户共享文件数
//	CString url;
//	url.Format("%s/share/updateShareNum.asp?username=%s&sharecount=%d",AspServer,UserName,ShareFileList->GetCount());
 //	UpLoad(url);

	return ShareFileList;
}

CList<CString,CString&>* CUploadShareFileList::FindFileInDir(char *Path)
{
	if(pMainWnd->m_quit)
		return NULL;

	CList<CString,CString&>* ShareFileList = new CList<CString,CString&>;

	try{
		char fname[_MAX_PATH] = {0};					//当前文件名
		char filePathName[_MAX_PATH] = {0};				//当前文件名的路径名
		char filePathNameCopy[_MAX_PATH] = {0};			//copy当前文件名的路径名，不带*
		BOOL bSearchFinished = FALSE;
		HANDLE hSearch;
		WIN32_FIND_DATA fd = {0};						//定义Find_DATA
		
		sprintf(filePathName, "%s\\*", Path);
		hSearch = ::FindFirstFile(filePathName, &fd);
		if (hSearch==INVALID_HANDLE_VALUE)
			return NULL;
		
		if(pMainWnd->m_quit)
			return NULL;
		//Is directory：是目录则调用递归操作
		if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )       
		{
			char subDirPath[_MAX_PATH+1] = {0};
			sprintf(subDirPath, "%s\\%s", Path, fd.cFileName);
			ShareFileList->AddTail(FindFileInDir(subDirPath));//递归调用
		}
		else if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")){
			ShareFileList->AddTail(CString(fd.cFileName));
		}
		while(!bSearchFinished && !pMainWnd->m_quit){
			if(::FindNextFile(hSearch, &fd)){//对同级目录的其他文件操作
				//is directory：是目录则向下递归
				if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )       
				{
					char subDirPath[_MAX_PATH+1] = {0};
					sprintf(subDirPath, "%s\\%s", Path, fd.cFileName);
					ShareFileList->AddTail(FindFileInDir(subDirPath));//递归调用
				}
				else if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")){
					ShareFileList->AddTail(CString(fd.cFileName));
				}
			}
			else
				bSearchFinished = TRUE;     //Terminate Search
		}
		::FindClose(hSearch);
	}
	catch(...){}

	return ShareFileList;
}

CList<CString,CString&>* CUploadShareFileList::PubDB(int filetype, LPCSTR iniPath, BOOL all)
{
	if(pMainWnd->m_quit)
		return NULL;

	CList<CString,CString&>* ShareFileList = new CList<CString,CString&>;

	try{
		CString title;
		title.Format("format-%d", filetype);
		
		char val[10] = {0};
		GetPrivateProfileString("share",title,NULL,val,10,iniPath);
		if(atoi(val) || all)
		{
			_RecordsetPtr m_pRecordset;
			HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
			if(!SUCCEEDED(hr))
				return NULL;
			char SQL[1024] = {0};
			sprintf(SQL, "SELECT [title],[addr],[cdate],[synFlag],[size] FROM loc WHERE type=%d ORDER BY cdate DESC", filetype);
			hr = m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(),
				adOpenStatic, adLockOptimistic, adCmdText);	
			while(!m_pRecordset->EndOfFile && !pMainWnd->m_quit){
				CString FileName = GetCollectString(m_pRecordset,"addr");
				ShareFileList->AddTail(FileName);
			}
		}
	}
	catch(...){}

	return ShareFileList;
}
