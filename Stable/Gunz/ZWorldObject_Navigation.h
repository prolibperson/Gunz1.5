#pragma once

#include "ZWorldObject.h"

class ZWorldObject_Navigation : public ZWorldObject_Movable
{
public:
	ZWorldObject_Navigation();
	virtual ~ZWorldObject_Navigation();

	virtual void Update(float elapsedtime) override;

	void RunWayPoints(float elapsedtime);

	void BuildNavigationPath();

	void Move(rvector& diff);

private:
	std::list<rvector> m_listWaypoint;

protected:
	virtual bool InitWithMesh(WorldObject const& worldObj) override;
};