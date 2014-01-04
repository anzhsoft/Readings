// HttpSocket.cpp: implementation of the CHttpSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HttpSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXHEADERSIZE 1024
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHttpSocket::CHttpSocket()
{
	m_s=NULL;
	m_phostent=NULL;
	m_port=80;

	m_bConnected=FALSE;

	for(int i=0;i<256;i++)
		m_ipaddr[i]='\0';
	memset(m_requestheader,0,MAXHEADERSIZE);
	memset(m_ResponseHeader,0,MAXHEADERSIZE);

	m_nCurIndex = 0;		//
	m_bResponsed = FALSE;
	m_nResponseHeaderSize = -1;
/*
	m_nBufferSize = nBufferSize;
	m_pBuffer = new char[nBufferSize];
	memset(m_pBuffer,0,nBufferSize);*/
}

CHttpSocket::~CHttpSocket()
{
	CloseSocket();
}

BOOL CHttpSocket::Socket()
{
	if(m_bConnected)
		return FALSE;
	m_s=socket(PF_INET, SOCK_STREAM, 0);
	if(m_s==INVALID_SOCKET)	return FALSE;
	return TRUE;
}

BOOL CHttpSocket::Connect(char *szHostName,int nPort)
{
	if(szHostName==NULL)
		return FALSE;

	///���Ѿ�����,���ȹر�
	if(m_bConnected)
	{
		CloseSocket();
	}

	///����˿ں�
	m_port=nPort;

	///�������������IP��ַ
	m_phostent=gethostbyname(szHostName);
	if(m_phostent==NULL){
		return FALSE;
	}
	
	///����
	struct in_addr ip_addr;
	memcpy(&ip_addr,m_phostent->h_addr_list[0],4);///h_addr_list[0]��4���ֽ�,ÿ���ֽ�8λ
	
	struct sockaddr_in destaddr;
	memset((void *)&destaddr,0,sizeof(destaddr)); 
	destaddr.sin_family=AF_INET;
	destaddr.sin_port=htons(nPort);
	destaddr.sin_addr=ip_addr;

	///����������IP��ַ�ַ���
	sprintf_s(m_ipaddr, 256, "%d.%d.%d.%d",
		destaddr.sin_addr.S_un.S_un_b.s_b1,
		destaddr.sin_addr.S_un.S_un_b.s_b2,
		destaddr.sin_addr.S_un.S_un_b.s_b3,
		destaddr.sin_addr.S_un.S_un_b.s_b4);
	/*inet_addr();�Ѵ����IP��ַ�ַ���ת��Ϊin_addr��ʽ;
	  inet_ntoa();�����෴*/
	
	/*ע�����sturct in_addr �Ľṹ:һ��32λ����;һ��ͬ�����ʽʹ��
	(1)ÿ8λһ����s_b1~s_b4;
	(2)ÿ16λһ����s_w1~s_w2;
	(3)32λs_addr
	struct   in_addr {
    union   {
			  struct{
				unsigned  char   s_b1,
								 s_b2,
								 s_b3,
								 s_b4;
					} S_un_b;
              struct{
				unsigned  short  s_w1,
                                 s_w2
			        }S_un_w;      
               unsigned long  S_addr;
			} S_un;
		};
	*/

	if(connect(m_s,(struct sockaddr*)&destaddr,sizeof(destaddr))!=0)
		return FALSE;

	///�����Ѿ����ӵı�־
	m_bConnected=TRUE;
	return TRUE;
}

