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
 * Readings 心跳机制实现类，在MainFrame中初始化
 * 集成一些定时器和程序启动时就启动的一些线程
******************************/

class CReadingsHeart
{
public://公共变量
	/*心跳相关*/
	long BreakTime;//心跳次数
	long interval;//心跳间隔,以秒为单位
	//CThreadList threadList;//活动线程列表,在pMainWnd中进行统一线程管理

	/*短信搜索相关*/
	BOOL IsSMSThreadFinished;//用于判断上次轮询是否结束
	CList<CString,CString&>* SMSDownloadList;//通过短信搜索正在下载的书籍的ID列表

	/*邮件轮询相关*/
	HANDLE hEvent;//用于暂停开始邮件轮询线程
	string MyEMailPassWord;
	string MyEMailUserName;
	string MyEMailServer;
	string PopServer;
	string SmtpServer;
	string publicemailaddr;//公共邮箱地址
	string publicemailpass;//公共邮箱密码
	int MailSearchCount;//用于判断是否是第一次轮询
	CString MailStat;//邮件轮询当前所处状态
	BOOL IsMailSearchDone;//用于判断上次轮询是否结束
	CList<CString,CString&>* HasDownLoadMailAtts;
	CList<CRequestMail,CRequestMail&>* UnReadedRequestMailList;
	CList<CAgreeMail,CAgreeMail&>* UnReadedAgreeMailList;
	CList<CChargeMail,CChargeMail&>* UnReadedChargeMailList;
	CList<CRefuseMail,CRefuseMail&>* UnReadedRefuseMailList;
	CList<CRequestMail,CRequestMail&>* ReadedRequestMailList;
	CList<CAgreeMail,CAgreeMail&>* ReadedAgreeMailList;
	CList<CChargeMail,CChargeMail&>* ReadedChargeMailList;
	CList<CRefuseMail,CRefuseMail&>* ReadedRefuseMailList;

	/*聊天相关*/

	/*上传共享文件信息相关*/

public://公共函数	
	/*心跳相关*/
	BOOL Break();//心跳一次
	BOOL Start(long time);//心脏开始活动
	CReadingsHeart();
	virtual ~CReadingsHeart();

	/*短信搜索相关*/
	BOOL AddToPushList(DownLoadMissionInfo* missionInfo);//加入到推送箱

	/*邮件轮询相关*/
	void ReflushMail();//邮件刷新
	bool GetEMailInfo();//获取邮箱信息

	/*聊天相关*/

	/*上传共享文件信息相关*/
	

public://开启的轮询线程
	void RefreshUser();//刷新在线用户信息
	static unsigned __stdcall _UpdateFriends(LPVOID pParam);//定时从服务器查看用户在线信息
	static unsigned __stdcall _UpdateOnlineUser2(LPVOID pParam);//定时从服务器查看用户在线信息
	static unsigned __stdcall _UpdateOnlineUser1(LPVOID pParam);//定时从服务器查看用户在线信息
	static unsigned __stdcall _GetChatRoom(LPVOID pParam);//获得用户的聊天室编号
	static unsigned __stdcall _Readman(LPVOID pParam);//Readman新闻
	static unsigned __stdcall _Push(LPVOID pParam);//推送
	static unsigned __stdcall _FileCollect(LPVOID pParam);//格式采集
	static unsigned __stdcall _SearchMail(LPVOID pParam);//定时轮询本人邮箱程序
	static unsigned __stdcall _GetBookFromSMS(LPVOID pParam);//通过短信搜索下载书籍
};

#endif