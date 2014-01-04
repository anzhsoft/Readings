#include "stdafx.h"
#include "base64.h"
#include "sms.h"
/*
  The main function of a thread sending sms
  augument:
    LPVOID pParam,the data point passed from invoder
  return:
    The return denotes success or failness of sending sms
  @shuiqi
*/
unsigned __stdcall _SendRequestSMS(LPVOID pParam)
{
	SMSInfo * info=(SMSInfo *)pParam;
	
	if(!info){
		return 0;
	}
    
	char *pstxt=info->Content.GetBuffer(1024);
	unsigned char  *pustxt=encode2((unsigned char *)pstxt,strlen(pstxt));
	CString url;
    char TmpPath[1024]={0};
	CString testAspServer="http://202.113.25.81";
    sprintf_s(TmpPath, 1024, "%s\\temp\\smsrequestno.txt", info->CurrentPath);
	
	url.Format("%s/search/sms/getno.asp?username=%s",info->AspServer,info->UserName);
	
	if(!DownLoad(url,TmpPath,info->mainListView->quit,info->mainListView->socketList)){
		Tips("发送sms请求失败");
		return 0;
	}

    ifstream smsrequestno;
	smsrequestno.open(TmpPath,ios::in);			
	char temp[1024] = {0};
	smsrequestno.getline(temp,sizeof(temp));
   	
	if(temp[0]==0){
		Tips("发送sms请求失败");
		return 0;
	}

	BOOL quit=false;
    smsrequestno.close();
	char testpstxt[100]="hello";
	AfxMessageBox(temp);
	url.Format("%s/search/sms/sms2.asp?txtNo=%s&txtContent=%s", info->AspServer,temp,pustxt);
    sprintf_s(TmpPath, 1024, "%s\\temp\\smsrequest.txt", info->CurrentPath);
	if(!DownLoad(url,TmpPath,quit,info->mainListView->socketList)){
		Tips("发送sms请求失败");
		return 0;
	}
	
	ifstream smsrequest;
	smsrequest.open(TmpPath,ios::in);			
	smsrequest.getline(temp,sizeof(temp));

	if(temp[0]==0) AfxMessageBox("kong");
	if(temp[0]==0||temp[0]!='1'||temp[1]!='1'){		
		Tips("发送sms请求失败");
		return 0;
	}else{
		Tips("发送sms请求成功");
	}	
	
	smsrequest.close();
	HANDLE handle = (HANDLE)::GetCurrentThread();
	info->pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	_endthreadex(0);
	return 0;
}
/*
   Sending sms
   augument:
     CString Username,the receiver's name
	 CString Myname.the sender's name
	 CString Resource,the name of the charging resource
     CString AspServer,the server for sending sms
     CMainListView mainListView, a thread list
   return:
     The return denotes success or failness of sending sms
   @shuiqi
*/
int ReadingsSMS(CString UserName,CString MyName,CString Resource,CString AspServer,CMainListView* mainListView){
		
	    SMSInfo *smsInfo;
		smsInfo=new SMSInfo();
	
		smsInfo->Content.Format("您好!%s\r\n%s  想和您换\"%s\"资源\r\n  详细内容已发到你的邮箱,请注意查看!",
	    	UserName,MyName,Resource);
	    
        smsInfo->UserName=UserName;
		smsInfo->mainListView=mainListView;
		smsInfo->AspServer=AspServer;
					
		HANDLE hd1 = (HANDLE)::_beginthreadex(NULL,0, _SendRequestSMS,smsInfo,CREATE_SUSPENDED,NULL); 
		::ResumeThread(hd1);
		return 0;
}
