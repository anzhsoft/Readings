// stdafx.cpp : source file that includes just the standard includes
//	Readings.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "HttpSocket.h"
#include "Readings.h"
#include "MainFrm.h"

extern char CurrentPath[_MAX_PATH];
extern CMainFrame* pMainWnd;

void Tips(CString msg, UINT type, UINT delay)
{
// 	CFile rf;
// 	rf.Open("c:\\lalal.txt",CFile::modeCreate||CFile::modeReadWrite);
// 	rf.Write((LPCSTR)msg,msg.GetLength());
//	rf.Close();
	if (pMainWnd->m_quit)
	{
		return;
	}
	#ifdef _DEBUG
	pMainWnd->ShowTip(type,msg,delay*1000);
	#endif
}

BOOL DownLoad(LPCSTR URL, LPCSTR Path, BOOL& quit, CSocketList& list)
{
try{
	
	CHttpSocket HttpSocket;
	CFile DownloadFile;
	CString strServer,strObject;
	unsigned short nPort;
	DWORD dwServiceType;
	long nLength;
	const char *pRequestHeader = NULL;
	SOCKET tmpS = 0;
	if(!AfxParseURL(URL,dwServiceType,strServer,strObject,nPort)){
		//AfxMessageBox("AfxParseURL");
		return FALSE;
	}
	try{
		pRequestHeader = HttpSocket.FormatRequestHeader((LPTSTR)(LPCTSTR)strServer,(LPTSTR)(LPCTSTR)strObject,nLength, nPort);	
		if(!HttpSocket.Socket())
			throw 0;
		tmpS = HttpSocket.GetSocket();
		list.AddTail(tmpS);
		if(quit){
			//AfxMessageBox("quit");
			return FALSE;
		}
//		HttpSocket.SetTimeout(10000,0);
		if(!HttpSocket.Connect((LPTSTR)(LPCTSTR)strServer,nPort))
			throw 1;
		if(quit)
			return FALSE;
		if(!HttpSocket.SendRequest())
			throw 1;
		if(quit)
			return FALSE;
		int nLineSize = 0;
		char szLine[256];
		while(nLineSize != -1){
			nLineSize = HttpSocket.GetResponseLine(szLine,256);
			if(quit)
				return FALSE;
		}
		char szValue[30];
		BOOL bContLen = TRUE;
		int nFileSize = -1;
		if(HttpSocket.GetField("Content-Length",szValue,30)==-1)
			bContLen = FALSE;
		else
			nFileSize = atoi(szValue);
		if(quit)
			return FALSE;
//		int nSvrState = HttpSocket.GetServerState();
		int nCompletedSize = 0;
		if(!DownloadFile.Open(Path, CFile::modeCreate|CFile::modeWrite))
			throw 2;
		if(quit)
			return FALSE;
		char pData[1024];
		int nReceSize = 0;
		BOOL first = TRUE;
		while(!quit){
			nReceSize = HttpSocket.Receive(pData,1024);
			if(quit)
				return FALSE;
			if(nReceSize == 0)
				break;
			if(nReceSize == -1)
				throw 3;
			if(first&&!bContLen){
				char* temp = strstr(pData,"\n");
				if(!temp)
					throw 3;
				DownloadFile.Write(temp+2,nReceSize-(temp+2-pData));
			}
			else
				DownloadFile.Write(pData,nReceSize);
			nCompletedSize += nReceSize;
			first = FALSE;
			if(bContLen && nCompletedSize>=nFileSize)
				break;
		}
		if(!bContLen && !quit){
			long len = (long)DownloadFile.GetLength();
			DownloadFile.SetLength(len-7);
		}
		DownloadFile.Close();
		POSITION pos = list.Find(tmpS);
		if(pos)
			list.RemoveAt(pos);
		HttpSocket.CloseSocket();
	}
	catch(int err){
		POSITION pos;
		switch(err){
		case 3:
			DownloadFile.Close();
		case 2:
		case 1:
			pos = list.Find(tmpS);
			if(pos)
				list.RemoveAt(pos);
			HttpSocket.CloseSocket();
			break;
		}
		return FALSE;
	}
	return !quit;
}
catch(...){
#ifdef _DEBUG
//	AfxMessageBox("BOOL DownLoad(LPCSTR URL, LPCSTR Path, BOOL& quit, CSocketList& list)");
#endif
}
	return FALSE;
}
//added by zhanganzhan on 20090317 to open a url on 20090317
BOOL UpLoad(LPCSTR URL)//, BOOL& quit, CSocketList& list, CMutex& mutex)
{
try{
	CHttpSocket HttpSocket;
	CFile DownloadFile;
	CString strServer,strObject;
	unsigned short nPort;
	DWORD dwServiceType;
	long nLength;
	const char *pRequestHeader = NULL;
	SOCKET tmpS = 0;
	if(!AfxParseURL(URL,dwServiceType,strServer,strObject,nPort))
		return FALSE;
	try{
		pRequestHeader = HttpSocket.FormatRequestHeader((LPTSTR)(LPCTSTR)strServer,(LPTSTR)(LPCTSTR)strObject,nLength, nPort);	
		if(!HttpSocket.Socket())
			throw 0;
		tmpS = HttpSocket.GetSocket();

		if(!HttpSocket.Connect((LPTSTR)(LPCTSTR)strServer,nPort))
			throw 1;
		if(!HttpSocket.SendRequest())
			throw 1;
		
		int nLineSize = 0;
		char szLine[256];
		while(nLineSize != -1)
			nLineSize = HttpSocket.GetResponseLine(szLine,256);
		
		HttpSocket.CloseSocket();
	}
	catch(int err){
// 		POSITION pos;
// 		CSingleLock SingleLock(&mutex);
		switch(err){
// 		case 3:
// 			DownloadFile.Close();
		case 2:
		case 1:
			HttpSocket.CloseSocket();
			break;
		}
		return FALSE;
	}
	return TRUE;

}
catch(...){
#ifdef _DEBUG
	AfxMessageBox("BOOL DownLoad(LPCSTR URL, LPCSTR Path, BOOL& quit, CSocketList& list)");
#endif
}
	return FALSE;
}
BOOL DownLoad(LPCSTR URL, LPCSTR Path, BOOL& quit, CSocketList& list, CMutex& mutex)
{
try{
	CHttpSocket HttpSocket;
	CFile DownloadFile;
	CString strServer,strObject;
	unsigned short nPort;
	DWORD dwServiceType;
	long nLength;
	const char *pRequestHeader = NULL;
	SOCKET tmpS = 0;
	if(!AfxParseURL(URL,dwServiceType,strServer,strObject,nPort))
	{
		return FALSE;
	}
	try{
		pRequestHeader = HttpSocket.FormatRequestHeader((LPTSTR)(LPCTSTR)strServer,(LPTSTR)(LPCTSTR)strObject,nLength, nPort);	
		if(!HttpSocket.Socket())
			throw 0;
		tmpS = HttpSocket.GetSocket();

		CSingleLock SingleLock(&mutex);
		SingleLock.Lock();
		if(SingleLock.IsLocked())
			list.AddTail(tmpS);
		SingleLock.Unlock();
		
		if(quit){
			return FALSE;
		}
//		HttpSocket.SetTimeout(20000,0);
		if(!HttpSocket.Connect((LPTSTR)(LPCTSTR)strServer,nPort))
			throw 1;
		if(quit)
			return FALSE;
		if(!HttpSocket.SendRequest())
			throw 1;
		if(quit)
			return FALSE;
		int nLineSize = 0;
		char szLine[256];
		while(nLineSize != -1){
			nLineSize = HttpSocket.GetResponseLine(szLine,256);
			if(quit)
				return FALSE;
		}
		char szValue[30];
		BOOL bContLen = TRUE;
		int nFileSize = -1;
		if(HttpSocket.GetField("Content-Length",szValue,30)==-1)
			bContLen = FALSE;
		else
			nFileSize = atoi(szValue);
		if(quit)
			return FALSE;
		int nCompletedSize = 0;
		if(!DownloadFile.Open(Path, CFile::modeCreate|CFile::modeWrite))
			throw 2;
		char pData[1024];
		int nReceSize = 0;
		BOOL first = TRUE;
		while(!quit){
			nReceSize = HttpSocket.Receive(pData,1024);
			if(quit)
				return FALSE;
			if(nReceSize == 0)
				break;
			if(nReceSize == -1)
				throw 3;
			if(first&&!bContLen){
				char* temp = strstr(pData,"\r\n");
				if(!temp)
					throw 3;
				DownloadFile.Write(temp+2,nReceSize-(temp+2-pData));
			}
			else
				DownloadFile.Write(pData,nReceSize);
			nCompletedSize += nReceSize;
			first = FALSE;
			if(bContLen && nCompletedSize>=nFileSize)
				break;
		}
		if(!bContLen && !quit){
			long len =(long) DownloadFile.GetLength();
			DownloadFile.SetLength(len-7);
		}
		DownloadFile.Close();
		SingleLock.Lock();
		if(SingleLock.IsLocked()){
			POSITION pos = list.Find(tmpS);
			if(pos)
				list.RemoveAt(pos);
		}
		SingleLock.Unlock();
		
		HttpSocket.CloseSocket();
	}
	catch(int err){
		POSITION pos;
		CSingleLock SingleLock(&mutex);
		switch(err){
		case 3:
			DownloadFile.Close();
		case 2:
		case 1:
			SingleLock.Lock();
			if(SingleLock.IsLocked()){
				pos = list.Find(tmpS);
				if(pos)
					list.RemoveAt(pos);
			}
			SingleLock.Unlock();
			HttpSocket.CloseSocket();
			break;
		}
		return FALSE;
	}
	return !quit;
}
catch(...){
#ifdef _DEBUG
	Tips("BOOL DownLoad(LPCSTR URL, LPCSTR Path, BOOL& quit, CSocketList& list)");
#endif
}
	return FALSE;
}

