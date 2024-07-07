#pragma once
#include "stdafx.h"


class BlitzAbility
{

public:
	BlitzAbility() noexcept {

	}

	virtual ~BlitzAbility() noexcept {

	}
protected:
	virtual void Run(float time);
	bool isactive;
};