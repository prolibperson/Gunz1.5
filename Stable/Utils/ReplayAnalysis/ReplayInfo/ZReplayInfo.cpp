#include <comutil.h>
#include "ZFile.h"
#include "ZReplayInfo.h"
#include <vector>
using namespace std;

unsigned int ZReplayLoader::m_nVersion=0;

ZReplayLoader::ZReplayLoader()
{
	memset(&m_StageSetting, 0, sizeof(REPLAY_STAGE_SETTING_NODE));
}

bool ZReplayLoader::Load(const char* filename)
{
	ZFile* file = zfopen(filename);

	if(!file) return false;
	
	if (!LoadHeader(file)) return false;
	if (!LoadStageSetting(file)) return false;

	if(m_nVersion>=4)
		if (!LoadStageSettingEtc(file)) return false;

	if (!LoadCharInfo(file)) return false;

	zfclose(file);
	return true;
}

bool ZReplayLoader::LoadHeader(ZFile* file)
{
	unsigned int version = 0;
	unsigned int header;
	int nRead;

	nRead = zfread(&header, sizeof(header), 1, file);
	if(nRead==0 || header!=GUNZ_REC_FILE_ID) return false;

	nRead = zfread(&version, sizeof(version), 1, file);
	if ( ( nRead == 0) || ( version > GUNZ_REC_FILE_VERSION))
		return false;

	m_nVersion = version;

	return true;
}

bool ZReplayLoader::LoadStageSetting(ZFile* file)
{
	// stage setting
	int nRead = zfread(&m_StageSetting, sizeof(REPLAY_STAGE_SETTING_NODE), 1, file);
	if(nRead==0) return false;

	return true;
}

bool ZReplayLoader::LoadStageSettingEtc(ZFile* file)
{
	// 게임룰 별 추가 세팅값 로드
	if(m_StageSetting.nGameType==MMATCH_GAMETYPE_DUEL)
	{
		//ZRuleDuel* pDuel = (ZRuleDuel*)ZGetGameInterface()->GetGame()->GetMatch()->GetRule();
		MTD_DuelQueueInfo temp;
		int nRead = zfread(&temp,sizeof(MTD_DuelQueueInfo),1,file);
		if(nRead==0) return false;
	}
	if(m_StageSetting.nGameType==MMATCH_GAMETYPE_DUELTOURNAMENT)
	{
		int tournamentType;
		int nRead = zfread(&tournamentType,sizeof(int),1,file);
		if(nRead==0) return false;

		int nCount = 0;
		nRead = zfread(&nCount,sizeof(int),1,file);
		if(nRead==0) return false;

		vector<DTPlayerInfo> vecPlayerInfo;
		DTPlayerInfo temp_DTPlayerInfo;
		for (int i=0; i<nCount; ++i)
		{
			nRead = zfread(&temp_DTPlayerInfo,sizeof(DTPlayerInfo),1,file);
			if(nRead==0) return false;
			vecPlayerInfo.push_back(temp_DTPlayerInfo);
		}

		//ZGetGameInterface()->SetDuelTournamentCharacterList((MDUELTOURNAMENTTYPE)tournamentType, vecPlayerInfo);
		// 원래 게임에서는 이미 캐릭터목록이 세팅된 상태로 룰이 생성되는데, 리플레이는 순서가 반대라서 캐릭터 목록을 가져가도록 직접 지시해야한다..
		//ZRuleDuelTournament* pRule = (ZRuleDuelTournament*)ZGetGameInterface()->GetGame()->GetMatch()->GetRule();
		//pRule->InitCharacterList();

		// 녹화시작 당시의 경기는 m_DTGameInfo 커맨드를 이미 받아버린 상태였기때문에 따로 로드해서 넣어줘야 에너지바가 제대로 출력된다
		MTD_DuelTournamentGameInfo temp_DuelTournamentGameInfo;
		nRead = zfread(&temp_DuelTournamentGameInfo,sizeof(MTD_DuelTournamentGameInfo),1,file);
		if(nRead==0) return false;
	}
	return true;
}

void ConvertCharInfo_v0_to_v2(MTD_CharInfo_v2* v2, MTD_CharInfo_v0* v0)
{
	// 변수 하나 추가된 것이므로 그냥 덮어씌우고 추가된 변수 초기화
	memcpy(v2, v0, sizeof(MTD_CharInfo_v0));
	v2->nClanCLID = 0;
};

