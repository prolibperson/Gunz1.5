
// MRSCompilerToolDlg.h : header file
//

#pragma once
#include "MZip.h"


// CMRSCompilerToolDlg dialog
class CMRSCompilerToolDlg : public CDialogEx
{
// Construction
public:
	CMRSCompilerToolDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MRSCOMPILERTOOL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void FillListBoxes();

// Implementation
protected:
	HICON m_hIcon;
	FFileList zipFileList;
	FFileList mrsFileList;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_DecompileList;
	CListBox m_CompileList;
	CButton m_decompileButton;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
