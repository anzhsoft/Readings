// BackupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "BackupDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackupDialog dialog


CBackupDialog::CBackupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBackupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBackupDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CBackupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackupDialog)
	DDX_Control(pDX, IDC_STATIC_TIME, m_tm);
	DDX_Control(pDX, IDC_STATIC_BACKUP, m_tip);
	DDX_Control(pDX, IDC_PROGRESS_BACKUP, m_progress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBackupDialog, CDialog)
	//{{AFX_MSG_MAP(CBackupDialog)
	ON_WM_TIMER()
	ON_MESSAGE(WM_FINISHED_DIALOG,OnFinished)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackupDialog message handlers

void CBackupDialog::PowerCopy(CString src, CString dst, BOOL &quit)
{
	_mkdir(dst);
	CFileFind find;
	src += "\\*";
	BOOL ext = find.FindFile(src);
	while(ext && !quit){
		ext = find.FindNextFile();
		if(find.IsDots())
			continue;
		if(find.IsDirectory()){
			CString newdst;
			newdst.Format("%s\\%s",dst,find.GetFileTitle());
			PowerCopy(find.GetFilePath(),newdst,quit);
			continue;
		}
		CString dstname;
		dstname.Format("%s\\%s",dst,find.GetFileName());
		CString tip;
		tip.Format("正在备份：%s", find.GetFilePath());
		m_tip.SetWindowText(find.GetFilePath());
		CopyFile(find.GetFilePath(),dstname,FALSE);
		finished += find.GetLength();
	}
	find.Close();
}

BOOL CBackupDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ULARGE_INTEGER lpfreetouse = {0};
	__int64 i64TotalBytes=0,i64FreeBytes=0;
	GetDiskFreeSpaceEx((LPCSTR)sourcePath,&lpfreetouse,
		(PULARGE_INTEGER)&i64TotalBytes,(PULARGE_INTEGER)&i64FreeBytes);

	used = i64TotalBytes-i64FreeBytes;
	finished = 0;
	m_progress.SetRange(0,100);
	m_progress.SetPos(0);

	th = AfxBeginThread(_Thread,this);
	th->m_bAutoDelete = TRUE;
	th->ResumeThread();

	SetTimer(1, 2000, NULL);

	q = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

UINT CBackupDialog::_Thread(LPVOID pParam)
{
	CBackupDialog* p = (CBackupDialog*)pParam;
	p->start = GetTickCount();
	p->PowerCopy(p->sourcePath,p->destPath, p->q);
	p->m_progress.SetPos(100);
	if(!p->q)
		AfxMessageBox("备份成功！");
	p->PostMessage(WM_FINISHED_DIALOG);
	return 0;
}


void CBackupDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1){
		static int count = 0;
		if(count%5==0){
			DWORD now = GetTickCount();
			if((now-start)/1000 == 0){
				m_tm.SetWindowText("正在计算剩余时间");
			}
			else{
				DWORD speed = finished/((now-start)/1000);
				if(speed==0)
					m_tm.SetWindowText("正在计算剩余时间");
				else{
					DWORD rest = (used-finished)/speed;
					CString str;
					if(rest>3600){
						str.Format("剩余 %d小时%d分%d秒", rest/3600, (rest%3600)/60, rest%60);
					}
					else if(rest>60){
						str.Format("剩余 %d分%d秒", rest/60, rest%60);
					}
					else{
						str.Format("剩余 %d秒", rest);
					}
					m_tm.SetWindowText(str);
				}
			}
		}
		count++;

		if(finished==0){
			m_progress.SetPos(0);
		}
		else{
			__int64 n = used/finished;
			if(n<1)
				n=1;
			double dn = n;
			dn = (1.0/dn)*100;
			m_progress.SetPos((int)dn);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CBackupDialog::OnOK() 
{
	th->SuspendThread();
	if(AfxMessageBox("是否取消备份？", MB_YESNO)==IDYES){
		q = TRUE;
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		KillTimer(1);
		m_tip.SetWindowText("正在停止,请稍候...");
	}
	th->ResumeThread();
}

LRESULT CBackupDialog::OnFinished(WPARAM w,LPARAM l)
{
	if(q)
		EndDialog(IDCANCEL);
	else
		EndDialog(IDOK);
	return TRUE;
}
