// CreateReGroup.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "CreateReGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateReGroup dialog


CCreateReGroup::CCreateReGroup(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateReGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateReGroup)
	m_des = _T("");
	m_name = _T("");
	m_key1 = _T("");
	m_key2 = _T("");
	m_key3 = _T("");
	m_keywords = _T("");
	//}}AFX_DATA_INIT
}


void CCreateReGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateReGroup)
	DDX_Text(pDX, IDC_EDIT_DES, m_des);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_KEY1, m_key1);
	DDX_Text(pDX, IDC_KEY2, m_key2);
	DDX_Text(pDX, IDC_KEY3, m_key3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateReGroup, CDialog)
	//{{AFX_MSG_MAP(CCreateReGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateReGroup message handlers

void CCreateReGroup::OnOK() 
{
	// TODO: Add extra validation here
// 	int namelength = m_name.GetLength();
// 	if (namelength == 0 )
// 	{
// 		AfxMessageBox("群落名不可以为空！");
// 		return ;
//	}
	UpdateData(TRUE);
	if (m_key1.GetLength()>0)
	{
		m_keywords = m_keywords + m_key1;// + " " + m_key2 + " " + m_key3;
	}
	if (m_key2.GetLength()>0)
	{
		m_keywords = m_keywords +" "+m_key2;
	}
	if (m_key3.GetLength()>0)
	{
		m_keywords = m_keywords +" "+m_key3;
	}
		
	CDialog::OnOK();
}
