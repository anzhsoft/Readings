// MainBar.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "MainBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainBar dialog
extern CMainFrame* pMainWnd;
extern bool gbOnSearching;// Added by zhanganzhan on 20081024

CMainBar::CMainBar()
{
	//{{AFX_DATA_INIT(CMainBar)
	m_keyword = _T("");
	//}}AFX_DATA_INIT
	m_bSearch = FALSE;
	m_bopen = FALSE;
}

void CMainBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainBar)
	DDX_Control(pDX, IDC_BUTTON_MAIL, m_mail);
	DDX_Control(pDX, IDC_BUTTON_OPEN_CLOSE, m_open_close);
	DDX_Control(pDX, IDC_BUTTON_USERADV, m_userAdv);
	DDX_Control(pDX, IDC_BUTTON_HISTORY, m_history);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_help);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_search);
	DDX_Control(pDX, IDC_BUTTON_SEARCH_STR, m_searchstr);
	DDX_Control(pDX, IDC_BUTTON_ORDERLIST, m_orderlist);
	DDX_Control(pDX, IDC_BUTTON_SYN, m_syn);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_refresh);
	DDX_Text(pDX, IDC_EDIT_SEARCH_KEY, m_keyword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainBar, CDialogBar)
	//{{AFX_MSG_MAP(CMainBar)
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_REFRESH, OnUpdateRefresh)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ORDERLIST, OnUpdateOrderList)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SYN, OnUpdateSyn)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HISTORY, OnUpdateHistory)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_USERADV, OnUpdateUserAdv)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_HELP, OnUpdateHelp)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_SEARCH, OnUpdateSearch)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_OPEN_CLOSE, OnUpdateOpenClose)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_MAIL,OnUpdateMail)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_MESSAGE(WM_SEARCH_FINISHED, OnSearchFinished)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CLOSE, OnButtonOpenClose)
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
	ON_BN_CLICKED(IDC_BUTTON_MAIL, OnButtonMail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainBar message handlers
LRESULT CMainBar::OnInitDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = HandleInitDialog(wParam, lParam);

	UpdateData(FALSE);

	m_search.SetButtonStyle(BS_DEFPUSHBUTTON|BS_OWNERDRAW);

	if(m_refresh.LoadBitmaps(IDB_MAINBAR_REFRESH,IDB_MAINBAR_REFRESH_CHK,IDB_MAINBAR_REFRESH_SEL,IDB_MAINBAR_REFRESH_DIS)){
		m_refresh.AutoSize();
		m_refresh.SetFocusCursor(IDC_CURSOR);
	}

	if(m_syn.LoadBitmaps(IDB_MAINBAR_SYN,IDB_MAINBAR_SYN_CHK,IDB_MAINBAR_SYN_SEL,IDB_MAINBAR_SYN_DIS)){
		m_syn.AutoSize();
		m_syn.SetFocusCursor(IDC_CURSOR);
	}
	
	if(m_orderlist.LoadBitmaps(IDB_MAINBAR_ORDER,IDB_MAINBAR_ORDER_CHK,IDB_MAINBAR_ORDER_SEL,IDB_MAINBAR_ORDER_DIS)){
		m_orderlist.AutoSize();
		m_orderlist.SetFocusCursor(IDC_CURSOR);
	}

	if(m_searchstr.LoadBitmaps(IDB_MAINBAR_RESEARCH_STR,IDB_MAINBAR_RESEARCH_STR,IDB_MAINBAR_RESEARCH_STR,IDB_MAINBAR_RESEARCH_STR)){
		m_searchstr.AutoSize();
	}
	
	if(m_search.LoadBitmaps(IDB_MAINBAR_SEARCH,IDB_MAINBAR_SEARCH_CHK,IDB_MAINBAR_SEARCH_SEL,IDB_MAINBAR_SEARCH_DIS)){
		m_search.AutoSize();
		m_search.SetFocusCursor(IDC_CURSOR);
	}

	if(m_history.LoadBitmaps(IDB_MAINBAR_HISTORY,IDB_MAINBAR_HISTORY_CHK,IDB_MAINBAR_HISTORY_SEL,IDB_MAINBAR_HISTORY_DIS)){
		m_history.AutoSize();
		m_history.SetFocusCursor(IDC_CURSOR);
	}

	if(m_userAdv.LoadBitmaps(IDB_MAINBAR_USERADV,IDB_MAINBAR_USERADV_CHK,IDB_MAINBAR_USERADV_SEL,IDB_MAINBAR_USERADV_DIS)){
		m_userAdv.AutoSize();
		m_userAdv.SetFocusCursor(IDC_CURSOR);
	}

	if(m_help.LoadBitmaps(IDB_MAINBAR_HELP,IDB_MAINBAR_HELP_CHK,IDB_MAINBAR_HELP_SEL,IDB_MAINBAR_HELP_DIS)){
		m_help.AutoSize();
		m_help.SetFocusCursor(IDC_CURSOR);
	}

	
	if (m_open_close.LoadBitmaps(IDB_MAINBAR_CLOSE,IDB_MAINBAR_CLOSE_CHK,
			IDB_MAINBAR_CLOSE_SEL,IDB_MAINBAR_CLOSE_DIS))
	{
		m_open_close.AutoSize();
		m_open_close.SetFocusCursor(IDC_CURSOR);
	}
		return bRet;
}

