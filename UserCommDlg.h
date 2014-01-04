#if !defined(AFX_USERCOMMDLG_H__3961EF42_EC7D_40AA_A6F9_271F5C5C5E23__INCLUDED_)
#define AFX_USERCOMMDLG_H__3961EF42_EC7D_40AA_A6F9_271F5C5C5E23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserCommDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserCommDlg dialog
#include "NameTabCtrl.h"
//#include "UserListCtrl.h"
#include "TreeCtrlEx.h"
#include "CommLog.h"	// Added by ClassView

class CUserCommDlg : public CDialog
{
// Construction
public:
	static unsigned __stdcall _GetMyStudyHall(LPVOID pParam);
	static unsigned __stdcall _GetMyStudyBuilding(LPVOID pParam);
	static unsigned __stdcall _GetMyUniversity(LPVOID pParam);
	static unsigned __stdcall _GetMessage(LPVOID pParam);//added by zhanganzhan for web-chatroom on 20090310
	void GetMyStudy(HTREEITEM hItem);
	static unsigned __stdcall _GetStudyHall(LPVOID pParam);
	void GetStudyHall(HTREEITEM hItem);
	static unsigned __stdcall _GetBuild(LPVOID pParam);
	void GetBuilding(HTREEITEM hItem);
	
	void GetSchool(HTREEITEM hItem);
	int reConnectServer();
	
	long MessageCount;
	int MessageFlag;
	char lastMessage[BUFFER_SIZE];
	int reconnectTimes;
	BOOL reConnectionFlag;//added by zhanganzhan to flag the re-connection.20090305
	CMsgList m_msgList;
	CWndList m_wndList;


	HANDLE hGetMsg;


	static BOOL CALLBACK EnumWinProc(HWND hwnd, LPARAM lParam);
	HWND CreateTalkWnd();
	struct EnumParam   
	{   
		HWND hMainWnd;   
		DWORD dwProcessID;   
	};   

	void ChangeChatRoom(char *buf);//zhanganzhan 090814
	void Dang();
	int RecordMsg(char * package);
	void ReadMsg(char* buf);
	void GetSubGroup(HTREEITEM hItem);
	void PubDB(int filetype, LPCSTR iniPath, BOOL all);
	void PubInfo();
	BOOL InitialTree();
	int DipHoleForComm(LPCSTR UN);
	void holeTest(char *);//zhanganzhan for p2p 20081204
	CString GetSavePath(CString filename);
	void MessageToMainWnd(UINT msg, LPCSTR buffer);
	CUserCommDlg(CWnd* pParent = NULL);   // standard constructor

	static unsigned __stdcall _SendXML(LPVOID pParam);//added by zhanganzhan on 20081118
	static unsigned __stdcall _Initial(LPVOID pParam);
	static unsigned __stdcall _Listen(LPVOID pParam);
	static unsigned __stdcall _ReConnectionInitial(LPVOID pParam);

	static unsigned __stdcall _GetSubGroup(LPVOID pParam);
	static unsigned __stdcall _GetSchool(LPVOID pParam);
	
	void Listen();
	BOOL InitSevConn();

	SOCKET serverSock;
	SOCKET heartSock;
	SOCKET fileSock;//added by zhanganzhan on 20081111

	CSocketList m_slist;
	CThreadList m_thlist;
	CMutex m_mutex;

	BOOL quit;
	int closeflag;

	CWinThreadList threadList;

	CWinThread* curTh;

	HTREEITEM hFriend;
	HTREEITEM hGroup;
	HTREEITEM hAllGroup;
	// add by dzh 20081215
	HTREEITEM hChatroom;//added by zhanganzhan on 20090325
	HTREEITEM hMyFriend;
	
	HTREEITEM hStudyhall;
	HTREEITEM hMyStudyhall;
	
	HTREEITEM hit_item;


	BOOL m_bShowList;

	CHARFORMAT cf_name;
	CHARFORMAT cf_msg;
	CHARFORMAT cf_flag;//zhanganzhan on 090814
	PARAFORMAT pf;
	CFont font;
// Dialog Data
	//{{AFX_DATA(CUserCommDlg)
	enum { IDD = IDD_DIALOG_COMMDLG };
	CButton	m_msgRecord;
	CRichEditCtrl	m_ctrRichR;
	CTreeCtrlEx	m_tree;
	CEdit	m_ctreSend;
	CButton	m_ctrSend;
	CButton	m_ctrClose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserCommDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUserCommDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonSend();
	afx_msg LRESULT OnRequestResource(WPARAM reserve, long sid);
	afx_msg LRESULT OnDownLoadFile(WPARAM w,LPARAM l);
	afx_msg LRESULT OnError(WPARAM w,LPARAM l);
	afx_msg LRESULT OnAddtoMyhall(WPARAM w,LPARAM l);
	afx_msg void OnButtonClose();
	afx_msg void OnSelchangedUserTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnThreadFinished(WPARAM w,LPARAM l);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnCreateTalkDlg(WPARAM w,LPARAM l);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonMsgrecord();
	afx_msg void OnClickUserTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickUserTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnChangeChatroom1(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnChangeChatroom2(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnChangeChatroom(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnChangeUser(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERCOMMDLG_H__3961EF42_EC7D_40AA_A6F9_271F5C5C5E23__INCLUDED_)
