
// ArticGUICompilerDlg.h : header file
//

#pragma once
#include <vector>
#include "MArticFileSystem.h"
// CArticGUICompilerDlg dialog
class CArticGUICompilerDlg : public CDialogEx
{
private:
	std::vector<LPCTSTR> folders;
	MArticFileList* m_fileList;
// Construction
public:
	CArticGUICompilerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARTICGUICOMPILER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CListBox DecompileList;
	CListBox CompileList;

	void FillListBoxes();
	void get_files(LPCTSTR const& path, LPCTSTR const& fileExt = "");

	afx_msg void OnBnClickedButton3();
};
