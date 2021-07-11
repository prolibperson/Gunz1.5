#ifndef __RBSPOBJECT_H
#define __RBSPOBJECT_H

#include <stdio.h>
#include <list>
#include <thread>
#include "RTypes.h"
//#include "RPath.h"
#include "RLightList.h"
#include "RMeshMgr.h"
#include "RAnimationMgr.h"
#include "RMaterialList.h"
#include "ROcclusionList.h"
#include "RSolidBsp.h"
#include "RNavigationMesh.h"
#include "ABspObject.h"
class MZFile;
class MZFileSystem;

_NAMESPACE_REALSPACE2_BEGIN

///TODO: finish sorting this mess of a file, group all the bools/voids/etc together
class RBspObject : public ABspObject
{ 
public:
	ROpenFlag m_OpenMode;

	RBspObject();
	virtual ~RBspObject();

	void ClearLightmaps();

	// open 을 수행하면 기본 확장자로 다음의 Open...펑션들을 순서대로 부른다.
	virtual bool Open(const char *, const char* descExtension, ROpenFlag nOpenFlag=ROF_RUNTIME,RFPROGRESSCALLBACK pfnProgressCallback=NULL, void *CallbackParam=NULL) override;
	virtual bool IsVisible(rboundingbox &bb) override;		// occlusion 에 의해 가려져있으면 false 를 리턴.
	virtual bool Draw() override;
	virtual bool Pick(const rvector &pos, const rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE) override;
	virtual bool PickTo(const rvector &pos, const rvector &to, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE) override;
	virtual bool PickOcTree(rvector &pos, rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE) override;
	// origin 에서 targetpos 로 이동하는데 미끄러짐을 감안해서 targetpos 를 조절해서 리턴해준다.
	virtual bool CheckWall(rvector &origin, rvector &targetpos, float fRadius, float fHeight = 0.f, RCOLLISIONMETHOD method = RCW_CYLINDER, int nDepth = 0, rplane *pimpactplane = NULL) override;
	// solid 영역 안에 있는지 ?
	virtual bool CheckSolid(rvector &pos, float fRadius, float fHeight = 0.f, RCOLLISIONMETHOD method = RCW_CYLINDER) override;
	virtual bool GetShadowPosition(rvector& pos_, rvector& dir_, rvector* outNormal_, rvector* outPos_) override;

	bool OpenDescription(const char *);				// 디스크립션 파일		.xml 파일을 연다.
	bool OpenRs(const char *);						// 실제 월드 정보파일	.rs 파일을 연다. 
	bool OpenBsp(const char *);						// bsp 정보 파일		.bsp 파일을 연다. 
	bool OpenLightmap();							// 라이트맵 정보파일	.lm  파일을 연다.
//	bool OpenPathNode(const char *);				// 경로 정보파일		.pat 파일을 연다.
	bool OpenCol(const char *);						// collision 정보 파일	.col 파일을 연다. 
	bool OpenNav(const char *);						// 네비게이션맵 정보파일 .nav 파일을 연다.
	bool DrawLight(RSBspNode *pNode, int nMaterial);
	bool GenerateLightmap(const char *filename, int nMaxLightmapSize, int nMinLightmapSize, int nSuperSample, float fToler, RGENERATELIGHTMAPCALLBACK pProgressFn = NULL);
	bool GetWireframeMode() { return m_bWireframe; }
	bool GetShowLightmapMode() { return m_bShowLightmap; }

	virtual void OptimizeBoundingBox() override;	// 게임의 런타임에서는 실제바운딩박스로 타이트하게 잡아준다.
	virtual void DrawObjects() override;
	//Custom: Sky
	void DrawSky();
	void DrawFlags();
	virtual void DrawLight(D3DLIGHT9 *pLight) override;			// 광원 처리를.. 멀티 패스로 덧그린다.
	virtual void SetWireframeMode(bool bWireframe) override { m_bWireframe = bWireframe; }
	virtual void OnInvalidate() override;
	virtual void OnRestore() override;
	virtual void LightMapOnOff(bool b) override;

	void SetObjectLight(rvector pos);
	void SetCharactorLight(rvector pos);
	void DrawBoundingBox();
	void DrawOcclusions();
	void DrawNormal(int nIndex, float fSize = 1.f);	// index : convex polygon index

	void DrawCollision_Polygon();
	void DrawCollision_Solid();

	void DrawSolid();					// 모든 solid 노드들을 그린다
	void DrawSolidNode();				// 마지막 충돌한 노드를 그린다
	void DrawColNodePolygon(rvector &pos);

	void DrawNavi_Polygon();
	void DrawNavi_Links();
	void GetNormal(int nConvexPolygon, rvector &position, rvector *normal);
	void test_MakePortals();
	virtual void SetMapObjectOcclusion(bool b) override { m_bNotOcclusion = b; }
	void SetShowLightmapMode(bool bShowLightmap) { m_bShowLightmap = bShowLightmap; }

	//bool GeneratePathData(const char *filename,float fAngle, float fToler);
	//void GeneratePathNodeTable();

