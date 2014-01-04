// ReadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "ReadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReadDlg dialog
#define TOP_VIEW_MARGIN		10
#define LEFT_VIEW_MARGIN	10
#define SCROLL_RANGE_V		200
#define SCROLL_RANGE_H		50
#define BGCOLOR		RGB(150,150,150)


extern char CurrentPath[_MAX_PATH];

CReadDlg::CReadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bOpen = FALSE;

	curPage = 0;
	totalPage = 0;

	VScrollPos = 0;
	HScrollPos = 0;

	FileType = unknownFileType;

	sysWidth = WIDTH;
	sysHeight = HEIGHT;
	fontsize = FONT_SIZE_NORMAL;

	Zoom = show_normal;
}

CReadDlg::~CReadDlg()
{
	bmp.DeleteObject();
	bgbmp.DeleteObject();
	memDC.DeleteDC();
	bgDC.DeleteDC();
	font.DeleteObject();
}


void CReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReadDlg)
	DDX_Control(pDX, IDC_BUTTON_FC, m_fc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReadDlg, CDialog)
	//{{AFX_MSG_MAP(CReadDlg)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_MESSAGE(WM_PAGENUM_CHANGE, OnPageNumChange)
	ON_MESSAGE(WM_CURPAGE_CHANGE, OnCurPageNumChange)
	ON_MESSAGE(WM_UP_PAGE, OnUpPage)
	ON_MESSAGE(WM_DOWN_PAGE, OnDownPage)
	ON_MESSAGE(WM_ZOOM, OnZoom)
	ON_MESSAGE(WM_BUTTON_MENU, OnMenu)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReadDlg message handlers

BOOL CReadDlg::Open(LPCSTR FilePath)
{
	if(m_bOpen)
		Close();
	VScrollPos = 0;
	HScrollPos = 0;
	curPage = 0;
	totalPage = 0;

	FileType = GetFileTypeFromFull(FilePath);

	switch(FileType){
	case wolFileType:
		wolParser = new CParseWolFile;
		wolParser->SetShowStyle(Zoom);

		//begin add by dongzhenhua 20090524 显示加密的wol
		char Code[1025];
		strcpy_s(Code, 1025, "HanLin-eBook(Jinke)-20020913-No000000001-Public-AZBYCXDWEVFUGT?");
		Code[strlen("HanLin-eBook(Jinke)-20020913-No000000001-Public-AZBYCXDWEVFUGT?")]='\0';
		//AfxMessageBox(Code);
		if(IsEncrypt(FilePath,Code)==2)
		{
    		 char TmpPath[1024]={0};
			 sprintf_s(TmpPath, 1024, "%s\\temp\\tmp.wol", CurrentPath);
			 tmpFilePath=TmpPath;
			 CopyFile(FilePath,TmpPath,FALSE);
			 decryption(TmpPath, Code);
			 
			 tmpWol = TRUE;
			// AfxMessageBox(TmpPath);
			 m_bOpen = FALSE;
			 return m_bOpen;
		}
		//end add by dongzhenhua 20090524 显示加密的wol
		
		if(wolParser->BeginParseFile(FilePath)==0){
			totalPage = wolParser->Total;
			m_bOpen = TRUE;
		}
		else
		{
			m_bOpen = FALSE;
			return m_bOpen;
		}
		m_controlBar.m_page.Format("封面");
		m_controlBar.UpdateData(FALSE);
		if(!m_BookMenu.GetSafeHwnd()){
			m_BookMenu.Create(TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS,CRect::CRect(0,0,1,1),GetParent(),IDC_TREE_WOLMENU);
			wolParser->CreateCataTree(&m_BookMenu);
		}
		break;
	case txtFileType:
		txtParser = new CParseTxtFile;
		txtParser->SetShowStyle(Zoom);
		if(txtParser->BeginParseFile(FilePath)==0){
			txtParser->p = this;
			m_bOpen = TRUE;
		}
		break;
	case unknownFileType:
		m_bOpen = FALSE;
	default:
		break;
	}
	return m_bOpen;
}

void CReadDlg::Close()
{
	curPage = 0;
	totalPage = 0;
	switch(FileType){
	case wolFileType:
		wolParser->Release();
		delete wolParser;
		wolParser = NULL;
		break;
	case txtFileType:
		txtParser->Release();
		delete txtParser;
		txtParser = NULL;
		break;
	}
	if(m_BookMenu.GetSafeHwnd())
		m_BookMenu.DestroyWindow();
	m_bOpen = FALSE;
}

void CReadDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch (nSBCode){
		case SB_LINEDOWN://下行
			ScrollDown();
			break;
		case SB_BOTTOM://底部
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEUP://上行
			ScrollUp();
			break;
		case SB_PAGEDOWN://下页
			ScrollDown();
			break;
		case SB_PAGEUP://上页
			ScrollUp();
			break;
		case SB_THUMBPOSITION://拖动结束
			VScrollPos = nPos;
			Invalidate(FALSE);
			break;
		case SB_THUMBTRACK: //Drag scroll box to specified position
			break;
		case SB_TOP:
			break;
	}
}

void CReadDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch (nSBCode){
	case SB_LEFT:		//Scroll to far left.
		ScrollLeft();
		break;
	case SB_ENDSCROLL:	//   End scroll.
		break;
	case SB_LINELEFT:	//   Scroll left.
		ScrollLeft();
		break;
	case SB_LINERIGHT:	//   Scroll right.
		ScrollRight();
		break;
	case SB_PAGELEFT:	//   Scroll one page left.
		ScrollLeft();
		break;
	case SB_PAGERIGHT:	//   Scroll one page right.
		ScrollRight();
		break;
	case SB_RIGHT:	//   Scroll to far right.
		ScrollRight();
		break;
	case SB_THUMBPOSITION:	//   Scroll to absolute position. The current position is specified by the nPos parameter.
		HScrollPos = nPos;
		Invalidate(FALSE);
		break;
	case SB_THUMBTRACK:		//	Drag scroll box to specified position. The current position is specified by the nPos parameter
		break;
	}
}

void CReadDlg::ScrollLeft()
{
	int minRange, maxRange;
	GetScrollRange(SB_HORZ, &minRange, &maxRange);
	if(HScrollPos>minRange){
		HScrollPos--;
		Invalidate(FALSE);
		SetScrollPos(SB_HORZ, HScrollPos);
	}
}

void CReadDlg::ScrollRight()
{
	int minRange, maxRange;
	GetScrollRange(SB_HORZ, &minRange, &maxRange);
	SCROLLINFO hinfo = {0};
	GetScrollInfo(SB_HORZ, &hinfo, SIF_PAGE);

	if(HScrollPos+hinfo.nPage<=maxRange){
		HScrollPos++;
		Invalidate(FALSE);
		SetScrollPos(SB_HORZ, HScrollPos);
	}
}

void CReadDlg::SetScroll(int cx, int cy)
{
	int Range = (sysHeight+TOP_VIEW_MARGIN>cy)?((sysHeight+TOP_VIEW_MARGIN)/SCROLL_RANGE_V):2;
	SetScrollRange(SB_VERT, 0, Range-1);
	VScrollPos = 0;
	SCROLLINFO vinfo = {0};
	vinfo.cbSize = sizeof(SCROLLINFO);
	vinfo.fMask = SIF_PAGE;
	vinfo.nPage = (UINT)(cy/SCROLL_RANGE_V);
	SetScrollInfo(SB_VERT, &vinfo);

	Range = (sysWidth+LEFT_VIEW_MARGIN*2>cx)?((sysWidth+LEFT_VIEW_MARGIN*2)/SCROLL_RANGE_H+1):2;
	SetScrollRange(SB_HORZ, 0, Range-1);
	HScrollPos = 0;

	
	SCROLLINFO hinfo = {0};
	hinfo.cbSize = sizeof(SCROLLINFO);
	hinfo.fMask = SIF_PAGE;
	hinfo.nPage = (UINT)(cx/SCROLL_RANGE_H);
	SetScrollInfo(SB_HORZ, &hinfo);
}

void CReadDlg::ScrollDown()
{
	int minRange, maxRange;
	GetScrollRange(SB_VERT, &minRange, &maxRange);
	SCROLLINFO vinfo = {0};
	GetScrollInfo(SB_VERT, &vinfo, SIF_PAGE);

	if(VScrollPos+vinfo.nPage<=maxRange){
		VScrollPos++;
		Invalidate(FALSE);
		SetScrollPos(SB_VERT, VScrollPos);
	}
	else{
		DownPage();
	}
}

