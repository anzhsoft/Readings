// UpLoadTorrent.cpp: implementation of the CUpLoadTorrent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "UpLoadTorrent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXHEADERSIZE 1024
extern char UserName[50];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUpLoadTorrent::CUpLoadTorrent()
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

CUpLoadTorrent::~CUpLoadTorrent()
{
	CloseSocket();
}

BOOL CUpLoadTorrent::Socket()
{
	if(m_bConnected) return FALSE;
	m_s=socket(PF_INET, SOCK_STREAM, 0);
	if(m_s==INVALID_SOCKET)	return FALSE;
	return TRUE;
}

BOOL CUpLoadTorrent::Connect(char *szHostName,int nPort)
{
	if(szHostName==NULL)
		return FALSE;

	///若已经连接,则先关闭
	if(m_bConnected)
	{
		CloseSocket();
	}

	///保存端口号
	m_port=nPort;

	///根据主机名获得IP地址
	m_phostent=gethostbyname(szHostName);
	if(m_phostent==NULL){
		return FALSE;
	}
	
	///连接
	struct in_addr ip_addr;
	memcpy(&ip_addr,m_phostent->h_addr_list[0],4);///h_addr_list[0]里4个字节,每个字节8位
	
	struct sockaddr_in destaddr;
	memset((void *)&destaddr,0,sizeof(destaddr)); 
	destaddr.sin_family=AF_INET;
	destaddr.sin_port=htons(nPort);
	destaddr.sin_addr=ip_addr;

	///保存主机的IP地址字符串
	sprintf(m_ipaddr,"%d.%d.%d.%d",
		destaddr.sin_addr.S_un.S_un_b.s_b1,
		destaddr.sin_addr.S_un.S_un_b.s_b2,
		destaddr.sin_addr.S_un.S_un_b.s_b3,
		destaddr.sin_addr.S_un.S_un_b.s_b4);
	/*inet_addr();把带点的IP地址字符串转化为in_addr格式;
	  inet_ntoa();作用相反*/
	
	/*注意理解sturct in_addr 的结构:一个32位的数;一共同体的形式使用
	(1)每8位一个即s_b1~s_b4;
	(2)每16位一个即s_w1~s_w2;
	(3)32位s_addr
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

	///设置已经连接的标志
	m_bConnected=TRUE;
	return TRUE;
}

///根据请求的相对URL输出HTTP请求头
const char *CUpLoadTorrent::FormatRequestHeader(CString torrentPath)
{
	CFile torrent;
	if (!torrent.Open(torrentPath,CFile::modeRead))
	{
		return NULL;
	}
	CString filename = torrent.GetFileName();
	char * buff = new char[torrent.GetLength()]+1;
#if 0
	// anzhsoft for platform
	torrent.ReadHuge(buff, torrent.GetLength());
#endif
	CString content = buff;
	CString head;
	head = "POST /ClientUpload.html HTTP/1.1\r\nAccept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-ms-application, application/x-ms-xbap, application/vnd.ms-xpsdocument, application/xaml+xml, */*\r\nReferer: http://61.181.14.184:6969/ClientUpload.html\r\nAccept-Language: zh-cn\r\nContent-Type: multipart/form-data; boundary=---------------------------7da5d8a0964\r\nAccept-Encoding: gzip, deflate\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; CIBA)\r\nHost: 61.181.14.184:6969\r\nContent-Length: 529\r\nConnection: Keep-Alive\r\nCache-Control: no-cache\r\nCookie: login=\""
		+CString("safelab")
		+"\"; md5=\"%5D2%C9%93%05L%EC%C2Q%B1%96%ABV%DA%BF%83\"\r\n\r\n-----------------------------7da5d8a0964\r\nContent-Disposition: form-data; name=\"torrent\"; filename=\""+torrentPath+"\"\r\nContent-Type: application/x-bittorrent\r\n\r\n"+content+"\r\n-----------------------------7da5d8a0964\r\nContent-Disposition: form-data; name=\""+filename+"\"\r\n\r\ncom\r\n-----------------------------7da5d8a0964--";
	strcpy(m_requestheader,head.GetBuffer(0));
	AfxMessageBox(m_requestheader);
	return m_requestheader;
}

///发送请求头
BOOL CUpLoadTorrent::SendRequest(const char *pRequestHeader, long Length)
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

long CUpLoadTorrent::Receive(char* pBuffer,long nMaxLength)
{
	if(!m_bConnected)return NULL;

	///接收数据
	long nLength;
	nLength=recv(m_s,pBuffer,nMaxLength,0);
/*	
	if(nLength <= 0)
		CloseSocket();
*/
	return nLength;
}

///关闭套接字
BOOL CUpLoadTorrent::CloseSocket()
{
	if(m_s != NULL){
		if(closesocket(m_s)==SOCKET_ERROR)
			return FALSE;
	}
	m_s = NULL;
	m_bConnected=FALSE;
	return TRUE;
}

int CUpLoadTorrent::GetRequestHeader(char *pHeader, int nMaxLength) const
{
	int nLength;
	if(int(strlen(m_requestheader))>nMaxLength)
		nLength=nMaxLength;
	else
		nLength=strlen(m_requestheader);
	memcpy(pHeader,m_requestheader,nLength);
	return nLength;
}

//设置接收或者发送的最长时间
BOOL CUpLoadTorrent::SetTimeout(int nTime, int nType)
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

//获取HTTP请求的返回头
const char* CUpLoadTorrent::GetResponseHeader(int &nLength)
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

//返回HTTP响应头中的一行.
int CUpLoadTorrent::GetResponseLine(char *pLine, int nMaxLength)
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

int CUpLoadTorrent::GetField(const char *szSession, char *szValue, int nMaxLength)
{
	//取得某个域值
	if(!m_bResponsed) return -1;
	
	CString strRespons;
	strRespons = m_ResponseHeader;
	int nPos = -1;
	nPos = strRespons.Find(szSession,0);
	if(nPos != -1)
	{
		nPos += strlen(szSession);
		nPos += 2;
		int nCr = strRespons.Find("\r\n",nPos);
		CString strValue = strRespons.Mid(nPos,nCr - nPos);
		strcpy(szValue,strValue);
		return (nCr - nPos);
	}
	else
	{
		return -1;
	}
}

int CUpLoadTorrent::GetServerState()
{
	//若没有取得响应头,返回失败
	if(!m_bResponsed) return -1;
	
	char szState[3];
	szState[0] = m_ResponseHeader[9];
	szState[1] = m_ResponseHeader[10];
	szState[2] = m_ResponseHeader[11];

	return atoi(szState);
}

SOCKET CUpLoadTorrent::GetSocket()
{
	return m_s;
}
