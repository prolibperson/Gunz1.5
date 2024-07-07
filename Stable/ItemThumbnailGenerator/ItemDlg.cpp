// ItemDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ItemThumbnailGenerator.h"
#include "ItemDlg.h"
#include ".\itemdlg.h"
#include "ZItemList.h"
#include "Camera.h"
#include "Thumbnail.h"
#include "MainDoc.h"
#include "MainView.h"

void ResetCamera();
extern ZItemList g_itemlist;
extern CTCamera g_camera;

// CItemDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemDlg, CDialog)
CItemDlg::CItemDlg(CWnd* pParent /*=NULL*/)
: CDialog(CItemDlg::IDD, pParent)
{
	m_pSwapChain = NULL;
	m_pTexThumbnail = NULL;
	GetUpdateNotifier()->Register(this);
}

CItemDlg::~CItemDlg()
{
}

void CItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_listbox);
	DDX_Control(pDX, IDC_PICTURE, m_preview);
	DDX_Control(pDX, IDC_ED_POS, m_edPos);
	DDX_Control(pDX, IDC_ED_ROT, m_edRot);
	DDX_Control(pDX, IDC_ED_SCALE, m_edScale);
	DDX_Control(pDX, IDC_ED_POS2, m_edPos2);
	DDX_Control(pDX, IDC_ED_ROT2, m_edRot2);
	DDX_Control(pDX, IDC_ED_SCALE2, m_edScale2);
}


BEGIN_MESSAGE_MAP(CItemDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST, OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BTN_RESETCAM, OnBnClickedBtnResetcam)
	ON_EN_KILLFOCUS(IDC_ED_POS, OnEnKillfocusPosRotScaleEdit)
	ON_EN_KILLFOCUS(IDC_ED_ROT, OnEnKillfocusPosRotScaleEdit)
	ON_EN_KILLFOCUS(IDC_ED_SCALE, OnEnKillfocusPosRotScaleEdit)
	ON_EN_KILLFOCUS(IDC_ED_POS2, OnEnKillfocusPosRotScale2Edit)
	ON_EN_KILLFOCUS(IDC_ED_ROT2, OnEnKillfocusPosRotScale2Edit)
	ON_EN_KILLFOCUS(IDC_ED_SCALE2, OnEnKillfocusPosRotScale2Edit)
	ON_BN_CLICKED(IDC_BTN_COPY, OnBnClickedBtnCopy)
	ON_BN_CLICKED(IDC_BTN_PASTE, OnBnClickedBtnPaste)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CItemDlg 메시지 처리기입니다.

void CItemDlg::OnBnClickedOk()
{
	//	OnOK();
}

void CItemDlg::OnBnClickedCancel()
{
	//	OnCancel();
}

void CItemDlg::MakeItemString(ZItem* pItem, CString& out_str)
{
	if (!pItem) {
		out_str = "";
		return;
	}
	if (pItem->m_strType == MICTOK_TYPE_AVATAR)
	{
		out_str.Format("%s (%d) AVATAR:%s", pItem->m_strMeshName.c_str(), pItem->m_nID, pItem->m_strAvatarHeadMeshName.c_str());
	}
	else
	{
		out_str.Format("%s (%d)", pItem->m_strMeshName.c_str(), pItem->m_nID);
	}
}

void CItemDlg::UpdateItemList()
{
	m_listbox.ResetContent();

	CString strItem;
	ZItemList::ItorItem it;
	for (it=g_itemlist.m_mapItem.begin(); it!=g_itemlist.m_mapItem.end(); ++it)
	{
		int id = it->first;
		ZItem* pItem = it->second;
		MakeItemString(pItem, strItem);
		int itemIdx = m_listbox.AddString(strItem);
		m_listbox.SetItemData(itemIdx, (DWORD)id);
	}
}

