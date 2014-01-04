// HttpSocket.h: interface for the CHttpSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPSOCKET_H__F49A8F82_A933_41A8_AF47_68FBCAC4ADA6__INCLUDED_)
#define AFX_HTTPSOCKET_H__F49A8F82_A933_41A8_AF47_68FBCAC4ADA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class _declspec(dllexport) CHttpSocket  
{
public:
	CHttpSocket();
	virtual ~CHttpSocket();
	//返回服务器状态码 -1表示不成功
	int	GetServerState();
	//返回某个域值,-1表示不成功
	int	GetField(const char* szSession,char *szValue,int nMaxLength);
	//获取返回头的一行
	int	GetResponseLine(char *pLine,int nMaxLength);
	//获取完整的返回头
	const char*	GetResponseHeader(int &Length);
	//格式化请求头
	const char*	FormatRequestHeader(char *pServer, char *pObject,
		long &Length, unsigned short nPort = 80, char* pCookie=NULL, char *pReferer=NULL,
		long nFrom=0, long nTo=0, int nServerType=0);
	int	GetRequestHeader(char *pHeader,int nMaxLength) const;
	BOOL SendRequest(const char* pRequestHeader = NULL,long Length = 0);
	BOOL SetTimeout(int nTime,int nType=0);
	long Receive(char* pBuffer,long nMaxLength);
	BOOL Connect(char* szHostName,int nPort=80);
	BOOL Socket();
	BOOL CloseSocket();
	SOCKET GetSocket();
protected:	
	char m_requestheader[1024];		//请求头
	char m_ResponseHeader[1024];	//回应头
	int m_port;						//端口
	char m_ipaddr[256];				//IP地址
	BOOL m_bConnected;
	SOCKET m_s;
	hostent *m_phostent;
	int m_nCurIndex;				//GetResponsLine()函数的游标记录
	BOOL m_bResponsed;				//是否已经取得了返回头
	int m_nResponseHeaderSize;		//回应头的大小
};

#endif // !defined(AFX_HTTPSOCKET_H__F49A8F82_A933_41A8_AF47_68FBCAC4ADA6__INCLUDED_)
