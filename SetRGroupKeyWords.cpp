// SetRGroupKeyWords.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "SetRGroupKeyWords.h"
#include "HttpDownLoadDlg.h"
#include "base64.h"

extern char UserName[50];
extern char CurrentPath[_MAX_PATH];
extern char AspServer[1024];
#ifdef _DEBUG

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRGroupKeyWords dialog


CSetRGroupKeyWords::CSetRGroupKeyWords(CWnd* pParent /*=NULL*/)
	: CDialog(CSetRGroupKeyWords::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRGroupKeyWords)
	m_key1 = _T("");
	m_key2 = _T("");
	m_key3 = _T("");
	m_okeywords = _T("");
	m_record = _T("");
	m_ztc1 = _T("");
	m_ztc2 = _T("");
	m_ztc3 = _T("");
	m_xxc1 = _T("");
	m_xxc2 = _T("");
	m_xxc3 = _T("");
	m_zsd1 = _T("");
	m_zsd2 = _T("");
	m_zsd3 = _T("");
	m_zyc1 = _T("");
	m_zyc2 = _T("");
	m_zyc3 = _T("");
	m_pcc1 = _T("");
	m_pcc2 = _T("");
	m_pcc3 = _T("");
	m_gjc1 = _T("");
	m_gjc2 = _T("");
	m_gjc3 = _T("");
	m_treename = _T("");
	m_static_gjc1 = _T("");
	m_static_gjc2 = _T("");
	m_static_gjc3 = _T("");
	m_static_zsd1 = _T("");
	m_static_zsd2 = _T("");
	m_static_zsd3 = _T("");
	m_school = _T("");
	m_keywords = _T("");
	//}}AFX_DATA_INIT
}

CSetRGroupKeyWords::CSetRGroupKeyWords(CString keywords,CWnd* pParent/* = NULL*/)
	: CDialog(CSetRGroupKeyWords::IDD, pParent)
{
		//三个编辑框中的显示
		m_key1 = _T("");
		m_key2 = _T("");
		m_key3 = _T("");
		
		//所有主题词的联合
		m_keywords = _T("");

		m_okeywords = _T("");
//added by zuoss 20080813//对应5个主题词（包括设定和提取）	
		int i = 0;
		for (i = 0; i < keyNum; i++)
		{
			m_key[i] = _T("");
		}
//end add

//added by zuoss 20080813
	CString  remaining = keywords;
	while (remaining != "")
	{
		int pos1 = remaining.Find(' ');
		if (pos1 > 0)
		{	
			m_key[i] = remaining.Left(pos1);
			++i;
		}
		else
		{
			m_key[i] = remaining;
			break;
		}
		remaining = remaining.Right(remaining.GetLength()-pos1-1);
	}
	m_key1 = m_key[0];
	m_key2 = m_key[1];
	m_key3 = m_key[2];
//end add
}
CSetRGroupKeyWords::CSetRGroupKeyWords(CString keywords,int id, int flag, CString treename,CWnd* pParent /*= NULL*/)
	: CDialog(CSetRGroupKeyWords::IDD, pParent)
{
		m_key1 = _T("");
		m_key2 = _T("");
		m_key3 = _T("");
		m_keywords = _T("");
		m_okeywords = _T("");
		m_school = _T("");
		m_treename = treename;
		TreeID = id;
		Flag = flag;
		school_num = 0;
//added by zuoss 20080813	//对应5个主题词（包括设定和提取）
		int i = 0;
		for (i = 0; i < keyNum; i++)
		{
			m_key[i] = _T("");
			m_gjc[i] = _T("");
			m_zsd[i] = _T("");
			m_pcc[i] = _T("");
			m_xxc[i] = _T("");
			m_zyc[i] = _T("");
		}
		for (i = 0; i < maxSchoolNum; i++)
		{
			all_school[i] = _T("");
		}
//end add

//added by zuoss 20080814
	GetWikiInfo(id,flag);	//读取当前节点的Wiki设定信息
//end add

	m_ztc1 = "主题词1：";
	m_ztc2 = "主题词2：";
	m_ztc3 = "主题词3：";

	m_static_gjc1 = "关键词1：";
	m_static_gjc2 = "关键词2：";
	m_static_gjc3 = "关键词3：";
	
	m_static_zsd1 = "知识点1：";
	m_static_zsd2 = "知识点2：";
	m_static_zsd3 = "知识点3：";
	
	ShowTagRecord(id, flag, UserName);//增加历史纪录部分
}

