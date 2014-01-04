// FavorPg.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "FavorPg.h"

#include "PathDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char FavorPath[_MAX_PATH];
extern char CurrentPath[_MAX_PATH];
/////////////////////////////////////////////////////////////////////////////
// CFavorPg dialog

CFavorPg::CFavorPg(CWnd* pParent /*=NULL*/)
	: CDialog(CFavorPg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFavorPg)
	m_favorFolder = _T("");
	m_allday = 3;
	m_papday = 3;
	//}}AFX_DATA_INIT
}


void CFavorPg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFavorPg)
	DDX_Control(pDX, IDC_LIST_COLLECT, m_collectList);
	DDX_Text(pDX, IDC_EDIT_FAVORFOLDER, m_favorFolder);
	DDX_Text(pDX, IDC_EDIT_ALLDAY, m_allday);
	DDV_MinMaxUInt(pDX, m_allday, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_PAPDAY, m_papday);
	DDV_MinMaxUInt(pDX, m_papday, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFavorPg, CDialog)
	//{{AFX_MSG_MAP(CFavorPg)
	ON_BN_CLICKED(IDC_BUTTON_FAVORFOLDER_BROWSE, OnButtonFavorfolderBrowse)
	ON_BN_CLICKED(IDC_RADIO_NEVER, OnRadioNever)
	ON_BN_CLICKED(IDC_RADIO_PDF_AUTO, OnRadioPdfAuto)
	ON_BN_CLICKED(IDC_RADIO_ALL_AUTO, OnRadioAllAuto)
	ON_BN_CLICKED(IDC_RADIO_ALLDISK, OnRadioAlldisk)
	ON_BN_CLICKED(IDC_RADIO_SPECDISK, OnRadioSpecdisk)
	ON_BN_CLICKED(IDC_BUTTON_ADD_COLLECTFOLDER, OnButtonAddCollectfolder)
	ON_LBN_SELCHANGE(IDC_LIST_COLLECT, OnSelchangeListCollect)
	ON_BN_CLICKED(IDC_BUTTON_DEL_COLLECTFOLDER, OnButtonDelCollectfolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFavorPg message handlers

BOOL CFavorPg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitialState();
	ChangeState();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFavorPg::InitialState()
{
	m_favorFolder = FavorPath;

	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH,  "%s\\system\\config.ini", CurrentPath);

	char val[50] = {0};
	GetPrivateProfileString("favor", "clear-style", NULL, val, 50, iniPath);
	clear_style = atoi(val);
	if(clear_style==clear_pdf){
		((CButton*)GetDlgItem(IDC_RADIO_PDF_AUTO))->SetCheck(1);
		ZeroMemory(val,50);
		GetPrivateProfileString("favor", "clear-paper-days", NULL, val, 50, iniPath);
		m_papday = atoi(val);
	}
	else if(clear_style==clear_all){
		((CButton*)GetDlgItem(IDC_RADIO_ALL_AUTO))->SetCheck(1);
		ZeroMemory(val,50);
		GetPrivateProfileString("favor", "clear-all-days", NULL, val, 50, iniPath);
		m_allday = atoi(val);
	}
	else{
		((CButton*)GetDlgItem(IDC_RADIO_NEVER))->SetCheck(1);
		clear_style = not_clear;
	}
	
	ZeroMemory(val,50);
	GetPrivateProfileString("favor", "collect-style", NULL, val, 50, iniPath);
	collect_style = atoi(val);
	if(collect_spec == collect_style){
		CString path;
		path.Format("%s\\system\\colPath.inf", CurrentPath);
		CStdioFile cf;
		if(cf.Open(path, CFile::modeRead)){
			CString buffer;
			while(cf.ReadString(buffer))
				m_collectList.AddString(buffer);
			cf.Close();
		}
		((CButton*)GetDlgItem(IDC_RADIO_SPECDISK))->SetCheck(1);
	}
	else{
		((CButton*)GetDlgItem(IDC_RADIO_ALLDISK))->SetCheck(1);
		collect_style = collect_all;
	}
	UpdateData(FALSE);
}

