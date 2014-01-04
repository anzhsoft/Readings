// CommLogDlg.cpp : implementation file
//created by yangxiao 2008.8.20

#include "stdafx.h"
#include "Readings.h"
#include "CommLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern _ConnectionPtr m_pConnection;
extern char hostOutIp[20];
extern char hostInerIp[20]; //add by yangxiao 2008.8.22

extern enum COMMOPER;
/////////////////////////////////////////////////////////////////////////////
// CCommLogDlg dialog


CCommLogDlg::CCommLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT



}


void CCommLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommLogDlg)
	DDX_Control(pDX, IDC_TREE_OPER, m_treeOper);
	DDX_Control(pDX, IDC_LIST_PUBINFO, m_listPubInfo);
	DDX_Control(pDX, IDC_LIST_LOG, m_listCommLog);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CCommLogDlg, CDialog)
	//{{AFX_MSG_MAP(CCommLogDlg)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OPER, OnSelchangedTreeOper)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommLogDlg message handlers

void CCommLogDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
	;
}

int CCommLogDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CCommLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//��ʼ�������б�
	m_listCommLog.InsertColumn(0, _T("����"),  LVCFMT_LEFT, 80);	
	m_listCommLog.InsertColumn(1, _T("ʱ��"),  LVCFMT_LEFT, 80);
	m_listCommLog.InsertColumn(2, _T("����"),  LVCFMT_LEFT, 160);
	m_listCommLog.InsertColumn(3, _T("�����ֽ���"),  LVCFMT_LEFT, 100);
	m_listCommLog.InsertColumn(4, _T("�ϴ�/����"),  LVCFMT_LEFT, 80);
	m_listCommLog.SetExtendedStyle(LVS_EX_GRIDLINES);

	showLogAll();
	//showLogSel(3);

	//��ʼ��ȫ��ͳ���б�
	m_listPubInfo.InsertColumn(0, _T("��Ŀ"),  LVCFMT_LEFT, 150);	
	m_listPubInfo.InsertColumn(1, _T("ֵ"),  LVCFMT_LEFT, 300);
	
	/*
	_T("����IP"), _T("192.168.4.127"),
	_T("����IP"), _T("61.181.14.185"),
	_T("��������ַ"), _T("61.181.14.184"),
	_T("������URL"), _T("www.jinke.com.cn"),
	_T("Ӧ�ò�Э��"), _T("HttpЭ��"),
	_T("TCP�����˿�"), _T("8088"),
	_T("UDP�����˿�"), _T("8088"),
	_T("windows����ǽ"), _T("δ���"),
	_T("NAT�˿�ӳ��"), _T("�ȴ���ȡIP��ַ"),
	_T("�ϴ��ٶ�"), _T("0 KB/s"),
	_T("�����ٶ�"), _T("0 KB/s"),
	_T("��ǰ���������ڴ�"), _T("10 MB"),
	*/
	
	m_listPubInfo.InsertItem(0,"");
	m_listPubInfo.SetItemText(0,0,"�ڲ�IP");
	m_listPubInfo.SetItemText(0,1, hostInerIp);

	m_listPubInfo.InsertItem(1,"");
	m_listPubInfo.SetItemText(1,0,"����IP");
	m_listPubInfo.SetItemText(1,1,hostOutIp);

	m_listPubInfo.InsertItem(2,"");
	m_listPubInfo.SetItemText(2,0,"��������ַ");
	m_listPubInfo.SetItemText(2,1,"61.181.14.184");


	m_listPubInfo.InsertItem(3,"");
	m_listPubInfo.SetItemText(3,0,"�ܼ���������");
	
	long tmpupflow = countFlowAll(0);	
	char upflow[100] = {0};
	sprintf_s(upflow, 100, "%d KB",tmpupflow);
	m_listPubInfo.SetItemText(3,1,upflow);

	m_listPubInfo.InsertItem(4,"");
	m_listPubInfo.SetItemText(4,0,"�ܼ���������");	

	long tmpdownflow = countFlowAll(1);	
	char downflow[100] = {0};
	sprintf_s(downflow, 100, "%d KB",tmpdownflow);
	m_listPubInfo.SetItemText(4,1, downflow);

	

	//��ʼ�����ؼ�
	HTREEITEM hItem,hSubItemLevel1,hSubItemLevel2;
	
	hItem = m_treeOper.InsertItem("���ε�¼",TVI_ROOT); //�ڸ���������Parent1
	hSubItemLevel1 = m_treeOper.InsertItem("�û�����",hItem); //��Parent1�����һ���ӽ��
	hSubItemLevel2 = m_treeOper.InsertItem("�û���Ϣ׷��",hSubItemLevel1);
	hSubItemLevel2 = m_treeOper.InsertItem("�û���Ϣɾ��",hSubItemLevel1,hSubItemLevel2);

	hSubItemLevel1 = m_treeOper.InsertItem("����ģ��",hItem,hSubItemLevel1); 
	hSubItemLevel2 = m_treeOper.InsertItem("����������Ϣ",hSubItemLevel1);
	hSubItemLevel2 = m_treeOper.InsertItem("����������Ϣ",hSubItemLevel1,hSubItemLevel2);

	hSubItemLevel1 = m_treeOper.InsertItem("��������",hItem,hSubItemLevel1); 
	hSubItemLevel2 = m_treeOper.InsertItem("��������",hSubItemLevel1);
	hSubItemLevel2 = m_treeOper.InsertItem("�������ȴ�����",hSubItemLevel1,hSubItemLevel2);
	
	hSubItemLevel1 = m_treeOper.InsertItem("�����ȴ�",hItem,hSubItemLevel1);

	hSubItemLevel1 = m_treeOper.InsertItem("���Readman",hItem,hSubItemLevel1);
	
	hSubItemLevel1 = m_treeOper.InsertItem("�������",hItem,hSubItemLevel1);
	

	hItem = m_treeOper.InsertItem("��ʷ��¼",TVI_ROOT,hItem);    
	hItem = m_treeOper.InsertItem("����",TVI_ROOT,hItem);    



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommLogDlg::showLogAll()
{	
	m_listCommLog.DeleteAllItems();
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			throw 0;
			char SQLSEL[1024] = {0};
			sprintf_s(SQLSEL, 1024, "SELECT [operation],[filesize],[upDownFlag],[year],[month],[day],[hour],[minute],[second],[weekDay] FROM currLog ");
			hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
			int itemIndex = 0;
			while(!m_pRecordset->EndOfFile){
				CString operation = GetCollectString(m_pRecordset, "operation");
				long filesize = GetCollectLong(m_pRecordset, "filesize");
				
	
				BYTE upDownFlag = 0;
				upDownFlag = GetCollectByte(m_pRecordset, "upDownFlag");
				int year = 0;
				year =GetCollectInt(m_pRecordset, "year");
				BYTE month = 0;
				month = GetCollectByte(m_pRecordset, "month");
				BYTE day = 0;
				day = GetCollectByte(m_pRecordset, "day");
				BYTE hour = 0;
				hour = GetCollectByte(m_pRecordset, "hour");
				BYTE minute = 0;
				minute = GetCollectByte(m_pRecordset, "minute");
				BYTE second = 0;
				second = GetCollectByte(m_pRecordset, "second");
				BYTE weekDay = 0;
				weekDay = GetCollectByte(m_pRecordset, "weekDay");
				
				
				CString date;
				date.Format("%d-%d-%d",year,month,day);
				CString time;
				time.Format("%d:%d:%d",hour,minute,second);
				CString logOper;
				logOper.Format("%s",operation);
				CString logFileSize;
				logFileSize.Format("%d",filesize);
				CString upDown ;
				if (upDownFlag == 0)
				{
					upDown = "�ϴ�";
				}
				if (upDownFlag == 1)
				{
					upDown = "����";
				}

				
				m_listCommLog.InsertItem(itemIndex,"");
				m_listCommLog.SetItemText(itemIndex,0,date);
				m_listCommLog.SetItemText(itemIndex,1,time);
				m_listCommLog.SetItemText(itemIndex,2,logOper);
				m_listCommLog.SetItemText(itemIndex,3,logFileSize);
				m_listCommLog.SetItemText(itemIndex,4,upDown);
				

				m_pRecordset->MoveNext();
				itemIndex++;
			}
			
	}
	catch(_com_error &e){
			if(SUCCEEDED(hr)){
				if(m_pRecordset->State)
					m_pRecordset->Close();
				//m_pRecordset.Release();
			}
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();


}