	static void SetDrawLightMap(bool b);
	static void DestroyShadeMap();



	// 월드 좌표 pos 에서 dir 방향의 반직선에 대해 pick !
	// PickTo() 역시 반직선 피킹이므로 함수명에 속지말고 결과가 true가 나왔을 때 거리 검사를 추가로 해주어야 원하는 결과를 얻을 수 있다


	// pathnode 는 봉인..
	/*
	// 화면 x,y 좌표에 있는 pathnode 를 리턴한다..
	bool PickPathNode(int x,int y,RPathNode **pOut,rvector *ColPos);
	// 월드 좌표 from 에서 to 로 가는 반직선에 대해 pick pathnode !
	bool PickPathNode(rvector &from,rvector &to,RPathNode **pOut,rvector *ColPos);

	RPathList *GetPathList() { return &m_PathNodes; }
	RPathNode *GetPathNode(RSBspNode *pNode,int nIndex);
*/

	// 해당 위치의 lightmap 을 얻어낸다.
	DWORD GetLightmap(rvector &Pos,RSBspNode *pNode,int nIndex);

	// 해당 폴리곤의 Material을 얻어낸다.
	virtual RBSPMATERIAL *GetMaterial(RSBspNode *pNode,int nIndex) override		{ return GetMaterial(pNode->pInfo[nIndex].nMaterial); }
	virtual RBSPMATERIAL *GetMaterial(int nIndex) override;

	// material 을 얻어낸다.
	int	GetMaterialCount()	{ return m_nMaterial; }

	virtual RMapObjectList	*GetMapObjectList() override { return &m_ObjectList; }
	//Custom: SkyBox
	RMapObjectList			*GetSkyBoxList() {return &m_SkyList;}
	void SetMapObjectList(RMapObjectList* objectList) { m_ObjectList =  *objectList; }
	virtual RDummyList		*GetDummyList()	override	 { return &m_DummyList; }
	RBaseTexture *GetBaseTexture(int n) { if(n>=0 && n<m_nMaterial) return m_pMaterials[n].texture; return NULL; }

	RLightList *GetMapLightList() { return &m_StaticMapLightList; }
	virtual RLightList *GetObjectLightList() override { return &m_StaticObjectLightList; }
	virtual RLightList *GetSunLightList() override { return &m_StaticSunLigthtList; }

	RSBspNode *GetOcRootNode() { return m_pOcRoot; }
	virtual RSBspNode *GetRootNode() override { return m_pBspRoot; }

	rvector GetDimension();

	int	GetVertexCount()		{ return m_nVertices; }
	int	GetPolygonCount()		{ return m_nPolygon; }
	int GetNodeCount()			{ return m_nNodeCount; }
	int	GetBspPolygonCount()	{ return m_nBspPolygon; }
	int GetBspNodeCount()		{ return m_nBspNodeCount; }
	int GetConvexPolygonCount() { return m_nConvexPolygon; }
	int GetLightmapCount()		{ return m_nLightmap; }
	float GetUnusedLightmapSize(int index) { return m_LightmapList[index]->m_fUnused; }


	
	// 위치에서 바닥에 닿는 점을 구한다.
	virtual rvector GetFloor(rvector &origin,float fRadius,float fHeight,rplane *pimpactplane=NULL) override;
	


	// 맵에 등록된 오브젝트 이외의 오브젝트 처리 ( 캐릭터 선택화면등 )



	virtual RMeshMgr*	GetMeshManager() override {
		return &m_MeshList;
	}


	// debug 를 위해 그려볼만한것들.
	//void DrawPathNode();


	RSolidBspNode *GetColRoot() { return m_pColRoot; }


	virtual FogInfo GetFogInfo() override { return m_FogInfo;}
	virtual list<AmbSndInfo*> GetAmbSndList() override { return m_AmbSndInfoList;	}


	static bool CreateShadeMap(const char *szShadeMap);

	RDEBUGINFO *GetDebugInfo() { return &m_DebugInfo; }
	virtual RNavigationMesh* GetNavigationMesh() override { return &m_NavigationMesh; }

	string m_filename;
private:

	string m_descfilename;
	
	// 에디터및 디버그를 위한 모드 
	bool m_bWireframe;
	bool m_bShowLightmap;
// 직접호출될일은 없다..
	bool DrawTNT(RSBspNode *bspNode,int nMaterial);	// no hardware T&L
	bool Draw(RSBspNode *bspNode,int nMaterial);

	void SetDiffuseMap(int nMaterial);

	bool Pick(RSBspNode *pNode, const rvector &v0, const rvector &v1);
//	bool PickCol(RSolidBspNode *pNode,rvector v0,rvector v1);
//	bool PickPathNode(RSBspNode *pNode);
	bool PickShadow(rvector &pos,rvector &to,RBSPPICKINFO *pOut);
	bool PickShadow(RSBspNode *pNode,rvector &v0,rvector &v1);