void CFavorPg::ChangeState()
{
	if(clear_style==clear_pdf){
		GetDlgItem(IDC_STATIC_TIPS1)->EnableWindow();
		GetDlgItem(IDC_STATIC_TIPS2)->EnableWindow();
		GetDlgItem(IDC_EDIT_PAPDAY)->EnableWindow();
		GetDlgItem(IDC_SPIN_PAP)->EnableWindow();
		GetDlgItem(IDC_STATIC_TIPS3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIPS4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ALLDAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_ALL)->EnableWindow(FALSE);
	}
	else if(clear_style==clear_all){
		GetDlgItem(IDC_STATIC_TIPS1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIPS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PAPDAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_PAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIPS3)->EnableWindow();
		GetDlgItem(IDC_STATIC_TIPS4)->EnableWindow();
		GetDlgItem(IDC_EDIT_ALLDAY)->EnableWindow();
		GetDlgItem(IDC_SPIN_ALL)->EnableWindow();
	}
	else{
		GetDlgItem(IDC_STATIC_TIPS1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIPS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PAPDAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_PAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIPS3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIPS4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ALLDAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_ALL)->EnableWindow(FALSE);
		clear_style = not_clear;
	}

	if(collect_style == collect_all){
		GetDlgItem(IDC_BUTTON_ADD_COLLECTFOLDER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEL_COLLECTFOLDER)->EnableWindow(FALSE);
		m_collectList.EnableWindow(FALSE);
	}
	else{
		GetDlgItem(IDC_BUTTON_ADD_COLLECTFOLDER)->EnableWindow();
		if(m_collectList.GetCount()>0)
			GetDlgItem(IDC_BUTTON_DEL_COLLECTFOLDER)->EnableWindow();
		else
			GetDlgItem(IDC_BUTTON_DEL_COLLECTFOLDER)->EnableWindow(FALSE);
		m_collectList.EnableWindow();
		collect_style = collect_spec;
	}
}

void CFavorPg::OnButtonFavorfolderBrowse() 
{
	CPathDialog dlg("请选择默认收藏夹的位置","文件夹选择",m_favorFolder, this);
	if(dlg.DoModal()!=IDOK)
		return;
	m_favorFolder = dlg.GetPathName();
	UpdateData(FALSE);
}

void CFavorPg::OnRadioNever() 
{
	UpdateData(TRUE);
	clear_style = not_clear;
	ChangeState();
}

void CFavorPg::OnRadioPdfAuto() 
{
	UpdateData(TRUE);
	clear_style = clear_pdf;
	ChangeState();
}

void CFavorPg::OnRadioAllAuto() 
{
	UpdateData(TRUE);
	clear_style = clear_all;
	ChangeState();
}

void CFavorPg::OnRadioAlldisk() 
{
	UpdateData(TRUE);
	collect_style = collect_all;
	ChangeState();
}

void CFavorPg::OnRadioSpecdisk() 
{
	UpdateData(TRUE);
	collect_style = collect_spec;
	ChangeState();
}

void CFavorPg::OnButtonAddCollectfolder() 
{
	CPathDialog dlg("请选择默认收藏夹的位置","文件夹选择",NULL, this);
	if(dlg.DoModal()!=IDOK)
		return;
	m_collectList.AddString(dlg.GetPathName());
}

void CFavorPg::OnSelchangeListCollect() 
{
	GetDlgItem(IDC_BUTTON_DEL_COLLECTFOLDER)->EnableWindow();
}

void CFavorPg::OnButtonDelCollectfolder() 
{
	int nIndex = m_collectList.GetCurSel();
	if(nIndex==LB_ERR)
		return;
	m_collectList.DeleteString(nIndex);
	if(m_collectList.GetCount()<=0)
		GetDlgItem(IDC_BUTTON_DEL_COLLECTFOLDER)->EnableWindow(FALSE);
}

void CFavorPg::SaveInfo()
{
	UpdateData(TRUE);

	char iniPath[_MAX_PATH] = {0};
	sprintf_s(iniPath, _MAX_PATH, "%s\\system\\config.ini", CurrentPath);

	WritePrivateProfileString("favor", "favorFolder", (LPCSTR)m_favorFolder,iniPath);

	char val[10] = {0};
	sprintf_s(val, 10, "%d", clear_style);
	WritePrivateProfileString("favor", "clear-style", val,iniPath);
	if(clear_style==clear_pdf){
		ZeroMemory(val,10);
		sprintf_s(val, 10, "%d", m_papday);
		WritePrivateProfileString("favor", "clear-paper-days", val,iniPath);
	}
	else if(clear_style==clear_all){
		ZeroMemory(val,10);
		sprintf_s(val, 10, "%d", m_allday);
		WritePrivateProfileString("favor", "clear-all-days", val,iniPath);
	}
	ZeroMemory(val,10);
	sprintf_s(val, 10, "%d", collect_style);
	WritePrivateProfileString("favor", "collect-style", val, iniPath);
	if(collect_style==collect_spec){
		CString path;
		path.Format("%s\\system\\colPath.inf", CurrentPath);
		CStdioFile cf;
		if(cf.Open(path, CFile::modeCreate|CFile::modeWrite)){
			CString buffer;
			for(int nIndex=0; nIndex<m_collectList.GetCount(); nIndex++){
				m_collectList.GetText(nIndex,buffer);
				cf.WriteString(buffer);
				cf.WriteString("\n");
			}
			cf.Close();
		}
	}
}
