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
	lastUpdateTime = 0;
	IsReversing = false;
	MaxHeight = 0;
	MinHeight = 0;
	Movable = false;
	ReverseAnimation = false;
	VisualMesh = nullptr;
	moveSpeed = 0;
}

ZWorldObject::~ZWorldObject() noexcept
{
	
}

//todo: init mesh differently, currently using npc mesh manager...
void ZWorldObject::InitMesh(WorldObject const& worldObj)
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
	//temporary scale
	VisualMesh->SetScale((D3DXVECTOR3)(worldObj.scale));

	StartPosition = worldObj.position;
	CurrPosition = StartPosition;
	Direction = worldObj.direction;
	SetCollidable(worldObj.collidable);
	SetCollRadius(worldObj.collradius);
	SetCollWidth(worldObj.collwidth);
	SetCollHeight(worldObj.collheight);
	SetCollisionType(worldObj.collisiontype);
	Movable = worldObj.movable;
	moveSpeed = worldObj.speed;
	MaxHeight = worldObj.maxheight;
	MinHeight = worldObj.minheight;
	ReverseAnimation = worldObj.reverseanimation;
	Sound = worldObj.sound;
	UsingNavMesh = worldObj.usepath;
	EndPosition = rvector(worldObj.endposition);


	//init world matrix once so object is positioned correctly
	if (Movable == false)
	{
		rmatrix mat = GetWorldMatrix();
		VisualMesh->SetWorldMatrix(mat);
	}
}

//TODO: determine best way to update the npcs position and framing
void ZWorldObject::Update(float time)
{
	//double thisTime = std::chrono::duration<double, std::ratio<1, 1000>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	//hard limit movement updates to 30fps
	if (Movable == true)
	{
		//if (thisTime - lastUpdateTime >= 33.33)
		//{
			if (UsingNavMesh == true)
			{
				RunWayPoints();
			}
			else
			{
				Move(time);
			}

			rmatrix mat = GetWorldMatrix();
			VisualMesh->SetWorldMatrix(mat);

		//	lastUpdateTime = std::chrono::duration<double, std::ratio<1, 1000>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	//	}
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

//todo: handle stuf a bit better i guess?
D3DXVECTOR3 lerp(const D3DXVECTOR3& start, const D3DXVECTOR3& end, float percent) {
	return start + (percent * (end - start));// a + (b - a) * t;
}
void ZWorldObject::Move(double const& moveDiff)
{
	if (CurrPosition.z >= MaxHeight)
	{
		ReverseAnimation = true;
	}
	else if(CurrPosition.z <= MinHeight)
	{
		ReverseAnimation = false;
	}
//	auto targetPos = ReverseAnimation ? StartPosition : EndPosition;

	//if (GetPosition() == targetPos)
	//	ReverseAnimation = !ReverseAnimation;
	//TODO: handle moveDiff differently????

	//CurrPosition = lerp(CurrPosition, ReverseAnimation ? StartPosition : EndPosition, 0.05 * moveDiff);
	if (ReverseAnimation == false)
	{
		//CurrPosition = lerp(CurrPosition, EndPosition, moveDiff);
		CurrPosition.z += roundf((moveSpeed * moveDiff));
	}
	else
	{
		CurrPosition.z -= (roundf(moveSpeed * moveDiff));
	}

}

void ZWorldObject::Move(rvector& diff)
{
	rvector origin, targetpos;
	rplane impactplane;
	float fCollUpHeight = max(120.f, 0.666667f * GetCollHeight() * 0.5f); // 0.666667f == 120.f/180.f

	origin = GetPosition() + rvector(0, 0, fCollUpHeight);
	targetpos = origin + diff;

	diff = targetpos - origin;
	CurrPosition = targetpos - rvector(0, 0, fCollUpHeight);// (targetpos - rvector(0, 0, fCollUpHeight));
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

//TODO: if object has reached the end of its path, need to reverse and go back to the start point
void ZWorldObject::BuildNavigationPath()
{
	if (EndPosition == rvector(-1, -1, -1))
		return;

	ZNavigationMesh navMesh = ZGetNavigationMesh();
	if (navMesh.IsNull())
		return;

	// Make navigation path
	float dist = Magnitude(GetPosition() - EndPosition);
	if (!navMesh.BuildNavigationPath(GetPosition(), EndPosition))
		return;

	for (list<rvector>::iterator it = navMesh.GetWaypointList().begin(); it != navMesh.GetWaypointList().end(); ++it)
		m_listWaypoint.push_back(*it);
}

void ZWorldObject::RunWayPoints()
{
	//TODO: reverse position
	if (m_listWaypoint.size() == 0)
	{
		BuildNavigationPath();
		//return;
	}

	rvector nextWaypoint = *m_listWaypoint.begin();
	rvector myPos = GetPosition();

	nextWaypoint.z = 0;
	myPos.z = 0;

	float diffSq = MagnitudeSq(nextWaypoint - myPos);
	if (diffSq <= 20.f * 20.f)
	{
		m_listWaypoint.pop_front();
		return;
	}

	rvector vWaypointDir = nextWaypoint - myPos;
	Normalize(vWaypointDir);

	// 다음 웨이포인트를 향해 방향 전환
	Direction = vWaypointDir;

	Move(vWaypointDir);
}