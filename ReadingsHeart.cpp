// ReadingsHeart.cpp: implementation of the CReadingsHeart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "FileCollect.h"
#include "ReadingsHeart.h"
#include "Push.h"
#include "PushAdv.h"
#include "HttpUtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern BOOL offline;
extern CMainFrame* pMainWnd;
extern char UserName[50];
extern char AspServer[1024];
extern char CurrentPath[_MAX_PATH];
extern string RequestMail;
extern string AgreeMail;
extern string RefuseMail;
extern string ChargingMail;
extern _ConnectionPtr m_pConnection;
int chatRoomID=  MAX_CHATROOM_ID;
int chatRoomID1= MAX_CHATROOM_ID;
int chatRoomID2= MAX_CHATROOM_ID;
extern BOOL isChatroom1;
extern BOOL isChatroom2;

CReadingsHeart::CReadingsHeart()
{
	
}

CReadingsHeart::~CReadingsHeart()
{
	
}

BOOL CReadingsHeart::Start(long time)
{
	/*�������*/
	BreakTime = 0;
	interval = time;
	
	/*�����������*/
	IsMailSearchDone = TRUE;
	SMSDownloadList = new CList<CString,CString&>;
	
	/*�ʼ���ѯ���*/
	LPCTSTR lpName = "EVENT";
	hEvent=::CreateEvent(NULL,FALSE,FALSE,lpName);
	IsMailSearchDone = TRUE;
	MailSearchCount = 0;
	UnReadedRequestMailList = new CList<CRequestMail,CRequestMail&>();
	UnReadedAgreeMailList = new CList<CAgreeMail,CAgreeMail&>();
	UnReadedChargeMailList = new CList<CChargeMail,CChargeMail&>();
	UnReadedRefuseMailList = new CList<CRefuseMail,CRefuseMail&>();
	ReadedRequestMailList = new CList<CRequestMail,CRequestMail&>();
	ReadedAgreeMailList = new CList<CAgreeMail,CAgreeMail&>();
	ReadedChargeMailList = new CList<CChargeMail,CChargeMail&>();
	ReadedRefuseMailList = new CList<CRefuseMail,CRefuseMail&>();
	HasDownLoadMailAtts = new CList<CString,CString&>();
	
	return TRUE;
}

BOOL CReadingsHeart::Break()
{
	HANDLE handle;
	if (BreakTime == 0 && !pMainWnd->m_quit)//��һ����������������ʱ�����������߳�
	{
#if 0
		//��ʽ�ɼ�
 		
		handle = (HANDLE)_beginthreadex(NULL,0,_FileCollect,NULL,CREATE_SUSPENDED,NULL);
		pMainWnd->m_threadList.AddTail(handle);
		ResumeThread(handle);
#endif	
		if (!offline && 
			strcmp(UserName,"Guest_Default")!=0 && 
			strcmp(UserName,"Guest")!=0)
		{
#if 0
			//Readman��������
			handle = (HANDLE)_beginthreadex(NULL,0,_Readman,this,CREATE_SUSPENDED,NULL);
			pMainWnd->m_threadList.AddTail(handle);
			ResumeThread(handle);
#endif		
			//��ú��ѣ������ҵ���Ϣ
			{
				handle=(HANDLE)_beginthreadex(NULL,0,_GetChatRoom,this,CREATE_SUSPENDED,NULL);
				pMainWnd->m_threadList.AddTail(handle);
				ResumeThread(handle);
				
				handle=(HANDLE)_beginthreadex(NULL,0,_UpdateOnlineUser1,NULL,CREATE_SUSPENDED,NULL);
				pMainWnd->m_threadList.AddTail(handle);
				ResumeThread(handle);
				
				handle=(HANDLE)_beginthreadex(NULL,0,_UpdateOnlineUser2,NULL,CREATE_SUSPENDED,NULL);
				pMainWnd->m_threadList.AddTail(handle);
				ResumeThread(handle);
				
				handle=(HANDLE)_beginthreadex(NULL,0,_UpdateFriends,NULL,CREATE_SUSPENDED,NULL);
				pMainWnd->m_threadList.AddTail(handle);
				ResumeThread(handle);
			}
		}
	}
	else
	{
		if (!offline && 
			strcmp(UserName,"Guest_Default")!=0 && 
			strcmp(UserName,"Guest")!=0 && !pMainWnd->m_quit)
		{
			//ͨ�����������������鼮,15sһ��
			if(interval*BreakTime % 15 == 0 && IsSMSThreadFinished)
			{
				HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _GetBookFromSMS,this,CREATE_SUSPENDED,NULL); 
				pMainWnd->m_threadList.AddTail(hd);
				::ResumeThread(hd);
			}
#if 0	
			//�ʼ���ѯ,30����һ��
			if (interval*BreakTime % (30*60) == 0 && IsMailSearchDone)
			{
				HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _SearchMail,this,CREATE_SUSPENDED,NULL); 
				::ResumeThread(hd);
			}
#endif
#if 0
			
			//����,6����һ��
			if (interval*BreakTime % (6*60) == 0)
			{
				HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _Push,NULL,CREATE_SUSPENDED,NULL); 
				pMainWnd->m_threadList.AddTail(hd);
				::ResumeThread(hd);
			}
#endif
#if 0
			//Readman,4Сʱһ��
			if (interval*BreakTime % (4*60*60) == 0)
			{
				HANDLE hd = (HANDLE)::_beginthreadex(NULL,0, _Readman,NULL,CREATE_SUSPENDED,NULL); 
				pMainWnd->m_threadList.AddTail(hd);
				::ResumeThread(hd);
			}
