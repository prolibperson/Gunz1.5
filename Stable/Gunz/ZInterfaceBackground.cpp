#include "stdafx.h"
#include "ZApplication.h"
#include "ZInterfaceBackground.h"
#include "RealSpace2.h"
#include "ZMapDesc.h"
#include "MDebug.h"
#include "ZScreenEffectManager.h"
#include "ZConfiguration.h"

#ifdef _CHURCH

#define DIR_LOGIN	"interface/church"
ZInterfaceBackground::ZInterfaceBackground()
{
	RealSpace2::RSetFileSystem(ZApplication::GetFileSystem());
	m_pChurch = NULL;
	m_pChurchEnd = NULL;
	m_pMapDesc = NULL;
	m_pPigeonMesh = NULL;
	m_nSceneNumber = NULL;
	m_nPrevSceneNumber = NULL;
	m_bReverseCam = false;
	m_bForcedChange = false;
	m_elapsedTime = 0.f;
}

ZInterfaceBackground::~ZInterfaceBackground()
{
	if (m_pChurch != NULL)
		delete m_pChurch;
	m_pChurch = NULL;

	if (m_pChurchEnd != NULL)
		delete m_pChurchEnd;
	m_pChurchEnd = NULL;

	if (m_pMapDesc != NULL)
		delete m_pMapDesc;
	m_pMapDesc = NULL;

	if (m_pPigeonMesh != NULL)
		delete m_pPigeonMesh;
	m_pPigeonMesh = NULL;

	m_VMeshMgr.DelAll();
}

void ZInterfaceBackground::SetScene(int nSceneNumber)
{

	if (m_nSceneNumber != nSceneNumber)
	{
		m_bForcedChange = false;
		m_nPrevSceneNumber = m_nSceneNumber;
		m_nSceneNumber = nSceneNumber;
		m_nSceneChangedTime = timeGetTime();
	}
	m_bReverseCam = false;
}

void ZInterfaceBackground::SetRenderState()
{
	// Set rendering state
	RSetViewport(0, 0, RGetScreenWidth(), RGetScreenHeight());
	RGetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);					// from rbs rendering
	RGetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	RGetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);		// Texture filtering
	RGetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	RGetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

void ZInterfaceBackground::SetFogState(float fStart, float fEnd, unsigned long int color)
{
	RGetDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE);
	RGetDevice()->SetRenderState(D3DRS_FOGCOLOR, color);

	RGetDevice()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	RGetDevice()->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fStart));
	RGetDevice()->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fEnd));
}



void ZInterfaceBackground::OnUpdate(float fElapsed)
{
	//	ZGetEffectManager()->Update(fElapsed);
	//	ZGetScreenEffectManager()->UpdateEffects();
	m_elapsedTime += fElapsed;
}

void ZInterfaceBackground::OnInvalidate()
{
	if (m_pChurch)
		m_pChurch->OnInvalidate();

	if (m_pChurchEnd)
		m_pChurchEnd->OnInvalidate();
}


void ZInterfaceBackground::OnRestore()
{
	if (m_pChurch)
		m_pChurch->OnRestore();

	if (m_pChurchEnd)
		m_pChurchEnd->OnRestore();
}

void ZInterfaceBackground::ReverseCam()
{
	m_bReverseCam = true;
	m_nReverseCamTime = timeGetTime();
}

void ZInterfaceBackground::SetForcedChange(bool bForcedChange)
{
	m_bForcedChange = bForcedChange;
}