CString GetXmlValue(MSXML::IXMLDOMNodeListPtr pNodeList, char* key, int pos)
{
	try{
		if(pNodeList==NULL)
			return "";
		MSXML::IXMLDOMNodePtr pSubNode;
		pSubNode = pNodeList->item[pos]->selectSingleNode(key);
		if(pSubNode==NULL)
			return "";
		_bstr_t strRequestType = pSubNode->Gettext();
		CString res;
		res.Format("%s",(char*)strRequestType);
		pSubNode.Release();
		return res;
	}
	catch(_com_error &e){
#ifdef _DEBUG
		AfxMessageBox(e.ErrorMessage());
#endif
		return "";
	}
}

CString GetCollectString(_RecordsetPtr m_pRecordset, char* key)
{
	try{
		VARIANT var = m_pRecordset->GetCollect(key);
		CString res;
		if(var.vt == VT_NULL)
			return res;
		res.Format("%s", (char*)(_bstr_t)var);
		return res;
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
	}
	return "";
}

int GetCollectInt(_RecordsetPtr m_pRecordset, char* key)
{
	try{
		VARIANT var = m_pRecordset->GetCollect(key);
		if(var.vt == VT_NULL)
			return NULL;
		return var.intVal;
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
	}
	return NULL;
}

BYTE GetCollectByte(_RecordsetPtr m_pRecordset, char* key)
{
	try{
		VARIANT var = m_pRecordset->GetCollect(key);
		if(var.vt == VT_NULL)
			return NULL;
		return var.intVal;
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
	}
	return NULL;
}

