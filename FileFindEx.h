// FileFindEx.h: interface for the CFileFindEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEFINDEX_H__DB3A7922_886C_43BD_ABDA_B5230ED9C29D__INCLUDED_)
#define AFX_FILEFINDEX_H__DB3A7922_886C_43BD_ABDA_B5230ED9C29D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef LPVOID(*PFOperation)(LPCSTR Path, WIN32_FIND_DATA* pData,
							 LPVOID pfParam, DWORD& subdata);
typedef void(*AFOperation)(LPCSTR Path, WIN32_FIND_DATA* pData,
						   LPVOID pParam, DWORD subdata, DWORD& data);
typedef void(*FOperation)(LPCSTR Path, WIN32_FIND_DATA* pData, DWORD& data);

class CFileFindEx  
{
public:
	PFOperation PreFolderOperation;
	AFOperation AftFolderOperation;
	FOperation FileOperation;
	void Find(LPCSTR Path, LPVOID pParam, DWORD& data);
	void Search(LPCSTR Path, LPVOID pfParam, DWORD& data);

	CFileFindEx(PFOperation pfolder, AFOperation afolder, FOperation file);
	virtual ~CFileFindEx();
};

#endif // !defined(AFX_FILEFINDEX_H__DB3A7922_886C_43BD_ABDA_B5230ED9C29D__INCLUDED_)
