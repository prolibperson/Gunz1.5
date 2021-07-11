#pragma once
#include <stdio.h>
#include <list>

#include "RTypes.h"
//#include "RPath.h"
#include "RLightList.h"
#include "RMeshMgr.h"
#include "RAnimationMgr.h"
#include "RMaterialList.h"
#include "ROcclusionList.h"
#include "RSolidBsp.h"
#include "RNavigationMesh.h"
#include "RDummyList.h"


struct FogInfo
{
	bool bFogEnable;
	DWORD dwFogColor;
	float fNear;
	float fFar;
	FogInfo() { bFogEnable = false; }
};

_NAMESPACE_REALSPACE2_BEGIN


struct RMATERIAL;
class RMaterialList;
class RBaseTexture;
class RSBspNode;


struct RDEBUGINFO {
	int nCall, nPolygon;
	int nMapObjectFrustumCulled;
	int nMapObjectOcclusionCulled;
	RSolidBspNode		*pLastColNode;
};

struct BSPVERTEX {

	float x, y, z;		// world position
						//float nx,ny,nz;		// normal				// 지금은 의미없다
	float tu1, tv1;		// texture coordinates
	float tu2, tv2;

	rvector *Coord() { return (rvector*)&x; }
	//rvector *Normal() { return (rvector*)&nx; }
};

//#define BSP_FVF	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2)
#define BSP_FVF	(D3DFVF_XYZ | D3DFVF_TEX2)

#define LIGHT_BSP_FVF	(D3DFVF_XYZ | D3DFVF_TEX2 | D3DFVF_DIFFUSE)

struct RPOLYGONINFO {
	rplane	plane;
	int		nMaterial;
	int		nConvexPolygon;
	int		nLightmapTexture;
	int		nPolygonID;
	DWORD	dwFlags;

	BSPVERTEX *pVertices;
	int		nVertices;
	int		nIndicesPos;
};

struct RCONVEXPOLYGONINFO {
	rplane	plane;
	rvector *pVertices;
	rvector *pNormals;
	int	nVertices;
	int	nMaterial;
	float fArea;
	DWORD	dwFlags;
};

struct ROBJECTINFO {
	string		name;
	int			nMeshID;
	RVisualMesh *pVisualMesh;
	RLIGHT		*pLight;
	float		fDist;
};

struct RBSPPICKINFO {
	RSBspNode* pNode;
	int nIndex;
	rvector PickPos;
	RPOLYGONINFO	*pInfo;
};

class RMapObjectList : public list<ROBJECTINFO*> {
public:
	RMapObjectList() = default;
	virtual ~RMapObjectList();

	iterator Delete(iterator i);
};

class RDrawInfo {
public:
	RDrawInfo() {
		nVertice = 0;
		pVertices = NULL;
		nIndicesOffset = 0;
		nTriangleCount = 0;
		pPlanes = NULL;
		pUAxis = NULL;
		pVAxis = NULL;

	}

	~RDrawInfo() {
		SAFE_DELETE(pVertices);
		SAFE_DELETE(pPlanes);
		SAFE_DELETE(pUAxis);
		SAFE_DELETE(pVAxis);
	}

	int				nVertice;		// 버텍스 수
	BSPVERTEX		*pVertices;		// 버텍스
	int				nIndicesOffset;	// index가 시작하는곳의 옵셋
	int				nTriangleCount;	// 삼각형 갯수
	rplane			*pPlanes;		// 평면의 방정식(삼각형개수만큼)
	rvector			*pUAxis;		// uv 계산에 필요한 기준축
	rvector			*pVAxis;		// uv 계산에 필요한 기준축
};

class RSBspNode
{
public:
	int				nPolygon;
	//	int				nPosition;		// vertex buffer 내의 위치.
	RPOLYGONINFO	*pInfo;			// 폴리곤 정보의 배열 시작위치
	RPOLYGONINFO	**ppInfoSorted;	// 소팅된 폴리곤 정보의 배열
	RDrawInfo		*pDrawInfo;		// material별 해당 폴리곤을 그리기위한 정보

	int				nFrameCount;		// 마지막 렌더링된 프레임..
	int				m_nBaseVertexIndex;	///< 인덱스들의 base vertex index
	int				m_nVertices;		///< 버텍스 수

										//	bool			bVisibletest;		// pvs 테스트용 . 임시.
										//	bool			bSolid;

	RSBspNode		*m_pPositive, *m_pNegative;

	rplane plane;
	rboundingbox	bbTree;

	RSBspNode();
	virtual ~RSBspNode();

	RSBspNode *GetLeafNode(rvector &pos);
	void DrawWireFrame(int nFace, DWORD color);
	void DrawBoundingBox(DWORD color);
};

// 자잘한 lightmap 을 큰 텍스쳐에 한번에 올리는걸 도와준다.

typedef list<POINT> RFREEBLOCKLIST;
struct RLIGHTMAPTEXTURE {
	int nSize;
	DWORD *data;
	bool bLoaded;
	POINT position;
	int	nLightmapIndex;
};

