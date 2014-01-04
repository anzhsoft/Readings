// PushAdv.cpp: implementation of the CPushAdv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Readings.h"
#include "PushAdv.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern BOOL offline;
extern char CurrentPath[_MAX_PATH];
extern char JspServer[1024];
extern char AspServer[1024];
extern char UserName[50];
extern char SDDisk[20];
extern char FavorPath[_MAX_PATH];
extern CMainFrame* pMainWnd;
extern _ConnectionPtr m_pConnection;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPushAdv::CPushAdv()
{

}

CPushAdv::~CPushAdv()
{

}

BOOL CPushAdv::PushReadman()
{
	char Tadd[1024] = {0};
	sprintf_s(Tadd, 1024, "%s\\temp\\push.xml", CurrentPath);
	char HttpAdd[1024] = {0};
	sprintf_s(HttpAdd, 1024, "%s/pushforreadings.asp?id=%s", AspServer, UserName);
	//Readman PUSH
	if(DownLoad(HttpAdd, Tadd, pMainWnd->m_quit, pMainWnd->m_socketList, pMainWnd->m_mutex) != -1){
		FILE* fp;
		fp = fopen(Tadd, "rb");
		if(!fp)
			return FALSE;
		fseek(fp, 0, 2);
		long len = ftell(fp);
		fseek(fp, 0, 0);
		char* buf;
		buf = (char*)malloc(len+1);
		memset(buf, 0, len+1);
		fread(buf, 1, len, fp);
		fclose(fp);
		int res = atoi(buf);
		free(buf);
		if(!res)
			return FALSE;
		memset(HttpAdd, 0, 1024);
		sprintf_s(HttpAdd, 1024, "%s/push/Readman_%d.wol", AspServer, res);

		memset(Tadd, 0, 1024);
		sprintf_s(Tadd, 1024, "%s\\Readman", FavorPath);
		if(!PathFileExists(Tadd))
			_mkdir(Tadd);
		strcat_s(Tadd, 1024, "\\News");
		if(!PathFileExists(Tadd))
			_mkdir(Tadd);
		//推送文件夹的标志文件
		char flagPath[_MAX_PATH] = {0};
		sprintf_s(flagPath, _MAX_PATH, "%s/push", Tadd);
		FILE* flagfp = fopen(flagPath, "wb");
		if(flagfp)
			fclose(flagfp);

		CTime tm = CTime::GetCurrentTime();
		memset(Tadd, 0, 1024);
		sprintf_s(Tadd, 1024, "%s\\Readman\\News\\Readman时报—", FavorPath);
		if(tm.GetHour()>0 && tm.GetHour()<=10)
			strcat_s(Tadd, 1024, "早.wol");
		else if(tm.GetHour()>10 && tm.GetHour()<=15)
			strcat_s(Tadd, 1024, "中.wol");
		else
			strcat_s(Tadd, 1024, "晚.wol");

		if(DownLoad(HttpAdd, Tadd, pMainWnd->m_quit,pMainWnd->m_socketList,pMainWnd->m_mutex) != -1){
			CreateSYN(Tadd);
			try{
				_RecordsetPtr m_pRecordset;
				char SQL[1000]={0};
				FILE *fp;
				try{
					fp = fopen(Tadd, "rb");
					if(!fp)
						throw 0;
					WolFileHeadInfo Info = {0};
					fread(&Info, 1, sizeof(WolFileHeadInfo), fp);
					if(memcmp(Info.szFlag, "WolfEbook", 9) != 0)
						throw 1;
					char *buf = (char *)malloc(Info.wDocumentInfoLen+1);
					if(!buf){
						throw 1;
					}
					memset(buf, 0, Info.wDocumentInfoLen + 1);
					fread(buf, 1, Info.wDocumentInfoLen, fp);
					fseek(fp, 0, 2);
					long len = ftell(fp);
					fclose(fp);
					char t[1000] = {0};
					char *tmp1 = strstr(buf, "<title>");
					if(tmp1){
						char *tmp2 = strstr(tmp1, "\x0d\x0a");
						if(tmp2){
							long len = tmp2 - tmp1 - strlen("<title>");
							if(len > 900)
								len = 900;
							strncpy_s(t, 1000, tmp1 + strlen("<title>"), len);
						}
					}
					if(buf)
						free(buf);
					sprintf_s(SQL, 1000, "DELETE from push WHERE path=\"%s\"", Tadd);
					m_pConnection->Execute(SQL, NULL,adCmdText);
					memset(SQL, 0, 1000);
					strcpy_s(SQL, 1000, "SELECT * FROM push");
					m_pRecordset.CreateInstance(__uuidof(Recordset));
					m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
							adOpenDynamic, adLockOptimistic, adCmdText);
					m_pRecordset->AddNew();
					m_pRecordset->PutCollect("title",(_variant_t)t);
					m_pRecordset->PutCollect("path",(_variant_t)Tadd);
					m_pRecordset->PutCollect("type",(_variant_t)("Readman"));
					m_pRecordset->PutCollect("filetype",(_variant_t)(long)(wolFileType));
					CString slen;
					slen.Format("%d", len);
					m_pRecordset->PutCollect("size",(_variant_t)slen);
					m_pRecordset->PutCollect("cdate",(_variant_t)GetDate());
					m_pRecordset->PutCollect("user",(_variant_t)UserName);
					m_pRecordset->Update();
					m_pRecordset->Close();
				//	m_pRecordset.Release();
				}
				catch(int err){
					if(err==1)	fclose(fp);
				}
			}
			catch(_com_error& e){}
		}
	}
	else{
		return FALSE;
	}
	return TRUE;
}

