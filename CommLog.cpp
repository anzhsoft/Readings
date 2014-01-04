// CommLog.cpp: implementation of the CCommLog class.
//
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* created by yangxiao  2008.08.08                                      */
/************************************************************************/

#include "stdafx.h"
#include "Readings.h"
#include "CommLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern char CurrentPath[_MAX_PATH];
extern CFile* currLogin;
extern _ConnectionPtr m_pConnection;

CCommLog::CCommLog()
{
	m_logItem = (COMMLOGITEM*)malloc(sizeof(COMMLOGITEM));
	ZeroMemory(m_logItem,sizeof(COMMLOGITEM));

	//文件创建移至BOOL CReadingsApp::InitInstance()	
	/* 
	CString logFile;
	logFile.Format("%s\\system\\logFile.inf",CurrentPath);
	currLogin = new CFile();
	if(!currLogin->Open(logFile,CFile::modeCreate|CFile::typeBinary|CFile::modeReadWrite))
		Tips("打开日志文件失败");
	*/
}

CCommLog::~CCommLog()
{

}

void CCommLog::setLogItem(char* oper, long szPacket, char upDown, char operNum)
{
//	char* tmpStr={0};
	CTime tm = CTime::GetCurrentTime();
	
	m_logItem->year = tm.GetYear();
	m_logItem->month = tm.GetMonth();
	m_logItem->day = tm.GetDay();
	m_logItem->weekDay = tm.GetDayOfWeek(); 
	m_logItem->hour = tm.GetHour();
	m_logItem->minute = tm.GetMinute();
	m_logItem->second = tm.GetSecond();
	strcpy_s(m_logItem->operation, 100, oper);
	m_logItem->fileSize = szPacket;
	m_logItem->upDownFlag = upDown;
	m_logItem->operNum = operNum;
		
}

void CCommLog::writeLogItem(COMMLOGITEM* logItem, CFile* file)
{
	//目前没用到相关参数，留做以后扩展用。
	
// 	currLogin->SeekToEnd();
// 	currLogin->Write(m_logItem,sizeof(COMMLOGITEM));
}

void CCommLog::writeLogItem()
{	
	//	currLogin->SeekToEnd();
	//	currLogin->Write(m_logItem,sizeof(COMMLOGITEM));
	
	_RecordsetPtr m_pRecordset;
	
	char SQL[1024] = {0};
	sprintf_s(SQL, 1024, "INSERT INTO currLog ([operation],[fileSize],[year],[month],[day],[hour],[minute],[second],[weekDay],[upDownFlag],[operNum] ) values(\"%s\",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",m_logItem->operation,m_logItem->fileSize,m_logItem->year,m_logItem->month,m_logItem->day,m_logItem->hour,m_logItem->minute,m_logItem->second,m_logItem->weekDay,m_logItem->upDownFlag,m_logItem->operNum);
	m_pConnection->Execute(SQL, NULL, adCmdText);
	
}
