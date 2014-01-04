// BTOperation.h: interface for the CBTOperation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BTOPERATION_H__880B33F8_D963_4F14_8BFC_1621C3A76D0A__INCLUDED_)
#define AFX_BTOPERATION_H__880B33F8_D963_4F14_8BFC_1621C3A76D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*****************************
 * create time : 2010 04 07
 * author : wangcong
 * description : 
 * BT������
 * ����BT�����ϴ���һЩ���������Լ��ϴ������������ӵ�
******************************/

class CBTOperation  
{
public:
	CBTOperation();
	virtual ~CBTOperation();

	/*�����ط������*/
	static BOOL OpenServer();//�����ط������

	/*�����س���ͨ�ź���*/
	static void sendMessage(CWnd *pWnd,string msg);//������Ϣ
	static void sendShareMsg(CWnd *pWnd,string msg);//���͹����ļ�����Ϣ
	static void sendDownloadMsg(CWnd *pWnd,string msg);//����������Ϣ

	/*�����ļ�,���������ļ���*/
	static CString DownloadFile(CString torrentURL, CString DstPath);

	/*��ȡ����·����*/
	static CString GetTorrentFloder();
};

#endif // !defined(AFX_BTOPERATION_H__880B33F8_D963_4F14_8BFC_1621C3A76D0A__INCLUDED_)