/*从长串中分解出各个词 20080826 
*flag 标示对哪个变量组赋值：
1 —— 主题词
2 —— 关键词
3 —— 知识点
4 —— 排除词
5 —— 信息词
6 —— 资源词
*/
void CSetRGroupKeyWords::GetAllKeys(CString words,int flag)
{
//	CString m_tempkeywords[keyNum];//暂存 
	CString m_tempkeywords[maxSchoolNum];//暂存 maxSchoolNum=20 比keyNum大 暂取20

	CString  remaining = words;
	int i = 0;
	int totalnum = 0; //words中分解得到的词的个数

//	if (words.GetLength() > 0)
//			m_okeywords = words;
	
	while (remaining != "")
	{
		int pos = remaining.Find(' ');
		while (pos == 0)
		{
			remaining = remaining.Right(remaining.GetLength()-1);
			pos = remaining.Find(' ');
		}
		if (pos > 0)
		{	
			m_tempkeywords[i] = remaining.Left(pos);
			i++;
		}
		else
		{
			m_tempkeywords[i] = remaining;
			i++;
			break;
		}
		remaining = remaining.Right(remaining.GetLength()-pos-1);
	}
	totalnum = i; 
	//存储并编辑框中显示	
	switch(flag)
	{
	case 1:
		for (i = 0;i < keyNum; ++i)
		{
			if (i < totalnum)
				m_key[i] = m_tempkeywords[i];
			else
				m_key[i] = "";
		}
		m_key1 = m_key[0];
		m_key2 = m_key[1];
		m_key3 = m_key[2];
		break;
	case 2:
		for (i = 0;i < keyNum; ++i)
		{
			if (i < totalnum)
				m_gjc[i] = m_tempkeywords[i];
			else
				m_gjc[i] = "";
		}
		m_gjc1 = m_gjc[0];
		m_gjc2 = m_gjc[1];
		m_gjc3 = m_gjc[2];
		break;
	case 3:
		for (i = 0;i < keyNum; ++i)
		{
			if (i < totalnum)
				m_zsd[i] = m_tempkeywords[i];
			else
				m_zsd[i] = "";
		}
		m_zsd1 = m_zsd[0];
		m_zsd2 = m_zsd[1];
		m_zsd3 = m_zsd[2];
		break;
	case  4:
		for (i = 0;i < keyNum; ++i)
		{
			if(i < totalnum)
				m_pcc[i] = m_tempkeywords[i];
			else
				m_pcc[i] = "";
		}
		m_pcc1 = m_pcc[0];
		m_pcc2 = m_pcc[1];
		m_pcc3 = m_pcc[2];
		break;
	case  5:
		for (i = 0;i < keyNum; ++i)
		{
			if(i < totalnum)
				m_xxc[i] = m_tempkeywords[i];
			else
				m_xxc[i] = "";
		}
		m_xxc1 = m_xxc[0];
		m_xxc2 = m_xxc[1];
		m_xxc3 = m_xxc[2];
		break;
	case  6:
		for (i = 0;i < keyNum; ++i)
		{
			if(i < totalnum)
				m_zyc[i] = m_tempkeywords[i];	
			else
				m_zyc[i] = "";
		}
		m_zyc1 = m_zyc[0];
		m_zyc2 = m_zyc[1];
		m_zyc3 = m_zyc[2];
		break;
//added by zuoss 20080918 学校信息分解存储到数组
	case  7:
		for (i = 0;i < maxSchoolNum; ++i)
		{
			if (i < totalnum)
			{
				all_school[i] = m_tempkeywords[i];
				school_num++;
			}
			else
				all_school[i] = "";
		}
  		break;
//end add
 	}
}
//end add

