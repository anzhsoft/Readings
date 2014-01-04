#if !defined(AFX_MAINLISTVIEW_H__1E1DF670_AE23_4B73_A9AD_FBCEC7341CF9__INCLUDED_)
#define AFX_MAINLISTVIEW_H__1E1DF670_AE23_4B73_A9AD_FBCEC7341CF9__INCLUDED_

#include "CommLog.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainListView view

class CMainListView : public CListView
{
protected:
	CMainListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMainListView)
private:
	int hit_index;
// Attributes
public:
	char* keyWord;
	CString searchkey;
	BOOL isShareFlag; //����Ƿ��ѵ������
	int sharePage;//�������������ǰҳ
	int curcondition;//��ǰҳ��״̬��1���������� 2����Ϣ���� 3��չʾ���� 4���ۿ۷��� 5���������
	BOOL basc;
	ORDERSTYLE orderstyle;
	int style;
	int showtype;
	CListCtrl* m_plist; //�����������������������������������

	CMutex socketlistmutex;
	CMutex mutex;

	CRect up1, down1;	//��ҳ
	CRect up2, down2;	//��ҳ
	CRect jumpEditRect1,jumpEditRect2;
	CRect mianze;	//����
/*********	
	CRect dwfl;		//�������
	CRect xxfw;		//��Ϣ����
	CRect zsfw;		//չʾ����
	CRect zkfw;		//�ۿ۷���
	CRect djhd;		//�������
**********/

//added by zuoss 2008.07.29
	CRect kjfl;		//��ݷ���
	CRect duzl;		//��������
	CRect wjsx;		//�ļ�����
	CRect yjxx;		//�м���Ϣ
	CRect djhd;		//�������
//end
	CRect jiaocai;	//�̲�
	CRect kejian;	//�μ�
	CRect xiti;		//ϰ��
	CRect kaoshi;	//����

	CRect shijian;	//ʱ��
	CRect daxiao;	//��С
	CRect gongxiangshu;		//������
	CRect wendingdu;		//�ȶ���

	CRect zuixin;		//����
	CRect zuikuai;		//���
	CRect zuijurenqi;	//�������

	CRect zuidazhekou;		//����ۿ�
	CRect zuidijiage;		//��ͼ۸�
	CRect dijiajuan;		//�ּ۾�
	CRect zengpin;			//��Ʒ

	CRect renqi;			//����
	CRect tuijian;			//�Ƽ�
	CRect jingdianhuigu;	//����ع�

	CRect zzqbh;	//����Ȩ����
	BOOL quit;
	CRect clkRect[SEARCHSOURCES];
	int status;
	DWORD page;
	DWORD page1;
	DWORD totalPage;
	DWORD totalPage1;
	DWORD totalCount;
	DWORD startTime,endTime;

	int forward;//added by zuoss 20081029

	DWORD m_filetype;
	int m_year;
//	CPageEdit* pEdit;
	CImageList imgList;
	CMap<int,int&,CRect,CRect&> btn1Map;
	CMap<int,int&,CRect,CRect&> btn2Map;

//zuoss 090217 ���Ľ���ͷ����Ӧ��
	CRect orderAll;
	CRect unOrderAll;
	CRect reverseAll;

	//add by dongzhenhua 0509	
	CMap<int,int&,CRect,CRect&> btn3Map;
	CMap<int,int&,CRect,CRect&> btn4Map;
	//add by linshan 0610	
	CMap<int,int&,CRect,CRect&> star1Map;
	CMap<int,int&,CRect,CRect&> star2Map;
	CMap<int,int&,CRect,CRect&> star3Map;
	CMap<int,int&,CRect,CRect&> star4Map;
	CMap<int,int&,CRect,CRect&> star5Map;
	//add by dongzhenhua 0613
	CMap<int,int&,CRect,CRect&> headprocess[SEARCHSOURCES];


	CMap<int,int&,FindIconListInfo,FindIconListInfo&> coverMap;

	CThreadList threadList;
	CSocketList socketList;

	CMenu m_menu;
	//istation�ڵ��channel id��type id
	int ichannelid;
	int itypeid;
	BOOL IS_istation;
// static
public:
	static unsigned __stdcall _AddToOrderCenter(LPVOID pParam);

	static unsigned __stdcall  _SearchNet(LPVOID pParam);
	static unsigned __stdcall  _SearchLocal(LPVOID pParam);
	static unsigned __stdcall _LoadSearchIcon(LPVOID pParam);

	static unsigned __stdcall _Select(LPVOID pParam);
	static unsigned __stdcall _DisSelect(LPVOID pParam);

	static unsigned __stdcall _OrderRss(LPVOID pParam);
	static unsigned __stdcall _SendRequestMail(LPVOID pParam);
	static unsigned __stdcall _SendRequestSMS(LPVOID pParam);

