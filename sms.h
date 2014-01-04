#include "mainlistview.h"
/*struct SMSInfo{
	CString UserName;
	CString Content;
	CMainListView* mainListView;
};*/
struct SMSInfo{
	CString CurrentPath;
	CString AspServer;
	CWnd* pMainWnd;
	CString UserName;
	CString Content;
	CMainListView* mainListView;
};
unsigned __stdcall _SendRequestSMS(LPVOID pParam);
int ReadingsSMS(CString UserName,CString MyName,CString Resource,CString AspServer,CMainListView* mainListView);