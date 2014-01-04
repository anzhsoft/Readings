// CommLog.h: interface for the CCommLog class.
//
//////////////////////////////////////////////////////////////////////

/************************************************************************/
/* created by yangxiao  2008.08.08                                      */
/************************************************************************/

#if !defined(AFX_COMMLOG_H__BE367395_E8F2_4045_B486_1AD66CA1919F__INCLUDED_)
#define AFX_COMMLOG_H__BE367395_E8F2_4045_B486_1AD66CA1919F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum upDown{up , down};


typedef struct  
{
	
	char operation [100];
	char operNum;
	long fileSize;
	char upDownFlag;
	short year;
	char month;
	char day;
	char weekDay;
	char hour;
	char minute;
	char second;

} COMMLOGITEM;

typedef struct  
{
	char fileName [256];
	char localPath [256];
	char targetURL [256];
	char dstIp [16];
	char sourceIp [16];
	
} COMMFILELOGITEM;
 

class CCommLog  
{
public:
	CCommLog();
	virtual ~CCommLog();

public:
	COMMLOGITEM* m_logItem;
	void writeLogItem(COMMLOGITEM* logItem, CFile* file);
	void writeLogItem();
	void setLogItem(char* oper, long szPacket, char upDown, char operNum);
//	CFile* currLogin;
};

#endif // !defined(AFX_COMMLOG_H__BE367395_E8F2_4045_B486_1AD66CA1919F__INCLUDED_)
