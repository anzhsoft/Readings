#if !defined(AFX_TREECTRLEX_H__9800A8EE_0E94_438E_AB79_C29200A0DF25__INCLUDED_)
#define AFX_TREECTRLEX_H__9800A8EE_0E94_438E_AB79_C29200A0DF25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx window

class CTreeCtrlEx : public CTreeCtrl
{
// Construction
public:
	CTreeCtrlEx();
	CMenu menu;

// Attributes
public:

// Operations
public:
	void SetTreeItemData(HTREEITEM hItem, BYTE type, char *ID, WORD Pages=0, DWORD Numbers=0);
	HTREEITEM CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Check(CString FilePath);
	void DeleteChildItems(HTREEITEM hItem);
	virtual ~CTreeCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlEx)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuitemCreaterg();
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMenuitemRefresh();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuCreateRegroup();
	afx_msg void OnMenuitemRickresource();
	afx_msg void OnMenuitemRickerinfo();
	afx_msg void OnAddRicker();
	afx_msg void OnDeleteRicker();
	afx_msg void OnMenuitemTomystudy();
	afx_msg void OnMenuitemAdduniversity();
	afx_msg void OnMenuitemAddhall();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	long rgid;
	HTREEITEM hCurItem;
protected:
	UINT          m_TimerTicks;      //处理滚动的定时器所经过的时间
	UINT          m_nScrollTimerID;  //处理滚动的定时器
	CPoint        m_HoverPoint;      //鼠标位置
	UINT          m_nHoverTimerID;   //鼠标敏感定时器
	DWORD         m_dwDragStart;     //按下鼠标左键那一刻的时间
	BOOL          m_bDragging;       //标识是否正在拖动过程中
	CImageList*   m_pDragImage;      //拖动时显示的图象列表
	HTREEITEM     m_hItemDragS;      //被拖动的标签
	HTREEITEM     m_hItemDragD;      //接受拖动的标签

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLEX_H__9800A8EE_0E94_438E_AB79_C29200A0DF25__INCLUDED_)
