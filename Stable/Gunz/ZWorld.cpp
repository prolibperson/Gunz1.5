#include "stdafx.h"
#include "ZWorld.h"
#include "ZMap.h"
#include "ZMapDesc.h"
#include "ZSkyBox.h"
#include "ZInitialLoading.h"
#include "ZConfiguration.h"
#include "ZMapCache.h"
#include <memory>

ZWorld::ZWorld() : m_pBsp(NULL), m_pMapDesc(NULL), m_nRefCount(1), m_bCreated(false)
{
#ifdef _MAP_CACHING
	if (!ZGetMapCache()->Find(ZGetGameClient()->GetMatchStageSetting()->GetMapIndex()))
		m_pBsp = nullptr;
#else
	m_pBsp = nullptr;
#endif
	m_szName[0]=0;

	m_bFog = false;
	m_dwFogColor = 0xFFFFFFFF;
	m_fFogNear = 0;
	m_fFogFar = 0;

	m_bWaterMap = false;
	m_fWaterHeight = 0.f;
	m_szBspName[0] = 0;

}

ZWorld::~ZWorld()
{
	Destroy();
}

void ZWorld::Update(float fDelta)
{
	UpdateMapObjects(fDelta);

	if(m_bWaterMap)
		m_waters.Update();

	m_flags.Update();

	////TODO: create movable objects with animations so things like elevators and platforms can be made to move in the future
	((RBspObject*)m_pBsp)->OnUpdate(fDelta);

}

void ZWorld::Draw()
{
	//Custom: Skybox now handled in rbspobject
	//if( m_pSkyBox != NULL )
	//{
 //		m_pSkyBox->Render();
	//}


	// farz clipping 을 위해 farz plane 을 다시 계산해준다
	if(m_bFog) {
		ComputeZPlane(RGetViewFrustum()+5,m_fFogFar,-1);
	}

	m_pBsp->Draw();


	RealSpace2::g_poly_render_cnt = 0;

	//render after world but before flags
	RenderMapObjects();

	__BP(16,"ZGame::Draw::flags");

	//custom: render and update flags in rbspobject (TODO: make the flags move)
	m_flags.Draw();

	__EP(16);
}

void ZWorldProgressCallBack(void *pUserParam,float fProgress)
{
	ZLoadingProgress *pLoadingProgress = (ZLoadingProgress*)pUserParam;
	pLoadingProgress->UpdateAndDraw(fProgress);
}