// Operations
public:
	static string tran_16bases(unsigned char * deviceid,int deviceidlen);
	static string rtran_16bases(unsigned char * input,int inputLen);

	CCommLog m_searchLog;
	void CurToHand(CPoint point);
	CString CurSearchKeywords;
	DWORD ShowAllResult(char* path,int pagenum);
	BOOL FormResultList(CString title,CString cover,CString link,CString abst1,CString abst2,CString abst3,int info_id);
	void SetRectCurEmpty();
	HCURSOR m_hand;
	int finishedSearchCount;
	void UpdateSearchCount(int flag, int id, int total);
	long headflag;
	DWORD ShowOneClass(char* path);
	DWORD ShowOneResource(int num);
	DWORD SeachCacheNet(ThreadInfo *info);
	static unsigned __stdcall _SearchCacheNet(LPVOID pParam);
	void SearchCache(int flag, int id,CString keyword="",int page=1);//20080711
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void ChangeToReport();
	void ChangeToList();
	void DownLoadOrder(OrderDetailInfo* info);
	void ClickFileList(int nItem, DiskBookInfo *info, CPoint point);
	void ClickOrderList(int nItem, OrderDetailInfo *info, CPoint point);

	void ClickOrderHead(int nItem, OrderDetailInfo *info, CPoint point);//zuoss090217����ͷ��ѡ��
	void ClickOneOrder(int nItem, OrderDetailInfo *info);//zuoss090218�����Ĵ���

	void ClickFindList(int nItem, FINDINFO* info, CPoint point);
	void ClickShareList(int nItem, ShareInfo* info, CPoint point);
	DWORD SearchNet(ThreadInfo *info);
	DWORD SearchLocal(char *keyword);

	void StopThread();
	CString CheckSrc(int src);

	void DrawOrderItemText(CDC* pDC, CRect rect, OrderDetailInfo* info, int nItem);
	void DrawFileItemText(CDC* pDC, CRect rect, DiskBookInfo* info, int nItem);
	void DrawFindResultText(CDC* pDC, CRect rect, FINDINFO* info, int nItem);
	void DrawShareItemText(CDC* pDC, CRect rect, ShareInfo* info, int nItem);
	
	void DrawFileItemIcon(CDC* pDC, CRect rect, int nIndex);
	void DrawFindResultIcon(CDC* pDC, CRect rect, FINDINFO* info);
	
	void DrawShareItem(CDC *pDC, CRect rect, ShareInfo* info, int nItem);
	void DrawOrderItem(CDC *pDC, CRect rect, OrderDetailInfo* info, int nItem);
	void DrawFileItem(CDC *pDC, CRect rect, DiskBookInfo* info, int nItem);
	void DrawFindResult(CDC *pDC, CRect rect, FINDINFO* info, int nItem);
	
	void DrawFindListHead(CDC* pDC, CRect rect, FINDINFO* info);
	void DrawOrderListHead(CDC* pDC, CRect rect, OrderDetailInfo* info);

	void DrawMovePageClick(CDC* pDC, CRect rect, int src);

	void ShowOrderListItem(NMLVCUSTOMDRAW* pLVCD);
	void ShowFindListItem(NMLVCUSTOMDRAW* pLVCD);
	void ShowFileListItem(NMLVCUSTOMDRAW *pLVCD);
	void ShowShareListItem(NMLVCUSTOMDRAW *pLVCD);
	void ShowNetBookListItem(NMLVCUSTOMDRAW *pLVCD);

	//added by zuoss 2090515 ��ʾistation������
	void ShowHanlinBookItem(NMLVCUSTOMDRAW* pLVCD);
    void DrawHanlinBookHead(CDC* pDC, CRect rect, FINDINFO* info);
	void DrawHanlinBookResult(CDC* pDC, CRect rect, FINDINFO*info, int nItem);
	void DrawHanlinBookResultText(CDC * pDC, CRect rect, FINDINFO *info, int nItem);
	//end 
	void ShowCollectYearFile(DWORD filetype, int year, DWORD currentPage);
	void ShowFolder(CString Path, int type);
	void ShowOrderItem(CString Path);
	void ShowPushFolder();
	void ShowRecentlyRead();
	void ShowResourceShareFiletype(CString FilePath);

	//added by zuoss 090526 ��������б���ʾ
	void ShowNetBookResource(CString FilePath);
	void ClickNetBookList(int nItem, FINDINFO* info, CPoint point);
	//end
	void ShowiBook(CString FilePath);//��ʾistation��Ŀ

	void SetType(int type);
	LRESULT Search(LPSTR keyword);
	void ShowNextSharePage();//��ʾ��һҳ������Ϣ
	void RefreshHanlinBook(TreeItemInfo* info,TreeItemInfo* Pinfo, CString FileName);//zuoss 090515 ˢ�º���ͼ����ʾ��

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainListView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnTaskFinished(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnThreadFinished(WPARAM w,LPARAM l);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuitemRead();
	afx_msg void OnMenuitemMove();
	afx_msg void OnMenuitemCopy();
	afx_msg void OnMenuitemDelete();
	afx_msg void OnMenuitemOpenfolder();
	afx_msg void OnMenuitemSyn();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuitemDownload();
	afx_msg void OnMenuitemOrder();
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOrderIsactive();
	afx_msg void OnOrderTitle();
	afx_msg void OnOrderFiletype();
	afx_msg void OnOrderDowntime();
	afx_msg void OnOrderCreatedate();
	afx_msg void OnOrderShareuser();
	afx_msg void OnMenuitemSize();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINLISTVIEW_H__1E1DF670_AE23_4B73_A9AD_FBCEC7341CF9__INCLUDED_)
