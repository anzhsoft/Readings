// ParseTxtFile.h: interface for the CParseTxtFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARSETXTFILE_H__6E44FCF5_9F0B_4DA9_B759_569836300444__INCLUDED_)
#define AFX_PARSETXTFILE_H__6E44FCF5_9F0B_4DA9_B759_569836300444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CParseTxtFile  
{
public:
	CParseTxtFile();
	virtual ~CParseTxtFile();
	
	LPVOID p;

	void SetShowStyle(int style);
	int BeginParseFile(LPCSTR FileName);
	void ShowFile(CDC* pDC, DWORD pageNum);
	void Release();
	DWORD Total;
	DWORD len;
	static UINT _Paged(LPVOID pParam);
	CMap<DWORD,DWORD&,DWORD,DWORD&> map;
	BOOL close;
	BOOL m_bInitialing;
protected:
	char* g_pTxtBuf;
	CWinThread* m_pThread;
	int sysWidth, sysHeight, fontsize;
	int show_style;
	CString m_Title;
	void ShowPage(CDC* pDC, DWORD pageNum);
	void ShowCover(CDC* pDC);
	void InitialPage();
};

#endif // !defined(AFX_PARSETXTFILE_H__6E44FCF5_9F0B_4DA9_B759_569836300444__INCLUDED_)