void ConvertCharInfo_v2_to_v5(MTD_CharInfo_v5* v5, MTD_CharInfo_v2* v2)
{
	memcpy(v5, v2, sizeof(MTD_CharInfo_v2));
	v5->nDTLastWeekGrade = 0;
}

void ConvertCharInfo_v5_to_curr(MTD_CharInfo* curr, MTD_CharInfo_v5* v5)
{
	// 일단 덮어씌우고
	memcpy(curr, v5, sizeof(MTD_CharInfo_v5));
	// nEquipedItemDesc[MMCIP_END] 에서 MMCIP_END가 변경됐으므로 직접 복사
	memset(curr->nEquipedItemDesc, 0, sizeof(curr->nEquipedItemDesc));
	for (int i=0; i<MMCIP_CUSTOM2+1; ++i)
		curr->nEquipedItemDesc[i] = v5->nEquipedItemDesc[i];
	// 그 뒤의 멤버변수 다시 복사
	curr->nUGradeID		= v5->nUGradeID;
	curr->nClanCLID		= v5->nClanCLID;
	curr->nDTLastWeekGrade	= v5->nDTLastWeekGrade;

	// 새로 추가된 변수들은 그냥 기본값으로 세팅
	memset(curr->uidEquipedItem, 0, sizeof(curr->uidEquipedItem));

	for (int i=0; i<MMCIP_END; ++i)
		curr->nEquipedItemCount[i] = 1;
}

void ConvertCharInfo(MTD_CharInfo* currCharInfo, void* oldCharInfo, int nVerOld)
{
	MTD_CharInfo_v2 v2;
	MTD_CharInfo_v5 v5;

	switch (nVerOld)
	{
	case 0: case 1:
		ConvertCharInfo_v0_to_v2(&v2, (MTD_CharInfo_v0*)oldCharInfo);
		oldCharInfo = &v2;

	case 2: case 3: case 4:
		ConvertCharInfo_v2_to_v5(&v5, (MTD_CharInfo_v2*)oldCharInfo);
		oldCharInfo = &v5;

	case 5:
		ConvertCharInfo_v5_to_curr(currCharInfo, (MTD_CharInfo_v5*)oldCharInfo);
	}
	// case에 break가 없는 것은 의도된 것임

	if (nVerOld == GUNZ_REC_FILE_VERSION)
		memcpy(currCharInfo, oldCharInfo, sizeof(MTD_CharInfo));
}

bool ZReplayLoader::LoadCharInfo(ZFile* file)
{
	int nRead;

	// character info
	int nCharacterCount;
	zfread(&nCharacterCount, sizeof(nCharacterCount), 1, file);
	m_nCharacterCount = nCharacterCount;

	for(int i = 0; i < nCharacterCount; i++)
	{
		bool bHero;
		nRead = zfread(&bHero, sizeof(bHero), 1, file);
		if(nRead != 1) return false;

		MTD_CharInfo info;

		switch (m_nVersion)
		{
		case 0: case 1:
			{
				MTD_CharInfo_v0 old;
				nRead = zfread(&old, sizeof(old), 1, file);
				if(nRead != 1) return false;
				ConvertCharInfo(&info, &old, m_nVersion);
			}
			break;
		case 2: case 3: case 4:
			{
				MTD_CharInfo_v2 old;
				nRead = zfread(&old, sizeof(old), 1, file);
				if(nRead != 1) return false;
				ConvertCharInfo(&info, &old, m_nVersion);
			}
			break;
		case 5:
			{
				MTD_CharInfo_v5 old;
				nRead = zfread(&old, sizeof(old), 1, file);
				if(nRead != 1) return false;
				ConvertCharInfo(&info, &old, m_nVersion);
			}
			break;
		case GUNZ_REC_FILE_VERSION:
			{
				nRead = zfread(&info, sizeof(info), 1, file);
				if(nRead != 1) return false;
			}
		}

		memcpy(&m_CharInfo[i], &info, sizeof(MTD_CharInfo));

		//ZCharacter* pChar=NULL;
		//if(bHero)
		//{
		//	ZGetGame()->m_pMyCharacter = new ZMyCharacter;			
		//	ZGetGame()->CreateMyCharacter(&info/*, NULL*/);			///< TodoH(상) - 리플레이 관련..
		//	pChar=ZGetGame()->m_pMyCharacter;
		//	pChar->Load(file,m_nVersion);
		//}else
		//{
		//	pChar=new ZNetCharacter;
		//	pChar->Load(file,m_nVersion);
		//	pChar->Create(&info/*, NULL*/);							///< TodoH(상) - 리플레이 관련..
		//}
		//ZGetCharacterManager()->Add(pChar);

		//pChar->SetVisible(true);
		ZCharacter_Load(file, m_nVersion);
	}

	return true;
}

