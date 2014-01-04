// SYNDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Readings.h"
#include "SYNDialog.h"
#include "SynListDlg.h"
#include "SelPathDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char SDDisk[20];
extern char FavorPath[_MAX_PATH];
extern char CurrentPath[_MAX_PATH];
extern _ConnectionPtr m_pConnection;
extern USB_DETAIL_INFO uDiskAuthInfo;

/////////////////////////////////////////////////////////////////////////////
// CSYNDialog dialog

CSYNDialog::CSYNDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSYNDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSYNDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSYNDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSYNDialog)
	DDX_Control(pDX, IDC_PROGRESS_SYN, m_ctrPro);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSYNDialog, CDialog)
	//{{AFX_MSG_MAP(CSYNDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSYNDialog message handlers

BOOL CSYNDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ctrPro.SetRange(0, 0);
	
	if(dire==PC2SD){
		CWinThread* th = AfxBeginThread(_SynToSD, this, THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
		th->m_bAutoDelete = TRUE;
		th->ResumeThread();
	}
	else{//SD2PC
		CWinThread* th = AfxBeginThread(_SynToPC, this, THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
		th->m_bAutoDelete = TRUE;
		th->ResumeThread();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

UINT CSYNDialog::SynToPC()
{
	try{
		if(strlen(SDDisk)==0)
			throw 0;

		list.RemoveAll();
		CList<CString,CString&> nls;
		CString findPath;
		findPath.Format("%s", SDDisk);
		Search(findPath,"其它");
	
		if(list.GetCount()==0){
			PostMessage(WM_CLOSE);
			Tips("SD卡中不存在待同步的文件!");
			return 0;
		}

		CSynListDlg dlg;
		dlg.m_synlist = &list;
		if(dlg.DoModal()!=IDOK){
			list.RemoveAll();
			PostMessage(WM_CLOSE);
			return 0;
		}

		m_ctrPro.SetRange(0,list.GetCount());
		POSITION pos = list.GetHeadPosition();
		int lp=0;
		char DstFolder[_MAX_PATH] = {0};
		CSelPathDlg sdlg;
		sdlg.defSel = 3;
		if(sdlg.DoModal()==IDOK){
			strcpy_s(DstFolder, _MAX_PATH, sdlg.strPath);
		}
		else{
			list.RemoveAll();
			PostMessage(WM_CLOSE);
			return 0;
		}
		while(pos){
			Sleep(500);
			m_ctrPro.SetPos(++lp);
			LOCALLIST locdata = list.GetNext(pos);
			if(!PathFileExists(locdata.FilePath))
				continue;
			char SrcPath[1024] = {0};
			char DstPath[_MAX_PATH] = {0};
			strcpy_s(SrcPath, 1024, locdata.FilePath);
			sprintf_s(DstPath, _MAX_PATH, "%s\\%s", DstFolder, locdata.Title);
			CopyFile(locdata.FilePath, DstPath, FALSE);//实体
			//证书
			if(locdata.FileType==wolFileType){
				SrcPath[strlen(SrcPath)-3] = 'c';
				SrcPath[strlen(SrcPath)-2] = 'i';
				SrcPath[strlen(SrcPath)-1] = 'f';

				DstPath[strlen(DstPath)-3] = 'c';
				DstPath[strlen(DstPath)-2] = 'i';
				DstPath[strlen(DstPath)-1] = 'f';
				CopyFile(SrcPath, DstPath, FALSE);
			}
		}
	}
	catch(int err){};
	PostMessage(WM_CLOSE);
	return 0;
}

UINT CSYNDialog::SynToSD()
{
	try{
		if(strlen(SDDisk)==0)
			throw 0;
		if (!(uDiskAuthInfo.flag &&uDiskAuthInfo.diskFlag == SDDisk[0])&&FALSE)
		{
			Tips("未找到合法的手持阅读器设备，请确认后重新同步",td_msg,6);
			PostMessage(WM_CLOSE);
			return 0;
		}

		char removePath[_MAX_PATH] = {0};
		sprintf_s(removePath, _MAX_PATH, "%s\\sys", SDDisk);
		_mkdir(removePath);
		strcat_s(removePath, _MAX_PATH, "\\Readings.sys");
		BOOL ext = FALSE;
		char* sysbuf=NULL;
		long size;
		FILE* rmfp;
		if(PathFileExists(removePath)){
			ext = TRUE;
			rmfp = fopen(removePath, "rb");
			if(!rmfp)
				throw 2;
			fseek(rmfp, 0, 2);
			size = ftell(rmfp);
			fseek(rmfp, 0, 0);
			sysbuf = (char*)malloc(size);
			fread(sysbuf, 1, size, rmfp);
			fclose(rmfp);
			DeleteFile(removePath);
		}

		CList<CString,CString&> nls;
		CString findPath;
		findPath = FavorPath;
		//findPath.Format("%s\\temp\\%s", CurrentPath, UserName);
		Search(findPath,"其它");
		SearchDB();
			
		if(list.GetCount()==0){
			if(sysbuf)
				free(sysbuf);
			Tips("本地不存在待同步的文件!");
			PostMessage(WM_CLOSE);
			return 0;
		}

		CSynListDlg dlg;
		dlg.m_synlist = &list;
		if(dlg.DoModal()!=IDOK){
			if(sysbuf)
				free(sysbuf);
			list.RemoveAll();
			PostMessage(WM_CLOSE);
			return 0;
		}
		m_ctrPro.SetRange(0,list.GetCount());
		POSITION pos = list.GetHeadPosition();
		int lp=0;
		rmfp = fopen(removePath, "wb");
	//	AfxMessageBox(removePath);
		if(!rmfp){
			if(sysbuf)
				free(sysbuf);
			list.RemoveAll();
			AfxMessageBox("文件同步失败，请检查SD卡是否处于写保护状态！");
			PostMessage(WM_CLOSE);
			return 0;
		}
			
		while(pos){
			m_ctrPro.SetPos(++lp);

			LOCALLIST locdata = list.GetNext(pos);
			READINGS_SYS sys = {0};

			if(!PathFileExists(locdata.FilePath))
				continue;
			char SrcPath[1024] = {0};
			char DstPath[_MAX_PATH] = {0};
			char ExtPath[_MAX_PATH] = {0};

			if(_stricmp(locdata.Type, "News") == 0
				&& strstr(locdata.Title, "Readman")){
				strcpy_s(SrcPath, 1024, locdata.FilePath);
				sprintf_s(ExtPath, _MAX_PATH, "%s\\Readman",SDDisk);
				if(!PathFileExists(ExtPath))
					_mkdir(ExtPath);
		//		strcat(ExtPath, "\\News");
		//		if(!PathFileExists(ExtPath))
		//			_mkdir(ExtPath);

				sprintf_s(DstPath, _MAX_PATH, "%s\\Readman\\%s",SDDisk, locdata.Title);
				CopyFile(locdata.FilePath, DstPath, FALSE);//实体
				//add by dongzhenhua 20080605 删除对应的评注文件
				CString CommentFileName = locdata.Title;
				CommentFileName.Replace('.','@');
				char CommentFilePath[_MAX_PATH] = {0};
				sprintf_s(CommentFilePath, _MAX_PATH, "%s\\Readman\\Comments\\@Readman@News@%s",SDDisk,CommentFileName);
				if (PathFileExists(CommentFilePath)){
					/*CString rtfloc;
					rtfloc.Format("%s\\Readman\\Comments\\@记事@notepad@pdf\\*.png", SDDisk);
					CFileFind findrtf;
					BOOL Ext = findrtf.FindFile(rtfloc);
					while(Ext && !quit){
						Ext = findrtf.FindNextFile();
						CString rtfpath;
						rtfpath.Format("%s\\手写文件", FavorPath, UserName);
						_mkdir(rtfpath);
						rtfpath.Format("%s\\手写文件\\记事文件", FavorPath, UserName);
						_mkdir(rtfpath);
						rtfpath.Format("%s\\手写文件\\记事文件\\%s", FavorPath, findrtf.GetFileName());
						CString pathtemp = findrtf.GetFilePath();
						CopyFile(pathtemp, rtfpath, FALSE);
					}
					findrtf.Close();*/
					CString CommentFiles;
					CommentFiles.Format("%s\\*",CommentFilePath);
					CFileFind findfile;
					BOOL Ext = findfile.FindFile(CommentFiles);
					while (Ext)
					{
						Ext = findfile.FindNextFile();
						DeleteFile(findfile.GetFilePath());
					}
				}
				//删除该批注目录
				_rmdir(CommentFilePath);
						
				FILE* clrfp;
				DstPath[strlen(DstPath)-3] = 'c';
				DstPath[strlen(DstPath)-2] = 'l';
				DstPath[strlen(DstPath)-1] = 'r';
				clrfp = fopen(DstPath, "wb");
				if(clrfp){
					fclose(clrfp);
				}
				//证书
				if(locdata.FileType==wolFileType){
					SrcPath[strlen(SrcPath)-3] = 'c';
					SrcPath[strlen(SrcPath)-2] = 'i';
					SrcPath[strlen(SrcPath)-1] = 'f';

					DstPath[strlen(DstPath)-3] = 'c';
					DstPath[strlen(DstPath)-2] = 'i';
					DstPath[strlen(DstPath)-1] = 'f';
					CopyFile(SrcPath, DstPath, FALSE);
				}
				sys.FileType = locdata.FileType;
				strcpy_s(sys.Title, 256, locdata.Title);
				sprintf_s(sys.Path, 1024, "Readman/%s", locdata.Title);
			}
			else{
				strcpy_s(SrcPath, 1024, locdata.FilePath);
				sprintf_s(ExtPath, _MAX_PATH, "%s\\%s",SDDisk, locdata.Type);
		//		sprintf(DstPath, "%s\\%s\\%s",SDDisk, locdata.Type, locdata.Title);
				sprintf_s(DstPath, _MAX_PATH, "%s\\Readings同步中心",SDDisk);
				if(!PathFileExists(DstPath))
					_mkdir(DstPath);
				sprintf_s(DstPath, _MAX_PATH, "%s\\Readings同步中心\\%s",SDDisk, locdata.Title);
				//			Tips(DstPath,td_alert,3);
				
				CopyFile(locdata.FilePath, DstPath, FALSE);//实体

				//证书
				if(locdata.FileType==wolFileType){
					SrcPath[strlen(SrcPath)-3] = 'c';
					SrcPath[strlen(SrcPath)-2] = 'i';
					SrcPath[strlen(SrcPath)-1] = 'f';

					DstPath[strlen(DstPath)-3] = 'c';
					DstPath[strlen(DstPath)-2] = 'i';
					DstPath[strlen(DstPath)-1] = 'f';
					CopyFile(SrcPath, DstPath, FALSE);
				}
			
				sys.FileType = locdata.FileType;
				strcpy_s(sys.Title, 256, locdata.Title);
			//	sprintf(sys.Path, "%s/%s", locdata.Type, locdata.Title);
				sprintf_s(sys.Path, 1024, "Readings同步中心\\%s",locdata.Title);
			}
			CString str = locdata.Title;
			if(!nls.Find(str)){
				fwrite(&sys, 1, sizeof(READINGS_SYS), rmfp);
				nls.AddTail(str);
			}
		}

		if(ext){
			for(int count=0;count<min(size/sizeof(READINGS_SYS),0xff-list.GetCount());count++){
				READINGS_SYS* tmp = (READINGS_SYS*)(sysbuf+sizeof(READINGS_SYS)*count);
				CString str = tmp->Title; 
				if(!nls.Find(str)){
					fwrite(tmp, 1, sizeof(READINGS_SYS), rmfp);
					nls.AddTail(str);
				}
			}
		}
		fclose(rmfp);
		if(ext)
			free(sysbuf);
	}
	catch(int err){};

	Tips("同步完成", td_tips);
	PostMessage(WM_CLOSE);
	
	return 0;
}

UINT CSYNDialog::_SynToPC(LPVOID pParam)//同步
{
	CSYNDialog* p = (CSYNDialog*)pParam;
	return p->SynToPC();
}

UINT CSYNDialog::_SynToSD(LPVOID pParam)//同步
{
	CSYNDialog* p = (CSYNDialog*)pParam;
	return p->SynToSD();
}

void CSYNDialog::Search(CString findPath,CString title)
{
	findPath += "\\*.*";
	CFileFind find;
	BOOL Ext = find.FindFile(findPath);
	while(Ext){
		Ext = find.FindNextFile();
		if(find.IsDots())
			continue;
		if(find.IsDirectory()){
			Search((LPCTSTR)find.GetFilePath(),find.GetFileTitle());
			continue;
		}
		if(ExistSYN(find.GetFilePath())){
			CString path = find.GetFilePath();
			LOCALLIST ls = {0};
			strcpy_s(ls.Title, 256, find.GetFileName());
			strcpy_s(ls.FilePath, 1024, path);
			strcpy_s(ls.Type, 20, title);	
			ls.FileType = GetFileTypeFromExt(GetExtNameFromFull(path));
			list.AddHead(ls);
			DeleteSYN(path);
		}
	}
	find.Close();
}

void CSYNDialog::SearchDB()
{
	_RecordsetPtr m_pRecordset;
	char SQL[1000]={0};
	try{
		strcpy_s(SQL, 1000, "SELECT addr,title,type,SynFlag FROM loc WHERE SynFlag=1");
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
			adOpenStatic, adLockOptimistic, adCmdText);
		int count = m_pRecordset->GetRecordCount();
		for(int index=0; index<count; index++){
			if(index!=0)
				m_pRecordset->MoveNext();
			LOCALLIST ls= {0};
			CString path = GetCollectString(m_pRecordset,"addr");
			strcpy_s(ls.Title, 256, GetCollectString(m_pRecordset,"title"));
			strcpy_s(ls.FilePath, 1024, path);
			strcpy_s(ls.Type, 20, GetExtNameFromFull(path));
			ls.FileType = (int)GetCollectLong(m_pRecordset,"type");
			list.AddHead(ls);
			m_pRecordset->PutCollect("SynFlag",(_variant_t)(0l));
			m_pRecordset->Update();
		}
		m_pRecordset->Close();
	//	m_pRecordset.Release();
	}
	catch(_com_error& e){}
}

void CSYNDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CSYNDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}
