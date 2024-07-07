#pragma once
#include "d3d9.h"

#define THUMBNAILSIZE 64

class CThumbnail
{
	// ∑ª¥ı≈∏∞Ÿ ≈ÿΩ∫√≥
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DSURFACE9 m_pSurface;

	LPDIRECT3DSURFACE9 m_pOldRenderTarget;

public:
	CThumbnail();
	~CThumbnail();
	//bool PrepareRenderTargetTexture();
	//void RestoreRenderTargetTexture();
};


class CThumbnailList
{
	typedef std::map<int, CThumbnail*>	MapThumbnail;
	typedef MapThumbnail::iterator		ItorThumbnail;

	MapThumbnail m_map;

public:
	void Add(int id, CThumbnail* pThumbnail);
};