#endif		
			//������������Ϣ,2����һ��
			if(interval*BreakTime % (2*60) == 0)
			{
				HANDLE handle=(HANDLE)_beginthreadex(NULL,0,_UpdateOnlineUser1,NULL,CREATE_SUSPENDED,NULL);
				pMainWnd->m_threadList.AddTail(handle);
				ResumeThread(handle);
				
				handle=(HANDLE)_beginthreadex(NULL,0,_UpdateOnlineUser2,NULL,CREATE_SUSPENDED,NULL);
				pMainWnd->m_threadList.AddTail(handle);
				ResumeThread(handle);
				
				handle=(HANDLE)_beginthreadex(NULL,0,_UpdateFriends,NULL,CREATE_SUSPENDED,NULL);
				pMainWnd->m_threadList.AddTail(handle);
				ResumeThread(handle);
			}
		}
	}
	BreakTime ++;
	
	return TRUE;
}

unsigned __stdcall CReadingsHeart::_GetBookFromSMS(LPVOID pParam)
{
	CReadingsHeart* heart = (CReadingsHeart*)pParam;
	heart->IsSMSThreadFinished = FALSE;
	CString tmpSMSReqPath;
	tmpSMSReqPath.Format("%s\\temp\\SMSReqList.xml",CurrentPath);
	DeleteFile(tmpSMSReqPath);
	CString SMSReqUrlPath;
	SMSReqUrlPath.Format("http://61.181.14.184:8088/Readings/getDLRequest.do?userName=%s&encoding=utf-8",UserName);
	/*if(!DownLoad(SMSReqUrlPath, tmpSMSReqPath, pMainWnd->m_quit, pMainWnd->m_socketList))
	{
		//AfxMessageBox(SMSReqUrlPath);
		heart->IsSMSThreadFinished = TRUE;
		pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	}*/
	char* buffer;
	HttpUtl utl;
	if(utl.request(SMSReqUrlPath.GetBuffer(0),buffer)==0 || pMainWnd->m_quit)
	{
		heart->IsSMSThreadFinished = TRUE;
		pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	}
	if (!WriteToFile(tmpSMSReqPath, buffer, strlen(buffer)) || pMainWnd->m_quit)
	{
		heart->IsSMSThreadFinished = TRUE;
		pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
		return 0;
	}
	try{
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		try{
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//���󴴽�ʧ��
			if(!pDoc->load(tmpSMSReqPath.GetBuffer(0)))
				throw 2;	//xml ����ʧ��
			pNodeList = pDoc->selectNodes("request");
			if(pNodeList == NULL)
				throw 3;	//xml ��ʽ����
			
			pNodeList = pDoc->selectNodes("request/item");
			if(pNodeList){
				for(int pos=0; pos<pNodeList->length; pos++){
					if (pMainWnd->m_quit)
					{
						return 0;
					}
					CString bookID = GetXmlValue(pNodeList, "bookID", pos);
					if (heart->SMSDownloadList->Find(bookID) != NULL)
					{
						continue;
					}
					heart->SMSDownloadList->AddTail(bookID);
					CString bookName = GetXmlValue(pNodeList, "bookName", pos);
					CString time = GetXmlValue(pNodeList, "time", pos);
					CString source = GetXmlValue(pNodeList, "source", pos);
					CString filetype = GetXmlValue(pNodeList, "fileType", pos);
					CString requestid = GetXmlValue(pNodeList, "id", pos);
					CString url = GetXmlValue(pNodeList, "url", pos);
					//AfxMessageBox(source);
					/*if(source == "makefile")
					{
						url.Format("%s/HanLinBook/MakeFilePage/temp/%s.%s", AspServer, bookName, filetype);
					}else{
						url.Format("http://61.181.14.184:8088/Readings/downloadBook.do?bookID=%s&source=%s",bookID,source);
					}*/
					//AfxMessageBox(url);
					DownLoadMissionInfo* missionInfo = (DownLoadMissionInfo*)malloc(sizeof(DownLoadMissionInfo));
					ZeroMemory(missionInfo, sizeof(DownLoadMissionInfo));
					CString DstPath;
					DstPath.Format("%s\\share\\%s.%s",CurrentPath,bookName,filetype);
					int i = 1;
					while (PathFileExists(DstPath))
					{
						DstPath.Format("%s\\share\\%s(%d).%s",CurrentPath,bookName,i,filetype);
						i ++;
					}
					strcpy_s(missionInfo->DstPath, _MAX_PATH, DstPath);
					strcpy_s(missionInfo->URL, 1024, url);
					strcpy_s(missionInfo->title, _MAX_PATH, bookName);
					strcpy_s(missionInfo->bookID, 50, bookID);
					strcpy_s(missionInfo->requestID, 50, requestid);
					missionInfo->sourceType = dl_http;
					missionInfo->from = from_SMS;
					pMainWnd->PostMessage(WM_FILEDOWNLOAD,(WPARAM)missionInfo);
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(int errnum){
			switch(errnum){
			case 3:		//�Ѹ���
				pNodeList.Release();
			case 2:		//��ʽ����
				pDoc.Release();
				break;
			}
		}
		catch(_com_error& e){}
	}
	catch(...){}
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	heart->IsSMSThreadFinished = TRUE;
	DeleteFile(tmpSMSReqPath);
	return 0;
}

unsigned __stdcall CReadingsHeart::_SearchMail(LPVOID pParam)
{
	CReadingsHeart* p = (CReadingsHeart*)pParam;
	if (p->IsMailSearchDone == FALSE)
	{
		return 0;
	}
	p->MailStat = "���ڽ����ʼ���ѯ";
	if (pMainWnd->RecvDlg != NULL)
	{
		pMainWnd->RecvDlg->m_stat.SetWindowText(p->MailStat);
	}
	
	p->IsMailSearchDone = FALSE;
	
	p->UnReadedRequestMailList->RemoveAll();
	p->UnReadedChargeMailList->RemoveAll();
	p->UnReadedRefuseMailList->RemoveAll();
	p->UnReadedAgreeMailList->RemoveAll();
	
	
	if (p->GetEMailInfo() == false)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		p->IsMailSearchDone = TRUE;
		p->MailStat = "��ȡ������Ϣʧ��";
		if (pMainWnd->RecvDlg != NULL)
		{
			pMainWnd->RecvDlg->m_stat.SetWindowText(p->MailStat);
		}
		Tips("��ȡ������Ϣʧ��");
		return 0;
	}
	
	string MyUserName = UserName;
	string MessageID;
	
	//�������ʼ�����
	CoInitialize(NULL); 
	if (p->MailSearchCount != 0)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		::SetEvent(p->hEvent);//�˳��ϴ��ʼ���ѯ,��һ����ѯǰû����һ����ѯ
	}else{
		p->MailSearchCount = 1;
	}
	
	try{
		jmail::IPOP3Ptr pPOP3test("JMail.POP3");
	}catch(_com_error e){
		//		Tips("δע��jmail");
		p->IsMailSearchDone = TRUE;
		p->MailStat = "δע��jmail";
		if (pMainWnd->RecvDlg != NULL)
		{
			pMainWnd->RecvDlg->m_stat.SetWindowText(p->MailStat);
		}
		::WaitForSingleObject(p->hEvent,INFINITE);
		return 0;
	}
	if (pMainWnd->m_quit)
	{
		return 0;
	}
	
	char WebPath[1024];
	sprintf_s(WebPath, 1024, "%s/emailrequest/getrequestlist.asp?username=%s",AspServer, UserName);
	//AfxMessageBox(WebPath);
	char* buffer;
	HttpUtl utl;
	
	if(utl.request(WebPath,buffer)==0)
	{
		Tips("��ȡ�ʼ������б�ʧ�ܣ����鵱ǰ�������ӣ�");
		return 0;
	}
	//AfxMessageBox("getXML");
	try{
		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		try{
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//���󴴽�ʧ��
			if(!pDoc->loadXML(buffer))
				throw 2;	//xml ����ʧ��
			pNodeList = pDoc->selectNodes("requestList");
			if(pNodeList == NULL)
				throw 3;	//xml ��ʽ����
			
			pNodeList = pDoc->selectNodes("requestList/request");
			if(pNodeList){
				for(int pos=0; pos<pNodeList->length; pos++){
					if (pMainWnd->m_quit)
					{
						return 0;
					}
					string bookName = GetXmlValue(pNodeList, "title", pos).GetBuffer(0);
					string ExName = bookName.substr(bookName.find_last_of('.') + 1 ,bookName.length() - bookName.find_last_of('.') - 1);
					string FileName = bookName.substr(0 ,bookName.length() - ExName.length() - 1);
					string mailstate = GetXmlValue(pNodeList, "stat", pos).GetBuffer(0);
					string time = GetXmlValue(pNodeList, "time", pos).GetBuffer(0);
					string requester = GetXmlValue(pNodeList, "requester", pos).GetBuffer(0);
					string requestid = GetXmlValue(pNodeList, "id", pos).GetBuffer(0);
					if (mailstate == "finished")
					{
						string msg = "����������ļ�\""+bookName+"\"�Է��Ѵ�����ϣ�����������ʼ�";
						Tips(msg.c_str());
						sprintf_s(WebPath, 1024, "%s/emailrequest/delrequest.asp?id=%s",AspServer,requestid.c_str());
						UpLoad(WebPath);
						continue;
					}
					string TheWholePath = CEMailADO::FindFilePath(bookName);
					//AfxMessageBox(TheWholePath.c_str());
					sprintf_s(WebPath, 1024, "%s/emailrequest/getemailaddr.asp?username=%s", AspServer,requester.c_str());
					char* buffer;
					HttpUtl utl;
					//AfxMessageBox(WebPath);
					if(utl.request(WebPath,buffer)==0)
					{
						Tips("�����ʼ�ʧ�ܣ����鵱ǰ�������ӣ�");
						return 0;
					}
					string requestermail(buffer);
					if(TheWholePath.length() == 0)
					{
						Tips("�������ļ�δ�ҵ�!������һ���ܾ��ʼ�");
						jmail::IMessagePtr pMessage1("JMail.Message");
						try
						{
							pMessage1->MailServerUserName = pMainWnd->Heart->MyEMailUserName.c_str();   
							pMessage1->MailServerPassWord = pMainWnd->Heart->publicemailpass.c_str();
							// ����������
							pMessage1->From = (pMainWnd->Heart->MyEMailUserName + "@" + pMainWnd->Heart->MyEMailServer).c_str();
							// ����������
							pMessage1->FromName = "Readings";
							// ����ռ���
							pMessage1->AddRecipient(requestermail.c_str(), "", "");
							// ���ȼ�����,1-5��ν���, 3Ϊ�м�
							pMessage1->Priority = 3;
							// ���뷽ʽ����, Ĭ����iso-8859-1
							pMessage1->Charset = "GB2312";
							// ����
							pMessage1->Subject = (bookName + RefuseMail).c_str();
							// ����
							pMessage1->AppendText(("username:" + string(UserName) + "\r\n").c_str());
							pMessage1->AppendText(("filename:" + bookName + "\r\n").c_str());
							pMessage1->AppendText("reason:�����ļ���Ŀ�������δ�ҵ�\r\n");
							// ��ʼ����
							pMessage1->Send(pMainWnd->Heart->SmtpServer.c_str(), VARIANT_FALSE);
							Tips("�ظ��ܾ��ʼ�!");
						}
						catch (_com_error e)
						{
							CString strErr;
							strErr.Format("������Ϣ��%s\r\n����������%s", (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
							Tips(strErr);
						}
						if (pMessage1 != NULL)
						{
							pMessage1.Release();
						}
					}else //�ҵ���������ļ��������ļ�
					{
						CFile att;
						att.Open(TheWholePath.c_str(),CFile::modeRead);
						if (att.GetLength() > 10*1024*1024)
						{
							att.Close();
							jmail::IMessagePtr pMessage1("JMail.Message");
							try
							{
								pMessage1->MailServerUserName = pMainWnd->Heart->MyEMailUserName.c_str();   
								pMessage1->MailServerPassWord = pMainWnd->Heart->publicemailpass.c_str();
								// ����������
								pMessage1->From = (pMainWnd->Heart->MyEMailUserName + "@" + pMainWnd->Heart->MyEMailServer).c_str();
								// ����������
								pMessage1->FromName = "Readings";
								// ����ռ���
								pMessage1->AddRecipient(requestermail.c_str(), "", "");
								// ���ȼ�����,1-5��ν���, 3Ϊ�м�
								pMessage1->Priority = 3;
								// ���뷽ʽ����, Ĭ����iso-8859-1
								pMessage1->Charset = "GB2312";
								// ����
								pMessage1->Subject = (bookName + RefuseMail).c_str();
								// ����
								pMessage1->AppendText(("username:" + string(UserName) + "\r\n").c_str());
								pMessage1->AppendText(("filename:" + bookName + "\r\n").c_str());
								pMessage1->AppendText("reason:�������ļ�����\r\n");
								// ��ʼ����
								pMessage1->Send(pMainWnd->Heart->SmtpServer.c_str(), VARIANT_FALSE);
								Tips("�ļ�����!������һ��ܾ��ʼ�");
							}
							catch (_com_error e)
							{
								CString strErr;
								strErr.Format("������Ϣ��%s\r\n����������%s", (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
								Tips(strErr);
							}
							sprintf_s(WebPath, 1024, "%s/emailrequest/finishrequest.asp?id=%s",AspServer,requestid.c_str());
							UpLoad(WebPath);
							continue;
						}
						att.Close();
						Tips("�ҵ��ļ�,���ڷ���...");
						jmail::IMessagePtr pMessage1("JMail.Message");
						try
						{
							pMessage1->MailServerUserName = pMainWnd->Heart->MyEMailUserName.c_str();   
							//AfxMessageBox(pMessage1->MailServerUserName);
							pMessage1->MailServerPassWord = pMainWnd->Heart->publicemailpass.c_str();
							//AfxMessageBox(pMessage1->MailServerPassWord);
							// ����������
							pMessage1->From = (pMainWnd->Heart->MyEMailUserName + "@" + pMainWnd->Heart->MyEMailServer).c_str();
							// ����������
							pMessage1->FromName = MyUserName.c_str();
							// ����ռ���
							pMessage1->AddRecipient(requestermail.c_str(), "", "");
							//AfxMessageBox(requestermail.c_str());
							// ���ȼ�����,1-5��ν���, 3Ϊ�м�
							pMessage1->Priority = 3;
							// ���뷽ʽ����, Ĭ����iso-8859-1
							pMessage1->Charset = "GB2312";
							// ����
							pMessage1->Subject = (bookName + " from " + string(UserName)).c_str();
							pMessage1->AppendText(pMessage1->Subject);
							//����
							//AfxMessageBox(ExName.c_str());
							pMessage1->AddAttachment(TheWholePath.c_str(),true,ExName.c_str());
							// ��ʼ����
							pMessage1->Send(pMainWnd->Heart->SmtpServer.c_str(), VARIANT_FALSE);
							Tips("�ʼ������ѳɹ��ظ�!");
						}
						catch (_com_error e)
						{
							CString strErr;
							strErr.Format("������Ϣ��%s\r\n����������%s", (LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description());
							Tips(strErr);
						}
						if (pMessage1 != NULL)
						{
							pMessage1.Release();
						}
						
					}
					sprintf_s(WebPath, 1024, "%s/emailrequest/finishrequest.asp?id=%s",AspServer,requestid.c_str());
					UpLoad(WebPath);
				}
			}
		}
		catch(int errnum){
			switch(errnum){
			case 3:		//�Ѹ���
				pNodeList.Release();
			case 2:		//��ʽ����
				pDoc.Release();
				break;
			}
		}
		catch(_com_error& e){
			//Tips(CString(e.Description()));
		}
	}
	catch(...){
		Tips("...");
	}
	p->IsMailSearchDone = TRUE;
	p->MailStat = "�ʼ���ѯ�����";
	if (pMainWnd->RecvDlg != NULL)
	{
		pMainWnd->RecvDlg->m_stat.SetWindowText(p->MailStat);
	}
	::WaitForSingleObject(p->hEvent,INFINITE);
	//////////////////////////////////////////////////////////////////////////
	return 0;
}

bool CReadingsHeart::GetEMailInfo()
{
	publicemailaddr = "publicreadings1@sina.com";
	publicemailpass = "123456";
	MyEMailUserName = "publicreadings1";
	MyEMailServer = "sina.com";
	SmtpServer = "smtp.sina.com.cn";
	PopServer = "pop.sina.com.cn";
	return true;
}

void CReadingsHeart::ReflushMail()
{
	HANDLE	handle = (HANDLE)_beginthreadex(NULL,0,_SearchMail,this,CREATE_SUSPENDED,NULL);
	ResumeThread(handle);
}

unsigned __stdcall CReadingsHeart::_FileCollect(LPVOID pParam)
{
	int i = 0;//5min��ʼ
	while (i < 60*10)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		i ++;
		Sleep(500);
	}
	if(pMainWnd){
		CFileCollect fileCol;
		if(fileCol.Initial()){
			fileCol.Do(pMainWnd->m_quit);
			fileCol.Release();
		}
	}
	HANDLE handle = (HANDLE)::GetCurrentThread();
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	return 0;
}

unsigned __stdcall CReadingsHeart::_Push(LPVOID pParam)
{
	try{
		char iniPath[_MAX_PATH] = {0};
		sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
		
		char val[50] = {0};
		GetPrivateProfileString("push", "resource", NULL, val, 50, iniPath);
		if(atoi(val))
			return 1;
		
		BOOL hasothernew = CPush::PushInfoToDB();
		hasothernew = CPush::DownLoadPushList();
		
		if(hasothernew && !pMainWnd->m_quit){
			Tips("������������������Ϣ��");
			pMainWnd->PostMessage(WM_SYN_TO_SD);
		}
	}
	catch(...){
	}
	
	HANDLE handle = (HANDLE)::GetCurrentThread();
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	return 0;
}

BOOL CReadingsHeart::AddToPushList(DownLoadMissionInfo* missionInfo)
{
	try{
		_RecordsetPtr m_pRecordset;
		char SQL[1000]={0};
		sprintf_s(SQL, 1000, "DELETE from push WHERE path=\"%s\"", missionInfo->DstPath);
		m_pConnection->Execute(SQL, NULL,adCmdText);
		memset(SQL, 0, 1000);
		strcpy_s(SQL, 1000, "SELECT * FROM push");
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // ��ȡ��ӿ��IDispatchָ��
			adOpenDynamic, adLockOptimistic, adCmdText);
		m_pRecordset->AddNew();
		m_pRecordset->PutCollect("title",(_variant_t)missionInfo->title);
		m_pRecordset->PutCollect("path",(_variant_t)missionInfo->DstPath);
		m_pRecordset->PutCollect("type",(_variant_t)("Readman"));
		m_pRecordset->PutCollect("filetype",(_variant_t)(long)GetFileTypeFromFull(missionInfo->DstPath));
		CString slen;
		slen.Format("%d", missionInfo->fileLen);
		m_pRecordset->PutCollect("size",(_variant_t)slen);
		m_pRecordset->PutCollect("cdate",(_variant_t)GetDate());
		m_pRecordset->PutCollect("user",(_variant_t)UserName);
		m_pRecordset->Update();
		m_pRecordset->Close();
	}catch(...){
		return FALSE;
	}
	return TRUE;
}

unsigned __stdcall CReadingsHeart::_Readman(LPVOID pParam)
{
	int i = 0;//200s��ʼ
	while (i < 400)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		i ++;
		Sleep(500);
	}
	try{
		char iniPath[_MAX_PATH] = {0};
		sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
		
		char val[50] = {0};
		GetPrivateProfileString("push", "Readman", NULL, val, 50, iniPath);
		if(atoi(val))
			return 1;
		
		BOOL hasothernew = CPushAdv::PushReadman();
		
		if(hasothernew && !pMainWnd->m_quit){
			Tips("Readman���ŵ��ˣ�");
			pMainWnd->PostMessage(WM_SYN_TO_SD);
		}
		
		CPushAdv::PushAdv(pParam);
	}
	catch(...){
	}
	
	HANDLE handle = (HANDLE)::GetCurrentThread();
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)handle);
	return 0;
}

unsigned __stdcall CReadingsHeart::_GetChatRoom(LPVOID pParam)
{
	CString URL,dstPath,temp;
	URL.Format("%s/Msg/getChatRoom.asp?username=%s", AspServer,UserName);
	dstPath.Format("%s/chatRoom.xml", CurrentPath);
	if (DownLoad(URL, dstPath, pMainWnd->m_quit, pMainWnd->m_socketList)!=TRUE)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		chatRoomID = MAX_CHATROOM_ID;
		chatRoomID1= MAX_CHATROOM_ID;
		chatRoomID2= MAX_CHATROOM_ID;
		return 1;
	}	
	if (pMainWnd->m_quit)
	{
		return 0;
	}
    MSXML::IXMLDOMDocumentPtr pDoc;
    MSXML::IXMLDOMNodeListPtr pNodeList;
	
	
	int nIndex;	
	try
	{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
		{
			throw 1;
		}	
		
		if(!pDoc->load((LPCSTR)dstPath))
		{
			throw 2;	//xml ����ʧ��
		}
		pNodeList = pDoc->selectNodes("chatroomlist/chatroom");
		if(pNodeList)
		{
			DWORD count = pNodeList->length;
			
			for(nIndex=0; nIndex<pNodeList->length&&nIndex<2 ;nIndex++)
			{
				temp = GetXmlValue(pNodeList,"chatroomid",nIndex);
				
				if (atoi(temp)>0)
				{
					chatRoomID2 = atoi(temp);
				}
				
				
			}
			pNodeList.Release();
		}
		pDoc.Release();
		
	}
	catch(int e)
	{
		switch(e)
		{
		case 1: pDoc.Release();
			break;
		case 2: pDoc.Release();
			break;
		}
	}
	chatRoomID1 =0;
	
	chatRoomID = chatRoomID1;
	DeleteFile(dstPath);
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return TRUE;
}

