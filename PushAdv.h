// PushAdv.h: interface for the CPushAdv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUSHADV_H__2D55898B_F244_4A07_9A7B_02DDF8E6F325__INCLUDED_)
#define AFX_PUSHADV_H__2D55898B_F244_4A07_9A7B_02DDF8E6F325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPushAdv  
{
public:
	CPushAdv();
	virtual ~CPushAdv();

	static BOOL PushReadman();

	static UINT PushAdv(LPVOID pParam);
	static UINT SynReadmanAdv(CString FileName, CString DstPath, CString URL,	int FileType, CString Type);
	static UINT SearchAndCopy(CString localPath, CString DestPath);
	static UINT ToSDCardAdv();
	static void DeleteAllFiles(CString Path);
};

#endif // !defined(AFX_PUSHADV_H__2D55898B_F244_4A07_9A7B_02DDF8E6F325__INCLUDED_)