bool ZReplayLoader::ZCharacter_Load(ZFile *file,int nVersion)
{
	MUID m_UID;
	ZCharacterStatus m_Status;
	rvector m_Direction;

	size_t n;

	n=zfread(&m_UID,sizeof(m_UID),1,file);
	if(n!=1) return false;

	ZCharacterProperty_Old oldProperty;
	n=zfread(&oldProperty,sizeof(oldProperty),1,file);
	if(n!=1) return false;
	//m_Property.CopyFromOldStruct(oldProperty);

	//InitHPAP();

	float fHP;
	n=zfread(&fHP,sizeof(float),1,file);
	if(n!=1) return false;
	//m_pModule_HPAP->SetHP(fHP);

	float fAP;
	n=zfread(&fAP,sizeof(float),1,file);
	if(n!=1) return false;
	//m_pModule_HPAP->SetAP(fAP);

	n=zfread(&m_Status,sizeof(ZCharacterStatus),1,file);
	if(n!=1) return false;
	//m_Status.MakeCrc();

	//if(!m_Items.Load(file, nVersion)) return false;
	if (!ZItem_Load(file, nVersion)) return false;

	/*
	MMatchCharItemParts nSelectedWeapon=m_Items.GetSelectedWeaponParts();
	m_Items.ResetItems();

	ChangeWeapon(nSelectedWeapon);
	*/

	rvector pos;
	n=zfread(&pos,sizeof(pos),1,file);
	if(n!=1) return false;
	//SetPosition(pos);

	n=zfread(&m_Direction,sizeof(m_Direction),1,file);
	if(n!=1) return false;

	MMatchTeam teamID;
	n=zfread(&teamID,sizeof(teamID),1,file);
	if(n!=1) return false;
	//m_nTeamID.Set_CheckCrc(teamID);

	//jintriple3 메모리 프록시...비트 패킹..
	//ZCharaterStatusBitPacking& uStatus = m_dwStatusBitPackingValue.Ref();

	bool bDie;
	n=zfread(&bDie,sizeof(bDie),1,file);
	//uStatus.m_bDie = bDie;
	if(n!=1) return false;

	if(nVersion>=2) {
		bool bAdminHide;
		n=zfread(&bAdminHide,sizeof(bAdminHide),1,file);
		//uStatus.m_bAdminHide = bAdminHide;
		if(n!=1) return false;
	}

	return true;
}

bool ZReplayLoader::ZItem_Load(ZFile *file, int nReplayVersion)
{
	enum MMatchCharItemParts_v0
	{
		MMCIP_HEAD_V0		= 0,
		MMCIP_CHEST_V0		= 1,
		MMCIP_HANDS_V0		= 2,
		MMCIP_LEGS_V0		= 3,
		MMCIP_FEET_V0		= 4,
		MMCIP_FINGERL_V0	= 5,
		MMCIP_FINGERR_V0	= 6,
		MMCIP_MELEE_V0		= 7,
		MMCIP_PRIMARY_V0	= 8,
		MMCIP_SECONDARY_V0	= 9,
		MMCIP_CUSTOM1_V0	= 10,
		MMCIP_CUSTOM2_V0	= 11,
		MMCIP_END_V0
	};

	struct Converter
	{
		static int v0_to_v6(int v0)
		{
			switch (v0)
			{
			case MMCIP_HEAD_V0:			return MMCIP_HEAD;
			case MMCIP_CHEST_V0:		return MMCIP_CHEST;
			case MMCIP_HANDS_V0:		return MMCIP_HANDS;
			case MMCIP_LEGS_V0:			return MMCIP_LEGS;
			case MMCIP_FEET_V0:			return MMCIP_FEET;
			case MMCIP_FINGERL_V0:		return MMCIP_FINGERL;
			case MMCIP_FINGERR_V0:		return MMCIP_FINGERR;
			case MMCIP_MELEE_V0:		return MMCIP_MELEE;
			case MMCIP_PRIMARY_V0:		return MMCIP_PRIMARY;
			case MMCIP_SECONDARY_V0:	return MMCIP_SECONDARY;
			case MMCIP_CUSTOM1_V0:		return MMCIP_CUSTOM1;
			case MMCIP_CUSTOM2_V0:		return MMCIP_CUSTOM2;
			}
			return MMCIP_END;
		}

		static int convert(int old, int nReplayVersion)
		{
			int curr = old;
			switch (nReplayVersion)
			{
			case 0:case 1:case 2:case 3:case 4:case 5:
				curr = v0_to_v6(curr);
			}
			return curr;
		}
	};

	int numCharItemParts = 0;
	if (0 <= nReplayVersion && nReplayVersion <= 5)
		numCharItemParts = MMCIP_END_V0;
	else
		numCharItemParts = MMCIP_END;

	size_t n;
	for(int i=0;i<numCharItemParts;i++)
	{
		int idxParts = Converter::convert(i, nReplayVersion);
		//ZItem *pItem=GetItem(MMatchCharItemParts(idxParts));
		int nBullet;
		n=zfread(&nBullet,sizeof(nBullet),1,file);
		//pItem->SetBulletSpare(nBullet);
		if(n!=1) return false;

		int nBulletCurrMagazine;
		n=zfread(&nBulletCurrMagazine,sizeof(nBulletCurrMagazine),1,file);
		//pItem->SetBulletCurrMagazine(nBulletCurrMagazine);
		if(n!=1) return false;
	}

	return true;
}