void CReadDlg::ScrollUp()
{
	int minRange, maxRange;
	GetScrollRange( SB_VERT, &minRange, &maxRange);
	if(VScrollPos>minRange){
		VScrollPos--;
		Invalidate(FALSE);
		SetScrollPos(SB_VERT, VScrollPos);
	}
	else{
		UpPage();
	}
}

void CReadDlg::ShowLogo()
{
	CBitmap tBmp;
	tBmp.LoadBitmap(IDB_BITMAP_LOGO);
	CDC tDC;
	tDC.CreateCompatibleDC(NULL);
	tDC.SelectObject(&tBmp);
	memDC.PatBlt(0, 0, sysWidth, sysHeight, WHITENESS);
	memDC.StretchBlt(LEFT_MARGIN,0,sysWidth-LEFT_MARGIN*2,sysHeight,&tDC,0,0,600,800,SRCCOPY);
	tBmp.DeleteObject();
	tDC.DeleteDC();
}

void CReadDlg::ShowPage(DWORD pn)
{
	if(!m_bOpen)
		return;

	VScrollPos = 0;
	HScrollPos = 0;
	SetScrollPos(SB_VERT, VScrollPos);
	SetScrollPos(SB_HORZ, HScrollPos);

	memDC.PatBlt(0, 0, sysWidth, sysHeight, WHITENESS);
	CPoint point(0,0);
	CRect rect(0,0,sysWidth,sysHeight);

	switch(FileType){
	case wolFileType:
		wolParser->SetShowStyle(Zoom);
		wolParser->ShowFile(point, rect, pn, &memDC);
		curPage = pn;
		break;
	case txtFileType:
		txtParser->SetShowStyle(Zoom);
		txtParser->ShowFile(&memDC,pn);
		curPage = pn;
		break;
	default:
		return;
	}
	PostMessage(WM_CURPAGE_CHANGE);

	Invalidate(FALSE);
}

BOOL CReadDlg::UpPage()
{
	if(curPage-1>=0)
		ShowPage(curPage-1);
	else
		return FALSE;

	return TRUE;
}

BOOL CReadDlg::DownPage()
{
	if(curPage+1<=totalPage)
		ShowPage(curPage+1);
	else
		return FALSE;
	return TRUE;
}


void CReadDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);
	bgbmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	bgDC.CreateCompatibleDC(NULL);
	bgDC.SelectObject(&bgbmp);
	bgDC.FillSolidRect(rect, BGCOLOR);

	int x;
	x = (rect.Width()-sysWidth)/2;

	if(x<LEFT_VIEW_MARGIN)
		x = LEFT_VIEW_MARGIN - HScrollPos*SCROLL_RANGE_H;
	
	int y = TOP_VIEW_MARGIN;
		
	int offset = VScrollPos*SCROLL_RANGE_V;

	bgDC.BitBlt(x,y-offset,sysWidth,sysHeight,&memDC,0,0,SRCCOPY);

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&bgDC,0,0,SRCCOPY);

	bgbmp.DeleteObject();
	bgDC.DeleteDC();

}

BOOL CReadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CPaintDC dc(this);
	bmp.CreateCompatibleBitmap(&dc, sysWidth, sysHeight);
	memDC.CreateCompatibleDC(NULL);
	memDC.SelectObject(&bmp);

    font.CreateFont(fontsize, // nHeight 
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
		_T("宋体")); 
	memDC.SelectObject(&font);
	ShowLogo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReadDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	SetScroll(cx, cy);	
}

int CReadDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_controlBar.Create(IDD_CONTROLBAR, GetParent()))
		return -1;
	
	return 0;
}

LRESULT CReadDlg::OnPageNumChange(WPARAM w,LPARAM l)
{
	BOOL initialing = (BOOL)w;
	DWORD total = (DWORD)l;
	if(initialing){
		totalPage = total;
		if(curPage==0)
			m_controlBar.m_page.Format("封面");
		else
			m_controlBar.m_page.Format("%d/%d", curPage, totalPage);

		if(curPage==totalPage)
			m_controlBar.m_right.EnableWindow(FALSE);
		else
			m_controlBar.m_right.EnableWindow(TRUE);

		m_controlBar.UpdateData(FALSE);
	}
	return TRUE;
}

