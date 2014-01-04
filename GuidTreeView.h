#if !defined(AFX_GUIDTREEVIEW_H__023B433A_664F_408C_A600_11E3A81E8AF4__INCLUDED_)
#define AFX_GUIDTREEVIEW_H__023B433A_664F_408C_A600_11E3A81E8AF4__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuidTreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuidTreeView view

class CGuidTreeView : public CTreeView
{
protected:
	           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGuidTreeView)
	CMenu m_menu;
	HTREEITEM hit_Item;
//zuoss 090320 左树节点变换颜色
protected:
	struct Color_Font
	{
		COLORREF color;
		LOGFONT  logfont;
	};

	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont;
//end

// Attributes
public:
	CGuidTreeView();
	CMutex m_mutex;
	CThreadList threadList;
	CSocketList socketList;
	BOOL quit;
	CTreeCtrl* m_pTree;

	HTREEITEM hMyFavor;
	HTREEITEM m_MyBookcaseItem;
	HTREEITEM hNetLibrary;
	HTREEITEM hpaperItem;
	HTREEITEM hmagItem;
	HTREEITEM hNetResource;
	HTREEITEM hOrderItem;
	HTREEITEM collectItemRead;
	HTREEITEM collectItemPic;
	HTREEITEM collectItemMeida;
	HTREEITEM hFriend;
	HTREEITEM hSDDisk;
	HTREEITEM hAllGroup;
	HTREEITEM hMyGroup;

	HTREEITEM m_firstChannelItem;
	HTREEITEM m_curChannelItem;
// Operations
public:

	void InsertDownload(HTREEITEM hItem);
	void InsertResourceShare(HTREEITEM hItem);
	void InsertBookstore(HTREEITEM hItem);
	void InsertSearchHistory(HTREEITEM hItem);
	void InsertDownloadCenter(HTREEITEM hItem);
	void InsertOrderCenter(HTREEITEM hItem);
	void InsertCollect(HTREEITEM hItem);
	void InsertMyFavor(HTREEITEM hItem);
	void InsertMyBookCase(HTREEITEM hItem);
	void InsertReadman(HTREEITEM hItem);
	void SetTreeItemData(HTREEITEM hItem, BYTE type, char *ID, WORD Pages=0, DWORD Numbers=0, CString KeyWords="",CString url="");
	BOOL InsertLocalFolder(HTREEITEM hItem);
	virtual BOOL RefreshTree();
	BOOL InitialImageList();
	BOOL ChannelIsNew(char *FileName);
	void SaveChannelInfo(char *FileName, char *ChannelFileName);
	void SaveOrderItem(char *FileName,char *destPath);

	HTREEITEM InsertChannelItem(HTREEITEM hRoot, char *FileName);

	static void CountFile(LPCSTR Path, WIN32_FIND_DATA* pData, DWORD& data);
	static LPVOID InsertFolder(LPCSTR Path, WIN32_FIND_DATA* pData,
							 LPVOID pfParam, DWORD& subdata);
	static LPVOID InsertSDFolder(LPCSTR Path, WIN32_FIND_DATA* pData,
							 LPVOID pfParam, DWORD& subdata);
	static void SetFolderStatics(LPCSTR Path, WIN32_FIND_DATA* pData,
						   LPVOID pParam, DWORD subdata, DWORD& data);
	
	void FreeTreeItem(HTREEITEM item);
	void DeleXmlFiles();//added by zuoss 20081027
	
	void SetItemFont(HTREEITEM hItem, LOGFONT& logfont);
	void SetItemBold(HTREEITEM hItem, BOOL bBold);
	void SetItemColor(HTREEITEM hItem, COLORREF color);
	BOOL GetItemFont(HTREEITEM hItem, LOGFONT * plogfont);
	BOOL GetItemBold(HTREEITEM hItem);
	COLORREF GetItemColor(HTREEITEM hItem);
//thread