bool ZReplayLoader::Save(const char* filename)
{
	char replayLogfile[128];
	sprintf(replayLogfile, "%s.txt", filename);

	FILE *pFile;
	pFile=fopen(replayLogfile,"w");
	if( pFile==NULL ) return false;

	char buf[1024] = {0, };
	sprintf(buf, "%s- Gunz Replay Version.[%d] \n", buf, m_nVersion);
	sprintf(buf, "%s- Game type [%s] \n", buf, GetGameType(m_StageSetting.nGameType));
	sprintf(buf, "%s- Game Map  [%s] \n\n", buf, m_StageSetting.szMapName);
	for(int i=0; i < m_nCharacterCount; i++)
	{
		sprintf(buf, "%s  [%s] Player \n", buf, m_CharInfo[i].szName);
		sprintf(buf, "%s    Clan  [%s] \n", buf, m_CharInfo[i].szClanName);
		sprintf(buf, "%s    Sex   [%s] \n", buf, (m_CharInfo[i].nSex)? "female":"male");
		sprintf(buf, "%s    Level [%d] \n", buf, m_CharInfo[i].nLevel);
	}

	fputs(buf, pFile);
	fclose(pFile);

	return true;
}

char* ZReplayLoader::GetGameType(MMATCH_GAMETYPE gameType)
{
	switch(gameType)
	{
	case MMATCH_GAMETYPE_DEATHMATCH_SOLO:	return "DeathMatch_Solo";		///< 개인 데쓰매치
	case MMATCH_GAMETYPE_DEATHMATCH_TEAM:	return "DeathMatch_Team";		///< 팀 데쓰매치
	case MMATCH_GAMETYPE_GLADIATOR_SOLO:	return "Gladiator_Solo";		///< 개인 글래디에이터
	case MMATCH_GAMETYPE_GLADIATOR_TEAM:	return "Gladiator_Team";		///< 팀 글래디에이터
	case MMATCH_GAMETYPE_ASSASSINATE:		return "Assassinate";			///< 보스전(암살전)
	case MMATCH_GAMETYPE_TRAINING:			return "Training";				///< 연습

	case MMATCH_GAMETYPE_SURVIVAL:			return "Survival";				///< 서바이벌
	case MMATCH_GAMETYPE_QUEST:				return "Quest";					///< 퀘스트

	case MMATCH_GAMETYPE_BERSERKER:			return "Berserker";				///< 데쓰매치 버서커
	case MMATCH_GAMETYPE_DEATHMATCH_TEAM2:	return "DeathMatch_Extreme";	///< 팀데쓰매치 익스트림
	case MMATCH_GAMETYPE_DUEL:				return "Duel";					///< 듀얼 매치
	case MMATCH_GAMETYPE_DUELTOURNAMENT:	return "Duel_Tournament";		///< 듀얼 토너먼트
	default:								return "Don't know";
	};
	return NULL;
}