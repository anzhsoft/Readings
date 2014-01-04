// HotKeyTree.h: interface for the CHotKeyTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOTKEYTREE_H__AEA6F0B0_B6C0_4249_A0B5_91A8CC158592__INCLUDED_)
#define AFX_HOTKEYTREE_H__AEA6F0B0_B6C0_4249_A0B5_91A8CC158592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuidTreeView.h"

class CHotKeyTree : public CGuidTreeView  
{
public:
	CHotKeyTree();
	virtual ~CHotKeyTree();
	BOOL RefreshTree();
};

#endif // !defined(AFX_HOTKEYTREE_H__AEA6F0B0_B6C0_4249_A0B5_91A8CC158592__INCLUDED_)
