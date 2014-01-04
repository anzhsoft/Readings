// Readings.h : main header file for the READINGS application
//

#if !defined(AFX_READINGS_H__C7006B66_CAF5_4FFB_B8D3_C2437BBCA63C__INCLUDED_)
#define AFX_READINGS_H__C7006B66_CAF5_4FFB_B8D3_C2437BBCA63C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CReadingsApp:
// See Readings.cpp for the implementation of this class
//
// added by zhanganzhan on 20100419 for UnHandledException.
LONG WINAPI RDSExceptionFun(_EXCEPTION_POINTERS * rdsexception);
class CReadingsApp : public CWinApp
{
public:
	BOOL RegisterActiveX();
	BOOL CheckNewVersion();
	void RemoveTempFiles();
	BOOL InitialSocket();
	CReadingsApp();
protected:
	BOOL ConnectToDB();
private:
	BOOL MakeSemaph();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReadingsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CReadingsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_READINGS_H__C7006B66_CAF5_4FFB_B8D3_C2437BBCA63C__INCLUDED_)
