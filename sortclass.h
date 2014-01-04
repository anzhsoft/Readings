#if !defined(AFX_SORTCLASS_H__AC1B50C7_FC0E_11D4_B49C_0080C8E8A300__INCLUDED_)
#define AFX_SORTCLASS_H__AC1B50C7_FC0E_11D4_B49C_0080C8E8A300__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortClass window

class CSortClass
{
// Construction
public:
	CSortClass(CListCtrl * _pWnd, const int _iCol);
	virtual ~CSortClass();
	int iCol;	
	CListCtrl * pWnd;	
	void Sort(const bool bAsc,int _iType);
	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
public:	
	class CSortItem	
	{
	public:
		virtual  ~CSortItem();
		CSortItem(const DWORD _dw, const CString &_txt, const BOOL _check);
		CString txt;
		DWORD dw;
		BOOL check;
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTCLASS_H__AC1B50C7_FC0E_11D4_B49C_0080C8E8A300__INCLUDED_)