unsigned __stdcall CReadingsHeart::_UpdateOnlineUser1(LPVOID pParam)
{
	int i = 0;//10s��ʼ
	while (i < 20)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		i ++;
		Sleep(500);
	}
	
	int nIndex=0;
	
	if ( chatRoomID1 == MAX_CHATROOM_ID)
	{
		return FALSE;
	}
    MSXML::IXMLDOMDocumentPtr pDoc;
    MSXML::IXMLDOMNodeListPtr pNodeList;
	//return TRUE;

	CString URL, dstPath;
//	while (p->m_quit==FALSE)
	{
		try
		{
		    HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		    if(!SUCCEEDED(hr))
			{
			    throw 1;
			}
			Sleep(2000);
			URL.Format("%s/msg/getUserInfo.asp?chatID=0", AspServer);//,0);
			dstPath.Format("%s/temp/userinfo1.xml", CurrentPath);
			if(DownLoad(URL, dstPath, pMainWnd->m_quit, pMainWnd->m_socketList)==FALSE)
			{
				pDoc.Release();
			//	AfxMessageBox("Download failed.");
				return FALSE;
			}	
			if (pMainWnd->m_quit)
			{
				DeleteFile(dstPath);
				return FALSE;
			}
		    if(!pDoc->load((LPCSTR)dstPath))
			{
			//	AfxMessageBox("load failed");
			    throw 2;	//xml ����ʧ��
			}
		    pNodeList = pDoc->selectNodes("userList/user");
		    if(pNodeList)
			{
// 				HTREEITEM hItem_temp = p->m_pCommDlg->m_tree.GetChildItem(p->m_pCommDlg->hFriend);
			DWORD count = pNodeList->length;
			HTREEITEM hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hFriend);
			while (hItem_temp)
			{
				pMainWnd->m_pCommDlg->m_tree.DeleteItem(hItem_temp);
				hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hFriend);
			//	AfxMessageBox("while failed.");
			}
					
				for(nIndex=0; nIndex<pNodeList->length ; nIndex++)
				{
					STRUCTUSERINFO* info = (STRUCTUSERINFO*)malloc(sizeof(STRUCTUSERINFO));			
					ZeroMemory(info,sizeof(STRUCTUSERINFO));
					sprintf_s(info->UserName, 30, "%s",GetXmlValue(pNodeList, "name",nIndex)); 	    
					sprintf_s(info->NetName, 50, "%s",GetXmlValue(pNodeList,"location",nIndex));				
					sprintf_s(info->IP, 20, "%s",GetXmlValue(pNodeList,"IP",nIndex));

					if(strlen(info->UserName))
					{
						hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hFriend);
						BOOL ext = FALSE;					
						while(hItem_temp)
						{						
							CString str = pMainWnd->m_pCommDlg->m_tree.GetItemText(hItem_temp);
							CString sname = "";
							sname = str.Right(str.GetLength()-str.Find('-')-1);
							sname = sname.Left(sname.Find('('));					
							if(sname.Compare(info->UserName)==0)
							{
								TreeItemInfo* tinfo = (TreeItemInfo*)pMainWnd->m_pCommDlg->m_tree.GetItemData(hItem_temp);
								CString lsPath;
								free((STRUCTUSERINFO*)tinfo->BookNumber);
								pMainWnd->m_pCommDlg->m_tree.DeleteItem(hItem_temp);
								//ext = TRUE;
							}
							hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetNextSiblingItem(hItem_temp);						
						}
						if(!ext)
						{
							CString title;
							title.Format("��ͺ���(%d��)", count);
							pMainWnd->m_pCommDlg->m_tree.SetItemText(pMainWnd->m_pCommDlg->hFriend, title);				
							CString ftitle;
						//	AfxMessageBox(info->NetName);
							if (strlen(info->NetName)<1)
							{
								ftitle.Format("δ֪����-%s",info->UserName);//GetXmlValue(pNodeList,"num",nIndex));				
							}
							else				
							    ftitle.Format("%s-%s",info->NetName,info->UserName);//GetXmlValue(pNodeList,"num",nIndex));				
				
							HTREEITEM hItem = pMainWnd->m_pCommDlg->m_tree.InsertItem(ftitle, 27, 27, pMainWnd->m_pCommDlg->hFriend);
							TreeItemInfo* tinfo = (TreeItemInfo*)malloc(sizeof(TreeItemInfo));
							ZeroMemory(tinfo, sizeof(TreeItemInfo));
							tinfo->Type = Is_NetFirend;
							tinfo->BookNumber = (DWORD)info;
							pMainWnd->m_pCommDlg->m_tree.SetItemData(hItem,(DWORD)tinfo);
						}		
					}
					else
						free(info);						
					
				}
			    pNodeList.Release();
			}
		    pDoc.Release();
		}
		catch(int e)
		{
			switch(e)
			{
			case 1: pDoc.Release();
				break;
			case 2: pDoc.Release();
				break;
			}
		}
		pMainWnd->m_pCommDlg->m_tree.Invalidate(FALSE);
		
	//	Sleep(TIME_1_MINUTE);
	}
	DeleteFile(dstPath);
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return TRUE;
}

