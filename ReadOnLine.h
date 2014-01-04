// ReadOnLine.h: interface for the CReadOnLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_READONLINE_H__2797DC77_A6EE_4522_ADEF_9801ED1D14A4__INCLUDED_)
#define AFX_READONLINE_H__2797DC77_A6EE_4522_ADEF_9801ED1D14A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuidTreeView.h"

class CReadOnLine : public CGuidTreeView  //ÔÚÏßÔÄ¶Á
{
public:
	BOOL RefreshTree();
	CReadOnLine();
	virtual ~CReadOnLine();
};

#endif // !defined(AFX_READONLINE_H__2797DC77_A6EE_4522_ADEF_9801ED1D14A4__INCLUDED_)
