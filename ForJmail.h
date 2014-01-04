// ForJmail.h: interface for the CForJmail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORJMAIL_H__7810791F_1E50_4C02_A769_D5A3D41A544C__INCLUDED_)
#define AFX_FORJMAIL_H__7810791F_1E50_4C02_A769_D5A3D41A544C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CForJmail  
{
public:
	int GetIndex(CString subject);
	CString DownAtt(int index, CString Path, CString title);
	CString add;
	BOOL DisConnectToPOP3();
	jmail::IMessagesPtr pMessages;
	jmail::IPOP3Ptr pPOP3;
	CString password;
	CString username;
	CString POP3Server;
	CString GetPOP3Server(CString addr);
	BOOL ConnectToPOP3();
	BOOL GetMailHead();
	CForJmail(CString addr, CString pass);
	virtual ~CForJmail();

};

#endif // !defined(AFX_FORJMAIL_H__7810791F_1E50_4C02_A769_D5A3D41A544C__INCLUDED_)
