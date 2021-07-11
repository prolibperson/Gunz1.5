#include "stdafx.h"
#include "XBspObject.h"

XBspObject::XBspObject()
{

}

XBspObject::~XBspObject()
{

}

bool XBspObject::Pick(const rvector &pos, const rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag)
{
	return false;
	//return ABspObject::Pick(pos, dir, pOut, dwPassFlag);
}

bool XBspObject::PickTo(const rvector &pos, const rvector &to, RBSPPICKINFO *pOut, DWORD dwPassFlag)
{
	return false;
	//return ABspObject::PickTo(pos, to, pOut, dwPassFlag);
}

bool XBspObject::PickOcTree(rvector &pos, rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag)
{
	return false;
	//return ABspObject::PickOcTree(pos, dir, pOut, dwPassFlag);
}

bool XBspObject::Draw()
{
	return false;
}

bool XBspObject::CheckWall(rvector &origin, rvector &targetpos, float fRadius, float fHeight, RCOLLISIONMETHOD method, int nDepth, rplane *pimpactplane)
{
	return false;
	//return ABspObject::CheckWall(origin, targetpos, fRadius, fHeight, method, nDepth, pimpactplane);
}

bool XBspObject::CheckSolid(rvector &pos, float fRadius, float fHeight, RCOLLISIONMETHOD method)
{
	return false;
	//return ABspObject::CheckSolid(pos, fRadius, fHeight, method);
}

bool XBspObject::GetShadowPosition(rvector& pos_, rvector& dir_, rvector* outNormal_, rvector* outPos_)
{
	return false;
	//return ABspObject::GetShadowPosition(pos_, dir_, outNormal_, outPos_);
}

bool XBspObject::IsVisible(rboundingbox& bb)
{
	return false;
	//return ABspObject::IsVisible(bb);
}


bool XBspObject::Open(const char * bspName , const char* descExtension, ROpenFlag nOpenFlag, RFPROGRESSCALLBACK pfnProgressCallback, void *CallbackParam)
{
	return false;
	bool result = ABspObject::Open(bspName, descExtension, nOpenFlag, pfnProgressCallback, CallbackParam);
	if (result)
	{
		///TODO: XBspObject specific functions
	}
}

void XBspObject::OnInvalidate()
{

}

void XBspObject::OnRestore()
{

}

void XBspObject::DrawObjects()
{

}

void XBspObject::OptimizeBoundingBox()
{

}

rvector XBspObject::GetFloor(rvector &origin, float fRadius, float fHeight, rplane *pimpactplane)
{
	return rvector(0,0,0);
	//return ABspObject::GetFloor(origin, fRadius, fHeight, pimpactplane);
}