bool ZWorld::Create(ZLoadingProgress *pLoading )
{
	if(m_bCreated) return true;
	///TODO: finish xbspobject
#ifdef _MAP_CACHING
	if (!ZGetMapCache()->Find(m_szName))
	{
		m_pBsp = new RBspObject;
	}
#else
		m_pBsp = new RBspObject;
#endif

#ifndef _MAP_CACHING

	//Custom: tod lighting
	if (ZGetGameClient()->GetMatchStageSetting()->IsQuestDrived() || ZGetGameClient()->GetMatchStageSetting()->GetGameType() == MMATCH_GAMETYPE_QUEST_CHALLENGE)
	{
		m_pBsp->SetLightMapIndex(0);
	}
	else
	{
		m_pBsp->SetLightMapIndex(ZGetGameClient()->GetMatchStageSetting()->GetLightMapIndex());
	}

	if(!m_pBsp->Open(m_szBspName, "xml", ROF_RUNTIME,ZWorldProgressCallBack,pLoading))
	{
		MLog("error while loading %s \n",m_szName);
		ZGetWorldManager()->Clear();
		return false;
	}

	m_pBsp->OptimizeBoundingBox();


#else
	if (!ZGetGameClient()->GetMatchStageSetting()->IsQuestDrived() && ZGetGameClient()->GetMatchStageSetting()->GetGameType() != MMATCH_GAMETYPE_QUEST_CHALLENGE)
	{
		if (ZGetMapCache()->Find(m_szName)) {
			m_pBsp = ZGetApplication()->FindCachedMap(m_szName);
			if (m_pBsp == nullptr) {
				MLog("error while loading %s \n", m_szName);
				ZGetWorldManager()->Clear();
				return false;
			}
			((RBspObject*)m_pBsp)->Make_LenzFalreList();
		}
		else {
			m_pBsp = new RBspObject();

			if (!m_pBsp->Open(m_szBspName, "xml", ROF_RUNTIME, ZWorldProgressCallBack, pLoading))
			{
				MLog("error while loading %s \n", m_szName);
				// 맵로딩에 실패하면 저장해둔 맵정보를 모두 지워준다.(버그수정:로딩에 실패할경우 실패된 맵정보를 그냥 저장 했었다)
				ZGetWorldManager()->Clear();
				return false;
			}
			((RBspObject*)m_pBsp)->Make_LenzFalreList();

		}
	}
	else {
		if (ZGetMapCache()->Find(m_szName)) {
			m_pBsp = ZGetApplication()->FindCachedMap(m_szName);
			//	m_pBsp = new RBspObject();
			if (m_pBsp == nullptr) {
				MLog("error while loading %s \n", m_szName);
				ZGetWorldManager()->Clear();
				return false;
			}
		}
		else {
			m_pBsp = new RBspObject();

			if (!m_pBsp->Open(m_szBspName, "xml", ROF_RUNTIME, ZWorldProgressCallBack, pLoading))
			{
				MLog("error while loading %s \n", m_szName);
				// 맵로딩에 실패하면 저장해둔 맵정보를 모두 지워준다.(버그수정:로딩에 실패할경우 실패된 맵정보를 그냥 저장 했었다)
				ZGetWorldManager()->Clear();
				return false;
			}
		}
		((RBspObject*)m_pBsp)->Make_LenzFalreList();

	}
	m_pBsp->OptimizeBoundingBox();
#endif


	char szMapPath[64] = "";
	ZGetCurrMapPath(szMapPath);

	ZWater*		water_instance;
	RMapObjectList* map_object_list		= m_pBsp->GetMapObjectList();
	RMeshMgr* mesh_mgr					= m_pBsp->GetMeshManager();

	for( auto iter = map_object_list->m_MapObjectList.begin(); iter != map_object_list->m_MapObjectList.end(); )
	{
		ROBJECTINFO* object_info		= *iter;
		RMesh* pMesh						= mesh_mgr->GetFast( object_info->nMeshID );
		if( pMesh->m_data_num <= 0 )
		{
			++iter;
			continue;
		}
		RMeshNode* pMeshNode		= pMesh->m_data[0];

		char* object_name = (char*)object_info->name.c_str();

		int len = int(strlen(m_szName)+1);
		object_name += len;

		//Custom: Render flags in rbspobject, as well as skybox. flags do not function fully let, skybox does
		if( pMeshNode->m_point_color_num > 0 ) // 맵에서 칼라값을 하나 이상 갖는 놈은 깃발이다.. 현재..
		{
			ZClothEmblem* new_instance	= new ZClothEmblem;
			new_instance->CreateFromMeshNode( pMeshNode , this );
			m_flags.Add( new_instance, object_name );
			iter	= map_object_list->Delete( iter );
			continue;
		}

		// Water
		//		if( g_pGame->m_bReflection )
		{
			// 임시....
			int nWater = 0;

			if( !strncmp( object_name, "obj_water", 9 ) )	nWater = 1;
			if( !strncmp( object_name, "obj_water2", 10 ) )	nWater = 3;//일렁임은 제거..
			if( !strncmp( object_name, "obj_sea", 7 ) )		nWater = 2;

			if( nWater ) {
				m_bWaterMap = true;
				m_fWaterHeight = pMeshNode->m_mat_base._42;
			}
			else {
				m_bWaterMap = false;
				m_fWaterHeight = 0.f;
			}

			if(nWater)	
//			if( !strncmp( object_name, "obj_water", 9 ) )
			{
				int id = object_info->nMeshID;

				RMesh* mesh = mesh_mgr->GetFast(id);
				RMeshNode* node = mesh->m_data[0];

				water_instance = new ZWater;

				water_instance->SetMesh(node);
				m_waters.push_back( water_instance );

					 if(nWater==1) water_instance->m_nWaterType = WaterType1;
//				else if(nWater==2) water_instance->m_nWaterType = WaterType1;
				else if(nWater==3) water_instance->m_nWaterType = WaterType2;
	

				if(nWater==2) 
				{ // 물 예외 처리 가능할때 통합..
//					water_instance->m_nWaterType = 1;//sea
					water_instance->m_isRender = false; // 안 그리고 오브젝트로 그린다..
					pMesh->m_LitVertexModel = true;		// 바다는 라이트 영향을 안받는다..
					++iter;
				}
				else 
				{
					iter = map_object_list->Delete( iter );
				}

//				iter = map_object_list->Delete( iter );

				continue;
			}
		}

		++iter;
	}

	char szBuf[128];
	
	if(  m_flags.size() > 0 )
	{
		sprintf( szBuf, "%s%s/flag.xml", szMapPath, ZGetGameClient()->GetMatchStageSetting()->GetMapName());
		m_flags.InitEnv(szBuf);

		// mlog("ZGame::Create() m_flags.InitEnv \n");
		mlog( "create game enrvironment.\n" );
	}

	m_pMapDesc = new ZMapDesc;
	m_pMapDesc->Open(m_pBsp);

	sprintf( szBuf, "%s%s/smoke.xml", szMapPath, ZGetGameClient()->GetMatchStageSetting()->GetMapName());
	m_pMapDesc->LoadSmokeDesc(szBuf);

	// mlog("ZGame::Create() pMapDesc->LoadSmokeDesc \n");

	// 맵레벨의 전역값 설정
	FogInfo finfo = GetBsp()->GetFogInfo();
	m_bFog = finfo.bFogEnable;
	m_fFogNear = finfo.fNear;
	m_fFogFar = finfo.fFar;
	m_dwFogColor = finfo.dwFogColor;

	if (ZGetGameClient()->GetMatchStageSetting()->GetGameType() == MMATCH_GAMETYPE_INFECTION)
	{
		m_fFogNear = 600.f;
		m_fFogFar = 2500.f;
		m_dwFogColor = 0xFF3333;
		m_bFog = true;
	}

	LoadWorldObjects();

	m_bCreated = true;



	mlog( "game world create success.\n" );

	return true;
}

