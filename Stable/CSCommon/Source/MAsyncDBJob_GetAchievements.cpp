#include "stdafx.h"
#include "MAsyncDBJob_GetAchievements.h"

bool MAsyncDBJob_GetAchievements::Input(int nCID)
{
	m_nCID = nCID;

	return true;
}


void MAsyncDBJob_GetAchievements::Run(void* pContext)
{
	MMatchDBMgr* pDBMgr = (MMatchDBMgr*)pContext;

	if (!pDBMgr->GetCharacterAchievements(m_nCID,achievements))
	{
		SetResult(MASYNC_RESULT_FAILED);
		return;
	}

	SetResult(MASYNC_RESULT_SUCCEED);
}