unsigned __stdcall CReadingsHeart::_UpdateOnlineUser2(LPVOID pParam)
{
	int i = 0;//15s��ʼ
	while (i < 30)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		i ++;
		Sleep(500);
	}

	int nIndex=0;

	if (chatRoomID2 == MAX_CHATROOM_ID)
	{
		int tmp=0;
		while (chatRoomID2 == MAX_CHATROOM_ID)
		{
			if (pMainWnd->m_quit)
			{
				return 0;
			}
			Sleep(2*1000);
			tmp++;
			if (tmp>5 && chatRoomID2 == MAX_CHATROOM_ID)
			{
			    HANDLE handle=(HANDLE)_beginthreadex(NULL,0,_GetChatRoom,pMainWnd,CREATE_SUSPENDED,NULL);
				ResumeThread(handle);
				return FALSE;
			}
		}	
	}
	MSXML::IXMLDOMDocumentPtr pDoc;
    MSXML::IXMLDOMNodeListPtr pNodeList;

	CString URL, dstPath;
//	while (p->m_quit==FALSE)
	{
		try
		{
		    HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		    if(!SUCCEEDED(hr))
			{
			    throw 1;
			}		
			Sleep(4000);
			URL.Format("%s/msg/getUserInfo.asp?chatID=%d", AspServer,chatRoomID2);
			dstPath.Format("%s/temp/userinfo2.xml", CurrentPath);
			if(DownLoad(URL, dstPath, pMainWnd->m_quit, pMainWnd->m_socketList)==FALSE)
				throw 1;
			if (pMainWnd->m_quit)
			{
				DeleteFile(dstPath);
				return FALSE;
			}
		    if(!pDoc->load((LPCSTR)dstPath))
			{
			    throw 2;	//xml ����ʧ��
			}
		    pNodeList = pDoc->selectNodes("userList/user");
		    if(pNodeList)
			{
 				HTREEITEM hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hChatroom);
				DWORD count = pNodeList->length;
 				while (hItem_temp)
 				{
 					pMainWnd->m_pCommDlg->m_tree.DeleteItem(hItem_temp);
					hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hChatroom);
 				}
					
				for(nIndex=0; nIndex<pNodeList->length ; nIndex++)
				{
					STRUCTUSERINFO* info = (STRUCTUSERINFO*)malloc(sizeof(STRUCTUSERINFO));			
					sprintf_s(info->UserName, 30, "%s",GetXmlValue(pNodeList, "name",nIndex)); 	    
					sprintf_s(info->NetName, 50, "%s",GetXmlValue(pNodeList, "location",nIndex));
					sprintf_s(info->IP, 20, "%s",GetXmlValue(pNodeList, "IP",nIndex));

					if(strlen(info->UserName))
					{
						HTREEITEM hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hChatroom);
						BOOL ext = FALSE;					
						while(hItem_temp)
						{						
							CString str = pMainWnd->m_pCommDlg->m_tree.GetItemText(hItem_temp);
							CString sname = "";
							sname = str.Right(str.GetLength()-str.Find('-')-1);
							sname = sname.Left(sname.Find('('));					
							if(sname.Compare(info->UserName)==0)
							{
								TreeItemInfo* tinfo = (TreeItemInfo*)pMainWnd->m_pCommDlg->m_tree.GetItemData(hItem_temp);
								CString lsPath;
								free((STRUCTUSERINFO*)tinfo->BookNumber);
								pMainWnd->m_pCommDlg->m_tree.DeleteItem(hItem_temp);
								//ext = TRUE;
							}
							hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetNextSiblingItem(hItem_temp);						
						}
						if(!ext)
						{
							CString title;
							title.Format("ͬ�����(%d��)", count);
							pMainWnd->m_pCommDlg->m_tree.SetItemText(pMainWnd->m_pCommDlg->hChatroom, title);				
							CString ftitle;

						
							if (strlen(info->NetName) <1)
							{
								ftitle.Format("δ֪����-%s",info->UserName);
							}
							else
								ftitle.Format("%s-%s",info->NetName,info->UserName);//,GetXmlValue(pNodeList,"num",nIndex));				
				
							HTREEITEM hItem = pMainWnd->m_pCommDlg->m_tree.InsertItem(ftitle, 27, 27, pMainWnd->m_pCommDlg->hChatroom);
							TreeItemInfo* tinfo = (TreeItemInfo*)malloc(sizeof(TreeItemInfo));
							ZeroMemory(tinfo, sizeof(TreeItemInfo));
							tinfo->Type = Is_NetFirend;
							tinfo->BookNumber = (DWORD)info;
							pMainWnd->m_pCommDlg->m_tree.SetItemData(hItem,(DWORD)tinfo);
						}		
					}
					else
						free(info);						
					
				}
			    pNodeList.Release();
			}
		    pDoc.Release();
		}
		catch(int e)
		{
			switch(e)
			{
			case 1: pDoc.Release();
				break;
			case 2: pDoc.Release();
				break;
			}
		}
		pMainWnd->m_pCommDlg->m_tree.Invalidate(FALSE);
		
	//	Sleep(TIME_1_MINUTE);
	}
	