struct RBSPMATERIAL : public RMATERIAL {
	RBSPMATERIAL() { texture = NULL; }
	RBSPMATERIAL(RMATERIAL *mat)
	{
		Ambient = mat->Ambient;
		Diffuse = mat->Diffuse;
		DiffuseMap = mat->DiffuseMap;
		dwFlags = mat->dwFlags;
		Name = mat->Name;
		Power = mat->Power;
		Specular = mat->Specular;
	};
	RBaseTexture *texture;
};

class RBspLightmapManager {

public:
	RBspLightmapManager();
	virtual ~RBspLightmapManager();

	void Destroy();

	int GetSize() { return m_nSize; }
	DWORD *GetData() { return m_pData; }

	void SetSize(int nSize) { m_nSize = nSize; }
	void SetData(DWORD *pData) { Destroy(); m_pData = pData; }

	bool Add(DWORD *data, int nSize, POINT *retpoint);
	// 2^nLevel 크기의 사용되지않은 RECT를 빼내준다..
	bool GetFreeRect(int nLevel, POINT *pt);

	void Save(const char *filename);

	// 실행과는 관계가 없고. 단순히 참고하기 위한 데이터임
	// 남은 양 0~1 을 계산해서 m_fUnused로 넣는다
	float CalcUnused();
	float m_fUnused;

protected:
	RFREEBLOCKLIST * m_pFreeList;
	DWORD *m_pData;
	int m_nSize;
};


struct AmbSndInfo
{
	int itype;
	char szSoundName[64];
	rvector min;
	rvector center;
	rvector max;
	float radius;
};

#define AS_AABB		0x01
#define AS_SPHERE	0x02
#define AS_2D		0x10
#define AS_3D		0x20

// 에디터에서 Generate Lightmap에 쓰일 Progress bar 나타낼때 쓰는 콜백펑션 타입. 취소되었으면 리턴 = false
typedef bool(*RGENERATELIGHTMAPCALLBACK)(float fProgress);


///TODO: Continue filling in code as necessary, this = bare minimum to make the game run
class ABspObject {
public:
	ABspObject();
	virtual ~ABspObject();

	ROpenFlag m_OpenMode;


	//Interface Functions
	virtual bool Pick(const rvector &pos, const rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE);
	virtual bool PickTo(const rvector &pos, const rvector &to, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE) = 0;
	virtual bool PickOcTree(rvector &pos, rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE) = 0;
	virtual bool Draw() = 0;
	virtual bool CheckWall(rvector &origin, rvector &targetpos, float fRadius, float fHeight = 0.f, RCOLLISIONMETHOD method = RCW_CYLINDER, int nDepth = 0, rplane *pimpactplane = NULL) = 0;
	virtual bool CheckSolid(rvector &pos, float fRadius, float fHeight = 0.f, RCOLLISIONMETHOD method = RCW_CYLINDER) = 0;
	virtual bool GetShadowPosition(rvector& pos_, rvector& dir_, rvector* outNormal_, rvector* outPos_) = 0;
	virtual bool IsVisible(rboundingbox& bb) = 0;
	virtual void LightMapOnOff(bool b) = 0;
	virtual void SetWireframeMode(bool b) = 0;


	virtual void OnInvalidate() = 0;
	virtual void OnRestore() = 0;
	virtual void DrawObjects() = 0;
	virtual void OptimizeBoundingBox() = 0;
	virtual void DrawLight(D3DLIGHT9 *pLight) = 0;

	//Abstract Functions
	virtual bool Open(const char * bspName, const char* descExtension, ROpenFlag nOpenFlag = ROpenFlag::ROF_RUNTIME, RFPROGRESSCALLBACK pfnProgressCallback = NULL, void *CallbackParam = NULL);


	//End Abstract


	virtual rvector GetFloor(rvector &origin, float fRadius, float fHeight, rplane *pimpactplane = NULL) = 0;

	virtual RDummyList* GetDummyList() = 0;
	virtual RMapObjectList* GetMapObjectList() = 0;
	virtual RMeshMgr* GetMeshManager() = 0;
	virtual RBSPMATERIAL * GetMaterial(int nIndex) = 0;
	virtual RBSPMATERIAL * GetMaterial(RSBspNode *pNode, int nIndex) = 0;
	virtual FogInfo GetFogInfo() = 0;
	virtual list<AmbSndInfo*> GetAmbSndList() = 0;
	virtual RSBspNode * GetRootNode() { return m_pBspRoot; }
	virtual RNavigationMesh* GetNavigationMesh() = 0;
	virtual RLightList* GetObjectLightList() = 0;
	virtual RLightList *GetSunLightList() = 0;

	virtual void SetMapObjectOcclusion(bool b) = 0;
private:
	RSBspNode *m_pBspRoot, *m_pOcRoot;
	string m_filename, m_descfilename;


};

_NAMESPACE_REALSPACE2_END