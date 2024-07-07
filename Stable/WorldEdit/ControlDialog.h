#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CControlDialog dialog

class CControlDialog : public CDialog
{
	DECLARE_DYNAMIC(CControlDialog)

public:
	CControlDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CControlDialog();

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTROL };

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStnClickedMinimap();
	afx_msg void OnBnClickedButtonResetcamera();
	afx_msg void OnSelLight();
	afx_msg void OnUpdateLight();

	CListBox m_lightList;

};
