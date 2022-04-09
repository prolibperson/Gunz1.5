#include "stdafx.h"
#include "ZWorldObject_Movable.h"

ZWorldObject_Movable::ZWorldObject_Movable()
{

}

ZWorldObject_Movable::~ZWorldObject_Movable()
{
}

void ZWorldObject_Movable::Update(float elapsed)
{
	Move(elapsed);
	rmatrix mat = GetWorldMatrix();
	VisualMesh->SetWorldMatrix(mat);
}

void ZWorldObject_Movable::Move(double const& moveDiff)
{
	float dist = Magnitude(TargetPosition - CurrPosition);
	if (dist <= 1.f)
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

	rvector diff = TargetPosition - CurrPosition;
	Normalize(diff);

	CurrPosition += diff * (moveSpeed * moveDiff);
}

void ZWorldObject_Movable::InitWithMesh(WorldObject const& worldObj)
{

	ZWorldObject::InitWithMesh(worldObj);

	moveSpeed = worldObj.speed;
	EndPosition = rvector(worldObj.endposition);
	TargetPosition = worldObj.endposition;
}
