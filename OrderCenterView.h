// OrderCenterView.h: interface for the COrderCenterView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERCENTERVIEW_H__304B074E_E986_4811_8E6F_758989D651A2__INCLUDED_)
#define AFX_ORDERCENTERVIEW_H__304B074E_E986_4811_8E6F_758989D651A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuidTreeView.h"

class COrderCenterView : public CGuidTreeView  //¶©ÔÄÖÐÐÄ
{
public:
	BOOL RefreshTree();
	COrderCenterView();
	virtual ~COrderCenterView();

};

#endif // !defined(AFX_ORDERCENTERVIEW_H__304B074E_E986_4811_8E6F_758989D651A2__INCLUDED_)
