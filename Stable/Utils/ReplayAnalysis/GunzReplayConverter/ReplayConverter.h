#pragma once

#include <Windows.h>
#include <stdio.h>
#include "MUID.h"
#include "MBaseGameType.h"
#include "MMatchGlobal.h"
#include "MMatchClan.h"
#include "MMatchItem.h"

//-----------------------------------------------------------------------------
// Gunz/ZReplay.h
struct REPLAY_STAGE_SETTING_NODE 
{
	MUID				uidStage;
	char				szMapName[MAPNAME_LENGTH];	// 맵이름
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

// 건즈 리플레이 파일
#define GUNZ_REC_FILE_ID		0x95b1308a

// version 4 : duel 모드의 현재 상태 저장이 추가되었습니다.
#define GUNZ_REC_FILE_VERSION	4
#define GUNZ_REC_FILE_EXT		"gzr"
//-----------------------------------------------------------------------------
// CSCommon/Include/MMatchObject.h
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

enum MMatchSex
{
	MMS_MALE = 0,		// 남자
	MMS_FEMALE = 1		// 여자
};
//-----------------------------------------------------------------------------
#pragma pack(push, old)
#pragma pack(1)
// CSCommon/Include/MMatchTransDataType.h
struct MTD_DuelQueueInfo
{
	MUID			m_uidChampion;
	MUID			m_uidChallenger;
	MUID			m_WaitQueue[14];				// 팀
	char			m_nQueueLength;
	char			m_nVictory;						// 연승수
	bool			m_bIsRoundEnd;					// 라운드 끝날때인가
};

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
};

/// 클라이언트가 알아야할 기타정보 : AdminHide 상태를 명시적으로 교환 & 녹화파일에 저장
struct MTD_ExtendInfo
{
	char			nTeam;
	unsigned char	nPlayerFlags;	// 플레이어 속성(운영자숨김등) - MTD_PlayerFlags 사용
	unsigned char	nReserved1;		// 여분
	unsigned char	nReserved2;
};

struct MTD_PeerListNode
{
	MUID			uidChar;
	DWORD			dwIP;
	unsigned int	nPort;
	MTD_CharInfo	CharInfo;
	MTD_ExtendInfo	ExtendInfo;
};

// 아이템 스폰 정보
struct MTD_WorldItem
{
	unsigned short	nUID;
	unsigned short	nItemID;
	unsigned short  nItemSubType;
	short			x;
	short			y;
	short			z;
	//float			x;
	//float			y;
	//float			z;
};
//-----------------------------------------------------------------------------
// ZPost.h
// 일정 시간간격으로 캐릭터들끼리 계속 주고받는 데이터
struct ZPACKEDBASICINFO {
	float	fTime;
	short	posx,posy,posz;
	short	velx,vely,velz;
	short	dirx,diry,dirz;
	BYTE	upperstate;
	BYTE	lowerstate;
	BYTE	selweapon;
};

struct ZPACKEDDASHINFO {
	short	posx, posy, posz;
	short	dirx, diry, dirz;
	BYTE	seltype;
};
#pragma pack(pop, old)
//-----------------------------------------------------------------------------
// ZCharacter.h
struct ZCharacterProperty
{
	char		szName[MATCHOBJECT_NAME_LENGTH];
	char		szClanName[CLAN_NAME_LENGTH];
	MMatchSex	nSex;
	int			nHair;
	int			nFace;
	int			nLevel;
	float		fMaxHP;		// *10 된값이다.
	float		fMaxAP;
	int			nMoveSpeed;
	int			nWeight;
	int			nMaxWeight;
	int			nSafeFall;
	ZCharacterProperty() :	nSex(MMS_MALE),
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
//-----------------------------------------------------------------------------
struct rvector
{
    float x;
    float y;
    float z;
};
//-----------------------------------------------------------------------------
class ZFile;
//-----------------------------------------------------------------------------
class Logger
{
public:
	Logger() : m_File(NULL) {}
	~Logger() { Close(); }

	void Init( const char* fileName ) { Close(); m_File = fopen( fileName, "w" ); }
	void Close() { if( m_File ) fclose( m_File ); }
	void Print( const char* text ) { if( m_File ) fprintf( m_File, text ); }

private:
	FILE *m_File;
};
void InitLog( const char* fileName );
void CloseLog();
void Log( const char* format, ... );
//-----------------------------------------------------------------------------
struct DIE_INFO
{
	float fTime;
	float x;
	float y;
	float z;
};
typedef vector<DIE_INFO> DieInfoList;
typedef DieInfoList::const_iterator DieInfoConstIter;
//-----------------------------------------------------------------------------
class Replay
{
public:
	Replay();
	~Replay();

	bool Load(const string& filename );
	const string& GetMapName() { return m_MapName; }
	const DieInfoList& GetDieInfoList() { return m_DieInfoList; }

private:
	bool LoadHeader( ZFile* file );
	bool LoadStageSetting( ZFile* file );
	bool LoadStageSettingEtc( ZFile* file );
	bool LoadCharInfo( ZFile* file );
	bool LoadCommandStream( ZFile* file );

private:
	typedef map<MUID, rvector> RecentPosition;
	typedef RecentPosition::const_iterator RecentPositionConstIter;

	unsigned int				m_nVersion;
	float						m_fGameTime;
	REPLAY_STAGE_SETTING_NODE	m_StageSetting;
	MTD_DuelQueueInfo			m_DuelQueueInfo;

	string						m_MapName;

	RecentPosition				m_RecentPosition;

	DieInfoList					m_DieInfoList;
};
//-----------------------------------------------------------------------------
class ReplayConverter
{
public:
	ReplayConverter();
	~ReplayConverter();

	bool Add( const string& replayFileName );
	bool SaveCSV();

private:
	typedef pair<string, Replay*> ReplayListPair;
	typedef multimap<string, Replay*> ReplayList;
	typedef ReplayList::iterator ReplayListIter;
	typedef ReplayList::const_iterator ReplayListConstIter;

	ReplayList m_ReplayList;
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------