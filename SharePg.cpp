// SharePg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "SharePg.h"

#include "PathDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSharePg dialog
extern char CurrentPath[_MAX_PATH];


CSharePg::CSharePg(CWnd* pParent /*=NULL*/)
	: CDialog(CSharePg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSharePg)
	m_bsharefolder = FALSE;
	m_bshareformat = FALSE;
	//}}AFX_DATA_INIT
}


void CSharePg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSharePg)
	DDX_Control(pDX, IDC_TREE_SHAREFORMAT, m_shareFormat);
	DDX_Control(pDX, IDC_BUTTON_DELSHARE, m_delbtn);
	DDX_Control(pDX, IDC_BUTTON_ADDSHARE, m_addbtn);
	DDX_Control(pDX, IDC_LIST_SHAREFOLDER, m_sharefolder);
	DDX_Check(pDX, IDC_CHECK_SHAREFOLDER, m_bsharefolder);
	DDX_Check(pDX, IDC_CHECK_SHAREFORMAT, m_bshareformat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSharePg, CDialog)
	//{{AFX_MSG_MAP(CSharePg)
	ON_BN_CLICKED(IDC_BUTTON_ADDSHARE, OnButtonAddshare)
	ON_BN_CLICKED(IDC_CHECK_SHAREFOLDER, OnCheckSharefolder)
	ON_BN_CLICKED(IDC_CHECK_SHAREFORMAT, OnCheckShareformat)
	ON_LBN_SELCHANGE(IDC_LIST_SHAREFOLDER, OnSelchangeListSharefolder)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_DELSHARE, OnButtonDelshare)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSharePg message handlers
BOOL CSharePg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitialFormatTree();
	InitialState();
	StateChanged();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSharePg::OnButtonAddshare() 
{
	CPathDialog pathDlg("请选择需要共享的文件夹", "文件夹选择", "C:\\", this);
	if(pathDlg.DoModal()!=IDOK)
		return;
	CString path = pathDlg.GetPathName();
/*	if (path.Find("\\我的Readings文件夹") == -1)
	{
		path += "\\我的Readings文件夹";
	}*/
	_mkdir(path);
	m_sharefolder.InsertString(-1,path);
}

void CSharePg::OnCheckSharefolder() 
{
	UpdateData(TRUE);
	StateChanged();
}

void CSharePg::OnCheckShareformat() 
{
	UpdateData(TRUE);
	StateChanged();
}

void CSharePg::StateChanged()
{
	if(m_bsharefolder){
		m_addbtn.EnableWindow();
		m_sharefolder.EnableWindow();
		if(LB_ERR !=m_sharefolder.GetCurSel())
			m_delbtn.EnableWindow();
		else
			m_delbtn.EnableWindow(FALSE);
	}
	else{
		m_addbtn.EnableWindow(FALSE);
		m_delbtn.EnableWindow(FALSE);
		m_sharefolder.EnableWindow(FALSE);
	}
	if(m_bshareformat)
		m_shareFormat.EnableWindow();
	else
		m_shareFormat.EnableWindow(FALSE);
}

void CSharePg::InitialState()
{
	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);

	char val[50] = {0};
	GetPrivateProfileString("share", "sharefolder", NULL, val, 50, iniPath);
	m_bsharefolder = atoi(val);
	if(m_bsharefolder)
		LoadShareList();

	ZeroMemory(val, 50);
	GetPrivateProfileString("share", "shareformat", NULL, val, 50, iniPath);
	m_bshareformat = atoi(val);

	UpdateData(FALSE);
}

void CSharePg::OnSelchangeListSharefolder() 
{
	m_delbtn.EnableWindow();
}