//added by zuoss 20080814
void CSetRGroupKeyWords::GetWikiInfo(int id,int flag)
{
	CTime ct = CTime::GetCurrentTime();
	int day = ct.GetDay();
	int month = ct.GetMonth();

	CHttpDownLoadDlg dlg;
	dlg.m_URL.Format("%s/share/getWikiNode.asp?id=%d&flag=%d", AspServer,id,flag);
	dlg.m_Path.Format("%s\\temp\\WikiNode_%d_%d_%d.xml", CurrentPath,id,month,day);
	if(dlg.DoModal()!=IDOK)
	{
		Tips("创建群落失败",td_alert,5);
		return ;
	}	
	
	int pos = 0;
	CString ztc_t,gjc_t,zsd_t,pcc_t,xxc_t,zyc_t;
	CString curnum;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try
	{
		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))	
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)dlg.m_Path))
			throw 2;	//xml 解析失败
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList)
		{
			ztc_t = GetXmlValue(pNodeList, "ztc", pos);
			gjc_t = GetXmlValue(pNodeList,"gjz",pos);
			zsd_t = GetXmlValue(pNodeList,"zsd",pos);
			pcc_t = GetXmlValue(pNodeList,"pcc",pos);
			xxc_t = GetXmlValue(pNodeList,"xxc",pos);
			zyc_t = GetXmlValue(pNodeList,"zyc",pos);
			m_school = GetXmlValue(pNodeList,"school",pos);
			curnum = GetXmlValue(pNodeList,"curNum",pos);//20080904

			m_curnum = atoi(curnum);
			latestNum = atoi(curnum);

			GetAllKeys(ztc_t,1);
			GetAllKeys(gjc_t,2);
			GetAllKeys(zsd_t,3);
			GetAllKeys(pcc_t,4);
			GetAllKeys(xxc_t,5);
			GetAllKeys(zyc_t,6);
        	GetAllKeys(m_school,7);

			pNodeList.Release();
		}
		pDoc.Release();
	}	
	catch(_com_error& e){}
		catch(...){}
}
//end add

void CSetRGroupKeyWords::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRGroupKeyWords)
	DDX_Text(pDX, IDC_EDIT_KEY1, m_key1);
	DDX_Text(pDX, IDC_EDIT_KEY2, m_key2);
	DDX_Text(pDX, IDC_EDIT_KEY3, m_key3);
	DDX_Text(pDX, IDC_EDIT_OKEYWORDS, m_okeywords);
	DDX_Text(pDX, IDC_EDIT_RECORD, m_record);
	DDX_Text(pDX, IDC_STATIC_ZTC1, m_ztc1);
	DDX_Text(pDX, IDC_STATIC_ZTC2, m_ztc2);
	DDX_Text(pDX, IDC_STATIC_ZTC3, m_ztc3);
	DDX_Text(pDX, IDC_EDIT_XXC1, m_xxc1);
	DDX_Text(pDX, IDC_EDIT_XXC2, m_xxc2);
	DDX_Text(pDX, IDC_EDIT_XXC3, m_xxc3);
	DDX_Text(pDX, IDC_EDIT_ZSD1, m_zsd1);
	DDX_Text(pDX, IDC_EDIT_ZSD2, m_zsd2);
	DDX_Text(pDX, IDC_EDIT_ZSD3, m_zsd3);
	DDX_Text(pDX, IDC_EDIT_ZYC1, m_zyc1);
	DDX_Text(pDX, IDC_EDIT_ZYC2, m_zyc2);
	DDX_Text(pDX, IDC_EDIT_ZYC3, m_zyc3);
	DDX_Text(pDX, IDC_EDIT_PCC1, m_pcc1);
	DDX_Text(pDX, IDC_EDIT_PCC2, m_pcc2);
	DDX_Text(pDX, IDC_EDIT_PCC3, m_pcc3);
	DDX_Text(pDX, IDC_EDIT_GJC1, m_gjc1);
	DDX_Text(pDX, IDC_EDIT_GJC2, m_gjc2);
	DDX_Text(pDX, IDC_EDIT_GJC3, m_gjc3);
	DDX_Text(pDX, IDC_EDIT_TREENAME, m_treename);
	DDX_Text(pDX, IDC_STATIC_GJC1, m_static_gjc1);
	DDX_Text(pDX, IDC_STATIC_GJC2, m_static_gjc2);
	DDX_Text(pDX, IDC_STATIC_GJC3, m_static_gjc3);
	DDX_Text(pDX, IDC_STATIC_ZSD1, m_static_zsd1);
	DDX_Text(pDX, IDC_STATIC_ZSD2, m_static_zsd2);
	DDX_Text(pDX, IDC_STATIC_ZSD3, m_static_zsd3);
	DDX_Text(pDX, IDC_EDIT_SCHOOL, m_school);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRGroupKeyWords, CDialog)
	//{{AFX_MSG_MAP(CSetRGroupKeyWords)
	ON_BN_CLICKED(IDC_BUTTON_ZTC_UP, OnButtonZtcUp)
	ON_BN_CLICKED(IDC_BUTTON_ZTC_DOWN, OnButtonZtcDown)
	ON_EN_CHANGE(IDC_EDIT_KEY1, OnChangeEditKey1)
	ON_EN_CHANGE(IDC_EDIT_KEY2, OnChangeEditKey2)
	ON_EN_CHANGE(IDC_EDIT_KEY3, OnChangeEditKey3)
	ON_BN_CLICKED(IDC_BUTTON_GJC_DOWN, OnButtonGjcDown)
	ON_BN_CLICKED(IDC_BUTTON_GJC_UP, OnButtonGjcUp)
	ON_BN_CLICKED(IDC_BUTTON_ZSD_DOWN, OnButtonZsdDown)
	ON_BN_CLICKED(IDC_BUTTON_ZSD_UP, OnButtonZsdUp)
	ON_EN_CHANGE(IDC_EDIT_GJC1, OnChangeEditGjc1)
	ON_EN_CHANGE(IDC_EDIT_GJC2, OnChangeEditGjc2)
	ON_EN_CHANGE(IDC_EDIT_GJC3, OnChangeEditGjc3)
	ON_EN_CHANGE(IDC_EDIT_ZSD1, OnChangeEditZsd1)
	ON_EN_CHANGE(IDC_EDIT_ZSD2, OnChangeEditZsd2)
	ON_EN_CHANGE(IDC_EDIT_ZSD3, OnChangeEditZsd3)
	ON_BN_CLICKED(IDC_HISTORY_UP, OnHistoryUp)
	ON_BN_CLICKED(IDC_HISTORY_DOWN, OnHistoryDown)
	ON_EN_CHANGE(IDC_EDIT_SCHOOL, OnChangeEditSchool)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRGroupKeyWords message handlers

