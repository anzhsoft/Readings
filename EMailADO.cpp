// EMailADO.cpp: implementation of the CEMailADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "readings.h"
#include "EMailADO.h"
#include "MainFrm.h"
#include "UploadShareFileList.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern _ConnectionPtr m_pConnection;
extern char CurrentPath[_MAX_PATH];
extern CMainFrame* pMainWnd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEMailADO::CEMailADO()
{

}

CEMailADO::~CEMailADO()
{

}

bool CEMailADO::InsertMail(CString Server, CString MailID, CString UserName, CString MailType, CString MailFrom)
{
	//CString TmpPath = CString(CurrentPath) + "\\temp\\UpDataMail.tmp";
	CString WebPath = Server + "/share/addmail.asp?messageid=" + MailID + "&username=" + UserName + "&messagetype=" + MailType + "&messagefrom=" + MailFrom;
	if(!UpLoad(WebPath)) 
	{
		Tips("写邮件ID失败！");
		return false;
	} 
	return true;
}

bool CEMailADO::DeleteMail(CString Server, CString MailID)
{
	//CString TmpPath = CString(CurrentPath) + "\\temp\\UpDataMail.tmp";
	CString WebPath = Server + "/share/deletemail.asp?messageid=" + MailID;
	if(!UpLoad(WebPath))
	{
		Tips("删除邮件ID失败！");
		return false;
	} 
	return true;
}

bool CEMailADO::GetAllReadedMail(CString Server, CString UserName)
{
	CString TmpPath = CString(CurrentPath) + "\\ReadedMail.txt";
	CString WebPath = Server + "/share/getreadedmail.asp?username=" + UserName;
	if(!DownLoad(WebPath, TmpPath, pMainWnd->m_quit, pMainWnd->m_socketList)) 
	{
		return false;
	} 
//	AfxMessageBox(TmpPath);
	return true;
}

string CEMailADO::FindFilePath(string file)
{
	//先从共享xml中查找
	int i = 0;
	string TheWholePath;
	CList<CString,CString&>* sharefloderlist = CUploadShareFileList::GetShareFileList();
	POSITION pos = sharefloderlist->GetHeadPosition();
	while (pos)
	{
		CString filename;
		filename.Format("%s\\%s", sharefloderlist->GetNext(pos), file.c_str());
		if (PathFileExists(filename))
		{
			return string(filename.GetBuffer(0));
		}
	}
	/*while (TheWholePath.length() == 0)
	{
		char xmlcount[25]; 
		itoa(i, xmlcount, 10);
		CString dstPath = CString(CurrentPath) + "\\system\\sharefile\\sharefile_" + CString(xmlcount) + ".xml";
		i ++;
		CFile tempfile1;
		if(tempfile1.Open(dstPath,CFile::modeRead) == 0)
		{
			break;
		}else{
			tempfile1.Close();
		}
		CString filepath;
		
		int pos =0;
		IXMLDOMDocumentPtr pDoc;
		IXMLDOMNodeListPtr pNodeList;
		HRESULT hr1 =  pDoc.CreateInstance(__uuidof(DOMDocument));
		if(!SUCCEEDED(hr1))
		{
			AfxMessageBox("数据集打开不成功!");
			return "";
		}
		if(!pDoc->load((LPCSTR)dstPath))
		{
			AfxMessageBox("共享资源列表打开失败!");
			return "";	//xml 解析失败
		}
		pNodeList = pDoc->selectNodes("items/item");
		if(pNodeList)
		{		
			for(pos=0; pos<pNodeList->length ; pos++)
			{
				filepath = GetXmlValue(pNodeList, "FileName", pos);
				string path = (char *)(LPCSTR)filepath;
				string filename = path.substr(path.find_last_of('\\') + 1 ,file.length() - file.find_last_of('\\') - 1);
				if (filename.compare(file) == 0)
				{
					CFile tempfile;
					if(tempfile.Open(path.c_str(),CFile::modeRead) == 0)
					{
						continue;
					}else
					{
						tempfile.Close();
						TheWholePath = path;
						break;
					}
				}
			}
		}
	}
	//AfxMessageBox(TheWholePath.c_str());
	if (TheWholePath.length() != 0)
	{
		return TheWholePath;
	}
	

	//未找到则去数据库中查询历史记录
	//把文件名和扩展名分别提取出来，文件名在进行数据库查找时用，扩展名在查找得到的结果中查找被请求文件而且是发送时的参数
	string ExName = file.substr(file.find_last_of('.') + 1 ,file.length() - file.find_last_of('.') - 1);
	string FileName = file.substr(0 ,file.length() - ExName.length() - 1);
	HRESULT hr;
	_RecordsetPtr m_pRecordset;
	hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
	if(!SUCCEEDED(hr))	//数据集打开不成功
	{
		AfxMessageBox("数据集打开不成功!");
		return "";
	}
	char SQL[1000]={0};
	sprintf(SQL,"SELECT [addr] FROM loc WHERE title=\"%s\"", FileName.c_str());
	m_pRecordset->Open(SQL, m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
		adOpenKeyset, adLockOptimistic, adCmdText);
	m_pConnection->Execute(SQL,NULL,adCmdText); 
	while(!m_pRecordset->EndOfFile)
	{
		char * wholepath = new char[_MAX_PATH+1];
		strcpy(wholepath,GetCollectString(m_pRecordset,"addr"));
		string MyWholePath = wholepath;
		string MyExName = MyWholePath.substr(MyWholePath.find_last_of('.') + 1 ,MyWholePath.length() - MyWholePath.find_last_of('.') - 1);
		if(MyExName.compare(ExName) == 0)
		{
			CFile tempfile;
			if(tempfile.Open(MyWholePath.c_str(),CFile::modeRead) == 0)
			{
				m_pRecordset->MoveNext();
				continue;
			}else
			{
				tempfile.Close();
				TheWholePath = MyWholePath;
				break;
			}
		}
		m_pRecordset->MoveNext();
		delete [] wholepath;
	}
	m_pRecordset->Close();
//	m_pRecordset.Release();*/
	return TheWholePath;
}

string CEMailADO::GetSavePath(string path)
{
	string savepath = path;
	CFile tempfile;
	while(tempfile.Open(savepath.c_str(),CFile::modeRead) != 0)
	{
		string ExName = savepath.substr(savepath.find_last_of('.') + 1 ,savepath.length() - savepath.find_last_of('.') - 1);
		string FileName = savepath.substr(0 ,savepath.length() - ExName.length() - 1);
		savepath = FileName + "(1)." + ExName;
		tempfile.Close();
	}
	//tempfile.Close();
	return savepath;
}
