#if !defined(AFX_PERSONINFO_H__585DA417_1A80_409B_8B8B_172AF7568B9C__INCLUDED_)
#define AFX_PERSONINFO_H__585DA417_1A80_409B_8B8B_172AF7568B9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PersonInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPersonInfo view

class CPersonInfo : public CView
{
public:
	CPersonInfo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPersonInfo)

// Attributes
public:

// Operations
public:
	void OnRefresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPersonInfo)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPersonInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPersonInfo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSONINFO_H__585DA417_1A80_409B_8B8B_172AF7568B9C__INCLUDED_)