void CSetRGroupKeyWords::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
//added by zuoss 20080813
	for (int i = 0; i < keyNum; ++i)
	{
		if (m_key[i].GetLength() > 20)
		{
			AfxMessageBox("主题词设定不能大于20，请重新设定！");
			return;
		}
		else
		{
			m_keywords = m_keywords + m_key[i] + " ";
		}
	}
//end add
		//访问服务器，把各类词的信息同步更新到数据库中
	UpdateAllKeyWords();
	CDialog::OnOK();
}

void CSetRGroupKeyWords::ShowTagRecord(int id, int flag, CString uname)
{
	//http://61.181.14.184/Readman/share/getmyGroupNode.asp?username=Celialin
		CHttpDownLoadDlg dlg;
		dlg.m_URL.Format("%s/share/showtag.asp?username=%s&id=%d&flag=%d", AspServer, UserName,id,flag);
		dlg.m_Path.Format("%s\\temp\\node_tag.xml", CurrentPath);
		if(dlg.DoModal()!=IDOK){
			Tips("创建群落失败",td_alert,5);
			return ;
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
			pNodeList = pDoc->selectNodes("NodeList/OneNode");
			if(pNodeList){
				for(pos=0; pos<pNodeList->length /*&& !pTreeView->quit*/; pos++){
					CString keywords = GetXmlValue(pNodeList, "key", pos);
					if (keywords.GetLength()<=0)
					{
						keywords = "(空)";
					}
					CString founder = GetXmlValue(pNodeList, "founder", pos);
					CString foundertime = GetXmlValue(pNodeList, "foundtime", pos);
					char *empty= (char*)malloc(100-keywords.GetLength());
					//char* buf = (char*)malloc(len+1);
					memset(empty,0,100-keywords.GetLength());
					memset(empty,' ',99-keywords.GetLength());
					m_record = m_record + "主题词："+keywords+empty+"添加者："+founder+"\r\n";
					//	m_comments = m_comments + datetime + "  " +name+"\r\n";
				}
				pNodeList.Release();
			}
			pDoc.Release();
		}
		catch(_com_error& e){}
		catch(...){}
	//	UpdateData(FALSE);
}

