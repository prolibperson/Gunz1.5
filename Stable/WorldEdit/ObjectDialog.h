#if !defined(AFX_OBJECTDIALOG_H__B3451C4F_9CCA_4114_8E09_8BDCC7030388__INCLUDED_)
#define AFX_OBJECTDIALOG_H__B3451C4F_9CCA_4114_8E09_8BDCC7030388__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectDialog.h : header file
//

#include "StdAfx.h"
#include "RBspObject.h"
#include "RVisualMeshMgr.h"

enum OBJTYPE
{
	OT_STATIONARY,
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
protected:
	std::string Name;
	std::string Model;
	std::string Sound;

	MapObjectCollision m_Collision;

	rvector StartPosition;
	rvector Direction;
	rvector CurrPosition;


	RVisualMesh* VisualMesh;
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
		MakeWorldMatrix(&m, CurrPosition, Direction, rvector(0, 0, 1));
		return m;
	}

	rvector const& GetPosition() {
		return CurrPosition;
	}

	rvector const& GetDirection() {
		return Direction;
	}

	virtual bool InitWithMesh(WorldObject const& worldObject);

	virtual void Update(float time);

	void Draw();

	bool Pick(rvector& pos, rvector& dir, RBSPPICKINFO* pOut);
};

/////////////////////////////////////////////////////////////////////////////
// CObjectDialog dialog

class CObjectDialog : public CDialog
{
// Construction
public:
	CObjectDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectDialog)
	enum { IDD = IDD_DIALOG_OBJECT };
	CListBox	m_ObjectList;
	RVisualMeshMgr VisualMeshMgr;

	void Initilize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnBnClickedRadio1();
		//custom: map objects
	std::vector<std::unique_ptr<ZWorldObject>> mapObjects;

	std::vector<std::unique_ptr<ZWorldObject>> const& GetWorldObjects()
	{
		return mapObjects;
	}

	void RenderMapObjects();
	void UpdateMapObjects(float delta);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTDIALOG_H__B3451C4F_9CCA_4114_8E09_8BDCC7030388__INCLUDED_)
