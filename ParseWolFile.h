#if !defined(AFX_PARSEWOLFILE_H__748CD992_5F21_4367_87A5_0BC5EA360B1E__INCLUDED_)
#define AFX_PARSEWOLFILE_H__748CD992_5F21_4367_87A5_0BC5EA360B1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParseWolFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParseWolFile dialog
typedef enum{
    BROWSER_FREE,
    BROWSER_FREE_ROTATE,//---------7.28
    BROWSER_SEQUENCE_N,
    BROWSER_SEQUENCE_NR,
    BROWSER_SEQUENCE_Z,
    BROWSER_SEQUENCE_N_ROTATE,
    BROWSER_SEQUENCE_NR_ROTATE,
    BROWSER_SEQUENCE_Z_ROTATE,
    BROWSER_DIR,
}BROWSERTYPE;

typedef struct voice_param{

         int curSpeaker;

         int curTone;

         int curSpeed;

         int curStyle;

         int curVolumn;

}voice_5_param;

struct PageCenter
{
  /************add by ljz**************/
  	int count;
	int TxtFlag;
 /*********************************/
	
	int DisplayLikeTxtFlag;
	//
	int iPageOffset;
	//
	int DoublePCState;
	double selx1, sely1, selw1, selh1;
	double selx2, sely2, selw2, selh2;
	//
	int wx, wy, wp, wpw, wph;
	//
	int iUserPCState;
	double dux, duy, duw, duh;
	//
	int pic_light;
	int pic_contrast;
	//
	voice_5_param v5ParamSave;
	//
	int		iMarginState;
	int		iBiggerLevelState;
    //page infomation
    int pageNo;
    //center position of the view box
    double XValue, YValue;
    //  double xstep_, ystep_;
    double CellWidth, CellHeight;
    //view box scale
    double BoxScale;
    //center position of the selector box
    double SXValue,SYValue;
    double SCellWidth,SCellHeight;
    //rotation
    int rotation;
    int fitMode;
    //browser info
    BROWSERTYPE type;
    int width;
    int height;
    int x,y;
    double selbox_x, selbox_y;
    double selbox_width, selbox_height;
    //bookmark
    int bookmark_page[3];
    int restoreLeftBar;
    //for user
    int UserDataLength;
    void * UserData;
    //magic
    unsigned int magic;
};

class CParseWolFile
{
// Construction
public:
	int show_style;
	int sysWidth, sysHeight, fontsize;

	BOOL m_bFileExist;
	BOOL GetInfo();	
	void ShowFile(CPoint point, CRect RangeRect, DWORD ShowPage, CDC *pDC);
	void DrawCover(CDC *pDC, CPoint point, CRect rect);
	BOOL Legal(LPCSTR FileName, DWORD &Len);
	char m_BookName[_MAX_PATH];
	void Release();
	CParseWolFile(CWnd* pParent = NULL);   // standard constructor
	int BeginParseFile(LPCSTR FileName);

	WolFileHeadInfo m_head;//文件头信息
	bool m_bFinish;			//解析是否完成
	_WOLFDECRYPT *pWolf;	//解密
	
	char FileName[_MAX_PATH];
	char* g_pWolBuf;
	DWORD Total;


public:
	char extension[4];
	char* pageCenterFilePath;
	CString getPageCenterFilePath(LPCSTR FileName);
	int getStartPage(LPCSTR FileName);
	void SetShowStyle(int style);
	int GetTagVal(char* buf, char* tag);
	BOOL DrawImage(CDC* pDC, char* buf, CPoint& p, int &len);
	void DrawText(CPoint &p, CDC* pDC, char*buf, int len);
	void Draw(CPoint point, CRect RangeRect, DWORD ShowPage, CDC* pDC);
	DWORD GetPagePos(int Page);

	//menu tree
	BOOL GetCataTreeWithSub(CTreeCtrl* pTree, HTREEITEM hItem);
	void CreateCataTree(CTreeCtrl* pTree);
	DWORD GetPageNum(DWORD offset, DWORD start);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARSEWOLFILE_H__748CD992_5F21_4367_87A5_0BC5EA360B1E__INCLUDED_)








