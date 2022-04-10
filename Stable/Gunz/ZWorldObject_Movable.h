#pragma once

#include "ZWorldObject.h"

class ZWorldObject_Movable : public ZWorldObject
{
public:
	ZWorldObject_Movable();
	virtual ~ZWorldObject_Movable();

protected:
	//rvector CurrPosition;
	//rvector EndPosition;
	//rvector TargetPosition;

	virtual void Update(float elapsedtime);
	virtual void Move(double const& moveDiff);

	virtual bool InitWithMesh(WorldObject const& worldObject);

	rvector TargetPosition;
	rvector EndPosition;
	double moveSpeed;

};