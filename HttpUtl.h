// HttpUtl.h: interface for the HttpUtl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPUTL_H__30EACFA0_5379_4894_BEF4_A1B319C9B1EF__INCLUDED_)
#define AFX_HTTPUTL_H__30EACFA0_5379_4894_BEF4_A1B319C9B1EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class HttpUtl  
{
public:
	int request(char *url,char *&buffer/*,int dataSize*/);
	int post(char *sserver,char *requestPage,char *torrentPath,char *sbuffer,int bufferLen,char *sfilename,int port);
	HttpUtl();
	virtual ~HttpUtl();

};

#endif // !defined(AFX_HTTPUTL_H__30EACFA0_5379_4894_BEF4_A1B319C9B1EF__INCLUDED_)
