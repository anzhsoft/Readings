// UploadShareFileList.h: interface for the CUploadShareFileList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPLOADSHAREFILELIST_H__8461D471_CD98_4A9E_81C3_38D79791974C__INCLUDED_)
#define AFX_UPLOADSHAREFILELIST_H__8461D471_CD98_4A9E_81C3_38D79791974C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*****************************
 * create time : 2010 03 31
 * author : wangcong
 * description : 
 * 专门用于获得用户的共享文件信息
******************************/

class CUploadShareFileList  
{
public:
	CUploadShareFileList();
	virtual ~CUploadShareFileList();

	/*获取本地共享的文件的路径列表,静态方法,只是获取列表,不做上传动作*/
	static CList<CString,CString&>* PubDB(int filetype, LPCSTR iniPath, BOOL all);//获取某一类文件的所有文件路径列表
	static CList<CString,CString&>* FindFileInDir(char* Path);//获取某个目录中的所有文件列表
	static CList<CString,CString&>* GetShareFileList();//获取所有共享的文件的路径列表
	
};

#endif // !defined(AFX_UPLOADSHAREFILELIST_H__8461D471_CD98_4A9E_81C3_38D79791974C__INCLUDED_)