void CCommLogDlg::OnSelchangedTreeOper(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CString nodeName;
    nodeName = m_treeOper.GetItemText(pNMTreeView->itemNew.hItem);
	
	//AfxMessageBox(nodeName);
	
	if (nodeName.Compare("���ε�¼")==0)
	{
		showLogAll();
	}
	else
	{
		if (nodeName.Compare("�û���Ϣ׷��")==0)
		{
			showLogSel(comm_oper_appendUser);
			countFlowSel(comm_oper_appendUser);
		}
		if (nodeName.Compare("�û���Ϣɾ��")==0)
		{
			showLogSel(comm_oper_delUser);
		}
		if (nodeName.Compare("�����ȴ�")==0)
		{
			showLogSel(comm_oper_tranHotKey);
		}
		if (nodeName.Compare("����������Ϣ")==0)
		{
			showLogSel(comm_oper_recvMsg);
		}
		if (nodeName.Compare("����������Ϣ")==0)
		{
			showLogSel(comm_oper_sentMSg);
		}
		if (nodeName.Compare("��������")==0)
		{
			showLogSel(comm_oper_leftTree_search);
		}
		if (nodeName.Compare("�������ȴ�����")==0)
		{
			showLogSel(comm_oper_search);
		}

	}
	*pResult = 0;
}

