// PubInfo.h: interface for the CPubInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBINFO_H__2EC92573_61A6_449B_BA42_DE4C7DAC7F8E__INCLUDED_)
#define AFX_PUBINFO_H__2EC92573_61A6_449B_BA42_DE4C7DAC7F8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPubInfo  
{
public:
	CPubInfo();
	CPubInfo(LPCSTR root);
	virtual ~CPubInfo();
public:
	void StartPubInfo(BOOL& quit);
	char rootDir[_MAX_PATH+1];
	char buffer[BUFFER_SIZE];	//定义存储发送包的缓冲区
	int	 offset;
protected:
	void FindFileInDir(char* Path,BOOL& quit);
	int GetFileType(char* filename);
	void FormShareInfo(char *filepath, WIN32_FIND_DATA* pfd, SHAREINFO *oneInfo);
	void LoadBuffer(SHAREINFO* oneInfo);
public:
	static UINT PubDataBase(int filetype, SOCKET s, BOOL& quit);
};

#endif // !defined(AFX_PUBINFO_H__2EC92573_61A6_449B_BA42_DE4C7DAC7F8E__INCLUDED_)
