#include "stdafx.h"

#include "AnticheatMain.h"

#include "AntiHook.h"
#include "AntiShotbot.h"

void ZInitAnticheat() {
	ZPerformHooks();
	//AntiShotbotLogger();
}