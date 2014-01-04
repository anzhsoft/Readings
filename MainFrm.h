// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C82E54C8_1796_4C83_B3F5_F1557F084FD9__INCLUDED_)
#define AFX_MAINFRM_H__C82E54C8_1796_4C83_B3F5_F1557F084FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainBar.h"
#include "GuidTreeView.h"
#include "HotKeyListView.h"
#include "HotKeyView.h"
#include "MainListView.h"
//#include "ReadingsView.h"
#include "ShowView.h"
#include "CommView.h"
#include "UserCommDlg.h"
#include "DownLoadListView.h"
#include "TipsDialog.h"

#include "SplitWnd.h"
#include "RowSplitWnd.h"

#include "WM_Show_Dialog.h"
#include "ResourceClassify.h"
#include "CreateReGroup.h"
#include "MidRowSplitWnd.h"
#include "CommLog.h"	// Added by ClassView
#include "CommLogDlg.h"	// Added by ClassView
#include "AddNetBookDlg.h"
#include "AddUniversity.h"
#include "DlgAddStudyBuilding.h"
#include "RECVMAILDIALOG.h"
#include "REQUESTMAILDIALOG.h"
#include "RequestMail.h"
#include "AgreeMail.h"
#include "RefuseMail.h"
#include "ChargeMail.h"
#include "PersonInfo.h"
#include "ReadingsHeart.h"	// Added by ClassView
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	long curCondition;
	CString currentkeyword;
	BOOL m_quit;
	CMutex m_mutex;
	CSocketList m_socketList;
	CThreadList m_threadList;
	CMainBar m_MainBar;

	HTREEITEM m_hItem;
	TreeItemInfo* m_info;
	BOOL isfreashing;//标记知识树是否正在刷新。zuoss20090108
	

//	CGuidTreeView* m_pGuidTreeView;
	COrderCenterView* m_OrderCenterView;
	CReadOnLine* m_ReadOnLineView;
	CManBooks* m_ManBooksView;
	CDownloadCenterTreeView* m_DownloadCenterView;
	CHotKeyTree* m_HotKeyView;
	CPersonInfo* m_PersonInfoView;
	CTreeCtrl* m_pHotKeyTree;
	CTreeCtrl* m_pOrderCenterTree;
	CTreeCtrl* m_pReadOnLineTree;
	CTreeCtrl* m_pManBooksTree;
	CTreeCtrl* m_pDownloadCenterTree;
	CTreeCtrl* m_pSelectedTree;
// Operations
public:
	CMainFrame();
	void AddIcon(int IconID);
	void DeleteIcon(int IconID);
	void ShowHide();
	void ShowMenu();
	void ConnectNetFriend(TreeItemInfo* info, CString FriendName);
	UINT OnLineUserRegister();
	void ShowCollectYearFile(TreeItemInfo* info);
	void ShowFolder(HTREEITEM hItem, CString root);
	CString RecordCourse(int treetype, TreeItemInfo* info);
	void RefreshBookList(HTREEITEM hItem);//zuoss 090514
	void SaveBookTypeFile(LPCTSTR ResName, LPCTSTR DesName);//zuoss 090514

	CUserCommDlg* m_pCommDlg;
	CDigHotkey* m_pHotDlg;
	CDrawerView* m_Drawers;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CReadingsHeart* Heart;//心跳机制
	void ResumeDownload();
//	CTreeCtrl* m_pTree;
	
	void DeleteFromDownLoadListView(DownLoadMissionInfo* info);
	BOOL AddToReadings(CString filepath);
	
	
	CRECVMAILDIALOG* RecvDlg;
	void ReflushMail();
	
	void DownLoadTools();
	CCommLogDlg m_commLogDlg;
	CCommLog m_commLog;
	CString compareIp(CString ipHost); // add by yangxiao 2008.8.12
	long curID;
	void BackupSDCard();
	BOOL ExistFile(int flag, int id);
	void MessageAlert();
	void ModifyIcon(int IconID);
	void Operation(HTREEITEM hItem, TreeItemInfo* info);
	void GetUserInfo();
	void InsertSDDisk();
	void ReMoveSDDisk();
	void ShowReadman(TreeItemInfo* info);
	void ShowOrderList();
	void ShowTip(UINT icon, CString tip, DWORD dwseconds);
	void ShowResourceShareFiletype(TreeItemInfo* info);
	void ShowNetBookResource(TreeItemInfo* info);//added by zuoss 090526
	void ShowiBook(int typeID,int channelID);//istation类别下的书目显示 zuoss 090618