long GetCollectLong(_RecordsetPtr m_pRecordset, char* key)
{
	try{
		VARIANT var = m_pRecordset->GetCollect(key);
		if(var.vt == VT_NULL)
			return NULL;

		return var.lVal;
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
	}
		return NULL;
}

long GetCollectMyLong(_RecordsetPtr m_pRecordset, char* key)
{
	try{
		VARIANT var = m_pRecordset->GetCollect(key);
		if(var.vt == VT_NULL)
			return NULL;

		return (long)var.dblVal;
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
	}
		return NULL;
}

CString GetCollectDate(_RecordsetPtr m_pRecordset, char* key)
{
	try{
		VARIANT var = m_pRecordset->GetCollect(key);
		if(var.vt == VT_NULL)
			return "";
		
		SYSTEMTIME st;
		VariantTimeToSystemTime(var.date, &st);
		CString date;
		date.Format("%d-%02d-%02d %02d:%02d", st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute);
		return date;
	}
	catch(_com_error& e){
		AfxMessageBox(e.ErrorMessage());
	}
		return "";
}

CString GetDate(int offset, BOOL ndHMS)
{
	CString res;
	//SYSTEMTIME st;
	//GetLocalTime(&st);
	CTime st = CTime::GetCurrentTime();
	CTimeSpan span(offset,0,0,0);
	st = st+span;
	if(ndHMS)
		res.Format("%d-%.2d-%.2d %.2d:%.2d:%.2d",st.GetYear(),st.GetMonth(),st.GetDay(),st.GetHour(),st.GetMinute(),st.GetSecond());
	else
		res.Format("%d-%.2d-%.2d",st.GetYear(),st.GetMonth(),st.GetDay());
	return res;
}