void ZInterfaceBackground::Draw()
{

	MWidget* pWidget = (MWidget*)ZApplication::GetGameInterface()->GetIDLResource()->FindWidget("CharSelection");
	if (pWidget)
	{
		if (ZApplication::GetGameInterface()->GetState() == GUNZ_CHARSELECTION)
			pWidget->Show(true);
	}

	MPicture* pPicture = (MPicture*)ZApplication::GetGameInterface()->GetIDLResource()->FindWidget("CharSel_TopScreen");
	if (pPicture)
		pPicture->SetOpacity(0);

	SetRenderState();

	const rvector CamPos(0, 0, 2200);	
	const rvector CamDir(0, 1, 1.5f);

	rvector ff = m_EndCamPos - (m_EndCamDir * 100);
	ff.z += 1;


	unsigned long int nCurrTime = timeGetTime();
	float fTimePerDistance = TIME_PER_DISTANCE;


	if (m_nSceneNumber == LOGIN_SCENE_FIXEDSKY)
	{

		void RSetProjection(float fFov, float fAspect, float fNearZ, float fFarZ);

		RSetProjection(D3DX_PI * 55 / 180, RGetScreenWidth() / (float)RGetScreenHeight(), 10.0f, 10000.0f);
		RSetCamera(CamPos, (CamPos + CamDir), rvector(0, 0, 1));

		SetFogState(4000.0f, 8000.0f, 0x008C6636);

		float fDiff = (int(m_elapsedTime * 1000) % TIME_PER_DISTANCE) / (float)TIME_PER_DISTANCE;

		for (int i = CHURCH_BLOCK_COUNT - 1; i >= 0; i--) {

			D3DXMatrixTranslation(&m_matWorld, 0, BLOCK_SIZE * (i - fDiff), 0);
			RGetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);

			m_pChurch->Draw();

		}

		UpdatePigeon();
	}
	else if (m_nSceneNumber == LOGIN_SCENE_FALLDOWN)
	{	
		if (m_nPrevSceneNumber == LOGIN_SCENE_FIXEDSKY)
		{
			RSetProjection(D3DX_PI * 60 / 180, RGetScreenWidth() / (float)RGetScreenHeight(), 10.0f, 15000.0f);

			fTimePerDistance = max(float(TIME_PER_DISTANCE) - float(nCurrTime - m_nSceneChangedTime) * 5, END_TIME_PER_DISTANCE);	// °¡¼Ó

			float fDiff = (nCurrTime - m_nSceneChangedTime + (m_nSceneChangedTime % TIME_PER_DISTANCE)) / (float)fTimePerDistance;
			float fLimit = (CHURCH_BLOCK_COUNT - 1) - (-m_EndCamPos.y - END_BLOCK_OFFSET) / BLOCK_SIZE;
			if (fDiff > fLimit) fDiff = fLimit;

			if (m_bForcedChange == true)
			{
				fTimePerDistance = END_TIME_PER_DISTANCE;
				fDiff = fLimit;
			}

			float t = fDiff / fLimit;

			SetFogState(4000.0f + 8000.0f * t, 8000.0f + 8000.0f * t, 0x008C6636);


			if (t == 1)
			{
				int yEndOffset = END_BLOCK_OFFSET;

				rvector cp = m_EndCamPos;
				rvector cd = m_EndCamDir;

				if (m_bReverseCam)
				{
					rvector cd2 = m_vCharDir;

					const float TIME_PER_DISTANCE_REV = 140.0f;
					const float TIME_PER_DISTANCE_END = 400.0f;
					const float fRevDiff = 500.0f;

					float fTimePerDist = max(float(TIME_PER_DISTANCE_REV) - float(nCurrTime - m_nReverseCamTime) * 5, TIME_PER_DISTANCE_REV);


					float fDiff;
					fDiff = min(fRevDiff, (float(nCurrTime - m_nReverseCamTime) / 1000.0f * fTimePerDist));

					rvector vDiff = cp + (fDiff * cd2);
					cp = vDiff;
				}

				RSetCamera(cp, (cp + cd), rvector(0, 0, 1));

				D3DXMatrixTranslation(&m_matWorld, 0, 0, 0);
				RGetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);

				m_pChurchEnd->Draw();
			}
			else
			{
				for (int i = ((t == 1) ? CHURCH_BLOCK_COUNT - 1 : 0); i < CHURCH_BLOCK_COUNT; i++) {
					int yEndOffset = 0;
					if (i == CHURCH_BLOCK_COUNT - 1) yEndOffset = END_BLOCK_OFFSET;

					float fYPos = BLOCK_SIZE * (i - fDiff) + yEndOffset;

					rvector ep(m_EndCamPos.x, 0, m_EndCamPos.z);
					rvector cp = CamPos + (ep - CamPos) * t;

					rvector cd = CamDir + (m_EndCamDir - CamDir) * t;

					RSetCamera(cp, (cp + cd), rvector(0, 0, 1));

					D3DXMatrixTranslation(&m_matWorld, 0, fYPos, 0);
					RGetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);

					if (i == CHURCH_BLOCK_COUNT - 1)
					{
						m_pChurchEnd->Draw();
					}
					else m_pChurch->Draw();
				}

			}
		}
		else
		{
			_ASSERT(0);
		}
	}
}

