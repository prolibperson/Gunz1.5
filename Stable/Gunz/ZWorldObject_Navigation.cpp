#include "stdafx.h"
#include "ZWorldObject_Navigation.h"

ZWorldObject_Navigation::ZWorldObject_Navigation()
{
}

ZWorldObject_Navigation::~ZWorldObject_Navigation()
{
}

bool ZWorldObject_Navigation::InitWithMesh(WorldObject const& worldObj)
{
	return ZWorldObject_Movable::InitWithMesh(worldObj);
}

void ZWorldObject_Navigation::Update(float elapsedtime)
{
	RunWayPoints(elapsedtime);

	rmatrix mat = GetWorldMatrix();
	VisualMesh->SetWorldMatrix(mat);
}



//TODO: if object has reached the end of its path, need to reverse and go back to the start point
void ZWorldObject_Navigation::BuildNavigationPath()
{
	ZNavigationMesh navMesh = ZGetNavigationMesh();
	if (navMesh.IsNull())
		return;

	// Make navigation path
	float dist = Magnitude(TargetPosition - CurrPosition);
	if (dist <= 50)
	{
		if (TargetPosition == StartPosition)
		{
			TargetPosition = EndPosition;
		}
		else
		{
			TargetPosition = StartPosition;
		}
	}

	if (!navMesh.BuildNavigationPath(GetPosition(), TargetPosition))
		return;

	for (list<rvector>::iterator it = navMesh.GetWaypointList().begin(); it != navMesh.GetWaypointList().end(); ++it)
		m_listWaypoint.push_back(*it);
}

//todo: implement speed
void ZWorldObject_Navigation::Move(rvector& diff)
{
	CurrPosition += diff;
}


//todo: clean write
void ZWorldObject_Navigation::RunWayPoints(float elapsedtime)
{
	//TODO: reverse position
	if (m_listWaypoint.size() == 0)
	{
		BuildNavigationPath();
	}

	rvector nextWaypoint = *m_listWaypoint.begin();
	rvector myPos = GetPosition();

	nextWaypoint.z = 0;
	myPos.z = 0;

	float diffSq = Magnitude(nextWaypoint - myPos);
	if (diffSq <= 30)
	{
		m_listWaypoint.pop_front();
		return;
	}

	rvector vWaypointDir = nextWaypoint - myPos;
	Normalize(vWaypointDir);

	// 다음 웨이포인트를 향해 방향 전환
	Direction = vWaypointDir;

	Move(vWaypointDir * (moveSpeed * elapsedtime));
}