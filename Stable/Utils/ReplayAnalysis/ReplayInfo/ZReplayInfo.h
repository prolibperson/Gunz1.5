#ifndef _ZREPLAYINFO_H
#define _ZREPLAYINFO_H

#include "ZFile.h"
#include "HeaderFile.h"



// 건즈 리플레이 파일
#define GUNZ_REC_FILE_ID		0x95b1308a

// version 4 : duel 모드의 현재 상태 저장이 추가되었습니다.
// version 5 : duel_tournament용 정보가 추가되었습니다. (MTD_CharInfo에 지난주 랭킹등급 정보 추가, 듀얼토너먼트용 게임 상태 저장)
// version 6 : 장착 아이템에 갯수정보가 추가되었습니다. (소모성 아이템용)
#define GUNZ_REC_FILE_VERSION	6
#define GUNZ_REC_FILE_EXT		"gzr"
#define MAXPLAYERCOUNT			16

class ZReplayInfo
{
private:
public:
	ZReplayInfo() {}
	~ZReplayInfo() {}

};

class ZReplayLoader
{
public:
	static unsigned int				m_nVersion;
private:
	REPLAY_STAGE_SETTING_NODE		m_StageSetting;
	int								m_nCharacterCount;
	MTD_CharInfo					m_CharInfo[MAXPLAYERCOUNT];

	bool LoadHeader(ZFile* file);
	bool LoadStageSetting(ZFile* file);
	bool LoadStageSettingEtc(ZFile* file);
	bool LoadCharInfo(ZFile* file);
	bool ZCharacter_Load(ZFile *file,int nVersion);
	bool ZItem_Load(ZFile *file, int nReplayVersion);
	char* GetGameType(MMATCH_GAMETYPE gameType);

public:
	ZReplayLoader();
	~ZReplayLoader() {}
	bool Load(const char* filename);
	bool Save(const char* filename);
};

typedef struct _DTPlayerInfo
{
	char m_szCharName[MATCHOBJECT_NAME_LENGTH];
	MUID uidPlayer;
	int m_nTP;
} DTPlayerInfo;

//////////////////////////////////////////////////////
// 기존 버전의 캐릭터 정보 구조체
//////////////////////////////////////////////////////

#pragma pack(push, old)
#pragma pack(1)

struct MTD_CharInfo_v0	// ver0 ver1
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
	unsigned long int	nEquipedItemDesc[12];		// MMatchCharItemParts_v0::MMCIP_END0

	// account 의 정보
	MMatchUserGradeID	nUGradeID;
};

struct MTD_CharInfo_v2	// ver2 ~ 4
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
	unsigned long int	nEquipedItemDesc[12];		// MMatchCharItemParts_v0::MMCIP_END0

	// account 의 정보
	MMatchUserGradeID	nUGradeID;

	// ClanCLID
	unsigned int		nClanCLID;
};

struct MTD_CharInfo_v5	// ver5
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
	unsigned long int	nEquipedItemDesc[12];		// MMatchCharItemParts_v0::MMCIP_END0

	// account 의 정보
	MMatchUserGradeID	nUGradeID;

	// ClanCLID
	unsigned int		nClanCLID;

	// 지난주 듀얼토너먼트 등급
	int					nDTLastWeekGrade;	
};

struct MTD_DuelQueueInfo
{
	MUID			m_uidChampion;
	MUID			m_uidChallenger;
	MUID			m_WaitQueue[14];				// 팀
	char			m_nQueueLength;
	char			m_nVictory;						// 연승수
	bool			m_bIsRoundEnd;					// 라운드 끝날때인가
};

struct MTD_DuelTournamentGameInfo
{
	MUID			uidPlayer1;					// 진행할 게임의 참가할 Player1
	MUID			uidPlayer2;					// 진행할 게임의 참가할 Player1
	int				nMatchType;					// 진행할 게임의 MatchType(
	int				nMatchNumber;				// 진행할 게임의 MatchNumber
	int				nRoundCount;				// 진행할 게임의 라운드수
	bool			bIsRoundEnd;				// 라운드가 종료되었는가에 대한 Flag(플레이어 이탈자 때문에..)
	char			nWaitPlayerListLength;		// 대기자 리스트의 Length
	byte			dummy[2];					// 4바이트씩 맞추기 위한 더미
	MUID			WaitPlayerList[8];			// 대기자들의 MUID
};

#pragma pack(pop, old)



#endif