void ZInterfaceBackground::Free()
{
	if (m_pChurch != NULL)
		delete m_pChurch;
	m_pChurch = NULL;

	if (m_pChurchEnd != NULL)
		delete m_pChurchEnd;
	m_pChurchEnd = NULL;

	if (m_pMapDesc != NULL)
		delete m_pMapDesc;
	m_pMapDesc = NULL;

	if (m_pPigeonMesh != NULL)
		delete m_pPigeonMesh;
	m_pPigeonMesh = NULL;

	m_VMeshMgr.DelAll();
}

void ZInterfaceBackground::LoadMesh()
{
	if (m_pChurch == NULL)
	{
		m_pChurch = new RBspObject();

		m_pChurch->Open(DIR_LOGIN"/church_b.rs", "xml");

		m_pChurch->OptimizeBoundingBox();

		m_pChurch->SetMapObjectOcclusion(true);
	}

	if (m_pChurchEnd == NULL)
	{
		m_pChurchEnd = new RBspObject();

		m_pChurchEnd->Open(DIR_LOGIN"/church_a.rs", "xml");

		RDummyList* pDummyList = m_pChurchEnd->GetDummyList();

		for (RDummyList::iterator itor = pDummyList->begin(); itor != pDummyList->end(); ++itor)
		{
			RDummy* pDummy = *itor;

			if (_stricmp(pDummy->szName.c_str(), "wait_pos_01") == 0)
			{
				m_EndCamPos = pDummy->Position;
				m_EndCamDir = pDummy->Direction;
			}
			else if (_stricmp(pDummy->szName.c_str(), "spawn_solo_01") == 0)
			{
				m_vCharPos = pDummy->Position;
				m_vCharDir = pDummy->Direction;
			}
		}

		RMapObjectList* pMapObject = m_pChurchEnd->GetMapObjectList();

		list<ROBJECTINFO*>::iterator it;

		ROBJECTINFO* pInfo = NULL;

		for (auto const& it : pMapObject->m_MapObjectList)
		{

			pInfo = it;

			if (!pInfo->pVisualMesh) continue;

			if (pInfo) {// ÇÊ¿äÇÏ¸é xml ·Î »©±â..
				string str = "login_obj_ef_sky02.elu";
				if (pInfo->name == str) {
					pInfo->pVisualMesh->SetUVAnimation(-0.055f, 0.f);
				}
			}
		}


		// Map description
		if (m_pMapDesc == NULL)
		{
			m_pMapDesc = new ZMapDesc();
			m_pMapDesc->Open(m_pChurchEnd);

			if (m_pMapDesc) {

				char szBuf[128];
				sprintf(szBuf, "%s/smoke.xml", DIR_LOGIN);
				m_pMapDesc->LoadSmokeDesc(szBuf);
				mlog("ZInterfaceBackground::LoadMesh pMapDesc->LoadSmokeDesc \n");
			}
		}

		m_pChurchEnd->SetMapObjectOcclusion(true);
	}

	if (m_pPigeonMesh == NULL)
	{
		m_pPigeonMesh = new RMesh();
		m_pPigeonMesh->ReadXml(DIR_LOGIN"/dove.xml");
		m_pPigeonMesh->ReloadAnimation();
		m_pPigeonMesh->SetTextureRenderOnOff(true);
		CreatePigeon(5);
	}
}

int ZInterfaceBackground::SetRandomPigeon(RPigeonVisualMesh* pVMesh, float add)
{
	if (pVMesh)
	{

		float x = -400.f + rand() % 800;
		float y = (12000.f + add) + rand() % 3000;
		float z = 6500.f - rand() % 200;

		float speed = 25.f + rand() % 10;
		pVMesh->m_vPos = rvector(x, y, z);
		pVMesh->SetAnimation("fly2");
		pVMesh->SetMoveSpeed(speed);
		AniFrameInfo* pInfo = pVMesh->GetFrameInfo(ani_mode_lower);

		if (pInfo->m_pAniSet)
		{
			pInfo->m_nFrame = (rand() * 160) % pInfo->m_pAniSet->GetMaxFrame();
		}
		return 1;
	}
	return -1;
}

bool ZInterfaceBackground::CreatePigeon(int nCnt)
{
	RPigeonVisualMesh* node = NULL;
	RPigeonVisualMesh* pVMesh = NULL;
	int id = -1;

	for (int i = 0; i < nCnt; i++) {

		node = new RPigeonVisualMesh;

		if (!node->Create(m_pPigeonMesh))
		{
			return false;
		}

		id = m_VMeshMgr.Add(node);
		if (id == -1)
		{
			return false;
		}

		pVMesh = (RPigeonVisualMesh*)m_VMeshMgr.GetFast(id);

		pVMesh->SetCheckViewFrustum(false);

		SetRandomPigeon(pVMesh);
	}
	return true;
}

