// SocketList.h: interface for the CSocketList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETLIST_H__7C00DEAB_5992_4E77_8C06_BDBFBBDDA745__INCLUDED_)
#define AFX_SOCKETLIST_H__7C00DEAB_5992_4E77_8C06_BDBFBBDDA745__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocketList  
{
protected:
	typedef struct SOCKETNODE{
		SOCKET sock;
		SOCKETNODE* next;
	};
	SOCKETNODE* head;
	SOCKETNODE* curr;
	int count;

public:
	CSocketList();
	virtual ~CSocketList();
public:
	void AddSocket(SOCKET sock);
	

};

#endif // !defined(AFX_SOCKETLIST_H__7C00DEAB_5992_4E77_8C06_BDBFBBDDA745__INCLUDED_)
