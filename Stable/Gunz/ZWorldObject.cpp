#include "stdafx.h"
#include "ZWorldObject.h"
#include "ZMap.h"

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
	LastMoveDiff = rvector(0, 0, 0);
}

ZWorldObject::~ZWorldObject() noexcept
{
	delete VisualMesh;
	VisualMesh = nullptr;

	//remove model from memory, will be loaded again next time it's needed
	ZGetMeshMgr()->Del((char*)Model.c_str());
}

//todo: init mesh differently, currently using npc mesh manager...
bool ZWorldObject::InitWithMesh(WorldObject const& worldObj)
{
	Name = worldObj.name;
	Model = worldObj.model;

	char szMapPath[64] = "";
	ZGetCurrMapPath(szMapPath);

	char szBuf[256];

	sprintf(szBuf, "%s%s/", szMapPath, ZGetGameClient()->GetMatchStageSetting()->GetMapName());
	std::string meshpath = szBuf;
	meshpath.append(Model);

	RMesh* pMesh = ZGetMeshMgr()->Get((char*)worldObj.name.c_str());

	if (pMesh == nullptr)
	{
		ZGetMeshMgr()->Add((char*)meshpath.c_str(), (char*)worldObj.name.c_str(), false, true);
	}


	pMesh = ZGetMeshMgr()->Get((char*)worldObj.name.c_str());
	if (pMesh == nullptr)
		return false;

	pMesh->ReloadAnimation();

	VisualMesh = new RVisualMesh;
	VisualMesh->Create(pMesh);

	if (worldObj.animation.empty() == false)
	{
		std::string meshpath = szBuf;
		meshpath.append(worldObj.animation.c_str());
		RAnimation* pAni = VisualMesh->GetMesh()->m_ani_mgr.AddGameLoad((char*)worldObj.animation.c_str(), (char*)meshpath.c_str(), -1, 0);
		pAni->SetAnimationLoopType(AnimationLoopType::RAniLoopType_Loop);
		VisualMesh->SetAnimation(worldObj.animation.c_str());
	}

	VisualMesh->SetVisibility(1.f);
	VisualMesh->GetMesh()->SetTextureRenderOnOff(true);
	VisualMesh->SetCheckViewFrustum(false);
	VisualMesh->SetScale((D3DXVECTOR3)(worldObj.scale));

	//collision
	{
		SetCollidable(worldObj.collidable);
		SetCollRadius(worldObj.collradius);
		SetCollWidth(worldObj.collwidth);
		SetCollHeight(worldObj.collheight);
		SetCollisionType(worldObj.collisiontype);
	}
	StartPosition = worldObj.position;
	CurrPosition = StartPosition;
	Direction = worldObj.direction;
	Sound = worldObj.sound;

	rmatrix mat = GetWorldMatrix();
	VisualMesh->SetWorldMatrix(mat);


	return true;
}

void ZWorldObject::Update(float elapsed)
{
	if (IsCollidable() && VisualMesh != nullptr)
	{
		//update bbox, i think is worth to use only when change the model scale after first initialization
		//VisualMesh->CalcBox();

		rvector vmin, vmax;
		VisualMesh->GetBBox(vmin, vmax);
		rboundingbox bbox(vmin, vmax);

		for (int i = 0; i < 3; ++i)
		{
			bbox.vmin[i] *= VisualMesh->GetScale()[i];
			bbox.vmax[i] *= VisualMesh->GetScale()[i];
		}

		rboundingbox transformed = bbox.Transform(GetWorldMatrix());

		m_Collision.SetHeight(abs(transformed.GetSize().z));
		m_Collision.SetRadius(abs(max(transformed.GetExtents().x, transformed.GetExtents().y)));
		m_Collision.SetWidth(m_Collision.GetRadius());
	}

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

	rvector vmin, vmax;
	VisualMesh->GetBBox(vmin, vmax);
	rboundingbox bbox(vmin, vmax);

	for (int i = 0; i < 3; ++i)
	{
		bbox.vmin[i] *= VisualMesh->GetScale()[i];
		bbox.vmax[i] *= VisualMesh->GetScale()[i];
	}

	rboundingbox transformed = bbox.Transform(GetWorldMatrix());


	_RS2::RGetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	_RS2::RGetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CONSTANT);
	_RS2::RGetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	_RS2::RGetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);
	_RS2::RGetDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_RS2::RGetDevice()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	_RS2::RGetDevice()->SetTextureStageState(0, D3DTSS_CONSTANT, 0xFFFF0000);

	int i, j;

	int ind[8][3] = { {0,0,0},{1,0,0},{1,1,0},{0,1,0}, {0,0,1},{1,0,1},{1,1,1},{0,1,1} };
	int lines[12][2] = { {0,1},{1,5},{5,4},{4,0},{5,6},{1,2},{0,3},{4,7},{7,6},{6,2},{2,3},{3,7} };

	for (i = 0; i < 12; i++)
	{
		rvector a, b;
		for (j = 0; j < 3; j++)
		{
			a[j] = ind[lines[i][0]][j] ? transformed.vmax[j] : transformed.vmin[j];
			b[j] = ind[lines[i][1]][j] ? transformed.vmax[j] : transformed.vmin[j];
		}

		_RS2::RDrawLine(a, b, 0xFFFF0000);
	}

//	RDrawCylinder(CurrPosition, GetCollisionType() == COLLTYPE::CT_CYLINDER ? GetCollRadius() : GetCollWidth(), GetCollHeight(), 8);

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

	//todo: improve some more but fixes the teleportation bug
	if (Magnitude(diff) < objDistance && fabs(CurrPosition.z - pos.z) < (GetCollHeight() + CHARACTER_HEIGHT))
	{
		if (pos.z + CHARACTER_HEIGHT >= CurrPosition.z + GetCollHeight())
			return true;

		return false;
	}
	return false;
}

bool ZWorldObject::OnCheckWallHang(rvector const& pos, rvector const& dir, bool const& initial)
{
	//todo: tweak this better
	if (IsCollidable() == false)
	{
		return false;
	}

	rvector diff;
	if (initial == false)
	{
		diff = GetPosition() - LastMoveDiff - pos;
	}
	else
	{
		diff = GetPosition() - pos;
	}
	diff.z = 0;

	float objDistance = 0;
	if (GetCollisionType() == CT_CYLINDER)
		objDistance = GetCollRadius();
	else
		objDistance = GetCollWidth();

	//get the difference in height
	float heightdiff = fabs(GetPosition().z - pos.z);

	float diff1 = Magnitude(diff) - (CHARACTER_RADIUS + objDistance);

	//if player is close enough to the object, but not too high, return true to hang.
	float collheight = GetCollHeight(); // coll height is a value of 110 for this object
	if (diff1 <= objDistance && heightdiff <= collheight)
	{
		return true;
	}
	return false;
}
