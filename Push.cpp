// Push.cpp: implementation of the CPush class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Readings.h"
#include "Push.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern _ConnectionPtr m_pConnection;

extern char CurrentPath[_MAX_PATH];
extern char AspServer[1024];
extern char UserName[50];
extern char FavorPath[_MAX_PATH];
extern CMainFrame* pMainWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPush::CPush()
{

}

CPush::~CPush()
{

}

BOOL CPush::PushInfoToDB()
{
BOOL resFlag = FALSE;
try{
	char HttpAdd[1024] = {0};
	char Tadd[1024] = {0};
	sprintf_s(HttpAdd, 1024, "%s/push.asp?id=%s", AspServer, UserName);
	sprintf_s(Tadd, 1024, "%s\\temp\\pushinfo.xml", CurrentPath);
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try{
		if(!DownLoad(HttpAdd,Tadd,pMainWnd->m_quit,pMainWnd->m_socketList,pMainWnd->m_mutex))
			throw 0;	//xml 下载失败
		
		HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load(Tadd))
			throw 2;	//xml 解析失败
		//上次更新时间
		char iniPath[_MAX_PATH] = {0};
		sprintf_s(iniPath, _MAX_PATH, "%s\\system\\sysinfo.ini", CurrentPath);
		char timeini[100] = {0};
		GetPrivateProfileString("PUSH", "date", "", timeini, 100, iniPath);
		//本次推送更新时间
		pNodeList = pDoc->selectNodes("PushInfo");
		if(pNodeList == NULL)
			throw 2;	//xml	格式错误
		CString pubtime = GetXmlValue(pNodeList, "PubDate", 0);
		DWORD localt, sevtm;
		if(strlen(timeini)>0){	
			localt = (DWORD)atol(timeini);//上次更新时间 位于 system/sysinfo.ini
			sevtm = (DWORD)atol((LPCSTR)pubtime);
			if(localt>=sevtm){
				throw 3;	
			}//已经是最新
		}
		else
			localt = 0;
		pNodeList = pDoc->selectNodes("PushInfo/Others/Source");
		int pos2=0;
		if(pNodeList){
			_RecordsetPtr m_pRecordset;
			char SQL[1000]={0};
			sprintf_s(SQL, 1000, "DELETE FROM pushlist WHERE updatetype=1 AND user='%s'", UserName);
			m_pConnection->Execute(SQL,NULL,adCmdText);   
			m_pRecordset.CreateInstance(__uuidof(Recordset));
			memset(SQL, 0, 1000);
			strcpy_s(SQL, 1000, "SELECT * FROM pushlist");
			HRESULT dbhr = m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
			adOpenDynamic, adLockOptimistic, adCmdText);
			if(SUCCEEDED(dbhr)){
				for(pos2=0; pos2<pNodeList->length; pos2++){
					CString URL = GetXmlValue(pNodeList, "SourceURL", pos2);
					CString name = GetXmlValue(pNodeList, "SourceName", pos2);
					CString type = GetXmlValue(pNodeList, "SourceType", pos2);
					CString title = GetXmlValue(pNodeList, "SourceTitle", pos2);
					CString filetype = GetXmlValue(pNodeList, "SourceFiletype", pos2);
					CString remoteid = GetXmlValue(pNodeList, "SourceId", pos2);
					CString updatetype = GetXmlValue(pNodeList, "SourceUpdateType", pos2);
					CString date = GetXmlValue(pNodeList, "Date", pos2);

					if(!URL.IsEmpty() && !name.IsEmpty() && !type.IsEmpty() 
						&& !title.IsEmpty() && !filetype.IsEmpty() && !remoteid.IsEmpty() 
						&& !updatetype.IsEmpty()){
						DWORD ddate = (DWORD)atol((LPCSTR)date);
						if((atoi((LPCSTR)updatetype)==1 && ddate>=localt)	//报纸
							|| atoi((LPCSTR)updatetype)!=1){					//杂志
							m_pRecordset->AddNew();
							m_pRecordset->PutCollect("title",(_variant_t)title);
							m_pRecordset->PutCollect("name",(_variant_t)name);
							m_pRecordset->PutCollect("filetype",(_variant_t)atol((LPCSTR)filetype));
							m_pRecordset->PutCollect("url",(_variant_t)URL);
							m_pRecordset->PutCollect("type",(_variant_t)type);
							m_pRecordset->PutCollect("remoteid",(_variant_t)atol((LPCSTR)remoteid));
							m_pRecordset->PutCollect("updatetype",(_variant_t)atol((LPCSTR)updatetype));
							m_pRecordset->PutCollect("user",(_variant_t)UserName);
							m_pRecordset->PutCollect("state",(_variant_t)0l);
							m_pRecordset->Update();
						}
					}
				}
				m_pRecordset->Close();
			}
			pNodeList.Release();
//			m_pRecordset.Release();
		}
		pDoc.Release();
		if(pos2!=0)
			resFlag = TRUE;
	}
	catch(_com_error& e){}
	catch(int errnum){
		switch(errnum){
		case 3:		//已更新
			pNodeList.Release();
		case 2:		//格式错误
			pDoc.Release();
			AfxMessageBox("Error in ");
			break;
		}
		return FALSE;
	}
}
catch(...){
	Tips("unsigned __stdcall CMainFrame::_Push(LPVOID pParam)");
}

	return resFlag;
}

