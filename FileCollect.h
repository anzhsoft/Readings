// FileCollect.h: interface for the CFileCollect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILECOLLECT_H__AFED542A_CF8C_4750_A951_BEA88BC13F55__INCLUDED_)
#define AFX_FILECOLLECT_H__AFED542A_CF8C_4750_A951_BEA88BC13F55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileCollect  
{
public:
	CFileCollect();
	virtual ~CFileCollect();
	void Do(BOOL& quit);
	BOOL Initial();
	void Release();
private:
	CString m_currDisk;
	CStringList m_diskList;
	__int64 finishedSize;
	unsigned _int64 GetDiskFree(char *disk);
	void Search(const char *Path, __int64& finishedSize, BOOL& quit);
	BOOL FileOperation(WIN32_FIND_DATA data, const char *Path, __int64& finishedSize, BOOL& quit);
};

#endif // !defined(AFX_FILECOLLECT_H__AFED542A_CF8C_4750_A951_BEA88BC13F55__INCLUDED_)
