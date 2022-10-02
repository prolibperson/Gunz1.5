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
#include <RealSpace2.h>

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

void CObjectDialog::RenderMapObjects()
{
}

void CObjectDialog::UpdateMapObjects(float delta)
{
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
	MXmlDocument doc;

	MXmlDocument	Data;
	Data.Create();

	MZFile mzf;

	std::string file = ((CWorldEditDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveView()->GetDocument())->FolderPath;
	file.append("objects.xml");

	if (!mzf.Open(file.c_str()))
	{
		return;
	}

	char* buffer;
	buffer = new char[mzf.GetLength() + 1];
	mzf.Read(buffer, mzf.GetLength());
	buffer[mzf.GetLength()] = 0;

	if (!Data.LoadFromMemory(buffer))
	{
		delete[] buffer;
		mlog("Error opening objects file\n");
		return;
	}

	delete[] buffer;
	mzf.Close();

	MXmlElement rootElement = Data.GetDocumentElement();
	int childCount = rootElement.GetChildNodeCount();

	for (int i = 0; i < childCount; ++i)
	{
		MXmlElement child = rootElement.GetChildNode(i);
		char TagName[256];
		child.GetTagName(TagName);
		if (TagName[0] == '#')
		{
			continue;
		}

		if (strcmp(TagName, "OBJECT") == 0)
		{
			WorldObject worldObject;
			child.GetAttribute(&worldObject.name, "name");
			child.GetAttribute(&worldObject.model, "model");
			child.GetAttribute(&worldObject.collradius, "collradius", 0);
			child.GetAttribute(&worldObject.collwidth, "collwidth", 0);
			child.GetAttribute(&worldObject.collheight, "collheight", 0);
			child.GetAttribute(&worldObject.collidable, "collidable", false);
			child.GetAttribute(&worldObject.reverseanimation, "reverseanimation", false);
			child.GetAttribute(&worldObject.sound, "sound", "");

			bool moving = false;
			bool usepath = false;
			child.GetAttribute(&moving, "moving", false);
			child.GetAttribute(&usepath, "usepath", false);

			int objectChildCount = child.GetChildNodeCount();
			for (int j = 0; j < objectChildCount; ++j)
			{
				MXmlElement subChild = child.GetChildNode(j);
				subChild.GetTagName(TagName);
				if (strcmp(TagName, "POSITION") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					rvector pos(0, 0, 0);
					sscanf(contents.c_str(), "%f,%f,%f", &worldObject.position.x, &worldObject.position.y, &worldObject.position.z);
				}
				if (strcmp(TagName, "DIRECTION") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					rvector pos(0, 1, 0);
					sscanf(contents.c_str(), "%f,%f,%f", &worldObject.direction.x, &worldObject.direction.y, &worldObject.direction.z);
				}
				if (strcmp(TagName, "SPEED") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					worldObject.speed = std::stof(contents);
				}
				if (strcmp(TagName, "ANIMATION") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					worldObject.animation = contents;
				}
				if (strcmp(TagName, "SCALE") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					sscanf(contents.c_str(), "%f,%f,%f", &worldObject.scale.x, &worldObject.scale.y, &worldObject.scale.z);
				}
				if (strcmp(TagName, "COLLISION") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					worldObject.collisiontype = static_cast<COLLTYPE>(std::stoi(contents));
				}
				if (strcmp(TagName, "ENDPOSITION") == 0)
				{
					std::string contents;
					rvector endpos(-1, -1, -1);
					subChild.GetContents(&contents);
					sscanf(contents.c_str(), "%f,%f,%f", &worldObject.endposition.x, &worldObject.endposition.y, &worldObject.endposition.z);
				}
			}
			std::unique_ptr<ZWorldObject> mapObject = std::make_unique<ZWorldObject>();
			mapObject->InitWithMesh(worldObject);
			m_ObjectList.AddString(worldObject.name.c_str());

			mapObjects.push_back(std::move(mapObject));
		}
	}
}

MapObjectCollision::MapObjectCollision()
{
	//TODO: make use of collisiontype
	CollisionType = CT_CYLINDER;
	Height = 0;
	Width = 0;
	Radius = 0;
	Collidable = false;
}

MapObjectCollision::~MapObjectCollision()
{

}

//todo: fetch information from a xml for the map
ZWorldObject::ZWorldObject() noexcept
{
	VisualMesh = nullptr;
}

ZWorldObject::~ZWorldObject() noexcept
{
	//	ZGetNpcMeshMgr()->DelAll();
		//ZGetMeshMgr()->Del((char*)Name.c_str());
}

//todo: init mesh differently, currently using npc mesh manager...
bool ZWorldObject::InitWithMesh(WorldObject const& worldObj)
{
	Name = worldObj.name;
	Model = worldObj.model;

	return true;
	//char szMapPath[64] = "";
	//ZGetCurrMapPath(szMapPath);

	//std::string folderpath = ((CWorldEditDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveView()->GetDocument())->FolderPath;

	//char szBuf[256];

	//sprintf(szBuf, "%s%s/", szMapPath, ZGetGameClient()->GetMatchStageSetting()->GetMapName());
	//std::string meshpath = szBuf;
	//meshpath.append(Model);

	//RMesh* pMesh = ZGetMeshMgr()->Get((char*)worldObj.name.c_str());

	//if (pMesh == nullptr)
	//{
	//	ZGetMeshMgr()->Add((char*)meshpath.c_str(), (char*)worldObj.name.c_str(), false, true);
	//}


	//pMesh = ZGetMeshMgr()->Get((char*)worldObj.name.c_str());
	//if (pMesh == nullptr)
	//	return false;

	//pMesh->ReloadAnimation();

	//int nVMID = m_VisualMeshMgr.Add(pMesh);
	//if (nVMID == -1) mlog("InitNpcMesh() - 캐릭터 생성 실패\n");

	//VisualMesh = ZGetGame()->m_VisualMeshMgr.GetFast(nVMID);

	//if (worldObj.animation.empty() == false)
	//{
	//	VisualMesh->SetAnimation(worldObj.animation.c_str());
	//}

	//VisualMesh->SetVisibility(1.f);
	//VisualMesh->GetMesh()->SetTextureRenderOnOff(true);
	//VisualMesh->SetCheckViewFrustum(false);
	//VisualMesh->SetScale((D3DXVECTOR3)(worldObj.scale));

	//SetCollidable(worldObj.collidable);
	//SetCollRadius(worldObj.collradius);
	//SetCollWidth(worldObj.collwidth);
	//SetCollHeight(worldObj.collheight);
	//SetCollisionType(worldObj.collisiontype);
	//StartPosition = worldObj.position;
	//CurrPosition = StartPosition;
	//Direction = worldObj.direction;
	//Sound = worldObj.sound;

	//rmatrix mat = GetWorldMatrix();
	//VisualMesh->SetWorldMatrix(mat);

	//return true;
}

void ZWorldObject::Update(float elapsed)
{

}

void ZWorldObject::Draw()
{
	if (VisualMesh == nullptr || VisualMesh->GetMesh()->m_isMeshLoaded == false)
		return;

	if (RGetMultiSampling() > D3DMULTISAMPLE_NONE)
		RGetDevice()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

	//need to update the frame at every draw call
	VisualMesh->Frame();
	VisualMesh->Render();

	//if (m_Collision.IsCollidable() && Movable == false)
	//{
	//	rvector pos = GetPosition();
	//	rvector dir = ZGetGame()->m_pMyCharacter->GetPosition() - pos;
	//	dir.z = 0;
	//	float fDist = Magnitude(dir);

	//	float fCOLLISION_DIST = GetCollRadius() + ZGetGame()->m_pMyCharacter->GetCollRadius();

	//	if (fDist < fCOLLISION_DIST && fabs(pos.z - ZGetGame()->m_pMyCharacter->GetPosition().z) < GetCollHeight())
	//	{
	//		ZGetGame()->m_pMyCharacter->SetPosition(rvector(pos.x, pos.y + fCOLLISION_DIST, ZGetGame()->m_pMyCharacter->GetPosition().z));
	//	}
	//}
	if (RGetMultiSampling() > D3DMULTISAMPLE_NONE)
		RGetDevice()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

}



//TODO: fill this in
bool ZWorldObject::Pick(rvector& pos, rvector& dir, RBSPPICKINFO* pOut)
{
	if (IsCollidable() == false)
	{
		return false;
	}
	rvector diff = GetPosition() - pos;
	diff.z = 0;

	// 나중에 radius상수값으로 된것 Object의 멤버변수로 고치자
	float objDistance = 0;
	if (GetCollisionType() == CT_CYLINDER)
		objDistance = GetCollRadius();
	else
		objDistance = GetCollWidth();
	if (Magnitude(diff) < objDistance && pos.z >= CurrPosition.z)
	{
		return true;
	}
	return false;
}