void CSetRGroupKeyWords::SetGroupTitle()//added by zuoss 20080905设置分组框标题
{
	int i;
	for (i = 0; i < keyNum; ++i)
	{
		if (m_key[i] == "")
		{
			break;
		}
	}//得到的i为已经设置的主题词的个数。
	CString title;
	title.Format("主题词（%d）",i);//初始GroupBox的Caption标题。
	CWnd::GetDlgItem(IDC_ZTC)->SetWindowText(title);
	
	for (i = 0; i < keyNum; ++i)
	{
		if (m_gjc[i] == "")
		{
			break;
		}
	}
	title.Format("关键词（%d）",i);
	CWnd::GetDlgItem(IDC_GJC)->SetWindowText(title);

	for (i = 0; i < keyNum; ++i)
	{
		if (m_zsd[i] == "")
		{
			break;
		}
	}
	title.Format("知识点（%d）",i);
	CWnd::GetDlgItem(IDC_ZSD)->SetWindowText(title);

	for (i = 0; i < keyNum; ++i)
	{
		if (m_pcc[i] == "")
		{
			break;
		}
	}
	title.Format("排除词（%d）",i);
	CWnd::GetDlgItem(IDC_PCC)->SetWindowText(title);

	for (i = 0; i < keyNum; ++i)
	{
		if (m_xxc[i] == "")
		{
			break;
		}
	}
	title.Format("信息词（%d）",i);
	CWnd::GetDlgItem(IDC_XXC)->SetWindowText(title);

	for (i = 0; i < keyNum; ++i)
	{
		if (m_zyc[i] == "")
		{
			break;
		}
	}
	title.Format("资源词（%d）",i);
	CWnd::GetDlgItem(IDC_ZYC)->SetWindowText(title);

}
//added by zuoss 20080812 添加向上向下按钮功能，并将主题词增加到5个
BOOL CSetRGroupKeyWords::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetGroupTitle();

	CWnd::GetDlgItem(IDC_BUTTON_ZTC_UP)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_BUTTON_GJC_UP)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_BUTTON_ZSD_UP)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_BUTTON_PCC_UP)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_BUTTON_XXC_UP)->EnableWindow(FALSE);
	CWnd::GetDlgItem(IDC_BUTTON_ZYC_UP)->EnableWindow(FALSE);

	if (m_curnum == 1 || m_curnum == 0)
	{
		CWnd::GetDlgItem(IDC_HISTORY_UP)->EnableWindow(FALSE);
 	}
	else
	{
		CWnd::GetDlgItem(IDC_HISTORY_UP)->EnableWindow(TRUE);
 	}
 	CWnd::GetDlgItem(IDC_HISTORY_DOWN)->EnableWindow(FALSE);

//added by zuoss 20080918 显示学校信息 
	CComboBox *pCombo;
	CString str;
	pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_SCHOOL); 
	pCombo->ResetContent();//消除ID为IDC_COMBO_SCHOOL现有所有内容
	for (int i = 0;i < school_num; ++i)
	{
		pCombo->InsertString(i,all_school[i]);//为控件添加数据
	}
	if (school_num!=0)
	{
		pCombo->SetCurSel(school_num - 1);//设置最后一次插入的学校为显示内容
		m_school = all_school[school_num -1];
	}

	UpdateData(FALSE);
//end add 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetRGroupKeyWords::OnButtonZtcUp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int flag = 0;
	for (int i = 2; i < (keyNum -1); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("主题词%d：",i);
		if (m_ztc1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_ztc1.Format("主题词%d：",flag - 1);
	m_ztc2.Format("主题词%d：",flag);
	m_ztc3.Format("主题词%d：",flag + 1);
	
	m_key1 = m_key[flag - 2];
	m_key2 = m_key[flag - 1];
	m_key3 = m_key[flag];

	UpdateData(FALSE);
	UpdateData(TRUE);

	if (m_ztc1 == "主题词1：")
		CWnd::GetDlgItem(IDC_BUTTON_ZTC_UP)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_ZTC_UP)->EnableWindow(TRUE);

	CString cap;
	cap.Format("主题词%d：",keyNum);
	if (m_ztc3 == cap)
		CWnd::GetDlgItem(IDC_BUTTON_ZTC_DOWN)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_ZTC_DOWN)->EnableWindow(TRUE);	
}

void CSetRGroupKeyWords::OnButtonZtcDown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 1; i < (keyNum - 2); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("主题词%d：",i);
		if (m_ztc1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_ztc1.Format("主题词%d：",flag + 1);
	m_ztc2.Format("主题词%d：",flag + 2);
	m_ztc3.Format("主题词%d：",flag + 3);

	m_key1 = m_key[flag];
	m_key2 = m_key[flag + 1];
	m_key3 = m_key[flag + 2];

	UpdateData(FALSE);
	UpdateData(TRUE);

	if (m_ztc1 == "主题词1：")
		CWnd::GetDlgItem(IDC_BUTTON_ZTC_UP)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_ZTC_UP)->EnableWindow(TRUE);

	CString cap;
	cap.Format("主题词%d：",keyNum);
	if (m_ztc3 == cap)
		CWnd::GetDlgItem(IDC_BUTTON_ZTC_DOWN)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_ZTC_DOWN)->EnableWindow(TRUE);	
}

void CSetRGroupKeyWords::OnChangeEditKey1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 1; i <= (keyNum - 2); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("主题词%d：",i);
		if (m_ztc1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_key[flag - 1] = m_key1;
}