void ZInterfaceBackground::UpdatePigeon()
{
	int size = (int)m_VMeshMgr.m_list.size();

	RPigeonVisualMesh* pVMesh = NULL;
	rmatrix m;

	for (int j = 0; j < size; j++)
	{

		pVMesh = (RPigeonVisualMesh*)m_VMeshMgr.GetFast(j);

		if (pVMesh) {

			pVMesh->Move();
			m = pVMesh->GetMoveMatrix();
			pVMesh->SetWorldMatrix(m);

			pVMesh->Frame();

			if (pVMesh->m_vPos.y < -1200.f)
			{
				SetRandomPigeon(pVMesh, 18000.f);
			}
			pVMesh->Render();
		}
	}
}
#else

#define DIR_LOGIN	"interface/login"
ZInterfaceBackground::ZInterfaceBackground( void)
{
	RealSpace2::RSetFileSystem( ZApplication::GetFileSystem());

	m_pLogin = NULL;
	m_pMapDesc = NULL;
//	m_bShowMaietLogo = false;
}


ZInterfaceBackground::~ZInterfaceBackground( void)
{
	if ( m_pLogin != NULL)
		delete m_pLogin;
	m_pLogin = NULL;

	if ( m_pMapDesc != NULL)
		delete m_pMapDesc;
	m_pMapDesc = NULL;
}

void ZInterfaceBackground::LoadMesh( void)
{
#ifdef _FASTDEBUG
	return;
#endif

	if ( m_pLogin == NULL)
	{
		m_pLogin = new RBspObject();

#ifndef _DEBUG
		const char* descExt = "xml";
#else
		const char* descExt = "xml";
#endif
		if (!m_pLogin->Open(DIR_LOGIN"/login.rs", descExt))
			return;

		RDummyList* pDummyList = m_pLogin->GetDummyList();

		for ( RDummyList::iterator itor = pDummyList->begin();  itor != pDummyList->end();  ++itor)
		{
			RDummy* pDummy = *itor;

			if( _stricmp( pDummy->szName.c_str(), "camera_pos 01" ) == 0 )
			{
				m_vCamPosSt = pDummy->Position;
				m_vCamDirSt = pDummy->Direction;
			}
			else if( _stricmp( pDummy->szName.c_str(), "camera_pos 02" ) == 0 )
			{
				m_vCamPosEd = pDummy->Position;
				m_vCamDirEd = pDummy->Direction;
			}
			else if( _stricmp( pDummy->szName.c_str(), "spawn_solo_101" ) == 0 )
			{
				m_vCharPos = pDummy->Position;
				m_vCharDir = pDummy->Direction;
			}
		}

		for (auto const& mapObj : m_pLogin->GetSkyBoxList()->m_MapObjectList)
		{
			ROBJECTINFO* skyBox = mapObj;

			if (skyBox)
			{
				string str = "login_obj_ef_sky02.elu";
				if (skyBox->name == str)
				{
					skyBox->pVisualMesh->SetUVAnimation(-0.055f, 0.f);
				}
			}
		}
			

		// Map description
		if ( m_pMapDesc == NULL)
		{
			m_pMapDesc = new ZMapDesc();
			m_pMapDesc->Open( m_pLogin);

			if( m_pMapDesc ) {

				char szBuf[128];
				sprintf( szBuf, "%s/smoke.xml", DIR_LOGIN );
				m_pMapDesc->LoadSmokeDesc(szBuf);
				mlog("Load smoke description mesh.\n");
			}

		}

		m_pLogin->SetMapObjectOcclusion( true );
	}
}

void ZInterfaceBackground::Free( void)
{
	if ( m_pLogin != NULL)
		delete m_pLogin;
	m_pLogin = NULL;

	if ( m_pMapDesc != NULL)
		delete m_pMapDesc;
	m_pMapDesc = NULL;
}

void ZInterfaceBackground::OnUpdate(float fElapsed)
{
	ZGetEffectManager()->Update( fElapsed);
	ZGetScreenEffectManager()->UpdateEffects();
}

