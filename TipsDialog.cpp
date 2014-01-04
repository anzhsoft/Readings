// TipsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "TipsDialog.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame* pMainWnd;
extern char CurrentPath[_MAX_PATH];
extern char UserName[50];
/////////////////////////////////////////////////////////////////////////////
// CTipsDialog dialog


CTipsDialog::CTipsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTipsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTipsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CTipsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTipsDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTipsDialog, CDialog)
	//{{AFX_MSG_MAP(CTipsDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_TIPDLG_BUTTON_OK, OnClickOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipsDialog message handlers

void CTipsDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SetBkMode(TRANSPARENT);   
	dc.SelectObject(&font);

	if(style==1){
		dc.TextOut(sz.cy-20,sz.cx-160,"下次不再提示");
	}
	
	int	len = m_tip.GetLength();
	if(m_tip.GetLength()<24){
		CRect rect(56, 20, sz.cx-5, sz.cy-30);
		dc.DrawText(m_tip, &rect, DT_CENTER|DT_SINGLELINE);
	}
	else{	
		CRect rect(56, 6, sz.cx-5, sz.cy-5);
		dc.DrawText(m_tip, &rect, DT_LEFT|DT_TOP|DT_WORDBREAK);
	}
}

BOOL CTipsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	sz.cx = 210;
	sz.cy = 70;

	font.CreateFont(12, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		FW_NORMAL, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		GB2312_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体")); // lpszFac			Monotype Corsiva
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
    HINSTANCE hInst = LoadLibrary("User32.DLL");     
    if(hInst){
		typedef BOOL(WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);                       
		MYFUNC fun = NULL;              
        fun = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)
			fun(GetSafeHwnd(),0,220,2);             
        FreeLibrary(hInst);     
    }

	int width = GetSystemMetrics(SM_CXSCREEN);   
	int height = GetSystemMetrics(SM_CYSCREEN); 
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}
	SetWindowPos(&(pMainWnd->wndTop), width-sz.cx-10, height-sz.cy-30,
		sz.cx, sz.cy, SWP_NOACTIVATE);

	CRect rbtn(sz.cx-70,sz.cy-25,sz.cx-20,sz.cy-5);
	btn.Create("",BS_OWNERDRAW,rbtn,this,IDC_TIPDLG_BUTTON_OK);
	btn.LoadBitmaps(IDB_TIPDLG_BUTTON,4,1,1,1,4);
	if (pMainWnd->m_quit)
	{
		return FALSE;
	}
	CRect cbtn(sz.cx-175, sz.cy-20, sz.cx-161, sz.cy-6);
	chk.Create("",BS_AUTOCHECKBOX,cbtn,this,IDC_TIPDLG_CHECK);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTipsDialog::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==1){
		KillTimer(1);
		ShowWindow(SW_HIDE);
	}
	CDialog::OnTimer(nIDEvent);
}

void CTipsDialog::OnDestroy() 
{
	CDialog::OnDestroy();
}

BOOL CTipsDialog::OnEraseBkgnd(CDC* pDC)
{
	CDC memDC;
	CBitmap cBitmap;
	CBitmap* pOldMemBmp = NULL;

	cBitmap.LoadBitmap(bg);
	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	pDC->BitBlt(0, 0, 210, 70, &memDC, 0, 0, SRCCOPY);

	if(pOldMemBmp)
		memDC.SelectObject(pOldMemBmp);
	if(memDC.GetSafeHdc())
		memDC.DeleteDC();

	return TRUE;
}

void CTipsDialog::OnClickOk()
{
	char Path[_MAX_PATH] = {0};
	char val[10] = {0};
	sprintf_s(Path, _MAX_PATH, "%s\\system\\option.ini", CurrentPath);
	if(chk.GetCheck())
		WritePrivateProfileString(UserName, key, "1", Path);
	else
		WritePrivateProfileString(UserName, key, "0", Path);
	ShowWindow(SW_HIDE);
}