void CSetRGroupKeyWords::OnChangeEditKey2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 2; i <= (keyNum - 1); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("主题词%d：",i);
		if (m_ztc2 == caption)
		{
			flag = i;
			break;
		}
	}
	m_key[flag - 1] = m_key2;	
}

void CSetRGroupKeyWords::OnChangeEditKey3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 3; i <= keyNum; ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("主题词%d：",i);
		if (m_ztc3 == caption)
		{
			flag = i;
			break;
		}
	}
	m_key[flag - 1] = m_key3;	
}

void CSetRGroupKeyWords::UpdateAllKeyWords()
{
	CString ztcs = "";
	CString gjcs = "";
	CString zsds = "";

 	CString pccs = m_pcc1+" "+m_pcc2+" "+m_pcc3;
	CString xxcs = m_xxc1+" "+m_xxc2+" "+m_xxc3;
	CString zycs = m_zyc1+" "+m_zyc2+" "+m_zyc3;

	int i;
	
	for ( i = 0; i < keyNum; i++)
	{
		if (m_key[i] != "")
		{
			ztcs += m_key[i] + " ";
		}
		if (m_gjc[i] != "")
		{
			gjcs += m_gjc[i] + " ";
		}
		if (m_zsd[i] != "")
		{
			zsds += m_zsd[i] + " ";
		}
//		if (m_pcc[i] != "")
//		{
//			pccs += m_pcc[i] + " ";
//		}
//		if (m_xxc[i] != "")
//		{
//			xxcs += m_xxc[i] + " ";
//		}
//		if (m_zyc[i] != "")
//		{
//			zycs += m_zyc[i] + " ";
//		}
	}
//added by zuoss 20080917 对学校下拉框操作

	char* ztc_b64 = (char*)encode2((PUCHAR)(LPCSTR)ztcs,strlen(ztcs));	
	char* gjc_b64 = (char*)encode2((PUCHAR)(LPCSTR)gjcs,strlen(gjcs));	
	char* pcc_b64 = (char*)encode2((PUCHAR)(LPCSTR)pccs,strlen(pccs));
	char* xxc_b64 = (char*)encode2((PUCHAR)(LPCSTR)xxcs,strlen(xxcs));
	char* zsd_b64 = (char*)encode2((PUCHAR)(LPCSTR)zsds,strlen(zsds));
	char* zyc_b64 = (char*)encode2((PUCHAR)(LPCSTR)zycs,strlen(zycs));
	
//added by zuoss 20080918 保存学校信息
	CString str_schools = "";
	for (i = 0;i < maxSchoolNum;++i)
	{
		if (all_school[i] != "")
		{
			str_schools = str_schools + all_school[i] + " "; 
		}		
	}
	char* school_b64 = (char*)encode2((PUCHAR)(LPCSTR)str_schools,strlen(str_schools));
//end add	
	CHttpDownLoadDlg dlg;
	dlg.m_URL.Format("%s/share/UpdateWikinode.asp?id=%d&flag=%d&ztc=%s&gjc=%s&zsd=%s&pcc=%s&xxc=%s&zyc=%s&school=%s", AspServer, TreeID,Flag,ztc_b64,gjc_b64,zsd_b64,pcc_b64,xxc_b64,zyc_b64,school_b64);
	dlg.m_Path.Format("%s\\temp\\UpdateWikinode.txt", CurrentPath);
	if(dlg.DoModal()!=IDOK){
		Tips("更新知识树信息失败",td_alert,5);
		return ;
	}
	CHttpDownLoadDlg wikidlg;
	wikidlg.m_URL.Format("%s/share/setWikiHistory.asp?id=%d&flag=%d",AspServer,TreeID,Flag);
	if (wikidlg.DoModal()!=IDOK)
	{
		Tips("存储历史记录失败",td_alert,5);
		return;
	}
}

void CSetRGroupKeyWords::OnButtonGjcDown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int flag = 0;
	for (int i = 1; i < (keyNum -2); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("关键词%d：",i);
		if (m_static_gjc1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_static_gjc1.Format("关键词%d：",flag + 1);
	m_static_gjc2.Format("关键词%d：",flag + 2);
	m_static_gjc3.Format("关键词%d：",flag + 3);
	
	m_gjc1 = m_gjc[flag];
	m_gjc2 = m_gjc[flag + 1];
	m_gjc3 = m_gjc[flag + 2];

	UpdateData(FALSE);
	UpdateData(TRUE);

	if (m_static_gjc1 == "关键词1：")
		CWnd::GetDlgItem(IDC_BUTTON_GJC_UP)->EnableWindow(FALSE);
	else
 		CWnd::GetDlgItem(IDC_BUTTON_GJC_UP)->EnableWindow(TRUE);

	CString cap;
	cap.Format("关键词%d：",keyNum);
	if (m_static_gjc3 == cap)
		CWnd::GetDlgItem(IDC_BUTTON_GJC_DOWN)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_GJC_DOWN)->EnableWindow(TRUE);		
}