UINT CPushAdv::PushAdv(LPVOID pParam)
{
	char URL[1024] = {0};
	sprintf_s(URL, 1024, "%s/RetailWeb/hanlinclient/getpushinfo.jsp?username=%s", JspServer, UserName);//页面有错090217
	char Tadd[_MAX_PATH] = {0};
	sprintf_s(Tadd, _MAX_PATH, "%s\\temp\\ReadmanPush.xml", CurrentPath);
	DeleteFile(Tadd);

	BOOL flag = FALSE;
	if(!DownLoad(URL, Tadd, pMainWnd->m_quit, pMainWnd->m_socketList, pMainWnd->m_mutex))
		return 0;

	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 实例创建
		if(!pDoc->load(Tadd))
			throw 2;	//xml 解析失败
		//上次更新时间
		char iniPath[_MAX_PATH] = {0};
		sprintf_s(iniPath, _MAX_PATH, "%s\\system\\sysinfo.ini", CurrentPath);
		char timeini[100] = {0};
		GetPrivateProfileString("PUSH", "ReadmanDate", "", timeini, 100, iniPath);

		long localt = 0;
		long curtime = 0;
		if(strlen(timeini)>0)
			localt = atol(timeini);
		
		pNodeList = NULL;
		pNodeList = pDoc->selectNodes("Readman/Entity");
		int pos=0;
		if(pNodeList){
			for(int pos=0; pos<pNodeList->length&&!pMainWnd->m_quit; pos++){
				CString Type = GetXmlValue(pNodeList, "Type", pos);
				CString URL = GetXmlValue(pNodeList, "url", pos);
				CString FileName = GetXmlValue(pNodeList, "FileName", pos);
				CString FileType = GetXmlValue(pNodeList, "FileType", pos);
				CString DstPath = GetXmlValue(pNodeList, "DstPath", pos);
				CString FileDate = GetXmlValue(pNodeList, "FileDate", pos);

				int ft = atoi((LPCSTR)FileType);
				long filedate = atol((LPCSTR)FileDate);
				if(localt<filedate){
					if(!FileName.IsEmpty() && !DstPath.IsEmpty() && !Type.IsEmpty() && !URL.IsEmpty()){
						SynReadmanAdv(FileName, DstPath, URL, ft, Type);
						curtime = filedate>curtime?filedate:curtime;
					}
				}
			}
			pNodeList.Release();
		}
		pDoc.Release();

		char date[20] = {0};
		sprintf_s(date, 20, "%ld", curtime);
		
		WritePrivateProfileString("PUSH", "ReadmanDate", date, iniPath);
	}
	catch(_com_error& e){}
	catch(int errnum){
		switch(errnum){
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			break;
		}
		return -1;
	}
	return 0;
}

