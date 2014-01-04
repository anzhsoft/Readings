#if !defined(AFX_READDLG_H__14A33DD0_B15E_44AE_8861_5AD69F249AAF__INCLUDED_)
#define AFX_READDLG_H__14A33DD0_B15E_44AE_8861_5AD69F249AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReadDlg dialog
#include "ParseWolFile.h"
#include "ParseTxtFile.h"
#include "ReadControlBar.h"

class CReadDlg : public CDialog
{
// Construction
public:
	int decryption(char* strWolfFile, char * strId);
	CString tmpFilePath;
	int IsEncrypt(const char* strWolfFile,char* strId);
	int getStartPage(LPCSTR FilePath, int FileType);
	CReadDlg(CWnd* pParent = NULL);   // standard constructor
	~CReadDlg();

	CReadControlBar m_controlBar;
	CTreeCtrl m_BookMenu;

	BOOL m_bOpen;
	BOOL Open(LPCSTR FilePath);
	void Close();

	void ShowPage(DWORD pn);
	BOOL UpPage();
	BOOL DownPage();

	CParseWolFile* wolParser;
	CParseTxtFile* txtParser;
	    BOOL tmpWol;
// Dialog Data
	//{{AFX_DATA(CReadDlg)
	enum { IDD = IDD_DIALOG_READ };
	CButton	m_fc;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReadDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReadDlg)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnPageNumChange(WPARAM w,LPARAM l);
	afx_msg LRESULT OnCurPageNumChange(WPARAM w,LPARAM l);
	afx_msg LRESULT OnUpPage(WPARAM w,LPARAM l);
	afx_msg LRESULT OnDownPage(WPARAM w,LPARAM l);
	afx_msg LRESULT OnZoom(WPARAM w,LPARAM l);
	afx_msg LRESULT OnMenu(WPARAM w,LPARAM l);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//variable
protected:
	int VScrollPos;
	int HScrollPos;
	int FileType;
	long curPage;
	long totalPage;

	CDC memDC, bgDC;
	CBitmap bmp, bgbmp;

	CFont font;

//method
protected:
	//zoom
	int sysWidth, sysHeight, fontsize;
	int Zoom;
	//scroll
	void ScrollRight();
	void ScrollLeft();
	void SetScroll(int cx, int cy);
	void ScrollUp();
	void ScrollDown();
	//draw
	void ShowLogo();
	//other
	DWORD GetPageNum(DWORD offset, DWORD start);
	void CreateMenu();
	void SetShowStyle(int style);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READDLG_H__14A33DD0_B15E_44AE_8861_5AD69F249AAF__INCLUDED_)
