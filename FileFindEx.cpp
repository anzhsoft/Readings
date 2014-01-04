// FileFindEx.cpp: implementation of the CFileFindEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Readings.h"
#include "FileFindEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileFindEx::CFileFindEx(PFOperation pfolder, AFOperation afolder, FOperation file)
{
	PreFolderOperation = pfolder;
	AftFolderOperation = afolder;
	FileOperation = file;
}

CFileFindEx::~CFileFindEx()
{

}

void CFileFindEx::Find(LPCSTR Path, LPVOID pParam, DWORD& data)
{
	Search(Path, pParam, data);
}

void CFileFindEx::Search(LPCSTR Path, LPVOID pfParam, DWORD& data)
{
	WIN32_FIND_DATA finddata = {0};
	char findPath[_MAX_PATH] = {0};
	sprintf_s(findPath, _MAX_PATH, "%s\\*",Path);
	HANDLE handle = ::FindFirstFile(findPath, &finddata);
	if(handle == INVALID_HANDLE_VALUE)
		return;
	else if(finddata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY){
		if(strcmp(finddata.cFileName, ".") && strcmp(finddata.cFileName, "..")){
			DWORD subdata = 0;
			LPVOID pParam = NULL;
			if(NULL != (pParam = PreFolderOperation(Path, &finddata, pfParam, subdata))){
				char CurrPath[_MAX_PATH] = {0};
				sprintf_s(CurrPath, _MAX_PATH, "%s\\%s", Path, finddata.cFileName);
				Search(CurrPath, pParam, subdata);
				AftFolderOperation(Path, &finddata, pParam, subdata, data);
			}
		}
	}
	else{
		FileOperation(Path, &finddata, data);
	}
	BOOL bSearch = TRUE;
	while(bSearch){
		if(::FindNextFile(handle, &finddata)){
			if(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if(strcmp(finddata.cFileName, ".") && strcmp(finddata.cFileName, "..")){
					DWORD subdata = 0;
					LPVOID pParam = NULL;
					if(NULL != (pParam = PreFolderOperation(Path, &finddata, pfParam, subdata))){
						char CurrPath[_MAX_PATH] = {0};
						sprintf_s(CurrPath, _MAX_PATH, "%s\\%s", Path, finddata.cFileName);
						Search(CurrPath, pParam, subdata);
						AftFolderOperation(Path, &finddata, pParam, subdata, data);
					}
				}
			}
			else{
				FileOperation(Path, &finddata, data);
			}
		}
		else
			bSearch = FALSE;
	}
	::FindClose(handle);
}