void CItemDlg::OnLbnSelchangeList()
{
	int idx = m_listbox.GetCurSel();

	CString str;
	m_listbox.GetText(idx, str);

	int itemId = (int)m_listbox.GetItemData(idx);
	if (itemId != 0)
		GetUpdateNotifier()->NotifyListeners(itemId);

	/*
	int itemId = 0;
	sscanf((LPCTSTR)str, "%d", &itemId);
	if (itemId != 0)
		GetUpdateNotifier()->NotifyListeners(itemId);
		*/
}

void CItemDlg::UpdateSelection(int id)
{
	ZItem* pItem = g_itemlist.GetZItem(id);
	if (!pItem) return;

	CString strItem;
	MakeItemString(pItem, strItem);
	m_listbox.SelectString(-1, strItem);

	ShowPreview(id);

	VectorToEditBox(m_edPos, pItem->m_vPos);
	VectorToEditBox(m_edRot, pItem->m_vRot);
	VectorToEditBox(m_edScale, pItem->m_vScale);

	VectorToEditBox(m_edPos2, pItem->m_vPos2);
	VectorToEditBox(m_edRot2, pItem->m_vRot2);
	VectorToEditBox(m_edScale2, pItem->m_vScale2);
}

void CItemDlg::ShowPreview(int id)
{
	ZItem* pItem = g_itemlist.GetZItem(id);
	if (!pItem) return;

	CString strPath;
	MakeThumbnailFilename(id, strPath);

	SAFE_RELEASE(m_pTexThumbnail);
	D3DXCreateTextureFromFile(RGetDevice(), (LPCTSTR)strPath, &m_pTexThumbnail);

	// bmp로 저장했을 때 쓰던 코드
	/*HBITMAP hbmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), (LPCTSTR)strPath,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbmp == NULL)
	{
		CClientDC dc(&m_preview);
		CRect rc;
		m_preview.GetClientRect(&rc);
		dc.Rectangle(&rc);
	}
	else
	{
		CBitmap bmp;
		bmp.Attach(hbmp);

		CClientDC dc(&m_preview);
		CDC bmDC;
		bmDC.CreateCompatibleDC(&dc);
		CBitmap* pOldbmp = bmDC.SelectObject(&bmp);

		BITMAP bi;
		bmp.GetBitmap(&bi);

		dc.BitBlt(0, 0, bi.bmWidth, bi.bmHeight, &bmDC, 0, 0, SRCCOPY);

		bmDC.SelectObject(pOldbmp);
	}

	DeleteObject(hbmp);*/
}

void CItemDlg::RenderPreview()
{
	if (!m_pSwapChain) return;

	LPDIRECT3DSURFACE9 pBackBuffer(NULL);
	HRESULT hr = m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	if (hr==D3D_OK)
	{
		RGetDevice()->SetRenderTarget(0, pBackBuffer);
		RGetDevice()->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,0xFFFFFFFF,1.0f ,0);

		RGetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		RGetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		RGetDevice()->SetTexture(0, m_pTexThumbnail);
		RGetDevice()->SetTexture(1, NULL);

		RGetDevice()->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		struct VERT { float x, y, z, rhw; DWORD diffuse; float u, v; };
		VERT vb[4] = {
			{0,				0,				0, 1, 0xffffffff, 0, 0},
			{THUMBNAILSIZE,	0,				0, 1, 0xffffffff, 1, 0},
			{0,				THUMBNAILSIZE,	0, 1, 0xffffffff, 0, 1},
			{THUMBNAILSIZE,	THUMBNAILSIZE,	0, 1, 0xffffffff, 1, 1}
		};

		hr = RGetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vb, sizeof(VERT));

		hr = RGetDevice()->EndScene();
		m_pSwapChain->Present(0, 0, m_preview.GetSafeHwnd(), 0, 0);
		hr = RGetDevice()->BeginScene();
		SAFE_RELEASE(pBackBuffer);
	}
}

void CItemDlg::OnBnClickedBtnResetcam()
{
	ResetCamera();
}

