#pragma once

#include "stdafx.h"
#include "ZObject.h"
#include "ZNavigationMesh.h"


enum OBJTYPE
{
	OT_STATIC,
	OT_MOVABLE,
	OT_NAVIGATION,
};

enum COLLTYPE
{
	CT_CYLINDER,
	CT_SQUARE,
	CT_END
};

struct WorldObject
{
	std::string name;
	std::string model;
	std::string animation;
	std::string sound;
	rvector position;
	rvector direction;
	rvector scale;
	rvector endposition;
	float speed;
	float collradius;
	float collheight;
	float collwidth;
	COLLTYPE collisiontype;
	OBJTYPE objecttype;
	bool collidable;
	bool reverseanimation;
};


class MapObjectCollision
{
public:
	MapObjectCollision();
	~MapObjectCollision();
private:
	COLLTYPE CollisionType;
	float Width;
	float Radius;
	float Height;
	bool  Collidable;
public:

	COLLTYPE const GetCollisionType() { return CollisionType; }
	float const& GetWidth() { return Width; }
	float const& GetRadius() { return Radius; }
	float const& GetHeight() { return Height; }
	bool const& IsCollidable() { return Collidable; }

	void SetWidth(float const& width) { Width = width; }
	void SetRadius(float const& radius) { Radius = radius; }
	void SetHeight(float const& height) { Height = height; }
	void SetCollisionType(COLLTYPE const& colltype) { CollisionType = colltype; }
	void SetCollidable(bool const& collidable) { Collidable = collidable; }
};


class ZWorldObject
{
private:
protected:
	std::string Name;
	std::string Model;
	std::string Sound;

	MapObjectCollision m_Collision;

	rvector StartPosition;
	rvector Direction;
	rvector CurrPosition;
	rvector LastMoveDiff;

	RVisualMesh* VisualMesh;
	rboundingbox bbox;
public:

	ZWorldObject() noexcept;
	virtual ~ZWorldObject() noexcept;

	float const& GetCollRadius()
	{
		return m_Collision.GetRadius();
	}

	float const& GetCollHeight()
	{
		return m_Collision.GetHeight();
	}

	float const& GetCollWidth()
	{
		return m_Collision.GetWidth();
	}

	bool const IsCollidable()
	{
		return m_Collision.IsCollidable();
	}

	void SetCollidable(bool const collidable)
	{
		m_Collision.SetCollidable(collidable);
	}

	void SetCollRadius(float const collRadius)
	{
		m_Collision.SetRadius(collRadius);
	}

	void SetCollHeight(float const collHeight)
	{
		m_Collision.SetHeight(collHeight);
	}

	void SetCollWidth(float const collWidth)
	{
		m_Collision.SetWidth(collWidth);
	}

	void SetCollisionType(COLLTYPE const type)
	{
		m_Collision.SetCollisionType(type);
	}

	float GetWidth()
	{
		return fabsf(bbox.vmax.y - bbox.vmin.y);
	}

	float GetLength()
	{
		return fabsf(bbox.vmax.x - bbox.vmin.x);
	}

	float GetHeight()
	{
		return fabsf(bbox.vmax.z - bbox.vmin.z);
	}

	COLLTYPE const GetCollisionType()
	{
		return m_Collision.GetCollisionType();
	}

	std::string const& GetName()
	{
		return Name;
	}

	std::string const& GetModelName()
	{
		return Model;
	}

	std::string const& GetSound()
	{
		return Sound;
	}

	RVisualMesh* const GetVisualMesh() { return VisualMesh; }

	rmatrix GetWorldMatrix() {
		rmatrix m;
		MakeWorldMatrix(&m, CurrPosition,Direction, rvector(0, 0, 1));
		return m;
	}

	rvector const& GetPosition() {
		return CurrPosition;
	}

	rvector const& GetDirection() {
		return Direction;
	}

	rvector const& GetLastMoveDiff() {
		return LastMoveDiff;
	}

	__forceinline rboundingbox GetBounds()
	{
		return bbox;
	}

	virtual bool InitWithMesh(WorldObject const& worldObject);

	virtual void Update(float time);

	void Draw();

	bool Pick(rvector& pos, rvector& dir, RBSPPICKINFO* pOut);
	bool OnCheckWallHang(rvector const& pos, rvector const& dir, bool const& initial = false);
	bool CheckWallRun(rvector const& pos, rvector const& dir, rvector& out);
	bool IsStandingOn(ZObject* const Object);
	bool IntersectsXY(const rvector& a, const rboundingbox& b);
	bool IntersectsXY(const rboundingbox& a, const rboundingbox& b);
	bool IntersectsZ(rvector const& a, rboundingbox const& b);
};