void CSetRGroupKeyWords::OnButtonGjcUp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 2; i < (keyNum -1); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("关键词%d：",i);
		if (m_static_gjc1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_static_gjc1.Format("关键词%d：",flag - 1);
	m_static_gjc2.Format("关键词%d：",flag);
	m_static_gjc3.Format("关键词%d：",flag + 1);
	
	m_gjc1 = m_gjc[flag - 2];
	m_gjc2 = m_gjc[flag - 1];
	m_gjc3 = m_gjc[flag];

	UpdateData(FALSE);
	UpdateData(TRUE);

	if (m_static_gjc1 == "关键词1：")
		CWnd::GetDlgItem(IDC_BUTTON_GJC_UP)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_GJC_UP)->EnableWindow(TRUE);

	CString cap;
	cap.Format("关键词%d：",keyNum);
	if (m_static_gjc3 == cap)
		CWnd::GetDlgItem(IDC_BUTTON_GJC_DOWN)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_GJC_DOWN)->EnableWindow(TRUE);		
}

void CSetRGroupKeyWords::OnButtonZsdDown() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int flag = 0;
	for (int i = 1; i < (keyNum - 2); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("知识点%d：",i);
		if (m_static_zsd1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_static_zsd1.Format("知识点%d：",flag + 1);
	m_static_zsd2.Format("知识点%d：",flag + 2);
	m_static_zsd3.Format("知识点%d：",flag + 3);
	
	m_zsd1 = m_zsd[flag];
	m_zsd2 = m_zsd[flag + 1];
	m_zsd3 = m_zsd[flag + 2];

	UpdateData(FALSE);
	UpdateData(TRUE);

	if (m_static_zsd1 == "知识点1：")
		CWnd::GetDlgItem(IDC_BUTTON_ZSD_UP)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_ZSD_UP)->EnableWindow(TRUE);

	CString cap;
	cap.Format("知识点%d：",keyNum);
	if (m_static_zsd3 == cap)
		CWnd::GetDlgItem(IDC_BUTTON_ZSD_DOWN)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_ZSD_DOWN)->EnableWindow(TRUE);		
}

void CSetRGroupKeyWords::OnButtonZsdUp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 2; i < (keyNum -1); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("知识点%d：",i);
		if (m_static_zsd1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_static_zsd1.Format("知识点%d：",flag - 1);
	m_static_zsd2.Format("知识点%d：",flag);
	m_static_zsd3.Format("知识点%d：",flag + 1);
	
	m_zsd1 = m_zsd[flag - 2];
	m_zsd2 = m_zsd[flag - 1];
	m_zsd3 = m_zsd[flag];

	UpdateData(FALSE);
	UpdateData(TRUE);

	if (m_static_zsd1 == "知识点1：")
		CWnd::GetDlgItem(IDC_BUTTON_ZSD_UP)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_ZSD_UP)->EnableWindow(TRUE);

	CString cap;
	cap.Format("知识点%d：",keyNum);
	if (m_static_zsd3 == cap)
		CWnd::GetDlgItem(IDC_BUTTON_ZSD_DOWN)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_BUTTON_ZSD_DOWN)->EnableWindow(TRUE);		
}

void CSetRGroupKeyWords::OnChangeEditGjc1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 1; i <= (keyNum - 2); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("关键词%d：",i);
		if (m_static_gjc1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_gjc[flag - 1] = m_gjc1;		
}

void CSetRGroupKeyWords::OnChangeEditGjc2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 2; i <= (keyNum - 1); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("关键词%d：",i);
		if (m_static_gjc2 == caption)
		{
			flag = i;
			break;
		}
	}
	m_gjc[flag - 1] = m_gjc2;	
}

void CSetRGroupKeyWords::OnChangeEditGjc3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 3; i <= keyNum; ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("关键词%d：",i);
		if (m_static_gjc3 == caption)
		{
			flag = i;
			break;
		}
	}
	m_gjc[flag - 1] = m_gjc3;		
}