void ZWorld::Destroy()
{
#ifdef _MAP_CACHING
	if (!ZGetMapCache()->Find(m_szName))
		SAFE_DELETE(m_pBsp);

#else
	SAFE_DELETE(m_pBsp);
#endif

	SAFE_DELETE(m_pMapDesc);

	m_flags.Clear();
	m_flags.OnInvalidate();
	m_waters.Clear();
	m_waters.OnInvalidate();

	//mapObjects.clear();
//	SAFE_DELETE(m_pSkyBox);

}

void ZWorld::OnInvalidate()
{
	m_pBsp->OnInvalidate();
//	m_waters.OnInvalidate();
	m_flags.OnInvalidate();
}

void ZWorld::OnRestore()
{
	m_pBsp->OnRestore();
//	m_waters.OnRestore();
	m_flags.OnRestore();
}

void ZWorld::LoadWorldObjects()
{
	char szMapPath[64] = "";
	ZGetCurrMapPath(szMapPath);

	char szBuf[256];

	sprintf(szBuf, "%s%s/objects.xml", szMapPath, ZGetGameClient()->GetMatchStageSetting()->GetMapName());

	MXmlDocument	Data;
	Data.Create();

	MZFile mzf;

	if (!mzf.Open(szBuf, g_pFileSystem))
	{
		return;
	}

	char* buffer;
	buffer = new char[mzf.GetLength() + 1];
	mzf.Read(buffer, mzf.GetLength());
	buffer[mzf.GetLength()] = 0;

	if (!Data.LoadFromMemory(buffer))
	{
		delete[] buffer;
		mlog("Error opening objects file\n");
		return;
	}

	delete[] buffer;
	mzf.Close();

	MXmlElement rootElement = Data.GetDocumentElement();
	int childCount = rootElement.GetChildNodeCount();

	for (int i = 0; i < childCount; ++i)
	{
		MXmlElement child = rootElement.GetChildNode(i);
		char TagName[256];
		child.GetTagName(TagName);
		if (TagName[0] == '#')
		{
			continue;
		}

		if (strcmp(TagName, "OBJECT") == 0)
		{
			WorldObject worldObject;
			child.GetAttribute(&worldObject.name, "name");
			child.GetAttribute(&worldObject.model, "model");
			child.GetAttribute(&worldObject.collradius, "collradius", 0);
			child.GetAttribute(&worldObject.collwidth, "collwidth", 0);
			child.GetAttribute(&worldObject.collheight, "collheight", 0);
			child.GetAttribute(&worldObject.collidable, "collidable", false);
			child.GetAttribute(&worldObject.reverseanimation, "reverseanimation", false);
			child.GetAttribute(&worldObject.sound, "sound", "");

			bool movable = false;
			bool usepath = false;
			child.GetAttribute(&movable, "moving", false);
			child.GetAttribute(&usepath, "usepath", false);

			int objectChildCount = child.GetChildNodeCount();
			for (int j = 0; j < objectChildCount; ++j)
			{
				MXmlElement subChild = child.GetChildNode(j);
				subChild.GetTagName(TagName);
				if (strcmp(TagName, "POSITION") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					rvector pos(0, 0, 0);
					sscanf(contents.c_str(), "%f,%f,%f", &worldObject.position.x,&worldObject.position.y,&worldObject.position.z);
				}
				if (strcmp(TagName, "DIRECTION") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					rvector pos(0, 1, 0);
					sscanf(contents.c_str(), "%f,%f,%f", &worldObject.direction.x, &worldObject.direction.y, &worldObject.direction.z);
				}
				if (strcmp(TagName, "SPEED") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					worldObject.speed = std::stof(contents);
				}
				if (strcmp(TagName, "ANIMATION") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					worldObject.animation = contents;
				}
				if (strcmp(TagName, "SCALE") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					sscanf(contents.c_str(), "%f,%f,%f", &worldObject.scale.x, &worldObject.scale.y, &worldObject.scale.z);
				}
				if (strcmp(TagName, "COLLISION") == 0)
				{
					std::string contents;
					subChild.GetContents(&contents);
					worldObject.collisiontype = static_cast<COLLTYPE>(std::stoi(contents));
				}
				if (strcmp(TagName, "ENDPOSITION") == 0)
				{
					std::string contents;
					rvector endpos(-1, -1, -1);
					subChild.GetContents(&contents);
					sscanf(contents.c_str(), "%f,%f,%f", &worldObject.endposition.x, &worldObject.endposition.y, &worldObject.endposition.z);
				}
			}
			std::unique_ptr<ZWorldObject> object;
			if (usepath == true || movable == true)
				worldObject.objecttype = OBJTYPE::OT_MOVABLE;
			if (usepath == true)
				object = std::make_unique<ZWorldObject_Navigation>();
			else if (movable == true)
				object = std::make_unique<ZWorldObject_Movable>();
			else
				object = std::make_unique<ZWorldObject>();

			if (object->InitWithMesh(worldObject))
			{
				mapObjects.push_back(std::move(object));
			}
		}
	}
}