//	Tips("New user comes.");
	DeleteFile(dstPath);
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return TRUE;
}

unsigned __stdcall CReadingsHeart::_UpdateFriends(LPVOID pParam)
{
	int i = 0;//20s��ʼ
	while (i < 40)
	{
		if (pMainWnd->m_quit)
		{
			return 0;
		}
		i ++;
		Sleep(500);
	}

	int nIndex=0;	

    MSXML::IXMLDOMDocumentPtr pDoc;
    MSXML::IXMLDOMNodeListPtr pNodeList;
	//return TRUE;

	CString URL, dstPath;
//	while (p->m_quit==FALSE)
	{
		try
		{
		    HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		    if(!SUCCEEDED(hr))
			{
			    throw 1;
			}			
			URL.Format("%s/msg/getfriends.asp?username=%s", AspServer,UserName);
			dstPath.Format("%s/temp/friends.xml", CurrentPath);
			if(DownLoad(URL, dstPath, pMainWnd->m_quit, pMainWnd->m_socketList)==FALSE)
			{
				pDoc.Release();
				return FALSE;
			}	
			if (pMainWnd->m_quit)
			{
				return FALSE;
			}
		    if(!pDoc->load((LPCSTR)dstPath))
			{
			    throw 2;	//xml ����ʧ��
			}
		    pNodeList = pDoc->selectNodes("userList/user");
		    if(pNodeList)
			{
				HTREEITEM hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hMyFriend);
				DWORD count = pNodeList->length;
 				while (hItem_temp)
 				{
 					pMainWnd->m_pCommDlg->m_tree.DeleteItem(hItem_temp);
					hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hMyFriend);
 				}
					
				for(nIndex=0; nIndex<pNodeList->length ; nIndex++)
				{
					STRUCTUSERINFO* info = (STRUCTUSERINFO*)malloc(sizeof(STRUCTUSERINFO));			
					ZeroMemory(info,sizeof(STRUCTUSERINFO));
					sprintf_s(info->UserName, 30, "%s",GetXmlValue(pNodeList, "name",nIndex)); 	    
					sprintf_s(info->NetName, 50, "%s",GetXmlValue(pNodeList,"location",nIndex));				
					sprintf_s(info->IP, 20, "%s",GetXmlValue(pNodeList,"IP",nIndex));

					if(strlen(info->UserName))
					{
						HTREEITEM hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetChildItem(pMainWnd->m_pCommDlg->hMyFriend);
						BOOL ext = FALSE;					
						while(hItem_temp)
						{						
							CString str = pMainWnd->m_pCommDlg->m_tree.GetItemText(hItem_temp);
							CString sname = "";
							sname = str.Right(str.GetLength()-str.Find('-')-1);
							sname = sname.Left(sname.Find('('));					
							if(sname.Compare(info->UserName)==0)
							{
								TreeItemInfo* tinfo = (TreeItemInfo*)pMainWnd->m_pCommDlg->m_tree.GetItemData(hItem_temp);
								CString lsPath;
								free((STRUCTUSERINFO*)tinfo->BookNumber);
								pMainWnd->m_pCommDlg->m_tree.DeleteItem(hItem_temp);
								//ext = TRUE;
							}
							hItem_temp = pMainWnd->m_pCommDlg->m_tree.GetNextSiblingItem(hItem_temp);						
						}
						if(!ext)
						{
							CString title;
							title.Format("�ҵĺ���(%d��)", count);
							pMainWnd->m_pCommDlg->m_tree.SetItemText(pMainWnd->m_pCommDlg->hMyFriend, title);				
							CString ftitle;
				
							ftitle.Format("%s-%s(%s)",info->NetName,info->UserName,GetXmlValue(pNodeList,"num",nIndex));				
				
							HTREEITEM hItem = pMainWnd->m_pCommDlg->m_tree.InsertItem(ftitle, 27, 27, pMainWnd->m_pCommDlg->hMyFriend);
							TreeItemInfo* tinfo = (TreeItemInfo*)malloc(sizeof(TreeItemInfo));
							ZeroMemory(tinfo, sizeof(TreeItemInfo));
							tinfo->Type = Is_NetFirend;
							tinfo->BookNumber = (DWORD)info;
							pMainWnd->m_pCommDlg->m_tree.SetItemData(hItem,(DWORD)tinfo);
						}		
					}
					else
						free(info);						
					
				}
			    pNodeList.Release();
			}
		    pDoc.Release();
		}
		catch(int e)
		{
			switch(e)
			{
			case 1: pDoc.Release();
				break;
			case 2: pDoc.Release();
				break;
			}
		}
		pMainWnd->m_pCommDlg->m_tree.Invalidate(FALSE);
		
	//	Sleep(TIME_1_MINUTE);
	}
	DeleteFile(dstPath);
	pMainWnd->PostMessage(WM_THREAD_FINISHED, (WPARAM)::GetCurrentThread());
	return TRUE;
}

void CReadingsHeart::RefreshUser()
{
	if (strcmp(UserName,"Guest_Default")!=0 &&strcmp(UserName,"Guest")!=0)
	{
		HANDLE handle1=(HANDLE)_beginthreadex(NULL,0,_GetChatRoom,NULL,CREATE_SUSPENDED,NULL);
		pMainWnd->m_threadList.AddTail(handle1);
		ResumeThread(handle1);

		handle1 = (HANDLE)_beginthreadex(NULL,0,_UpdateOnlineUser1,NULL,CREATE_SUSPENDED,NULL);
		pMainWnd->m_threadList.AddTail(handle1);
		ResumeThread(handle1);

		handle1 = (HANDLE)_beginthreadex(NULL,0,_UpdateOnlineUser2,NULL,CREATE_SUSPENDED,NULL);
		pMainWnd->m_threadList.AddTail(handle1);
		ResumeThread(handle1);
	}
}
