// FileCollect.cpp: implementation of the CFileCollect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainFrm.h"
#include "Readings.h"
#include "FileCollect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern CMainFrame* pMainWnd;
extern char CurrentPath[_MAX_PATH];
extern _ConnectionPtr m_pConnection;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileCollect::CFileCollect()
{

}

CFileCollect::~CFileCollect()
{

}

BOOL CFileCollect::Initial()
{
	m_diskList.RemoveAll();

	char Path[_MAX_PATH] = {0};
	char collect_style[100] = {0};
	sprintf_s(Path, _MAX_PATH, "%s/system/config.ini", CurrentPath);
	GetPrivateProfileString("favor","collect-style", "", collect_style, 100, Path);
	if (strlen(collect_style)>0 && atoi(collect_style)==1) {
		memset(Path, 0, _MAX_PATH);
		sprintf_s(Path, _MAX_PATH, "%s\\system\\colPath.inf", CurrentPath);

		if(PathFileExists(Path)){//指定搜索
			CStdioFile cf;
			if(cf.Open(Path, CFile::modeRead)){
				long len = cf.GetLength();
				CString temp;
				while (cf.ReadString(temp)) {
					m_diskList.AddTail(temp);
					temp.Empty();
				}
			}
		}
		else
			return FALSE;
	}
	else{
		__int64 i64TotalUsed = 0;
		DWORD driversbit=GetLogicalDrives();
		driversbit = driversbit;
		for(int i = 0; i < 32; i++){
			if(driversbit & 1){
				char driver[10] = {"a:"};
				driver[0] = 'a' + i;
				if(GetDriveType(driver)==DRIVE_FIXED){
					__int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes ;
					GetDiskFreeSpaceEx(driver,
						(PULARGE_INTEGER)&i64FreeBytesToCaller,
						(PULARGE_INTEGER)&i64TotalBytes,
						(PULARGE_INTEGER)&i64FreeBytes);
					CString temp = driver;
					m_diskList.AddTail(temp);
					i64TotalUsed += (i64TotalBytes-i64FreeBytes);
				}
			}
			driversbit = driversbit >> 1;
		}
		char tBuf[100] = {0};
		sprintf_s(tBuf, 100, "%I64d", i64TotalUsed);
		char Path[_MAX_PATH] = {0};
		sprintf_s(Path, _MAX_PATH, "%s/system/sysinfo.ini", CurrentPath);
		WritePrivateProfileString("UpdateDB","TotalSize", tBuf, Path);
	}
	return TRUE;
}

unsigned _int64 CFileCollect::GetDiskFree(char *disk)
{
	ULARGE_INTEGER lpfreetouse = {0},lpfreeclunum = {0},lpclunum = {0};
	GetDiskFreeSpaceEx(disk,&lpfreetouse,&lpclunum,&lpfreeclunum);
	return lpfreetouse.QuadPart;
}

void CFileCollect::Do(BOOL& quit)
{
	try{
		char Path[_MAX_PATH] = {0};
		sprintf_s(Path, _MAX_PATH, "%s\\system\\sysinfo.ini", CurrentPath);
		char info[100] = {0};
		GetPrivateProfileString("UpdateDB","start", "",info,100, Path);
		CString strInfo = info;
		if (m_diskList.GetCount() == 0)
		{
			return;
		}
		POSITION pos = m_diskList.Find(strInfo);
		if(!pos){
			pos = m_diskList.GetHeadPosition();
			strInfo = m_diskList.GetAt(pos);
			strcpy_s(info, 100, strInfo);
			WritePrivateProfileString("UpdateDB", "start", info, Path);
		}

		char tFSize[100] = {0};
		GetPrivateProfileString("UpdateDB","FinishSize", "",tFSize,100, Path);
		finishedSize = _atoi64(tFSize);
		while(pos && !quit){
			CString driver = m_diskList.GetNext(pos);
			WritePrivateProfileString("UpdateDB","start", driver, Path);
			Search(driver, finishedSize, quit);

			ZeroMemory(tFSize, 100);
			sprintf_s(tFSize, 100, "%I64d", finishedSize);
			WritePrivateProfileString("UpdateDB","FinishSize", tFSize, Path);
		}
		if(!quit)
			WritePrivateProfileString("UpdateDB","start", "c:", Path);
	}catch(...){Tips("本地格式采集失败");}
}

void CFileCollect::Search(const char *Path, __int64& finishedSize, BOOL& quit)
{
try{
	WIN32_FIND_DATA data = {0};
	char findPath[_MAX_PATH] = {0};
	sprintf_s(findPath, _MAX_PATH, "%s\\*",Path);
	HANDLE handle = ::FindFirstFile(findPath, &data);
	if(handle == INVALID_HANDLE_VALUE)
		return;
	else{
		FileOperation(data, Path, finishedSize, quit);
	}

	BOOL bSearch = TRUE;
	while(bSearch && !quit){
		if (pMainWnd->m_quit)
		{
			return;
		}
		Sleep(10);
		if(::FindNextFile(handle, &data)){
			FileOperation(data, Path, finishedSize, quit);
		}
		else{
			if(::GetLastError()==ERROR_NO_MORE_FILES)
				bSearch = FALSE;    //Terminate Search
		}
	}
	::FindClose(handle);

	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\sysinfo.ini", CurrentPath);
	char tFSize[100] = {0};
	sprintf_s(tFSize, 100, "%I64d", finishedSize);
	WritePrivateProfileString("UpdateDB","FinishSize", tFSize, iniPath);
}
catch(...){
#ifdef _DEBUG
	Tips("void CFileCollect::Search(const char *Path, __int64& finishedSize, BOOL& quit)");
#endif
}
}

