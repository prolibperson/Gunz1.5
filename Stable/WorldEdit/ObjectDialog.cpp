// ObjectDialog.cpp : implementation file
//


#include "stdafx.h"
#include "MZFileSystem.h"
#include "WorldEdit.h"
#include "ObjectDialog.h"
#include "FileInfo.h"
#include "MXml.h"
#include "WorldEditDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CObjectDialog dialog


CObjectDialog::CObjectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectDialog)
	DDX_Control(pDX, IDC_LIST1, m_ObjectList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectDialog, CDialog)
	//{{AFX_MSG_MAP(CObjectDialog)
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectDialog message handlers

void CObjectDialog::Initilize()
{
	//MXmlDocument doc;

	//MXmlDocument	Data;
	//Data.Create();

	//MZFile mzf;

	//std::string file = ((CWorldEditDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveView()->GetDocument())->FolderPath;
	//file.append("objects.xml");

	//if (!mzf.Open(file.c_str()))
	//{
	//	return;
	//}

	//char* buffer;
	//buffer = new char[mzf.GetLength() + 1];
	//mzf.Read(buffer, mzf.GetLength());
	//buffer[mzf.GetLength()] = 0;

	//if (!Data.LoadFromMemory(buffer))
	//{
	//	delete[] buffer;
	//	mlog("Error opening objects file\n");
	//	return;
	//}

	//delete[] buffer;
	//mzf.Close();

	//MXmlElement rootElement = Data.GetDocumentElement();
	//int childCount = rootElement.GetChildNodeCount();

	//for (int i = 0; i < childCount; ++i)
	//{
	//	MXmlElement child = rootElement.GetChildNode(i);
	//	char TagName[256];
	//	child.GetTagName(TagName);
	//	if (TagName[0] == '#')
	//	{
	//		continue;
	//	}

	//	if (strcmp(TagName, "OBJECT") == 0)
	//	{
	//		ZWorldObject worldObject;
	//		child.GetAttribute(&worldObject.name, "name");
	//		child.GetAttribute(&worldObject.model, "model");
	//		child.GetAttribute(&worldObject.collradius, "collradius", 0);
	//		child.GetAttribute(&worldObject.collwidth, "collwidth", 0);
	//		child.GetAttribute(&worldObject.collheight, "collheight", 0);
	//		child.GetAttribute(&worldObject.movable, "moving", false);
	//		child.GetAttribute(&worldObject.collidable, "collidable", false);
	//		child.GetAttribute(&worldObject.reverseanimation, "reverseanimation", false);
	//		child.GetAttribute(&worldObject.sound, "sound", "");
	//		child.GetAttribute(&worldObject.usepath, "usepath", false);

	//		int objectChildCount = child.GetChildNodeCount();
	//		for (int j = 0; j < objectChildCount; ++j)
	//		{
	//			MXmlElement subChild = child.GetChildNode(j);
	//			subChild.GetTagName(TagName);
	//			if (strcmp(TagName, "POSITION") == 0)
	//			{
	//				std::string contents;
	//				subChild.GetContents(&contents);
	//				rvector pos(0, 0, 0);
	//				sscanf(contents.c_str(), "%f,%f,%f", &worldObject.position.x, &worldObject.position.y, &worldObject.position.z);
	//			}
	//			if (strcmp(TagName, "DIRECTION") == 0)
	//			{
	//				std::string contents;
	//				subChild.GetContents(&contents);
	//				rvector pos(0, 1, 0);
	//				sscanf(contents.c_str(), "%f,%f,%f", &worldObject.direction.x, &worldObject.direction.y, &worldObject.direction.z);
	//			}
	//			if (strcmp(TagName, "SPEED") == 0)
	//			{
	//				std::string contents;
	//				subChild.GetContents(&contents);
	//				worldObject.speed = std::stof(contents);
	//			}
	//			if (strcmp(TagName, "ANIMATION") == 0)
	//			{
	//				std::string contents;
	//				subChild.GetContents(&contents);
	//				worldObject.animation = contents;
	//			}
	//			if (strcmp(TagName, "SCALE") == 0)
	//			{
	//				std::string contents;
	//				subChild.GetContents(&contents);
	//				sscanf(contents.c_str(), "%f,%f,%f", &worldObject.scale.x, &worldObject.scale.y, &worldObject.scale.z);
	//			}
	//			if (strcmp(TagName, "MINHEIGHT") == 0)
	//			{
	//				std::string contents;
	//				subChild.GetContents(&contents);
	//				worldObject.minheight = std::stoi(contents);
	//			}
	//			if (strcmp(TagName, "MAXHEIGHT") == 0)
	//			{
	//				std::string contents;
	//				subChild.GetContents(&contents);
	//				worldObject.maxheight = std::stoi(contents);
	//			}
	//			if (strcmp(TagName, "COLLISION") == 0)
	//			{
	//				std::string contents;
	//				subChild.GetContents(&contents);
	//				worldObject.collisiontype = static_cast<COLLTYPE>(std::stoi(contents));
	//			}
	//			if (strcmp(TagName, "ENDPOSITION") == 0)
	//			{
	//				std::string contents;
	//				rvector endpos(-1, -1, -1);
	//				subChild.GetContents(&contents);
	//				sscanf(contents.c_str(), "%f,%f,%f", &worldObject.endposition.x, &worldObject.endposition.y, &worldObject.endposition.z);
	//			}
	//		}
	//		std::unique_ptr<ZMapObject> mapObject = std::make_unique<ZMapObject>();
	//		mapObject->InitMesh(worldObject);
	//		mapObjects.push_back(std::move(mapObject));
	//	}
	//}
}