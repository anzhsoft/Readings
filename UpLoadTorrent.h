// UpLoadTorrent.h: interface for the CUpLoadTorrent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPLOADTORRENT_H__E7EFA136_1DE7_43E4_917F_4D530481ED66__INCLUDED_)
#define AFX_UPLOADTORRENT_H__E7EFA136_1DE7_43E4_917F_4D530481ED66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUpLoadTorrent  
{
public:
	CUpLoadTorrent();
	virtual ~CUpLoadTorrent();
	//���ط�����״̬�� -1��ʾ���ɹ�
	int	GetServerState();
	//����ĳ����ֵ,-1��ʾ���ɹ�
	int	GetField(const char* szSession,char *szValue,int nMaxLength);
	//��ȡ����ͷ��һ��
	int	GetResponseLine(char *pLine,int nMaxLength);
	//��ȡ�����ķ���ͷ
	const char*	GetResponseHeader(int &Length);
	//��ʽ������ͷ
	const char*	FormatRequestHeader(CString torrentPath);
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

#endif // !defined(AFX_UPLOADTORRENT_H__E7EFA136_1DE7_43E4_917F_4D530481ED66__INCLUDED_)
