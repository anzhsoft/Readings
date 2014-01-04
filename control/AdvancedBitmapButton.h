#if !defined(AFX_ADVANCEDBITMAPBUTTON_H__3A4DE579_D81B_4057_8277_72F2DE636861__INCLUDED_)
#define AFX_ADVANCEDBITMAPBUTTON_H__3A4DE579_D81B_4057_8277_72F2DE636861__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvancedBitmapButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvancedBitmapButton window

class CAdvancedBitmapButton : public CButton
{
// Construction
public:
	CAdvancedBitmapButton();
	BOOL LoadBitmaps(UINT nIDBitmapResource, UINT nIDBitmapResourceSel = 0, UINT nIDBitmapResourceFocus = 0, UINT nIDBitmapResourceDisabled = 0);
	BOOL LoadBitmaps(LPCTSTR lpszBitmapResource, LPCTSTR lpszBitmapResourceSel = NULL, LPCTSTR lpszBitmapResourceFocus = NULL, LPCTSTR lpszBitmapResourceDisabled = NULL);
	void SetFocusCursor(UINT nIDCursor = 0);
	void AutoSize();
	BOOL IsNotCapture;

// Attributes
public:
	CBitmap m_bmpCommon, m_bmpSel, m_bmpFocus, m_bmpDisable;
	UINT ButtonType;


protected:
	
	HCURSOR hFocusCur;
	int m_nWidth, m_nHeight;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedBitmapButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdvancedBitmapButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdvancedBitmapButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDBITMAPBUTTON_H__3A4DE579_D81B_4057_8277_72F2DE636861__INCLUDED_)