void TernminateAllThread(CSocketList& sockList, CThreadList& threadList)
{
try{
	if(!sockList.IsEmpty()){
		POSITION pos = sockList.GetHeadPosition();
		while(pos){
			SOCKET s = sockList.GetNext(pos);
			::closesocket(s);
		}
	}

	if(!threadList.IsEmpty())
	{
		POSITION pos = threadList.GetHeadPosition();
		while(pos)
		{
			HANDLE hdl_thread = threadList.GetNext(pos);
			while(TRUE)
			{
				if(::WaitForSingleObject(hdl_thread, 0) == WAIT_OBJECT_0)
					break;
				MSG msg;
				while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
				{ 
					if (!AfxGetApp()->PumpMessage()) 
						break; 
				}
			}
		}
		threadList.RemoveAll();//added by zuoss 20090108
	}	
/*	if(!threadList.IsEmpty())
	{
		int n = 0;
		HANDLE phandle[2000] = {0}; 
		POSITION pos = threadList.GetHeadPosition();
		while(pos)
		{
			HANDLE hdl_thread = threadList.GetNext(pos);
			phandle[n] = hdl_thread;
			n++;	
		}
		::WaitForMultipleObjects(n,phandle,true,1000);
	}*/	
}
catch(...){
#ifdef _DEBUG
	AfxMessageBox("TernminateAllThread");
#endif
}
}

/*void TernminateAllThread(CClientList& sockList, CWinThreadList& threadList)
{
try{
	if(!sockList.IsEmpty()){
		POSITION pos = sockList.GetHeadPosition();
		while(pos){
			SOCKET s = sockList.GetNext(pos);
			UDT::close(s);
		}
	}

	if(!threadList.IsEmpty()){
		POSITION pos = threadList.GetHeadPosition();
		while(pos){
			CWinThread* pThread = threadList.GetNext(pos);
			while(TRUE){
				if(::WaitForSingleObject(pThread->m_hThread, 0) == WAIT_OBJECT_0)
					break;
				MSG msg;
				while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ 
					if (!AfxGetApp()->PumpMessage()) 
						break; 
				} 
			}
		}
	}

}
catch(...){
#if _DEBUG
	AfxMessageBox("void TernminateAllThread(CClientList& sockList, CWinThreadList& threadList)");
#endif
}
}
*/

