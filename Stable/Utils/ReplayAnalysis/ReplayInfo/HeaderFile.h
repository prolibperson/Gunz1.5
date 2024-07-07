#ifndef _HEADERFILE_H
#define _HEADERFILE_H

#define MATCHOBJECT_NAME_LENGTH			32	// 캐릭터 이름 길이 - 역시 리플레이때문에 DB에 맞출수 없음. 새로 추가하는 쪽으로 수정. - by SungE. 2007-03-20
#define CLAN_NAME_LENGTH				16	// 클랜 이름 최대 길이 - 이게 변경되면 프로토콜 버전도 변경되어야 함.

/// 게임 타입
enum MMATCH_GAMETYPE {
	MMATCH_GAMETYPE_DEATHMATCH_SOLO		=0,			///< 개인 데쓰매치
	MMATCH_GAMETYPE_DEATHMATCH_TEAM		=1,			///< 팀 데쓰매치
	MMATCH_GAMETYPE_GLADIATOR_SOLO		=2,			///< 개인 글래디에이터
	MMATCH_GAMETYPE_GLADIATOR_TEAM		=3,			///< 팀 글래디에이터
	MMATCH_GAMETYPE_ASSASSINATE			=4,			///< 보스전(암살전)
	MMATCH_GAMETYPE_TRAINING			=5,			///< 연습

	MMATCH_GAMETYPE_SURVIVAL			=6,			///< 서바이벌
	MMATCH_GAMETYPE_QUEST				=7,			///< 퀘스트

	MMATCH_GAMETYPE_BERSERKER			=8,			///< 데쓰매치 버서커
	MMATCH_GAMETYPE_DEATHMATCH_TEAM2	=9,			///< 팀데쓰매치 익스트림
	MMATCH_GAMETYPE_DUEL				=10,		///< 듀얼 매치
	MMATCH_GAMETYPE_DUELTOURNAMENT		=11,		///< 듀얼 토너먼트
/*
#ifndef _CLASSIC
	MMATCH_GAMETYPE_CLASSIC_SOLO,
	MMATCH_GAMETYPE_CLASSIC_TEAM,
#endif
*/
	MMATCH_GAMETYPE_MAX,
};

// 클랜 등급
enum MMatchClanGrade
{
	MCG_NONE		= 0,		// 클랜원이 아님
	MCG_MASTER		= 1,		// 클랜 마스터
	MCG_ADMIN		= 2,		// 클랜 운영자

	MCG_MEMBER		= 9,		// 일반 클랜원
	MCG_END
};

/// 장비부위
enum MMatchCharItemParts
{
	MMCIP_HEAD		= 0,
	MMCIP_CHEST		= 1,
	MMCIP_HANDS  	= 2,
	MMCIP_LEGS		= 3,
	MMCIP_FEET		= 4,
	MMCIP_FINGERL	= 5,
	MMCIP_FINGERR	= 6,
	MMCIP_MELEE		= 7,
	MMCIP_PRIMARY	= 8,
	MMCIP_SECONDARY	= 9,
	MMCIP_CUSTOM1	= 10,	
	MMCIP_CUSTOM2	= 11,
	MMCIP_AVATAR	= 12,
	MMCIP_COMMUNITY1	= 13,
	MMCIP_COMMUNITY2	= 14,
	MMCIP_LONGBUFF1	= 15,
	MMCIP_LONGBUFF2	= 16,
	MMCIP_END
};

// 등급 - 이것은 디비의 UserGrade테이블과 싱크가 맞아야 한다.
enum MMatchUserGradeID
{
	MMUG_FREE			= 0,	// 무료 계정
	MMUG_REGULAR		= 1,	// 정액 유저
	MMUG_STAR			= 2,	// 스타유저(게임짱)

	MMUG_CRIMINAL		= 100,	// 전과자
	MMUG_WARNING_1		= 101,	// 1차 경고
	MMUG_WARNING_2		= 102,	// 2차 경고
	MMUG_WARNING_3		= 103,	// 3차 경고
	MMUG_CHAT_LIMITED	= 104,  // 채팅 금지
	MMUG_PENALTY		= 105,	// 기간 정지

	MMUG_EVENTMASTER	= 252,	// 이벤트 진행자
	MMUG_BLOCKED		= 253,	// 사용 정지
	MMUG_DEVELOPER		= 254,	// 개발자
	MMUG_ADMIN			= 255	// 관리자
};

enum MMatchTeam
{
	MMT_ALL			= 0,
	MMT_SPECTATOR	= 1,
	MMT_RED			= 2,
	MMT_BLUE		= 3,
	MMT_END
};

enum MMatchSex
{
	MMS_MALE = 0,		// 남자
	MMS_FEMALE = 1		// 여자
};