void ZInterfaceBackground::Draw(void)
{
#ifdef _FASTDEBUG
	MWidget* pWidget = (MWidget*)ZApplication::GetGameInterface()->GetIDLResource()->FindWidget( "CharSelection");
	if ( pWidget)
	{
		if ( ZApplication::GetGameInterface()->GetState() == GUNZ_CHARSELECTION)
			pWidget->Show( true);
	}

	MPicture* pPicture = (MPicture*)ZApplication::GetGameInterface()->GetIDLResource()->FindWidget( "CharSel_TopScreen");
	if ( pPicture)
		pPicture->SetOpacity( 0);

	return;
#endif

	// Set rendering state
	RSetViewport( 0, 0, RGetScreenWidth(), RGetScreenHeight());
	RGetDevice()->SetRenderState( D3DRS_CULLMODE  ,D3DCULL_CW);					// from rbs rendering
	RGetDevice()->SetRenderState( D3DRS_NORMALIZENORMALS , TRUE );
	RGetDevice()->SetSamplerState( 0, D3DSAMP_MINFILTER , D3DTEXF_LINEAR);		// Texture filtering
	RGetDevice()->SetSamplerState( 0, D3DSAMP_MAGFILTER , D3DTEXF_LINEAR);
	RGetDevice()->SetSamplerState( 0, D3DSAMP_MIPFILTER , D3DTEXF_LINEAR);

	// Set camera position and direction
    rvector vCamPos, vCamDir;
	float fForgDensity;

	switch ( m_nSceneNumber)
	{
		case  LOGIN_SCENE_FIXEDSKY :
		{
			// Fixed camera
			vCamPos = m_vCamPosSt;
			vCamDir = m_vCamDirSt;

			// Fixed fog -> black screen
			fForgDensity = 0.0f;

			break;
		}

		case  LOGIN_SCENE_MAIN:
		{
			DWORD dwClock = (timeGetTime() - m_dwClock);

			fForgDensity = dwClock * 15.0f;

			float fSeed = dwClock * 0.00035f;

			vCamPos = m_vCamPosEd + (m_vCamPosSt - m_vCamPosEd);
			vCamDir = m_vCamDirEd + (m_vCamDirSt - m_vCamDirEd);

			break;
		}

		case  LOGIN_SCENE_FALLDOWN :
		{
			// Get current clock
			DWORD dwClock = ( timeGetTime() - m_dwClock);

			/*
			// Show maiet logo
			if ( !m_bShowMaietLogo)
			{
				m_bShowMaietLogo = true;
				ZGetScreenEffectManager()->AddScreenEffect( "maiet_logo");
			}
			*/

			// Set fog density
			fForgDensity = dwClock * 15.0f;

			// Set wait time
			if ( dwClock < 500)
				dwClock = 0;
			else
				dwClock -= 500;

			// Play BGM music
			if ( dwClock > 2000)
				ZApplication::GetSoundEngine()->PlayMusic( true);


			// End of scroll camera
			float fSeed = dwClock * 0.00035f;			// Ä«¸Þ¶ó°¡ ÀüºÎ ´Ù ³»·Á¿À±â ±îÁö °É¸®´Â ½Ã°£( dwClock¿¡ °öÇØÁÖ´Â °ªÀÌ
														// ÀÛÀ»¼ö·Ï »¡¸® ³»·Á¿È)
			if ( fSeed > 3.14)
			{
				m_nSceneNumber = LOGIN_SCENE_FIXEDCHAR;
//				m_bShowMaietLogo = false;
			}


			// Move camera position & direction
			float fGain = ( cos( fSeed) + 1.0f) / 2.0f;			// 0 < fGain < 1.0
			vCamPos = m_vCamPosEd + ( m_vCamPosSt - m_vCamPosEd) * fGain;
			vCamDir = m_vCamDirEd + ( m_vCamDirSt - m_vCamDirEd) * fGain;

			//if (m_bShowMaietLogo)
			//{
			//	ZGetScreenEffectManager()->UpdateEffects();
				ZGetScreenEffectManager()->DrawEffects();
			//}

			break;
		}

		case  LOGIN_SCENE_FIXEDCHAR :
		{
			// Show menu UI
			MWidget* pWidget = (MWidget*)ZApplication::GetGameInterface()->GetIDLResource()->FindWidget( "CharSelection");
			if ( pWidget)
			{
				if ( ZApplication::GetGameInterface()->GetState() == GUNZ_CHARSELECTION)
					pWidget->Show( true);
			}
			MPicture* pPicture = (MPicture*)ZApplication::GetGameInterface()->GetIDLResource()->FindWidget( "CharSel_TopScreen");
			if ( pPicture)
				pPicture->SetOpacity( 0);

			// Play BGM music
			ZApplication::GetSoundEngine()->PlayMusic( true);

			// Fixed camera
			vCamPos = m_vCamPosEd;
			vCamDir = m_vCamDirEd;

			// Cleared fog
			fForgDensity = 50000.0f;
			break;
		}

		case  LOGIN_SCENE_SELECTCHAR :
		{
			// Get current clock
			DWORD dwClock = ( timeGetTime() - m_dwClock);
			float fGain = ( cos( dwClock * 0.0012f) + 1.0f) / 2.0f;
			#ifdef _MOVING_CAMERA_AFTER_CHAR_SEL
			vCamPos = m_vCamPosEd + ( m_vCamPosSt - m_vCamPosEd) * fGain;
			vCamDir = m_vCamDirEd + ( m_vCamDirSt - m_vCamDirEd) * fGain;
			#else
			vCamPos = m_vCamPosEd;
			vCamDir = m_vCamDirEd;
			#endif

			MPicture* pPicture = (MPicture*)ZApplication::GetGameInterface()->GetIDLResource()->FindWidget( "CharSel_TopScreen");
			if ( pPicture)
			{
				// Set wait time
				if ( dwClock < 2000)
					dwClock = 0;
				else
					dwClock -= 2000;

				// Set opacity of black screen
				int nOpacity = (int)( dwClock / 3.0f);
				if ( nOpacity > 255)
					nOpacity = 255;

				pPicture->SetOpacity( nOpacity);
			}

			// Cleared fog
			fForgDensity = 50000.0f;
			break;
		}
	}

	// Set camera
	RSetCamera( vCamPos, (vCamPos + vCamDir), rvector( 0, 0, 1));

	// Draw
	RSetProjection( D3DX_PI * 70 / 180, RGetScreenWidth() / (float)RGetScreenHeight(), 10.0f, 15000.0f);
	SetFogState( fForgDensity-10000.0f, fForgDensity, 0x00000000);
	D3DXMatrixTranslation( &m_matWorld, 0, 0, 0);
	RGetDevice()->SetTransform( D3DTS_WORLD, &m_matWorld);

	// Draw background
	if ( m_pLogin)
	{
		m_pLogin->Draw();
		m_pLogin->DrawObjects();
	}

	if ( m_pMapDesc)
		m_pMapDesc->DrawMapDesc();

	// Draw effects(smoke, cloud)
//	ZGetEffectManager()->Draw( timeGetTime());

	// Draw maiet logo effect
//	ZGetScreenEffectManager()->DrawEffects();
}