public:
	void InsertRStore();
	static HBITMAP ImageLoad(CString szFile);
	void InsertFanShuHotBook();
	void InsertHotKey();
	int type;
	void UpdateTreeName(CString newname, int flag, int id);
	
	int GetFileNum(CString Path);
	void UpdateSearchCount(int flag, int id, int total);
	
	void GetMyGrouplevel(HTREEITEM hItem);
	void GetMyNetBook(HTREEITEM hItem);
	void GetMySubNetBook(HTREEITEM hItem);
	void GetSubPublishCom(HTREEITEM hItem);
	void GetMyReGroup(HTREEITEM hItem);
	void InsertMyResourceGroup();
	void InsertResourceGroup();
	void InsertNetBook();//zuoss 090312
	void InsertPublishCom();//zuoss 090410
	void InsertFanshuBook();//zuoss 090910
	void InsertRemoveableDisk();
	void InsertSDFolderItem(HTREEITEM hItem);
	void ShowTree();
	void InsertFriendList(HTREEITEM hItem);
	void GetNetStatics();
	void InsertOrderItem(char* FilePath);
	void GetOrderInfo();
	void InsertNetLibrary(char* FileName, HTREEITEM hItem);
	void GetNetLibraryInfo();
	static unsigned __stdcall _GetNetLibraryInfo(LPVOID pParam);
	static unsigned __stdcall _GetNetInfo(LPVOID pParam);
	static unsigned __stdcall _GetOrderInfo(LPVOID pParam);
	static unsigned __stdcall _GetLocalInfo(LPVOID pParam);
	static unsigned __stdcall _ShowTree(LPVOID pParam);
	static unsigned __stdcall _GetShareInfo(LPVOID pParam);
	static unsigned __stdcall _GetShareInfoTotal(LPVOID pParam);
	static unsigned __stdcall _GetShareInfoRoot(LPVOID pParam);

	void AddLeaf(HTREEITEM hItem);
	void GetSubGroup(HTREEITEM hItem);
	void GetSubNetBook(HTREEITEM hItem);//zuoss 090312 网络原创读物
	void GetMySub(HTREEITEM hItem);	//zuoss 090105
	void GetSubFanshuBook(HTREEITEM hItem); //zuoss 090910 番薯网读物
	
	static unsigned __stdcall _GetSubGroup(LPVOID pParam);
	static unsigned __stdcall _GetSubNetBook(LPVOID pParam);//zuoss 090312
	static unsigned __stdcall _GetMySubNetBook(LPVOID pParam);
	static unsigned __stdcall _GetSubPublishCom(LPVOID pParam);
	static unsigned __stdcall _GetSubFanshuBook(LPVOID pParam);//zuoss 090910番薯网

	static unsigned __stdcall _GetMySub(LPVOID pParam);//zuoss 090105
	static unsigned __stdcall _GetGroupLeaf(LPVOID pParam);
	static unsigned __stdcall _GetMyGroup(LPVOID pParam);
	static unsigned __stdcall _GetMyGroupLevel(LPVOID pParam);

	static unsigned __stdcall _GetMyNetBook(LPVOID pParam);
	static unsigned __stdcall _GetHotKey1(LPVOID pParam);//获取最新热词
	static unsigned __stdcall _GetHotKey2(LPVOID pParam);//获取最热热词
	static unsigned __stdcall _GetHotKey3(LPVOID pParam);//获取推荐热词
	static unsigned __stdcall _GetFanShuHotBook(LPVOID pParam);
	//begin istation图书类别节点 zuoss 090618
	void GetSubBookType(HTREEITEM hItem);
	static unsigned __stdcall _GetSubBookType(LPVOID pParam);
	//end

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuidTreeView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CGuidTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGuidTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnThreadFinished(WPARAM w,LPARAM l);
	afx_msg void OnDestroy();
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuitemOpenfolder();
	afx_msg void OnMenuitemAddfolder();
	afx_msg void OnMenuitemDelfolder();
	afx_msg LRESULT	OnCreateNewFavorFolder(WPARAM w,LPARAM l);
	afx_msg LRESULT RecordBehavior(LPSTR beha);
	afx_msg void OnMenuCreateRegroup();
	afx_msg void OnMenuCreateLevelgroup();
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuKeywords();
	afx_msg void OnMenuTomyfavor();
	afx_msg void OnMenuitemBackup();
	afx_msg void OnMenuShowtag();
	afx_msg void OnMenuAddnetbook();
	afx_msg void OnMenuSetnetbook();
	afx_msg void OnMenuDelnetbook();
	afx_msg void OnMenuMoveup();
	afx_msg void OnMenuitemRemove();
	afx_msg void OnMenuMakebook();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIDTREEVIEW_H__023B433A_664F_408C_A600_11E3A81E8AF4__INCLUDED_)
