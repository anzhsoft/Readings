// SocketList.cpp: implementation of the CSocketList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Readings.h"
#include "SocketList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketList::CSocketList()
{
	head = curr = NULL;
}

CSocketList::~CSocketList()
{

}

void CSocketList::AddSocket(SOCKET sock)
{
	if(!curr){
		curr = new SOCKETNODE;
		ZeroMemory(curr, sizeof(SOCKETNODE));
		curr->sock = s;
	}
}
