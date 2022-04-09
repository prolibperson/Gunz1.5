#include "stdafx.h"
#include "ZWorldObject.h"

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
}

//todo: init mesh differently, currently using npc mesh manager...
void ZWorldObject::InitWithMesh(WorldObject const& worldObj)
{
	RMesh* pMesh;

	Name = worldObj.name;
	Model = worldObj.model;

	pMesh = ZGetNpcMeshMgr()->Get(worldObj.model.c_str());
	if (!pMesh)
	{
		_ASSERT(0);
		return;
	}
	if (pMesh->m_isMeshLoaded == false)
	{
			ZGetNpcMeshMgr()->Load(worldObj.model.c_str());
			ZGetNpcMeshMgr()->ReloadAllAnimation();
	}

	int nVMID = ZGetGame()->m_VisualMeshMgr.Add(pMesh);
	if (nVMID == -1) mlog("InitNpcMesh() - 캐릭터 생성 실패\n");

	VisualMesh = ZGetGame()->m_VisualMeshMgr.GetFast(nVMID);

	if (worldObj.animation.empty() == false)
	{
		VisualMesh->SetAnimation(worldObj.animation.c_str());
	}

	VisualMesh->SetVisibility(1.f);
	VisualMesh->GetMesh()->SetTextureRenderOnOff(true);
	VisualMesh->SetCheckViewFrustum(false);
	VisualMesh->SetScale((D3DXVECTOR3)(worldObj.scale));

	SetCollidable(worldObj.collidable);
	SetCollRadius(worldObj.collradius);
	SetCollWidth(worldObj.collwidth);
	SetCollHeight(worldObj.collheight);
	SetCollisionType(worldObj.collisiontype);
	StartPosition = worldObj.position;
	CurrPosition = StartPosition;
	Direction = worldObj.direction;
	Sound = worldObj.sound;

	rmatrix mat = GetWorldMatrix();
	VisualMesh->SetWorldMatrix(mat);
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