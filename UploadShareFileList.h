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
 * ר�����ڻ���û��Ĺ����ļ���Ϣ
******************************/

class CUploadShareFileList  
{
public:
	CUploadShareFileList();
	virtual ~CUploadShareFileList();

	/*��ȡ���ع�����ļ���·���б�,��̬����,ֻ�ǻ�ȡ�б�,�����ϴ�����*/
	static CList<CString,CString&>* PubDB(int filetype, LPCSTR iniPath, BOOL all);//��ȡĳһ���ļ��������ļ�·���б�
	static CList<CString,CString&>* FindFileInDir(char* Path);//��ȡĳ��Ŀ¼�е������ļ��б�
	static CList<CString,CString&>* GetShareFileList();//��ȡ���й�����ļ���·���б�
	
};

#endif // !defined(AFX_UPLOADSHAREFILELIST_H__8461D471_CD98_4A9E_81C3_38D79791974C__INCLUDED_)