void CItemDlg::OnEnKillfocusPosRotScaleEdit()
{
	ZItem* pItem = g_itemlist.GetCurrZItem();
	if (!pItem) return;
	
	EditBoxToVector(m_edPos, pItem->m_vPos);
	EditBoxToVector(m_edRot, pItem->m_vRot);
	EditBoxToVector(m_edScale, pItem->m_vScale);
}

void CItemDlg::OnEnKillfocusPosRotScale2Edit()
{
	ZItem* pItem = g_itemlist.GetCurrZItem();
	if (!pItem) return;

	EditBoxToVector(m_edPos2, pItem->m_vPos2);
	EditBoxToVector(m_edRot2, pItem->m_vRot2);
	EditBoxToVector(m_edScale2, pItem->m_vScale2);
}

bool CItemDlg::EditBoxToVector(CEdit& ed, rvector& out)
{
	rvector temp;
	CString str;
	ed.GetWindowText(str);
	if (3 == sscanf((LPCTSTR)str, "%f, %f, %f", &temp.x, &temp.y, &temp.z))
	{
		out = temp;
		return true;
	}
	else
	{
		// 형식이 올바르지 않다면 0으로 초기화
		out = rvector(0,0,0);
		ed.SetWindowText("0,0,0");
		return false;
	}
	return true;
}

void CItemDlg::VectorToEditBox(CEdit& ed, rvector& out)
{
	CString str;
	str.Format("%.1f, %.1f, %.1f", out.x, out.y, out.z);
	ed.SetWindowText(str);;
}

void CItemDlg::OnBnClickedBtnCopy()
{
	// 현재 아이템에서 카메라나 pos rot scale 세팅을 복사해둔다
	ZItem* pItem = g_itemlist.GetCurrZItem();
	if (!pItem) return;

	m_vPos[0] = pItem->m_vPos;
	m_vPos[1] = pItem->m_vPos2;
	m_vRot[0] = pItem->m_vRot;
	m_vRot[1] = pItem->m_vRot2;
	m_vScale[0] = pItem->m_vScale;
	m_vScale[1] = pItem->m_vScale2;

	// 아이템의 카메라 세팅이 아니라, 현재 카메라 상태를 저장한다
	g_camera.GetCameraSetting(m_cameraSetting);
}

void CItemDlg::OnBnClickedBtnPaste()
{
	// 예전에 복사해둔 카메라, pos rot scale 세팅을 덮어씌운다
	ZItem* pItem = g_itemlist.GetCurrZItem();
	if (!pItem) return;

	if(!m_cameraSetting.bInitialized) return;

	pItem->m_vPos = m_vPos[0];
	pItem->m_vPos2 = m_vPos[1];
	pItem->m_vRot = m_vRot[0];
	pItem->m_vRot2 = m_vRot[1];
	pItem->m_vScale = m_vScale[0];
	pItem->m_vScale2 = m_vScale[1];

	pItem->m_cameraSetting = m_cameraSetting;

	GetUpdateNotifier()->NotifyListeners(pItem->m_nID);
}

BOOL CItemDlg::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CItemDlg::OnDestroy()
{
	__super::OnDestroy();
}

void CItemDlg::InitSwapChain()
{
	// 프리뷰 창에 스왑체인을 만들어준다
	CRect rc;
	m_preview.GetClientRect(&rc);
	D3DPRESENT_PARAMETERS d3dpp = g_d3dpp;
	d3dpp.BackBufferWidth = rc.Width();
	d3dpp.BackBufferHeight = rc.Height();
	RGetDevice()->CreateAdditionalSwapChain(&d3dpp, &m_pSwapChain);
}

void CItemDlg::ReleaseSwapChain()
{
	SAFE_RELEASE(m_pTexThumbnail);
	SAFE_RELEASE(m_pSwapChain);
}

BOOL CItemDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			g_pView->OnReturnKey();
		}
	}

	return __super::PreTranslateMessage(pMsg);
}