int GetFileTypeFromExt(CString type)
{
	if(_stricmp(type, "wol")==0)
		return wolFileType;
	else if(_stricmp(type, "html")==0 || _stricmp(type, "htm")==0)
		return htmFileType;
	else if(_stricmp(type, "txt")==0)
		return txtFileType;
	else if(_stricmp(type, "doc")==0)
		return docFileType;
	else if(_stricmp(type, "pdf")==0)
		return pdfFileType;
	else if(_stricmp(type, "bmp")==0)
		return bmpFileType;
	else if(_stricmp(type, "jpg")==0)
		return jpgFileType;
	else if(_stricmp(type, "gif")==0)
		return gifFileType;
	else if(_stricmp(type, "zip")==0)
		return zipFileType;
	else if(_stricmp(type, "rar")==0)
		return rarFileType;
	else if(_stricmp(type, "chm")==0)
		return chmFileType;
	else if(_stricmp(type, "exe")==0)
		return magFileType;
	else if(_stricmp(type, "mabs")==0)
		return absmagFileType;
	else if(_stricmp(type, "ppt")==0)
		return pptFileType;
	else if(_stricmp(type, "png")==0)
		return pngFileType;
	else if(_stricmp(type, "wav")==0)
		return wavFileType;
	else if(_stricmp(type, "mp3")==0)
		return mp3FileType;
	else if(_stricmp(type, "avi")==0)
		return aviFileType;
	else if(_stricmp(type, "rm")==0 || _stricmp(type, "rmvb")==0)
		return rmFileType;
	else if(_stricmp(type, "wmv")==0 || _stricmp(type, "wma")==0)
		return wmvFileType;
	else if(_stricmp(type, "asf")==0)
		return asfFileType;
	else if(_stricmp(type, "3gp")==0)
		return _3gpFileType;
	else  if(_stricmp(type,"ttf")==0)
		return ttfFileType;
	else
		return unknownFileType;
	
}

CString GetFileIcon(int type)
{
	CString picPath;
	switch(type){
	case wolFileType:
		picPath.Format("%s/system/icon/small/wolsmall.bmp", CurrentPath);
		return picPath;
	case txtFileType:
		picPath.Format("%s/system/icon/small/txtsmall.bmp", CurrentPath);
		return picPath;
	case htmFileType:
		picPath.Format("%s/system/icon/small/htmsmall.bmp", CurrentPath);
		return picPath;
	case docFileType:
		picPath.Format("%s/system/icon/small/docsmall.bmp", CurrentPath);
		return picPath;
	case pdfFileType:
		picPath.Format("%s/system/icon/small/pdfsmall.bmp", CurrentPath);
		return picPath;
	case bmpFileType:
		picPath.Format("%s/system/icon/small/bmpsmall.bmp", CurrentPath);
		return picPath;
	case jpgFileType:
		picPath.Format("%s/system/icon/small/jpgsmall.bmp", CurrentPath);
		return picPath;
	case gifFileType:
		picPath.Format("%s/system/icon/small/gifsmall.bmp", CurrentPath);
		return picPath;
	case zipFileType:
	case rarFileType:
		picPath.Format("%s/system/icon/small/rarsmall.bmp", CurrentPath);
		return picPath;
	case chmFileType:
		picPath.Format("%s/system/icon/small/chmsmall.bmp", CurrentPath);
		return picPath;
	case magFileType:
		picPath.Format("%s/system/icon/small/magsmall.bmp", CurrentPath);
		return picPath;
	case absmagFileType:
		picPath.Format("%s/system/icon/small/magsmall.bmp", CurrentPath);
		return picPath;
	case pptFileType:
		picPath.Format("%s/system/icon/small/pptsmall.bmp", CurrentPath);
		return picPath;
	case mp3FileType:
		picPath.Format("%s/system/icon/small/mp3small.bmp", CurrentPath);
		return picPath;
	case aviFileType:
		picPath.Format("%s/system/icon/small/avismall.bmp", CurrentPath);
		return picPath;
	case wmvFileType:
	case rmFileType:
	case asfFileType:
	case _3gpFileType:
		picPath.Format("%s/system/icon/small/videosmall.bmp", CurrentPath);
		return picPath;
	case wavFileType:
		picPath.Format("%s/system/icon/small/audiosmall.bmp", CurrentPath);
		return picPath;
	case pngFileType:
		picPath.Format("%s/system/icon/small/pngsmall.bmp", CurrentPath);
		return picPath;
	case unknownFileType:
		picPath.Format("%s/system/icon/small/zwfm.bmp", CurrentPath);
		return picPath;
	default:
		picPath.Format("%s/system/icon/small/zwfm.bmp", CurrentPath);
		return picPath;
	}
	picPath.Format("%s/system/icon/small/zwfm.bmp", CurrentPath);
	return picPath;
}