	void ChooseNodes(RSBspNode *bspNode);
	int ChooseNodes(RSBspNode *bspNode,rvector &center,float fRadius);			// 리턴값은 선택된 노드의 개수
//	void TraverseTreeAndRender(RSBspNode *bspNode);
//	void DrawNodeFaces(RSBspNode *bspNode);

	inline RSBspNode *GetLeafNode(rvector &pos) { return m_pBspRoot->GetLeafNode(pos); }

	//void GetFloor(rvector *ret,RSBspNode *pNode,rvector &origin,const rvector &diff);

// for loading
	bool ReadString(MZFile *pfile,char *buffer,int nBufferSize);
	bool Open_Nodes(RSBspNode *pNode,MZFile *pfile);
	bool Open_ColNodes(RSolidBspNode *pNode,MZFile *pfile);
	bool Open_MaterialList(MXmlElement *pElement);
	bool Open_LightList(MXmlElement *pElement);
	bool Open_ObjectList(MXmlElement *pElement);
	bool Open_DummyList(MXmlElement *pElement);
//	bool Open_LightList(MZFile *pfile);
	bool Open_ConvexPolygons(MZFile *pfile);
	bool Open_OcclusionList(MXmlElement *pElement);
public:
	bool Make_LenzFalreList();
	///Custom: update to make objects move in game
	void OnUpdate(float fElapsed);
protected:
	bool Set_Fog(MXmlElement *pElement);
	bool Set_AmbSound(MXmlElement *pElement);

	void CreateRenderInfo();
	void CreatePolygonTableAndIndexBuffer();
	void CreatePolygonTableAndIndexBuffer(RSBspNode *pNode);
	void Sort_Nodes(RSBspNode *pNode);

	bool CreateVertexBuffer();
	bool UpdateVertexBuffer();

	bool CreateIndexBuffer();
	bool UpdateIndexBuffer();

	bool CreateDynamicLightVertexBuffer();
	void InvalidateDynamicLightVertexBuffer();
	bool FlushLightVB();
	bool LockLightVB();
	LPDIRECT3DVERTEXBUFFER9 m_pDynLightVertexBuffer;

	static RBaseTexture *m_pShadeMap;

// 실제 트리
	BSPVERTEX *m_pBspVertices,*m_pOcVertices;
	WORD	*m_pBspIndices,*m_pOcIndices;
	RSBspNode *m_pBspRoot,*m_pOcRoot;
	RPOLYGONINFO *m_pBspInfo,*m_pOcInfo;
	int m_nPolygon,m_nNodeCount,m_nVertices,m_nIndices;
	int m_nBspPolygon,m_nBspNodeCount,m_nBspVertices,m_nBspIndices;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	
// Materials & texture map
	int m_nMaterial;
	RBSPMATERIAL *m_pMaterials;

	rplane m_localViewFrustum[6];

// occlusions
	ROcclusionList m_OcclusionList;

	/*
	int m_nOcclusion;
	ROcclusion	*m_pOcclusion;
	*/


// 실제로 텍스쳐 메모리에 올라가는 라이트맵
	int							m_nLightmap;
	LPDIRECT3DTEXTURE9			*m_ppLightmapTextures;
	vector<RBspLightmapManager*> m_LightmapList;

// lightmap 텍스처로 생성시 필요한 것..
	void CalcLightmapUV(RSBspNode *pNode,int *pLightmapInfo,vector<RLIGHTMAPTEXTURE*> *pLightmaps);

// interpolated normal
	void GetNormal(RCONVEXPOLYGONINFO *poly,rvector &position,rvector *normal,int au,int av);
// interpolated uv
	void GetUV(rvector &Pos,RSBspNode *pNode,int nIndex,float *uv);

// 원본 폴리곤 보관
	int					m_nConvexPolygon,m_nConvexVertices;
	rvector				*m_pConvexVertices;
	rvector				*m_pConvexNormals;
	RCONVEXPOLYGONINFO	*m_pConvexPolygons;

// 광원정보들..
	rvector		m_AmbientLight;
	RLightList	m_StaticMapLightList;
	RLightList	m_StaticObjectLightList;
	RLightList	m_StaticSunLigthtList;

// pathfinding 에 필요한 자료
//	RPathList m_PathNodes;

// 맵에 기본적으로 있는 object 들..
	RMeshMgr			m_MeshList;
	RAnimationMgr		m_AniList;
	RMapObjectList		m_ObjectList;
	RMapObjectList		m_SkyList;
	RMapObjectList		m_FlagList;
	bool				m_bNotOcclusion;

// 충돌체크용 맵
	RSolidBspNode		*m_pColRoot;
	rvector				*m_pColVertices;

// 네비게이션용 맵
	RNavigationMesh		m_NavigationMesh;


// 더미 리스트
	RDummyList	m_DummyList;

// Fog정보
	FogInfo m_FogInfo;

// 사운드 정보
	list<AmbSndInfo*>	m_AmbSndInfoList;

// 디버그 정보
	RDEBUGINFO			m_DebugInfo;
};

_NAMESPACE_REALSPACE2_END


#endif