LRESULT CReadDlg::OnCurPageNumChange(WPARAM w,LPARAM l)
{
	if(curPage==0){
		m_controlBar.m_page.Format("封面");
		m_controlBar.m_left.EnableWindow(FALSE);
	}
	else{
		m_controlBar.m_page.Format("%d/%d", curPage, totalPage);
		if(curPage == totalPage)
			m_controlBar.m_right.EnableWindow(FALSE);
		else
			m_controlBar.m_right.EnableWindow(TRUE);
		m_controlBar.m_left.EnableWindow(TRUE);
	}
	m_controlBar.UpdateData(FALSE);
	return TRUE;
}

LRESULT CReadDlg::OnUpPage(WPARAM w,LPARAM l)
{
	UpPage();	
	return TRUE;
}

LRESULT CReadDlg::OnDownPage(WPARAM w,LPARAM l)
{
	DownPage();
	return TRUE;
}

void CReadDlg::SetShowStyle(int style)
{
	if(style == show_large){
		Zoom = show_large;
		sysWidth = WIDTH_LARGE;
		sysHeight = HEIGHT_LARGE;
		fontsize = FONT_SIZE_LARGE;
	}
	else if(style == show_normal){
		Zoom = show_normal;
		sysWidth = WIDTH;
		sysHeight = HEIGHT;
		fontsize = FONT_SIZE_NORMAL;
	}
	else{
		Zoom = show_normal;
		sysWidth = WIDTH;
		sysHeight = HEIGHT;
		fontsize = FONT_SIZE_NORMAL;
	}

	CPaintDC dc(this);
	bmp.DeleteObject();
	bmp.CreateCompatibleBitmap(&dc, sysWidth, sysHeight);
	memDC.SelectObject(&bmp);
	
	CRect rect;
	GetClientRect(rect);
	SetScroll(rect.Width(), rect.Height());

	if(font.GetSafeHandle())
		font.DeleteObject();
	font.CreateFont(fontsize, // nHeight 
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
		_T("宋体")); 
	memDC.SelectObject(&font);

}

LRESULT CReadDlg::OnZoom(WPARAM w,LPARAM l)
{
	if(Zoom==show_normal)
		SetShowStyle(show_large);
	else
		SetShowStyle(show_normal);
	ShowPage(curPage);
	return TRUE;
}

BOOL CReadDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(!m_bOpen)
		return CDialog::PreTranslateMessage(pMsg);

	m_fc.SetFocus();

	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_LEFT )
			UpPage();
		else if(pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_SPACE)
			DownPage();
		else if(pMsg->wParam == VK_DOWN || pMsg->wParam == VK_NEXT)
			ScrollDown();
		else if(pMsg->wParam == VK_UP || pMsg->wParam == VK_PRIOR)
			ScrollUp();
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CReadDlg::OnMenu(WPARAM w,LPARAM l)
{
	if(m_BookMenu.GetSafeHwnd()){
		if(m_BookMenu.IsWindowVisible())
			m_BookMenu.ShowWindow(SW_HIDE);
		else
			m_BookMenu.ShowWindow(SW_SHOW);
	}
	return TRUE;
}

BOOL CReadDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	short temp = zDelta;
	VScrollPos = GetScrollPos(SB_VERT);
	int minRange, maxRange;
	GetScrollRange( SB_VERT, &minRange, &maxRange);

	if(temp>0){
		ScrollUp();
	}
	else{
		ScrollDown();
	}
	SetScrollPos(SB_VERT, VScrollPos);//m_Vpos的范围有在考虑
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

int CReadDlg::getStartPage(LPCSTR FilePath, int fileType)
{
	int page=0;
	if(fileType==wolFileType)
		page= wolParser->getStartPage(FilePath);
	return page;
}

