// ManBooks.h: interface for the CManBooks class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANBOOKS_H__F8665CEE_4171_4140_AA7E_9DFEFED16B4C__INCLUDED_)
#define AFX_MANBOOKS_H__F8665CEE_4171_4140_AA7E_9DFEFED16B4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuidTreeView.h"

class CManBooks : public CGuidTreeView  //∂¡ŒÔπ‹¿Ì
{
public:
	BOOL RefreshTree();
	CManBooks();
	virtual ~CManBooks();

};

#endif // !defined(AFX_MANBOOKS_H__F8665CEE_4171_4140_AA7E_9DFEFED16B4C__INCLUDED_)