UINT CPushAdv::SynReadmanAdv(CString FileName, CString DstPath, CString URL,
								int FileType, CString Type)
{
	BOOL flag = FALSE;

	char LocalPath[_MAX_PATH] = {0};
	sprintf_s(LocalPath, _MAX_PATH, "%s\\_Adv", FavorPath);
	_mkdir(LocalPath);

	DstPath.Replace('/', '\\');
	char DestinationPath[_MAX_PATH] = {0};
	strcpy_s(DestinationPath, _MAX_PATH, DstPath);

	char* start = DestinationPath;
	char* cur = NULL;



	while(cur = strchr(start, '\\')){
		*cur=0;
		strcat_s(LocalPath, _MAX_PATH, "\\");
		strcat_s(LocalPath, _MAX_PATH, start);
		_mkdir(LocalPath);
		start = cur+1;
		if(!start)
			break;
	}
	
	if(!cur){
		strcat_s(LocalPath, _MAX_PATH, "\\");
		strcat_s(LocalPath, _MAX_PATH, start);
		_mkdir(LocalPath);
	}
	strcat_s(LocalPath, _MAX_PATH, "\\");
	strcat_s(LocalPath, _MAX_PATH, FileName);

	if(!DownLoad(URL, LocalPath, pMainWnd->m_quit, pMainWnd->m_socketList, pMainWnd->m_mutex))
		return -1;

	return 0;
}

UINT CPushAdv::SearchAndCopy(CString localPath, CString DestPath)
{
	CFileFind find;
	localPath += "\\*";
	BOOL Ext = find.FindFile(localPath);
	while(Ext){
		Ext = find.FindNextFile();
		if(find.IsDots())
			continue;
		if(find.IsDirectory()){
			CString newPath;
			newPath.Format("%s\\%s", DestPath, find.GetFileTitle());
			if(PathFileExists(newPath))
				DeleteAllFiles(newPath);
			else
				_mkdir(newPath);
			SearchAndCopy(find.GetFilePath(), newPath);
			continue;
		}
		CString destFilePath;
		destFilePath.Format("%s\\%s", DestPath, find.GetFileName());
		CopyFile(find.GetFilePath(), destFilePath, FALSE);
	}
	find.Close();
	return 0;
}

UINT CPushAdv::ToSDCardAdv()
{
	char LocalPath[_MAX_PATH];
	sprintf_s(LocalPath, _MAX_PATH, "%s\\_Adv", FavorPath);
	if(!PathFileExists(LocalPath))
		return -1;

	char iniPath[_MAX_PATH] = {0};
	char val[20] = {0};	//推送时间
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\sysinfo.ini", CurrentPath);
	GetPrivateProfileString("PUSH", "ReadmanDate", NULL, val, 20, iniPath);

	char sdiniPath[_MAX_PATH] = {0};
	char sdval[20] = {0};
	sprintf_s(sdiniPath, _MAX_PATH, "%s\\info.ini", SDDisk);
	if(PathFileExists(sdiniPath)){
		GetPrivateProfileString("info", "UpdataDate", NULL, sdval, 20, sdiniPath);
		if(strlen(sdval)>0){
			if(strlen(val)>0){
				if(atoi(sdval)>=atoi(val))	//sd卡时间 不小于 本地时间
					return 1;
			}
			else{
				return 1;
			}
		}
	}

	char DestPath[_MAX_PATH] = {0};
	sprintf_s(DestPath, _MAX_PATH, "%s\\", SDDisk);
	_mkdir(DestPath);
	
	SearchAndCopy(LocalPath, DestPath);

	WritePrivateProfileString("info", "UpdataDate", val, sdiniPath);
	
	return 0;
}

void CPushAdv::DeleteAllFiles(CString Path){
	CFileFind find;
	CString tPath;
	tPath.Format("%s/*", Path);
	BOOL ext = find.FindFile(tPath);
	while(ext){
		ext = find.FindNextFile();
		if(find.IsDots())
			continue;
		if(find.IsDirectory()){
			DeleteAllFiles(find.GetFilePath());
			continue;
		}
		DeleteFile(find.GetFilePath());
	}
	find.Close();
	_rmdir(Path);
}
