#ifndef _ZBRAIN_H
#define _ZBRAIN_H

class ZActor;
class ZNavigationMesh;

#include "ZActorAnimation.h"
#include "ZTaskID.h"
#include "ZTask.h"
#include "ZTaskManager.h"
#include "ZBehavior.h"
#include "ZTimer.h"

#define		DIST_FORCEDIN		300000.0f
#define		DIST_IN				1600000.0f
#define		DIST_OUT			5000000.0f
#define		DIST_HEIGHT			320.0f


///< Actor의 AI주체
class ZBrain
{
private:

	// Timer
	ZUpdateTimer		m_PathFindingTimer;
	ZUpdateTimer		m_AttackTimer;
	ZUpdateTimer		m_DefaultAttackTimer;
	DWORD				m_dwNoSkillTimer;
	DWORD				m_dwNeglectTimer;

	float				m_fDistForcedIn;
	float				m_fDistIn;
	float				m_fDistOut;

protected:
	ZActor*				m_pBody;			///< 몸체
	ZBehavior			m_Behavior;
	MUID				m_uidTarget;		///< 타게팅하고 있는 적 UID
	
	rvector				m_exPosition;		///< 몸체의 예전 위치
	DWORD				m_dwExPositionTime;	///< 예전 위치 기억한 시각
	rvector				m_exPositionForWarp;		///< 몸체의 예전 위치
	DWORD				m_dwExPositionTimeForWarp;	///< 예전 위치 기억한 시각
	

	// 길찾기 관련
	list<rvector>		m_WayPointList;

	// Update tiemr
	void MakeNeglectUpdateTime();
	void AdjustWayPointWithBound(list<rvector>& wayPointList, ZNavigationMesh navMesh);
	bool EscapeFromStuckIn(list<rvector>& wayPointList);
	void ResetStuckInState();
	void ResetStuckInStateForWarp();
	void PushWayPointsToTask();


	// Think
	bool FindTarget();
	void ProcessAttack( float fDelta);
	bool GetUseableSkill( int *pnSkill, MUID *pTarget, rvector *pTargetPosition);
	void ProcessBuildPath( float fDelta);


public:
	// 몸체에서의 이벤트 발생 인터페이스
	void OnBody_AnimEnter( ZA_ANIM_STATE nAnimState);
	void OnBody_AnimExit( ZA_ANIM_STATE nAnimState);
	void OnBody_OnTaskFinished( ZTASK_ID nLastID);
	void OnBody_CollisionWall();


public:
	ZBrain();
	~ZBrain();

	void Init( ZActor* pBody);
	void Think( float fDelta);
	void OnDamaged();

	ZActor* GetBody()					{ return m_pBody; }
	ZObject* GetTarget();

};


#endif