void CFileCollect::Release()
{
	m_diskList.RemoveAll();
}

BOOL CFileCollect::FileOperation(WIN32_FIND_DATA data, const char *Path, __int64& finishedSize, BOOL& quit)
{
try{
	//FALSE为文件夹  TRUE为文件
	char cp[_MAX_PATH] = {0};
	sprintf_s(cp, _MAX_PATH, "%s\\temp", CurrentPath);	//临时文件目录

	if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
		if(_stricmp(data.cFileName, ".")==0)
			return FALSE;
		else if(_stricmp(data.cFileName, "..")==0)
			return FALSE;
		else if(_stricmp(data.cFileName, "cookies")==0)
			return FALSE;
		else if(_stricmp(data.cFileName, "Temporary Internet Files")==0)
			return FALSE;
		else if(_stricmp(data.cFileName, "windows")==0)
			return FALSE;
		else if(_stricmp(data.cFileName, "winnt")==0)
			return FALSE;
		else if(_stricmp(data.cFileName, "Program Files")==0)
			return FALSE;
		else if(_stricmp(data.cFileName, "temp")==0 && _stricmp(data.cFileName, cp)!=0)
			return FALSE;
		else if(_stricmp(data.cFileName, "RECYCLER")==0)
			return FALSE;
		else if(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			return FALSE;
		else if(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			return FALSE;
		else if(data.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
			return FALSE;
		else if(strlen(data.cFileName)>5){
			if(_stricmp(data.cFileName+strlen(data.cFileName)-5, ".files")==0)
				return FALSE;
		}
		char CurrentFilePath[_MAX_PATH] = {0};
		sprintf_s(CurrentFilePath, _MAX_PATH, "%s\\%s", Path, data.cFileName);
		Search(CurrentFilePath, finishedSize, quit);

		return FALSE;
	}

	if(strlen(data.cFileName)==0)
		return FALSE;
	
	char* dotloc = strrchr(data.cFileName, '.');
	if(!dotloc)
		return FALSE;
	
	char* extension = dotloc+1;
	if(dotloc+1-data.cFileName>strlen(data.cFileName))
		return FALSE;

	CString FilePath;
	FilePath.Format("%s\\%s", Path, data.cFileName);

	*dotloc = 0;	//把文件名和扩展名分开

	finishedSize+=data.nFileSizeLow;

	int filetype = GetFileTypeFromExt(extension);
	if(filetype==docFileType){
		if(data.cFileName[0] == '~' && data.cFileName[1] == '$')// doc 缓存文件
			filetype = unknownFileType;
	}
	if(filetype!=unknownFileType){
		SYSTEMTIME tm;
		::FileTimeToSystemTime(&data.ftLastWriteTime, &tm);
		CString strTime;	//修改时间
		strTime.Format("%d-%d-%d %d:%d", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute);
		CString strTitle;
		if(strlen(data.cFileName)>0)
			strTitle = data.cFileName;	//主名
		_RecordsetPtr m_pRecordset;
		HRESULT hr;
		try{
			hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
			if(!SUCCEEDED(hr))	//数据集打开不成功
				return TRUE;
	
			char SQL[1000]={0};
			sprintf_s(SQL, 100, "SELECT [addr],[cdate],[type],[title],[size],[year],[nlist] FROM loc WHERE type=%d and addr=\"%s\"", filetype, FilePath);
			m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
				adOpenKeyset, adLockOptimistic, adCmdText);
			if(m_pRecordset->EndOfFile){
				m_pRecordset->AddNew();
				m_pRecordset->PutCollect("addr", _variant_t(FilePath));
				m_pRecordset->PutCollect("cdate", _variant_t(strTime));
				m_pRecordset->PutCollect("type", _variant_t((long)filetype));
				m_pRecordset->PutCollect("title", _variant_t(strTitle));
				m_pRecordset->PutCollect("size", _variant_t((long)data.nFileSizeLow));
				m_pRecordset->PutCollect("year", _variant_t((long)tm.wYear));
				long nlist=255;
				switch(filetype){
				case wolFileType:
					nlist = 1;
					break;
				case pdfFileType:
					nlist = 2;
					break;
				case docFileType:
					nlist = 3;
					break;
				case chmFileType:
					nlist = 4;
					break;
				case htmFileType:
					nlist = 5;
					break;
				case txtFileType:
					nlist = 6;
					break;
				default:
					nlist = 7;
				}
				m_pRecordset->PutCollect("nlist", _variant_t((long)nlist));
				m_pRecordset->Update();
			}
			m_pRecordset->Close();
		//	m_pRecordset.Release();
		}
		catch(_com_error &e){
			if(SUCCEEDED(hr)){
				if(m_pRecordset->State)
					m_pRecordset->Close();
			//	m_pRecordset.Release();
			}
		}
	}
	return TRUE;
}
catch(...){
#ifdef _DEBUG
	/*
	Tips("BOOL CFileCollect::FileOperation(WIN32_FIND_DATA data, const char *Path, __int64& finishedSize, BOOL& quit)");
	FILE* fp = fopen(".\\errorReport.txt", "wb");
	if(fp){
		fseek(fp, 0, SEEK_END);
		fwrite(&data, 1, sizeof(WIN32_FIND_DATA), fp);
		fclose(fp);
	}
	*/
#endif
	return FALSE;
}
}