void CSetRGroupKeyWords::OnChangeEditZsd1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 1; i <= (keyNum - 2); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("知识点%d：",i);
		if (m_static_zsd1 == caption)
		{
			flag = i;
			break;
		}
	}
	m_zsd[flag - 1] = m_zsd1;			
}

void CSetRGroupKeyWords::OnChangeEditZsd2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 2; i <= (keyNum - 1); ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("知识点%d：",i);
		if (m_static_zsd2 == caption)
		{
			flag = i;
			break;
		}
	}
	m_zsd[flag - 1] = m_zsd2;	
}

void CSetRGroupKeyWords::OnChangeEditZsd3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int flag = 0;
	for (int i = 3; i <= keyNum; ++i)
	{
		CString   caption;		//第一个标签的内容
		caption.Format("知识点%d：",i);
		if (m_static_zsd3 == caption)
		{
			flag = i;
			break;
		}
	}
	m_zsd[flag - 1] = m_zsd3;		
}

void CSetRGroupKeyWords::OnHistoryUp() 
{
	// TODO: Add your control notification handler code here
	--m_curnum;
	ShowHistory();
}

void CSetRGroupKeyWords::OnHistoryDown() 
{
	// TODO: Add your control notification handler code here
	++m_curnum;
    ShowHistory();
}
void CSetRGroupKeyWords::ShowHistory()
{
	if (m_curnum == 1)
		CWnd::GetDlgItem(IDC_HISTORY_UP)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_HISTORY_UP)->EnableWindow(TRUE);
	if (m_curnum == latestNum)
		CWnd::GetDlgItem(IDC_HISTORY_DOWN)->EnableWindow(FALSE);
	else
		CWnd::GetDlgItem(IDC_HISTORY_DOWN)->EnableWindow(TRUE);

// 修改，20080908 
// 为避免频繁的服务器请求，先从本地xml中load，如果成功则说明已经下载过该文件
// 不必重新从服务器下载，直接读本地xml即可。否则再下载。
	CString filename;
	filename.Format("wiki_%d_%d_%d.xml",TreeID,Flag,m_curnum);

	CString ztc_t,gjc_t,zsd_t,pcc_t,xxc_t,zyc_t;
	MSXML::IXMLDOMDocumentPtr pDoc;
	MSXML::IXMLDOMNodeListPtr pNodeList;
	try
	{
		CHttpDownLoadDlg wikidlg;
		wikidlg.m_Path.Format("%s\\temp\\%s", CurrentPath,filename);//本地xml文件
		int pos = 0;

		HRESULT hr =  pDoc.CreateInstance(__uuidof(MSXML::DOMDocument));
		if(!SUCCEEDED(hr))
			throw 1;	//xml 解析失败
		if(!pDoc->load((LPCSTR)wikidlg.m_Path))//如果本地已有该文件，则load成功。否则从服务器下载。
		{
			wikidlg.m_URL.Format("%s/share/WikiHistoryXml/%s",AspServer,filename);
			wikidlg.m_Path.Format("%s\\temp\\%s", CurrentPath,filename);
			if (wikidlg.DoModal()!=IDOK)
			{
				Tips("下载历史信息失败",td_alert,5);
				return;
			}
			if(!SUCCEEDED(hr))
				throw 1;	//xml 解析失败
			if(!pDoc->load((LPCSTR)wikidlg.m_Path))
				throw 2;	//xml 解析失败		
		}
		
		pNodeList = pDoc->selectNodes("NodeList/Node");
		if(pNodeList)
		{
			ztc_t = GetXmlValue(pNodeList, "ztc", pos);
			gjc_t = GetXmlValue(pNodeList,"gjz",pos);
			zsd_t = GetXmlValue(pNodeList,"zsd",pos);
			pcc_t = GetXmlValue(pNodeList,"pcc",pos);
			xxc_t = GetXmlValue(pNodeList,"xxc",pos);
			zyc_t = GetXmlValue(pNodeList,"zyc",pos);
			m_school = GetXmlValue(pNodeList,"school",pos);

			GetAllKeys(ztc_t,1);
			GetAllKeys(gjc_t,2);
			GetAllKeys(zsd_t,3);
			GetAllKeys(pcc_t,4);
			GetAllKeys(xxc_t,5);
			GetAllKeys(zyc_t,6);
			pNodeList.Release();
		}
		pDoc.Release();
		SetGroupTitle();
		UpdateData(FALSE);
	}	
	catch(_com_error& e){}	
		catch(...){}
}

//added by zuoss 20080918
void CSetRGroupKeyWords::OnChangeEditSchool() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	all_school[school_num] = m_school;
}
//end add 
