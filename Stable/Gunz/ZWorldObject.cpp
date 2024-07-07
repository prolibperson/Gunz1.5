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
	LastMoveDiff = rvector(0, 0, 0);

	//remove model from memory, will be loaded again next time it's needed
	ZGetMeshMgr()->Del((char*)Name.c_str());
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
		ZGetMeshMgr()->Add((char*)meshpath.c_str(), (char*)Name.c_str(), false, true);
	}


	pMesh = ZGetMeshMgr()->Get((char*)Name.c_str());
	if (pMesh == nullptr)
		return false;

	pMesh->ReloadAnimation();

	VisualMesh = new RVisualMesh;
	VisualMesh->Create(pMesh);



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

	if (worldObj.animation.empty() == false)
	{
		VisualMesh->SetAnimation(worldObj.animation.c_str());
	}


	return true;
}

bool ZWorldObject::IntersectsZ(rvector const& a, rboundingbox const& b)
{

	return
		a.z >= b.vmin.z &&
		a.z <= b.vmax.z;
}

void ZWorldObject::Update(float elapsed)
{
	//todo: write check for if object is movable. need to add variable and decide where to call this later
	//for (auto const& itor : ZGetGame()->m_CharacterManager)
	//{
	//	ZCharacterObject* player = itor.second;

	//	if (IntersectsXY(player->GetPosition(), bbox))
	//	{
	//		if (fabs((CurrPosition.z + GetHeight()) - player->GetPosition().z) < CHARACTER_HEIGHT)
	//		{
	//			continue;
	//		}

	//		rvector newthispos = CurrPosition + ((ZModule_Movable*)player->GetModule(ZMID_MOVABLE))->GetLastMove() * (CHARACTER_RADIUS + 1.f);
	//		newthispos.z = 0;

	//		rvector diff = newthispos - CurrPosition;

	//		CurrPosition.x += diff.x + CHARACTER_RADIUS * elapsed;
	//		CurrPosition.y += diff.y + CHARACTER_RADIUS * elapsed;
	//	}
	//}

	rmatrix mat = GetWorldMatrix();
	VisualMesh->SetWorldMatrix(mat);

	rvector vmin, vmax;
	VisualMesh->GetBBox(vmin, vmax);
	bbox = rboundingbox(vmin, vmax);

	for (int i = 0; i < 3; ++i)
	{
		bbox.vmin[i] *= VisualMesh->GetScale()[i];
		bbox.vmax[i] *= VisualMesh->GetScale()[i];
	}

	rboundingbox transformed = bbox.Transform(GetWorldMatrix());

	bbox = transformed;
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
			a[j] = ind[lines[i][0]][j] ? bbox.vmax[j] : bbox.vmin[j];
			b[j] = ind[lines[i][1]][j] ? bbox.vmax[j] : bbox.vmin[j];
		}

		_RS2::RDrawLine(a, b, 0xFFFF0000);
	}

	if (RGetMultiSampling() > D3DMULTISAMPLE_NONE)
	{
		RGetDevice()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	}
}


//TODO: fill this in
bool ZWorldObject::Pick(rvector& pos, rvector& dir, RBSPPICKINFO* pOut)
{
	if (IsCollidable() == false)
	{
		return false;
	}

	if (IntersectsXY(pos, bbox) && fabs(CurrPosition.z - pos.z) < fabs((CurrPosition.z + GetHeight()) - pos.z) < 100)
	{
		return true;
	}

	return false;
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
	//todo: rewrite this, intersectsZ works but need to check the difference between the pos and min/max values of boundingbox
	//todo: handle direction???
	if (IsCollidable() == false)
	{
		return false;
	}

	rvector dirdiff = pos - CurrPosition;
	Normalize(dirdiff);

	rboundingbox playerbox;
	playerbox.vmin = pos + rvector(100, 100, 0);
	playerbox.vmax = pos - rvector(100, 100, 0);
	if (IntersectsXY(playerbox,bbox) && IntersectsZ(pos,bbox))
	{
		return true;
	}

	return false;
}

bool ZWorldObject::CheckWallRun(rvector const& pos, rvector const& dir, rvector& out)
{
	if (IsCollidable() == false)
	{
		return false;
	}

	rboundingbox playerbox;
	playerbox.vmin = pos + rvector(CHARACTER_RADIUS, CHARACTER_RADIUS, 0);
	playerbox.vmax = pos - rvector(CHARACTER_RADIUS, CHARACTER_RADIUS, 0);

	if (IntersectsXY(playerbox, bbox))
	{
		return true;
	}

	return false;
}


bool ZWorldObject::IntersectsXY(const rvector& a, const rboundingbox& b)
{
	return
		a.x >= b.vmin.x &&
		a.x <= b.vmax.x&&
		a.y >= b.vmin.y &&
		a.y <= b.vmax.y;
}

bool ZWorldObject::IntersectsXY(const rboundingbox& a, const rboundingbox& b)
{
	return a.vmin.x >= b.vmin.x && a.vmax.x <= b.vmax.x
		&& a.vmin.y >= b.vmin.y && a.vmax.y <= b.vmax.y;
}

bool ZWorldObject::IsStandingOn(ZObject* const Object)
{
	if (IsCollidable() == false)
	{
		return false;
	}


	if (IntersectsXY(Object->GetPosition(), bbox) && fabs((CurrPosition.z + GetHeight()) - Object->GetPosition().z) < CHARACTER_HEIGHT)
	{
		return true;
	}

	return false;
}