BOOL CPush::DownLoadPushList()
{
	static BOOL first = TRUE;
	if(!first)
		return FALSE;

	first = FALSE;

	BOOL res = FALSE;

	_RecordsetPtr m_pRecordset;
	char SQL[1000]={0};
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	while(TRUE&&!pMainWnd->m_quit){
		try{
			memset(SQL, 0, 1000);
			sprintf_s(SQL, 1000, "SELECT TOP 1 * FROM pushlist WHERE user='%s' ORDER BY updatetype ASC", UserName);
			m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
				adOpenDynamic, adLockOptimistic, adCmdText);
			if(m_pRecordset->EndOfFile){
				m_pRecordset->Close();
				break;
			}
			CString url = GetCollectString(m_pRecordset, "url");
			CString title = GetCollectString(m_pRecordset, "title");
			CString name = GetCollectString(m_pRecordset, "name");
			CString type = GetCollectString(m_pRecordset, "type");
			long filetype = GetCollectLong(m_pRecordset, "filetype");
			m_pRecordset->Delete(adAffectCurrent); 

			CString path;
			path.Format("%s\\%s", FavorPath, type);
			_mkdir(path);

			//推送文件夹的标志文件
			char flagPath[_MAX_PATH] = {0};
			sprintf_s(flagPath, _MAX_PATH, "%s/push", path);
			FILE* flagfp = fopen(flagPath, "wb");
			if(flagfp){
				fclose(flagfp);
			}

			path += '/';
			path += name;
			path += ".nd";
			
			if(DownLoad(url,path,pMainWnd->m_quit,pMainWnd->m_socketList,pMainWnd->m_mutex)){
				m_pRecordset->Close();
				CString newPath;
				newPath = path.Left(path.GetLength()-3);
				DeleteFile(newPath);
				rename(path, newPath);
				if(filetype==wolFileType)	//只有为wol文件默认为同步
					CreateSYN(newPath);
				CFile cf;
				if(cf.Open(newPath, CFile::modeRead)){
					long len = cf.GetLength();
					cf.Close();
					CString slen;
					slen.Format("%d", len);
					memset(SQL, 0, 1000);
					sprintf_s(SQL, 1000, "DELETE from push where path=\"%s\"", newPath);
					m_pConnection->Execute(SQL, NULL,adCmdText);
					memset(SQL, 0, 1000);
					strcpy_s(SQL, 1000, "select * from push");
					m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
						adOpenDynamic, adLockOptimistic, adCmdText);
					m_pRecordset->AddNew();
					m_pRecordset->PutCollect("title",(_variant_t)title);
					m_pRecordset->PutCollect("path",(_variant_t)newPath);
					m_pRecordset->PutCollect("type",(_variant_t)type);
					m_pRecordset->PutCollect("filetype",(_variant_t)filetype);
					m_pRecordset->PutCollect("size",(_variant_t)slen);
					m_pRecordset->PutCollect("cdate",(_variant_t)GetDate());
					m_pRecordset->PutCollect("user",(_variant_t)UserName);
					m_pRecordset->Update();
					res = TRUE;
				}
			}
			m_pRecordset->Close();
		}
		catch(_com_error& e){
			break;
		}
	}
//	m_pRecordset.Release();
	first = TRUE;
	return res && !pMainWnd->m_quit;
}

