// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__ED834953_D4A7_4F79_8856_D1A8E07BEA60__INCLUDED_)
#define AFX_STDAFX_H__ED834953_D4A7_4F79_8856_D1A8E07BEA60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <atlbase.h>
#include <afxtempl.h>
#include <afxcview.h>
#include <shlwapi.h>
#include <winsock2.h>
#include <afxinet.h>
#include <direct.h>
#include <msxml.h>
#include <afxmt.h>
#include <process.h>

#include "resource.h"

#include "lib/udt/udt.h"

#import "msxml.dll" //no_namespace
//using namespace MSXML;

#import "msado15.dll" no_namespace rename("EOF", "EndOfFile")

#import  "jmail.dll"
using namespace jmail;

#include "p2p.h"

#include "variable.h"
#include "struct.h"
#include "MSG.h"
#include "PW.h"
#include "EMailADO.h"
#include "RequestMail.h"
#include "ForJmail.h"
#include "gfxgroupedit.h"
#include "gfxoutbarctrl.h"
#include "gfxpopupmenu.h"
#include "Drawers.h"
#include "DrawerView.h"
#include "MainFrm.h"
#include "DownloadCenterTreeView.h"
#include "HotKeyTree.h"
#define TIME_1_MINUTE 60*1000
#define TIME_10_SECOND 10*1000
#define TIME_30_SECOND 30*1000
#define TIME_2_MINUTE 120*1000
#define TIME_5_MINUTE 300*1000

//typedef int (CALLBACK* pDownload1)(const char *Url, const char *SaveFileAdd); //œ¬‘ÿ∫Ø ˝÷∏’Î
const char EncyStr[20] = {"www.jinke.com.cn.sl"};

void Tips(CString msg, UINT type = td_msg, UINT delay = 5);
BOOL DownLoad(LPCSTR URL, LPCSTR Path, BOOL& quit, CSocketList& list);
BOOL DownLoad(LPCSTR URL, LPCSTR Path, BOOL& quit, CSocketList& list, CMutex& mutex);
BOOL UpLoad(LPCSTR URL);//added by zhanganzhan to open a url on 20090317

CString GetXmlValue(MSXML::IXMLDOMNodeListPtr pNodeList, char* key, int pos);
CString GetCollectString(_RecordsetPtr m_pRecordset, char* key);
int GetCollectInt(_RecordsetPtr m_pRecordset, char* key);
long GetCollectLong(_RecordsetPtr m_pRecordset, char* key);
long GetCollectMyLong(_RecordsetPtr m_pRecordset, char* key); //add by yangxiao 2008.8.22
BYTE GetCollectByte(_RecordsetPtr m_pRecordset, char* key);  //add by yangxiao 2008.8.8
CString GetCollectDate(_RecordsetPtr m_pRecordset, char* key);

void TernminateAllThread(CSocketList& sockList, CThreadList& threadList);
//void TernminateAllThread(CClientList& sockList, CWinThreadList& threadList);

int GetFileTypeFromExt(CString type);
CString GetFileIcon(int type);
CString GetFileExtentName(int type);
CString GetExtNameFromFull(CString FullName);
int GetFileTypeFromFull(CString FullName);
UINT GetSmallIcon(int filetype);

CString GetDate(int offset = 0, BOOL ndHMS = TRUE);

void CreateSYN(CString FilePath);
void DeleteSYN(CString FilePath);
SYNFLAG ExistSYN(CString FilePath);
BOOL WriteToFile(CString Path ,char* buff, long len);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__ED834953_D4A7_4F79_8856_D1A8E07BEA60__INCLUDED_)
