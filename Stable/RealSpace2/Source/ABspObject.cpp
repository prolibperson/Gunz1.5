#include "stdafx.h"
#include "ABspObject.h"

#include <crtdbg.h>
#include <map>

#include "MZFileSystem.h"
//#include "RBspObject.h"
#include "RMaterialList.h"
#include "RealSpace2.h"
#include "RBaseTexture.h"
#include "MDebug.h"
#include "RVersions.h"
#include "RMaterialList.h"
#include "RVisualMeshMgr.h"
#include "FileInfo.h"
#include "ROcclusionList.h"
#include "MProfiler.h"
#include "RShaderMgr.h"
#include "RLenzFlare.h"
#include <DxErr9.h>
#include "RNavigationNode.h"
#define __BP(i,n) ;
#define __EP(i) ;

bool			g_bPickFound;
rvector			g_PickOrigin;
rvector			g_PickTo;
rvector			g_PickDir;
RBSPPICKINFO	*g_pPickOut;
//RPathNode		**g_pPickOutPathNode;
rvector			*g_pColPos;
rplane			g_PickPlane;
float			g_fPickDist;
DWORD			g_dwPassFlag;


///TODO: make abstract functions for every necessary function


///TODO: Add function bodies for various functions
ABspObject::ABspObject()
{
	///TODO : Init shared resources
}

ABspObject::~ABspObject()
{
	///TODO : Destroy shared resources
}

bool ABspObject::Pick(const rvector &pos, const rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag)
{
	//if (!m_pBspRoot) return false;

	//__BP(195, "RBspObject::Pick");

	//g_PickOrigin = pos;
	//g_PickTo = pos + 10000.f*dir;
	//g_PickDir = dir;
	//Normalize(g_PickDir);

	//g_bPickFound = false;
	//g_pPickOut = pOut;
	//D3DXPlaneFromPointNormal(&g_PickPlane, &g_PickOrigin, &g_PickDir);
	//g_fPickDist = FLT_MAX;

	//g_dwPassFlag = dwPassFlag;

	//g_bPickFound = Pick(m_pBspRoot, pos, g_PickTo);

	//__EP(195);

	//if (g_bPickFound)
	//	return true;

	//return false;
	return true;
}

bool ABspObject::Open(const char * bspName, const char* descExtension, ROpenFlag nOpenFlag , RFPROGRESSCALLBACK pfnProgressCallback, void *CallbackParam)
{

	return false;
/*	mlog("BspObject open : begin %s \n", bspName);

	m_OpenMode = nOpenFlag;
	m_filename = bspName;

	char xmlname[_MAX_PATH];
	sprintf(xmlname, "%s.%s", bspName, descExtension);

	if (!OpenDescription(xmlname))
	{
		MLog("Error while loading %s\n", xmlname);
		return false;
	}
	if (pfnProgressCallback) pfnProgressCallback(CallbackParam, .3f);

	mlog("RBspObject::Open : OpenDescription\n");

	if (!OpenRs(filename))
	{
		MLog("Error while loading %s\n", filename);
		return false;
	}

	if (pfnProgressCallback) pfnProgressCallback(CallbackParam, .6f);
	mlog("RBspObject::Open : OpenRs \n");


	char bspname[_MAX_PATH];
	sprintf(bspname, "%s.bsp", filename);
	if (!OpenBsp(bspname))
	{
		MLog("Error while loading %s\n", bspname);
		return false;
	}

	if (pfnProgressCallback) pfnProgressCallback(CallbackParam, .8f);
	mlog("RBspObject::Open : OpenBsp \n");

	char colfilename[_MAX_PATH];
	sprintf(colfilename, "%s.col", filename);
	if (!OpenCol(colfilename))
	{
		MLog("Error while loading %s\n", colfilename);
		return false;
	}

	// Äù½ºÆ®¸Ê¸¸ ³×ºñ°ÔÀÌ¼Ç ¸ÊÀ» ÀÐ´Â´Ù.
	char navfilename[_MAX_PATH];
	sprintf(navfilename, "%s.nav", filename);
	if (!OpenNav(navfilename))
	{
		//MLog("Error while loading %s\n",navfilename);
		//return false;
	}


	if (RIsHardwareTNL())
	{
		if (!CreateVertexBuffer())
			mlog("Error while Creating VB\n");
	}

	///Only for RBspObject
	if (m_bisDrawLightMap)
		OpenLightmap();
	
	CreateRenderInfo();

	mlog("RBspObject::Open : done\n");

	if (pfnProgressCallback) pfnProgressCallback(CallbackParam, 1.f);

	return true;*/
}

rvector ABspObject::GetFloor(D3DXVECTOR3& origin, float fRadius, float fHeight, D3DXPLANE* pimpactplane)
{
	return rvector(0, 0, 0);
}