int CReadDlg::IsEncrypt(const char* strWolfFile,char* strId)
{
		unsigned long dwIdLen=strlen(strId);
	if((dwIdLen==0)||(dwIdLen>=1024))
		return -1;

	unsigned short wDocumentLen;
	unsigned long dwCatalogLen;
	unsigned long dwCoverLen;
	unsigned long dwContentLen;

	FILE *fp;
	fp=fopen(strWolfFile,"r+b"); //r+b__w+b
	if(!fp)
		return -1;
	
	fseek(fp,23,0);
	fread(&wDocumentLen,sizeof(WORD),1,fp);

	fseek(fp,25,0);
	fread(&dwCoverLen,sizeof(DWORD),1,fp);
	
	fseek(fp,30,0);
	fread(&dwCatalogLen,sizeof(DWORD),1,fp);

	fseek(fp,38,0);
	fread(&dwContentLen,sizeof(DWORD),1,fp);

	//写入加密方式  第55位 写入0x51
	char cZgb[2];
	
	fseek(fp,55,0);
	fread(&cZgb,1,1,fp);
	cZgb[1]='\0';

	//该书未加密
	if (cZgb[0]==0x00)
	{
		fclose(fp);
		return 1;
	}
	
//add by dongzhenhua 20090524

	if (cZgb[0]==0x51)
	{
		fclose(fp);
		return 2;
	}
	//
// 	if(cZgb[0]!=0x00)
// //	if(cZgb[0]==0x00)
// 	{
// 		fclose(fp);
// ///		MessageBox(NULL,"书籍已加密，本次加密不成功！","加密失败",MB_OK);
// 		
 		return false;
//	}
/*
	//判断是否为终端绑定字异或加密方法
	if (cZgb[0]!=0x51)
	{
		cZgb[0]=0x51;
		cZgb[1]='\0';
	}
	else{
		cZgb[0]=0x00;
		cZgb[1]='\0';
	}
	fseek(fp,55,0);
	fwrite(&cZgb,1,1,fp);

	unsigned long n,m,p,q;
	unsigned long dwFirst=0;
	unsigned long dwTempNum=0;
	
	DWORD dwZgb=0;
	char cContent[4097]; //4*1024+1
	char cIn;
	p=dwContentLen/4096;
	q=dwContentLen%4096;
	
	char cVer[2];
	fseek(fp,11,0);
	fread(&cVer,1,1,fp);
	cVer[1]='\0';

	if(cVer[0]==0x31)
	{
		dwCatalogLen=0;
	}
	for(dwFirst=0;dwFirst<p;dwFirst++)
	{
		fseek(fp,(128+wDocumentLen+dwCatalogLen+dwCoverLen+dwFirst*4096),0);
		fread(cContent,4096,1,fp);
		cContent[4096]='\0';
		
		n=4096/dwIdLen;
		m=4096%dwIdLen;
		
		for(dwTempNum=0;dwTempNum<n;dwTempNum++)
		{	
			for(dwZgb=0;dwZgb<dwIdLen;dwZgb++)
			{
				cIn=cContent[dwTempNum*dwIdLen+dwZgb]^strId[dwZgb];
				cContent[dwTempNum*dwIdLen+dwZgb]=cIn;	
			}	
		}
		if(m!=0)
		{
			for(dwZgb=0;dwZgb<m;dwZgb++)
			{
				cIn=cContent[n*dwIdLen+dwZgb]^strId[dwZgb];
				cContent[n*dwIdLen+dwZgb]=cIn;	
			}	
		}
		
		cContent[4096]='\0';
		fseek(fp,(128+wDocumentLen+dwCatalogLen+dwCoverLen+dwFirst*4096),0);
		fwrite(cContent,4096,1,fp);
	}
	if(q!=0)
	{
		fseek(fp,(128+wDocumentLen+dwCatalogLen+dwCoverLen+p*4096),0);
		fread(cContent,q,1,fp);
		cContent[q]='\0';
		
		n=q/dwIdLen;
		m=q%dwIdLen;
		
		for(dwTempNum=0;dwTempNum<n;dwTempNum++)
		{	
			for(dwZgb=0;dwZgb<dwIdLen;dwZgb++)
			{
				cIn=cContent[dwTempNum*dwIdLen+dwZgb]^strId[dwZgb];	
				cContent[dwTempNum*dwIdLen+dwZgb]=cIn;
			}	
		}
		if(m!=0)
		{
			for(dwZgb=0;dwZgb<m;dwZgb++)
			{
				cIn=cContent[n*dwIdLen+dwZgb]^strId[dwZgb];
				cContent[n*dwIdLen+dwZgb]=cIn;	
			}	
		}
		
		cContent[q]='\0';
		fseek(fp,(128+wDocumentLen+dwCatalogLen+dwCoverLen+p*4096),0);
		fwrite(cContent,q,1,fp);
	}

	fclose(fp);
	return 2;
*/
}

