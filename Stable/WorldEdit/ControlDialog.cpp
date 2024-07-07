// ControlDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEdit.h"
#include "ControlDialog.h"
#include "MainFrm.h"
#include "WorldEditDoc.h"
#include "WorldEditView.h"

#include "RealSpace2.h"
#include <afxrich.h>

#include "LightEditDlg.h"
// CControlDialog dialog

IMPLEMENT_DYNAMIC(CControlDialog, CDialog)
CControlDialog::CControlDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CControlDialog::IDD, pParent)
{
}

CControlDialog::~CControlDialog()
{
}

void CControlDialog::DoDataExchange(CDataExchange* ex)
{
	CDialog::DoDataExchange(ex);

	DDX_Control(ex, IDC_LIST2, m_lightList);

}
void CControlDialog::OnDraw(CDC* pDC)
{

}

BEGIN_MESSAGE_MAP(CControlDialog, CDialog)
//ON_WM_HSCROLL()
ON_STN_CLICKED(ID_MINIMAP, OnStnClickedMinimap)
ON_BN_CLICKED(IDC_BUTTON_RESETCAMERA, OnBnClickedButtonResetcamera)
ON_LBN_SELCHANGE(IDC_LIST2,OnSelLight)
ON_LBN_DBLCLK(IDC_LIST2,OnUpdateLight)
END_MESSAGE_MAP()

void CControlDialog::OnStnClickedMinimap()
{
	//// TODO: Add your control notification handler code here
	//RBspObject* map = ((CWorldEditView*)((CMainFrame*)GetParent())->GetActiveView())->GetDocument()->m_pBspObject;
	//if (map)
	//{
	//	for (RLightList::iterator itor = map->GetMapLightList()->begin(); itor != map->GetMapLightList()->end(); ++itor)
	//	{
	//		this->lis
	//	}
	//}
}

//BOOL CControlDialog::OnCommand(WPARAM wParam, LPARAM lParam)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDialog::OnCommand(wParam, lParam);
//}

void CControlDialog::OnBnClickedButtonResetcamera()
{
	// TODO: Add your control notification handler code here
	((CWorldEditView*)((CMainFrame*)GetParent())->GetActiveView())->OnResetCamera();
}

void CControlDialog::OnSelLight()
{
	CString targetLight;
	m_lightList.GetText(m_lightList.GetCurSel(), targetLight);

	RBspObject* bspObj = ((CWorldEditDoc*)((CMainFrame*)GetParent())->GetActiveView()->GetDocument())->m_pBspObject;
	if (bspObj == nullptr)
	{
		return;
	}

	RLIGHT* light = bspObj->GetMapLightList()->Find(targetLight.GetString());
	if (light == nullptr)
	{
		return;
	}

	((CWorldEditView*)((CMainFrame*)GetParent())->GetActiveView())->OnResetCamera(light->Position);
}

void CControlDialog::OnUpdateLight()
{
	CString targetLight;
	m_lightList.GetText(m_lightList.GetCurSel(), targetLight);

	RBspObject* bspObj = ((CWorldEditDoc*)((CMainFrame*)GetParent())->GetActiveView()->GetDocument())->m_pBspObject;
	if (bspObj == nullptr)
	{
		return;
	}

	RLIGHT* light = bspObj->GetMapLightList()->Find(targetLight.GetString());
	if (light == nullptr)
	{
		return;
	}

	string lightPos = "";
	lightPos.append(to_string(light->Position.x)+" : ");
	lightPos.append(to_string(light->Position.y)+" : ");
	lightPos.append(to_string(light->Position.z));

	string lightColor = "";
	lightColor.append(to_string(light->Color.x) +" : ");
	lightColor.append(to_string(light->Color.y) +" : ");
	lightColor.append(to_string(light->Color.z));

	LightEditDlg lightEdit(CString(light->Name.c_str()), CString(lightPos.c_str()), CString(lightColor.c_str()),
		CString(to_string(light->fIntensity).c_str()), CString(to_string(light->fAttnStart).c_str()),
		CString(to_string(light->fAttnEnd).c_str()), CString(light->dwFlags & RM_FLAG_CASTSHADOW ? "true" : "false"));
	lightEdit.DoModal();
}