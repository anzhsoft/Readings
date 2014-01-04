// DialogComments.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "HttpDownLoadDlg.h"
#include "DialogComments.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern char CurrentPath[_MAX_PATH];
extern char AspServer[1024];
/////////////////////////////////////////////////////////////////////////////
// CDialogComments dialog


CDialogComments::CDialogComments(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogComments::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogComments)
	m_comments = _T("");
	m_mycomments = _T("");
	//}}AFX_DATA_INIT
}

CDialogComments::CDialogComments(CString title, int id, CString username, LPCTSTR lpszTemplateName/*=""*/,CWnd* pParent/*=NULL*/)
	: CDialog(CDialogComments::IDD, pParent)
{
	m_comments = _T("");
	m_mycomments = _T("");
	sid = id;
	name = username;
	resourcename = title;
	//this->SetWindowTextA(title);
}
CDialogComments::CDialogComments(int id,CWnd* pParent /*= NULL*/)
	: CDialog(CDialogComments::IDD, pParent)
{
	m_comments = _T("");
	m_mycomments = _T("");
	sid = id;
}
void CDialogComments::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogComments)
	DDX_Text(pDX, IDC_EDIT_COMMENTS, m_comments);
	DDX_Text(pDX, IDC_EDIT_MYCOMMENTS, m_mycomments);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogComments, CDialog)
	//{{AFX_MSG_MAP(CDialogComments)
	ON_BN_CLICKED(IDC_BUTTON_SUBMIT, OnButtonSubmit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogComments message handlers

void CDialogComments::OnButtonSubmit() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_mycomments=="")
		return ;
	char datetime[BUFFER_SIZE] = {0};	
	CTime tm = CTime::GetCurrentTime();
	sprintf_s(datetime, BUFFER_SIZE, "%02d-%02d-%02d %02d:%02d:%02d", tm.GetYear(),tm.GetMonth(),tm.GetDay(),tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	CString comments = m_mycomments;
	//1、向大家的评论中写内容
	UpdateData(TRUE);
	m_comments = m_comments + datetime + "  " +name+"\r\n";
	m_comments = m_comments + comments+"\r\n";
	m_mycomments = "";
	UpdateData(FALSE);
	//2、把comments插入数据库
	try{
		CHttpDownLoadDlg dlg;
		//insertcomments.asp?sid=1&username=start_dzh&comments=test
		dlg.m_URL.Format( "%s/share/comments/insertcomments.asp?sid=%d&username=%s&comments=%s", AspServer,sid,name,comments);
		dlg.m_Path.Format("%s\\temp\\subcomments_%d", CurrentPath,sid);
		if(dlg.DoModal()!=IDOK){
			Tips("创建群落失败",td_alert,5);
			return ;
		}
	}
	catch (_com_error& e) {
		AfxMessageBox("操作失败，请重新尝试！");
		return ;
	}
	return ;
}

BOOL CDialogComments::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//初始化 其他人的评论
	OtherComments();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDialogComments::OtherComments()
{
	try{
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format( "%s/share/comments/getcomments.asp?sid=%d", AspServer,sid);
		dlg.m_Path.Format("%s\\temp\\comments_%d", CurrentPath,sid);
		if(dlg.DoModal()!=IDOK){
			Tips("创建群落失败",td_alert,5);
			return FALSE;
		}
			int pos = 0;

		MSXML::IXMLDOMDocumentPtr pDoc;
		MSXML::IXMLDOMNodeListPtr pNodeList;
		try{
			HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
			if(!SUCCEEDED(hr))
				throw 1;	//xml 解析失败
			if(!pDoc->load((LPCSTR)dlg.m_Path))
				throw 2;	//xml 解析失败
			pNodeList = pDoc->selectNodes("comments/onecomment");
			if(pNodeList){
				for(pos=0; pos<pNodeList->length /*&& !pTreeView->quit*/; pos++){
					CString datetime = GetXmlValue(pNodeList, "datetime", pos);
					CString comment = GetXmlValue(pNodeList, "comment", pos);
					CString username = GetXmlValue(pNodeList, "username", pos);
					if (username=="")
						username = "匿名";
					m_comments = m_comments + datetime +"  " + username +"\r\n";
					m_comments = m_comments + comment +"\r\n";
					//CString totalSource = GetXmlValue(pNodeList,"sourcenum",pos);
					//CString tag = GetXmlValue(pNodeList,"tag",pos);
					//gname = gname+"("+totalSource+")";//加入统计信息
					//GROUPINFO* ginfo = (GROUPINFO*)calloc(1,sizeof(GROUPINFO));
					//ginfo->gid = atol((LPCSTR)gid);
					//strcpy(ginfo->groupname, gname);
					//ginfo->totalnum  = atol(totalPeople);
				//	HTREEITEM hCurItem = pTree->InsertItem(gname, 28, 28, hItem);
				//	pTreeView->SetTreeItemData(hCurItem, Is_NetGroup, NULL, 0, (DWORD)ginfo,tag);
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(_com_error& e){}
		UpdateData(FALSE);
	}
	catch(_com_error& e){
		AfxMessageBox("操作失败，请重新尝试！");
	}
	return TRUE;
}
