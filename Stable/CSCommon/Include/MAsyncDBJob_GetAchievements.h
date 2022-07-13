#pragma once
#include "MAsyncDBJob.h"
#

class MAsyncDBJob_GetAchievements : public MAsyncJob
{	
public:
	int m_nCID;


	MAsyncDBJob_GetAchievements(const MUID& uidOwner) :
		MAsyncJob(MASYNCDBJOB_GETACHIEVEMENTS, uidOwner) {}

	virtual ~MAsyncDBJob_GetAchievements() {}

	bool Input(int nCID);

	virtual void Run(void* pContext);

	std::vector<MTD_Achievement> achievements;

};