BOOL CMainBar::OnEraseBkgnd(CDC* pDC) 
{
	//考虑到系统默认背景不同,才更改这个值
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect,RGB(236, 233, 216));
	return TRUE;
}

void CMainBar::OnUpdateRefresh(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainBar::OnUpdateOrderList(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainBar::OnUpdateSyn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainBar::OnUpdateHistory(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainBar::OnUpdateUserAdv(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainBar::OnUpdateHelp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainBar::OnUpdateSearch(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainBar::OnUpdateOpenClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CMainBar::OnUpdateMail(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
void CMainBar::OnButtonSearch() 
{
	//if(!gbOnSearching){
// 		if (gbOnSearching == TRUE)
// 		{
// 			MessageBox("正在搜索，请稍候...");
// 			return;
// 		}
// 		gbOnSearching = TRUE;
		UpdateData(TRUE);
		if(m_keyword.IsEmpty())
			return;
		
// 		m_search.LoadBitmaps(IDB_BITMAP_BUTTON_STOPSEARCH,IDB_BITMAP_BUTTON_STOPSEARCH_CHK,
// 			IDB_BITMAP_BUTTON_STOPSEARCH_SEL,IDB_BITMAP_BUTTON_STOPSEARCH_DIS);
// 		m_search.Invalidate(FALSE);
		
		char* keyword = (char*)malloc(m_keyword.GetLength()+1);
		ZeroMemory(keyword, m_keyword.GetLength()+1);
		strcpy_s(keyword, m_keyword.GetLength()+1, m_keyword);
		m_bSearch = TRUE;
		//changed by dongzhenhua 080529
		pMainWnd->PostMessage(WM_SEARCH, (WPARAM)keyword, NULL);
	//}
	//else
	//	pMainWnd->StopSearch();
}

LRESULT CMainBar::OnSearchFinished(WPARAM w,LPARAM l)
{
	m_search.LoadBitmaps(IDB_MAINBAR_SEARCH,IDB_MAINBAR_SEARCH_CHK,
		IDB_MAINBAR_SEARCH_SEL,IDB_MAINBAR_SEARCH_DIS);
	m_search.Invalidate(FALSE);
	m_bSearch = FALSE;
    gbOnSearching = FALSE;
	
	return TRUE;
}

void CMainBar::Search(CString key)
{
	//Added by zhanganzhan on 20081022
    /*if(m_bSearch == TRUE)
    {
		UpdateData(FALSE);
		MessageBox("正在进行搜索，请稍候...");	
		return ;
    }*/
	m_keyword = key;
	UpdateData(FALSE);
	//pMainWnd->StopSearch();
	OnButtonSearch();
}

void CMainBar::OnButtonOpenClose() 
{
	// TODO: Add your control notification handler code here
	if (m_bopen==FALSE)
	{
	//改变图标
   	   if (m_open_close.LoadBitmaps(IDB_MAINBAR_OPEN,IDB_MAINBAR_OPEN_CHK,
	       IDB_MAINBAR_OPEN_SEL,IDB_MAINBAR_CLOSE))//IDB_MAINBAR_OPEN_DIS))
	   {
	    	m_open_close.AutoSize();
		    m_open_close.SetFocusCursor(IDC_CURSOR);
			m_open_close.Invalidate(FALSE);
	
	   }
		//多窗口变单窗口操作		
		pMainWnd->PostMessage(WM_OPENWINDOW);
		m_bopen = TRUE;
	}
	else
	{
	//改变图标
    	if (m_open_close.LoadBitmaps(IDB_MAINBAR_CLOSE,IDB_MAINBAR_CLOSE_CHK,
	    	IDB_MAINBAR_CLOSE_SEL,IDB_MAINBAR_CLOSE_DIS))
		{
	    	m_open_close.AutoSize();
	    	m_open_close.SetFocusCursor(IDC_CURSOR);
				m_open_close.Invalidate(FALSE);
		}
	//单窗口变多窗口操作
		pMainWnd->PostMessage(WM_CLOSEWINDOW);
		m_bopen = FALSE;
	}
}

void CMainBar::OnButtonMail() 
{
	// TODO: Add your control notification handler code here
	CRECVMAILDIALOG* recvmaildialog = new CRECVMAILDIALOG();
	recvmaildialog->Create(IDD_RECVMAILDIALOG);
	recvmaildialog->ShowWindow(SW_RESTORE);

}
