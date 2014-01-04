// Push.h: interface for the CPush class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUSH_H__423129CF_B99E_4373_85FE_5CF1322AD3C1__INCLUDED_)
#define AFX_PUSH_H__423129CF_B99E_4373_85FE_5CF1322AD3C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPush  
{
public:
	CPush();
	virtual ~CPush();

public:
	static BOOL PushInfoToDB();
	static BOOL DownLoadPushList();
};

#endif // !defined(AFX_PUSH_H__423129CF_B99E_4373_85FE_5CF1322AD3C1__INCLUDED_)
