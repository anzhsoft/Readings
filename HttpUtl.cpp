// HttpUtl.cpp: implementation of the HttpUtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "HttpUtl.h"
#include <afxinet.h>



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMainFrame* pMainWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HttpUtl::HttpUtl()
{

}

HttpUtl::~HttpUtl()
{

}

int HttpUtl::request(char *url,char * &content/*,int dataSize=4096*/){
	TRY{
		try{
	//	content=new char[4096];
		int dataSize=4096;
		char *tmp=new char[dataSize];
		tmp[0]=0;

		CInternetSession sess(_T("Update   1.0"),1,INTERNET_OPEN_TYPE_DIRECT);

		
		sess.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,10000);
		DWORD dwFlag=INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD;

		CHttpFile *pFile;
		pFile=(CHttpFile*)sess.OpenURL((LPCTSTR)url,1,dwFlag);
		
	
		int count=0;
		int size=0;
		while(TRUE){
			if (pMainWnd->m_quit)
			{
				return 0;
			}
            char *buf = new char[128];
			UINT n=pFile->Read(buf,128);
			if(n==0) break;
		//	strncpy(tmp,buf,n);
			memcpy(tmp+size,buf,n);
		//	printf(data);
			delete [] buf;
			size+=n;

			if(size>=dataSize/2){
				dataSize*=2;
				char *tmp1=new  char[dataSize];
				memcpy(tmp1,tmp,size);
				delete []tmp;
				tmp=tmp1;
			}
		}
		tmp[size]=0;
		content=tmp;
	//	CString out;
	//	CTranscoding::UTF_8ToGB2312(out,tmp,size);
	//	strcpy(content,out.GetBuffer(out.GetLength()));
	//	strcpy(content,tmp);
	
	    pFile->Close();
	    sess.Close();
	    delete pFile;
	//	content[size]=0;
		return size;
		}
		catch(CInternetException e){
			e.Delete();
		}
	}
	CATCH_ALL(e){
		e->Delete();
		return 0;
	}
	END_CATCH_ALL
		return 1;
}

int HttpUtl::post(char *sserver,char *requestPath,char *storrentPath,char *sbuffer,int bufferLen,char *sfilename,int port){
	TRY{
		try{
			/*
			CFile f("F:\\ComputerID.vi.torrent",CFile::modeRead);
			int flen = f.GetLength();
			char *data = new char[flen+1];
			memset(data,0,sizeof(char)*flen+1);
			f.Read(data,flen);
			AfxMessageBox(data);
			f.Close();
			*/
			CInternetSession sess(_T("Update   1.0"),1,INTERNET_OPEN_TYPE_DIRECT);
		
			CString strHtml;
			CString torrentPath=storrentPath;
			//CString content=sbuffer;
			CString filename=sfilename;
			CString boundary="-----------------------------7da5d8a0964";
	        
			char *requestData=new char[bufferLen+1024];
            int dataLen=0;

			CString strRequestTop =boundary
				+"\r\nContent-Disposition: form-data; name=\"torrent\"; filename=\""
				+torrentPath
				+"\"\r\nContent-Type: application/x-bittorrent\r\n\r\n";

           memcpy(requestData,strRequestTop.GetBuffer(0),strRequestTop.GetLength());
		   dataLen+=strRequestTop.GetLength();
		   memcpy(requestData+dataLen,sbuffer,bufferLen);
		   dataLen+=bufferLen;
			strRequestTop=CString("\r\n")
				+boundary
				+"\r\nContent-Disposition: form-data; name=\""+filename
				+"\"\r\n\r\ncom\r\n"
				+boundary
				+"--";
			memcpy(requestData+dataLen,strRequestTop.GetBuffer(0),strRequestTop.GetLength());
			dataLen+=strRequestTop.GetLength();

			CString len;
			len.Format("%d",dataLen);
			CString strHeaders ="Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-ms-application, application/x-ms-xbap, application/vnd.ms-xpsdocument, application/xaml+xml, */*\r\nReferer: http://61.181.14.184:6969/ClientUpload.html\r\nAccept-Language: zh-cn\r\nContent-Type: multipart/form-data; boundary="
				+boundary
				+"\r\nAccept-Encoding: gzip, deflate\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; CIBA)\r\nHost: 61.181.14.184:6969\r\nContent-Length: "
				+len
				+"\r\nConnection: Keep-Alive\r\nCache-Control: no-cache\r\n";//Cookie: login=\""
			//	+CString("safelab")
			//	+"\"; md5=\"%5D2%C9%93%05L%EC%C2Q%B1%96%ABV%DA%BF%83\"\r\n";

			sess.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT,10000);
			DWORD dwFlag=INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD;
			
			CHttpFile *pFile;
			//	pFile=(CHttpFile*)sess.OpeURL((LPCTSTR)url,1,dwFlag);
			
			CHttpConnection* pServer = sess.GetHttpConnection(sserver,0, port,NULL,NULL);
			
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST,requestPath);
			
			pFile->AddRequestHeaders(strHeaders);
			
			pFile->SendRequestEx(dataLen);
			
			pFile->Write(requestData,dataLen);
			
		//	pFile->WriteString(strRequest); //重要-->m_Request 中有"name=aaa&name2=BBB&..."
			
			pFile->EndRequest();
			
			DWORD dwRet;
			
			pFile->QueryInfoStatusCode(dwRet);
			
		
			int nRead;
			if (dwRet == HTTP_STATUS_OK){
				
				CString strLine;
				
				while ((nRead = pFile->ReadString(strLine))>0)
					
				{
		    		strHtml += strLine;
					
				}
	
				
			}     
	
			pFile->Close();
			sess.Close();
			delete pFile;
			
		}
		catch(CInternetException e){
			e.Delete();
		}
	}
	CATCH_ALL(e){
		e->Delete();
		return 0;
	}
	END_CATCH_ALL
		return 1;
	
}