void CSharePg::InitialFormatTree()
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_IMGLIST_TREE);
	CImageList imgList;
	imgList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 11, 1);
	imgList.Add(&bitmap, RGB(236,233,214));
	m_shareFormat.SetImageList(&imgList, TVSIL_NORMAL);
	bitmap.DeleteObject();
	imgList.Detach();

	HTREEITEM hRoot = m_shareFormat.InsertItem("本地资源", 0, 0);
	m_shareFormat.SetItemData(hRoot,0);
	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	CString title;
	title.Format("all-format");
	char val[10] = {0};
	GetPrivateProfileString("share",title,NULL,val,10,iniPath);
	if(atoi(val))
		m_shareFormat.SetCheck(hRoot);

	HTREEITEM hWOL = m_shareFormat.InsertItem("WOL", 4, 4, hRoot);
	SetItemDataAndCheck(hWOL,wolFileType);
		
	HTREEITEM hTXT = m_shareFormat.InsertItem("TXT", 4, 4, hRoot);
	SetItemDataAndCheck(hTXT, txtFileType);
	
	HTREEITEM hHTM = m_shareFormat.InsertItem("HTML", 4, 4, hRoot);
	SetItemDataAndCheck(hHTM, htmFileType);
	
	HTREEITEM hDOC = m_shareFormat.InsertItem("DOC", 4, 4, hRoot);
	SetItemDataAndCheck(hDOC, docFileType);
	
	HTREEITEM hPPT = m_shareFormat.InsertItem("PPT", 4, 4, hRoot);
	SetItemDataAndCheck(hPPT, pptFileType);
	
	HTREEITEM hPDF = m_shareFormat.InsertItem("PDF", 4, 4, hRoot);
	SetItemDataAndCheck(hPDF, pdfFileType);
	
	HTREEITEM hCHM = m_shareFormat.InsertItem("CHM", 4, 4, hRoot);
	SetItemDataAndCheck(hCHM, chmFileType);

	HTREEITEM hRAR = m_shareFormat.InsertItem("RAR\\ZIP", 4, 4, hRoot);
	SetItemDataAndCheck(hRAR, rarFileType);
	
	HTREEITEM hJPG = m_shareFormat.InsertItem("JPG", 4, 4, hRoot);
	SetItemDataAndCheck(hJPG, jpgFileType);

	HTREEITEM hBMP = m_shareFormat.InsertItem("BMP", 4, 4, hRoot);
	SetItemDataAndCheck(hBMP, bmpFileType);
	
	HTREEITEM hGIF = m_shareFormat.InsertItem("GIF", 4, 4, hRoot);
	SetItemDataAndCheck(hGIF, gifFileType);

	HTREEITEM hWAV = m_shareFormat.InsertItem("WAV", 4, 4, hRoot);
	SetItemDataAndCheck(hWAV, wavFileType);
		
	HTREEITEM hMP3 = m_shareFormat.InsertItem("MP3", 4, 4, hRoot);
	SetItemDataAndCheck(hMP3, mp3FileType);

	HTREEITEM hWMV = m_shareFormat.InsertItem("WMV\\WMA", 4, 4, hRoot);
	SetItemDataAndCheck(hWMV, wmvFileType);
	
	HTREEITEM hRM = m_shareFormat.InsertItem("RM\\RMVB", 4, 4, hRoot);
	SetItemDataAndCheck(hRM, rmFileType);

	HTREEITEM hAVI = m_shareFormat.InsertItem("AVI", 4, 4, hRoot);
	SetItemDataAndCheck(hAVI, aviFileType);

	HTREEITEM hASF = m_shareFormat.InsertItem("ASF", 4, 4, hRoot);
	SetItemDataAndCheck(hASF, asfFileType);

	HTREEITEM h3GP = m_shareFormat.InsertItem("3GP", 4, 4, hRoot);
	SetItemDataAndCheck(h3GP, _3gpFileType);

	m_shareFormat.Expand(hRoot,TVE_EXPAND);
}

void CSharePg::OnDestroy() 
{
	CImageList* il;
	il = m_shareFormat.GetImageList(TVSIL_NORMAL);
	if(il)
		il->DeleteImageList();

	CDialog::OnDestroy();
}

void CSharePg::SaveInfo()
{
	UpdateData(TRUE);

	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);

	char val[50] = {0};
	if(m_bsharefolder){
		strcpy_s(val, 50, "1");
		SaveShareFolder();
	}
	else
		strcpy_s(val, 50, "0");
	WritePrivateProfileString("share","sharefolder",val,iniPath);

	ZeroMemory(val, 50);
	if(m_bshareformat){
		strcpy_s(val, 50, "1");
		SaveShareFormat(iniPath);
	}
	else
		strcpy_s(val, 50, "0");
	WritePrivateProfileString("share","shareformat",val,iniPath);
}

BOOL CSharePg::SaveShareFolder()
{
	CString infoPath;
	infoPath.Format("%s\\system\\sharepath.inf", CurrentPath);
	CStdioFile cf;
	if(!cf.Open(infoPath, CFile::modeCreate|CFile::modeWrite))
		return FALSE;
	for(int nIndex=0; nIndex<m_sharefolder.GetCount(); nIndex++){
		CString buffer;
		m_sharefolder.GetText(nIndex, buffer);
		cf.WriteString(buffer);
		cf.WriteString("\n");
	}
	cf.Close();
	return TRUE;
}

void CSharePg::SaveShareFormat(LPCSTR iniPath)
{
	char yes[5] = {0};
	char no[5] = {0};
	strcpy_s(yes, 5, "1");
	strcpy_s(no, 5, "0");

	HTREEITEM hRoot = m_shareFormat.GetRootItem();
	WritePrivateProfileString("share", "all-format", 
		m_shareFormat.GetCheck(hRoot)?yes:no, iniPath);

	HTREEITEM hItem = m_shareFormat.GetChildItem(hRoot);
	BOOL ext = FALSE;
	while(hItem){
		DWORD data = m_shareFormat.GetItemData(hItem);
		CString title;
		title.Format("format-%d", data);
		WritePrivateProfileString("share", (LPCSTR)title, 
			m_shareFormat.GetCheck(hItem)?yes:no, iniPath);
		hItem = m_shareFormat.GetNextSiblingItem(hItem);
	}
}

void CSharePg::LoadShareList()
{
	CString infoPath;
	
	infoPath.Format("%s\\system\\sharepath.inf", CurrentPath);
	CStdioFile cf;
	if(cf.Open(infoPath,CFile::modeRead)){
		CString path;
		while(cf.ReadString(path)){
			m_sharefolder.AddString(path);
		}
		cf.Close();
	}
}

BOOL CSharePg::IsShareFormat(int filetype)
{
	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);
	CString title;
	title.Format("format-%d", filetype);
	char val[10] = {0};
	GetPrivateProfileString("share",title,NULL,val,10,iniPath);
	return atoi(val);
}

void CSharePg::SetItemDataAndCheck(HTREEITEM hItem, int filetype)
{
	m_shareFormat.SetItemData(hItem, filetype);
	m_shareFormat.SetCheck(hItem, IsShareFormat(filetype));
}

void CSharePg::OnButtonDelshare() 
{
	int nItem = m_sharefolder.GetCurSel();
	if(LB_ERR == nItem)
		return;
	m_sharefolder.DeleteString(nItem);
}
