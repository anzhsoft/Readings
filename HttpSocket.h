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
	//���ط�����״̬�� -1��ʾ���ɹ�
	int	GetServerState();
	//����ĳ����ֵ,-1��ʾ���ɹ�
	int	GetField(const char* szSession,char *szValue,int nMaxLength);
	//��ȡ����ͷ��һ��
	int	GetResponseLine(char *pLine,int nMaxLength);
	//��ȡ�����ķ���ͷ
	const char*	GetResponseHeader(int &Length);
	//��ʽ������ͷ
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
	char m_requestheader[1024];		//����ͷ
	char m_ResponseHeader[1024];	//��Ӧͷ
	int m_port;						//�˿�
	char m_ipaddr[256];				//IP��ַ
	BOOL m_bConnected;
	SOCKET m_s;
	hostent *m_phostent;
	int m_nCurIndex;				//GetResponsLine()�������α��¼
	BOOL m_bResponsed;				//�Ƿ��Ѿ�ȡ���˷���ͷ
	int m_nResponseHeaderSize;		//��Ӧͷ�Ĵ�С
};

#endif // !defined(AFX_HTTPSOCKET_H__F49A8F82_A933_41A8_AF47_68FBCAC4ADA6__INCLUDED_)
