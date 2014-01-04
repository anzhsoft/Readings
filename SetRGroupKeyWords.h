#if !defined(AFX_SETRGROUPKEYWORDS_H__01112254_C601_42F2_8D14_F0DD38D1241F__INCLUDED_)
#define AFX_SETRGROUPKEYWORDS_H__01112254_C601_42F2_8D14_F0DD38D1241F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRGroupKeyWords.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRGroupKeyWords dialog
const int keyNum = 5; //added by zuoss 20080813
                      //可以设定的主题词的个数（也可用于“关键词”，“知识点”等）
const int maxSchoolNum = 20;//added by zuoss 20080918 设置最多学校数
class CSetRGroupKeyWords : public CDialog
{
// Construction
public:
	void ShowTagRecord(int id, int flag, CString uname);
	CSetRGroupKeyWords(CWnd* pParent = NULL);   // standard constructor
	CSetRGroupKeyWords(CString keywords,CWnd* pParent = NULL);
	CSetRGroupKeyWords(CString keywords,int id, int flag, CString username,CWnd* pParent = NULL);
	void GetWikiInfo(int id,int flag);
	void GetAllKeys(CString ztc,int flag);
	void UpdateAllKeyWords();
	void SetGroupTitle();
	void ShowHistory();
public:
	
	int Flag;
	long TreeID;
	CString m_keywords;
	CString m_key[keyNum];//added by zuoss 20080813
	CString m_gjc[keyNum];
	CString m_zsd[keyNum];
	CString m_pcc[keyNum];
	CString m_xxc[keyNum];
	CString m_zyc[keyNum];
	int     m_curnum;//added by zuoss 2080904存储当前历史号
	int  	latestNum;//存储最新历史号	
	CString all_school[maxSchoolNum];//added by zuoss 20080918 存储学校，最大学校数可设
	int     school_num;//added by zuoss 20080918 存储实际已设置的学校数
// Dialog Data
	//{{AFX_DATA(CSetRGroupKeyWords)
	enum { IDD = IDD_DIALOG_RGROUPKEYWORD };
	CString	m_key1;
	CString	m_key2;
	CString	m_key3;
	CString	m_okeywords;
	CString	m_record;
	CString	m_ztc1;
	CString	m_ztc2;
	CString	m_ztc3;
	CString	m_xxc1;
	CString	m_xxc2;
	CString	m_xxc3;
	CString	m_zsd1;
	CString	m_zsd2;
	CString	m_zsd3;
	CString	m_zyc1;
	CString	m_zyc2;
	CString	m_zyc3;
	CString	m_pcc1;
	CString	m_pcc2;
	CString	m_pcc3;
	CString	m_gjc1;
	CString	m_gjc2;
	CString	m_gjc3;
	CString	m_treename;
	CString	m_static_gjc1;
	CString	m_static_gjc2;
	CString	m_static_gjc3;
	CString	m_static_zsd1;
	CString	m_static_zsd2;
	CString	m_static_zsd3;
	CString	m_school;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRGroupKeyWords)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRGroupKeyWords)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonZtcUp();
	afx_msg void OnButtonZtcDown();
	afx_msg void OnChangeEditKey1();
	afx_msg void OnChangeEditKey2();
	afx_msg void OnChangeEditKey3();
	afx_msg void OnButtonGjcDown();
	afx_msg void OnButtonGjcUp();
	afx_msg void OnButtonZsdDown();
	afx_msg void OnButtonZsdUp();
	afx_msg void OnChangeEditGjc1();
	afx_msg void OnChangeEditGjc2();
	afx_msg void OnChangeEditGjc3();
	afx_msg void OnChangeEditZsd1();
	afx_msg void OnChangeEditZsd2();
	afx_msg void OnChangeEditZsd3();
	afx_msg void OnHistoryUp();
	afx_msg void OnHistoryDown();
	afx_msg void OnChangeEditSchool();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETRGROUPKEYWORDS_H__01112254_C601_42F2_8D14_F0DD38D1241F__INCLUDED_)
