#pragma once
#include "afxwin.h"
#include "ZItemList.h"

// CItemDlg 대화 상자입니다.

class CItemDlg : public CDialog, public IUpdateListener
{
	DECLARE_DYNAMIC(CItemDlg)

public:
	CItemDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOOL };

	LPDIRECT3DSWAPCHAIN9 m_pSwapChain;
	LPDIRECT3DTEXTURE9 m_pTexThumbnail;

	// 어떤 아이템에서 카메라나 pos rot scale 세팅을 다른 아이템으로 복사하고 싶을 때를 위해서 저장해두는 변수들
	rvector m_vPos[2];
	rvector m_vRot[2];
	rvector m_vScale[2];
	CameraSetting m_cameraSetting;

	void UpdateItemList();

	virtual void UpdateSelection(int id);
	void MakeItemString(ZItem* pItem, CString& out_str);
	void ShowPreview(int id);
	bool EditBoxToVector(CEdit& ed, rvector& out);
	void VectorToEditBox(CEdit& ed, rvector& out);

	void InitSwapChain();
	void RenderPreview();
	void ReleaseSwapChain();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CListBox m_listbox;
	CStatic m_preview;
public:
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedBtnResetcam();
	afx_msg void OnEnKillfocusPosRotScaleEdit();
	afx_msg void OnEnKillfocusPosRotScale2Edit();
	
	CEdit m_edPos;
	CEdit m_edRot;
	CEdit m_edScale;

	CEdit m_edPos2;
	CEdit m_edRot2;
	CEdit m_edScale2;

	afx_msg void OnBnClickedBtnCopy();
	afx_msg void OnBnClickedBtnPaste();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