void ZInterfaceBackground::OnInvalidate()
{
#ifdef _FASTDEBUG
	return;
#endif

	if( m_pLogin)
		m_pLogin->OnInvalidate();
}


void ZInterfaceBackground::OnRestore()
{
#ifdef _FASTDEBUG
	return;
#endif

	if( m_pLogin)
		m_pLogin->OnRestore();
}


void ZInterfaceBackground::SetScene( int nSceneNumber)
{
	if ( nSceneNumber == m_nSceneNumber)
		return;

	if ( (nSceneNumber == LOGIN_SCENE_FALLDOWN) || (nSceneNumber == LOGIN_SCENE_SELECTCHAR))
		m_dwClock = timeGetTime();

	// ·Î°í¸¦ »Ñ·ÁÁØ´Ù

	#ifdef _MAIET_LOGO
	if( nSceneNumber == LOGIN_SCENE_FALLDOWN)
		ZGetScreenEffectManager()->AddScreenEffect( "maiet_logo");
	#endif

	#ifdef _MOVING_CAMERA_AFTER_CHAR_SEL
	if ( nSceneNumber == LOGIN_SCENE_SELECTCHAR)
	{
		m_vCamPosSt = m_vCamPosEd;
		m_vCamPosEd.y = -780;
		m_vCamPosEd.z = 60;

		m_vCamDirSt = m_vCamDirEd;
		m_vCamDirEd.x = -0.55f;
		m_vCamDirEd.z = 0.40f;
	}
	#endif

	m_nSceneNumber = nSceneNumber;
}

void ZInterfaceBackground::SetFogState( float fStart, float fEnd, unsigned long int color)
{
	RGetDevice()->SetRenderState( D3DRS_FOGENABLE, TRUE);
	RGetDevice()->SetRenderState( D3DRS_FOGCOLOR, color);

	RGetDevice()->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	RGetDevice()->SetRenderState( D3DRS_FOGSTART, *(DWORD *)(&fStart));
	RGetDevice()->SetRenderState( D3DRS_FOGEND,   *(DWORD *)(&fEnd));
}
#endif