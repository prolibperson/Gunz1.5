#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class LightEditDlg : public CDialog
{
	DECLARE_DYNAMIC(LightEditDlg)

public:
	LightEditDlg(CString Lightname,CString LightPos,CString LightColor, CString LightIntensity,
		CString LightAttnStart, CString LightAttnEnd, CString castShadow);   // standard constructor
	virtual ~LightEditDlg();

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void UpdateData(BOOL update =1);

// Dialog Data
	enum { IDD = IDD_LIGHTEDIT_DLG };

protected:
	DECLARE_MESSAGE_MAP()

public:
	CString lightNameBox;
	CString lightPosition;
	CString lightColor;
	CString lightIntensity;
	CString lightAttnStart;
	CString lightAttnEnd;
	CString castShadow;

private:
	afx_msg void OnClickedOk();

	bool OnCheckUpdateLight();
	bool UpdateLightXML();
};