CString GetFileExtentName(int type)
{
	CString ExtName;
	switch(type){
	case wolFileType:
		ExtName = "wol";
		return ExtName;
	case txtFileType:
		ExtName = "txt";
		return ExtName;
	case htmFileType:
		ExtName = "htm";
		return ExtName;
	case docFileType:
		ExtName = "doc";
		return ExtName;
	case pdfFileType:
		ExtName = "pdf";
		return ExtName;
	case bmpFileType:
		ExtName = "bmp";
		return ExtName;
	case jpgFileType:
		ExtName = "jpg";
		return ExtName;
	case gifFileType:
		ExtName = "gif";
		return ExtName;
	case zipFileType:
		ExtName = "zip";
		return ExtName;
	case rarFileType:
		ExtName = "rar";
		return ExtName;
	case chmFileType:
		ExtName = "chm";
		return ExtName;
	case magFileType:
		ExtName = "exe";
		return ExtName;
	case absmagFileType:
		ExtName = "mabs";
		return ExtName;
	case pptFileType:
		ExtName = "ppt";
		return ExtName;
	case wavFileType:
		ExtName = "wav";
		return ExtName;
	case mp3FileType:
		ExtName = "mp3";
		return ExtName;
	case wmvFileType:
		ExtName = "wmv";
		return ExtName;
	case rmFileType:
		ExtName = "rm";
		return ExtName;
	case aviFileType:
		ExtName = "avi";
		return ExtName;
	case asfFileType:
		ExtName = "asf";
		return ExtName;
	case _3gpFileType:
		ExtName = "3gp";
		return ExtName;
	case pngFileType:
		ExtName = "png";
		return ExtName;
	case unknownFileType:
		return "unknown";
	default:
		return "unknown";
	}
	return "unknown";
}

CString GetExtNameFromFull(CString FullName)
{
	int index = FullName.ReverseFind('.');
	if(index<0)
		return "";
	return FullName.Right(FullName.GetLength()-index-1);
}

int GetFileTypeFromFull(CString FullName)
{
	return GetFileTypeFromExt(GetExtNameFromFull(FullName));
}

void CreateSYN(CString FilePath)
{
	FilePath += ".syn";
	CFile cf;
	if(!cf.Open(FilePath, CFile::modeCreate|CFile::modeWrite))
		return;
	cf.Close();
}

void DeleteSYN(CString FilePath)
{
	FilePath += ".syn";
	DeleteFile(FilePath);
}

SYNFLAG ExistSYN(CString FilePath)
{
	FilePath += ".syn";
	return PathFileExists(FilePath)?syn_yes:syn_no;
}

UINT GetSmallIcon(int filetype)
{
	UINT icontype = 0;
	switch(filetype){
	case wolFileType:
		icontype = 10;
		break;
	case txtFileType:
		icontype = 1;
		break;
	case htmFileType:
		icontype = 5;
		break;
	case docFileType:
		icontype = 3;
		break;
	case pdfFileType:
		icontype = 8;
		break;
	case chmFileType:
		icontype = 9;
		break;
	case jpgFileType:
		icontype = 7;
		break;
	case bmpFileType:
		icontype = 2;
		break;
	case gifFileType:
		icontype = 4;
		break;
	case rarFileType:
		icontype = 0;
		break;
	case zipFileType:
		icontype = 0;
		break;
	case pptFileType:
		icontype = 13;
		break;
	case syn_moveup:
		icontype = 12;
		break;
	case syn_movedown:
		icontype = 11;
		break;
	default:
		icontype = 14;
		break;
	}
	return icontype;
}

BOOL WriteToFile(CString Path ,char* buff, long len)
{
	CFile ofile;
	if(!ofile.Open(Path, CFile::modeCreate))
	{
		Tips("ofile "+CString(Path)+" create");
		return FALSE;
	}
	ofile.Close();
	if(!ofile.Open(Path, CFile::modeReadWrite))
	{
		Tips("ofile "+CString(Path)+" write");
		return FALSE;
	}
	//AfxMessageBox(Tadd);
	ofile.Write(buff, len);
	ofile.Close();
	return TRUE;
}