ZWorldObject* ZWorld::PickWorldObject(rvector& pos, rvector& dir)
{
	rvector realfloor = GetBsp()->GetFloor(pos, CHARACTER_RADIUS, CHARACTER_HEIGHT, nullptr);// , fHeight, pimpactplane);

	for (auto const& worldObject : mapObjects)
	{
		if (worldObject->Pick(pos, dir, nullptr))
		{
			if (realfloor.z >= worldObject->GetPosition().z + worldObject->GetHeight())
				continue;
			return worldObject.get();
		}
	}
	return nullptr;
}

ZWorldObject* ZWorld::CheckWallHang(rvector const& pos, rvector const& dir, bool const& initial)
{
	for (auto const& worldObject : mapObjects)
	{
		if (worldObject->OnCheckWallHang(pos, dir,initial) == true)
		{
			return worldObject.get();
		}
	}
	return nullptr;
}

ZWorldObject* ZWorld::CheckStandingOnObject(ZObject* const Object)
{
	rvector realfloor = GetBsp()->GetFloor(const_cast<rvector&>(Object->GetPosition()), CHARACTER_RADIUS, CHARACTER_HEIGHT, nullptr);// , fHeight, pimpactplane);

	for (auto const& worldObject : mapObjects)
	{
		if (worldObject->IsStandingOn(Object))
		{
			if (realfloor.z >= worldObject->GetPosition().z + worldObject->GetHeight())
				continue;
			return worldObject.get();
		}
	}
	return nullptr;
}

ZWorldObject* ZWorld::CheckWallRun(rvector const& pos, rvector const& dir, rvector& out)
{

	for (auto const& worldObject : mapObjects)
	{
		if (worldObject->CheckWallRun(pos, dir, out))
		{
			return worldObject.get();
		}
	}
	return nullptr;
}

rvector ZWorld::GetFloor(rvector& origin, float fRadius, float fHeight, rplane* pimpactplane)
{
	rvector realfloor = GetBsp()->GetFloor(origin, fRadius, fHeight, pimpactplane);
	ZWorldObject* worldObject = CheckStandingOnObject(ZGetGame()->m_pMyCharacter);// origin, rvector(0, 1, 0));
	if (worldObject != nullptr && worldObject->IsCollidable())
	{
		rvector floor = worldObject->GetPosition() + rvector(worldObject->GetLength(), worldObject->GetWidth(), worldObject->GetHeight());
		if (realfloor.z < floor.z)
			return floor;
	}

	return realfloor;
}

void ZWorld::SetFog(bool bFog)
{
	if(bFog) {
		RSetFog( m_bFog, m_fFogNear, m_fFogFar, m_dwFogColor );
	}
	else {
		RSetFog( FALSE );
	}
}



void ZWorld::RenderMapObjects()
{
	for (auto const& mapObjs : mapObjects)
	{
		mapObjs->Draw();//(0);
	}
}

void ZWorld::UpdateMapObjects(float delta)
{
	for (auto const& mapObjs : mapObjects)
	{
		mapObjs->Update(delta);
	}
}