struct MUID{
	unsigned long int	High;	///< High 4 Byte
	unsigned long int	Low;	///< High 4 Byte
};

struct rvector
{
	float x;
	float y;
	float z;
};




////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// 건즈 클라이언트에서 리플레이 파일 저장될때 클래스가 변한다면 
// 아래 클래스도 같이 변경되어야 한다.
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

struct REPLAY_STAGE_SETTING_NODE 
{
	MUID				uidStage;
	char				szMapName[MATCHOBJECT_NAME_LENGTH];	// 맵이름
	char				nMapIndex;					// 맵인덱스
	MMATCH_GAMETYPE		nGameType;					// 게임타입
	int					nRoundMax;					// 라운드
	int					nLimitTime;					// 제한시간(1 - 1분)
	int					nLimitLevel;				// 제한레벨
	int					nMaxPlayers;				// 최대인원
	bool				bTeamKillEnabled;			// 팀킬여부
	bool				bTeamWinThePoint;			// 선승제 여부
	bool				bForcedEntryEnabled;		// 게임중 참가 여부
};


#pragma pack(push, old)
#pragma pack(1)
// 패킷을 보내기 위한 구조체이기 때문에 1Byte씩 자르는 패킹을 해준다.
struct MTD_CharInfo
{
	// 캐릭터 정보
	char				szName[MATCHOBJECT_NAME_LENGTH];
	char				szClanName[CLAN_NAME_LENGTH];
	MMatchClanGrade		nClanGrade;
	unsigned short		nClanContPoint;
	char				nCharNum;
	unsigned short		nLevel;
	char				nSex;
	char				nHair;
	char				nFace;
	unsigned long int	nXP;
	int					nBP;
	float				fBonusRate;
	unsigned short		nPrize;
	unsigned short		nHP;
	unsigned short		nAP;
	unsigned short		nMaxWeight;
	unsigned short		nSafeFalls;
	unsigned short		nFR;
	unsigned short		nCR;
	unsigned short		nER;
	unsigned short		nWR;

	// 아이템 정보
	unsigned long int	nEquipedItemDesc[MMCIP_END];

	// account 의 정보
	MMatchUserGradeID	nUGradeID;

	// ClanCLID
	unsigned int		nClanCLID;

	// 지난주 듀얼토너먼트 등급
	int					nDTLastWeekGrade;

	// 아이템 정보 추가
	MUID				uidEquipedItem[MMCIP_END];
	unsigned long int	nEquipedItemCount[MMCIP_END];
};
#pragma pack(pop, old)


/// 캐릭터 속성 - 이 값은 변하지 않는다.
struct ZCharacterProperty_Old
{
	char		szName[MATCHOBJECT_NAME_LENGTH];
	char		szClanName[CLAN_NAME_LENGTH];
	MMatchSex	nSex;
	int			nHair;
	int			nFace;
	int			nLevel;
	float		fMaxHP;
	float		fMaxAP;
	int			nMoveSpeed;
	int			nWeight;
	int			nMaxWeight;
	int			nSafeFall;
	ZCharacterProperty_Old() :	nSex(MMS_MALE),
		nHair(0),
		nFace(0),
		nLevel(1),
		fMaxHP(1000.f), 
		fMaxAP(1000.f), 
		nMoveSpeed(100), 
		nWeight(0), 
		nMaxWeight(100), 
		nSafeFall(100)
	{ 
		szName[0] = 0;
		szClanName[0] = 0;
	}
	void SetName(const char* name) { strcpy(szName, name); }
	void SetClanName(const char* name) { strcpy(szClanName, name); }
};

/// 캐릭터 상태값
struct ZCharacterStatus
{
	int			nLife;
	int			nKills;
	int			nDeaths;
	int			nLoadingPercent;	// 처음 방에 들어올때 로딩이 다 되었는지의 퍼센트 100이 되면 로딩이 다된것
	int			nCombo;
	int			nMaxCombo;
	int			nAllKill;
	int			nExcellent;
	int			nFantastic;
	int			nHeadShot;
	int			nUnbelievable;
	int			nExp;

	ZCharacterStatus() :	
	nLife(10),
		nKills(0),
		nDeaths(0),
		nLoadingPercent(0),
		nCombo(0),
		nMaxCombo(0),
		nAllKill(0),
		nExcellent(0),
		nFantastic(0),
		nHeadShot(0),
		nUnbelievable(0),
		nExp(0)
	{  }

	void AddKills(int nAddedKills = 1) { nKills += nAddedKills; }
	void AddDeaths(int nAddedDeaths = 1) { nDeaths += nAddedDeaths;  }
	void AddExp(int _nExp=1) { nExp += _nExp; }
};

#endif _HEADERFILE_H