// EMailADO.h: interface for the CEMailADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMAILADO_H__3D7213F6_65B7_41D1_8B4F_4242B375BEA8__INCLUDED_)
#define AFX_EMAILADO_H__3D7213F6_65B7_41D1_8B4F_4242B375BEA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEMailADO  
{
public:
	static string GetSavePath(string path);
	static string FindFilePath(string FileName);
	static bool GetAllReadedMail(CString Server, CString UserName);
	static bool DeleteMail(CString Server, CString MailID);
	static bool InsertMail(CString Server, CString MailID,CString UserName,CString MailType,CString MailFrom);
	CEMailADO();
	virtual ~CEMailADO();

};

#endif // !defined(AFX_EMAILADO_H__3D7213F6_65B7_41D1_8B4F_4242B375BEA8__INCLUDED_)
