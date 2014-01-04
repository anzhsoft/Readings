// DialogSMS.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "DialogSMS.h"
#include "base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSMS dialog


CDialogSMS::CDialogSMS(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSMS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSMS)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



void CDialogSMS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSMS)
	DDX_Control(pDX, IDC_SMS_RESOURCE, m_resource);
	DDX_Control(pDX, IDC_SMS_MESSAGE, m_message);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSMS, CDialog)
	//{{AFX_MSG_MAP(CDialogSMS)
	ON_BN_CLICKED(IDC_SMS_EXPLORER, OnSmsExplorer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSMS message handlers

void CDialogSMS::init(CString userName,CString name,CString requestResource,CString AspServer,CMainListView *mainListView,CString path)
{
	this->userName=userName;
	this->name=name;
	this->requestResource=requestResource;
	this->mainListView=mainListView;
	this->AspServer=AspServer;
	this->dstPath=path;
}

void CDialogSMS::OnOK() 
{
//	int length;
                                    
    m_message.GetWindowText(message);
    m_resource.GetWindowText(myResource);
     
	
    		
	CDialog::OnOK();
}

void CDialogSMS::OnSmsExplorer() 
{
	CFileDialog fileDialog(TRUE);
	if(IDOK==fileDialog.DoModal()){
		m_resource.SetWindowText(fileDialog.GetFileName());
	}else{
		m_resource.SetWindowText("");
	}
}
