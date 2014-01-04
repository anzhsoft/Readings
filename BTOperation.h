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
 * BT操作类
 * 包含BT下载上传的一些基本操作以及上传共享制作种子等
******************************/

class CBTOperation  
{
public:
	CBTOperation();
	virtual ~CBTOperation();

	/*打开下载服务程序*/
	static BOOL OpenServer();//打开下载服务进程

	/*与下载程序通信函数*/
	static void sendMessage(CWnd *pWnd,string msg);//发送消息
	static void sendShareMsg(CWnd *pWnd,string msg);//发送共享文件夹消息
	static void sendDownloadMsg(CWnd *pWnd,string msg);//发送下载消息

	/*下载文件,返回种子文件名*/
	static CString DownloadFile(CString torrentURL, CString DstPath);

	/*获取种子路径名*/
	static CString GetTorrentFloder();
};

#endif // !defined(AFX_BTOPERATION_H__880B33F8_D963_4F14_8BFC_1621C3A76D0A__INCLUDED_)