///������������URL���HTTP����ͷ
const char *CHttpSocket::FormatRequestHeader(char *pServer,char *pObject, long &Length,
											 unsigned short nPort, char *pCookie,char *pReferer,
											 long nFrom, long nTo, int nServerType)
{
	char szPort[10];
	char szTemp[20];
	sprintf_s(szPort, 10,"%d",m_port);
	memset(m_requestheader,'\0',1024);

	///��1��:����,�����·��,�汾
	strcat_s(m_requestheader, 1024, "GET ");
	strcat_s(m_requestheader, 1024, pObject);
	strcat_s(m_requestheader, 1024, " HTTP/1.1");
    strcat_s(m_requestheader, 1024, "\r\n");

	///��2��:����
	CString host;
	host.Format("%s:%d", pServer, nPort);

    strcat_s(m_requestheader, 1024, "Host:");
	strcat_s(m_requestheader, 1024, host);
    strcat_s(m_requestheader, 1024, "\r\n");

	///��3��:
	if(pReferer != NULL)
	{
		strcat_s(m_requestheader, 1024, "Referer:");
		strcat_s(m_requestheader, 1024, pReferer);
		strcat_s(m_requestheader, 1024, "\r\n");		
	}

	///��4��:���յ���������
    strcat_s(m_requestheader, 1024, "Accept:*/*");
    strcat_s(m_requestheader, 1024, "\r\n");

	///��5��:���������
    strcat_s(m_requestheader, 1024, "User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
    strcat_s(m_requestheader, 1024, "\r\n");

	///��6��:��������,����
	strcat_s(m_requestheader, 1024, "Connection:Keep-Alive");
	strcat_s(m_requestheader, 1024, "\r\n");

	///��7��:Cookie.
	if(pCookie != NULL)
	{
		strcat_s(m_requestheader, 1024, "Set Cookie:0");
		strcat_s(m_requestheader, 1024, pCookie);
		strcat_s(m_requestheader, 1024, "\r\n");
	}

	///��8��:�����������ʼ�ֽ�λ��(�ϵ������Ĺؼ�)
	if(nFrom > 0)
	{
		strcat_s(m_requestheader, 1024, "Range: bytes=");
		_ltoa_s(nFrom,szTemp, 20, 10);
		strcat_s(m_requestheader, 1024, szTemp);
		strcat_s(m_requestheader, 1024, "-");
		if(nTo > nFrom)
		{
			_ltoa_s(nTo,szTemp, 20, 10);
			strcat_s(m_requestheader, 1024, szTemp);
		}
		strcat_s(m_requestheader, 1024, "\r\n");
	}

	strcat_s(m_requestheader, 1024, "Accept-Language: zh-cn\r\n");
	
	///���һ��:����
	strcat_s(m_requestheader, 1024, "\r\n");

	///���ؽ��
	Length=strlen(m_requestheader);
	return m_requestheader;
}

///��������ͷ
BOOL CHttpSocket::SendRequest(const char *pRequestHeader, long Length)
{
	if(!m_bConnected)return FALSE;

	if(pRequestHeader==NULL)
		pRequestHeader=m_requestheader;
	if(Length==0)
		Length=strlen(m_requestheader);

	if(send(m_s,pRequestHeader,Length,0)==SOCKET_ERROR)
		return FALSE;
	int nLength;
	GetResponseHeader(nLength);
	return TRUE;
}

long CHttpSocket::Receive(char* pBuffer,long nMaxLength)
{
	if(!m_bConnected)return NULL;

	///��������
	long nLength;
	nLength=recv(m_s,pBuffer,nMaxLength,0);
/*	
	if(nLength <= 0)
		CloseSocket();
*/
	return nLength;
}

///�ر��׽���
BOOL CHttpSocket::CloseSocket()
{
	if(m_s != NULL){
		if(closesocket(m_s)==SOCKET_ERROR)
			return FALSE;
	}
	m_s = NULL;
	m_bConnected=FALSE;
	return TRUE;
}

int CHttpSocket::GetRequestHeader(char *pHeader, int nMaxLength) const
{
	int nLength;
	if(int(strlen(m_requestheader))>nMaxLength)
		nLength=nMaxLength;
	else
		nLength=strlen(m_requestheader);
	memcpy(pHeader,m_requestheader,nLength);
	return nLength;
}

//���ý��ջ��߷��͵��ʱ��
BOOL CHttpSocket::SetTimeout(int nTime, int nType)
{
	if(nType == 0)
		nType = SO_RCVTIMEO;
	else
		nType = SO_SNDTIMEO;

	DWORD dwErr;
    dwErr=setsockopt(m_s,SOL_SOCKET,nType,(char*)&nTime,sizeof(nTime)); 
	if(dwErr)
		return FALSE;

	return TRUE;
}

//��ȡHTTP����ķ���ͷ
const char* CHttpSocket::GetResponseHeader(int &nLength)
{
	if(!m_bResponsed)
	{
		char c = 0;
		int nIndex = 0;
		BOOL bEndResponse = FALSE;
		while(!bEndResponse && nIndex < MAXHEADERSIZE)
		{
			recv(m_s,&c,1,0);
			m_ResponseHeader[nIndex++] = c;
			if(nIndex >= 4)
			{
				if(m_ResponseHeader[nIndex - 4] == '\r' && m_ResponseHeader[nIndex - 3] == '\n'
					&& m_ResponseHeader[nIndex - 2] == '\r' && m_ResponseHeader[nIndex - 1] == '\n')
					bEndResponse = TRUE;
			}
		}
		m_nResponseHeaderSize = nIndex;
		m_bResponsed = TRUE;
	}
	
	nLength = m_nResponseHeaderSize;
	return m_ResponseHeader;
}

//����HTTP��Ӧͷ�е�һ��.
int CHttpSocket::GetResponseLine(char *pLine, int nMaxLength)
{
	if(m_nCurIndex >= m_nResponseHeaderSize)
	{
		m_nCurIndex = 0;
		return -1;
	}

	int nIndex = 0;
	char c = 0;
	do 
	{
		c = m_ResponseHeader[m_nCurIndex++];
		pLine[nIndex++] = c;
	} while(c != '\n' && m_nCurIndex < m_nResponseHeaderSize && nIndex < nMaxLength);
	
	return nIndex;
}

int CHttpSocket::GetField(const char *szSession, char *szValue, int nMaxLength)
{
	//ȡ��ĳ����ֵ
	if(!m_bResponsed) return -1;
	
	CString strRespons;
	strRespons = m_ResponseHeader;
	int nPos = -1;
	nPos = strRespons.Find(szSession,0);
	if(nPos != -1)
	{
		nPos += strlen(szSession);
		nPos += 2;
		int nCr = strRespons.Find("\n",nPos);
		CString strValue = strRespons.Mid(nPos,nCr - nPos);
		strcpy_s(szValue, nMaxLength, strValue);
		return (nCr - nPos);
	}
	else
	{
		return -1;
	}
}

int CHttpSocket::GetServerState()
{
	//��û��ȡ����Ӧͷ,����ʧ��
	if(!m_bResponsed) return -1;
	
	char szState[3];
	szState[0] = m_ResponseHeader[9];
	szState[1] = m_ResponseHeader[10];
	szState[2] = m_ResponseHeader[11];

	return atoi(szState);
}

SOCKET CHttpSocket::GetSocket()
{
	return m_s;
}
