#include "stdafx.h"
#include "ZWorldObject_Movable.h"

ZWorldObject_Movable::ZWorldObject_Movable()
{
	moveSpeed = 0;
}

ZWorldObject_Movable::~ZWorldObject_Movable()
{
}

void ZWorldObject_Movable::Update(float elapsed)
{
	Move(elapsed);
	rmatrix mat = GetWorldMatrix();
	VisualMesh->SetWorldMatrix(mat);

	ZWorldObject::Update(elapsed);

}

void ZWorldObject_Movable::Move(double const& moveDiff)
{
	if (CurrPosition > EndPosition)
	{
		TargetPosition = StartPosition;
	}
	else if (CurrPosition < StartPosition)
	{
		TargetPosition = EndPosition;
	}

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

	LastMoveDiff = diff * (moveSpeed * moveDiff);

	CurrPosition += LastMoveDiff;
}

bool ZWorldObject_Movable::InitWithMesh(WorldObject const& worldObj)
{
	if (ZWorldObject::InitWithMesh(worldObj) == false)
		return false;


	moveSpeed = worldObj.speed;
	EndPosition = rvector(worldObj.endposition);
	TargetPosition = worldObj.endposition;

	return true;
}