//��ʾѡ�е���־��
void CCommLogDlg::showLogSel(char operNumber)
{
	m_listCommLog.DeleteAllItems();
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			throw 0;
			char SQLSEL[1024] = {0};
			sprintf_s(SQLSEL, 1024, "SELECT [operation],[filesize],[upDownFlag],[year],[month],[day],[hour],[minute],[second],[weekDay] FROM currLog where operNum = %d ", operNumber);
			hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
			int itemIndex = 0;
			while(!m_pRecordset->EndOfFile){
				CString operation = GetCollectString(m_pRecordset, "operation");
				long filesize = GetCollectLong(m_pRecordset, "filesize");
				
	
				BYTE upDownFlag = 0;
				upDownFlag = GetCollectByte(m_pRecordset, "upDownFlag");
				int year = 0;
				year =GetCollectInt(m_pRecordset, "year");
				BYTE month = 0;
				month = GetCollectByte(m_pRecordset, "month");
				BYTE day = 0;
				day = GetCollectByte(m_pRecordset, "day");
				BYTE hour = 0;
				hour = GetCollectByte(m_pRecordset, "hour");
				BYTE minute = 0;
				minute = GetCollectByte(m_pRecordset, "minute");
				BYTE second = 0;
				second = GetCollectByte(m_pRecordset, "second");
				BYTE weekDay = 0;
				weekDay = GetCollectByte(m_pRecordset, "weekDay");
				
				
				CString date;
				date.Format("%d-%d-%d",year,month,day);
				CString time;
				time.Format("%d:%d:%d",hour,minute,second);
				CString logOper;
				logOper.Format("%s",operation);
				CString logFileSize;
				logFileSize.Format("%d",filesize);
				CString upDown ;
				if (upDownFlag == 0)
				{
					upDown = "�ϴ�";
				}
				if (upDownFlag == 1)
				{
					upDown = "����";
				}

				
				m_listCommLog.InsertItem(itemIndex,"");
				m_listCommLog.SetItemText(itemIndex,0,date);
				m_listCommLog.SetItemText(itemIndex,1,time);
				m_listCommLog.SetItemText(itemIndex,2,logOper);
				m_listCommLog.SetItemText(itemIndex,3,logFileSize);
				m_listCommLog.SetItemText(itemIndex,4,upDown);
				

				m_pRecordset->MoveNext();
				itemIndex++;
			}
			
	}
	catch(_com_error &e){
			if(SUCCEEDED(hr)){
				if(m_pRecordset->State)
					m_pRecordset->Close();
				//m_pRecordset.Release();
			}
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();


	

}

long CCommLogDlg::countFlowSel(char operNumber)
{
	long flowSel = 0;
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			throw 0;
			char SQLSEL[1024] = {0};
			sprintf_s(SQLSEL, 1024, "SELECT sum(filesize) as flow FROM currLog where operNum = %d ", operNumber);
			hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
			int itemIndex = 0;
			if(!m_pRecordset->EndOfFile)
			{
				flowSel = GetCollectMyLong(m_pRecordset, "flow");
				return flowSel;
			}
			
	}
	catch(_com_error &e){
			if(SUCCEEDED(hr)){
				if(m_pRecordset->State)
					m_pRecordset->Close();
				m_pRecordset.Release();
			}
			return 0;
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();

	return 0;
}

long CCommLogDlg::countFlowAll(char upDown)
{
	long flowAll = 0;
	_RecordsetPtr m_pRecordset;
	HRESULT hr;
	try{
		hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(!SUCCEEDED(hr))
			throw 0;
			char SQLSEL[1024] = {0};
			sprintf_s(SQLSEL, 1024, "SELECT sum([filesize]) as [flow] FROM currLog where upDownFlag = %d ", upDown);
			hr = m_pRecordset->Open(SQLSEL, m_pConnection.GetInterfacePtr(),
					adOpenStatic, adLockOptimistic, adCmdText);
			int itemIndex = 0;
			if(!m_pRecordset->EndOfFile)
			{
				flowAll = GetCollectMyLong(m_pRecordset, "flow");
			}
			
	}
	catch(_com_error &e){
			if(SUCCEEDED(hr)){
				if(m_pRecordset->State)
					m_pRecordset->Close();
				m_pRecordset.Release();
			}
			return 0;
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
	return flowAll/1024;
}
