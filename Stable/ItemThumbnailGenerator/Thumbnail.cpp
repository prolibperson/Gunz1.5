#include "StdAfx.h"
#include ".\thumbnail.h"
#include "MDebug.h"
#include "RealSpace2.h"

_USING_NAMESPACE_REALSPACE2

CThumbnail::CThumbnail()
: m_pTexture(0)
, m_pSurface(0)
, m_pOldRenderTarget(0)
{
}

CThumbnail::~CThumbnail()
{
	SAFE_RELEASE(m_pOldRenderTarget);
	SAFE_RELEASE(m_pSurface);
	SAFE_RELEASE(m_pTexture);
}
/* 렌더타겟 텍스처를 활용하려고 했는데, 그냥 백버퍼에서 특정 부위를 crop해오는게 낫겠다 싶어서 취소
bool CThumbnail::PrepareRenderTargetTexture()
{
	if(FAILED( D3DXCreateTexture( RGetDevice(), THUMBNAILSIZE, THUMBNAILSIZE, 1, D3DUSAGE_RENDERTARGET, 
		RGetPixelFormat(), D3DPOOL_DEFAULT, &m_pTexture)))
	{
		mlog( "RenderTarget Texture Creation Failed.\n" );
		return false;
	}

	if(FAILED(m_pTexture->GetSurfaceLevel( 0, &m_pSurface )))
	{
		mlog( "RenderTarget Texture GetSurface Failed.\n");
		return false;
	}

	RGetDevice()->GetRenderTarget(0, &m_pOldRenderTarget);
	RGetDevice()->SetRenderTarget(0, m_pSurface);
	
	return true;
}

void CThumbnail::RestoreRenderTargetTexture()
{
	RGetDevice()->SetRenderTarget(0, m_pOldRenderTarget);
	SAFE_RELEASE(m_pOldRenderTarget);
}
*/
void CThumbnailList::Add( int id, CThumbnail* pThumbnail )
{
	if (m_map.end() != m_map.find(id)) {
		mlog("썸네일 리스트에 중복된 id를 추가하려합니다(id:%d)\n", id);
		return;
	}

	m_map[id] = pThumbnail;
}