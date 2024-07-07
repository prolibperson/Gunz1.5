#include "StdAfx.h"
#include "WorldEdit.h"
#include "LightEditDlg.h"
#include "RBspObject.h"
#include "WorldEditDoc.h"
#include "MainFrm.h"
#include "MXml.h"

IMPLEMENT_DYNAMIC(LightEditDlg, CDialog)
LightEditDlg::LightEditDlg(CString lightName,CString LightPos,CString LightColor,CString LightIntensity,CString LightAttenStart,
	CString LightAttenEnd,CString CastShadow)
	: CDialog(LightEditDlg::IDD, nullptr)
{
	lightNameBox = lightName;// = lightName;// / = lightName;
	lightPosition = LightPos;
	lightColor = LightColor;
	lightIntensity = LightIntensity;
	lightAttnStart = LightAttenStart;
	lightAttnEnd = LightAttenEnd;
	castShadow = CastShadow;
}

LightEditDlg::~LightEditDlg()
{
}

BEGIN_MESSAGE_MAP(LightEditDlg, CDialog)
	//ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK,OnClickedOk)
END_MESSAGE_MAP()

void LightEditDlg::UpdateData(BOOL bUpdateData)
{
	/* Construct CDataExchange */
	CDataExchange DX(this, bUpdateData);

	/* Call DDX with the object */
	this->DoDataExchange(&DX);
}

void LightEditDlg::DoDataExchange(CDataExchange* ex)
{
	CDialog::DoDataExchange(ex);

	DDX_Text(ex, IDC_LIGHTEDIT, lightNameBox);
	DDX_Text(ex, IDC_LIGHTEDIT_POS, lightPosition);
	DDX_Text(ex, IDC_LIGHTEDIT_COLOR, lightColor);
	DDX_Text(ex, IDC_LIGHTEDIT_INTENSITY, lightIntensity);

}

void LightEditDlg::OnDraw(CDC* pDC)
{
	UpdateData();
}

void LightEditDlg::OnClickedOk()
{

	if (OnCheckUpdateLight())
	{
		bool result = UpdateLightXML();
		if (result == false)
		{
			MessageBox("Error updateing light data in xml", "ERROR");
		}
	}

	OnOK();
}

bool LightEditDlg::OnCheckUpdateLight()
{
	UpdateData();
	CWnd* pMainframe = AfxGetMainWnd();

	bool needsUpdate = false;

	RBspObject* bspObj = ((CWorldEditDoc*)((CMainFrame*)pMainframe)->GetActiveView()->GetDocument())->m_pBspObject;
	if (bspObj == nullptr)
	{
		return false;
	}

	RLIGHT* light = bspObj->GetMapLightList()->Find(lightNameBox.GetString());
	if (light == nullptr)
	{
		return false;
	}

	char tmp1[256] = "", tmp2[256] = "", tmp3[256] = "";
	sscanf(lightPosition.GetString(), "%s : %s : %s", tmp1, tmp2, tmp3);
	//TODO: update and save xml file
	D3DXVECTOR3 pos = D3DXVECTOR3(atof(tmp1), atof(tmp2), atof(tmp3));
	if (pos != light->Position)
	{
		light->Position = pos;

		needsUpdate = true;
	}

	sscanf(lightColor.GetString(), "%s : %s : %s", tmp1, tmp2, tmp3);
	D3DXVECTOR3 color = D3DXVECTOR3(atof(tmp1), atof(tmp2), atof(tmp3));
	if (color != light->Color)
	{
		light->Color = color;
		needsUpdate = true;
	}

	if (light->fIntensity != atof(lightIntensity.GetString()))
	{
		light->fIntensity = atof(lightIntensity.GetString());

		needsUpdate = true;
	}

	//if (light->fAttnStart != atof(lightAttnStart.GetString()))
	//{
	//	light->fAttnStart = atof(lightAttnStart.GetString());

	//	needsUpdate = true;
	//}

	//if (light->fAttnEnd != atof(lightAttnEnd.GetString()))
	//{
	//	light->fAttnEnd = atof(lightAttnEnd.GetString());

	//	needsUpdate = true;
	//}

	//if (stricmp(castShadow.GetString(), "false") == 0)
	//{
	//	if (light->dwFlags & RM_FLAG_CASTSHADOW)
	//	{
	//		light->dwFlags &= ~RM_FLAG_CASTSHADOW;
	//	}

	//	needsUpdate = true;
	//}
	//else if (stricmp(castShadow.GetString(), "true") == 0)
	//{
	//	if ((light->dwFlags & RM_FLAG_CASTSHADOW) == 0)
	//	{
	//		light->dwFlags |= RM_FLAG_CASTSHADOW;
	//	}
	//	needsUpdate = true;
	//}

	return needsUpdate;
}

#include <algorithm>

bool LightEditDlg::UpdateLightXML()
{
	CWnd* pMainframe = AfxGetMainWnd();

	RBspObject* bspObj = ((CWorldEditDoc*)((CMainFrame*)pMainframe)->GetActiveView()->GetDocument())->m_pBspObject;
	MZFile mzf;
	if (!mzf.Open(bspObj->m_descfilename.c_str()))
		return false;


	char* buffer;
	buffer = new char[mzf.GetLength() + 1];
	mzf.Read(buffer, mzf.GetLength());
	buffer[mzf.GetLength()] = 0;

	MXmlDocument aXml;
	aXml.Create();
	if (!aXml.LoadFromMemory(buffer))
	{
		delete[] buffer;
		return false;
	}

	MXmlElement rootElement = aXml.GetDocumentElement();
	MXmlElement lightListElement;

	bool result = rootElement.FindChildNode("LIGHTLIST", &lightListElement);
	if (result == true)
	{
		int childNodeCount = lightListElement.GetChildNodeCount();
		for (int i = 0; i < childNodeCount; ++i)
		{
			MXmlElement lightElement = lightListElement.GetChildNode(i);
			char tagName[256];
			lightElement.GetTagName(tagName);

			if (stricmp(tagName, "LIGHT") == 0)
			{

				char name[128];
				lightElement.GetAttribute(name, "name");

				if (stricmp(name, lightNameBox.GetString()) == 0)
				{
					MXmlElement posElem;
					if (lightElement.FindChildNode("POSITION", &posElem))
					{
						string outText = lightPosition.GetString();
						outText.erase(remove(outText.begin(), outText.end(), ':'), outText.end());
						posElem.SetText(outText.c_str());
					}

					MXmlElement colorElemn;
					if (lightElement.FindChildNode("COLOR", &colorElemn))
					{
						string outText = lightColor.GetString();
						outText.erase(remove(outText.begin(), outText.end(), ':'), outText.end());
						colorElemn.SetText(outText.c_str());
					}
				}
			}
		}
	}

	aXml.SaveToFile(bspObj->m_descfilename.c_str());

	delete[] buffer;
	aXml.Destroy();
	return true;
}