int CReadDlg::decryption(char *strWolfFile, char *strId)
{
	unsigned long dwIdLen=strlen(strId);
	if((dwIdLen==0)||(dwIdLen>=1024))
		return -1;

	unsigned short wDocumentLen;
	unsigned long dwCatalogLen;
	unsigned long dwCoverLen;
	unsigned long dwContentLen;

	FILE *fp;
	fp=fopen(strWolfFile,"r+b"); //r+b__w+b
	if(!fp)
		return -1;
	
	fseek(fp,23,0);
	fread(&wDocumentLen,sizeof(WORD),1,fp);

	fseek(fp,25,0);
	fread(&dwCoverLen,sizeof(DWORD),1,fp);
	
	fseek(fp,30,0);
	fread(&dwCatalogLen,sizeof(DWORD),1,fp);

	fseek(fp,38,0);
	fread(&dwContentLen,sizeof(DWORD),1,fp);

	//写入加密方式  第55位 写入0x51
	char cZgb[2];
	
	fseek(fp,55,0);
	fread(&cZgb,1,1,fp);
	cZgb[1]='\0';

	//该书未加密
	if (cZgb[0]==0x00)
	{
		fclose(fp);
		return 1;
	}
	
//add by dongzhenhua 20090524

	//
// 	if(cZgb[0]!=0x00)
// //	if(cZgb[0]==0x00)
// 	{
// 		fclose(fp);
// ///		MessageBox(NULL,"书籍已加密，本次加密不成功！","加密失败",MB_OK);
// 		
// 		return false;
//	}

	//判断是否为终端绑定字异或加密方法
	if (cZgb[0]!=0x51)
	{
		cZgb[0]=0x51;
		cZgb[1]='\0';
	}
	else{
		cZgb[0]=0x00;
		cZgb[1]='\0';
	}
	fseek(fp,55,0);
	fwrite(&cZgb,1,1,fp);

	unsigned long n,m,p,q;
	unsigned long dwFirst=0;
	unsigned long dwTempNum=0;
	
	DWORD dwZgb=0;
	char cContent[4097]; //4*1024+1
	char cIn;
	p=dwContentLen/4096;
	q=dwContentLen%4096;
	
	char cVer[2];
	fseek(fp,11,0);
	fread(&cVer,1,1,fp);
	cVer[1]='\0';

	if(cVer[0]==0x31)
	{
		dwCatalogLen=0;
	}
	for(dwFirst=0;dwFirst<p;dwFirst++)
	{
		fseek(fp,(128+wDocumentLen+dwCatalogLen+dwCoverLen+dwFirst*4096),0);
		fread(cContent,4096,1,fp);
		cContent[4096]='\0';
		
		n=4096/dwIdLen;
		m=4096%dwIdLen;
		
		for(dwTempNum=0;dwTempNum<n;dwTempNum++)
		{	
			for(dwZgb=0;dwZgb<dwIdLen;dwZgb++)
			{
				cIn=cContent[dwTempNum*dwIdLen+dwZgb]^strId[dwZgb];
				cContent[dwTempNum*dwIdLen+dwZgb]=cIn;	
			}	
		}
		if(m!=0)
		{
			for(dwZgb=0;dwZgb<m;dwZgb++)
			{
				cIn=cContent[n*dwIdLen+dwZgb]^strId[dwZgb];
				cContent[n*dwIdLen+dwZgb]=cIn;	
			}	
		}
		
		cContent[4096]='\0';
		fseek(fp,(128+wDocumentLen+dwCatalogLen+dwCoverLen+dwFirst*4096),0);
		fwrite(cContent,4096,1,fp);
	}
	if(q!=0)
	{
		fseek(fp,(128+wDocumentLen+dwCatalogLen+dwCoverLen+p*4096),0);
		fread(cContent,q,1,fp);
		cContent[q]='\0';
		
		n=q/dwIdLen;
		m=q%dwIdLen;
		
		for(dwTempNum=0;dwTempNum<n;dwTempNum++)
		{	
			for(dwZgb=0;dwZgb<dwIdLen;dwZgb++)
			{
				cIn=cContent[dwTempNum*dwIdLen+dwZgb]^strId[dwZgb];	
				cContent[dwTempNum*dwIdLen+dwZgb]=cIn;
			}	
		}
		if(m!=0)
		{
			for(dwZgb=0;dwZgb<m;dwZgb++)
			{
				cIn=cContent[n*dwIdLen+dwZgb]^strId[dwZgb];
				cContent[n*dwIdLen+dwZgb]=cIn;	
			}	
		}
		
		cContent[q]='\0';
		fseek(fp,(128+wDocumentLen+dwCatalogLen+dwCoverLen+p*4096),0);
		fwrite(cContent,q,1,fp);
	}

	fclose(fp);
	return 2;

}