//	BOOL PushReadman();
//	BOOL DownLoadPushList();
//	BOOL PushInfoToDB();
	void ShowOrderPaper(TreeItemInfo* info, HTREEITEM hItem);
	void StopSearch();
	static unsigned __stdcall _GetReady(LPVOID pParam);
	static unsigned __stdcall CMainFrame::_ShowDownInfo(LPVOID pParam);
	static unsigned __stdcall _OnLineUserRegister(LPVOID pParam);
	static unsigned __stdcall _GetUserInfo(LPVOID pParam);
	//add by dongzhenhua 20080604 定时清理缓存文件，生成新缓存
	//static unsigned __stdcall _ConductCache(LPVOID pParam);
	//add by dzh 20081118 定时轮询本人邮箱程序
	//static unsigned __stdcall _SearchMail(LPVOID pParam);
	//add by dzh 20081224 记录用户操作行为
	static unsigned __stdcall _RecordUserBehavior(LPVOID behav);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CReBar m_ReBar;
	
//	CIEView* m_pIEView;
	CMainListView* m_pMainListView;
	CDownLoadListView* m_pDownLoadListView;
//	CGuidTreeView* m_pGuidTreeView;
	CCommView* m_pCommView;
	CShowView* m_pShowView;

	CListCtrl* m_pMainList;

	
	CSplitWnd m_colWndSplitter;
	CRowSplitWnd m_v0WndSplitter;
//	CRowSplitWnd m_v1WndSplitter;
	CRowSplitWnd m_v2WndSplitter;
	CMidRowSplitWnd m_v1WndSplitter;
	
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnIcon(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnSearch(WPARAM w,LPARAM l);
	afx_msg LRESULT OnCacheSearch(WPARAM w,LPARAM l);
	afx_msg LRESULT OnSearchFinished(WPARAM w,LPARAM l);
	afx_msg LRESULT OnTreeItemSelected(WPARAM w,LPARAM l);
	afx_msg LRESULT OnUserTreeClicked(WPARAM w,LPARAM l);
	afx_msg LRESULT OnAppendUserInfo(WPARAM w,LPARAM l);
	afx_msg LRESULT OnDeleteUserInfo(WPARAM w,LPARAM l);
	afx_msg LRESULT OnFileDownload(WPARAM w,LPARAM l);
	afx_msg LRESULT OnNewDownload(WPARAM w,LPARAM l);
	afx_msg LRESULT OnUpdateDownload(WPARAM w,LPARAM l);
	afx_msg void OnUpdateHideUI(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowUI(CCmdUI* pCmdUI);
	afx_msg void OnShowUI();
	afx_msg void OnHideUI();
	afx_msg void OnUpdateHelp(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnAppExit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OpenFile(WPARAM w,LPARAM l);
	afx_msg LRESULT OpenURL(WPARAM w,LPARAM l=0);
	afx_msg LRESULT OpenURLNotShow(WPARAM w,LPARAM l);
	afx_msg LRESULT OpenTagURL(WPARAM w,LPARAM l);
	afx_msg LRESULT OnRequestResource(WPARAM reserve, long sid);
	afx_msg LRESULT OnAddOrderItem(WPARAM w,LPARAM l);
	afx_msg LRESULT OnThreadFinished(WPARAM w,LPARAM l);
	afx_msg void OnHelp();
	afx_msg void OnButtonUseradv();
	afx_msg void OnButtonHistory();
	afx_msg void OnButtonOrderList();
	afx_msg void OnButtonRefresh();
	afx_msg void OnSyn();
	afx_msg LRESULT OnSynToSD(WPARAM w,LPARAM l);
	afx_msg void OnMenuitemList();
	afx_msg void OnMenuitemReport();
	afx_msg void OnUpdateMenuitemList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuitemReport(CCmdUI* pCmdUI);
	afx_msg LRESULT OnAppendGroupInfo(WPARAM w,LPARAM l);
	afx_msg LRESULT OnShowGroupResource(WPARAM reserve, LONG gid);
	afx_msg LRESULT OnShowUserResource(WPARAM w,LPARAM l);
	afx_msg void OnMenuitemConfig();
	afx_msg LRESULT	OnCreateNewFavorFolder(WPARAM w,LPARAM l);
	afx_msg LRESULT OpenWindow(WPARAM w,LPARAM l);
	afx_msg LRESULT CloseWindow(WPARAM w,LPARAM l);
	afx_msg void OnMenuitemBackup();
	afx_msg LRESULT UpdateGuidTreeCount(WPARAM w,LPARAM l);
	afx_msg void OnMenuitemCommlog();
	afx_msg void OnMenuitemCommanal();
	afx_msg LRESULT RecordBehavior(WPARAM w,LPARAM l);
	afx_msg void OnToolsReadwolf();
	afx_msg void OnToolsMakewolf();
	afx_msg void OnMenuitemModify();
	afx_msg void OnMenuitemScore();
	afx_msg LRESULT OnRefreshUser(WPARAM wParam,LPARAM lParam);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CTipsDialog m_tipsDlg;
	BOOL InitialToolBar();
	BOOL SaveOrderType(LPCSTR IbxFileName, LPCSTR BinFileName);
	unsigned _int64 CMainFrame::GetDiskFree(char *disk);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C82E54C8_1796_4C83_B3F5_F1557F084FD9__INCLUDED_)
