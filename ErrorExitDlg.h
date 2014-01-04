#pragma once


// CErrorExitDlg dialog

class CErrorExitDlg : public CDialog
{
	DECLARE_DYNAMIC(CErrorExitDlg)

public:
	CErrorExitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CErrorExitDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_Error_Exit };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
