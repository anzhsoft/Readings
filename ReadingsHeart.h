// ReadingsHeart.h: interface for the CReadingsHeart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_READINGSHEART_H__9AC7091D_C4C0_41EF_B21C_7E330B2CD2C2__INCLUDED_)
#define AFX_READINGSHEART_H__9AC7091D_C4C0_41EF_B21C_7E330B2CD2C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*****************************
 * create time : 2010 03 23
 * author : wangcong
 * description : 
 * Readings ��������ʵ���࣬��MainFrame�г�ʼ��
 * ����һЩ��ʱ���ͳ�������ʱ��������һЩ�߳�
******************************/

class CReadingsHeart
{
public://��������
	/*�������*/
	long BreakTime;//��������
	long interval;//�������,����Ϊ��λ
	//CThreadList threadList;//��߳��б�,��pMainWnd�н���ͳһ�̹߳���

	/*�����������*/
	BOOL IsSMSThreadFinished;//�����ж��ϴ���ѯ�Ƿ����
	CList<CString,CString&>* SMSDownloadList;//ͨ�����������������ص��鼮��ID�б�

	/*�ʼ���ѯ���*/
	HANDLE hEvent;//������ͣ��ʼ�ʼ���ѯ�߳�
	string MyEMailPassWord;
	string MyEMailUserName;
	string MyEMailServer;
	string PopServer;
	string SmtpServer;
	string publicemailaddr;//���������ַ
	string publicemailpass;//������������
	int MailSearchCount;//�����ж��Ƿ��ǵ�һ����ѯ
	CString MailStat;//�ʼ���ѯ��ǰ����״̬
	BOOL IsMailSearchDone;//�����ж��ϴ���ѯ�Ƿ����
	CList<CString,CString&>* HasDownLoadMailAtts;
	CList<CRequestMail,CRequestMail&>* UnReadedRequestMailList;
	CList<CAgreeMail,CAgreeMail&>* UnReadedAgreeMailList;
	CList<CChargeMail,CChargeMail&>* UnReadedChargeMailList;
	CList<CRefuseMail,CRefuseMail&>* UnReadedRefuseMailList;
	CList<CRequestMail,CRequestMail&>* ReadedRequestMailList;
	CList<CAgreeMail,CAgreeMail&>* ReadedAgreeMailList;
	CList<CChargeMail,CChargeMail&>* ReadedChargeMailList;
	CList<CRefuseMail,CRefuseMail&>* ReadedRefuseMailList;

	/*�������*/

	/*�ϴ������ļ���Ϣ���*/

public://��������	
	/*�������*/
	BOOL Break();//����һ��
	BOOL Start(long time);//���࿪ʼ�
	CReadingsHeart();
	virtual ~CReadingsHeart();

	/*�����������*/
	BOOL AddToPushList(DownLoadMissionInfo* missionInfo);//���뵽������

	/*�ʼ���ѯ���*/
	void ReflushMail();//�ʼ�ˢ��
	bool GetEMailInfo();//��ȡ������Ϣ

	/*�������*/

	/*�ϴ������ļ���Ϣ���*/
	

public://��������ѯ�߳�
	void RefreshUser();//ˢ�������û���Ϣ
	static unsigned __stdcall _UpdateFriends(LPVOID pParam);//��ʱ�ӷ������鿴�û�������Ϣ
	static unsigned __stdcall _UpdateOnlineUser2(LPVOID pParam);//��ʱ�ӷ������鿴�û�������Ϣ
	static unsigned __stdcall _UpdateOnlineUser1(LPVOID pParam);//��ʱ�ӷ������鿴�û�������Ϣ
	static unsigned __stdcall _GetChatRoom(LPVOID pParam);//����û��������ұ��
	static unsigned __stdcall _Readman(LPVOID pParam);//Readman����
	static unsigned __stdcall _Push(LPVOID pParam);//����
	static unsigned __stdcall _FileCollect(LPVOID pParam);//��ʽ�ɼ�
	static unsigned __stdcall _SearchMail(LPVOID pParam);//��ʱ��ѯ�����������
	static unsigned __stdcall _GetBookFromSMS(LPVOID pParam);//ͨ���������������鼮
};

#endif