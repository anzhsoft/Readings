// GetUserInfo.h: interface for the CGetUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETUSERINFO_H__378BF7EB_2322_45AA_B8FE_344427A49107__INCLUDED_)
#define AFX_GETUSERINFO_H__378BF7EB_2322_45AA_B8FE_344427A49107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGetUserInfo  
{
public:
	void GetNotes(CString path, CString loc, BOOL& quit);
	void UpBugLib(BOOL& quit, CSocketList& socketList, CMutex& mutex);
	void GetRestore(BOOL& quit);
	void GetComment(BOOL& quit, CSocketList& socketList, CMutex& mutex);
	void GetWriting(BOOL& quit);
	CGetUserInfo();
	virtual ~CGetUserInfo();

};

#endif // !defined(AFX_GETUSERINFO_H__378BF7EB_2322_45AA_B8FE_344427A49107__INCLUDED_)
