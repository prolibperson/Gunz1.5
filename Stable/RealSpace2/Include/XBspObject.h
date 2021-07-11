#pragma once
#include "ABspObject.h"

///TODO: write dx11 renderer
class XBspObject : public ABspObject
{
public:
	XBspObject();
	virtual ~XBspObject();

private:
	//Interface Functions
	virtual bool Pick(const rvector &pos, const rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE) override ;
	virtual bool PickTo(const rvector &pos, const rvector &to, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE) override;
	virtual bool PickOcTree(rvector &pos, rvector &dir, RBSPPICKINFO *pOut, DWORD dwPassFlag = RM_FLAG_ADDITIVE | RM_FLAG_USEOPACITY | RM_FLAG_HIDE) override;
	virtual bool Draw() override;
	virtual bool CheckWall(rvector &origin, rvector &targetpos, float fRadius, float fHeight = 0.f, RCOLLISIONMETHOD method = RCW_CYLINDER, int nDepth = 0 , rplane *pimpactplane = NULL) override;
	virtual bool CheckSolid(rvector &pos, float fRadius, float fHeight = 0.f, RCOLLISIONMETHOD method = RCW_CYLINDER) override;
	virtual bool GetShadowPosition(rvector& pos_, rvector& dir_, rvector* outNormal_, rvector* outPos_) override;
	virtual bool IsVisible(rboundingbox& bb) override;
	virtual bool Open(const char * bspName , const char* descExtension, ROpenFlag nOpenFlag = ROpenFlag::ROF_RUNTIME, RFPROGRESSCALLBACK pfnProgressCallback = NULL, void *CallbackParam = NULL) override ;
	virtual void LightMapOnOff(bool b) override {/*empty, XBspObject won't use*/}
	virtual void SetWireframeMode(bool b) override {/*empty, will end up doing though*/ }


	virtual void OnInvalidate() override;
	virtual void OnRestore() override;
	virtual void DrawObjects() override;
	virtual void OptimizeBoundingBox() override;
	virtual void DrawLight(D3DLIGHT9 *pLight) override {/*nothing for now*/};
	virtual void SetMapObjectOcclusion(bool b) override {};



	virtual rvector GetFloor(rvector &origin, float fRadius, float fHeight, rplane *pimpactplane = NULL) override;

	//return nullptrs for now, this will be setup later
	virtual RDummyList* GetDummyList() override { return nullptr; }
	virtual RMapObjectList* GetMapObjectList() override { return nullptr; }
	virtual RMeshMgr* GetMeshManager() override { return nullptr; }
	virtual RBSPMATERIAL * GetMaterial(int nIndex) override { return nullptr; }
	virtual RBSPMATERIAL * GetMaterial(RSBspNode *pNode, int nIndex) override { return nullptr; }
	virtual FogInfo GetFogInfo() override { return {}; }
	virtual list<AmbSndInfo*> GetAmbSndList() override { return {}; }
	virtual RSBspNode * GetRootNode() override { return nullptr; }
	virtual RNavigationMesh* GetNavigationMesh() override { return nullptr; }
	virtual RLightList* GetObjectLightList() override { return nullptr; }
	virtual RLightList *GetSunLightList() override { return nullptr; }
};