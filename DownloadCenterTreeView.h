// DownloadCenterTreeView.h: interface for the CDownloadCenterTreeView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOWNLOADCENTERTREEVIEW_H__61DE209D_AE98_4997_93E0_057F42ECDA49__INCLUDED_)
#define AFX_DOWNLOADCENTERTREEVIEW_H__61DE209D_AE98_4997_93E0_057F42ECDA49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuidTreeView.h"

class CDownloadCenterTreeView : public CGuidTreeView  
{
public:
	BOOL RefreshTree();
	CDownloadCenterTreeView();
	virtual ~CDownloadCenterTreeView();

};

#endif // !defined(AFX_DOWNLOADCENTERTREEVIEW_H__61DE209D_AE98_4997_93E0_057F42ECDA49__INCLUDED_)
