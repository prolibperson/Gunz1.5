#pragma once
#include "stdafx.h"
#include "../ZModule.h"

class BlitzAbility_Hunter : ZModule
{
public:
	DECLARE_ID(ZMID_HUNTER)
	BlitzAbility_Hunter();
	~BlitzAbility_Hunter();

	virtual bool Update(float elapsedtime) override;
	virtual void Active(bool bActive = true);	// (비)활성화 시킨다

private:
	float honorincratio;
};